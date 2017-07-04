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
#include "thread.h"


CThread::CThread(void) :
	m_pRunnable(NULL),
	m_bRun(false)
{
}

CThread::~CThread(void)
{
}

CThread::CThread(Runnable* pRunnable) :
	m_ThreadName(""),
	m_pRunnable(pRunnable),
	m_bRun(false)
{
}

CThread::CThread(const char* ThreadName, Runnable* pRunnable) :
	m_ThreadName(ThreadName),
	m_pRunnable(pRunnable),
	m_bRun(false)
{
}

CThread::CThread(std::string ThreadName, Runnable* pRunnable) :
	m_ThreadName(ThreadName),
	m_pRunnable(pRunnable),
	m_bRun(false)
{
}

bool CThread::Start(bool bSuspend)
{
	if (m_bRun) {
		return true;
	}
	if (bSuspend) {
		m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this,
		                                  CREATE_SUSPENDED, &m_ThreadID);
	} else {
		m_handle = (HANDLE)_beginthreadex(NULL, 0, StaticThreadFunc, this, 0,
		                                  &m_ThreadID);
	}
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bRun = (NULL != m_handle);
	return m_bRun;
}

void CThread::Run()
{
	if (!m_bRun) {
		return;
	}
	if (NULL != m_pRunnable) {
		m_pRunnable->Run();
	}
	m_bRun = false;
}

void CThread::Join(int timeout)
{
	if (NULL == m_handle || !m_bRun) {
		return;
	}
	if (timeout <= 0) {
		timeout = INFINITE;
	}
	::WaitForSingleObject(m_handle, timeout);
}

void CThread::Resume()
{
	if (NULL == m_handle || !m_bRun) {
		return;
	}
	::ResumeThread(m_handle);
}

void CThread::Suspend()
{
	if (NULL == m_handle || !m_bRun) {
		return;
	}
	::SuspendThread(m_handle);
}

bool CThread::Terminate(unsigned long ExitCode)
{
	if (NULL == m_handle || !m_bRun) {
		return true;
	}
	if (::SetEvent(m_event)) {
		//::CloseHandle(m_event);
		//::CloseHandle(m_handle);
		return true;
	}
	return false;
}

unsigned int CThread::GetThreadID()
{
	return m_ThreadID;
}

HANDLE CThread::GetEvent(void)
{
	return m_event;
}
std::string CThread::GetThreadName()
{
	return m_ThreadName;
}

void CThread::SetThreadName(std::string ThreadName)
{
	m_ThreadName = ThreadName;
}

void CThread::SetThreadName(const char* ThreadName)
{
	if (NULL == ThreadName) {
		m_ThreadName = "";
	} else {
		m_ThreadName = ThreadName;
	}
}

unsigned int CThread::StaticThreadFunc(void* arg)
{
	CThread* pThread = (CThread*)arg;
	pThread->Run();
	return 0;
}

BOOL CThread::SetPriority(int pri)
{
	return ::SetThreadPriority(m_handle, pri);
}