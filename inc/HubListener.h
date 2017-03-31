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
#pragma once

#include "gfTypes.h"

/// \namespace gf
namespace gf
{

	/// \class HubListener
	/// \brief
	///		The callback interface for processing events from the Hub. The
	///     application needs to implement this class and pass its instance
	///     to Hub::registerListener()
	///
	///
	class HubListener
	{
	public:
		/// This callback is called when the Hub finishes scanning devices.
		///
		/// \remark
		///		This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual void onScanfinished() = 0;

		/// This callback is called when the hub finds a device.
		///
		/// \param device The pointer to Device that was found.
		virtual void onDeviceFound(WPDEVICE device) = 0;

		/// This callback is called when a previous connected device has
		/// disconnected from the hub.
		///
		/// \param device The pointer to Device that was previously found and
		///				  passed to the application.
		///
		virtual void onDeviceDiscard(WPDEVICE device) = 0;

		/// This callback is called when a device is specialized.
		///
		/// \param oldDevice
		/// 	The pointer to an Device object, which has become invalid.
		/// \param newDevice
		///		The pointer to and Device object, which is the substitue to
		///     oldDevice.
		///
		/// \remark
		///		When a device is specialized, it means that more functionalities
		///     of it are recongnized. In such case, a new Device object pointed
		///     by newDevice is created to take over the old one pointed by
		///	    oldDevice, nothing more to be cared but discard the oldDevice.
		virtual void onDeviceSpecialized(WPDEVICE oldDevice, WPDEVICE newDevice) = 0;

		/// This callback is called when the state of the hub changed
		/// \param state
		///		An enumerate of HubState which indicates the state of the hub. 
		virtual void onStateChanged(HubState state) = 0;
	};

}
