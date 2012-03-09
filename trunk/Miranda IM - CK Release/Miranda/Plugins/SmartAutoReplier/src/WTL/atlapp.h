// Windows Template Library - WTL version 7.1
// Copyright (C) 1997-2003 Microsoft Corporation
// All rights reserved.
//
// This file is a part of the Windows Template Library.
// The code and information is provided "as-is" without
// warranty of any kind, either expressed or implied.

#ifndef __ATLAPP_H__
#define __ATLAPP_H__

#pragma once

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error atlapp.h requires atlbase.h to be included first
#endif

#ifndef _WIN32_WCE
  #if (WINVER < 0x0400)
	#error WTL requires Windows version 4.0 or higher
  #endif

  #if (_WIN32_IE < 0x0300)
	#error WTL requires IE version 3.0 or higher
  #endif
#endif

#ifdef _ATL_NO_COMMODULE
	#error WTL requires that _ATL_NO_COMMODULE is not defined
#endif //_ATL_NO_COMMODULE

#if defined(_WIN32_WCE) && defined(_ATL_MIN_CRT)
	#pragma message("Warning: WTL for Windows CE doesn't use _ATL_MIN_CRT")
#endif //defined(_WIN32_WCE) && defined(_ATL_MIN_CRT)

#include <limits.h>
#if !defined(_ATL_MIN_CRT) && defined(_MT)
  #include <process.h>	// for _beginthreadex
#endif

#include <commctrl.h>
#ifndef _WIN32_WCE
#pragma comment(lib, "comctl32.lib")
#endif //!_WIN32_WCE

#ifndef _WIN32_WCE
  #include "WTL\atlres.h"
#else // CE specific
  #include <atlresce.h>
#endif //_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// WTL version number

#define _WTL_VER	0x0710


///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CMessageFilter
// CIdleHandler
// CMessageLoop
//
// CAppModule
// CServerAppModule
//
// Global functions:
//   AtlGetDefaultGuiFont()
//   AtlCreateBoldFont()
//   AtlInitCommonControls()


///////////////////////////////////////////////////////////////////////////////
// Global support for Windows CE

#ifdef _WIN32_WCE

#ifndef SW_SHOWDEFAULT
  #define SW_SHOWDEFAULT	SW_SHOWNORMAL
#endif //!SW_SHOWDEFAULT

#ifdef lstrlenW
  #undef lstrlenW
  #define lstrlenW (int)wcslen
#endif //lstrlenW

#define lstrlenA (int)strlen

#ifndef lstrcpyn
  inline LPTSTR lstrcpyn(LPTSTR lpstrDest, LPCTSTR lpstrSrc, int nLength)
  {
	if(lpstrDest == NULL || lpstrSrc == NULL || nLength <= 0)
		return NULL;
	int nLen = min(lstrlen(lpstrSrc), nLength - 1);
	LPTSTR lpstrRet = (LPTSTR)memcpy(lpstrDest, lpstrSrc, nLen * sizeof(TCHAR));
	lpstrDest[nLen] = 0;
	return lpstrRet;
  }
#endif //!lstrcpyn

#ifdef TrackPopupMenu
  #undef TrackPopupMenu
#endif // TrackPopupMenu

// This get's ORed in a constant and will have no effect.
// Defining it does reduced the number of #ifdefs required for CE.
#define LR_DEFAULTSIZE      0

#define DECLARE_WND_CLASS_EX(WndClassName, style, bkgnd) \
static CWndClassInfo& GetWndClassInfo() \
{ \
	static CWndClassInfo wc = \
	{ \
		{ style, StartWindowProc, \
		  0, 0, NULL, NULL, NULL, (HBRUSH)(bkgnd + 1), NULL, WndClassName }, \
		NULL, NULL, IDC_ARROW, TRUE, 0, _T("") \
	}; \
	return wc; \
}

#define _MAX_FNAME	_MAX_PATH

inline int MulDiv(IN int nNumber, IN int nNumerator, IN int nDenominator)
{
	__int64 multiple = nNumber * nNumerator;
	return static_cast<int>(multiple / nDenominator);
}

inline BOOL IsMenu(HMENU hMenu)
{
	MENUITEMINFO mii = { 0 };
	mii.cbSize = sizeof(mii);
	::SetLastError(0);
	BOOL bRet = ::GetMenuItemInfo(hMenu, 0, TRUE, &mii);
	if(!bRet)
		bRet = (::GetLastError() != ERROR_INVALID_MENU_HANDLE) ? TRUE : FALSE;
	return bRet;
}

