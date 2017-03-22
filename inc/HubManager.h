#pragma once

#include <mutex>
#include <atomic>

#include "gfTypes.h"
#include "Hub.h"

namespace gf
{

	class HubManager
	{
	public:
		// Note that client don't care of maintaining the Hub pointer
		static gfsPtr<Hub> getHubInstance(const tstring& sIdentifier);

	private:
		static gfsPtr<Hub> mTheSharedPtr;
		static std::atomic<Hub*> mTheHub;

	private:
		HubManager();
		~HubManager();

		static std::mutex mMutex;
	};

} // namespace gf
