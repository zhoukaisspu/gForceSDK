#include "RemoteDevice.h"
#define LOCAL_GATT_CLIENT_MTU_SIZE 23

/*RemoteDevice is a abstract of Remote Device.*/
#define TEMP_BUFFER_SIZE 256
GF_UINT8 Temp_Buffer[TEMP_BUFFER_SIZE];
GF_UINT8 Total;

/*used to receive message from AdapterManager.*/
GF_VOID GF_CRemoteDevice::Run()
{
	MESSAGE* msg;
	GF_PUINT8	buf;
	GF_UINT16	size;
	N = 0;

	while (1) {
		N++;
		if (mThreadRunning == GF_FALSE)
		{
			if (mThrandEvent != NULL)
			{
				CloseHandle(mThread->GetEvent());
				mThrandEvent = NULL;
				printf(("num = %d, aaa threads exit with err=%d\n"), N, GetLastError());
			}
			/*why execute this code after return??*/
			printf(("num = %d, threads exit with err=%d\n"), N, GetLastError());
			return;
		}

		EnterCriticalSection(&mMutex);
		if (mMessage.size() != 0) //get msg from message queue
		{
			LOGDEBUG(mTag, "Message.size() = 0x%x \n", mMessage.size());
			msg = mMessage.front();
			if(msg != NULL)
			{
				LOGDEBUG(mTag, "shimeng pop msg = 0x%x \n", msg);
				buf = (GF_PUINT8)msg->data;
				size = (GF_UINT16)msg->length;

				mMessage.pop_front();
				LeaveCriticalSection(&mMutex);
			}
			else
			{
				LeaveCriticalSection(&mMutex);
				return;
			}
			LOGDEBUG(mTag, "received message 0x%x \n", msg->event);
			
			switch (mState) 
			{
				case GF_DEVICE_STATE_IDLE:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_IDLE state received message \n");
					IdleStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_W4CONN:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_W4CONN state received message \n");
					W4ConnStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_W4SECU:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_W4SECU state received message \n");
					W4SecuStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_GATT_PRI_SVC:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_GATT_PRI_SVC state received message \n");
					W4GattPriSvcStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_GATT_INC_SVC:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_GATT_INC_SVC state received message \n");
					W4GattIncSvcStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_GATT_DIS_CHARC:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_GATT_DIS_CHARC state received message \n");
					W4GattDisCharcStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_GATT_READ_CHARC_VALUE:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_GATT_READ_CHARC_VALUE state received message \n");
					W4GattReadCharcValueStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_GATT_READ_CHARC_DESCRIPTOR_VALUE:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_GATT_READ_CHARC_DESCRIPTOR_VALUE state received message \n");
					W4GattReadCharcDesValueStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				case GF_DEVICE_STATE_CONNECTED:
					LOGDEBUG(mTag, "GF_DEVICE_STATE_CONNECTED state received message \n");
					DeviceConnectedStateProcessMessage((GF_DEVICE_EVENT)msg->event, buf, size);
					break;
				default:
					break;
			}

			delete msg;
		}
		else 
		{
			LeaveCriticalSection(&mMutex);
			Sleep(50);
		}
	}

	LOGDEBUG(mTag, "thread stop to process message! \n");
}

GF_CRemoteDevice::GF_CRemoteDevice(GF_CNpiInterface* minterface, GF_UINT8 addr_type, GF_PUINT8 address, GF_CCallBack* callback)
{
	mInterface = minterface;
	mCallback = callback;
	mAddrType = addr_type;
	memcpy(mAddr, address, BT_ADDRESS_SIZE);
	memset(mDevName, 0, BLE_DEVICE_NAME_LENGTH);
	mTag = MODUAL_TAG_RD;

	mDatabase = new GF_CDatabase(mAddr, BT_ADDRESS_SIZE);
	mHandle = INVALID_HANDLE;
	mState = GF_DEVICE_STATE_IDLE;
}

GF_CRemoteDevice::~GF_CRemoteDevice()
{
	delete mDatabase;
}


GF_VOID GF_CRemoteDevice::Init()
{
	LOGDEBUG(mTag, "Init thread to process message... \n");
	InitializeCriticalSection(&mMutex);
	mThreadRunning = GF_TRUE;
	/*create the thread to process message.*/
	mThread = new CThread(this);
	mThread->Start();
	mThread->Join(100);
	mThreadID = mThread->GetThreadID();
	mThrandEvent = mThread->GetEvent();
	mThreadRunning = GF_TRUE;
	mNeedSaveLTK = GF_TRUE;
	mNeedSaveService = GF_TRUE;
	mHandle = INVALID_HANDLE;
	mMessage.clear();
}

GF_VOID GF_CRemoteDevice::DeInit()
{
	LOGDEBUG(mTag, "DeInit... \n");
	if (mThread != NULL)
	{
		mThreadRunning = GF_FALSE;
	}
	mState = GF_DEVICE_STATE_IDLE;
	mHandle = INVALID_HANDLE;
}