extern "C" void WINAPI ListView_SetItemSpacing(HWND hwndLV, int iHeight);

#if (_WIN32_WCE < 400)
  #define MAKEINTATOM(i)  (LPTSTR)((ULONG_PTR)((WORD)(i)))

  #define WHEEL_PAGESCROLL                (UINT_MAX)
  #define WHEEL_DELTA                     120
#endif //(_WIN32_WCE < 400)

#endif //_WIN32_WCE


///////////////////////////////////////////////////////////////////////////////
// Global support for using original VC++ 6.0 headers with WTL

#ifndef _ATL_NO_OLD_HEADERS_WIN64
#if !defined(_WIN64) && (_ATL_VER < 0x0700)

  #ifndef PSM_INSERTPAGE
    #define PSM_INSERTPAGE          (WM_USER + 119)
  #endif //!PSM_INSERTPAGE

  #ifndef GetClassLongPtr
    #define GetClassLongPtrA   GetClassLongA
    #define GetClassLongPtrW   GetClassLongW
    #ifdef UNICODE
      #define GetClassLongPtr  GetClassLongPtrW
    #else
      #define GetClassLongPtr  GetClassLongPtrA
    #endif // !UNICODE
  #endif // !GetClassLongPtr

  #ifndef GCLP_HICONSM
    #define GCLP_HICONSM        (-34)
  #endif //!GCLP_HICONSM

  #ifndef GetWindowLongPtr
    #define GetWindowLongPtrA   GetWindowLongA
    #define GetWindowLongPtrW   GetWindowLongW
    #ifdef UNICODE
      #define GetWindowLongPtr  GetWindowLongPtrW
    #else
      #define GetWindowLongPtr  GetWindowLongPtrA
    #endif // !UNICODE
  #endif // !GetWindowLongPtr

  #ifndef SetWindowLongPtr
    #define SetWindowLongPtrA   SetWindowLongA
    #define SetWindowLongPtrW   SetWindowLongW
    #ifdef UNICODE
      #define SetWindowLongPtr  SetWindowLongPtrW
    #else
      #define SetWindowLongPtr  SetWindowLongPtrA
    #endif // !UNICODE
  #endif // !SetWindowLongPtr

  #ifndef GWLP_WNDPROC
    #define GWLP_WNDPROC        (-4)
  #endif
  #ifndef GWLP_HINSTANCE
    #define GWLP_HINSTANCE      (-6)
  #endif
  #ifndef GWLP_HWNDPARENT
    #define GWLP_HWNDPARENT     (-8)
  #endif
  #ifndef GWLP_USERDATA
    #define GWLP_USERDATA       (-21)
  #endif
  #ifndef GWLP_ID
    #define GWLP_ID             (-12)
  #endif

  #ifndef DWLP_MSGRESULT
    #define DWLP_MSGRESULT  0
  #endif

  typedef long LONG_PTR;
  typedef unsigned long ULONG_PTR;
  typedef ULONG_PTR DWORD_PTR;

  #ifndef HandleToUlong
    #define HandleToUlong( h ) ((ULONG)(ULONG_PTR)(h) )
  #endif
  #ifndef HandleToLong
    #define HandleToLong( h ) ((LONG)(LONG_PTR) (h) )
  #endif
  #ifndef LongToHandle
    #define LongToHandle( h) ((HANDLE)(LONG_PTR) (h))
  #endif
  #ifndef PtrToUlong
    #define PtrToUlong( p ) ((ULONG)(ULONG_PTR) (p) )
  #endif
  #ifndef PtrToLong
    #define PtrToLong( p ) ((LONG)(LONG_PTR) (p) )
  #endif
  #ifndef PtrToUint
    #define PtrToUint( p ) ((UINT)(UINT_PTR) (p) )
  #endif
  #ifndef PtrToInt
    #define PtrToInt( p ) ((INT)(INT_PTR) (p) )
  #endif
  #ifndef PtrToUshort
    #define PtrToUshort( p ) ((unsigned short)(ULONG_PTR)(p) )
  #endif
  #ifndef PtrToShort
    #define PtrToShort( p ) ((short)(LONG_PTR)(p) )
  #endif
  #ifndef IntToPtr
    #define IntToPtr( i )    ((VOID *)(INT_PTR)((int)i))
  #endif
  #ifndef UIntToPtr
    #define UIntToPtr( ui )  ((VOID *)(UINT_PTR)((unsigned int)ui))
  #endif
  #ifndef LongToPtr
    #define LongToPtr( l )   ((VOID *)(LONG_PTR)((long)l))
  #endif
  #ifndef ULongToPtr
    #define ULongToPtr( ul )  ((VOID *)(ULONG_PTR)((unsigned long)ul))
  #endif

