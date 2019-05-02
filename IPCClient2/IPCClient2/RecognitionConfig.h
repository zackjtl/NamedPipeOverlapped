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
	CRecognitionConfig();
	~CRecognitionConfig();

	TRecognitionConfig	Config;

	wstring ToString(int Indent);

protected:
	wstring		m_StageConfigFile;
	
};
//---------------------------------------------------------------------------
