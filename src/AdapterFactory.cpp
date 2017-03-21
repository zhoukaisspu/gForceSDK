#include "LogUtils.h"
#include "BLEAdapter.h"
#include "AdapterFactory.h"

using namespace std;
using namespace oym;

mutex AdapterFactory::mMutex;
atomic<Adapter*> AdapterFactory::mTheAdapter;
gfsPtr<Adapter> AdapterFactory::mTheSharedPtr;

gfsPtr<Adapter> AdapterFactory::getAdapterInstance(const tstring& sIdentifier)
{
	Adapter* rawPtr = mTheAdapter.load(memory_order_acquire);
	if (nullptr == rawPtr || nullptr == mTheSharedPtr || mTheSharedPtr.get() != rawPtr)
	{
		lock_guard<mutex> lock(mMutex);
		rawPtr = mTheAdapter.load(memory_order_relaxed);
		if (nullptr == rawPtr || nullptr == mTheSharedPtr || mTheSharedPtr.get() != rawPtr)
		{
			if (nullptr == mTheSharedPtr)
			{
				GF_LOGD("Creating adapter instance.");
				mTheSharedPtr = dynamic_pointer_cast<Adapter>(make_shared<BLEAdapter>(sIdentifier));
			}
			mTheAdapter.store(mTheSharedPtr.get(), memory_order_release);
		}
	}
	if (nullptr == mTheSharedPtr)
	{
		GF_LOGF("getAdapterInstance error, object is NULL.");
	}
	return mTheSharedPtr;
}
