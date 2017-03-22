#include "NpiInterface.h"
#include <oym_types.h>
#define COM_PORT_NUM 3

/* GF_NIF as a abstract of NPI_Interface, upper level get GF_NIF instant, which is able send command to  
lower layer, register callback function to receiver event. the event is masked by eventmask.
*/

GF_STATUS GF_CNpiInterface::GF_Process_Event(GF_UINT16 event_code, GF_PUINT8 data, GF_UINT8 length)
{
	GF_UINT32 event = 0x00;
	GF_STATUS result = GF_FAIL;

	LOGDEBUG(mTag, "received event 0x%x \n", event_code);

	switch (event_code) {
		case HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT: //0x0700
			LOGDEBUG(mTag, "HCI_EXT_GAP_DEVICE_INIT_DONE_MSG \n");
			break;

		case HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT: //0x0701
			for (GF_UINT8 i = 0; i < CALLBACK_MAX_INDEX; i++)
			{
				if (mCallback[i] != NULL)
				{
					GF_UINT32 result = ((mCallback[i]->GetEventMask()) & EVENT_MASK_GAP_DEVICE_DISCOVERY_MSG);
					LOGDEBUG(mTag, "result = %d \n", result);
					if (result != 0)
					{
						mCallback[i]->OnEvent(EVENT_MASK_GAP_SCAN_FINISHED, NULL, 0);
					}
				}
			}
			return GF_INNER_SUCCESS;

		case HCI_EXT_GAP_DEVICE_INFO_EVENT: //0x070D
			for (GF_UINT8 i = 0; i < CALLBACK_MAX_INDEX; i++)
			{
				if (mCallback[i] != NULL)
				{
					GF_UINT32 result = ((mCallback[i]->GetEventMask()) & EVENT_MASK_GAP_DEVICE_INFO_MSG);

					LOGDEBUG(mTag, "result = %d \n", result);
					if (result != 0)
					{
						mCallback[i]->OnEvent(EVENT_MASK_GAP_SCAN_RESULT, data, length);
					}
				}
			}
			return GF_INNER_SUCCESS;

		case HCI_EXT_GAP_LINK_ESTABLISHED_EVENT: //0x0705
			LOGDEBUG(mTag, "HCI_EXT_GAP_LINK_ESTABLISHED_MSG \n");
			event = EVENT_MASK_GAP_LINK_ESTABLISHED_MSG;
			break;

		case HCI_EXT_GAP_LINK_TERMINATED_EVENT: //0x0706
			LOGDEBUG(mTag, "HCI_EXT_GAP_LINK_TERMINATED_MSG \n");
			event = EVENT_MASK_GAP_LINK_TERMINATED_MSG;
			break;

		case HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT: //0x707
			LOGDEBUG(mTag, "HCI_EXT_GAP_LINK_PARAM_UPDATE_MSG \n");
			event = EVENT_MASK_LINK_PARA_UPDATE_MSG;
			break;

		case HCI_EXT_GAP_AUTH_COMPLETE_EVENT://0x070A
			LOGDEBUG(mTag, "HCI_EXT_GAP_AUTH_COMPLETE_MSG \n");
			event = EVENT_MASK_AUTH_COMPLETE_MSG;
			break;

		case HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_EVENT: //0x070C
			LOGDEBUG(mTag, "HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_MSG \n");
			event = EVENT_MASK_SLAVE_REQUESTED_SECURITY_MSG;
			break;

		case HCI_EXT_GAP_BOND_COMPLETE_EVENT: //0x70E
			event = EVENT_MASK_BOND_COMPLETE_MSG;
			break;

		case HCI_EXT_GAP_CMD_STATUS_EVENT: //0x404 Gap status msg event only sent to high level with error status
			LOGDEBUG(mTag, "HCI_GAP_STATUS_MSG \n");
			event = EVENT_MASK_GAP_STATUS_MSG;
			break;

		case ATT_ERROR_EVENT: //0x0501
			LOGDEBUG(mTag, "ATT_ERROR_MSG \n");
			event = EVENT_MASK_ATT_ERROR_MSG;
			break;

		case ATT_EXCHANGE_MTU_EVENT: //0x0503
			LOGDEBUG(mTag, "ATT_EXCHANGE_MTU_MSG \n");
			event = EVENT_MASK_ATT_EXCHANGE_MTU_MSG;
			break;

		case ATT_FIND_INFO_EVENT: //0x0505 characteristic descriptor
			LOGDEBUG(mTag, "ATT_FIND_INFO_MSG \n");
			event = EVENT_MASK_ATT_READ_BY_INFO_MSG;
			break;

		case ATT_READ_BY_TYPE_EVENT: //0x0509
			LOGDEBUG(mTag, "ATT_READ_BY_TYPE_MSG \n");
			event = EVENT_MASK_ATT_READ_BY_TYPE_MSG;
			break;

		case ATT_READ_EVENT: //0x050B
			LOGDEBUG(mTag, "ATT_READ_MSG \n");
			event = EVENT_MASK_ATT_READ_RESP_MSG;
			break;

		case ATT_READ_BLOB_EVENT: //0x050D
			LOGDEBUG(mTag, "ATT_READ_BLOB_MSG \n");
			event = EVENT_MASK_ATT_READ_BLOB_RESP_MSG;
			break;

		case ATT_READ_BY_GRP_TYPE_EVENT: //0x0511
			LOGDEBUG(mTag, "ATT_READ_BY_GRP_TYPE_MSG \n");
			event = EVENT_MASK_ATT_READ_BY_GRP_TYPE_MSG;
			break;

		case ATT_WRITE_EVENT: //0x0513
			LOGDEBUG(mTag, "ATT_WRITE_MSG \n");
			event = EVENT_MASK_ATT_WRITE_MSG;
			break;

		case ATT_HANDLE_VALUE_NOTI://0x051B
			LOGDEBUG(mTag, "ATT_HANDLE_VALUE_NOTI_MSG \n");
			event = EVENT_MASK_ATT_NOTI_MSG;
			break;

	default:
		printf("NPI_EVT£ºErr msg type=%4X !\n", event_code);
		break;
	}

	for (GF_UINT8 i = 0; i < CALLBACK_MAX_INDEX; i++)
	{
		if (mCallback[i] != NULL)
		{
			GF_UINT32 result = ((mCallback[i]->GetEventMask()) & event);
			if (result != 0)
			{
				mCallback[i]->OnEvent(event, data, length);
				result = GF_INNER_SUCCESS;
			}
		}
	}

	return result;
}
void GF_CNpiInterface::Run()
{
	GF_UINT8 length;

	while (mEventQueue != NULL)
	{
		sEvt* pEvt = mEventQueue->Pop();
		UINT16 opCode;
		if (pEvt->type == HCI_EXIT_PACKET)
		{
			LOGDEBUG(mTag, "GF_NIF event thread exit!! \n");
			printf("GF_NIF event thread exit!! \n");
			CloseHandle(mEvtThread->GetEvent());
			delete pEvt;
			return;
		}
		
		if (pEvt->type == HCI_PORT_CLOSE_PACKET)
		{
			if (mCallback[ADAPTER_MANAGER_CALLBACK_INDEX] != NULL)
			{
				mCallback[ADAPTER_MANAGER_CALLBACK_INDEX]->OnEvent(EVENT_SERIAL_PORT_NOT_AVAILABLE, NULL, 0);
			}
			
			delete pEvt;
			return;
		}

		if (pEvt->evtCode == HCI_LE_EVENT_CODE)
		{
			sHciEvt* pHciEvt = (sHciEvt*)pEvt;
			opCode = BUILD_UINT16(pHciEvt->op_lo, pHciEvt->op_hi);
			length = pHciEvt->len - 3;
			GF_Process_Event(opCode, &(pHciEvt->status), length);
		}
		else 
		{
			sNpiEvt* pNpiEvt = (sNpiEvt*)pEvt;
			UINT16 opCode = (pNpiEvt->event_lo + (pNpiEvt->event_hi << 8));
			length = pNpiEvt->len - 2;
			GF_Process_Event(opCode, &(pNpiEvt->status), length);
		}
		
		delete pEvt;
	}
}

