#include "stdafx.h"
#include "OYM_NIF.h"
#include <oym_types.h>
#define COM_PORT_NUM 3

/* OYM_NIF as a abstract of NPI_Interface, upper level get OYM_NIF instant, which is able send command to  
lower layer, register callback function to receiver event. the event is masked by eventmask.
*/

OYM_STATUS OYM_NPI_Interface::OYM_Process_Event(OYM_UINT16 event_code, OYM_PUINT8 data, OYM_UINT8 length)
{
	LISTCALLBACK::iterator ii;
	OYM_UINT32 event = 0x00;
	OYM_STATUS result = OYM_FAIL;

	LOGDEBUG("received event 0x%x \n", event_code);

	switch (event_code) {
		case HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT: //0x0700
			LOGDEBUG("HCI_EXT_GAP_DEVICE_INIT_DONE_MSG \n");
			break;

		case HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT: //0x0701
			LOGDEBUG("HCI_EXT_GAP_DEVICE_DISCOVERY_MSG size of callback is %d\n", mCallback.size());
			for (ii = mCallback.begin(); ii != mCallback.end(); ii++)
			{
				OYM_UINT16 result = (((*ii)->GetEventMask()) & EVENT_MASK_GAP_DEVICE_DISCOVERY_MSG);
				LOGDEBUG("(*ii)->GetEventMask() = %d \n", (*ii)->GetEventMask());
				LOGDEBUG("result = %d \n", result);
				if (result != 0)
				{
					(*ii)->OnEvent(EVENT_MASK_GAP_SCAN_FINISHED, NULL, 0);
				}
			}
			break;

		case HCI_EXT_GAP_DEVICE_INFO_EVENT: //0x070D
			LOGDEBUG("HCI_EXT_GAP_DEVICE_INFO_MSG with size = %d, size of callback is %d\n", length, mCallback.size());
			for (ii = mCallback.begin(); ii != mCallback.end(); ii++)
			{
				OYM_UINT16 result = (((*ii)->GetEventMask()) & EVENT_MASK_GAP_DEVICE_INFO_MSG);
				LOGDEBUG("(*ii)->GetEventMask() = 0x%x \n", (*ii)->GetEventMask());

				LOGDEBUG("result = %d \n", result);
				if (result != 0)
				{
					(*ii)->OnEvent(EVENT_MASK_GAP_SCAN_RESULT, data, length);
				}
			}
			break;

		case HCI_EXT_GAP_LINK_ESTABLISHED_EVENT: //0x0705
			LOGDEBUG("HCI_EXT_GAP_LINK_ESTABLISHED_MSG \n");
			event = EVENT_MASK_GAP_LINK_ESTABLISHED_MSG;
			break;

		case HCI_EXT_GAP_LINK_TERMINATED_EVENT: //0x0706
			LOGDEBUG("HCI_EXT_GAP_LINK_TERMINATED_MSG \n");
			event = EVENT_MASK_GAP_LINK_TERMINATED_MSG;
			break;

		case HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT: //0x707
			LOGDEBUG("HCI_EXT_GAP_LINK_PARAM_UPDATE_MSG \n");
			event = EVENT_MASK_LINK_PARA_UPDATE_MSG;
			break;

		case HCI_EXT_GAP_AUTH_COMPLETE_EVENT://0x070A
			LOGDEBUG("HCI_EXT_GAP_AUTH_COMPLETE_MSG \n");
			event = EVENT_MASK_AUTH_COMPLETE_MSG;
			break;

		case HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_EVENT: //0x070C
			LOGDEBUG("HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_MSG \n");
			event = EVENT_MASK_SLAVE_REQUESTED_SECURITY_MSG;
			break;

		case HCI_EXT_GAP_BOND_COMPLETE_EVENT: //0x70E
			event = EVENT_MASK_BOND_COMPLETE_MSG;
			break;

		case HCI_EXT_GAP_CMD_STATUS_EVENT: //0x404 Gap status msg event only sent to high level with error status
			LOGDEBUG("HCI_GAP_STATUS_MSG \n");
			event = EVENT_MASK_GAP_STATUS_MSG;
			break;

		case ATT_ERROR_EVENT: //0x0501
			LOGDEBUG("ATT_ERROR_MSG \n");
			event = EVENT_MASK_ATT_ERROR_MSG;
			break;

		case ATT_EXCHANGE_MTU_EVENT: //0x0503
			LOGDEBUG("ATT_EXCHANGE_MTU_MSG \n");
			event = EVENT_MASK_ATT_EXCHANGE_MTU_MSG;
			break;

		case ATT_FIND_INFO_EVENT: //0x0505 characteristic descriptor
			LOGDEBUG("ATT_FIND_INFO_MSG \n");
			event = EVENT_MASK_ATT_READ_BY_INFO_MSG;
			break;

		case ATT_READ_BY_TYPE_EVENT: //0x0509
			LOGDEBUG("ATT_READ_BY_TYPE_MSG \n");
			event = EVENT_MASK_ATT_READ_BY_TYPE_MSG;
			break;

		case ATT_READ_EVENT: //0x050B
			LOGDEBUG("ATT_READ_MSG \n");
			event = EVENT_MASK_ATT_READ_RESP_MSG;
			break;

		case ATT_READ_BLOB_EVENT: //0x050D
			LOGDEBUG("ATT_READ_BLOB_MSG \n");
			event = EVENT_MASK_ATT_READ_BLOB_RESP_MSG;
			break;

		case ATT_READ_BY_GRP_TYPE_EVENT: //0x0511
			LOGDEBUG("ATT_READ_BY_GRP_TYPE_MSG \n");
			event = EVENT_MASK_ATT_READ_BY_GRP_TYPE_MSG;
			break;

		case ATT_WRITE_EVENT: //0x0513
			LOGDEBUG("ATT_WRITE_MSG \n");
			event = EVENT_MASK_ATT_WRITE_MSG;
			break;

		case ATT_HANDLE_VALUE_NOTI://0x051B
			LOGDEBUG("ATT_HANDLE_VALUE_NOTI_MSG \n");
			event = EVENT_MASK_ATT_NOTI_MSG;
			break;

	default:
		printf("NPI_EVT£ºErr msg type=%4X !\n", event_code);
		break;
	}

	for (ii = mCallback.begin(); ii != mCallback.end(); ii++)
	{
		OYM_UINT32 result = (((*ii)->GetEventMask()) & event);

		LOGDEBUG("event = 0x%x, result = 0x%x \n", event, result);
		if (result != 0)
		{
			(*ii)->OnEvent(event, data, length);
			result = OYM_SUCCESS;
		}
		else
		{
			LOGDEBUG("EventMask = 0x%x \n", ((*ii)->GetEventMask()));
		}
	}

	return result;
}
void OYM_NPI_Interface::Run()
{
	OYM_UINT8 length;

	while (mEventQueue != NULL)
	{
		sEvt* pEvt = mEventQueue->Pop();
		UINT16 opCode;
		if (pEvt->type == HCI_EXIT_PACKET)
		{
			LOGDEBUG("OYM_NIF event thread exit!! \n");
			printf("OYM_NIF event thread exit!! \n");
			CloseHandle(mEvtThread->GetEvent());
			delete pEvt;
			return;
		}
		
		if (pEvt->evtCode == HCI_LE_EVENT_CODE)
		{
			sHciEvt* pHciEvt = (sHciEvt*)pEvt;
			opCode = BUILD_UINT16(pHciEvt->op_lo, pHciEvt->op_hi);
			length = pHciEvt->len - 3;
			OYM_Process_Event(opCode, &(pHciEvt->status), length);
		}
		else 
		{
			sNpiEvt* pNpiEvt = (sNpiEvt*)pEvt;
			UINT16 opCode = (pNpiEvt->event_lo + (pNpiEvt->event_hi << 8));
			length = pNpiEvt->len - 2;
			OYM_Process_Event(opCode, &(pNpiEvt->status), length);
		}
		
		delete pEvt;
	}
}

