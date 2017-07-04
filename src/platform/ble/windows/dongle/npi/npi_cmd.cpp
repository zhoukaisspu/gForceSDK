/*
 * Copyright 2017, OYMotion Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
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
#include"npi_cmd.h"
#include"npi_tl.h"
NPI_CMD::NPI_CMD()
{
}

NPI_CMD::NPI_CMD(UINT8 port): Com(port)
{
}

NPI_CMD::NPI_CMD(UINT8 nPort, DWORD nBaud, UINT8 nParity, UINT8 nByteSize,
                 UINT8 nStopBit) : Com(nPort, nBaud, nParity, nByteSize, nStopBit)
{
}


NPI_CMD::~NPI_CMD()
{
}

/*---------------------------
/*      HCI Extended Command    /
/*-------------------------*/
DWORD NPI_CMD::WaitFor_Msg(void)
{
	//MSG msg;
	DWORD ret = INVALID_RET_STATUS;

	//if (GetMessage(&msg, 0, 0, 0)) { //get msg from message queue
	//      if (RX_STATUS_MSG == msg.message) {
	//              ret = (UINT8)msg.wParam;
	//      } else {
	//              printf("Err msg type:%4X !\n", msg.message);
	//      }
	//}
	return ret;
}

BOOL NPI_CMD::HciExt_SetRxGain(eRxGain rxGain)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_RX_GAIN;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)rxGain;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SetTxPower(eTxPwr txPower)
{
	BOOL ret = INVALID_RET_STATUS;
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_TX_POWER;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)txPower;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_ClkDivOnHalt(eClkDivOnHalt ctrl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_CLK_DIVIDE_ON_HALT;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)ctrl;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_DecNvUsage(eNvUsage ctrl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_DECLARE_NV_USAGE;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)ctrl;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_Decrypt(PUINT8 pkey, PUINT8 pdata)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x20];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_DECRYPT;
	cmd->len = 0x20;
	memcpy(&cmd->data[0], pkey, 0x10);
	memcpy(&cmd->data[16], pdata, 0x10);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SetLocalFeatures(PUINT8 ctrl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x08];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_LOCAL_SUPPORTED_FEATURES;
	cmd->len = 0x08;
	memcpy(&cmd->data[0], ctrl, cmd->len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SetFastTxRespTime(eFastTxRespTime ctrl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_FAST_TX_RESP_TIME;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)ctrl;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_ModemTestTx(eCwModem cw, UINT8 channel)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_MODEM_TEST_TX;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)cw;
	cmd->data[1] = (UINT8)channel;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_ModemHopTestTx(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_MODEM_HOP_TEST_TX;
	cmd->len = 0x00;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_ModemTestRx(UINT8 channel)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_MODEM_TEST_RX;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)channel;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_EndModemTest(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_END_MODEM_TEST;
	cmd->len = 0x00;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SetBdAddr(PUINT8 addr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_BDADDR;
	cmd->len = 0x06;
	memcpy(&cmd->data[0], addr, cmd->len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SetSCA(UINT16 sca)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_SCA;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(sca & 0x00FF);
	cmd->data[1] = (UINT8)((sca >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SetFreqTune(eFreqTune tune)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_FREQ_TUNE;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)tune;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HciExt_SaveFreqTune(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SAVE_FREQ_TUNE;
	cmd->len = 0x00;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_SetMaxDtmTxPower(eTxPwr tx_pwr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_SET_MAX_DTM_TX_POWER;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)tx_pwr;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_MapPmIoPort(ePmloPort port, ePmloPortPin port_pin)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_MAP_PM_IO_PORT;
	cmd->len = 0x02;
	cmd->data[0] = port;
	cmd->data[1] = port_pin;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_DisconnectImmed(UINT16 handle)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_DISCONNECT_IMMED;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_PER(UINT16 handle, ePerTestCmd perCmd)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_PER;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)perCmd;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_ExtendRfRange(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_EXTEND_RF_RANGE;
	cmd->len = 0x00;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_HaltDuringRf(eHaltDurRF halt)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_HALT_DURING_RF;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)halt;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_OverrideSL(eOverrideSL state)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_OVERRIDE_SL;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)state;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_BuildRevision(eBuildRevMode mode, UINT16 num)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_BUILD_REVISION;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)mode;
	cmd->data[1] = (UINT8)(num & 0x00FF);
	cmd->data[2] = (UINT8)((num >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_DelaySleep(UINT16 delay)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_DELAY_SLEEP;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(delay & 0x00FF);
	cmd->data[1] = (UINT8)((delay >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_ResetSystem(eResetType type)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_RESET_SYSTEM;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)type;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_OverlappedProcessing(eEnDisMode mode)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_OVERLAPPED_PROCESSING;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)mode;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCIExt_NumComplPktsLimit(UINT8 limit, eEnDisMode mode)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_EXT_OVERLAPPED_PROCESSING;
	cmd->len = 0x01;
	cmd->data[0] = limit;
	cmd->data[1] = (UINT8)mode;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

/*---------------------------
/*      L2CAP Command   /
/*-------------------------*/
BOOL NPI_CMD::L2CAP_DisconnectReq(UINT16 cid)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_DISCON_REQ;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(cid & 0x00FF);
	cmd->data[1] = (UINT8)((cid >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_InfoReq(UINT16 handle, eInfoType type)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_INFO_REQ;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	cmd->data[2] = (UINT16)type & 0x00FF;
	cmd->data[3] = (UINT16)type & 0xFF00;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_ConnParamUpdateReq(UINT16 handle, UINT16 intervalMin,
                                       UINT16 intervalMax, UINT16 slaveLatency, UINT16 timeout)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x0A];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_CON_PAR_UPDATE;
	cmd->len = 0x0A;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(intervalMin & 0x00FF);
	cmd->data[3] = (UINT8)((intervalMin >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(intervalMax & 0x00FF);
	cmd->data[5] = (UINT8)((intervalMax >> 8) & 0x00FF);
	cmd->data[6] = (UINT8)(slaveLatency & 0x00FF);
	cmd->data[7] = (UINT8)((slaveLatency >> 8) & 0x00FF);
	cmd->data[8] = (UINT8)(timeout & 0x00FF);
	cmd->data[9] = (UINT8)((timeout >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_ConnectReq(UINT16 handle, UINT16 PSM, UINT16 peerPSM)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_CONNECT_REQ;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(PSM & 0x00FF);
	cmd->data[3] = (UINT8)((PSM >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(peerPSM & 0x00FF);
	cmd->data[5] = (UINT8)((peerPSM >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_FlowCtrlCredit(UINT16 CID, UINT16 peerCredits)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_FLOW_CTRL_CRT;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(CID & 0x00FF);
	cmd->data[1] = (UINT8)((CID >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(peerCredits & 0x00FF);
	cmd->data[3] = (UINT8)((peerCredits >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_Data(UINT16 CID, PUINT8 pSDU, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + sizeof(CID) + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_DATA;
	cmd->len = sizeof(CID) + len;
	cmd->data[0] = (UINT8)(CID & 0x00FF);
	cmd->data[1] = (UINT8)((CID >> 8) & 0x00FF);
	memcpy(&cmd->data[2], pSDU, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_RegisterPsm(UINT16 psm, UINT16 mtu,
                                UINT16 InitialPeerCrs,
                                UINT16 PeerCrThreshld, UINT8 MaxNumChannels, UINT8 SecurityVrfctn)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x0A];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_REG_PSM;
	cmd->len = 0x0A;
	cmd->data[0] = (UINT8)(psm & 0x00FF);
	cmd->data[1] = (UINT8)((psm >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(mtu & 0x00FF);
	cmd->data[3] = (UINT8)((mtu >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(InitialPeerCrs & 0x00FF);
	cmd->data[5] = (UINT8)((InitialPeerCrs >> 8) & 0x00FF);
	cmd->data[6] = (UINT8)(PeerCrThreshld & 0x00FF);
	cmd->data[7] = (UINT8)((PeerCrThreshld >> 8) & 0x00FF);
	cmd->data[8] = (UINT8)MaxNumChannels;
	cmd->data[9] = (UINT8)SecurityVrfctn;
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_DeregisterPsm(UINT16 psm)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_DEREG_PSM;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(psm & 0x00FF);
	cmd->data[1] = (UINT8)((psm >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::L2CAP_PsmChannels(UINT16 psm)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = L2CAP_PSM_CHANNEL;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(psm & 0x00FF);
	cmd->data[1] = (UINT8)((psm >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}


/*---------------------------
/*      ATT Command     /
/*-------------------------*/
BOOL NPI_CMD::ATT_ExchangeMTUReq(UINT16 con_hdl, UINT16 mtu)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_EXC_MTU_REQ;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(mtu & 0x00FF);
	cmd->data[3] = (UINT8)((mtu >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_FindInfoReq(UINT16 con_hdl, UINT16 sta_hdl,
                              UINT16 end_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_FIND_INFO_REQ;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_FindByTypeValueReq(UINT16 con_hdl, UINT16 sta_hdl,
                                     UINT16 end_hdl,
                                     UINT16 type, PUINT8 pvalue, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x08 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_FIND_BY_TYPE;
	cmd->len = 0x08 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);
	cmd->data[6] = (UINT8)(type & 0x00FF);
	cmd->data[7] = (UINT8)((type >> 8) & 0x00FF);
	memcpy(&cmd->data[8], pvalue, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_ReadByTypeReq(UINT16 con_hdl, UINT16 sta_hdl,
                                UINT16 end_hdl,
                                PUINT8 pvalue, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_READ_BY_TYPE_REQ;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[6], pvalue, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_ReadReq(UINT16 con_hdl, UINT16 att_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_READ_REQ;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((att_hdl >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_ReadBlobReq(UINT16 con_hdl, UINT16 att_hdl,
                              UINT16 offset)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_READ_REQ;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((att_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(offset & 0x00FF);
	cmd->data[5] = (UINT8)((offset >> 8) & 0x00FF);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_ReadMultiReq(UINT16 con_hdl, PUINT8 att_hdl, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_READ_MULTI_REQ;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[2], (PUINT8)att_hdl, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_ReadByGroupTypeReq(UINT16 con_hdl, UINT16 sta_hdl,
                                     UINT16 end_hdl,
                                     PUINT8 pvalue, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_READ_BY_GROUP_TYPE_REQ;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[6], pvalue, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_WriteReq(UINT16 con_hdl, eYesNoMode sig,
                           eYesNoMode command, UINT16 att_hdl,
                           PUINT8 pvalue, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_WRITE_REQ;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)sig;
	cmd->data[3] = (UINT8)command;
	cmd->data[4] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((att_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[6], pvalue, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_PrepareWriteReq(UINT16 con_hdl, UINT16 att_hdl,
                                  UINT16 offset,
                                  PUINT8 pvalue, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_PREP_WRITE_REQ;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((att_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(offset & 0x00FF);
	cmd->data[5] = (UINT8)((offset >> 8) & 0x00FF);
	memcpy(&cmd->data[6], pvalue, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::ATT_ExecuteWriteReq(UINT16 con_hdl, eAttExeWrFlag flag)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = ATT_EXE_WRITE_REQ;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)flag;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

/*---------------------------
/*      GATT Command    /
/*-------------------------*/
BOOL NPI_CMD::GATT_ExchangeMTU(UINT16 con_hdl, UINT16 mtu)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_EXCHANGE_MTU;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(mtu & 0x00FF);
	cmd->data[3] = (UINT8)((mtu >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_DiscAllPrimaryServices(UINT16 con_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_DISC_ALL_PRI_SERV;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_DiscPrimaryServiceByUUID(UINT16 con_hdl, PUINT8 buf,
                UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_DISC_PRI_SERV_BY_UUID;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[2], buf, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_FindIncludedServices(UINT16 con_hdl, UINT16 sta_hdl,
                                        UINT16 end_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_FIND_INC_SERV;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_DiscAllChar(UINT16 con_hdl, UINT16 sta_hdl,
                               UINT16 end_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_DISC_ALL_CHAR;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_DiscCharByUUID(UINT16 con_hdl, UINT16 sta_hdl,
                                  UINT16 end_hdl, PUINT8 uuid, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_DISC_CHAR_BY_UUID;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[6], uuid, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_DiscAllCharDesc(UINT16 con_hdl, UINT16 sta_hdl,
                                   UINT16 end_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_DISC_ALL_CHAR_DESC;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReadCharVal(UINT16 con_hdl, UINT16 hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_READ_CHAR_VAL;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(hdl & 0x00FF);
	cmd->data[3] = (UINT8)((hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReadUseCharUUID(UINT16 con_hdl, UINT16 sta_hdl,
                                   UINT16 end_hdl, PUINT8 uuid, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_READ_USE_CHAR_UUID;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(sta_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((sta_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(end_hdl & 0x00FF);
	cmd->data[5] = (UINT8)((end_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[6], uuid, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReadLongCharValue(UINT16 con_hdl, UINT16 hdl,
                                     UINT16 offset)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_READ_LONG_CHAR_VAL;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(hdl & 0x00FF);
	cmd->data[3] = (UINT8)((hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(offset & 0x00FF);
	cmd->data[5] = (UINT8)((offset >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReadMultiCharValues(UINT16 con_hdl, PUINT8 att_hdl,
                                       UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_READ_MULTI_CHAR_VAL;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[2], (PUINT8)att_hdl, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_WriteCharValue(UINT16 con_hdl, UINT16 att_hdl,
                                  PUINT8 data, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_WRITE_CHAR_VAL;
	cmd->len = 0x04 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((att_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[4], data, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}


BOOL NPI_CMD::GATT_WriteLongCharValue(UINT16 con_hdl, UINT16 att_hdl,
                                      UINT16 offset, PUINT8 data, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_WRITE_LONG_CHAR_VAL;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((att_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(offset & 0x00FF);
	cmd->data[5] = (UINT8)((offset >> 8) & 0x00FF);
	memcpy(&cmd->data[6], data, len);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReliableWrites(UINT16 con_hdl, UINT8 num_req,
                                  sGattWriteVal* pval)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	UINT8 i;
	UINT8 pack_len;
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_WRITE_LONG_CHAR_VAL;
	cmd->len = 0x03;
	for (i = 0; i < num_req; i++) {
		pack_len = (pval + i)->value_len + 5;
		memcpy(&cmd->data[cmd->len], pval + i, pack_len);
		cmd->len += pack_len;
		if (cmd->len > MAX_TX_SIZE - 3) {
			return INVALIDPARAMETER;
		}
	}
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = num_req;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReadCharDesc(UINT16 con_hdl, UINT16 hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_READ_CHAR_DESC;
	cmd->len = 0x04;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(hdl & 0x00FF);
	cmd->data[3] = (UINT8)((hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_ReadLongCharDesc(UINT16 con_hdl, UINT16 hdl,
                                    UINT16 offset)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_READ_CHAR_DESC;
	cmd->len = 0x06;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(hdl & 0x00FF);
	cmd->data[3] = (UINT8)((hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(offset & 0x00FF);
	cmd->data[5] = (UINT8)((offset >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_WriteCharDesc(UINT16 con_hdl, UINT16 offset,
                                 PUINT8 val, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x04 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_WRITE_CHAR_DESC;
	cmd->len = 0x04 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(offset & 0x00FF);
	cmd->data[3] = (UINT8)((offset >> 8) & 0x00FF);
	memcpy(&cmd->data[4], val, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_WriteLongCharDesc(UINT16 con_hdl, UINT16 att_hdl,
                                     UINT16 offset, PUINT8 val, UINT8 len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x06 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_WRITE_LONG_CHAR_DESC;
	cmd->len = 0x06 + len;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(att_hdl & 0x00FF);
	cmd->data[3] = (UINT8)((att_hdl >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(offset & 0x00FF);
	cmd->data[5] = (UINT8)((offset >> 8) & 0x00FF);
	memcpy(&cmd->data[6], val, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_AddService(sGattServiceUUID uuid, UINT16 num_attrs,
                              UINT8 enc_keySize)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x05];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_ADD_SERVICE;
	cmd->len = 0x05;
	cmd->data[0] = (UINT8)(uuid & 0x00FF);
	cmd->data[1] = (UINT8)((uuid >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(num_attrs & 0x00FF);
	cmd->data[3] = (UINT8)((num_attrs >> 8) & 0x00FF);
	cmd->data[4] = enc_keySize;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_DelService(UINT16 handle)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_DEL_SERVICE;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GATT_AddAttribute(PUINT8 uuid, UINT8 len, UINT8 permission)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + len + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GATT_ADD_ATTRIBUTE;
	cmd->len = len + 0x01;
	memcpy(&cmd->data[0], uuid, len);
	cmd->data[len] = permission;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

/*---------------------------
/*      GAP Command     /
/*-------------------------*/
BOOL NPI_CMD::GAP_DeviceInit(gapRole_t role, UINT8 max_scanRes, PUINT8 irk,
                             PUINT8 csrk, PUINT8 sign_count)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x26];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_DEVICE_INIT;
	cmd->len = 0x26;
	cmd->data[0] = role.data;
	cmd->data[1] = max_scanRes;
	memcpy(&cmd->data[2], irk, IRK_LEN);
	memcpy(&cmd->data[18], csrk, CSRK_LEN);
	memcpy(&cmd->data[34], sign_count, SIGN_COUNT_LEN);
	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_ConfigDeviceAddr(eGapAddrType addr_type, PUINT8 addr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x07];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_CONFIG_DEVICE_ADDR;
	cmd->len = 0x07;
	cmd->data[0] = (UINT8)addr_type;
	memcpy(&cmd->data[1], addr, BLE_ADDR_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_DeviceDiscoveryRequest(eGapDiscMode disc_mdoe,
                eEnDisMode act_scan, eEnDisMode white_list)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_DEVICE_DISCOVERY_REQ;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)disc_mdoe;
	cmd->data[1] = (UINT8)act_scan;
	cmd->data[2] = (UINT8)white_list;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_DeviceDiscoveryCancel(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_DEVICE_DISCOVERY_CANCEL;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

/*
*       chl_map:Bit mask
*       #define ADV_CHANNEL_37          0x01
*       #define ADV_CHANNEL_38          0x02
*       #define ADV_CHANNEL_39          0x04
*       #define ADV_CHANNEL_ALL         (ADV_CHANNEL_37 | ADV_CHANNEL_38 | ADV_CHANNEL_39)
*/
BOOL NPI_CMD::GAP_MakeDiscoverable(eGapEventType evt_type,
                                   eGapAddrType addr_type, PUINT8 addr, UINT8 chl_map, eGapFilterPol filter_pol)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x0A];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_MAKE_DISCOVERABLE;
	cmd->len = 0x0A;
	cmd->data[0] = (UINT8)evt_type;
	cmd->data[1] = (UINT8)addr_type;
	memcpy(&cmd->data[2], addr, BLE_ADDR_LEN);
	cmd->data[8] = (UINT8)chl_map;
	cmd->data[9] = (UINT8)filter_pol;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_UpdateAdvertisingData(eGapAdvDataType adv_type,
                                        UINT8 len, PUINT8 adv_Data)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_UPDATE_ADV_DATA;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)adv_type;
	cmd->data[1] = (UINT8)len;
	memcpy(&cmd->data[2], adv_Data, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_EndDiscoverable(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_END_DISCOVERABLE;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_EstablishLinkRequest(eEnDisMode high_duty,
                                       eEnDisMode white_list, eGapAddrType addr_type, PUINT8 addr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x09];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_ESTABLISH_LINK_REQ;
	cmd->len = 0x09;
	cmd->data[0] = (UINT8)high_duty;
	cmd->data[1] = (UINT8)white_list;
	cmd->data[2] = (UINT8)addr_type;
	memcpy(&cmd->data[3], addr, BLE_ADDR_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_TerminateLinkRequest(UINT16 handle)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_TERMINATE_LINK_REQ;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	cmd->data[2] = LL_STATUS_ERROR_PEER_TERM;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_Authenticate(UINT16 con_hdl, sGapAuth* pAuth)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x1D];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_AUTHENTIC_REQ;
	cmd->len = 0x1D;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(pAuth->sec_ioCaps);
	cmd->data[3] = (UINT8)(pAuth->sec_oobFlag);
	memcpy(&cmd->data[4], pAuth->sec_oobData, OOB_DATA_LEN);
	cmd->data[20] = (UINT8)(pAuth->sec_authReq.oper);
	cmd->data[21] = (UINT8)(pAuth->sec_maxEncKeySize);
	cmd->data[22] = (UINT8)(pAuth->sec_keyDist.oper);
	cmd->data[23] = (UINT8)(pAuth->pair_en);
	cmd->data[24] = (UINT8)(pAuth->pair_ioCaps);
	cmd->data[25] = (UINT8)(pAuth->pair_oobFlag);
	cmd->data[26] = (UINT8)(pAuth->pair_authReq.oper);
	cmd->data[27] = (UINT8)(pAuth->pair_maxEncKeySize);
	cmd->data[28] = (UINT8)(pAuth->pair_keyDist.oper);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_PasskeyUpdate(UINT16 con_hdl, PUINT8 key)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x08];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_PASSKEY_UPDATE;
	cmd->len = 0x08;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[2], key, PASSKEY_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_SlaveSecurityRequest(UINT16 con_hdl, uAuthReq auth)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_SLAVE_SEC_REQ;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(auth.oper);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_Signable(UINT16 con_hdl, PUINT8 csrk, PUINT8 sign_count)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x17];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_SIGNABLE;
	cmd->len = 0x17;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	memcpy(&cmd->data[2], csrk, CSRK_LEN);
	memcpy(&cmd->data[18], sign_count, SIGN_COUNT_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_Bond(UINT16 con_hdl, eEnDisMode auth, PUINT8 ltk,
                       UINT16 div, PUINT8 rand, UINT8 ltk_size)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x1E];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_BOND;
	cmd->len = 0x1E;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)auth;
	memcpy(&cmd->data[3], ltk, LTK_LEN);
	cmd->data[19] = (UINT8)(div & 0x00FF);
	cmd->data[20] = (UINT8)((div >> 8) & 0x00FF);
	memcpy(&cmd->data[21], rand, LTK_RAND_PAIR_LEN);
	cmd->data[29] = (UINT8)ltk_size;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_TerminateAuth(UINT16 con_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_TERMINATE_AUTH;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)BLE_SUCCESS;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_UpdateLinkParamReq(UINT16 con_hdl, UINT16 int_min,
                                     UINT16 int_max, UINT16 latency, UINT16 timeout)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x0A];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_UPDATE_LINK_PARA_REQ;
	cmd->len = 0x0A;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(int_min & 0x00FF);
	cmd->data[3] = (UINT8)((int_min >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(int_max & 0x00FF);
	cmd->data[5] = (UINT8)((int_max >> 8) & 0x00FF);
	cmd->data[6] = (UINT8)(latency & 0x00FF);
	cmd->data[7] = (UINT8)((latency >> 8) & 0x00FF);
	cmd->data[8] = (UINT8)(timeout & 0x00FF);
	cmd->data[9] = (UINT8)((timeout >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_SetParam(eGapParamIDs paraId, UINT16 val)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x03];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_SET_PARAM;
	cmd->len = 0x03;
	cmd->data[0] = (UINT8)paraId;
	cmd->data[1] = (UINT8)(val & 0x00FF);
	cmd->data[2] = (UINT8)((val >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_GetParam(eGapParamIDs paraId)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_GET_PARAM;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)paraId;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_ResolvePrivateAddr(PUINT8 irk, PUINT8 addr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x16];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_RESOLVE_PRI_ADDR;
	cmd->len = 0x16;
	memcpy(&cmd->data[0], irk, IRK_LEN);
	memcpy(&cmd->data[16], addr, BLE_ADDR_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_SetAdvToken(eGapAdvTokenType type, UINT8 len,
                              PUINT8 adv_data)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_SET_ADV_TOKEN;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)type;
	cmd->data[1] = (UINT8)len;
	memcpy(&cmd->data[2], adv_data, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_RemoveAdvToken(eGapAdvTokenType type)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_REMOVE_ADV_TOKEN;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)type;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_UpdateAdvTokens(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_UPDATE_ADV_TOKEN;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_BondSetParam(UINT8 paramId, UINT8 len, PUINT16 value)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_BOND_SET_PARAM;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)paramId;
	cmd->data[1] = (UINT8)len;
	memcpy(&cmd->data[2], value, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::GAP_BondGetParam(UINT8 paramId)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = GAP_BOND_GET_PARAM;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)paramId;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}
/*---------------------------
/*              UTIL  Command           /
/*-------------------------*/
BOOL NPI_CMD::UTIL_Reset(eResetType type)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = UTIL_RESET;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)type;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::UTIL_NVRead(UINT8 nv_id, UINT8 data_len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = UTIL_NV_READ;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)nv_id;
	cmd->data[1] = (UINT8)data_len;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::UTIL_NVWrite(UINT8 nv_id, UINT8 data_len, PUINT8 data)
{
	/*This command may invalidate the image, be careful*/
	return BLE_SUCCESS;
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + data_len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = UTIL_NV_WRITE;
	cmd->len = 0x02 + data_len;
	cmd->data[0] = (UINT8)nv_id;
	cmd->data[1] = (UINT8)data_len;
	memcpy(&cmd->data[2], data, data_len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

/*---------------------------
/*              HCI  Command        /
/*-------------------------*/
BOOL NPI_CMD::HCI_ReadLocalSupportedFeatures(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_READ_LOCAL_SUPPORTED_FEATURES;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_ReadBdAddr(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_READ_BDADDR;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_ReadRSSI(UINT16 con_hdl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_READ_RSSI;
	cmd->len = 0x02;
	cmd->data[0] = (UINT8)(con_hdl & 0x00FF);
	cmd->data[1] = (UINT8)((con_hdl >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LEReadLocalSupportedFeatures(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_READ_LOCAL_SUPPORTED_FEATURES;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LEReadWhiteListSize(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_READ_WHITE_LIST_SIZE;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LEClearWhiteList(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_CLEAR_WHITE_LIST;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LEAddDeviceToWhiteList(eHciAddrType type, PUINT8 addr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x07];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_ADD_WHITE_LIST;
	cmd->len = 0x07;
	cmd->data[0] = (UINT8)type;
	memcpy(&cmd->data[1], addr, BLE_ADDR_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LERemoveDeviceFromWhiteList(eHciAddrType type,
                PUINT8 addr)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x07];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_REMOVE_WHITE_LIST;
	cmd->len = 0x07;
	cmd->data[0] = (UINT8)type;
	memcpy(&cmd->data[1], addr, BLE_ADDR_LEN);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LEConnectionUpdate(UINT16 handle, UINT16 int_min,
                                     UINT16 int_max,
                                     UINT16 latency, UINT16 timeout, UINT16 min_len, UINT16 max_len)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x07];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_CONNECTION_UPDATE;
	cmd->len = 0x07;
	cmd->len = 0x0E;
	cmd->data[0] = (UINT8)(handle & 0x00FF);
	cmd->data[1] = (UINT8)((handle >> 8) & 0x00FF);
	cmd->data[2] = (UINT8)(int_min & 0x00FF);
	cmd->data[3] = (UINT8)((int_min >> 8) & 0x00FF);
	cmd->data[4] = (UINT8)(int_max & 0x00FF);
	cmd->data[5] = (UINT8)((int_max >> 8) & 0x00FF);
	cmd->data[6] = (UINT8)(latency & 0x00FF);
	cmd->data[7] = (UINT8)((latency >> 8) & 0x00FF);
	cmd->data[8] = (UINT8)(timeout & 0x00FF);
	cmd->data[9] = (UINT8)((timeout >> 8) & 0x00FF);
	cmd->data[10] = (UINT8)(min_len & 0x00FF);
	cmd->data[11] = (UINT8)((min_len >> 8) & 0x00FF);
	cmd->data[12] = (UINT8)(max_len & 0x00FF);
	cmd->data[13] = (UINT8)((max_len >> 8) & 0x00FF);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LEReceiverTest(UINT8 rx_chl)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x01];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_RECEIVER_TEST;
	cmd->len = 0x01;
	cmd->data[0] = (UINT8)rx_chl;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LETransmitterTest(UINT8 tx_chl, UINT8 len, PUINT8 data)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x02 + len];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_TRANSMITTER_TEST;
	cmd->len = 0x02 + len;
	cmd->data[0] = (UINT8)tx_chl;
	cmd->data[1] = (UINT8)len;
	memcpy(&cmd->data[2], data, len);

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

BOOL NPI_CMD::HCI_LETestEnd(void)
{
	sCMD* cmd = (sCMD*)new UINT8[CMD_HEAD_LEN + 0x00];
	cmd->type = HCI_CMD_PACKET;
	/*Opcode*/
	cmd->opcode = HCI_LE_TEST_END;
	cmd->len = 0x00;

	((NPI_TX*)m_tx)->Get_Queue()->Push(cmd);
	return NPI_TRUE;//(BOOL)WaitFor_Msg();
}