GF_STATUS GF_CRemoteDevice::OnDeviceConnected(GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS status;
	LOGDEBUG(mTag, "GF_DEVICE_EVENT_DEVICE_CONNECTED with length = %d\n", length);
	for (GF_UINT16 i = 0; i < length; i++)
	{
		LOGDEBUG(mTag, "the data of [%d]th bytes is 0x%02x \n", i, data[i]);
	}
	status = data[EVENT_DEVICE_CCONNECTED_STATUS];
	if (status != GF_SUCCESS)
	{
		return status;
	}

	mHandle = data[EVENT_DEVICE_CCONNECTED_CONN_HANDLE_OFFSET] + (data[EVENT_DEVICE_CCONNECTED_CONN_HANDLE_OFFSET+1] << 8);
	LOGDEBUG(mTag, "mHandle = 0x%02x \n", mHandle);
	mConnRole = (GF_DEVICE_ROLE)data[EVENT_DEVICE_CCONNECTED_ROLE_OFFSET];
	mConnInternal = data[EVENT_DEVICE_CCONNECTED_CONN_INTERVAL_OFFSET] + (data[EVENT_DEVICE_CCONNECTED_CONN_INTERVAL_OFFSET+1] << 8);
	mSlaveLatency = data[EVENT_DEVICE_CCONNECTED_SLAVE_LATENCY_OFFSET] + (data[EVENT_DEVICE_CCONNECTED_SLAVE_LATENCY_OFFSET+1] << 8);
	mConnTimeout = data[EVENT_DEVICE_CCONNECTED_CONN_TIMEOUT_OFFSET] + (data[EVENT_DEVICE_CCONNECTED_CONN_TIMEOUT_OFFSET+1] << 8);
	mClockAccuracy = data[EVENT_DEVICE_CCONNECTED_CLOCK_ACCURACY_OFFSET];

	LOGDEBUG(mTag, "mConnRole = 0x%02x \n", mConnRole);
	LOGDEBUG(mTag, "mConnInternal = 0x%02x \n", mConnInternal);
	LOGDEBUG(mTag, "mSlaveLatency = 0x%02x \n", mSlaveLatency);
	LOGDEBUG(mTag, "mConnTimeout = 0x%02x \n", mConnTimeout);
	LOGDEBUG(mTag, "mClockAccuracy = 0x%02x \n", mClockAccuracy);

	/*start to authentication automatic if we do not have the link key of remote device*/
	/*check whether we have the link key, if not, start to authenticate, to do...*/
	status = mDatabase->LoadLinkKey(&mKeySize, mLTK, &mDIV, mRAND);
	//status = GF_FAIL;
	printf("LoadLinkKey done!! with status = %d \n", status);
	if (status == GF_SUCCESS)
	{
		/*link key exist, start to encrprtion*/
		LOGDEBUG(mTag, "Start to Bond device... \n");
		status = mInterface->Bond(mHandle, mLTK, mDIV, mRAND, mKeySize);
		mNeedSaveLTK = GF_FALSE;
	}
	else
	{
		/*link key donot exist, start to authentication first.*/
		LOGDEBUG(mTag, "Start to Authenticat device... \n");
		status = mInterface->Authenticate(mHandle);
	}

	return status;
}