GF_CNpiInterface::GF_CNpiInterface()
{
	mTag = MODUAL_TAG;
	for (GF_UINT8 i = 0; i < CALLBACK_MAX_INDEX; i++)
	{
		mCallback[i] = NULL;
	}
}

GF_CNpiInterface::~GF_CNpiInterface()
{
}

GF_STATUS GF_CNpiInterface::Init(GF_UINT8 com_num)
{
	mCommand = new NPI_CMD(com_num);
	if (mCommand == NULL)
	{
		return GF_FAIL;
	}

	if (NULL == (mEventQueue = (mCommand->Connect(NULL, 1))))
	{
		delete mCommand;

		return GF_FAIL;
	}

	/*create the event thread to receive message from NPI dongle.*/
	mEvtThread = new CThread(this);
	mEvtThread->Start();
	mEvtThread->Join(100);
	mEvtThreadID = mEvtThread->GetThreadID();

	return GF_INNER_SUCCESS;
}

GF_STATUS GF_CNpiInterface::Deinit()
{
	LOGDEBUG(mTag, "Deinit!! \n");
	//no need to close com port??
	if (mCommand != NULL)
	{
		mCommand->DisConnect();
		delete mCommand;
		mCommand = NULL;
	}

	return GF_INNER_SUCCESS;
}

