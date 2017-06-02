/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
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
#include <deque>
#include <functional>
#include <thread>
#include <sstream>
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
			mCondition.notify_one();
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

} // namespace gf
