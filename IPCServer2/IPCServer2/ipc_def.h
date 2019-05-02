#pragma once
#include <minwindef.h>

typedef struct _Header
{
	UINT32 Opcode;
	UINT32 AttachmentSize;	// unit:byte;
	UINT32 P0;
	UINT32 P1;
}THeader;

enum eIpcOpCode
{
	OP_ACK = 10,
	OP_SEND_RECOGNITION_CONFIG = 11,
	OP_GET_RECOGNITION_CONFIG = 12,
	OP_WRITE_BMP = 20,
	OP_READ_BMP = 21,
	OP_RECOGNIZE_START = 30,
	OP_RECOGNIZE_RESULT = 31
};