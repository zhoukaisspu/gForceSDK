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

// stdafx.cpp : source file that includes just the standard includes
// SDK_BLE_GUI.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

/*convert special wide string(eg: "FF:FF") to hex
*param:
const wchar_t  *src: source address
UINT8          *dst: dest address
size_t     maxCount: length of dest buf
const wchar_t  *sample: sample string
*return:
        length of converted hex data
*/
size_t wstr2hex(const wchar_t* src, UINT8* dst, size_t maxCount,
                const wchar_t* sample)
{
	size_t i, j, k = 0;
	size_t ret = 0;
	size_t dataLen = 0;
	long data;
	size_t strLen = wcslen(src);
	wchar_t* out;
	wchar_t temp[512] = { 0 };

	//analyise sample string
	UINT8 step = wcslen(sample);
	long x = wcstol(sample, NULL, 16);
	while (x) {
		dataLen++;
		x = x >> 8;
	}
	wchar_t flag = sample[step - 1];
	//convert to hex data refer to sample string
	for (i = 0, j = 0; i < strLen; i = i + step, j = j + dataLen) {
		if (i < maxCount) {

			wmemcpy_s(temp, maxCount, (src + k * step), step);
			if ((temp[step - 1] == L'\0') || (temp[step - 1] == flag)) {
				data = wcstol(temp, &out, 16);
				if ((out[0] == L'\0') || (out[0] == flag)) {
					memcpy_s((dst + j), maxCount, &data, dataLen);
					ret++;
				} else {
					return 0;
				}
			} else {
				return 0;
			}
		} else {
			return 0;
		}
		k++;
	}
	return ret;
}

void hex2wstr(wchar_t* dst, const UINT8* src, UINT8 len)
{
	INT16 i;
	char buf[256] = { 0 };
	char step = sizeof("00:") - 1;
	for (i = 0; i < len - 1; i++) {
		sprintf_s(&buf[step * i], 256 - step * i, "%02X:", *(src + i));
		SHAnsiToUnicode(&buf[step * i], dst + step * i, step + 1);
	}
	sprintf_s(&buf[step * i], 256 - step * i, "%02X", *(src + i));
	SHAnsiToUnicode(&buf[step * i], dst + step * i, step + 1);
}

wstring ANSIToUnicode(const string& str)
{
	int  len = 0;
	len = str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP,
	                                        0,
	                                        str.c_str(),
	                                        -1,
	                                        NULL,
	                                        0);
	wchar_t*   pUnicode;
	pUnicode = new  wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP,
	                      0,
	                      str.c_str(),
	                      -1,
	                      (LPWSTR)pUnicode,
	                      unicodeLen);
	wstring  rt;
	rt = (wchar_t*)pUnicode;
	delete[]  pUnicode;

	return  rt;
}

string UnicodeToANSI(const wstring& str)
{
	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP,
	                               0,
	                               str.c_str(),
	                               -1,
	                               NULL,
	                               0,
	                               NULL,
	                               NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP,
	                      0,
	                      str.c_str(),
	                      -1,
	                      pElementText,
	                      iTextLen,
	                      NULL,
	                      NULL);
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}
