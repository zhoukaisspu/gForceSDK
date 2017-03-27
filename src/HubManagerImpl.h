#pragma once

#include "gforce.h"
#include <mutex>
#include <atomic>

#include "gfTypes.h"
#include "Hub.h"

namespace gf
{

	class HubManagerImpl
	{
	private:
		// Note that client don't care of maintaining the Hub pointer
		static gfsPtr<Hub> getHubInstanceImpl(const tstring& sIdentifier);

	private:
		static gfsPtr<Hub> mTheSharedPtr;
		static std::atomic<Hub*> mTheHub;

	private:
		HubManagerImpl();
		~HubManagerImpl();

		static std::mutex mMutex;

		friend class HubManager;
	};

} // namespace gf