#endif //!defined(_WIN64) && (_ATL_VER < 0x0700)
#endif //!_ATL_NO_OLD_HEADERS_WIN64


///////////////////////////////////////////////////////////////////////////////
// Miscellaneous global support

// define useful macros from winuser.h
#ifndef IS_INTRESOURCE
  #define IS_INTRESOURCE(_r) (((ULONG_PTR)(_r) >> 16) == 0)
#endif //IS_INTRESOURCE

// protect template members from windowsx.h macros
#ifdef _INC_WINDOWSX
  #undef SubclassWindow
#endif //_INC_WINDOWSX

// define useful macros from windowsx.h
#ifndef GET_X_LPARAM
  #define GET_X_LPARAM(lParam)	((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
  #define GET_Y_LPARAM(lParam)	((int)(short)HIWORD(lParam))
#endif

// Dummy structs for compiling with /CLR
#if (_MSC_VER >= 1300) && defined(_MANAGED)
  __if_not_exists(_IMAGELIST::_IMAGELIST) { struct _IMAGELIST { }; }
  __if_not_exists(_TREEITEM::_TREEITEM) { struct _TREEITEM { }; }
  __if_not_exists(_PSP::_PSP) { struct _PSP { }; }
#endif


namespace WTL
{

#if (_ATL_VER >= 0x0700)
  DECLARE_TRACE_CATEGORY(atlTraceUI);
  #ifdef _DEBUG
    __declspec(selectany) ATL::CTraceCategory atlTraceUI(_T("atlTraceUI"));
  #endif // _DEBUG
#else //!(_ATL_VER >= 0x0700)
  enum wtlTraceFlags
  {
	atlTraceUI = 0x10000000
  };
#endif //!(_ATL_VER >= 0x0700)

// Windows version helper
inline bool AtlIsOldWindows()
{
	OSVERSIONINFO ovi = { 0 };
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bRet = ::GetVersionEx(&ovi);
	return (!bRet || !((ovi.dwMajorVersion >= 5) || (ovi.dwMajorVersion == 4 && ovi.dwMinorVersion >= 90)));
}

// default GUI font helper
inline HFONT AtlGetDefaultGuiFont()
{
#ifndef _WIN32_WCE
	return (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
#else // CE specific
	return (HFONT)::GetStockObject(SYSTEM_FONT);
#endif //_WIN32_WCE
}

// bold font helper (NOTE: Caller owns the font, and should destroy it when done using it)
inline HFONT AtlCreateBoldFont(HFONT hFont = NULL)
{
	if(hFont == NULL)
		hFont = AtlGetDefaultGuiFont();
	ATLASSERT(hFont != NULL);
	HFONT hFontBold = NULL;
	LOGFONT lf = { 0 };
	if(::GetObject(hFont, sizeof(LOGFONT), &lf) == sizeof(LOGFONT))
	{
		lf.lfWeight = FW_BOLD;
		hFontBold =  ::CreateFontIndirect(&lf);
		ATLASSERT(hFontBold != NULL);
	}
	else
	{
		ATLASSERT(FALSE);
	}
	return hFontBold;
}

// Common Controls initialization helper
inline BOOL AtlInitCommonControls(DWORD dwFlags)
{
	INITCOMMONCONTROLSEX iccx = { sizeof(INITCOMMONCONTROLSEX), dwFlags };
	BOOL bRet = ::InitCommonControlsEx(&iccx);
	ATLASSERT(bRet);
	return bRet;
}


///////////////////////////////////////////////////////////////////////////////
// CMessageFilter - Interface for message filter support

class CMessageFilter
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// CIdleHandler - Interface for idle processing

class CIdleHandler
{
public:
	virtual BOOL OnIdle() = 0;
};

#ifndef _ATL_NO_OLD_NAMES
  // for compatilibility with old names only
  typedef CIdleHandler CUpdateUIObject;
  #define DoUpdate OnIdle
#endif //!_ATL_NO_OLD_NAMES


///////////////////////////////////////////////////////////////////////////////
// CMessageLoop - message loop implementation

class CMessageLoop
{
public:
	ATL::CSimpleArray<CMessageFilter*> m_aMsgFilter;
	ATL::CSimpleArray<CIdleHandler*> m_aIdleHandler;
	MSG m_msg;

// Message filter operations
	BOOL AddMessageFilter(CMessageFilter* pMessageFilter)
	{
		return m_aMsgFilter.Add(pMessageFilter);
	}

	BOOL RemoveMessageFilter(CMessageFilter* pMessageFilter)
	{
		return m_aMsgFilter.Remove(pMessageFilter);
	}

// Idle handler operations
	BOOL AddIdleHandler(CIdleHandler* pIdleHandler)
	{
		return m_aIdleHandler.Add(pIdleHandler);
	}

	BOOL RemoveIdleHandler(CIdleHandler* pIdleHandler)
	{
		return m_aIdleHandler.Remove(pIdleHandler);
	}

#ifndef _ATL_NO_OLD_NAMES
	// for compatilibility with old names only
	BOOL AddUpdateUI(CIdleHandler* pIdleHandler)
	{
		ATLTRACE2(atlTraceUI, 0, "CUpdateUIObject and AddUpdateUI are deprecated. Please change your code to use CIdleHandler and OnIdle\n");
		return AddIdleHandler(pIdleHandler);
	}

	BOOL RemoveUpdateUI(CIdleHandler* pIdleHandler)
	{
		ATLTRACE2(atlTraceUI, 0, "CUpdateUIObject and RemoveUpdateUI are deprecated. Please change your code to use CIdleHandler and OnIdle\n");
		return RemoveIdleHandler(pIdleHandler);
	}
#endif //!_ATL_NO_OLD_NAMES

// message loop
	int Run()
	{
		BOOL bDoIdle = TRUE;
		int nIdleCount = 0;
		BOOL bRet;

		for(;;)
		{
			while(bDoIdle && !::PeekMessage(&m_msg, NULL, 0, 0, PM_NOREMOVE))
			{
				if(!OnIdle(nIdleCount++))
					bDoIdle = FALSE;
			}

			bRet = ::GetMessage(&m_msg, NULL, 0, 0);

			if(bRet == -1)
			{
				ATLTRACE2(atlTraceUI, 0, _T("::GetMessage returned -1 (error)\n"));
				continue;   // error, don't process
			}
			else if(!bRet)
			{
				ATLTRACE2(atlTraceUI, 0, _T("CMessageLoop::Run - exiting\n"));
				break;   // WM_QUIT, exit message loop
			}

			if(!PreTranslateMessage(&m_msg))
			{
				::TranslateMessage(&m_msg);
				::DispatchMessage(&m_msg);
			}

			if(IsIdleMessage(&m_msg))
			{
				bDoIdle = TRUE;
				nIdleCount = 0;
			}
		}

		return (int)m_msg.wParam;
	}

	static BOOL IsIdleMessage(MSG* pMsg)
	{
		// These messages should NOT cause idle processing
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
#ifndef _WIN32_WCE
		case WM_NCMOUSEMOVE:
#endif //!_WIN32_WCE
		case WM_PAINT:
		case 0x0118:	// WM_SYSTIMER (caret blink)
			return FALSE;
		}

		return TRUE;
	}

// Overrideables
	// Override to change message filtering
	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		// loop backwards
		for(int i = m_aMsgFilter.GetSize() - 1; i >= 0; i--)
		{
			CMessageFilter* pMessageFilter = m_aMsgFilter[i];
			if(pMessageFilter != NULL && pMessageFilter->PreTranslateMessage(pMsg))
				return TRUE;
		}
		return FALSE;   // not translated
	}