GF_STATUS GF_CRemoteDevice::IdleStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	LOGDEBUG(mTag, "IdleStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_DEVICE_CONNECTED:
		{
			result = OnDeviceConnected(data, length);
			if (result == GF_SUCCESS)
			{
				mState = GF_DEVICE_STATE_W4SECU;
			}
			else
			{
				mState = GF_DEVICE_STATE_IDLE;
			}
		}
		break;

		case GF_DEVICE_STATE_W4CONN:

		break;

		default:
			break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::W4ConnStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	GF_STATUS status;
	LOGDEBUG(mTag, "W4ConnStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_DEVICE_CONNECTED:
		{
			result = OnDeviceConnected(data, length);
			if (result == GF_SUCCESS)
			{
				mState = GF_DEVICE_STATE_W4SECU;
			}
			else
			{
				mState = GF_DEVICE_STATE_IDLE;
			}
			break;
		}
		case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST with length = %d\n", length);

			status = data[EVENT_SLAVE_SECURY_REQUEST_STATUS_OFFSET];
			if (status != GF_SUCCESS)
			{
				LOGDEBUG(mTag, "GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST with error status = %d\n", status);
				return result;
			}

			if (0 == memcmp(data + EVENT_SLAVE_SECURY_REQUEST_ADDRESS_OFFSET, mAddr, BT_ADDRESS_SIZE))
			{
				GF_UINT8 auth_type = data[EVENT_SLAVE_SECURY_REQUEST_AUTH_TYPE_OFFSET];
				if (auth_type == 1)
				{
					LOGDEBUG(mTag, "---------->mInterface->Authenticate start....\n");
					mInterface->Authenticate(mHandle);
					mState = GF_DEVICE_STATE_W4SECU;
					LOGDEBUG(mTag, "---------->mInterface->Authenticate end....\n");
				}
				else
				{
					LOGDEBUG(mTag, "---------->mInterface->Authenticate not called....\n");
				}
			}
			else
			{
				LOGDEBUG(mTag, "address is not patch");
			}
			break;

		case GF_DEVICE_EVENT_AUTH_COMPLETE:
		case GF_DEVICE_EVENT_BOND_COMPLETE:
			LOGDEBUG(mTag, "W4ConnStateProcessMessage with error message!!!");
			break;
		
		default:
			break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::AuthenticationComplete(GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS status;
	int KeySizeOffset = 0;
	int DIVOffset = 0;
	int LTKOffset = 0;
	int RandOffset = 0;
	LOGDEBUG(mTag, "GF_DEVICE_EVENT_AUTH_COMPLETE with length = %d\n", length);
	for (GF_UINT16 i = 0; i < length; i++)
	{
		LOGDEBUG(mTag, "the data of [%d]th bytes is 0x%02x \n", i, data[i]);
	}

	if (data[EVENT_AUTH_COMPLETE_STATUS_OFFSET] != GF_SUCCESS)
	{
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_AUTH_COMPLETE with error!! \n");
		return GF_FAIL;
	}

	if (0x01 == data[EVENT_AUTH_COMPLETE_ENC_ENABLE_OFFSET])
	{
		LOGDEBUG(mTag, "Link Key available 111 !!! \n");
		KeySizeOffset = EVENT_AUTH_COMPLETE_KEYSIZE_OFFSET;
		LTKOffset = EVENT_AUTH_COMPLETE_LTK_OFFSET;
		DIVOffset = EVENT_AUTH_COMPLETE_DIV_OFFSET;
		RandOffset = EVENT_AUTH_COMPLETE_RAND_OFFSET;
	}
	else if (0x01 == data[32])
	{
		LOGDEBUG(mTag, "Link Key available 222!!! \n");
		KeySizeOffset = 33;
		LTKOffset = 34;
		DIVOffset = 50;
		RandOffset = 52;
	}
	else
	{
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_AUTH_COMPLETE with enable = %d!! \n ", data[32]);
		LOGDEBUG(mTag, "NO Link Key available!!! \n");
		return GF_FAIL;
	}

	mKeySize = data[KeySizeOffset];
	LOGDEBUG(mTag, "GF_DEVICE_EVENT_AUTH_COMPLETE with keysize = %d!! \n ", mKeySize);
	/*need to check the length of keysize.*/
	memcpy(mLTK, data + LTKOffset, mKeySize);
	for (GF_UINT16 i = 0; i < mKeySize; i++)
	{
		LOGDEBUG(mTag, "the mLTK of [%d]th bytes is 0x%02x \n", i, mLTK[i]);
	}

	mDIV = data[DIVOffset] + (data[DIVOffset + 1] << 8);
	LOGDEBUG(mTag, "GF_DEVICE_EVENT_AUTH_COMPLETE with mDIV = 0x%02x!! \n ", mDIV);

	memcpy(mRAND, data + RandOffset, 8);
	for (GF_UINT16 i = 0; i < 8; i++)
	{
		LOGDEBUG(mTag, "the mRAND of [%d]th bytes is 0x%02x \n", i, mRAND[i]);
	}
	LOGDEBUG(mTag, "---------->mInterface->Bond start....\n");
	status = mInterface->Bond(mHandle, mLTK, mDIV, mRAND, mKeySize);
	LOGDEBUG(mTag, "---------->mInterface->Bond end....\n");

	return status;
}

GF_STATUS GF_CRemoteDevice::BondComplete(GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS status;
	LOGDEBUG(mTag, "BondComplete with length = %d\n", length);

	status = data[EVENT_BOND_COMPLETE_STATUS_OFFSET];
	if (status == GF_SUCCESS)
	{
		LOGDEBUG(mTag, "BondComplete successful!!!\n");
		//save link key to xml file.
		if (mNeedSaveLTK == GF_TRUE)
		{
			//mDatabase->SaveLinkKey(mKeySize, mLTK, mDIV, mRAND);
			mNeedSaveLTK = GF_FALSE;
		}

		//start to exchange MTU size
		status = mInterface->ExchangeMTUSize(mHandle, LOCAL_GATT_CLIENT_MTU_SIZE);
		if (GF_SUCCESS == status)
		{
			LOGDEBUG(mTag, "Gatt start to exchange MTU size! \n");
			mState = GF_DEVICE_STATE_GATT_PRI_SVC;
		}
		else
		{
			//fail to discovery all primary service, disconnect connection
		}
	}
	else if (status == 0x06) //key missing
	{
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_BOND_COMPLETE fail with status = %d!!!\n", status);
		//Remove information of remote device;
		mDatabase->DeleteLinkKey();
		mDatabase->DeleteGattService();
		mNeedSaveLTK = GF_TRUE;
		mNeedSaveService = GF_TRUE;
		//mInterface->Authenticate(mHandle);
	}
	return status;
}

GF_STATUS GF_CRemoteDevice::ConnectionParameterUpdated(GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS status;
	LOGDEBUG(mTag, "ConnectionParameterUpdated with length = %d\n", length);
	status = data[EVENT_BOND_COMPLETE_STATUS_OFFSET];
	if (status == GF_SUCCESS)
	{
		mConnInternal = data[EVENT_LINK_PARA_UPDATE_INTERVEL_OFFSET] + (data[EVENT_LINK_PARA_UPDATE_INTERVEL_OFFSET] << 8);
		mSlaveLatency = data[EVENT_LINK_PARA_UPDATE_LATENCY_OFFSET] + (data[EVENT_LINK_PARA_UPDATE_LATENCY_OFFSET+1] << 8);
		mConnTimeout = data[EVENT_LINK_PARA_UPDATE_TIMEOUT_OFFSET] + (data[EVENT_LINK_PARA_UPDATE_TIMEOUT_OFFSET+1] << 8);
		LOGDEBUG(mTag, "mConnInternal = 0x%02x \n", mConnInternal);
		LOGDEBUG(mTag, "mSlaveLatency = 0x%02x \n", mSlaveLatency);
		LOGDEBUG(mTag, "mConnTimeout = 0x%02x \n", mConnTimeout);
	}
	return status;
}
GF_STATUS GF_CRemoteDevice::W4SecuStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	LOGDEBUG(mTag, "W4SecuStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST with length = %d\n", length);
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST skip it--------> \n");
			break;
		}

		case GF_DEVICE_EVENT_AUTH_COMPLETE:
		{
			result = AuthenticationComplete(data, length);
			if(result != GF_SUCCESS)
			{
				LOGDEBUG(mTag, "Authentication Failed!!! \n");
			}
			break;
		}

		case GF_DEVICE_EVENT_BOND_COMPLETE:
		{
			result = BondComplete(data, length);
			if(result != GF_SUCCESS)
			{
				LOGDEBUG(mTag, "Bond Failed!!! \n");
			}
			break;
		}
		case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
		{
			ConnectionParameterUpdated(data, length);
			break;
		}

		default:
			break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::W4GattPriSvcStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	LOGDEBUG(mTag, "W4GattPriSvcStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
	case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
	case GF_DEVICE_EVENT_AUTH_COMPLETE:
	case GF_DEVICE_EVENT_BOND_COMPLETE:
		LOGDEBUG(mTag, "W4GattPriSvcStateProcessMessage process error Message = %d \n", event);
		break;

	case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
		{
			ConnectionParameterUpdated(data, length);
			break;
		}
		
		break;

	case GF_DEVICE_EVENT_ATT_EXCHANGE_MTU_MSG:
	{
		GF_UINT8 status = data[EVENT_ATT_EXCHANGE_MTU_MSG_STATUS_OFFSET];
		GF_UINT16 Server_MTU_Size;
		if (status == GF_SUCCESS)
		{
			GF_UINT data_len = data[EVENT_ATT_EXCHANGE_MTU_MSG_LEN_OFFSET];
			if (data_len == 0x02)
			{
				Server_MTU_Size = data[EVENT_ATT_EXCHANGE_MTU_MSG_DATA_OFFSET] + (data[EVENT_ATT_EXCHANGE_MTU_MSG_DATA_OFFSET + 1] << 8);
				LOGDEBUG(mTag, "Server_MTU_Size = 0x%02x \n", Server_MTU_Size);
			}
		}

		mMTUSize = min(Server_MTU_Size, LOCAL_GATT_CLIENT_MTU_SIZE);
		/*Load Gatt Service*/
		LOGDEBUG(mTag, "start to Load GATT Service ... \n");
		if (GF_SUCCESS == mDatabase->LoadService(&mService))
		{
			LOGDEBUG(mTag, "Load Service Successful!! \n");
			mCurrentPrimaryServiceIndex = 0;
			mCurrentCharactericticIndex = 0;
			mCurrentCharactericticDescriptorIndex = 0;
			ProcessCharacteristicConfiguration(mCurrentPrimaryServiceIndex, mCurrentCharactericticIndex, mCurrentCharactericticDescriptorIndex);
			mState = GF_DEVICE_STATE_GATT_READ_CHARC_DESCRIPTOR_VALUE;
			mNeedSaveService = GF_FALSE;
		}
		else
		{
			//start to discaovery all primary service.
			LOGDEBUG(mTag, "start to discaovery all primary service.\n");
			if (GF_SUCCESS == mInterface->DiscoveryAllPrimaryService(mHandle))
			{
				//mState = GF_DEVICE_STATE_GATT_PRI_SVC;
			}
			else
			{
				//fail to discovery all primary service, disconnect connection
				LOGDEBUG(mTag, "DiscoveryAllPrimaryService failed \n");
			}
		}
		
		break;
	}

	case GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG:
	{
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with length = %d\n", length);
		GF_UINT8 status = data[0];
		GF_UINT8 length = data[3];
		GF_UINT8 pair_len = data[4];
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with pair_len = %d\n", pair_len);
		GF_UINT8 num_svc = 1;
		if (pair_len != 0)
		{
			num_svc = (length - 1) / pair_len;
		}
		GF_PUINT8 p = data + 5;

		LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with status = %d\n", status);
		if (GF_ATT_PRODECURE_COMPLETE == status)
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG completeed with service number is %d! \n", mService.mNumOfPrivateService);
			//change state to discovery included service and characteristic.
			GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
			if (NULL != service)
			{
				LOGDEBUG(mTag, "start to discovery %dth primary service!!!\n", mService.mCurrentPriService);
				mInterface->DiscoveryIncludedPrimaryService(mHandle, service->mStartHandle, service->mEndHandle);
				mState = GF_DEVICE_STATE_GATT_INC_SVC;
			}
			else
			{
				LOGDEBUG(mTag, "mService.mCurrentPriService = %d is not available. \n", mService.mCurrentPriService);
			}
		}
		else if (GF_ATT_PROCEDURE_SUCCESS == status)
		{
			if ((length - 1) % pair_len == 0)
			{
				for (GF_UINT8 i = 0; i < num_svc; i++)
				{
					mService.AddPriSvcIntoService(p, pair_len);
					p += pair_len;
				}
			}
			else
			{
				LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong length = %d, pair_len = %d\n", length, pair_len);
			}
		}
		else
		{
			LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with error status! %d\n");
		}
	}
	break;

	default:
		break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::W4GattIncSvcStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	GF_UINT8 status = data[GF_ATT_READ_BY_TYPE_RES_STATUS_OFFSET];
	GF_UINT8 pdu_len;
	GF_UINT8 pair_len;
	GF_UINT8 num_svc;
	LOGDEBUG(mTag, "W4GattIncSvcStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
		case GF_DEVICE_EVENT_AUTH_COMPLETE:
		case GF_DEVICE_EVENT_BOND_COMPLETE:
		case GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG:
			LOGDEBUG(mTag, "W4GattIncSvcStateProcessMessage process error Message = %d \n", event);
			break;

		case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
		{
			ConnectionParameterUpdated(data, length);
			break;
		}

		case GF_DEVICE_EVENT_ATT_ERROR_MSG:
		{
			//get error code
			GF_UINT8 error_code = data[GF_ATT_ERROR_MSG_ERROR_CODE_OFFSET];
			if (GF_ERROR_CODE_ATTR_NOT_FOUND == error_code)
			{
				//no included service found, start to discovery charactistic.
				this->StartDiscoveryCharacteristic();
			}
			break;
		}	
		case GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with length = %d\n, status = 0x%02x", length, status);
			if (GF_ATT_PROCEDURE_SUCCESS == status)
			{
				pdu_len = data[GF_ATT_READ_BY_TYPE_RES_PDU_LEN_OFFSET]; //get pdu length from raw data
				pair_len = data[GF_ATT_READ_BY_TYPE_RES_PAIR_LEN_OFFSET]; //get characteristic struct length from raw data
				GF_PUINT8 p = data + 5;
	
				if (pair_len != 0)
				{
					num_svc = (pdu_len - 1) / pair_len;
				}
				else
				{
					LOGERROR(mTag, "pair_len error!!!");
				}

				GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
				if ((pdu_len - 1) % pair_len == 0)
				{
					for (GF_UINT8 i = 0; i < num_svc; i++)
					{
						service->AddIncSvcIntoPriService(p, pair_len);
						p += pair_len;
					}
				}
				else
				{
					LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with wrong length = %d, pair_len = %d\n", length, pair_len);
				}
				//UUID handle = data[GF_ATT_READ_BY_TYPE_RES_START_HANDLE_OFFSET] + data[GF_ATT_READ_BY_TYPE_RES_HANDLE_OFFSET + 1] << 8;
			}
			else if (GF_ATT_PRODECURE_COMPLETE == status)
			{
				//included service find completeed.
				this->StartDiscoveryCharacteristic();
			}
			else
			{
				LOGERROR(mTag, "included service end with error status!!!");
				//ERROR handle to do...
			}
		}
		default:
			break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::W4GattDisCharcStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	GF_UINT8 status = data[GF_ATT_READ_BY_TYPE_RES_STATUS_OFFSET];
	GF_UINT8 pdu_len;
	GF_UINT8 pair_len;
	LOGDEBUG(mTag, "W4GattDisCharcStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
	case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
	case GF_DEVICE_EVENT_AUTH_COMPLETE:
	case GF_DEVICE_EVENT_BOND_COMPLETE:
		LOGDEBUG(mTag, "W4GattPriSvcStateProcessMessage process error Message = %d \n", event);
		break;

	case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
	{
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_LINK_PARA_UPDATE with length = %d\n", length);
		ConnectionParameterUpdated(data, length);
		break;
	}

	case GF_DEVICE_EVENT_ATT_ERROR_MSG:
	{
		//get error code
		GF_UINT8 error_code = data[GF_ATT_ERROR_MSG_ERROR_CODE_OFFSET];
		if (GF_ERROR_CODE_ATTR_NOT_FOUND == error_code)
		{
			//no characteristic found, start to discovery next primary service.
			GF_CPrimaryService* service;
			if((service = mService.GetNextAvailablePriSvc()) != NULL)
			{
				LOGDEBUG(mTag, "start to discovery %dth primary service!!!\n", mService.mCurrentPriService);
				mInterface->DiscoveryIncludedPrimaryService(mHandle, service->mStartHandle, service->mEndHandle);
				mState = GF_DEVICE_STATE_GATT_INC_SVC;
				return GF_SUCCESS;
			}
		}
		break;
	}

	case GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG:
	{
		pdu_len = data[GF_ATT_READ_BY_TYPE_RES_PDU_LEN_OFFSET]; //get pdu length from raw data
		pair_len = data[GF_ATT_READ_BY_TYPE_RES_PAIR_LEN_OFFSET]; //get characteristic struct length from raw data
		GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
		LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG with status = %d\n", status);
		if (GF_ATT_PRODECURE_COMPLETE == status)
		{
			if (NULL != service)
			{
				LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG completeed with characteristic number is %d! \n", service->mNumOfCharacteristic);

				LOGDEBUG(mTag, "start to read value------>! \n");
				if (service->mNumOfCharacteristic != 0)
				{
					service->mCurrentCharateristic = 0;
					GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
					if (characteristic != NULL)
					{
						if (characteristic->mProperty & GF_ATT_PROPERTY_READ)
						{
							//characteristic is readable
							mInterface->ReadCharacteristicValue(mHandle, characteristic->mValueHandle);
							mState = GF_DEVICE_STATE_GATT_READ_CHARC_VALUE;
						}
						else
						{
							//do not need to read characteristic value. start to read characteristic descriptor if need.
							this->DiscoveryDescriptor();
						}
					}	
				}
				else
				{
					LOGDEBUG(mTag, "no characteristic found, change state to next priservice! \n");
				}
				
				//change state to read characteristic value.
				// to do
			}
			else
			{
				LOGDEBUG(mTag, "mService.mCurrentPriService = %d is not available. \n", mService.mCurrentPriService);
			}
		}
		else if (GF_ATT_PROCEDURE_SUCCESS == status)
		{
			GF_UINT8 num_chars;
			GF_PUINT8 p = data + 5;

			if (pair_len != 0)
			{
				num_chars = (pdu_len - 1) / pair_len;
			}
			else
			{
				LOGERROR(mTag, "pair_len with error!!!");
			}

			LOGDEBUG(mTag, "mService.mCurrentPriService = %d characteristic number = %d. \n", mService.mCurrentPriService, num_chars);
			if ((pdu_len - 1) % pair_len == 0)
			{
				for (GF_UINT8 i = 0; i < num_chars; i++)
				{
					service->AddCharacterIntoPriService(p, pair_len);
					p += pair_len;
				}
			}
			else
			{
				LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with wrong length = %d, pair_len = %d\n", length, pair_len);
			}
		}
		else
		{
			LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_GRP_TYPE_MSG with error status! %d\n");
		}
	}

	default:
		break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::W4GattReadCharcValueStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	GF_UINT8 status = data[GF_ATT_READ_VALUE_RESP_STATUS_OFFSET];
	LOGDEBUG(mTag, "W4GattReadCharcValueStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
		case GF_DEVICE_EVENT_AUTH_COMPLETE:
		case GF_DEVICE_EVENT_BOND_COMPLETE:
			LOGDEBUG(mTag, "W4GattPriSvcStateProcessMessage process error Message = %d \n", event);
			break;

		case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_LINK_PARA_UPDATE with length = %d\n", length);
			ConnectionParameterUpdated(data, length);
			break;
		}

		case GF_DEVICE_EVENT_ATT_ERROR_MSG:
		{
			//get error code
			GF_UINT8 error_code = data[GF_ATT_ERROR_MSG_ERROR_CODE_OFFSET];
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_ERROR_MSG with error code = %d \n", error_code);
			LOGDEBUG(mTag, "read value error, start to discovery discriptor... \n");
			GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
			if (service != NULL)
			{
				GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
				LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG success!!!\n");
				if (characteristic != NULL)
				{
					characteristic->mAttriValue->mLength = 0;
					characteristic->mAttriValue->mData = NULL;
					DiscoveryDescriptor();
				}
			}
		}
		
		case GF_DEVICE_EVENT_ATT_READ_BY_TYPE_MSG:
			break;
			
		/*characteristic value has been read.*/
		case GF_DEVICE_EVENT_ATT_READ_RESP_MSG:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG \n");
			GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
			if (service != NULL)
			{
				GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
				LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG success!!!\n");
				if (characteristic != NULL)
				{
					GF_UINT8 length = data[GF_ATT_READ_VALUE_RESP_LEN_OFFSET];
					LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG with length = %d !!!\n", length);
					if (length < 22)
					{
						characteristic->mAttriValue->mLength = length;
						characteristic->mAttriValue->mData = (GF_PUINT8)malloc(length);
						if (characteristic->mAttriValue->mData != NULL)
						{
							memcpy(characteristic->mAttriValue->mData, data + GF_ATT_READ_VALUE_RESP_DATA_OFFSET, length);
						}
						LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG success!!!\n");
						this->DiscoveryDescriptor();
					}
					else
					{
						mInterface->ReadCharacteristicLongValue(mHandle, characteristic->mValueHandle, 0x00);
						Total = 0;
						memset(Temp_Buffer, 0, TEMP_BUFFER_SIZE);
						LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG too long, use ReadLongValue instand!!!\n");
					}
				}
				else
				{
					LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG characteristic not found!!!\n");
				}
			}
			else
			{
				LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG service not found!!!\n");
			}
			
		}
			break;

		case GF_DEVICE_EVENT_ATT_READ_BLOB_RESP_MSG:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BLOB_RESP_MSG with status %d\n", status);
			GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
			GF_CCharacteristic* characteristic = NULL;
			if (service != NULL)
			{
				characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
			}

			if (status == GF_ATT_PROCEDURE_SUCCESS)
			{
				if (characteristic != NULL)
				{
					GF_UINT8 length = data[GF_ATT_READ_VALUE_RESP_LEN_OFFSET];
					LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BLOB_RESP_MSG with length = %d !!!\n", length);
					memcpy(Temp_Buffer+Total, data + GF_ATT_READ_VALUE_RESP_DATA_OFFSET, length);
					Total += length;
				}
			}
			else if (status == GF_ATT_PRODECURE_COMPLETE)
			{
				if (characteristic != NULL)
				{
					characteristic->mAttriValue->mLength = Total;
					characteristic->mAttriValue->mData = (GF_PUINT8)malloc(Total);
					if (characteristic->mAttriValue->mData != NULL)
					{
						memcpy(characteristic->mAttriValue->mData, Temp_Buffer, Total);
					}
					LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BLOB_RESP_MSG with Total length = %d !!!\n", Total);
					this->DiscoveryDescriptor();
				}
			}
			break;
		}

		case GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG: //charecteristic descriptor
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG with length %d\n", length);
			GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
			if (status == GF_ATT_PROCEDURE_SUCCESS)
			{
				if (service != NULL)
				{
					GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
					LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG success!!!\n");
					if (characteristic != NULL)
					{
						GF_UINT8 len = data[3];
						GF_UINT8 format = data[4];
						GF_UINT8 num_char_des = 0;
						GF_PUINT8 p = data + 5;
						/*16bit UUID*/
						if (format == 1)
						{
							if ((len - 1) % 4 == 0)
							{
								num_char_des = (len - 1) / 4;
								for (GF_UINT8 i = 0; i< num_char_des; i++)
								{
									characteristic->AddDescriptorIntoCharacteristic(p, 4);
									LOGDEBUG(mTag, "UUID of descriptor = 0x%02x!!!\n", (p[2] + (p[3] << 8) ));
									p += 4;
								}
							}
						}
						/*128bit UUID*/
						else if (format == 2)
						{
							if ((len - 1) % 18 == 0)
							{
								num_char_des = (len - 1) / 18;
								for (GF_UINT8 i = 0; i< num_char_des; i++)
								{
									characteristic->AddDescriptorIntoCharacteristic(p, 18);
									p += 18;
								}
							}

						}
						else
						{
							LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG invalid format = %d !!!\n", format);
						}

						LOGDEBUG(mTag, "descriptor number is %d !!!\n", characteristic->mNumOfDescriptor);
					}
					else
					{
						LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG characteristic not found!!!\n");

					}
				}
				else
				{
					LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG service not found!!!\n");
				}
			}
			else if (status == GF_ATT_PRODECURE_COMPLETE)
			{
				LOGDEBUG(mTag, "descriptor discovery finished!!\n");
				NextCharacterateristic();
			}
			else
			{
				LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_BY_INFO_MSG with error status = %d!!!\n", status);
			}
		}

		default:
			break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::W4GattReadCharcDesValueStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	GF_UINT8 status = data[GF_ATT_READ_VALUE_RESP_STATUS_OFFSET];
	LOGDEBUG(mTag, "W4GattReadCharcDesValueStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_SLAVE_SECURY_REQUEST:
		case GF_DEVICE_EVENT_AUTH_COMPLETE:
		case GF_DEVICE_EVENT_BOND_COMPLETE:
			LOGDEBUG(mTag, "W4GattReadCharcDesValueStateProcessMessage process error Message = %d \n", event);
			break;

		case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_LINK_PARA_UPDATE with length = %d\n", length);
			ConnectionParameterUpdated(data, length);
			break;
		}

		/*characteristic value has been read.*/
		case GF_DEVICE_EVENT_ATT_READ_RESP_MSG:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG! \n");
			GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mCurrentPrimaryServiceIndex);
			GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(mCurrentCharactericticIndex);
			GF_CCharacteristicDescriptor* descriptor = characteristic->FindDescriptorByIndex(mCurrentCharactericticDescriptorIndex);
			descriptor->mDataLen = data[GF_ATT_READ_VALUE_RESP_LEN_OFFSET];
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG! with length = %d \n", descriptor->mDataLen);
			descriptor->mData = (GF_PUINT8)malloc(descriptor->mDataLen + 1);
			if (descriptor->mData != NULL)
			{
				memcpy(descriptor->mData, data + GF_ATT_READ_VALUE_RESP_DATA_OFFSET, descriptor->mDataLen);
			}
			mCurrentCharactericticDescriptorIndex += 1;
			if (GF_SUCCESS == ProcessCharacteristicConfiguration(mCurrentPrimaryServiceIndex, mCurrentCharactericticIndex, mCurrentCharactericticDescriptorIndex))
			{
				if (mNeedSaveService == GF_TRUE)
				{
					mDatabase->SaveService(&mService);
				}
				mState = GF_DEVICE_STATE_CONNECTED;
				if (mCallback != NULL)
				{
					GF_UINT8 data[2] = {mHandle&0xFF, ((mHandle&0xFF00)>>8)};
					mCallback->OnEvent(EVENT_MASK_DEVICE_CONNECTED, data, 2);
				}
			}
			break;
		}
			
		case GF_DEVICE_EVENT_ATT_WRITE_MSG:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_ATT_WRITE_MSG! \n");
			mCurrentCharactericticDescriptorIndex += 1;
			if (GF_SUCCESS == ProcessCharacteristicConfiguration(mCurrentPrimaryServiceIndex, mCurrentCharactericticIndex, mCurrentCharactericticDescriptorIndex))
			{
				if (mNeedSaveService == GF_TRUE)
				{
					mDatabase->SaveService(&mService);
					mState = GF_DEVICE_STATE_CONNECTED;
					if (mCallback != NULL)
					{
						GF_UINT8 data[2] = {mHandle&0xFF, ((mHandle&0xFF00)>>8)};
						mCallback->OnEvent(EVENT_MASK_DEVICE_CONNECTED, data, 2);
					}
				}
			}
			break;
		}
			
		default:
			break;
	}
	return result;
}

