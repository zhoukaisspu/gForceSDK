#include "LogUtils.h"
#include "BLEHub.h"
#include "HubManager.h"
#include "HubManagerImpl.h"

using namespace std;
using namespace gf;

mutex HubManagerImpl::mMutex;
atomic<Hub*> HubManagerImpl::mTheHub;
gfsPtr<Hub> HubManagerImpl::mTheSharedPtr;

gfsPtr<Hub> HubManagerImpl::getHubInstanceImpl(const tstring& sIdentifier)
{
	Hub* rawPtr = mTheHub.load(memory_order_acquire);
	if (nullptr == rawPtr || nullptr == mTheSharedPtr || mTheSharedPtr.get() != rawPtr)
	{
		lock_guard<mutex> lock(mMutex);
		rawPtr = mTheHub.load(memory_order_relaxed);
		if (nullptr == rawPtr || nullptr == mTheSharedPtr || mTheSharedPtr.get() != rawPtr)
		{
			if (nullptr == mTheSharedPtr)
			{
				GF_LOGD("Creating hub instance.");
				mTheSharedPtr = dynamic_pointer_cast<Hub>(make_shared<BLEHub>(sIdentifier));
			}
			mTheHub.store(mTheSharedPtr.get(), memory_order_release);
		}
	}
	if (nullptr == mTheSharedPtr)
	{
		GF_LOGF("getHubInstance error, object is NULL.");
	}
	return mTheSharedPtr;
}