	// override to change idle processing
	virtual BOOL OnIdle(int /*nIdleCount*/)
	{
		for(int i = 0; i < m_aIdleHandler.GetSize(); i++)
		{
			CIdleHandler* pIdleHandler = m_aIdleHandler[i];
			if(pIdleHandler != NULL)
				pIdleHandler->OnIdle();
		}
		return FALSE;   // don't continue
	}
};


///////////////////////////////////////////////////////////////////////////////
// CStaticDataInitCriticalSectionLock and CWindowCreateCriticalSectionLock
// internal classes to manage critical sections for both ATL3 and ATl7

class CStaticDataInitCriticalSectionLock
{
public:
#if (_ATL_VER >= 0x0700)
	ATL::CComCritSecLock<ATL::CComCriticalSection> m_cslock;

	CStaticDataInitCriticalSectionLock() : m_cslock(ATL::_pAtlModule->m_csStaticDataInitAndTypeInfo, false)
	{ }
#endif //(_ATL_VER >= 0x0700)

	HRESULT Lock()
	{
#if (_ATL_VER >= 0x0700)
		return m_cslock.Lock();
#else //!(_ATL_VER >= 0x0700)
		::EnterCriticalSection(&ATL::_pModule->m_csStaticDataInit);
		return S_OK;
#endif //!(_ATL_VER >= 0x0700)
	}

