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
	std::string GetThreadName();
	void SetThreadName(std::string ThreadName);
	void SetThreadName(const char* ThreadName);

private:
	static unsigned int WINAPI StaticThreadFunc(void* arg);

private:
	HANDLE m_handle;
	Runnable* const m_pRunnable;
	unsigned int m_ThreadID;
	std::string m_ThreadName;
	volatile bool m_bRun;
};

#endif
