
// IPCServer2Dlg.cpp: 實作檔案
//
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "IPCServer2.h"
#include "IPCServer2Dlg.h"
#include "afxdialogex.h"
#include "UsrMsg.h"
#include <tchar.h>
#include <strsafe.h>
#include "IniFile.h"
#include "RecognitionConfig.h"
#include "ipc_def.h"
//---------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//---------------------------------------------------------------------------
// 對 App About 使用 CAboutDlg 對話方塊
//---------------------------------------------------------------------------
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
//---------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
//---------------------------------------------------------------------------
// CIPCServer2Dlg 對話方塊
//---------------------------------------------------------------------------
CIPCServer2Dlg::CIPCServer2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPCSERVER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
	DDX_Control(pDX, IDC_EDIT1, m_LogEdit);
	DDX_Control(pDX, IDC_EDIT2, m_ClientEdit);
	DDX_Control(pDX, IDC_EDITSTATION, m_StationEdit);
	DDX_Control(pDX, IDC_EDIT_CAMIP0, m_CamIPEdit[0]);
	DDX_Control(pDX, IDC_EDIT_CAMIP1, m_CamIPEdit[1]);
	DDX_Control(pDX, IDC_EDIT_CAMIP2, m_CamIPEdit[2]);
	DDX_Control(pDX, IDC_EDIT_CAMIP3, m_CamIPEdit[3]);
	DDX_Control(pDX, IDC_EDITNPNAME, m_NPNameEdit);
	DDX_Control(pDX, IDC_LIST3, m_ListCtrl);
}
//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CIPCServer2Dlg, CDialogEx)	
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_THREAD_DONE, OnThreadDone)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BTNBROWSE, &CIPCServer2Dlg::OnBnClickedBtnbrowse)
	ON_BN_CLICKED(IDC_BTNOPENCLIENT, &CIPCServer2Dlg::OnBnClickedBtnopenclient)
	ON_BN_CLICKED(IDC_BTNSENDCONFIG, &CIPCServer2Dlg::OnBnClickedBtnsendconfig)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, OnNMClickList)
	ON_MESSAGE(WM_NOTIFY_DESCRIPTION_EDITED, OnNotifyDescriptionEdited)