	void Unlock()
	{
#if (_ATL_VER >= 0x0700)
		m_cslock.Unlock();
#else //!(_ATL_VER >= 0x0700)
		::LeaveCriticalSection(&ATL::_pModule->m_csStaticDataInit);
#endif //!(_ATL_VER >= 0x0700)
	}
};


class CWindowCreateCriticalSectionLock
{
public:
#if (_ATL_VER >= 0x0700)
	ATL::CComCritSecLock<ATL::CComCriticalSection> m_cslock;

	CWindowCreateCriticalSectionLock() : m_cslock(ATL::_AtlWinModule.m_csWindowCreate, false)
	{ }
#endif //(_ATL_VER >= 0x0700)

	HRESULT Lock()
	{
#if (_ATL_VER >= 0x0700)
		return m_cslock.Lock();
#else //!(_ATL_VER >= 0x0700)
		::EnterCriticalSection(&ATL::_pModule->m_csWindowCreate);
		return S_OK;
#endif //!(_ATL_VER >= 0x0700)
	}

	void Unlock()
	{
#if (_ATL_VER >= 0x0700)
		m_cslock.Unlock();
#else //!(_ATL_VER >= 0x0700)
		::LeaveCriticalSection(&ATL::_pModule->m_csWindowCreate);
#endif //!(_ATL_VER >= 0x0700)
	}
};


///////////////////////////////////////////////////////////////////////////////
// CAppModule - module class for an application

class CAppModule : public ATL::CComModule
{
public:
	DWORD m_dwMainThreadID;
	ATL::CSimpleMap<DWORD, CMessageLoop*>* m_pMsgLoopMap;
	ATL::CSimpleArray<HWND>* m_pSettingChangeNotify;

// Overrides of CComModule::Init and Term
	HRESULT Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL)
	{
		HRESULT hRet = CComModule::Init(pObjMap, hInstance, pLibID);
		if(FAILED(hRet))
			return hRet;

		m_dwMainThreadID = ::GetCurrentThreadId();
		typedef ATL::CSimpleMap<DWORD, CMessageLoop*>   _mapClass;
		m_pMsgLoopMap = NULL;
		ATLTRY(m_pMsgLoopMap = new _mapClass);
		if(m_pMsgLoopMap == NULL)
			return E_OUTOFMEMORY;
		m_pSettingChangeNotify = NULL;

		return hRet;
	}

	void Term()
	{
		TermSettingChangeNotify();
		delete m_pMsgLoopMap;
		CComModule::Term();
	}

// Message loop map methods
	BOOL AddMessageLoop(CMessageLoop* pMsgLoop)
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::AddMessageLoop.\n"));
			ATLASSERT(FALSE);
			return FALSE;
		}

		ATLASSERT(pMsgLoop != NULL);
		ATLASSERT(m_pMsgLoopMap->Lookup(::GetCurrentThreadId()) == NULL);   // not in map yet

		BOOL bRet = m_pMsgLoopMap->Add(::GetCurrentThreadId(), pMsgLoop);

		lock.Unlock();

		return bRet;
	}

	BOOL RemoveMessageLoop()
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::RemoveMessageLoop.\n"));
			ATLASSERT(FALSE);
			return FALSE;
		}

		BOOL bRet = m_pMsgLoopMap->Remove(::GetCurrentThreadId());

		lock.Unlock();

		return bRet;
	}

	CMessageLoop* GetMessageLoop(DWORD dwThreadID = ::GetCurrentThreadId()) const
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::GetMessageLoop.\n"));
			ATLASSERT(FALSE);
			return NULL;
		}

		CMessageLoop* pLoop =  m_pMsgLoopMap->Lookup(dwThreadID);

		lock.Unlock();

		return pLoop;
	}

