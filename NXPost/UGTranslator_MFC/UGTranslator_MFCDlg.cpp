
// UGTranslator_MFCDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "UGTranslator_MFC.h"
#include "UGTranslator_MFCDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>	// for TransCAD exit

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUGTranslator_MFCDlg ��ȭ ����




CUGTranslator_MFCDlg::CUGTranslator_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUGTranslator_MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	TransCADStatus = 0;
}

void CUGTranslator_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUGTranslator_MFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUGTranslator_MFCDlg::OnBnClickedImportPath)
	ON_BN_CLICKED(IDC_BUTTON2, &CUGTranslator_MFCDlg::OnBnClickedRunUGPre)
	ON_BN_CLICKED(IDC_BUTTON3, &CUGTranslator_MFCDlg::OnBnClickedExportPath)
	ON_BN_CLICKED(IDC_BUTTON4, &CUGTranslator_MFCDlg::OnBnClickedRunUGPost)
	ON_BN_CLICKED(IDC_BUTTON5, &CUGTranslator_MFCDlg::OnBnClickedRunTransCAD)
	ON_BN_CLICKED(IDC_BUTTON6, &CUGTranslator_MFCDlg::OnBnClickedExitTransCAD)
	ON_BN_CLICKED(IDC_BUTTON7, &CUGTranslator_MFCDlg::OnBnClickedExitTranslator)
END_MESSAGE_MAP()


// CUGTranslator_MFCDlg �޽��� ó����

BOOL CUGTranslator_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CUGTranslator_MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CUGTranslator_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CUGTranslator_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUGTranslator_MFCDlg::OnBnClickedImportPath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString filter = _T("UG part file (*.prt) |*.prt|");
	// If role of dlg is selection --> TRUE
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, filter);

	if (dlg.DoModal())
	{
		CString _path = dlg.m_ofn.lpstrFile;

		if (!_path.IsEmpty())
		{
			SetDlgItemText(IDC_EDIT1, dlg.m_ofn.lpstrFile);
		}
	}
}


void CUGTranslator_MFCDlg::OnBnClickedRunUGPre()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CUGTranslator_MFCDlg::OnBnClickedExportPath()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString filter = _T("UG part file (*.prt) |*.prt|");
	// If role of dlg is save --> FALSE
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, filter);

	CString extention = _T(".prt");

	if (dlg.DoModal())
	{
		CString _path = dlg.m_ofn.lpstrFile;

		if (!_path.IsEmpty())
		{
			if (_path.Find(L".prt") != -1)
				SetDlgItemText(IDC_EDIT2, dlg.m_ofn.lpstrFile);
			else
				SetDlgItemText(IDC_EDIT2, dlg.m_ofn.lpstrFile + extention);			
		}
	}
}


void CUGTranslator_MFCDlg::OnBnClickedRunUGPost()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//HINSTANCE hDLL;
	HMODULE hDLL;
	StartService fnStartService;
	Translate fnTranslate;

	hDLL = LoadLibrary(_T("dllUGPost.dll"));

	
	if (!hDLL)
		AfxMessageBox(_T("Error : UGpost dll isn't loaded..."));

	TRACE("%d", GetLastError());

	CString cstrUGfileName;
	wchar_t* wchar_str;
	char* ch_UGfileName;
	int char_str_len;

	GetDlgItemText(IDC_EDIT2, cstrUGfileName);
	
	// Unicodeȯ�濡�� CString to char * ��ȯ
	wchar_str = cstrUGfileName.GetBuffer(cstrUGfileName.GetLength());
	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	ch_UGfileName = new char[char_str_len];
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, ch_UGfileName, char_str_len, 0, 0);


	if (hDLL != NULL && !cstrUGfileName.IsEmpty())
	{
		// �ҷ��� DLL���� �Ʒ��� �Լ��� Ž�� --> �ν��Ͻ� ����
		fnStartService = (StartService)GetProcAddress(hDLL, "UGPostStartService");
		fnTranslate = (Translate)GetProcAddress(hDLL, "UGPostTranslate");

		if (!fnStartService || !fnTranslate)
		{
			// handle the error
			FreeLibrary(hDLL);
			AfxMessageBox(_T("There are NO FUNCTIONS(StartService or Translate) in dll file"));
		}
		else
		{
			fnStartService();
			bool status = fnTranslate(ch_UGfileName) ? true : false;
			//bool status = fnTranslate((char *)(LPCTSTR)UG_fileName) ? true : false;

			if (!status)
				AfxMessageBox(_T("Make TransCAD Model First!"));
			else
				TransCADStatus = 1;
		}
	}

	FreeLibrary(hDLL);
}


void CUGTranslator_MFCDlg::OnBnClickedRunTransCAD()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (TransCADStatus == 0)
	{
		HRESULT hr = ::CoInitialize(NULL);	// COM Automation API Initialization
		if (FAILED(hr))
		{

			AfxMessageBox(_T("COM Automation API Initialization Failed!"));
		}
		else
		{
			TransCAD::IApplicationPtr g_spApplication;

			// create TransCAD
			hr = g_spApplication.CreateInstance(__uuidof(TransCAD::Application));

			if (FAILED(hr))
				AfxMessageBox(_T("COM Automation API Initialization Failed!"));
			else
			{
				// visualize TransCAD
				g_spApplication->Visible = VARIANT_TRUE;

				TransCAD::IDocsPtr _spDocuments = g_spApplication->Documents;
				TransCAD::IPartDocumentPtr	_spDocument = _spDocuments->AddPartDocument();
				TransCAD::IPartPtr _spPart = _spDocument->Part;

				TransCADStatus = 1;
			}
		}
	}
}


void CUGTranslator_MFCDlg::OnBnClickedExitTransCAD()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (TransCADStatus == 1)
	{
		DWORD dwSize = 250;
		HANDLE hSnapShot;
		PROCESSENTRY32 pEntry;
		BOOL bCrrent = FALSE;


		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

		pEntry.dwSize = sizeof(pEntry);
		// �������� ���μ������� ù���� ������ �����´�.
		Process32First(hSnapShot, &pEntry);

		// Tool�� ���������� Ȯ��
		while (1)
		{
			// ������ ���μ����� ������ �����´�.
			BOOL hRes = Process32Next(hSnapShot, &pEntry);

			if (hRes == FALSE)
				break;
			if (!strncmp(_bstr_t(pEntry.szExeFile), "TransCAD.exe", 15))
			{
				bCrrent = TRUE;
			}
			if (bCrrent)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
				if (hProcess)
				{
					if (TerminateProcess(hProcess, 0))
					{
						unsigned long nCode; //���μ��� ���� ����
						GetExitCodeProcess(hProcess, &nCode);
					}
					CloseHandle(hProcess);
				}
				break;
			}
		}

		TransCADStatus = 0;
	}
}


void CUGTranslator_MFCDlg::OnBnClickedExitTranslator()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnOK();
}