GF_STATUS GF_CNpiInterface::InitDevice()
{
	GF_STATUS result;
	if (mCommand == NULL)
	{
		return GF_FAIL;
	}

	gapRole_t gap_role;
	GF_UINT8 irk[16] = { 0 };
	GF_UINT8 csrk[16] = { 0 };
	GF_UINT8 ltk[16] = { 0 };
	GF_UINT8 rand[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	GF_UINT8 sign_count = 1;
	gap_role.data = 0x08;
	if (GF_TRUE == mCommand->GAP_DeviceInit((gapRole_t)gap_role, 5, irk, csrk, (GF_PUINT8)&sign_count))
	{
		result = GF_INNER_SUCCESS;
	}
	else
	{
		result = GF_FAIL;
	}
	return result;
}

GF_STATUS GF_CNpiInterface::StartLEScan()
{
	GF_STATUS result;
	GF_BOOL status = GF_FAIL;

	if (mCommand != NULL)
	{
		status = mCommand->GAP_DeviceDiscoveryRequest(DEVDISC_MODE_ALL, NPI_ENABLE, NPI_DISABLE);
	}
		

	return result = (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::StopLEScan()
{
	GF_STATUS result;
	GF_BOOL status = GF_FAIL;

	if (mCommand != NULL)
	{
		status = mCommand->GAP_DeviceDiscoveryCancel();
	}
		
	return result = (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::Connect(GF_PUINT8 addr, UINT8 addr_type, GF_BOOL use_whitelist)
{
	GF_BOOL status = GF_FAIL;

	if (mCommand != NULL)
	{
		status = mCommand->GAP_EstablishLinkRequest(NPI_DISABLE, (eEnDisMode)use_whitelist, (eGapAddrType)addr_type, addr);
	}

	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::Disconnect(GF_UINT16 handle)
{
	GF_BOOL status = GF_FAIL;

	if (mCommand != NULL)
	{
		status = mCommand->GAP_TerminateLinkRequest(handle);
	}

	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::RegisterCallback(GF_CCallBack *callback)
{
	GF_UINT8 index = callback->GetIndex();
	if (index < CALLBACK_MAX_INDEX)
	{
		mCallback[index] = callback;
		return GF_INNER_SUCCESS;
	}
	else
	{
		return GF_FAIL;
	}
}

GF_STATUS GF_CNpiInterface::UnRegisterCallback(GF_CCallBack *callback)
{
	GF_UINT8 index = callback->GetIndex();
	if (index < CALLBACK_MAX_INDEX)
	{
		mCallback[index] = NULL;
		return GF_INNER_SUCCESS;
	}
	else
	{
		return GF_FAIL;
	}
}

GF_STATUS GF_CNpiInterface::Authenticate(GF_UINT16 handle)
{
	GF_BOOL status = GF_FAIL;

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
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::Bond(GF_UINT16 handle, GF_PUINT8 ltk, GF_UINT16 div, GF_PUINT8 rand, GF_UINT8 ltk_size)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GAP_Bond(handle, NPI_DISABLE, ltk, div, rand, ltk_size);

	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::ExchangeMTUSize(GF_UINT16 handle, GF_UINT16 mtu)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_ExchangeMTU(handle, mtu);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::UpdateConnectionParameter(GF_UINT16 handle, GF_UINT16 int_min, GF_UINT16 int_max, GF_UINT16 slave_latency, GF_UINT16 supervision_timeout)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->L2CAP_ConnParamUpdateReq(handle, int_min, int_max, slave_latency, supervision_timeout);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::DiscoveryAllPrimaryService(GF_UINT16 handle)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_DiscAllPrimaryServices(handle);

	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::DiscoveryIncludedPrimaryService(GF_UINT16 conn_handle, GF_UINT16 start_handle, GF_UINT16 end_handle)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_FindIncludedServices(conn_handle, start_handle, end_handle);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::DiscoveryCharacteristic(GF_UINT16 conn_handle, GF_UINT16 start_handle, GF_UINT16 end_handle)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_DiscAllChar(conn_handle, start_handle, end_handle);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::ReadCharacteristicValue(GF_UINT16 conn_handle, GF_UINT16 att_handle)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_ReadCharVal(conn_handle, att_handle);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::ReadCharacteristicLongValue(GF_UINT16 conn_handle, GF_UINT16 att_handle, UINT16 offset)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_ReadLongCharValue(conn_handle, att_handle, offset);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::FindCharacteristicDescriptor(GF_UINT16 conn_handle, GF_UINT16 start_handle, GF_UINT16 end_handle)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_DiscAllCharDesc(conn_handle, start_handle, end_handle);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

GF_STATUS GF_CNpiInterface::WriteCharacVlaue(GF_UINT16 conn_handle, GF_UINT16 att_handle, GF_PUINT8 data, GF_UINT8 len)
{
	GF_BOOL status = GF_FAIL;
	status = mCommand->GATT_WriteCharValue(conn_handle, att_handle, data, len);
	return (status == GF_TRUE) ? GF_INNER_SUCCESS : GF_FAIL;
}

void setColor(UINT8 color)
{
	return;
}

void setData(WCHAR *buf)
{
	return;
}

void setMessageBox(WCHAR *buf)
{
	return;
}