GF_STATUS GF_CRemoteDevice::DeviceConnectedStateProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	GF_STATUS result = GF_FAIL;
	GF_UINT8 status = data[GF_ATT_READ_VALUE_RESP_STATUS_OFFSET];
	LOGDEBUG(mTag, "DeviceConnectedStateProcessMessage process Message = %d \n", event);
	switch (event)
	{
		case GF_DEVICE_EVENT_LINK_PARA_UPDATE:
		{
			LOGDEBUG(mTag, "GF_DEVICE_EVENT_LINK_PARA_UPDATE with length = %d\n", length);
			ConnectionParameterUpdated(data, length);
			break;
		}

		case GF_DEVICE_EVENT_ATT_EXCHANGE_MTU_MSG:
		{
			GF_UINT8 status = data[EVENT_ATT_EXCHANGE_MTU_MSG_STATUS_OFFSET];
			GF_UINT16 Server_MTU_Size;
			if (status == GF_SUCCESS)
			{
				GF_UINT data_len = data[EVENT_ATT_EXCHANGE_MTU_MSG_LEN_OFFSET];
				if (data_len == 0x02)
				{
					Server_MTU_Size = data[EVENT_ATT_EXCHANGE_MTU_MSG_DATA_OFFSET] + (data[EVENT_ATT_EXCHANGE_MTU_MSG_DATA_OFFSET + 1] << 8);
					LOGDEBUG(mTag, "Server_MTU_Size = 0x%02x \n", Server_MTU_Size);
				}
			}

			mMTUSize = min(Server_MTU_Size, LOCAL_GATT_CLIENT_MTU_SIZE);
		 }
		default:
			break;
	}

	return result;
}

