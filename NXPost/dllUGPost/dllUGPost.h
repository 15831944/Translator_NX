#pragma once

class UGReferenceManager;
extern UGReferenceManager * g_pRefManager;

#ifdef UGPOST_DLL_EXPORTS
#define dllUGPost_API extern "C" __declspec(dllexport)
#else
#define dllUGPost_API extern "C" __declspec(dllimport)
#endif


// �� Ŭ������ UGpost_DLL.dll���� �������� �Լ��Դϴ�.
dllUGPost_API int UGPostStartService(void);
dllUGPost_API int UGPostTranslate(char * nxFileName);