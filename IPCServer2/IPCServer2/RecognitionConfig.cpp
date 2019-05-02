#include "stdafx.h"
#include "RecognitionConfig.h"
#include "IniFile.h"
#include "TypeConv.h"
#include "Error.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
CRecognitionConfig::CRecognitionConfig(UINT32 Station, BYTE IP[4], const wstring& StageConfig)
	: m_StageConfigFile(StageConfig)
{
	Config.Station = Station;
	memcpy(Config.CameraIP, IP, 4);
	LoadStageConfig();
}
//---------------------------------------------------------------------------
CRecognitionConfig::~CRecognitionConfig()
{
}
//---------------------------------------------------------------------------
BOOL CRecognitionConfig::LoadStageConfig()
{
	if (!PathFileExists(m_StageConfigFile.c_str())) {
		return FALSE;
	}
	CIniFile iniFile(m_StageConfigFile);

	for (int i = 0; i < MAX_STAGES; ++i) {
		wstring title = L"Stage " + IntToWStr(i);
		Config.StageInfo[i].Algorithm = iniFile.ReadInteger(title, L"Algorithm", 0);
		Config.StageInfo[i].Attribute = iniFile.ReadInteger(title, L"Attribute", 0);

		vector<UINT32>	p_vect;

		try {
			iniFile.ReadIntegers(title, L"P", p_vect);

			if (p_vect.size() >= MAX_STAGE_INFO_PARAMETERS) {
				for (int j = 0; j < MAX_STAGE_INFO_PARAMETERS; ++j) {
					Config.StageInfo[i].P[j] = p_vect[j];
				}
			}
			else {
				memset(Config.StageInfo[i].P, 0, MAX_STAGE_INFO_PARAMETERS);
			}

		}
		catch (CError & Error) {
			memset(Config.StageInfo[i].P, 0, MAX_STAGE_INFO_PARAMETERS);
		}
	}
}
//---------------------------------------------------------------------------
wstring CRecognitionConfig::ToString(int Indent)
{
	wostringstream        text;
	wstring               spaces(Indent, L' ');

	text << spaces << L"Station  : " << Config.Station << L"\r\n";
	text << spaces << L"Cam IP   : ";
	
	for (int i = 0; i < 4; ++i) {
		text << Config.CameraIP[0];
		if (i != 3) {
			text << L".";
		}
	}
	text << L"\r\n";

	for (int i = 0; i < MAX_STAGES; ++i) {
		text << L"\r\n";
		text << spaces << L"[Stage " << i << L" Config]\r\n";
		text << spaces << L"  Algorithm : " << Config.StageInfo[i].Algorithm << L"\r\n";
		text << spaces << L"  Attribute : " << Config.StageInfo[i].Attribute << L"\r\n";
		text << spaces << L"  Parameters: " << Config.StageInfo[i].Attribute << L"\r\n";

		for (int j = 0; j < MAX_STAGE_INFO_PARAMETERS; ++j) {
			if ((j != 0) && (j % 4 == 0)) {
				text << L"\r\n";
			}
			if (j != 0) {
				text << spaces + L"              ";
			}
			text << IntToHexWStr(Config.StageInfo[i].P[j], 4) << L" ";
		}
	}
	return text.str();
}
//---------------------------------------------------------------------------