// Setting change notify methods
	// Note: Call this from the main thread for MSDI apps
	BOOL InitSettingChangeNotify(DLGPROC pfnDlgProc = _SettingChangeDlgProc)
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::InitSettingChangeNotify.\n"));
			ATLASSERT(FALSE);
			return FALSE;
		}

		if(m_pSettingChangeNotify == NULL)
		{
			typedef ATL::CSimpleArray<HWND>   _notifyClass;
			ATLTRY(m_pSettingChangeNotify = new _notifyClass);
			ATLASSERT(m_pSettingChangeNotify != NULL);
		}

		BOOL bRet = (m_pSettingChangeNotify != NULL);
		if(bRet && m_pSettingChangeNotify->GetSize() == 0)
		{
			// init everything
			_ATL_EMPTY_DLGTEMPLATE templ;
			HWND hNtfWnd = ::CreateDialogIndirect(GetModuleInstance(), &templ, NULL, pfnDlgProc);
			ATLASSERT(::IsWindow(hNtfWnd));
			if(::IsWindow(hNtfWnd))
			{
// need conditional code because types don't match in winuser.h
#ifdef _WIN64
				::SetWindowLongPtr(hNtfWnd, GWLP_USERDATA, (LONG_PTR)this);
#else
				::SetWindowLongPtr(hNtfWnd, GWLP_USERDATA, PtrToLong(this));
#endif
				bRet = m_pSettingChangeNotify->Add(hNtfWnd);
			}
			else
			{
				bRet = FALSE;
			}
		}

		lock.Unlock();

		return bRet;
	}

	void TermSettingChangeNotify()
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::TermSettingChangeNotify.\n"));
			ATLASSERT(FALSE);
			return;
		}

		if(m_pSettingChangeNotify != NULL && m_pSettingChangeNotify->GetSize() > 0)
			::DestroyWindow((*m_pSettingChangeNotify)[0]);
		delete m_pSettingChangeNotify;
		m_pSettingChangeNotify = NULL;

		lock.Unlock();
	}

	BOOL AddSettingChangeNotify(HWND hWnd)
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::AddSettingChangeNotify.\n"));
			ATLASSERT(FALSE);
			return FALSE;
		}

		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = FALSE;
		if(InitSettingChangeNotify() != FALSE)
			bRet = m_pSettingChangeNotify->Add(hWnd);

		lock.Unlock();

		return bRet;
	}

	BOOL RemoveSettingChangeNotify(HWND hWnd)
	{
		CStaticDataInitCriticalSectionLock lock;
		if(FAILED(lock.Lock()))
		{
			ATLTRACE2(atlTraceUI, 0, _T("ERROR : Unable to lock critical section in CAppModule::RemoveSettingChangeNotify.\n"));
			ATLASSERT(FALSE);
			return FALSE;
		}

		BOOL bRet = FALSE;
		if(m_pSettingChangeNotify != NULL)
			bRet = m_pSettingChangeNotify->Remove(hWnd);

		lock.Unlock();

		return bRet;
	}

// Implementation - setting change notify dialog template and dialog procedure
	struct _ATL_EMPTY_DLGTEMPLATE : DLGTEMPLATE
	{
		_ATL_EMPTY_DLGTEMPLATE()
		{
			memset(this, 0, sizeof(_ATL_EMPTY_DLGTEMPLATE));
			style = WS_POPUP;
		}
		WORD wMenu, wClass, wTitle;
	};

#ifdef _WIN64
	static INT_PTR CALLBACK _SettingChangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#else
	static BOOL CALLBACK _SettingChangeDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
