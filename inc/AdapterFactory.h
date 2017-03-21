#pragma once

#include <mutex>
#include <atomic>

#include "gfTypes.h"
#include "Adapter.h"

namespace oym
{

	class AdapterFactory
	{
	public:
		// Note that client don't care of maintaining the Hub pointer
		static gfsPtr<Adapter> getAdapterInstance(const tstring& sIdentifier);

	private:
		static gfsPtr<Adapter> mTheSharedPtr;
		static std::atomic<Adapter*> mTheAdapter;

	private:
		AdapterFactory();
		~AdapterFactory();

		static std::mutex mMutex;
	};

} // namespace oym
