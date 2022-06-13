#pragma once

namespace Utility
{
	class CSLock
	{
	public:
		CSLock(void);
		~CSLock(void);

		void lock(void);
		void unlock(void);
		bool tryLock(void);

	private:
		CRITICAL_SECTION m_criticalSection;
	};

	class CAutoLocker
	{
	public:
		explicit CAutoLocker(CSLock* pLocker);

		~CAutoLocker(void);

	private:
		CSLock* m_lock;
	};
}
