#pragma once

#include "Pattern\IDataHandler.h"
#include "Com\SerialPort.h"


class CRouter : public IDataHandler
{
public:
	CRouter();
	~CRouter();

public:
	virtual void HandleData(UINT8* pRxData, size_t length);

	void    RegisterHandler(IDataHandler* pHandler,  UINT8 whatCmd);

	void    SendData(UINT8* pData, size_t length);

	void    AttachSender(CSerialPort* pSender);

	uint32_t CRC32Software(uint8_t *pData, uint16_t Length);
	bool CheckCRC32(uint8_t *pData, uint16_t Length);
private:	
    CSerialPort*    mSender;
	IDataHandler*   mHandleres[10];

	UINT8   mTxCache[10][20];
	UINT8   mRxCache[10][20];

	UINT16  mTxCursor;
	UINT16  mRxCursor;

	CRITICAL_SECTION mCS;
};