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
	Dongle* retVal = mTheDongle.load(memory_order_acquire);
	if (nullptr == retVal)
	{
		lock_guard<mutex> lock(mMutex);
		retVal = mTheDongle.load(memory_order_relaxed);
		if (nullptr == retVal)
		{
			GF_LOGD("Creating the dongle.");
			mTheSharedPtr = dynamic_pointer_cast<Dongle>(make_shared<BLEDongle>(sIdentifier));
			mTheDongle.store(mTheSharedPtr.get(), memory_order_release);
		}
	}
	if (nullptr == mTheSharedPtr)
	{
		GF_LOGD("getDongleInstance error, object is NULL.");
	}
	return mTheSharedPtr;
}
