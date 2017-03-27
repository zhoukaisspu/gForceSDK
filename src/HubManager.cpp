#include "HubManager.h"
#include "HubManagerImpl.h"

using namespace gf;

gfsPtr<Hub> HubManager::getHubInstance(const tstring& sIdentifier)
{
	return HubManagerImpl::getHubInstanceImpl(sIdentifier);
}
