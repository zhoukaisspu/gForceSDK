// ConsoleApplication1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include "com.h"
#include "npi_cmd.h"
#include "app.h"

int main(int charc, char* argv[]) {
	/*NPI*/
	NPI_CMD handle(3);//Com:3
	handle.Connect();

	UINT8 bd_addr[BLE_ADDR_LEN] = { 0x35, 0x78, 0x51, 0xf8, 0xe6, 0xa0 };
	//Test_Init(&handle, bd_addr);
	/*------------Hci Extended Command----------*/
	//HciExt_UnitTest(&handle);

	/*-------------L2cap Command----------------*/
	//L2cap_UnitTest(&handle);

	/*-------------Att Command------------------*/
	//Att_UnitTest(&handle);

	/*-------------Gatt Command-----------------*/
	//Gatt_UnitTest(&handle);

	/*-------------Gap Command------------------*/
	Gap_UnitTest(&handle);

	/*-------------HCI Command------------------*/
	//Hci_UnitTest(&handle);

	while (1) {
		Sleep(5000);
		printf("main thread!\n");
	};
}

void HciExt_UnitTest(NPI_CMD *handle) {
	handle->HciExt_SetRxGain(HCI_EXT_RX_GAIN_STD);
	handle->HciExt_SetTxPower(HCI_EXT_TX_POWER_MINUS_6_DBM);
	handle->HciExt_ClkDivOnHalt(HCI_EXT_DISABLE_CLK_DIVIDE_ON_HALT);
	handle->HciExt_DecNvUsage(HCI_EXT_NV_NOT_IN_USE);
	UINT8 nkey[16] = { 0xBF,0x01,0xFB,0x9D,0x4E,0xF3,0xbc,0x36,0xD8,0x74,0xF5,0x39,0x41,0x38,0x68,0x4C };
	UINT8 ndata[16] = { 0x66,0xc6,0xc2,0x27,0x8e,0x3b,0x8e,0x05,0x3e,0x7e,0xa3,0x26,0x52,0x1b,0xad,0x99 };
	handle->HciExt_Decrypt(nkey, ndata);
	UINT8 nfeatures[8] = { 0x01,0,0,0,0,0,0,0 };
	handle->HciExt_SetLocalFeatures(nfeatures);
	handle->HciExt_SetFastTxRespTime(HCI_EXT_DISABLE_FAST_TX_RESP_TIME);
	handle->HciExt_ModemTestTx(HCI_EXT_TX_MODULATED_CARRIER, 38);
	handle->HciExt_SetSCA(0x1100);
}

void L2cap_UnitTest(NPI_CMD *handle) {
	handle->L2CAP_DisconnectReq(64);
	handle->L2CAP_InfoReq(0xFFFE, EXTENDED_FEATURES);
	handle->L2CAP_ConnParamUpdateReq(0xFFFE, 80, 160, 0, 1000);
	handle->L2CAP_ConnectReq(0xFFFE, 1, 1);
	handle->L2CAP_FlowCtrlCredit(64, 10);
	UINT8 l2cap_data[8] = { 0x01,2,3,4,5,6,7,8 };
	handle->L2CAP_Data(64, l2cap_data, 8);
	handle->L2CAP_RegisterPsm(1,23,10,1,1,0);
	handle->L2CAP_DeregisterPsm(1);
}

void Att_UnitTest(NPI_CMD *handle) {
	UINT8 data[8] = { 0x01,2,3,4,5,6,7,8 };
	handle->ATT_ExchangeMTUReq(0xFFFE,23);
	handle->ATT_FindInfoReq(0xFFFE, 1, 65535);
	handle->ATT_FindByTypeValueReq(0xFFFE, 1, 65535, 0, data, 8);
	handle->ATT_ReadByTypeReq(0xFFFE, 1, 65535, data, 2);
	handle->ATT_ReadReq(0xFFFE, 1);
	handle->ATT_ReadBlobReq(0xFFFE, 1, 0);
	handle->ATT_ReadMultiReq(0xFFFE, data, 8);
	handle->ATT_ReadByGroupTypeReq(0xFFFE, 1, 0xFFFF, data, 2);
	handle->ATT_WriteReq(0xFFFE, NPI_NO, NPI_NO, 1, data, 2);
	handle->ATT_PrepareWriteReq(0xFFFE, 1, 0, data, 2);
	handle->ATT_ExecuteWriteReq(0xFFFE, Immediately_Write_All_Pending_Prepared_Values);
}

