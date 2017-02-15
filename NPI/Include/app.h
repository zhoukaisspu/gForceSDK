#pragma once

//define com port number
#define COM_PORT_NUM 3

#define LOCAL_GATT_CLIENT_MTU_SIZE 23

#define STD_OUT
void HciExt_UnitTest(NPI_CMD * handle);
void L2cap_UnitTest(NPI_CMD * handle);
void Att_UnitTest(NPI_CMD * handle);
void Gatt_UnitTest(NPI_CMD * handle);

void Gap_UnitTest(NPI_CMD * handle);
void Util_UnitTest(NPI_CMD * handle);
void Hci_UnitTest(NPI_CMD * handle);
void Test_Init(NPI_CMD * handle, PUINT8 bd_addr);
