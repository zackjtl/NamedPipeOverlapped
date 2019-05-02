#pragma once
//---------------------------------------------------------------------------
#include <minwindef.h>
#include <string>
using namespace std;
//---------------------------------------------------------------------------
#define MAX_ACK_TIMEOUT			5000	// unit: ms
#define MAX_STAGE_INFO_PARAMETERS	14
#define MAX_STAGES			20
#pragma pack(push) /* push current alignment to stack */
#pragma pack(1) /* set alignment to 1 byte boundary */
//---------------------------------------------------------------------------
typedef struct _StageInfo {
	UINT32  Algorithm;		// 0:disabled; 1:�w����; 2:����, P0:R, P1:���(�|�Q���H100�নdobule), P2 and P3:�Ƕ�;
	UINT32  Attribute;
	UINT32  P[MAX_STAGE_INFO_PARAMETERS];
}TStageInfo;
//---------------------------------------------------------------------------
typedef struct _RecogiionConfig
{
	UINT32			Station;
	BYTE			CameraIP[4];
	TStageInfo		StageInfo[MAX_STAGES];
}TRecognitionConfig;
#pragma pack(pop) /* restore original alignment from stack */
//---------------------------------------------------------------------------
class CRecognitionConfig
{
public:
	CRecognitionConfig();
	~CRecognitionConfig();

	TRecognitionConfig	Config;

	wstring ToString(int Indent);

protected:
	wstring		m_StageConfigFile;
	
};
//---------------------------------------------------------------------------
