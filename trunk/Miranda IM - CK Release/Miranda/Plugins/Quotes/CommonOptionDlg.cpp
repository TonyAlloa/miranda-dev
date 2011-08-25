#include "StdAfx.h"
#include "CommonOptionDlg.h"
#include "QuotesProviderBase.h"
#include "resource.h"
#include "EconomicRateInfo.h"
#include "DBUtils.h"
#include "QuotesProviderVisitorDbSettings.h"
#include "WinCtrlHelper.h"
#include "SettingsDlg.h"

namespace
{
	typedef boost::shared_ptr<CAdvProviderSettings> TAdvSettingsPtr;
	typedef std::map<const IQuotesProvider*,TAdvSettingsPtr> TAdvSettings;

	TAdvSettings g_aAdvSettings;

	CAdvProviderSettings* get_adv_settings(const IQuotesProvider* pProvider,bool bCreateIfNonExist)
	{
		TAdvSettings::iterator i = g_aAdvSettings.find(pProvider);
		if(i != g_aAdvSettings.end())
		{
			return i->second.get();
		}
		else if(true == bCreateIfNonExist)
		{
			TAdvSettingsPtr pAdvSet(new CAdvProviderSettings(pProvider));
			g_aAdvSettings.insert(std::make_pair(pProvider,pAdvSet));
			return pAdvSet.get();
		}
		else
		{
			return NULL;
		}
	}

	void remove_adv_settings(const IQuotesProvider* pProvider)
	{
		TAdvSettings::iterator i = g_aAdvSettings.find(pProvider);
		if(i != g_aAdvSettings.end())
		{
			g_aAdvSettings.erase(i);
		}
	}
}

