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
#include <windows.h>


template<class T, unsigned max>
class NPI_Queue
{
	HANDLE space_avail; // at least one slot empty
	HANDLE data_avail;  // at least one slot full
	CRITICAL_SECTION mutex; // protect buffer, in_pos, out_pos

	T buffer[max];
	long in_pos, out_pos;
public:
	NPI_Queue() : in_pos(0), out_pos(0)
	{
		space_avail = CreateSemaphore(NULL, max, max, NULL);
		data_avail = CreateSemaphore(NULL, 0, max, NULL);
		InitializeCriticalSection(&mutex);
	}

	void Push(T data)
	{
		WaitForSingleObject(space_avail, INFINITE);
		EnterCriticalSection(&mutex);
		buffer[in_pos] = data;
		in_pos = (in_pos + 1) % max;
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(data_avail, 1, NULL);
	}

	T Pop()
	{
		WaitForSingleObject(data_avail, INFINITE);
		EnterCriticalSection(&mutex);
		T retval = buffer[out_pos];
		out_pos = (out_pos + 1) % max;
		LeaveCriticalSection(&mutex);
		ReleaseSemaphore(space_avail, 1, NULL);
		return retval;
	}

	~NPI_Queue()
	{
		DeleteCriticalSection(&mutex);
		CloseHandle(data_avail);
		CloseHandle(space_avail);
	}

};



