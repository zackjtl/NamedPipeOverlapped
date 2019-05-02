#pragma once
#include <minwindef.h>
//---------------------------------------------------------------------------
#define MAX_ACK_TIMEOUT						5000	// unit: ms
#define MAX_STAGE_CONFIG_PARAMETERS			14
#define MAX_STAGES							20
#define MAX_CAMERA_CONFIG_PARAMETERS		14
//---------------------------------------------------------------------------
#pragma pack(push) /* push current alignment to stack */
#pragma pack(1) /* set alignment to 1 byte boundary */
//---------------------------------------------------------------------------
typedef struct _Header
{
	UINT32 Opcode;
	UINT32 AttachmentSize;	// unit:byte;
	UINT32 P0;
	UINT32 P1;
}THeader;
//---------------------------------------------------------------------------
typedef struct _StageInfo {
	UINT32  Algorithm;		// 0:disabled; 1:定中心; 2:輪廓, P0:R, P1:精度(會被除以100轉成dobule), P2 and P3:灰階;
	UINT32  Attribute;
	UINT32  P[MAX_STAGE_CONFIG_PARAMETERS];
}TStageInfo;
//---------------------------------------------------------------------------
typedef struct _CameraConfig
{
	UINT32	StationNum;
	BYTE	IP[4];
	UINT32	P[MAX_CAMERA_CONFIG_PARAMETERS];
}TCameraConfig;
//---------------------------------------------------------------------------
typedef struct _RecogiionConfig
{
	TCameraConfig	Camera;
	TStageInfo		StageInfo[MAX_STAGES];
}TRecognitionConfig;
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
#pragma pack(pop) /* restore original alignment from stack */