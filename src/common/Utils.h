/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
#pragma once

#include "gfTypes.h"
#include <memory>
#include <locale.h>
#include <algorithm>
#include <assert.h>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <thread>
#include <sstream>
#include <atomic>
#include <vector>
#include <chrono>
#include "LogUtils.h"
using namespace std;

namespace gf {

#if defined(DEBUG) || defined(_DEBUG)

#define ASSERT_VALID_PTR(p) assert(nullptr != (p))
#define ASSERT_IF(c) assert((c))

#else // DEBUG

#define ASSERT_VALID_PTR(p)
#define ASSERT_IF(c)

#endif // DEBUG

	//#define __GF_INLINE__ inline
#define __GF_INLINE__ static

	struct utils
	{
		__GF_INLINE__ wstring towstring(const string& str)
		{
			if (str.length() == 0)
				return wstring();
			size_t convertedChars = 0;
			GF_SIZE len = str.size() * 2 + 1;
			//string curLocale = setlocale(LC_CTYPE, "");
			unique_ptr<wchar_t[]> up(new wchar_t[len]);
			wchar_t* p = up.get();
			p[len - 1] = L'\0';
#ifdef WIN32
			mbstowcs_s(&convertedChars, p, len, str.c_str(), _TRUNCATE);
#else // WIN32
			mbstowcs(p, str.c_str(), str.length()*2);
#endif
			wstring wstr(p);
			//setlocale(LC_CTYPE, curLocale.c_str());
			return wstr;
		}

		__GF_INLINE__ wstring towstring(const wstring& wstr)
		{
			return wstr;
		}

		__GF_INLINE__ string tostring(const wstring& wstr)
		{
			if (wstr.length() == 0)
				return string();
			size_t convertedChars = 0;
			GF_SIZE len = wstr.size() * 4;
			//string curLocale = setlocale(LC_CTYPE, "");
			unique_ptr<char[]> up(new char[len + 1]);
			char* p = up.get();
			p[len] = '\0';
#ifdef WIN32
			wcstombs_s(&convertedChars, p, len, wstr.c_str(), _TRUNCATE);
#else // WIN32
			wcstombs(p, wstr.c_str(), len);
#endif
			string str(p);
			//setlocale(LC_CTYPE, curLocale.c_str());
			return str;
		}

		__GF_INLINE__ string tostring(const string& str)
		{
			return str;
		}

		__GF_INLINE__ tstring totstring(const string& str)
		{
#if defined(UNICODE)
			return towstring(str);
#else
			return tostring(str);
#endif
		}

		__GF_INLINE__ string deviceAddressToString(const GF_UINT8 addr[], GF_SIZE len)
		{
			if (nullptr == addr)
				return string();
			GF_SIZE alloclen = len*(3) + 1;
			unique_ptr<char[]> up(new char[alloclen]);
			char* str = up.get();
			for (GF_SIZE i = 0; i < len; ++i)
			{
#ifdef WIN32
				sprintf_s(str + (3 * i), alloclen - (3 * i), "%2.2X:", addr[i]);
#else
				snprintf(str+ (3 * i), alloclen - (3 * i), "%2.2X:", addr[i]);
#endif
			}
			str[alloclen - 2] = '\0';
			return string(str);
		}

		__GF_INLINE__ tstring deviceAddressToStringT(const GF_UINT8 addr[], GF_SIZE len)
		{
			return totstring(deviceAddressToString(addr, len));
		}

		__GF_INLINE__ string threadIdToString(thread::id id)
		{
			std::ostringstream stm;
			stm << id;
			return stm.str();
		}
	};

	template <class _T> struct DevComp {
		bool operator ()(const _T& left, const _T& right) const {
			return left->operator<(*right);
		}
	};

	template <class _T> struct ConnectedDevComp {
		bool operator ()(const _T& left, const _T& right) const {
			return left->getHandle() < right->getHandle();
		}
	};

	template <class _T> struct WeakPtrComp {
		bool operator ()(const gfwPtr<_T>& left, const gfwPtr<_T>& right) const {
			auto sl = left.lock(); auto sr = right.lock();
			return sl < sr;
		}
	};

	template <class _T> void cleanInvalidWeakP(_T& container) {
		for (auto itor = container.begin(); itor != container.end();) {
			if (nullptr == itor->lock()) {
				container.erase(itor++);
			}
			else {
				++itor;
			}
		}
	}

	template <class _Type>
	class BQueue // block queue
	{
	public:
		BQueue() {}
		~BQueue() {}

		void push(_Type i)
		{
			{
				lock_guard<mutex> lock(mMutex);
				mQ.push_back(i);
			}
			mCondition.notify_all();
		}

		_Type pop()
		{
			unique_lock<mutex> lock(mMutex);
			auto& q = mQ;
			mCondition.wait(lock, [&q](){ return !q.empty(); });
			ASSERT_IF(!q.empty());
			_Type r = q.front();
			q.pop_front();
			return r;
		}

		template< class Clock, class Duration>
		bool pop_until(_Type& r, const std::chrono::time_point<Clock, Duration>& tp)
		{
			unique_lock<mutex> lock(mMutex);
			auto& q = mQ;
			bool ret = mCondition.wait_until(lock, tp, [&q](){ return !q.empty(); });
			if (ret)
			{
				r = q.front();
				q.pop_front();
				return true;
			}
			else
			{
				return false;
			}
		}

		void clear()
		{
			mQ.clear();
		}

	private:
		deque<_Type> mQ;
		mutex mMutex;
		condition_variable mCondition;
	};

	// a timer
	template <class TimerType> class TimerManager;

