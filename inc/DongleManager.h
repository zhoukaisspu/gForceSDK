#pragma once

#include <mutex>
#include <atomic>

#include "gfTypes.h"
#include "Dongle.h"

namespace oym
{

	class DongleManager
	{
	public:
		// Note that client don't care of maintaining the Hub pointer
		static gfsPtr<Dongle> getDongleInstance(const tstring& sIdentifier);

	private:
		static gfsPtr<Dongle> mTheSharedPtr;
		static std::atomic<Dongle*> mTheDongle;

	private:
		DongleManager();
		~DongleManager();

		static std::mutex mMutex;
	};

} // namespace oym