END_MESSAGE_MAP()
//---------------------------------------------------------------------------
// CIPCServer2Dlg 訊息處理常式
//---------------------------------------------------------------------------
BOOL CIPCServer2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AfxInitRichEdit5();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	
	m_Connected = false;

	InitialListControl();
	InitialGUIConfig();
	

	CString npName;
	m_NPNameEdit.GetWindowText(npName);

	wstring wholeName = L"\\\\.\\pipe\\vns" + npName;

	m_NPServer.Create(wholeName, 16384);

	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);
	m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = 0;
	////m_NPThread->LogCallBack = ShowText;
	m_NPThread->CreateThread();

	m_Connected = true;	
	

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::InitialListControl()
{
	LVCOLUMN lvColumn;
	int nCol;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 50;
	lvColumn.pszText = L"No";
	nCol = m_ListCtrl.InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 150;
	lvColumn.pszText = L"Value";
	nCol = m_ListCtrl.InsertColumn(1, &lvColumn);

	////m_ListCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);

	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EDITLABELS | LVS_EX_GRIDLINES);
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::InitialGUIConfig()
{

	if (PathFileExists(L"ui.ini")) {
		CIniFile iniFile(L"ui.ini");

		wstring clientFile = iniFile.ReadString(L"Main", L"Client File", L"");
		wstring npName = iniFile.ReadString(L"Main", L"NP Name", L"000");
		int station = iniFile.ReadInteger(L"Main", L"Station", 0);
		m_ClientEdit.SetWindowText(clientFile.c_str());
		m_NPNameEdit.SetWindowText(npName.c_str());
		m_StationEdit.SetWindowText(IntToWStr(station).c_str());

		bool readIpFail = false;
		vector<wstring> ips;

		try {
			iniFile.ReadStrings(L"Main", L"Camera IP", ips);

			if (ips.size() != 4) {
				readIpFail = true;
			}
			else {
				for (int i = 0; i < 4; ++i) {
					m_CamIPEdit[i].SetWindowText(ips[i].c_str());
				}
			}
		}
		catch (CError & Error) {
			readIpFail = true;
		}
		if (readIpFail) {
			for (int i = 0; i < 4; ++i) {
				m_CamIPEdit[i].SetWindowText(L"000");
			}
		}
		InitCameraParamList(&iniFile);
	}
	else {
		m_NPNameEdit.SetWindowText(L"000");
		m_StationEdit.SetWindowText(L"0");

		for (int i = 0; i < 4; ++i) {
			m_CamIPEdit[i].SetWindowText(L"000");
		}
		InitCameraParamList(NULL);
	}
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::InitCameraParamList(PVOID IniFile)
{
	CIniFile* pIniFile = (CIniFile*)IniFile;
	vector<wstring>  camParams;

	bool fromIniFile = false;

	if (pIniFile != NULL) {		
		try {
			pIniFile->ReadStrings(L"Main", L"Camera Params", camParams);

			if (camParams.size() >= MAX_CAMERA_CONFIG_PARAMETERS) {
				fromIniFile = true;
			}
		}
		catch (CError & Error) {
			;
		}
	}
	for (int i = 0; i < MAX_CAMERA_CONFIG_PARAMETERS; ++i) {
		int l_iItem = m_ListCtrl.InsertItem(LVIF_TEXT | LVIF_STATE, i, IntToWStr(i).c_str(), 0, LVIS_SELECTED, 0, 0);
		m_ListCtrl.SetItemText(l_iItem, 1, fromIniFile ? camParams[i].c_str() : L"0");
	}
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::SaveGUIConfig()
{
	if (!PathFileExists(L"ui.ini")) {
		CIniFile iniFile(L"ui.ini", FILE_CREATE_NEW);
		iniFile.UpdateFile();
	}
	CString winText;
	CString npName;
	CString strStation;
	CString strIp[4];
	CString strIpWhole;
	m_ClientEdit.GetWindowText(winText);
	m_NPNameEdit.GetWindowText(npName);
	m_StationEdit.GetWindowText(strStation);

	for (int i = 0; i < 4; ++i) {
		m_CamIPEdit[i].GetWindowText(strIp[i]);
		strIpWhole += strIp[i];
		if (i != 3) {
			strIpWhole += L" ";
		}
	}

	CIniFile iniFile(L"ui.ini");
	iniFile.WriteString(L"Main", L"Client File", (LPCWSTR)winText);
	iniFile.WriteString(L"Main", L"NP Name", (LPCWSTR)npName);
	iniFile.WriteString(L"Main", L"Station", (LPCWSTR)strStation);
	iniFile.WriteString(L"Main", L"Camera IP", (LPCWSTR)strIpWhole);

	wstring totalCamParam = L"";
	for (int i = 0; i < MAX_CAMERA_CONFIG_PARAMETERS; ++i) {
		CString temp = m_ListCtrl.GetItemText(i, 1);
		if (i != 0) temp = L" " + temp;
		totalCamParam += temp;
	}
	iniFile.WriteString(L"Main", L"Camera Params", totalCamParam);

	iniFile.UpdateFile();
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnCancel()
{
	SaveGUIConfig();
	CDialog::OnCancel();
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::DumpBuffer(BYTE* Buffer, UINT Size)
{
	CString temp;
	CString text;

	text.Format(L"Buffer Data (%d bytes)\r\n", Size);
	ShowText(text);

	for (int i = 0; i < Size; ++i) {

		if (i != 0 && (i % 16 == 0)) {
			ShowText(L"\r\n");
		}
		temp.Format(L"%02x ", Buffer[i]);
		ShowText(temp);
	}
	ShowText(L"\r\n");
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
//---------------------------------------------------------------------------
// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//---------------------------------------------------------------------------
CIPCServer2Dlg::~CIPCServer2Dlg()
{
	if (m_NPServer.IsConnected()) {
		m_NPServer.DisConnect();
	}
}
//---------------------------------------------------------------------------
void __stdcall CIPCServer2Dlg::ShowText(LPCWSTR Text)
{
	m_LogEdit.SetSel(-1, -1);
	m_LogEdit.ReplaceSel(Text);
}
//---------------------------------------------------------------------------
// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
//---------------------------------------------------------------------------
HCURSOR CIPCServer2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//---------------------------------------------------------------------------
LRESULT CIPCServer2Dlg::OnNotifyDescriptionEdited(WPARAM wParam, LPARAM lParam)
{
	// Get the changed Description field text via the callback
	LV_DISPINFO* dispinfo = reinterpret_cast<LV_DISPINFO*>(lParam);

	// Persist the selected attachment details upon updating its text
	m_ListCtrl.SetItemText(dispinfo->item.iItem, dispinfo->item.iSubItem, dispinfo->item.pszText);

	return 0;
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_ListCtrl.SetClicked(true);
	////m_fClickedList = true;
	m_ListCtrl.OnLButtonDown(MK_LBUTTON, InterviewListCursorPosition());

	*pResult = 0;
}
//---------------------------------------------------------------------------
// Obtain cursor position and offset it to position it at interview list control
CPoint CIPCServer2Dlg::InterviewListCursorPosition() const
{
	DWORD pos = GetMessagePos();
	CPoint pt(LOWORD(pos), HIWORD(pos));
	ScreenToClient(&pt);

	CRect rect;
	CWnd* pWnd = GetDlgItem(m_ListCtrl.GetDlgCtrlID());
	pWnd->GetWindowRect(&rect);
	ScreenToClient(&rect);

	pt.x -= rect.left;
	pt.y -= rect.top;

	return pt;
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnBnClickedBtnsendconfig()
{
	// TODO: 在此加入控制項告知處理常式程式碼

	////m_NPThread->Delete();
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);

	byte buffer[16384];

	CString strStation;
	CString strIp[4];

	m_StationEdit.GetWindowText(strStation);
	m_CamIPEdit[0].GetWindowText(strIp[0]);
	m_CamIPEdit[1].GetWindowText(strIp[1]);
	m_CamIPEdit[2].GetWindowText(strIp[2]);
	m_CamIPEdit[3].GetWindowText(strIp[3]);

	BYTE camIp[4] = { StrToInt(strIp[0]), StrToInt(strIp[1]), StrToInt(strIp[2]), StrToInt(strIp[3])};
	UINT32 camParams[MAX_CAMERA_CONFIG_PARAMETERS];

	for (int i = 0; i < MAX_CAMERA_CONFIG_PARAMETERS; ++i) {
		camParams[i] = StrToInt(m_ListCtrl.GetItemText(i, 1));
	}

	CRecognitionConfig  config(StrToInt(strStation), camIp, camParams, L"StageConfig.ini");

	THeader* pHeader = (THeader*)buffer;

	pHeader->Opcode = OP_SEND_RECOGNITION_CONFIG;
	pHeader->AttachmentSize = sizeof(TRecognitionConfig);
	pHeader->P0 = 0;
	pHeader->P1 = 0;

	TRecognitionConfig* pRecogConfig = (TRecognitionConfig*)& buffer[sizeof(THeader)];

	*pRecogConfig = config.Config;

	int totalSize = sizeof(THeader) + sizeof(TRecognitionConfig);

	m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = NamedPipeWrite;
	m_NPThread->Data = buffer;
	m_NPThread->Length = totalSize;
	m_NPThread->CreateThread();

	////m_NPThread->Delete();
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);

	m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = NamedPipeRead;
	m_NPThread->Data = buffer;
	m_NPThread->Length = 16384;
	m_NPThread->CreateThread();

	////CDialogEx::OnOK();
}
//---------------------------------------------------------------------------
LRESULT CIPCServer2Dlg::OnThreadDone(WPARAM wParam, LPARAM lParam)
{
	CString text;
	int function = wParam;
	int result = lParam;

	if (function == NamedPipeConnect) {
		if (result) {
			ShowText(L"Connect successfully.\r\n");
		}
		else {
			ShowText(L"Connection failed.\r\n");
		}
	}
	else if (function == NamedPipeWrite) {
		if (result) {
			ShowText(L"Write data successfully.\r\n");
		}
		else {
			ShowText(L"Write data failed.\r\n");
		}
	}
	else if (function == NamedPipeRead) {
		if (result) {
			text.Format(L"Read data %d bytes successfully.\r\n", m_NPThread->BytesRead);
			ShowText(text);
			DumpBuffer(m_NPThread->Data, m_NPThread->BytesRead);
		}
		else {
			ShowText(L"Read data failed.\r\n");
		}
	}

	WaitForSingleObject(m_NPThread->m_hThread, 5000);

	m_NPThread->Delete();

	return 0;
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnBnClickedBtnbrowse()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	const TCHAR szFilter[] = L"executable file (*.exe)|*.exe|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, L"exe", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() == IDOK) {
		CString sFilePath = dlg.GetPathName();
		m_ClientEdit.SetWindowText(sFilePath);
	}
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnBnClickedBtnopenclient()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString clientPath;
	CString parameters;
	CString npName;
	m_ClientEdit.GetWindowText(clientPath);
	m_NPNameEdit.GetWindowText(npName);

	parameters = npName;

	m_ClientThread = new CClientThread(m_hWnd, clientPath, parameters, WindowNormal);

	m_ClientThread->m_bAutoDelete = false;
	m_ClientThread->CreateThread();
}
//---------------------------------------------------------------------------