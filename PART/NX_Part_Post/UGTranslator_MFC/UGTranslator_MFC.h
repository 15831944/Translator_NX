
// UGTranslator_MFC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CUGTranslator_MFCApp:
// �� Ŭ������ ������ ���ؼ��� UGTranslator_MFC.cpp�� �����Ͻʽÿ�.
//

class CUGTranslator_MFCApp : public CWinApp
{
public:
	CUGTranslator_MFCApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUGTranslator_MFCApp theApp;