	template <class TimePointType, class TimeDurationType = typename TimePointType::duration>
	class GfTimer
	{
		using SelfType = GfTimer < TimePointType > ;
	public:
		using TimePoint = TimePointType;
		using TimeDuration = TimeDurationType;
		using TimerFunc = function < void() > ;

	public:
		GfTimer()
			: mRunning(false)
			, mMgr(TimerManager<SelfType>::getInstance())
		{
		}
		~GfTimer()
		{
			stop();
		}
		bool start(TimeDuration duration, TimerFunc task)
		{
			lock_guard<mutex> lock(mMutex);
			if (mRunning)
			{
				GF_LOGD("Error: Timer is running.");
				return false;
			}
			mTimePoint = chrono::system_clock::now() + duration;
			mTask = task;
			mRunning = mMgr.addTimer(*this);
			return mRunning;
		}
		bool stop()
		{
			lock_guard<mutex> lock(mMutex);
			if (!mRunning)
				return true;
			if (mMgr.removeTimer(*this))
			{
				mRunning = false;
			}
			return !mRunning;
		}

	private:
		void trigger()
		{
			mRunning = false;
			mTask();
		}

		mutex mMutex;
		atomic<bool> mRunning;
		TimePoint mTimePoint;
		TimerFunc mTask;
		TimerManager<SelfType>& mMgr;
		friend class TimerManager < SelfType > ;
	};

	template <class TimerType>
	class TimerManager
	{
	public:
		using Timer = TimerType;
		using TimePoint = typename Timer::TimePoint;
		struct TimerParam
		{
			Timer* timer;
			bool operator>(const TimerParam& _Right) const {
				return timer->mTimePoint > _Right.timer->mTimePoint;
			}
		};
		TimerManager(int useless)
			: mRunning(true)
		{
			make_heap(mTimers.begin(), mTimers.end(), greater<TimerParam>());
		}
		~TimerManager()
		{
			if (mTimerThread.joinable())
			{
				{
					lock_guard<mutex> lock(mMutexWait);
				}
				mRunning = false;
				mWaitFact.notify_all();
				mTimerThread.join();
			}
		}
		bool addTimer(Timer& t)
		{
			//GF_LOGD("%s", __FUNCTION__);
			if (!mTimerThread.joinable())
			{
				mTimerThread = thread([this](){ this->threadProc(); });
			}
			lock_guard<mutex> lock(mMutexTimers);
			for (auto& tp : mTimers)
			{
				if (tp.timer == &t)
				{
					// existing
					return false;
				}
			}
			TimerParam newtp;
			newtp.timer = &t;
			// check if the earlest one
			bool needReset = false;
			auto earliest = mTimers.begin();
			if (earliest != mTimers.end())
			{
				if (*earliest > newtp)
					needReset = true;
			}
			else
			{
				// the first one
				needReset = true;
			}
			// insert new item
			mTimers.push_back(newtp);
			push_heap(mTimers.begin(), mTimers.end(), greater<TimerParam>());
			// check if need reset timer
			if (needReset)
			{
				{
					lock_guard<mutex> lock(mMutexWait);
				}
				mWaitFact.notify_all();
			}

			return true;
		}
		bool removeTimer(Timer& t)
		{
			//GF_LOGD("%s", __FUNCTION__);
			lock_guard<mutex> lock(mMutexTimers);
			for (auto tp = mTimers.begin(); tp != mTimers.end(); tp++)
			{
				if (tp->timer == &t)
				{
					// check if the earlest one
					bool needReset = false;
					if (tp == mTimers.begin())
					{
						needReset = true;
					}
					// existing, remove it
					mTimers.erase(tp);
					make_heap(mTimers.begin(), mTimers.end(), greater<TimerParam>());
					if (needReset)
					{
						{
							lock_guard<mutex> lock(mMutexWait);
						}
						mWaitFact.notify_all();
					}
					return true;
				}
			}
			return false;
		}
		static TimerManager& getInstance()
		{
			return mInstance;
		}

	private:
		void threadProc()
		{
			while (mRunning)
			{
				auto now = chrono::system_clock::now();
				TimePoint earliest;
				Timer* pTimer = nullptr;
				{
					lock_guard<mutex> timerslock(mMutexTimers);
					auto first = mTimers.begin();
					if (first == mTimers.end())
					{
						earliest = now;
					}
					else
					{
						earliest = first->timer->mTimePoint;
						if (now >= earliest)
						{
							// this timer is expired
							pTimer = first->timer;
							pop_heap(mTimers.begin(), mTimers.end(), greater<TimerParam>());
							mTimers.pop_back();
						}
					}
				}
				if (nullptr != pTimer)
				{
					pTimer->trigger();
				}
				else if (earliest != now)
				{
					// need setup timer
					unique_lock<mutex> timerlock(mMutexWait);
					mWaitFact.wait_until(timerlock, earliest);
				}
				else
				{
					// no timer in list
					unique_lock<mutex> timerlock(mMutexWait);
					mWaitFact.wait(timerlock);
				}
			};
		}

	private:
		static TimerManager mInstance;
		atomic<bool> mRunning;
		vector<TimerParam> mTimers;
		thread mTimerThread;
		mutex mMutexTimers;
		mutex mMutexWait;
		condition_variable mWaitFact;
	};

	using SimpleTimer = GfTimer < chrono::system_clock::time_point > ;

#define TIMERMANAGER_STATIC_INSTANCE(TYPE)							\
	template<> TimerManager<TYPE> TimerManager<TYPE>::mInstance(0);	\
	namespace gf { template class TimerManager<TYPE>; }

} // namespace gf
