#pragma once

#include "locker.h"

namespace Utility
{
	CSLock::CSLock(void)
	{
		InitializeCriticalSection(&m_criticalSection);
	}

	CSLock::~CSLock(void)
	{
		DeleteCriticalSection(&m_criticalSection);
	}

	void CSLock::lock(void)
	{
		EnterCriticalSection(&m_criticalSection);
	}

	void CSLock::unlock(void)
	{
		LeaveCriticalSection(&m_criticalSection);
	}

	bool CSLock::tryLock()
	{
		return TryEnterCriticalSection(&m_criticalSection) ? true : false;
	}

	CAutoLocker:: CAutoLocker(CSLock* pLocker)
		: m_lock(pLocker)
	{
		m_lock->lock();
	}

	CAutoLocker::~CAutoLocker(void)
	{
		m_lock->unlock();
		m_lock = NULL;
	}
}
