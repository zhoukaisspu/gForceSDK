#include "LogUtils.h"
#include "BLEDongle.h"
#include "DongleManager.h"

using namespace std;
using namespace oym;

mutex DongleManager::mMutex;
atomic<Dongle*> DongleManager::mTheDongle;
gfsPtr<Dongle> DongleManager::mTheSharedPtr;

gfsPtr<Dongle> DongleManager::getDongleInstance(const tstring& sIdentifier)
{
	Dongle* rawPtr = mTheDongle.load(memory_order_acquire);
	if (nullptr == rawPtr || nullptr == mTheSharedPtr || mTheSharedPtr.get() != rawPtr)
	{
		lock_guard<mutex> lock(mMutex);
		rawPtr = mTheDongle.load(memory_order_relaxed);
		if (nullptr == rawPtr || nullptr == mTheSharedPtr || mTheSharedPtr.get() != rawPtr)
		{
			if (nullptr == mTheSharedPtr)
			{
				GF_LOGD("Creating dongle instance.");
				mTheSharedPtr = dynamic_pointer_cast<Dongle>(make_shared<BLEDongle>(sIdentifier));
			}
			mTheDongle.store(mTheSharedPtr.get(), memory_order_release);
		}
	}
	if (nullptr == mTheSharedPtr)
	{
		GF_LOGF("getDongleInstance error, object is NULL.");
	}
	return mTheSharedPtr;
}