void Gatt_UnitTest(NPI_CMD *handle) {
	UINT8 data[8] = { 0x01,2,3,4,5,6,7,8 };
	handle->GATT_ExchangeMTU(0xFFFE, 23);
	handle->GATT_DiscAllPrimaryServices(0xFFFE);
	handle->GATT_DiscPrimaryServiceByUUID(0xFFFE, data, 2);
	handle->GATT_FindIncludedServices(0xFFFE,1, 0xFFFF);
	handle->GATT_DiscAllChar(0xFFFE, 1, 0xFFFF);
	handle->GATT_DiscCharByUUID(0xFFFE, 1, 0xFFFF, data, 2);
	handle->GATT_DiscAllCharDesc(0xFFFE, 1, 0xFFFF);
	handle->GATT_ReadCharVal(0xFFFE, 1);
	handle->GATT_ReadUseCharUUID(0xFFFE, 1, 0xFFFF, data, 2);
	handle->GATT_ReadLongCharValue(0xFFFE, 1, 0);
	handle->GATT_ReadMultiCharValues(0xFFFE, data, 8);
	handle->GATT_WriteCharValue(0xFFFE, 1, data, 2);
	handle->GATT_WriteLongCharValue(0xFFFE, 1,0, data, 2);
	sGattWriteVal val[2];
	val[0].value_len = 1;
	val[1].value_len = 1;
	handle->GATT_ReliableWrites (0xFFFE, 2, &val[0]);
	handle->GATT_ReadCharDesc(0xFFFE, 1);
	handle->GATT_ReadLongCharDesc(0xFFFE, 1, 0);
	handle->GATT_WriteCharDesc(0xFFFE, 0, data, 2);
	handle->GATT_WriteLongCharDesc(0xFFFE, 1, 0, data, 2);
	handle->GATT_AddService(PRIMARY_SERVICE, 2, 16);
	handle->GATT_AddAttribute(data, 2, 1);
}

void Gap_UnitTest(NPI_CMD *handle) {
	UINT8 bd_addr[BLE_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };
	UINT8 data[] = { 0,1,2,3,4,5,6,7,8,9 };
	UINT8 irk[16] = { 0 };
	UINT8 csrk[16] = { 0 };
	UINT8 ltk[16] = { 0 };
	UINT8 rand[8] = {1,2,3,4,5,6,7,8};
	UINT32 sign_count = 1;
	handle->GAP_DeviceInit(GAP_CENTRAL_ROLE, 5, irk, csrk, (PUINT8)&sign_count);
	handle->GAP_ConfigDeviceAddr(ADDRTYPE_PUBLIC, bd_addr);
	handle->GAP_DeviceDiscoveryRequest(DEVDISC_MODE_ALL, NPI_ENABLE, NPI_DISABLE);
	handle->GAP_DeviceDiscoveryCancel();
	handle->GAP_MakeDiscoverable(GAP_ADTYPE_ADV_IND, ADDRTYPE_PUBLIC, bd_addr, 7, GAP_FILTER_POLICY_ALL);
	handle->GAP_UpdateAdvertisingData(GAP_ADVERTISEMENT_DATA, 1, data);
	handle->GAP_EndDiscoverable();
	handle->GAP_EstablishLinkRequest(NPI_DISABLE, NPI_DISABLE, ADDRTYPE_PUBLIC, bd_addr);
	handle->GAP_TerminateLinkRequest(0xFFFE);
	sGapAuth auth;
	handle->GAP_Authenticate(0xFFFE, &auth);
	handle->GAP_PasskeyUpdate(0xFFFE, bd_addr);
	uAuthReq authReq;
	handle->GAP_SlaveSecurityRequest(0xFFFE, authReq);
	handle->GAP_Signable(0xFFFE, csrk, (PUINT8)&sign_count);
	handle->GAP_Bond(0xFFFE, NPI_DISABLE, ltk, 4396, rand, 16);
	handle->GAP_TerminateAuth(0xFFFE);
	handle->GAP_UpdateLinkParamReq(0xFFFE, 80, 160, 0, 1000);
	handle->GAP_SetParam(TGAP_GEN_DISC_ADV_MIN, 0);
	handle->GAP_GetParam(TGAP_GEN_DISC_ADV_MIN);
	handle->GAP_ResolvePrivateAddr(irk, bd_addr);
	handle->GAP_SetAdvToken(GAP_ADTYPE_FLAGS, 2, data);
	handle->GAP_RemoveAdvToken(GAP_ADTYPE_FLAGS);
	handle->GAP_UpdateAdvTokens();
}

void Util_UnitTest(NPI_CMD *handle) {
	//Nothing
}

void Hci_UnitTest(NPI_CMD *handle) {
	UINT8 bd_addr[BLE_ADDR_LEN] = { 0, 0, 0, 0, 0, 0 };
	handle->HCI_ReadLocalSupportedFeatures();
	handle->HCI_ReadBdAddr();
	handle->HCI_ReadRSSI(0xFFFE);
	handle->HCI_LEReadLocalSupportedFeatures();
	handle->HCI_LEReadWhiteListSize();
	handle->HCI_LEClearWhiteList();
	handle->HCI_LEAddDeviceToWhiteList(PUBLIC_DEVICE, bd_addr);
	handle->HCI_LERemoveDeviceFromWhiteList(PUBLIC_DEVICE, bd_addr);
	handle->HCI_LEConnectionUpdate(1, 6, 6, 0, 10, 0, 0);
	handle->HCI_LEReceiverTest(0);
	handle->HCI_LETransmitterTest(0,6, bd_addr);
	handle->HCI_LETestEnd();
}

void Test_Init(NPI_CMD *handle, PUINT8 bd_addr) {
	UINT8 irk[16] = { 0 };
	UINT8 csrk[16] = { 0 };
	UINT32 sign_count = 1;
	handle->GAP_DeviceInit(GAP_CENTRAL_ROLE, 5, irk, csrk, (PUINT8)&sign_count);
	handle->GAP_EstablishLinkRequest(NPI_DISABLE, NPI_DISABLE, ADDRTYPE_PUBLIC, bd_addr);
}