void CommonOptionDlgProc(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp,CCommonDlgProcData& rData)
{
// 	USES_CONVERSION;

	switch(msg)
	{
	case WM_INITDIALOG:
		{
			assert(rData.m_pQuotesProvider);
		
			CQuotesProviderVisitorDbSettings visitor;
			rData.m_pQuotesProvider->Accept(visitor);
			assert(visitor.m_pszDbRefreshRateType);
			assert(visitor.m_pszDbRefreshRateValue);
			assert(visitor.m_pszDbDisplayNameFormat);
			assert(visitor.m_pszDbStatusMsgFormat);

			// set contact list display format
			tstring sDspNameFrmt = Quotes_DBGetStringT(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbDisplayNameFormat,visitor.m_pszDefDisplayFormat);
			::SetDlgItemText(hWnd,IDC_EDIT_CONTACT_LIST_FORMAT,sDspNameFrmt.c_str());

			// set status message display format
			tstring sStatusMsgFrmt = Quotes_DBGetStringT(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbStatusMsgFormat,visitor.m_pszDefStatusMsgFormat);
			::SetDlgItemText(hWnd,IDC_EDIT_STATUS_MESSAGE_FORMAT,sStatusMsgFrmt.c_str());

			// refresh rate
			HWND hwndCombo = ::GetDlgItem(hWnd,IDC_COMBO_REFRESH_RATE);
			LPCTSTR pszRefreshRateTypes[] = {TranslateT("Seconds"),TranslateT("Minutes"),TranslateT("Hours")};
			for(int i = 0;i < SIZEOF(pszRefreshRateTypes);++i)
			{
				::SendMessage(hwndCombo,CB_ADDSTRING,0,reinterpret_cast<LPARAM>(pszRefreshRateTypes[i]));
			}

			int nRefreshRateType = DBGetContactSettingWord(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbRefreshRateType,RRT_HOURS);
			if(nRefreshRateType < RRT_SECONDS || nRefreshRateType > RRT_HOURS)
			{
				nRefreshRateType = RRT_MINUTES;
			}

			UINT nRate = DBGetContactSettingWord(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbRefreshRateValue,3);
			switch(nRefreshRateType)
			{
			default:
			case RRT_SECONDS:
			case RRT_MINUTES:
				if(nRate < 1 || nRate > 60)
				{
					nRate = 1;
				}
				spin_set_range(::GetDlgItem(hWnd,IDC_SPIN_REFRESH_RATE),1,60);
				break;
			case RRT_HOURS:
				if(nRate < 1 || nRate > 24)
				{
					nRate = 1;
				}
				spin_set_range(::GetDlgItem(hWnd,IDC_SPIN_REFRESH_RATE),1,24);
				break;
			}

			::SendMessage(hwndCombo,CB_SETCURSEL,nRefreshRateType,0);
			::SetDlgItemInt(hWnd,IDC_EDIT_REFRESH_RATE,nRate,FALSE);
			
			PropSheet_UnChanged(::GetParent(hWnd),hWnd);
		}
		break;
	case WM_COMMAND:
		switch(HIWORD(wp))
		{
		case CBN_SELCHANGE:
			if(IDC_COMBO_REFRESH_RATE == LOWORD(wp))
			{
				ERefreshRateType nType = static_cast<ERefreshRateType>(::SendMessage(reinterpret_cast<HWND>(lp),CB_GETCURSEL,0,0));
				switch(nType)
				{
				default:
				case RRT_SECONDS:
				case RRT_MINUTES:
					spin_set_range(::GetDlgItem(hWnd,IDC_SPIN_REFRESH_RATE),1,60);
					break;
				case RRT_HOURS:
					{
						spin_set_range(::GetDlgItem(hWnd,IDC_SPIN_REFRESH_RATE),1,24);
						BOOL bOk = FALSE;
						UINT nRefreshRate = ::GetDlgItemInt(hWnd,IDC_EDIT_REFRESH_RATE,&bOk,FALSE);
						if(TRUE == bOk && nRefreshRate > 24)
						{
							::SetDlgItemInt(hWnd,IDC_EDIT_REFRESH_RATE,24,FALSE);
						}
					}
					break;
				}

				PropSheet_Changed(::GetParent(hWnd),hWnd);
			}
			break;
		case EN_CHANGE:
			switch(LOWORD(wp))
			{
			case IDC_EDIT_REFRESH_RATE:
			case IDC_EDIT_CONTACT_LIST_FORMAT:
			case IDC_EDIT_STATUS_MESSAGE_FORMAT:
				if(reinterpret_cast<HWND>(lp) == ::GetFocus())
				{
					PropSheet_Changed(::GetParent(hWnd),hWnd);
				}
				break;
			}
			break;
		case BN_CLICKED:
			switch( LOWORD(wp))
			{
			case IDC_BUTTON_DESCRIPTION:
			case IDC_BUTTON_DESCRIPTION_STATUS_MESSAGE:
				show_variable_list(hWnd,rData.m_pQuotesProvider);
				break;
			case IDC_BUTTON_ADVANCED_SETTINGS:
				{
					CAdvProviderSettings* pAdvSet = get_adv_settings(rData.m_pQuotesProvider,true);
					assert(pAdvSet);
					if(true == ShowSettingsDlg(hWnd,pAdvSet))
					{
						PropSheet_Changed(::GetParent(hWnd),hWnd);
					}
				}
				break;
			}
			break;
		}
		break;
	case WM_NOTIFY:
		{
			LPNMHDR pNMHDR = reinterpret_cast<LPNMHDR>(lp);
			switch(pNMHDR->code)
			{
			case PSN_KILLACTIVE:
				{
					BOOL bOk = FALSE;
					UINT nRefreshRate = ::GetDlgItemInt(hWnd,IDC_EDIT_REFRESH_RATE,&bOk,FALSE);
					ERefreshRateType nType = static_cast<ERefreshRateType>(::SendMessage(::GetDlgItem(hWnd,IDC_COMBO_REFRESH_RATE),CB_GETCURSEL,0,0));
					switch(nType)
					{
					default:
					case RRT_MINUTES:
					case RRT_SECONDS:
						if(FALSE == bOk || nRefreshRate < 1 || nRefreshRate > 60)
						{
							prepare_edit_ctrl_for_error(::GetDlgItem(hWnd,IDC_EDIT_REFRESH_RATE));
							Quotes_MessageBox(hWnd,TranslateT("Enter integer value between 1 and 60."),MB_OK|MB_ICONERROR);
							bOk = FALSE;
						}
						break;
					case RRT_HOURS:
						if(FALSE == bOk || nRefreshRate < 1 || nRefreshRate > 24)
						{
							prepare_edit_ctrl_for_error(::GetDlgItem(hWnd,IDC_EDIT_REFRESH_RATE));
							Quotes_MessageBox(hWnd,TranslateT("Enter integer value between 1 and 24."),MB_OK|MB_ICONERROR);
							bOk = FALSE;
						}
						break;
					}

					if(TRUE == bOk)
					{
						HWND hEdit = ::GetDlgItem(hWnd,IDC_EDIT_CONTACT_LIST_FORMAT);
						assert(IsWindow(hEdit));

						tstring s = get_window_text(hEdit);
						if(true == s.empty())
						{
							prepare_edit_ctrl_for_error(hEdit);
							Quotes_MessageBox(hWnd,TranslateT("Enter text to display in contact list."),MB_OK|MB_ICONERROR);
							bOk = FALSE;
						}
					}

					::SetWindowLongPtr(hWnd,DWLP_MSGRESULT,(TRUE == bOk) ? FALSE : TRUE);
				}
				break;
			case PSN_APPLY:
				{
					BOOL bOk = FALSE;
					UINT nRefreshRate = ::GetDlgItemInt(hWnd,IDC_EDIT_REFRESH_RATE,&bOk,FALSE);
					assert(TRUE == bOk);
					ERefreshRateType nType = static_cast<ERefreshRateType>(::SendMessage(::GetDlgItem(hWnd,IDC_COMBO_REFRESH_RATE),CB_GETCURSEL,0,0));

					assert(rData.m_pQuotesProvider);

					CQuotesProviderVisitorDbSettings visitor;
					rData.m_pQuotesProvider->Accept(visitor);
					assert(visitor.m_pszDbRefreshRateType);
					assert(visitor.m_pszDbRefreshRateValue);
					assert(visitor.m_pszDbDisplayNameFormat);
					assert(visitor.m_pszDbStatusMsgFormat);

					rData.m_bFireSetingsChangedEvent = true;
					DBWriteContactSettingWord(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbRefreshRateType,nType);
					DBWriteContactSettingWord(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbRefreshRateValue,nRefreshRate);

					tstring s = get_window_text(::GetDlgItem(hWnd,IDC_EDIT_CONTACT_LIST_FORMAT));
					DBWriteContactSettingTString(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbDisplayNameFormat,s.c_str());

					s = get_window_text(::GetDlgItem(hWnd,IDC_EDIT_STATUS_MESSAGE_FORMAT));
					DBWriteContactSettingTString(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbStatusMsgFormat,s.c_str());

					CAdvProviderSettings* pAdvSet = get_adv_settings(rData.m_pQuotesProvider,false);
					if(pAdvSet)
					{
						pAdvSet->SaveToDb();						
					}
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		remove_adv_settings(rData.m_pQuotesProvider);
		break;
	}
}