#endif
	{
		if(uMsg == WM_SETTINGCHANGE)
		{
// need conditional code because types don't match in winuser.h
#ifdef _WIN64
			CAppModule* pModule = (CAppModule*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
#else
			CAppModule* pModule = (CAppModule*)LongToPtr(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
#endif
			ATLASSERT(pModule != NULL);
			ATLASSERT(pModule->m_pSettingChangeNotify != NULL);
			const UINT uTimeout = 1500;   // ms
			for(int i = 1; i < pModule->m_pSettingChangeNotify->GetSize(); i++)
			{
#if !defined(_WIN32_WCE)
				::SendMessageTimeout((*pModule->m_pSettingChangeNotify)[i], uMsg, wParam, lParam, SMTO_ABORTIFHUNG, uTimeout, NULL);
#elif(_WIN32_WCE >= 400) // CE specific
				::SendMessageTimeout((*pModule->m_pSettingChangeNotify)[i], uMsg, wParam, lParam, SMTO_NORMAL, uTimeout, NULL);
#else //_WIN32_WCE < 400 specific
				uTimeout;
				::SendMessage((*pModule->m_pSettingChangeNotify)[i], uMsg, wParam, lParam);
#endif
			}
			return TRUE;
		}
		return FALSE;
	}
};


///////////////////////////////////////////////////////////////////////////////
// CServerAppModule - module class for a COM server application

class CServerAppModule : public CAppModule
{
public:
	HANDLE m_hEventShutdown;
	bool m_bActivity;
	DWORD m_dwTimeOut;
	DWORD m_dwPause;

// Override of CAppModule::Init
	HRESULT Init(ATL::_ATL_OBJMAP_ENTRY* pObjMap, HINSTANCE hInstance, const GUID* pLibID = NULL)
	{
		m_dwTimeOut = 5000;
		m_dwPause = 1000;
		return CAppModule::Init(pObjMap, hInstance, pLibID);
	}

	void Term()
	{
		if(m_hEventShutdown != NULL && ::CloseHandle(m_hEventShutdown))
			m_hEventShutdown = NULL;
		CAppModule::Term();
	}

// COM Server methods
	LONG Unlock()
	{
		LONG lRet = CComModule::Unlock();
		if(lRet == 0)
		{
			m_bActivity = true;
			::SetEvent(m_hEventShutdown); // tell monitor that we transitioned to zero
		}
		return lRet;
	}

	void MonitorShutdown()
	{
		while(1)
		{
			::WaitForSingleObject(m_hEventShutdown, INFINITE);
			DWORD dwWait = 0;
			do
			{
				m_bActivity = false;
				dwWait = ::WaitForSingleObject(m_hEventShutdown, m_dwTimeOut);
			}
			while(dwWait == WAIT_OBJECT_0);
			// timed out
			if(!m_bActivity && m_nLockCnt == 0) // if no activity let's really bail
			{
#if ((_WIN32_WINNT >= 0x0400 ) || defined(_WIN32_DCOM)) && defined(_ATL_FREE_THREADED) & !defined(_WIN32_WCE)
				::CoSuspendClassObjects();
				if(!m_bActivity && m_nLockCnt == 0)
#endif
					break;
			}
		}
		// This handle should be valid now. If it isn't, 
		// check if _Module.Term was called first (it shouldn't)
		if(::CloseHandle(m_hEventShutdown))
			m_hEventShutdown = NULL;
		::PostThreadMessage(m_dwMainThreadID, WM_QUIT, 0, 0);
	}

	bool StartMonitor()
	{
		m_hEventShutdown = ::CreateEvent(NULL, false, false, NULL);
		if(m_hEventShutdown == NULL)
			return false;
		DWORD dwThreadID;
#if !defined(_ATL_MIN_CRT) && defined(_MT)
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, (UINT (WINAPI*)(void*))MonitorProc, this, 0, (UINT*)&dwThreadID);
#else
		HANDLE hThread = ::CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
#endif
		bool bRet = (hThread != NULL);
		if(bRet)
			::CloseHandle(hThread);
		return bRet;
	}

	static DWORD WINAPI MonitorProc(void* pv)
	{
		CServerAppModule* p = (CServerAppModule*)pv;
		p->MonitorShutdown();
		return 0;
	}

#if (_ATL_VER < 0x0700)
	// search for an occurence of string p2 in string p1
	static LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
	{
		while(p1 != NULL && *p1 != NULL)
		{
			LPCTSTR p = p2;
			while(p != NULL && *p != NULL)
			{
				if(*p1 == *p)
					return ::CharNext(p1);
				p = ::CharNext(p);
			}
			p1 = ::CharNext(p1);
		}
		return NULL;
	}
#endif //(_ATL_VER < 0x0700)
};


///////////////////////////////////////////////////////////////////////////////
// CString forward reference (enables CString use in atluser.h and atlgdi.h)

#if defined(_WTL_FORWARD_DECLARE_CSTRING) && !defined(_WTL_USE_CSTRING)
  #define _WTL_USE_CSTRING
#endif //defined(_WTL_FORWARD_DECLARE_CSTRING) && !defined(_WTL_USE_CSTRING)

#ifdef _WTL_USE_CSTRING
class CString;   // forward declaration (include atlmisc.h for the whole class)
#endif //_WTL_USE_CSTRING

