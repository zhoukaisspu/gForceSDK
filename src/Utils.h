#pragma once

#include "gfTypes.h"
#include <memory>
#include <locale.h>
#include <algorithm>
using namespace std;

namespace oym {

	//#define __GF_INLINE__ inline
#define __GF_INLINE__ static

	struct utils
	{
		__GF_INLINE__ wstring towstring(const string& str)
		{
			size_t convertedChars = 0;
			unsigned len = str.size() * 2 + 1;
			//string curLocale = setlocale(LC_CTYPE, "");
			unique_ptr<wchar_t[]> up(new wchar_t[len]);
			wchar_t* p = up.get();
			p[len - 1] = L'\0';
			mbstowcs_s(&convertedChars, p, len, str.c_str(), _TRUNCATE);
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
			unsigned len = wstr.size() * 4;
			//string curLocale = setlocale(LC_CTYPE, "");
			unique_ptr<char[]> up(new char[len + 1]);
			char* p = up.get();
			p[len] = '\0';
			wcstombs_s(&convertedChars, p, len, wstr.c_str(), _TRUNCATE);
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

		__GF_INLINE__ tstring deviceAddressToString(const GF_UINT8 addr[], GF_SIZE len)
		{
			if (nullptr == addr)
				return tstring();
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
			return totstring(string(str));
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

} // namespace oym