//revice message from AdapterManager and queue message to a list.
GF_STATUS GF_CRemoteDevice::ProcessMessage(GF_DEVICE_EVENT event, GF_PUINT8 data, GF_UINT16 length)
{
	LOGDEBUG(mTag, "----->ProcessMessage event = 0x%x sizeof(MESSAGE) = %d \n", event, sizeof(MESSAGE));
	MESSAGE *message = (MESSAGE *) new GF_UINT8[sizeof(MESSAGE)];
	if(message != NULL)
	{
		message->event = event;
		message->length = length;
		memcpy((void*)(message->data), data, length);
		LOGDEBUG(mTag, "shimeng push msg = 0x%x \n", message);
		EnterCriticalSection(&mMutex);
		mMessage.push_back(message);
		LeaveCriticalSection(&mMutex);
	}
	else
	{
		LOGDEBUG(mTag, "----->ProcessMessage new Message Failed!!! \n");
	}
	return GF_SUCCESS;
}

GF_STATUS GF_CRemoteDevice::NextCharacterateristic()
{
	LOGERROR(mTag, "start to Next characteristic!!!\n");
	GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
	GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);

	if (service->mCurrentCharateristic < (service->mNumOfCharacteristic - 1))
	{
		service->mCurrentCharateristic++;
		/*find the next characteristic*/
		characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
		if (characteristic != NULL)
		{
			if (characteristic->mProperty & GF_ATT_PROPERTY_READ)
			{
				//characteristic is readable
				mInterface->ReadCharacteristicValue(mHandle, characteristic->mValueHandle);
				mState = GF_DEVICE_STATE_GATT_READ_CHARC_VALUE;
			}
			else
			{
				//do not need to read characteristic value. start to read characteristic descriptor if need.
				this->DiscoveryDescriptor();
			}
		}
		else
		{
			LOGERROR(mTag, "find charcteristic error!!!\n");
		}
	}
	/*no more chracteristic exist, move to next private service if exist*/
	else if ((service = mService.GetNextAvailablePriSvc()) != NULL)
	{
		LOGDEBUG(mTag, "start to discovery %dth primary service!!!\n", mService.mCurrentPriService);
		mInterface->DiscoveryIncludedPrimaryService(mHandle, service->mStartHandle, service->mEndHandle);
		mState = GF_DEVICE_STATE_GATT_INC_SVC;
		return GF_SUCCESS;
	}
	/*no more characteristic and no more priamry service exist.*/
	else
	{
		LOGDEBUG(mTag, "no more primary service exist, service discovery process has been finished!!!\n");
		LOGDEBUG(mTag, "start to write CCC value!!!\n");
		mCurrentPrimaryServiceIndex = 0;
		mCurrentCharactericticIndex = 0;
		mCurrentCharactericticDescriptorIndex = 0;
		ProcessCharacteristicConfiguration(mCurrentPrimaryServiceIndex, mCurrentCharactericticIndex, mCurrentCharactericticDescriptorIndex);
		mState = GF_DEVICE_STATE_GATT_READ_CHARC_DESCRIPTOR_VALUE;
	}

	return GF_SUCCESS;
}

