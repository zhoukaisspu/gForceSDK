#include"stdafx.h"

#include"log.h"
#include"npi_cmd.h"

#include <stdio.h>


Log::Log(DWORD com_id, DWORD evt_id)
{
	comThreadID = com_id;
	evtThreadID = evt_id;
}

Log::~Log()
{
}

void Log::Run()
{
	MSG	msg;
	PUINT8	buf;
	UINT8	size;
	while (1) {
		if (GetMessage(&msg, 0, 0, 0)) //get msg from message queue
		{
			if ((msg.message >= LOG_MSG) && (msg.message <= LOG_MSG+0xFF)) {
				buf = (PUINT8)msg.wParam;
				size = (UINT8)msg.lParam;
				switch (buf[0]) {
				case HCI_CMD_PACKET:
					Analyze_TX(buf, size);
					break;
				case HCI_EVENT_PACKET:
					Analyze_RX(buf, size);
					break;
				default:
					printf("LOG Err\n");
					break;
				}
			}
			else {
				printf("Log:Err msg type=%4X !\n", msg.message);
			}
		}
	}
}

void Log::Analyze_TX(PUINT8 buf, UINT8 size)
{
	sCMD *pCmd = (sCMD *)buf;
	SYSTEMTIME sys;
	UINT8 i=0;
	GetLocalTime(&sys);
	printf("<Tx> -");
	printf("%02d:%02d:%02d.%03d\n",sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	printf("-Type			: 0x%02X\n",pCmd->type);
	printf("-opcode			: 0x%04X\n",pCmd->opcode);
	printf("Data Lenth		: 0x%02X\n",pCmd->len);

	Sprintf_DumpTx(buf, size);
	delete buf;
}

void Log::Analyze_RX(PUINT8 buf, UINT8 size)
{
	SYSTEMTIME sys;
	UINT8 i = 0;
	UINT16 event;
	sNpiEvt *pEvt = (sNpiEvt *)buf;

	GetLocalTime(&sys);
	printf("<Rx> -");
	printf("%02d:%02d:%02d.%03d\n", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	printf("-Type			: 0x%02X\n", pEvt->type);
	printf("-EventCode		: 0x%02X\n", pEvt->event_code);
	printf("-Data Length		: 0x%02X\n", pEvt->len);

	if (pEvt->event_code == HCI_LE_EVENT_CODE) {
		Analyze_LE_Hci_Event(buf, size);
	}
	else {
		event = pEvt->event_lo + (pEvt->event_hi << 8);
		printf("Event			: 0x%04X\n", event);
		printf("Status			: 0x%02X\n", pEvt->status);

		switch (event & 0xFF80) {
			case HCI_EXT_LL_EVENT:
				Analyze_LL_Event(buf, size);
				break;
			case HCI_EXT_L2CAP_EVENT:
				Analyze_L2CAP_Event(buf, size);
				break;
			case HCI_EXT_ATT_EVENT:
				Analyze_ATT_Event(buf, size);
				break;
			case HCI_EXT_GATT_EVENT:
				Analyze_GATT_Event(buf, size);
				break;
			case HCI_EXT_GAP_EVENT:
				Analyze_GAP_Event(buf, size);
				break;
			default:
				break;
		}
	}
}

void Log::Sprintf_HexData(PUINT8 pBuf, INT16 len) {
	INT16 i;
	for (i = 0; i < len-1; i++) {
		printf("%02X:",*(pBuf + i));
	}
	printf("%02X\n", *(pBuf + i));
}

void Log::Sprintf_DumpRx(PUINT8 pBuf, INT16 len) {
	INT16 i;
	printf("dump(Rx):\n");
	for (i = 0; i<len; i++) {
		printf("%02X ", *(pBuf + i));
	}
	printf("\n");
	printf("----------------------------------------\n");
}
void Log::Sprintf_DumpTx(PUINT8 pBuf, INT16 len) {
	INT16 i;
	printf("dump(Tx):\n");
	for (i = 0; i<len; i++) {
		printf("%02X ", *(pBuf + i));
	}
	printf("\n");
	printf("----------------------------------------\n");
}

void Log::Analyze_LL_Event(PUINT8 buf, UINT8 size) {
	BOOL del_flag = TRUE;
	sNpiEvt *pEvt = (sNpiEvt *)buf;
	UINT16 event = pEvt->event_lo + (pEvt->event_hi << 8);
	UINT16 opCode = pEvt->data[0] + (pEvt->data[1] << 8);

	switch (event) {
		case HCI_EXT_SET_RX_GAIN_EVENT:
		case HCI_EXT_SET_TX_POWER_EVENT:
		case HCI_EXT_CLK_DIVIDE_ON_HALT_EVENT:
		case HCI_EXT_DECLARE_NV_USAGE_EVENT:
		case HCI_EXT_SET_LOCAL_SUPPORTED_FEATURES_EVENT:
		case HCI_EXT_SET_FAST_TX_RESP_TIME_EVENT:
		case HCI_EXT_MODEM_TEST_TX_EVENT:
		case HCI_EXT_MODEM_HOP_TEST_TX_EVENT:
		case HCI_EXT_MODEM_TEST_RX_EVENT:
		case HCI_EXT_END_MODEM_TEST_EVENT:
		case HCI_EXT_SET_BDADDR_EVENT:
		case HCI_EXT_SET_SCA_EVENT:
		case HCI_EXT_SET_FREQ_TUNE_EVENT:
		case HCI_EXT_SAVE_FREQ_TUNE_EVENT:
		case HCI_EXT_SET_MAX_DTM_TX_POWER_EVENT:
		case HCI_EXT_MAP_PM_IO_PORT_EVENT:
		case HCI_EXT_DISCONNECT_IMMED_EVENT:
		case HCI_EXT_PER_EVENT:
		case HCI_EXT_EXTEND_RF_RANGE_EVENT:
		case HCI_EXT_HALT_DURING_RF_EVENT:
		case HCI_EXT_OVERRIDE_SL_EVENT:
		case HCI_EXT_BUILD_REVISION_EVENT:
		case HCI_EXT_DELAY_SLEEP_EVENT:
		case HCI_EXT_RESET_SYSTEM_EVENT:
		case HCI_EXT_OVERLAPPED_PROCESSING_EVENT:
		case HCI_EXT_NUM_COMPLETED_PKTS_LIMIT_EVENT:
		case HCI_EXT_GET_CONNECTION_INFO_EVENT:
			printf("OpCode			: 0x%04X\n", opCode);
			Sprintf_DumpRx(buf, size);
			/*send status message to com-thread*/
			while (!PostThreadMessage(comThreadID, RX_STATUS_MSG, pEvt->status, 0)) {
				printf("Post RX_STATUS_MSG Err:%d\n", GetLastError());
				Sleep(500);
			}
			/*delete heap buffer ?*/
			delete buf;
			break;

		case HCI_EXT_DECRYPT_EVENT:
			printf("Decrypted Data		: ");
			Sprintf_HexData((PUINT8)pEvt->data, pEvt->len - 3);
			Sprintf_DumpRx(buf, size);
			/*send status message to com-thread*/
			while (!PostThreadMessage(comThreadID, RX_STATUS_MSG, pEvt->status, 0)) {
				printf("Post RX_STATUS_MSG Err:%d\n", GetLastError());
				Sleep(500);
			}
			/*send event message to evt-thread and delete heap in that thread*/
			while (!PostThreadMessage(evtThreadID, HCI_DECRYPT_MSG, (WPARAM)pEvt->data, pEvt->len - 3)) {
				printf("Post HCI_DECRYPT_MSG Err:%d\n", GetLastError());
				Sleep(500);
			}
			break;
		default:
			break;

	}
}

void Log::Analyze_L2CAP_Event(PUINT8 buf, UINT8 size) {
	BOOL del_flag = TRUE;
	sNpiEvt *pEvt = (sNpiEvt *)buf;
	UINT16 event = BUILD_UINT16(pEvt->event_lo, pEvt->event_hi);
	UINT16 msg_type = 0;
	sL2capCmdRejEvt *pcmd_rej;
	switch (event) {
		case L2CAP_COMMAND_REJECT_EVT:
			pcmd_rej = (sL2capCmdRejEvt *)&pEvt->status;
			printf("Handle			: 0x%04X\n", BUILD_UINT16(pcmd_rej->handle_lo, pcmd_rej->handle_hi));
			printf("RejectReason		: 0x%04X\n", BUILD_UINT16(pcmd_rej->reason_lo, pcmd_rej->reason_hi));
			msg_type = L2CAP_COMMAND_REJECT_MSG;
			break;
		case L2CAP_DISCONNECT_RSP_EVT:
			msg_type = L2CAP_DISCONNECT_RSP_MSG;
			break;
		case L2CAP_INFO_RSP_EVT:
			msg_type = L2CAP_INFO_RSP_MSG;
			break;
		case L2CAP_PARAM_UPDATE_RSP_EVT:
			msg_type = L2CAP_PARAM_UPDATE_RSP_MSG;
			break;
		case L2CAP_CONNECT_RSP:
			msg_type = L2CAP_CONNECT_RSP_MSG;
			break;
		case L2CAP_CHANNEL_ESTABLISHED_EVT:
			msg_type = L2CAP_CHANNEL_ESTABLISHED_MSG;
			break;
		case L2CAP_CHANNEL_TERMINATED_EVT:
			msg_type = L2CAP_CHANNEL_TERMINATED_MSG;
			break;
		case L2CAP_OUT_OF_CREDIT_EVT:
			msg_type = L2CAP_OUT_OF_CREDIT_MSG;
			break;
		case L2CAP_PEER_CREDIT_THRESHOLD_EVT:
			msg_type = L2CAP_PEER_CREDIT_THRESHOLD_MSG;
			break;
		case L2CAP_SEND_SDU_DONE_EVT:
			msg_type = L2CAP_SEND_SDU_DONE_MSG;
			break;
		case L2CAP_DATA_EVT:
			msg_type = L2CAP_DATA_MSG;
			break;
		default:
			break;
	}
	Sprintf_DumpRx(buf, size);
	if (msg_type != 0) {
		while (!PostThreadMessage(evtThreadID, msg_type, (WPARAM)&pEvt->status, pEvt->len - 2)) {
			printf("Post L2CAP_MSG Err:%d, Type:0x%x\n", GetLastError(), msg_type);
			Sleep(500);
		}
	}

}

void Log::Analyze_ATT_Event(PUINT8 buf, UINT8 size) {
	BOOL del_flag = TRUE;
	sNpiEvt *pEvt = (sNpiEvt *)buf;
	UINT16 event = BUILD_UINT16(pEvt->event_lo, pEvt->event_hi);
	UINT16 msg_type = 0;
	sAttExMtuRsp *pExMtuEvt;
	switch (event) {
	case ATT_ERROR_EVENT:
		msg_type = ATT_ERROR_MSG;
		break;
	case ATT_EXCHANGE_MTU_EVENT:
		pExMtuEvt = (sAttExMtuRsp *)&pEvt->status;

		printf("connHandle		: 0x%04X\n", BUILD_UINT16(pExMtuEvt->handle_lo, pExMtuEvt->handle_hi));
		printf("PduLen			: 0x%02X\n", pExMtuEvt->pdu_len);
		printf("ServerRxMtu		: 0x%04X\n", BUILD_UINT16(pExMtuEvt->mtu_lo, pExMtuEvt->mtu_hi));
		msg_type = ATT_EXCHANGE_MTU_MSG;
		break;
	case ATT_FIND_INFO_EVENT:
		//printf("connHandle			: 0x%04X\n", BUILD_UINT16(pExMtuEvt->handle_lo, pExMtuEvt->handle_hi));
		//printf("PduLen				: 0x%02X\n", pExMtuEvt->pdu_len);
		msg_type = ATT_FIND_INFO_MSG;
		break;
	case ATT_FIND_BY_TYPE_VALUE_EVENT:
		msg_type = ATT_FIND_BY_TYPE_VALUE_MSG;
		break;
	case ATT_READ_BY_TYPE_EVENT:
		msg_type = ATT_READ_BY_TYPE_MSG;
		break;
	case ATT_READ_EVENT:
		msg_type = ATT_READ_MSG;
		break;
	case ATT_READ_BLOB_EVENT:
		msg_type = ATT_READ_BLOB_MSG;
		break;
	case ATT_READ_MULTI_EVENT:
		msg_type = ATT_READ_MULTI_MSG;
		break;
	case ATT_READ_BY_GRP_TYPE_EVENT:
		msg_type = ATT_READ_BY_GRP_TYPE_MSG;
		break;
	case ATT_WRITE_EVENT:
		msg_type = ATT_WRITE_MSG;
		break;
	case ATT_PREPARE_WRITE_EVENT:
		msg_type = ATT_PREPARE_WRITE_MSG;
		break;
	case ATT_EXECUTE_WRITE_EVENT:
		msg_type = ATT_EXECUTE_WRITE_MSG;
		break;
	case ATT_HANDLE_VALUE_NOTI:
		msg_type = ATT_HANDLE_VALUE_NOTI_MSG;
		break;
	case ATT_HANDLE_VALUE_IND:
		msg_type = ATT_HANDLE_VALUE_IND_MSG;
		break;
	case ATT_HANDLE_VALUE_CFM:
		msg_type = ATT_HANDLE_VALUE_CFM_MSG;
		break;
	case ATT_FLOW_CTRL_VIOLATED_EVENT:
		msg_type = ATT_FLOW_CTRL_VIOLATED_MSG;
		break;
	case ATT_MTU_UPDATED_EVENT:
		msg_type = ATT_MTU_UPDATED_MSG;
		break;
	default:
		break;
	}
	Sprintf_DumpRx(buf, size);
	if (msg_type != 0) {
		while (!PostThreadMessage(evtThreadID, msg_type, (WPARAM)&pEvt->status, pEvt->len - 2)) {
			printf("Post L2CAP_MSG Err:%d, Ttpe:0x%x\n", GetLastError(), msg_type);
			Sleep(500);
		}
	}
}

void Log::Analyze_GATT_Event(PUINT8 buf, UINT8 size) {
	/*Nothing*/
}

void Log::Analyze_GAP_Event(PUINT8 buf, UINT8 size) {
	BOOL del_flag = FALSE;
	sNpiEvt *pEvt = (sNpiEvt *)buf;
	UINT16 event = BUILD_UINT16(pEvt->event_lo, pEvt->event_hi);
	UINT16 opCode = BUILD_UINT16(pEvt->data[0], pEvt->data[1]);
	UINT16 msg_type = 0;
	switch (event) {
		case HCI_EXT_GAP_CMD_STATUS_EVENT:
			printf("OpCode			: 0x%04X\n", opCode);
			printf("Data Length		: 0x%02X\n", pEvt->data[2]);

			/*delete heap buffer ?*/
			if (pEvt->data[0] != 0) {
				/*send event message to evt-thread*/
				msg_type = HCI_GAP_STATUS_MSG;
			}
			else {
				del_flag = TRUE;
			}
			break;
		case HCI_EXT_GAP_DEVICE_INIT_DONE_EVENT:
			msg_type = HCI_EXT_GAP_DEVICE_INIT_DONE_MSG;
			break;
		case HCI_EXT_GAP_DEVICE_DISCOVERY_EVENT:
			msg_type = HCI_EXT_GAP_DEVICE_DISCOVERY_MSG;
			break;
		case HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_EVENT:
			msg_type = HCI_EXT_GAP_ADV_DATA_UPDATE_DONE_MSG;
			break;
		case HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_EVENT:
			msg_type = HCI_EXT_GAP_MAKE_DISCOVERABLE_DONE_MSG;
			break;
		case HCI_EXT_GAP_END_DISCOVERABLE_DONE_EVENT:
			msg_type = HCI_EXT_GAP_END_DISCOVERABLE_DONE_MSG;
			break;
		case HCI_EXT_GAP_LINK_ESTABLISHED_EVENT:
			msg_type = HCI_EXT_GAP_LINK_ESTABLISHED_MSG;
			break;
		case HCI_EXT_GAP_LINK_TERMINATED_EVENT:
			msg_type = HCI_EXT_GAP_LINK_TERMINATED_MSG;
			break;
		case HCI_EXT_GAP_LINK_PARAM_UPDATE_EVENT:
			printf("ConnHandle		: 0x%04X\n", BUILD_UINT16(pEvt->data[0], pEvt->data[1]));
			printf("ConnInterval		: 0x%04X\n", BUILD_UINT16(pEvt->data[2], pEvt->data[3]));
			printf("ConnLatency		: 0x%04X\n", BUILD_UINT16(pEvt->data[4], pEvt->data[5]));
			printf("ConnTimeout		: 0x%04X\n", BUILD_UINT16(pEvt->data[6], pEvt->data[7]));
			msg_type = HCI_EXT_GAP_LINK_PARAM_UPDATE_MSG;
			break;
		case HCI_EXT_GAP_RANDOM_ADDR_CHANGED_EVENT:
			msg_type = HCI_EXT_GAP_RANDOM_ADDR_CHANGED_MSG;
			break;
		case HCI_EXT_GAP_SIGNATURE_UPDATED_EVENT:
			msg_type = HCI_EXT_GAP_SIGNATURE_UPDATED_MSG;
			break;
		case HCI_EXT_GAP_AUTH_COMPLETE_EVENT:
			msg_type = HCI_EXT_GAP_AUTH_COMPLETE_MSG;
			break;
		case HCI_EXT_GAP_PASSKEY_NEEDED_EVENT:
			msg_type = HCI_EXT_GAP_PASSKEY_NEEDED_MSG;
			break;
		case HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_EVENT:
			msg_type = HCI_EXT_GAP_SLAVE_REQUESTED_SECURITY_MSG;
			break;
		case HCI_EXT_GAP_DEVICE_INFO_EVENT:
			msg_type = HCI_EXT_GAP_DEVICE_INFO_MSG;
			break;
		case HCI_EXT_GAP_BOND_COMPLETE_EVENT:
			msg_type = HCI_EXT_GAP_BOND_COMPLETE_MSG;
			break;
		case HCI_EXT_GAP_PAIRING_REQ_EVENT:
			msg_type = HCI_EXT_GAP_PAIRING_REQ_MSG;
			break;
		default:
			break;
	}
	Sprintf_DumpRx(buf, size);
	/*send status message to com-thread*/
	while (!PostThreadMessage(comThreadID, RX_STATUS_MSG, pEvt->status, 0)) {
		printf("Post RX_STATUS_MSG Err:%d\n", GetLastError());
		Sleep(500);
	}

	if (msg_type != 0) {
		/*send event message to evt-thread*/
		while (!PostThreadMessage(evtThreadID, msg_type, (WPARAM)pEvt->data, pEvt->len - 3)) {
			printf("Post GAP EVENT Err:%d, Type:0x%x\n", GetLastError(), msg_type);
			Sleep(500);
		}
	}
	if (del_flag) {
		delete buf;
	}
}

void Log::Analyze_LE_Hci_Event(PUINT8 buf, UINT8 size) {
	BOOL del_flag = FALSE;
	sHciEvt *pEvt = (sHciEvt *)buf;
	UINT16 opCode = BUILD_UINT16(pEvt->op_lo, pEvt->op_hi);
	UINT16 msg_type = 0;
	printf("Packets			: 0x%02X\n", pEvt->packets);
	printf("OpCode			: 0x%04X\n", opCode);
	printf("Status			: 0x%02X\n", pEvt->status);
	switch (opCode) {
		case HCI_LE_CLEAR_WHITE_LIST:
		case HCI_LE_ADD_WHITE_LIST:
		case HCI_LE_REMOVE_WHITE_LIST:
		case HCI_LE_RECEIVER_TEST:
		case HCI_LE_TRANSMITTER_TEST:
			/*delete heap buffer ?*/
			del_flag = TRUE;
			break;
		case HCI_READ_LOCAL_SUPPORTED_FEATURES:
			printf("Feature Set		: ");
			Sprintf_HexData(pEvt->data, FEATURES_LEN);
			msg_type = HCI_READ_LOCAL_SUPPORT_FEAT_MSG;
			break;
		case HCI_READ_BDADDR:
			printf("BD Addr			: ");
			Sprintf_HexData(pEvt->data, BLE_ADDR_LEN);
			msg_type = HCI_READ_BDADDR_MSG;
			break;
		case HCI_READ_RSSI:
			printf("Handle			: 0x%04X\n", BUILD_UINT16(pEvt->data[0], pEvt->data[1]));
			printf("RSSI			: 0x%02X\n", pEvt->data[2]);
			msg_type = HCI_READ_RSSI_MSG;
			break;
		case HCI_LE_SET_EVENT_MASK:
			msg_type = HCI_LE_SET_EVENT_MASK_MSG;
			break;
		case HCI_LE_READ_LOCAL_SUPPORTED_FEATURES:
			printf("Feature Set		: ");
			Sprintf_HexData(pEvt->data, FEATURES_LEN);
			msg_type = HCI_LE_READ_LOCAL_SUPPORT_FEAT_MSG;
			break;
		case HCI_LE_READ_WHITE_LIST_SIZE:
			printf("White List Size		: 0x%02X\n", pEvt->data[0]);
			msg_type = HCI_LE_READ_WHITE_LIST_SIZE_MSG;
			break;
		case HCI_LE_CONNECTION_UPDATE:
			msg_type = HCI_LE_CONNECTION_UPDATE_MSG;
			break;
		case HCI_LE_TEST_END:
			printf("Num Of Packets		: 0x%04X\n", BUILD_UINT16(pEvt->data[0], pEvt->data[1]));
			msg_type = HCI_LE_TEST_END_MSG;
			break;
		case HCI_LE_REMOTE_CONN_PARAM_REQ_REPLY:
			msg_type = HCI_LE_REM_CONN_PARAM_REQ_REP_MSG;
			break;
		case HCI_LE_REMOTE_CONN_PARAM_REQ_NEG_REPLY:
			msg_type = HCI_LE_REM_CONN_PARAM_REQ_NEG_REP_MSG;
			break;
		default:
			break;
	}

	Sprintf_DumpRx(buf, size);
	/*Send status message to Com-thread*/
	while (!PostThreadMessage(comThreadID, RX_STATUS_MSG, pEvt->status, 0)) {
		printf("Post RX_STATUS_MSG Err:%d\n", GetLastError());
		Sleep(500);
	}
	/*Message to user*/
	if (msg_type != 0) {
		/*send event message to evt-thread*/
		while (!PostThreadMessage(evtThreadID, msg_type, (WPARAM)pEvt->data, pEvt->len - 4)) {
			printf("Post HCI LE EVENT Err:%d, Type:0x%x\n", GetLastError(), msg_type);
			Sleep(500);
		}
	}
	if (del_flag) {
		delete buf;
	}
}

size_t LogE(const wchar_t *fmt, ...) {
	va_list argp;
	const size_t BUF_SIZE = 512;
	WCHAR buf[BUF_SIZE];
	size_t len;

	va_start(argp, fmt);

	len = swprintf_s(buf, BUF_SIZE, L"ERROR[%d]: ", GetLastError());
	len += vswprintf_s(buf + len, BUF_SIZE - len, fmt, argp);
	fwprintf_s(stderr, buf);

	va_end(argp);

	return len;
}

size_t LogW(const wchar_t *fmt, ...) {
	va_list argp;
	const size_t BUF_SIZE = 512;
	WCHAR buf[BUF_SIZE];
	size_t len;

	va_start(argp, fmt);

	len = swprintf_s(buf, BUF_SIZE, L"WARNING: ");
	len += vswprintf_s(buf + len, BUF_SIZE - len, fmt, argp);
	fwprintf_s(stdout, buf);

	va_end(argp);

	return len;
}

size_t LogI(const wchar_t *fmt, ...) {
	va_list argp;
	const size_t BUF_SIZE = 512;
	WCHAR buf[BUF_SIZE];
	size_t len;

	va_start(argp, fmt);

	len = swprintf_s(buf, BUF_SIZE, L"INFO: ");
	len += vswprintf_s(buf + len, BUF_SIZE - len, fmt, argp);
	fwprintf_s(stdout, buf);

	va_end(argp);

	return len;
}

UINT16 GetLogMsg(void) {
	static UINT16 msg = LOG_MSG;
	if (msg++ == LOG_MSG + 0xFF) {
		msg = LOG_MSG;
	}
	return msg;
}