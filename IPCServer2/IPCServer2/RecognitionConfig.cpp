#include "stdafx.h"
#include "RecognitionConfig.h"
#include "IniFile.h"
#include "TypeConv.h"
#include "Error.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
CRecognitionConfig::CRecognitionConfig(UINT32 Station, BYTE IP[4], UINT32 CamParams[MAX_CAMERA_CONFIG_PARAMETERS], const wstring& StageConfig)
	: m_StageConfigFile(StageConfig)
{
	Config.Camera.StationNum = Station;
	memcpy(Config.Camera.IP, IP, 4);
	memcpy(Config.Camera.P, CamParams, MAX_CAMERA_CONFIG_PARAMETERS * sizeof(CamParams[0]));
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

			if (p_vect.size() >= MAX_STAGE_CONFIG_PARAMETERS) {
				for (int j = 0; j < MAX_STAGE_CONFIG_PARAMETERS; ++j) {
					Config.StageInfo[i].P[j] = p_vect[j];
				}
			}
			else {
				memset(Config.StageInfo[i].P, 0, MAX_STAGE_CONFIG_PARAMETERS);
			}

		}
		catch (CError & Error) {
			memset(Config.StageInfo[i].P, 0, MAX_STAGE_CONFIG_PARAMETERS);
		}
	}
}
//---------------------------------------------------------------------------
wstring CRecognitionConfig::ToString(int Indent)
{
	wostringstream        text;
	wstring               spaces(Indent, L' ');

	text << spaces << L"Station     : " << Config.Camera.StationNum << L"\r\n";
	text << spaces << L"Cam IP      : ";	
	
	for (int i = 0; i < 4; ++i) {
		text << Config.Camera.IP[i];
		if (i != 3) {
			text << L".";
		}
	}
	text << L"\r\n";	
	text << spaces << L"Cam Params  : ";
	for (int i = 0; i < MAX_CAMERA_CONFIG_PARAMETERS; ++i) {
		if (i % 8 == 0) {
			if (i != 0) {
				text << L"\r\n";
				text << spaces + L"              ";
			}
		}
		wostringstream temp;
		temp << std::hex << setw(8) << uppercase << setfill(L'0') << right << Config.Camera.P[i];
		text << temp.str() << L" ";
	}

	text << L"\r\n";

	for (int i = 0; i < MAX_STAGES; ++i) {
		text << L"\r\n";
		text << spaces << L"[Stage " << i << L" Config]\r\n";
		text << spaces << L"  Algorithm : " << Config.StageInfo[i].Algorithm << L"\r\n";
		text << spaces << L"  Attribute : " << Config.StageInfo[i].Attribute << L"\r\n";
		text << spaces << L"  Parameters: ";		

		for (int j = 0; j < MAX_STAGE_CONFIG_PARAMETERS; ++j) {
			if (j % 8 == 0) {
				if (j != 0) {
					text << L"\r\n";
					text << spaces + L"              ";
				}
			}
			wostringstream temp;
			temp << std::hex << setw(8) << uppercase << setfill(L'0') << right << Config.StageInfo[i].P[j];
			text << temp.str() << L" ";
		}
		text << L"\r\n";
	}
	return text.str();
}
//---------------------------------------------------------------------------