GF_STATUS GF_CRemoteDevice::DiscoveryDescriptor()
{
	LOGERROR(mTag, "start to DiscoveryDescriptor!!!\n");
	GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
	GF_CCharacteristic* characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
	GF_UINT16 start_handle = (characteristic->mValueHandle + 1);
	GF_UINT16 end_handle;
	/*if next charcteristic exist, end handle is the start handle of next charcteristic*/
	if (service->mCurrentCharateristic < (service->mNumOfCharacteristic - 1))
	{
		GF_CCharacteristic* next_characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic + 1);
		end_handle = next_characteristic->mHandle - 1;
	}
	/*if next charcteristic do not exist, but next private service exist, end handle is the start handle of next private service*/
	else if (mService.mCurrentPriService < (mService.mNumOfPrivateService - 1))
	{
		GF_CPrimaryService* next_service = mService.FindPriSvcbyIndex(mService.mCurrentPriService + 1);
		end_handle = next_service->mStartHandle - 1;
	}
	else
		/*else, this characteristic is the last one.*/
	{
		end_handle = 0xFFFF;
	}
	LOGDEBUG(mTag, "start_handle = %d, end_handle = %d !!!\n", start_handle, end_handle);

	if (end_handle >= start_handle)
	{
		LOGDEBUG(mTag, "start to find charcteristic descriptor!!!\n");
		mInterface->FindCharacteristicDescriptor(mHandle, start_handle, end_handle);
		mState = GF_DEVICE_STATE_GATT_READ_CHARC_VALUE;
	}
	else
	{
		LOGERROR(mTag, "GF_DEVICE_EVENT_ATT_READ_RESP_MSG no characteristic descriptor exist, move to next characteristic!!!\n");
		if (service->mCurrentCharateristic < (service->mNumOfCharacteristic - 1))
		{
			service->mCurrentCharateristic++;
			/*find the next characteristic*/
			characteristic = service->FindCharacteristicbyIndex(service->mCurrentCharateristic);
			if (characteristic != NULL)
			{
				if (characteristic->mProperty & GF_ATT_PROPERTY_READ)
				{
					//characteristic is readable
					mInterface->ReadCharacteristicValue(mHandle, characteristic->mValueHandle);
					mState = GF_DEVICE_STATE_GATT_READ_CHARC_VALUE;
				}
				else
				{
					//do not need to read characteristic value. start to read characteristic descriptor if need.
					this->DiscoveryDescriptor();
				}
			}
			else
			{
				LOGERROR(mTag, "find charcteristic error!!!\n");
			}
		}
		/*no more chracteristic exist, move to next private service if exist*/
		else if ((service = mService.GetNextAvailablePriSvc()) != NULL)
		{
			LOGDEBUG(mTag, "start to discovery %dth primary service!!!\n", mService.mCurrentPriService);
			mInterface->DiscoveryIncludedPrimaryService(mHandle, service->mStartHandle, service->mEndHandle);
			mState = GF_DEVICE_STATE_GATT_INC_SVC;
			return GF_SUCCESS;
		}
		/*no more characteristic and no more priamry service exist.*/
		else
		{
			LOGDEBUG(mTag, "no more primary service exist, service discovery process has been finished!!!\n");
			LOGDEBUG(mTag, "start to write CCC value!!!\n");
			mCurrentPrimaryServiceIndex = 0;
			mCurrentCharactericticIndex = 0;
			mCurrentCharactericticDescriptorIndex = 0;
			ProcessCharacteristicConfiguration(mCurrentPrimaryServiceIndex, mCurrentCharactericticIndex, mCurrentCharactericticDescriptorIndex);
			mState = GF_DEVICE_STATE_GATT_READ_CHARC_DESCRIPTOR_VALUE;
		}
	}

	return GF_SUCCESS;
}

