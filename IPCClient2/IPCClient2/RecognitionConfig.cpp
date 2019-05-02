#include "Pch.h"
#include "RecognitionConfig.h"
////#include "TypeConv.h"
#include "Error.h"
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;
//---------------------------------------------------------------------------
CRecognitionConfig::CRecognitionConfig()
{
}
//---------------------------------------------------------------------------
CRecognitionConfig::~CRecognitionConfig()
{
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