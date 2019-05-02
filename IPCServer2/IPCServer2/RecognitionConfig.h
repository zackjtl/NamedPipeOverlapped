#pragma once
//---------------------------------------------------------------------------
#include <minwindef.h>
#include <string>
#include "ipc_def.h"
using namespace std;
//---------------------------------------------------------------------------
class CRecognitionConfig
{
public:
	CRecognitionConfig(UINT32 Station, BYTE IP[4], UINT32 CamParams[MAX_CAMERA_CONFIG_PARAMETERS], const wstring& StageConfig);
	~CRecognitionConfig();
	BOOL LoadStageConfig();

	TRecognitionConfig	Config;

	wstring ToString(int Indent);

protected:
	wstring		m_StageConfigFile;
	
};
//---------------------------------------------------------------------------
