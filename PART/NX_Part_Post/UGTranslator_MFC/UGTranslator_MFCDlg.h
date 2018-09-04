
// UGTranslator_MFCDlg.h : ��� ����
//

#pragma once


// CUGTranslator_MFCDlg ��ȭ ����
class CUGTranslator_MFCDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CUGTranslator_MFCDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	typedef UINT(WINAPIV* StartService)(void);
	typedef UINT(WINAPIV* Translate)(char * path);
	int TransCADStatus;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UGTRANSLATOR_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedImportPath();
	afx_msg void OnBnClickedRunUGPre();
	afx_msg void OnBnClickedExportPath();
	afx_msg void OnBnClickedRunUGPost();
	afx_msg void OnBnClickedRunTransCAD();
	afx_msg void OnBnClickedExitTransCAD();
	afx_msg void OnBnClickedExitTranslator();
};