OYM_NPI_Interface::OYM_NPI_Interface()
{
	mLog = new OYM_Log(MODUAL_TAG, sizeof(MODUAL_TAG));
	mCallback.clear();
}

OYM_NPI_Interface::~OYM_NPI_Interface()
{
	delete mLog;
}

OYM_STATUS OYM_NPI_Interface::Init()
{
	mCommand = new NPI_CMD(COM_PORT_NUM);
	if (mCommand == NULL)
	{
		return OYM_FAIL;
	}

	if (NULL == (mEventQueue = (mCommand->Connect(NULL))))
	{
		delete mCommand;

		return OYM_FAIL;
	}

	/*create the event thread to receive message from NPI dongle.*/
	mEvtThread = new CThread(this);
	mEvtThread->Start();
	mEvtThread->Join(100);
	mEvtThreadID = mEvtThread->GetThreadID();

	return OYM_SUCCESS;
}

OYM_STATUS OYM_NPI_Interface::Deinit()
{
	LOGDEBUG("Deinit!! \n");
	//no need to close com port??
	if (mCommand != NULL)
	{
		mCommand->DisConnect();
		delete mCommand;
		mCommand = NULL;
	}

	return OYM_SUCCESS;
}

OYM_STATUS OYM_NPI_Interface::InitDevice()
{
	OYM_STATUS result;
	if (mCommand == NULL)
	{
		return OYM_FAIL;
	}

	gapRole_t gap_role;
	OYM_UINT8 irk[16] = { 0 };
	OYM_UINT8 csrk[16] = { 0 };
	OYM_UINT8 ltk[16] = { 0 };
	OYM_UINT8 rand[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	OYM_UINT8 sign_count = 1;
	gap_role.data = 0x08;
	if (OYM_TRUE == mCommand->GAP_DeviceInit((gapRole_t)gap_role, 5, irk, csrk, (OYM_PUINT8)&sign_count))
	{
		result = OYM_SUCCESS;
	}
	else
	{
		result = OYM_FAIL;
	}
	return result;
}

OYM_STATUS OYM_NPI_Interface::StartLEScan()
{
	OYM_STATUS result;
	OYM_BOOL status = OYM_FAIL;

	if (mCommand != NULL)
	{
		status = mCommand->GAP_DeviceDiscoveryRequest(DEVDISC_MODE_ALL, NPI_ENABLE, NPI_DISABLE);
	}
		

	return result = (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::StopLEScan()
{
	OYM_STATUS result;
	OYM_BOOL status = OYM_FAIL;

	if (mCommand != NULL)
	{
		status = mCommand->GAP_DeviceDiscoveryCancel();
	}
		
	return result = (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::Connect(OYM_PUINT8 addr, UINT8 addr_type, OYM_BOOL use_whitelist)
{
	OYM_STATUS result;
	OYM_BOOL status = OYM_FAIL;

	if (mCommand != NULL)
	{
		mCommand->GAP_EstablishLinkRequest(NPI_DISABLE, (eEnDisMode)use_whitelist, (eGapAddrType)addr_type, addr);
	}

	return result = status;
}

OYM_STATUS OYM_NPI_Interface::Disconnect(OYM_UINT16 handle)
{
	OYM_STATUS result;
	OYM_BOOL status = OYM_FAIL;

	if (mCommand != NULL)
	{
		mCommand->GAP_TerminateLinkRequest(handle);
	}

	return result = status;
}

OYM_STATUS OYM_NPI_Interface::RegisterCallback(OYM_CallBack *callback)
{
	LOGDEBUG("before register, num of callback is %d \n", mCallback.size());
	mCallback.push_front(callback);
	LOGDEBUG("after register, num of callback is %d \n", mCallback.size());

	return OYM_SUCCESS;
}

OYM_STATUS OYM_NPI_Interface::UnRegisterCallback(OYM_CallBack *callback)
{
	LOGDEBUG("before unregister, num of callback is %d \n", mCallback.size());
	LISTCALLBACK::iterator ii;
	for (ii = mCallback.begin(); ii != mCallback.end(); ii++)
	{
		if ((*ii)->GetIndex() == callback->GetIndex())
		{
			ii == mCallback.erase(ii);
			break;
		}
	}
	LOGDEBUG("after unregister, num of callback is %d \n", mCallback.size());

	return OYM_SUCCESS;
}

OYM_STATUS OYM_NPI_Interface::Authenticate(OYM_UINT16 handle)
{
	OYM_BOOL status = OYM_FAIL;

	sGapAuth auth;
	memset(&auth, 0, sizeof(auth));
	auth.sec_authReq.oper = 0x01;
	auth.sec_ioCaps = (eGapIOCaps)0x04;
	auth.sec_maxEncKeySize = 0x10;
	auth.sec_keyDist.oper = 0x77;
	auth.sec_oobFlag = NPI_FALSE;

	auth.pair_ioCaps = (eGapIOCaps)0x03;
	auth.pair_en = NPI_DISABLE;
	auth.pair_keyDist.oper = 0x77;
	auth.pair_maxEncKeySize = 0x10;
	auth.pair_authReq.oper = 0x01;
	auth.pair_oobFlag = NPI_FALSE;

	status = mCommand->GAP_Authenticate(handle, &auth);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::Bond(OYM_UINT16 handle, OYM_PUINT8 ltk, OYM_UINT16 div, OYM_PUINT8 rand, OYM_UINT8 ltk_size)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GAP_Bond(handle, NPI_DISABLE, ltk, div, rand, ltk_size);

	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::ExchangeMTUSize(OYM_UINT16 handle, OYM_UINT16 mtu)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_ExchangeMTU(handle, mtu);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::UpdateConnectionParameter(OYM_UINT16 handle, OYM_UINT16 int_min, OYM_UINT16 int_max, OYM_UINT16 slave_latency, OYM_UINT16 supervision_timeout)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->L2CAP_ConnParamUpdateReq(handle, int_min, int_max, slave_latency, supervision_timeout);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::DiscoveryAllPrimaryService(OYM_UINT16 handle)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_DiscAllPrimaryServices(handle);

	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::DiscoveryIncludedPrimaryService(OYM_UINT16 conn_handle, OYM_UINT16 start_handle, OYM_UINT16 end_handle)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_FindIncludedServices(conn_handle, start_handle, end_handle);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::DiscoveryCharacteristic(OYM_UINT16 conn_handle, OYM_UINT16 start_handle, OYM_UINT16 end_handle)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_DiscAllChar(conn_handle, start_handle, end_handle);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::ReadCharacteristicValue(OYM_UINT16 conn_handle, OYM_UINT16 att_handle)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_ReadCharVal(conn_handle, att_handle);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::ReadCharacteristicLongValue(OYM_UINT16 conn_handle, OYM_UINT16 att_handle, UINT16 offset)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_ReadLongCharValue(conn_handle, att_handle, offset);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::FindCharacteristicDescriptor(OYM_UINT16 conn_handle, OYM_UINT16 start_handle, OYM_UINT16 end_handle)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_DiscAllCharDesc(conn_handle, start_handle, end_handle);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}

OYM_STATUS OYM_NPI_Interface::WriteCharacVlaue(OYM_UINT16 conn_handle, OYM_UINT16 att_handle, OYM_PUINT8 data, OYM_UINT8 len)
{
	OYM_BOOL status = OYM_FAIL;
	status = mCommand->GATT_WriteCharValue(conn_handle, att_handle, data, len);
	return (status == OYM_TRUE) ? OYM_SUCCESS : OYM_FAIL;
}