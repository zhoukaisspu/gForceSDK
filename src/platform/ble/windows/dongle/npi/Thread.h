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
#ifndef __THREAD_H__
#define __THREAD_H__

#include <string>

#include   <windows.h>
#include   <process.h>

class Runnable
{
public:
	virtual ~Runnable() {};
	virtual void Run() = 0;
};

class CThread : public Runnable
{
private:
	explicit CThread(const CThread& rhs);

public:
	CThread();
	CThread(Runnable* pRunnable);
	CThread(const char* ThreadName, Runnable* pRunnable = NULL);
	CThread(std::string ThreadName, Runnable* pRunnable = NULL);
	~CThread(void);

	/**
	@Start thread
	@arg bSuspend:suspend or not
	**/
	bool Start(bool bSuspend = false);

	/**
	@thread function to run, can rewrite this function with derived classes.
	**/
	virtual void Run();

	/**
	@wait for this thread with timeout parameter
	@arg timeout: If it is negative,wait for indefinitely
	**/
	void Join(int timeout = -1);
	/**
	@Resume thread
	**/
	void Resume();
	/**
	@Suspend thread
	**/
	void Suspend();
	/**
	@End thread
	**/
	bool Terminate(unsigned long ExitCode);

	unsigned int GetThreadID();
	HANDLE GetEvent(void);
	std::string GetThreadName();
	void SetThreadName(std::string ThreadName);
	void SetThreadName(const char* ThreadName);
	BOOL SetPriority(int pri);
private:
	static unsigned int WINAPI StaticThreadFunc(void* arg);

private:
	HANDLE m_handle;
	HANDLE m_event;
	Runnable* const m_pRunnable;
	unsigned int m_ThreadID;
	std::string m_ThreadName;
	volatile bool m_bRun;
};

#endif
