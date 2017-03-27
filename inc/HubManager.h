#pragma once

#include "gforce.h"
#include "gfTypes.h"
#include "Hub.h"

namespace gf
{

	class GFORCESDK_API HubManager
	{
	public:
		// Note that client don't care of maintaining the Hub pointer
		static gfsPtr<Hub> getHubInstance(const tstring& sIdentifier);
	};

} // namespace gf
