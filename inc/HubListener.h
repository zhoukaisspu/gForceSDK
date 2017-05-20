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
/*!
 * \file HubListener.h
 * \brief The definition of gForce callbacks
 *
 * \version 0.1
 * \date 2017.4.3
 */
#pragma once

#include "gfTypes.h"
#include "Quaternion.h"

namespace gf
{

	/// \class HubListener
	/// \brief
	///     The callback interface for processing messages from the Hub.
	///
	///     The application needs to implement this class and pass its instance
	///     to Hub::registerListener()
	///
	class HubListener
	{
	public:
		/// \brief This callback is called when the Hub finishes scanning devices.
		/// \remark
		///     This callback may be called after a series of onDeviceFound()
		///     are called.
		virtual void onScanfinished() {}

		/// \brief This callback is called when the state of the hub changed
		/// \param state
		///     An enumerated value of HubState which indicates the state of the
        ///     hub.
		virtual void onStateChanged(HubState state) {}

		/// \brief
        ///     This callback is called when the hub finds a device during
        ///     scanning.
		/// \param device The pointer to a Device object that was found.
		virtual void onDeviceFound(WPDEVICE device) {}

		/// \brief
        ///     This callback is called when a previously found but not
        ///     connected device has been dropped by the Hub.
		/// \param device
        ///     The pointer to the Device object that was previously found and
		///     passed to the application.
		virtual void onDeviceDiscard(WPDEVICE device) {}

		/// \brief
        ///     This callback is called when a device has been connected to
        ///     the hub successfully.
		/// \param device
        ///     The pointer to the Device object that the hub has connected to.
		virtual void onDeviceConnected(WPDEVICE device) {}

		/// \brief
        ///     This callback is called when a device has been disconnected from
		///     connection state or failed to connect to
		/// \param device
        ///     The pointer to the Device object that was disconnected.
		/// \param reason The reason of why device disconnected.
		virtual void onDeviceDisconnected(WPDEVICE device, GF_UINT8 reason) {}

		/// \brief
        ///     This callback is called when the orientation of the device has
        ///     changed.
		/// \param device
        ///     The pointer to the Device.
		/// \param orientation
        ///     The Quaternion object that indicates the updated orientation of
        ///     the device.
		virtual void onOrientationData(WPDEVICE device, const Quaternion& orientation) {}

		/// \brief This callback is called when the gesture data is recevied
		/// \param device The Pointer to the Device.
		/// \param gest The Gesture object.
		virtual void onGestureData(WPDEVICE device, Gesture gest) {}

		/// \brief
        ///     This callback is called when the user has just re-centered the
        ///     device.
		/// \param device Pointer to the Device sending data
        /// \remark
        ///     This callback is to notifiy the application of that the user has
        ///     re-centered the device, by pressing the button on the gForce
        ///     device, to use his/her current orientation as the origin of
        ///     his/her coordinate system (i.e. [w=1, x=0, y=0, z=0] in
        ///     quaternion). Please also see <a href="https://support.google.com/daydream/answer/7184599?hl=en">Google Daydram - Re-center your view & cursor</a>
		virtual void onReCenter(WPDEVICE device) {}

		virtual ~HubListener() {}
	};
}