#ifndef _CSTRING_NS
  #ifdef __ATLSTR_H__
  #define _CSTRING_NS	ATL
  #else
  #define _CSTRING_NS	WTL
  #endif
#endif //_CSTRING_NS

}; //namespace WTL


///////////////////////////////////////////////////////////////////////////////
// General DLL version helpers (excluded from atlbase.h if _ATL_DLL is defined)

#if (_ATL_VER < 0x0700) && defined(_ATL_DLL) && !defined(_WIN32_WCE)

namespace ATL
{

inline HRESULT AtlGetDllVersion(HINSTANCE hInstDLL, DLLVERSIONINFO* pDllVersionInfo)
{
	ATLASSERT(pDllVersionInfo != NULL);
	if(pDllVersionInfo == NULL)
		return E_INVALIDARG;

	// We must get this function explicitly because some DLLs don't implement it.
	DLLGETVERSIONPROC pfnDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hInstDLL, "DllGetVersion");
	if(pfnDllGetVersion == NULL)
		return E_NOTIMPL;

	return (*pfnDllGetVersion)(pDllVersionInfo);
}

inline HRESULT AtlGetDllVersion(LPCTSTR lpstrDllName, DLLVERSIONINFO* pDllVersionInfo)
{
	HINSTANCE hInstDLL = ::LoadLibrary(lpstrDllName);
	if(hInstDLL == NULL)
		return E_FAIL;
	HRESULT hRet = AtlGetDllVersion(hInstDLL, pDllVersionInfo);
	::FreeLibrary(hInstDLL);
	return hRet;
}

// Common Control Versions:
//   Win95/WinNT 4.0    maj=4 min=00
//   IE 3.x     maj=4 min=70
//   IE 4.0     maj=4 min=71
inline HRESULT AtlGetCommCtrlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
	if(pdwMajor == NULL || pdwMinor == NULL)
		return E_INVALIDARG;

	DLLVERSIONINFO dvi;
	::ZeroMemory(&dvi, sizeof(dvi));
	dvi.cbSize = sizeof(dvi);
	HRESULT hRet = AtlGetDllVersion(_T("comctl32.dll"), &dvi);

	if(SUCCEEDED(hRet))
	{
		*pdwMajor = dvi.dwMajorVersion;
		*pdwMinor = dvi.dwMinorVersion;
	}
	else if(hRet == E_NOTIMPL)
	{
		// If DllGetVersion is not there, then the DLL is a version
		// previous to the one shipped with IE 3.x
		*pdwMajor = 4;
		*pdwMinor = 0;
		hRet = S_OK;
	}

	return hRet;
}

// Shell Versions:
//   Win95/WinNT 4.0                    maj=4 min=00
//   IE 3.x, IE 4.0 without Web Integrated Desktop  maj=4 min=00
//   IE 4.0 with Web Integrated Desktop         maj=4 min=71
//   IE 4.01 with Web Integrated Desktop        maj=4 min=72
inline HRESULT AtlGetShellVersion(LPDWORD pdwMajor, LPDWORD pdwMinor)
{
	ATLASSERT(pdwMajor != NULL && pdwMinor != NULL);
	if(pdwMajor == NULL || pdwMinor == NULL)
		return E_INVALIDARG;

	DLLVERSIONINFO dvi;
	::ZeroMemory(&dvi, sizeof(dvi));
	dvi.cbSize = sizeof(dvi);
	HRESULT hRet = AtlGetDllVersion(_T("shell32.dll"), &dvi);

	if(SUCCEEDED(hRet))
	{
		*pdwMajor = dvi.dwMajorVersion;
		*pdwMinor = dvi.dwMinorVersion;
	}
	else if(hRet == E_NOTIMPL)
	{
		// If DllGetVersion is not there, then the DLL is a version
		// previous to the one shipped with IE 4.x
		*pdwMajor = 4;
		*pdwMinor = 0;
		hRet = S_OK;
	}

	return hRet;
}

}; //namespace ATL

#endif //(_ATL_VER < 0x0700) && defined(_ATL_DLL) && !defined(_WIN32_WCE)


// These are always included
#include <WTL\atlwinx.h>
#include <WTL\atluser.h>
#include <WTL\atlgdi.h>

#ifndef _WTL_NO_AUTOMATIC_NAMESPACE
using namespace WTL;
#endif //!_WTL_NO_AUTOMATIC_NAMESPACE

#endif // __ATLAPP_H__