GF_STATUS GF_CRemoteDevice::StartDiscoveryCharacteristic()
{
	GF_STATUS result;
	GF_CPrimaryService* service = mService.FindPriSvcbyIndex(mService.mCurrentPriService);
	if (NULL != service)
	{
		LOGDEBUG(mTag, "start to find the characteristic of  %dth service. \n", mService.mCurrentPriService);
		if (mInterface != NULL)
		{
			result = mInterface->DiscoveryCharacteristic(mHandle, service->mStartHandle, service->mEndHandle);
			if (result == GF_SUCCESS)
			{
				mState = GF_DEVICE_STATE_GATT_DIS_CHARC;
			}
		}
		else
		{
			//error handling.
		}
	}
	else
	{
		LOGDEBUG(mTag, "mService.mCurrentPriService = %d is not available. \n", mService.mCurrentPriService);
	}

	return result;
}

GF_STATUS GF_CRemoteDevice::ProcessCharacteristicConfiguration(GF_UINT8 currentprisvc, GF_UINT8 currentchrac, GF_UINT8 currentchracdes)
{
	GF_CPrimaryService* service;
	GF_CCharacteristic* characteristic;
	GF_CCharacteristicDescriptor* descriptor;
	GF_UUID uuid;
	GF_UINT8 data[2] = {0x01, 0x00};
	uuid.type = GF_UUID_16;
	uuid.value.uuid16 = 0x2902;
	LOGDEBUG(mTag, "current service = %d, current characteristic = %d current chracteristic descriptor = %d. \n", currentprisvc, currentchrac, currentchracdes);
	while (currentprisvc < mService.mNumOfPrivateService)
	{
		service = mService.FindPriSvcbyIndex(currentprisvc);
		service->mCurrentCharateristic = 0;
		LOGDEBUG(mTag, "%d characteristic in %d th primary service. \n", service->mNumOfCharacteristic, currentprisvc);
		while (currentchrac < service->mNumOfCharacteristic)
		{
			characteristic = service->FindCharacteristicbyIndex(currentchrac);
			LOGDEBUG(mTag, "      %d th characteristic finding... with descriptor num = %d\n", currentchrac, characteristic->mNumOfDescriptor);
			while (currentchracdes < characteristic->mNumOfDescriptor)
			{
				descriptor = characteristic->FindDescriptorByIndex(currentchracdes);
				if (descriptor != NULL)
				{
					LOGDEBUG(mTag, "found!! handle is 0x%04x \n", descriptor->mHandle);
					if (descriptor->mUUID.value.uuid16 == 0x2902)
					{
						mInterface->WriteCharacVlaue(mHandle, descriptor->mHandle, data, 2);
					}
					else
					{
						mInterface->ReadCharacteristicValue(mHandle, descriptor->mHandle);
					}
					mCurrentPrimaryServiceIndex = currentprisvc;
					mCurrentCharactericticIndex = currentchrac;
					mCurrentCharactericticDescriptorIndex = currentchracdes;
					return GF_FAIL;
				}
			}
			
			currentchrac++;
			currentchracdes = 0;
		}
		currentprisvc++;
		currentchrac = 0;
		currentchracdes = 0;
	}
	
	return GF_SUCCESS;
}

