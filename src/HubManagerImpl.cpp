/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 *
 * IMPORTANT: Your use of this Software is limited to those specific rights
 * granted under the terms of a software license agreement between you and
 * OYMotion.  You may not use this Software unless you agree to abide by the
 * terms of the License. The License limits your use, and you acknowledge,
 * that the Software may not be modified, copied or distributed unless used
 * solely and exclusively in conjunction with an OYMotion product.  Other
 * than for the foregoing purpose, you may not use, reproduce, copy, prepare
 * derivative works of, modify, distribute, perform, display or sell this
 * Software and/or its documentation for any purpose.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 */
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