GF_STATUS GF_CRemoteDevice::ExchangeMTUSize(GF_UINT16 mtu_size)
{
	if (mInterface != NULL && mState == GF_DEVICE_STATE_CONNECTED)
	{
		mMTUSize = mtu_size;
		return mInterface->ExchangeMTUSize(mHandle, mtu_size);
	}
	else
	{
		return GF_FAIL;
	}
}

GF_STATUS GF_CRemoteDevice::ConnectionParameterUpdateRequest(GF_UINT16 conn_interval_min, GF_UINT16 conn_interval_max, GF_UINT16 slave_latence, GF_UINT16 supervision_timeout)
{
	if (mInterface != NULL && mState == GF_DEVICE_STATE_CONNECTED)
	{
		return mInterface->UpdateConnectionParameter(mHandle, conn_interval_min, conn_interval_max, slave_latence, supervision_timeout);
	}
	else
	{
		return GF_FAIL;
	}
}

GF_STATUS GF_CRemoteDevice::WriteCharacteristicValue(GF_UINT16 attribute_handle, GF_UINT8 data_length, GF_PUINT8 data)
{
	if (mInterface != NULL && mState == GF_DEVICE_STATE_CONNECTED)
	{
		return mInterface->WriteCharacVlaue(mHandle, attribute_handle, data, data_length);
	}
	else
	{
		return GF_FAIL;
	}
}

GF_STATUS GF_CRemoteDevice::ReadCharacteristicValue(GF_UINT16 attribute_handle)
{
	if (mInterface != NULL && mState == GF_DEVICE_STATE_CONNECTED)
	{
		return mInterface->ReadCharacteristicValue(mHandle, attribute_handle);
	}
	else
	{
		return GF_FAIL;
	}
}
