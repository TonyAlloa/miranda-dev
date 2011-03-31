#include "StdAfx.h"
#include "QuotesProviderBase.h"
#include "IXMLEngine.h"
#include "ModuleInfo.h"
#include "CreateFilePath.h"
#include "EconomicRateInfo.h"
#include "DBUtils.h"
#include "Locale.h"
#include "ExtraImages.h"
#include "QuotesProviderVisitor.h"
#include "QuotesProviderVisitorFormater.h"
#include "QuotesProviderVisitorDbSettings.h"
#include "IsWithinAccuracy.h"
//#include "Log.h"
#include "SettingsDlg.h"
#include "IconLib.h"

extern int g_nStatus;
extern HANDLE g_hEventWorkThreadStop;


struct CQuotesProviderBase::CXMLFileInfo
{
	CXMLFileInfo() : m_qs(_T("Unknown")){}
	IQuotesProvider::CProviderInfo m_pi;
	CQuotesProviderBase::CQuoteSection m_qs;
	tstring m_sURL;
};

namespace
{
	inline tstring get_ini_file_name(LPCTSTR pszFileName)
	{
		return CreateFilePath(pszFileName);
	}

	bool parse_quote(const IXMLNode::TXMLNodePtr& pTop,CQuotesProviderBase::CQuote& q)
	{
		tstring sSymbol;
		tstring sDescription;
		tstring sID;

		size_t cChild = pTop->GetChildCount();
		for(size_t i = 0;i < cChild;++i)
		{
			IXMLNode::TXMLNodePtr pNode = pTop->GetChildNode(i);
			tstring sName = pNode->GetName();
			if(0 == quotes_stricmp(_T("symbol"),sName.c_str()))
			{	
				sSymbol = pNode->GetText();
				if(true == sSymbol.empty())
				{
					return false;
				}
			}
			else if(0 == quotes_stricmp(_T("description"),sName.c_str()))
			{	
				sDescription = pNode->GetText();
			}
			else if(0 == quotes_stricmp(_T("id"),sName.c_str()))
			{		
				sID = pNode->GetText();
				if(true == sID.empty())
				{
					return false;
				}
			}
		}

		q = CQuotesProviderBase::CQuote(sID,TranslateTS(sSymbol.c_str()),TranslateTS(sDescription.c_str()));
		return true;
	}

	bool parse_section(const IXMLNode::TXMLNodePtr& pTop,CQuotesProviderBase::CQuoteSection& qs)
	{
		CQuotesProviderBase::CQuoteSection::TSections aSections;
		CQuotesProviderBase::CQuoteSection::TQuotes aQuotes;
		tstring sSectionName;

		size_t cChild = pTop->GetChildCount();
		for(size_t i = 0;i < cChild;++i)
		{
			IXMLNode::TXMLNodePtr pNode = pTop->GetChildNode(i);
			tstring sName = pNode->GetName();
			if(0 == quotes_stricmp(_T("section"),sName.c_str()))
			{					
				CQuotesProviderBase::CQuoteSection qs;
				if(true == parse_section(pNode,qs))
				{
					aSections.push_back(qs);
				}
			}
			else if(0 == quotes_stricmp(_T("quote"),sName.c_str()))
			{
				CQuotesProviderBase::CQuote q;
				if(true == parse_quote(pNode,q))
				{
					aQuotes.push_back(q);
				}
			}
			else if(0 == quotes_stricmp(_T("name"),sName.c_str()))
			{
				sSectionName = pNode->GetText();
				if(true == sSectionName.empty())
				{
					return false;
				}
			}
		}

		qs = CQuotesProviderBase::CQuoteSection(TranslateTS(sSectionName.c_str()),aSections,aQuotes);
		return true;
	}

	IXMLNode::TXMLNodePtr find_provider(const IXMLNode::TXMLNodePtr& pRoot)
	{
		IXMLNode::TXMLNodePtr pProvider;
		size_t cChild = pRoot->GetChildCount();
		for(size_t i = 0;i < cChild;++i)
		{
			IXMLNode::TXMLNodePtr pNode = pRoot->GetChildNode(i);
			tstring sName = pNode->GetName();
			if(0 == quotes_stricmp(_T("Provider"),sName.c_str()))
			{
				pProvider = pNode;
				break;
			}
			else
			{
				pProvider = find_provider(pNode);
				if(pProvider)
				{
					break;
				}
			}
		}

		return pProvider;
	}

	CQuotesProviderBase::CXMLFileInfo parse_ini_file(const tstring& rsXMLFile)
	{
		CQuotesProviderBase::CXMLFileInfo res;
		CQuotesProviderBase::CQuoteSection::TSections aSections;

		const CModuleInfo::TXMLEnginePtr& pXMLEngine = CModuleInfo::GetXMLEnginePtr();
		IXMLNode::TXMLNodePtr pRoot = pXMLEngine->LoadFile(rsXMLFile);
		if(pRoot)
		{
			IXMLNode::TXMLNodePtr pProvider = find_provider(pRoot);
			if(pProvider)
			{
				size_t cChild = pProvider->GetChildCount();
				for(size_t i = 0;i < cChild;++i)
				{
					IXMLNode::TXMLNodePtr pNode = pProvider->GetChildNode(i);
					tstring sName = pNode->GetName();
					if(0 == quotes_stricmp(_T("section"),sName.c_str()))
					{					
						CQuotesProviderBase::CQuoteSection qs;
						if(true == parse_section(pNode,qs))
						{
							aSections.push_back(qs);
						}
					}
					else if(0 == quotes_stricmp(_T("Name"),sName.c_str()))
					{
						res.m_pi.m_sName = pNode->GetText();
					}
					else if(0 == quotes_stricmp(_T("ref"),sName.c_str()))
					{
						res.m_pi.m_sURL = pNode->GetText();
					}
					else if(0 == quotes_stricmp(_T("url"),sName.c_str()))
					{
						res.m_sURL = pNode->GetText();
					}
				}
			}
		}
		res.m_qs = CQuotesProviderBase::CQuoteSection(res.m_pi.m_sName,aSections);
		return res;
	}

	CQuotesProviderBase::CXMLFileInfo init_xml_info(LPCTSTR pszFileName)
	{
		tstring sIniFile = get_ini_file_name(pszFileName);
		return parse_ini_file(sIniFile);
	}
}

CQuotesProviderBase::CQuotesProviderBase()
					: m_hEventSettingsChanged(::CreateEvent(NULL,FALSE,FALSE,NULL)),
					  m_hEventRefreshContact(::CreateEvent(NULL,FALSE,FALSE,NULL)),
					  m_bRefreshInProgress(false)
{
}

CQuotesProviderBase::~CQuotesProviderBase()
{
	::CloseHandle(m_hEventSettingsChanged);
	::CloseHandle(m_hEventRefreshContact);
}

CQuotesProviderBase::CXMLFileInfo* CQuotesProviderBase::GetXMLFileInfo()const
{
	if(!m_pXMLInfo)
	{
		CQuotesProviderVisitorDbSettings visitor;
		Accept(visitor);
		assert(visitor.m_pszXMLIniFileName);
		m_pXMLInfo.reset(new CXMLFileInfo(init_xml_info(visitor.m_pszXMLIniFileName)));
	}

	return m_pXMLInfo.get();
}

const CQuotesProviderBase::CProviderInfo& CQuotesProviderBase::GetInfo()const
{
	return GetXMLFileInfo()->m_pi;
}

const CQuotesProviderBase::CQuoteSection& CQuotesProviderBase::GetQuotes()const
{
	return GetXMLFileInfo()->m_qs;
}

const tstring& CQuotesProviderBase::GetURL()const
{
	return GetXMLFileInfo()->m_sURL;
}

bool CQuotesProviderBase::IsOnline()
{
	return ID_STATUS_ONLINE == g_nStatus;
}

void CQuotesProviderBase::AddContact(HANDLE hContact)
{
	// 	CCritSection cs(m_cs);
	assert(m_aContacts.end() == std::find(m_aContacts.begin(),m_aContacts.end(),hContact));

	m_aContacts.push_back(hContact);
}

void CQuotesProviderBase::DeleteContact(HANDLE hContact)
{
	CGuard<CLightMutex> cs(m_cs);

	TContracts::iterator i = std::find(m_aContacts.begin(),m_aContacts.end(),hContact);
	if(i != m_aContacts.end())
	{
		m_aContacts.erase(i);
	}
}

void CQuotesProviderBase::SetContactStatus(HANDLE hContact,int nNewStatus)
{
	int nStatus = DBGetContactSettingWord(hContact,QUOTES_PROTOCOL_NAME,DB_STR_STATUS,ID_STATUS_OFFLINE);
	if(nNewStatus != nStatus)
	{
		DBWriteContactSettingWord(hContact,QUOTES_PROTOCOL_NAME,DB_STR_STATUS,nNewStatus);

		if(ID_STATUS_ONLINE != nNewStatus)
		{
			DBDeleteContactSetting(hContact,LIST_MODULE_NAME,STATUS_MSG_NAME);
			tstring sSymbol = Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_SYMBOL);
			if(false == sSymbol.empty())
			{
				DBWriteContactSettingTString(hContact,LIST_MODULE_NAME,CONTACT_LIST_NAME,sSymbol.c_str());
			}

			CExtraImages::GetInstance().SetContactExtraImage(hContact,CExtraImages::eiEmpty);
		}
	}
}

namespace
{

// 	tstring format_double(double dValue,int nWidth)
// 	{
// 		tostringstream o;
// 		o.imbue(GetSystemLocale());
// 
// 		if(nWidth > 0 && nWidth <= 9)
// 		{
// 			o << std::setprecision(nWidth) << std::showpoint << std::fixed;
// 		}
// 		o << dValue;
// 
// 		return o.str();
// 	}

	tstring format_rate(const IQuotesProvider* pProvider,
						HANDLE hContact,
						const tstring& rsFrmt,
						double dRate)
	{
		tstring sResult;

		for(tstring::const_iterator i = rsFrmt.begin();i != rsFrmt.end();)
		{
			TCHAR chr = *i;
			switch(chr)
			{
			default:
				sResult += chr;
				++i;
				break;
			case _T('\\'):
				++i;
				if(i != rsFrmt.end())
				{
					TCHAR t = *i;
					switch(t)
					{
					case _T('%'):sResult += _T("%"); break;
					case _T('t'):sResult += _T("\t"); break;
					case _T('n'):sResult += _T("\n"); break;
					case _T('\\'):sResult += _T("\\"); break;
					default:sResult += chr;sResult += t;break;
					}
					++i;
				}
				else
				{
					sResult += chr;
				}
				break;
			case _T('%'):
				++i;
				if(i != rsFrmt.end())
				{
					chr = *i;

					byte nWidth = 0;
					if(::isdigit(chr))
					{
						nWidth = chr-0x30;
						++i;
						if(i == rsFrmt.end())
						{
							sResult += chr;
							break;
						}
						else
						{
							chr = *i;
						}
					}

					CQuotesProviderVisitorFormater visitor(hContact,chr,nWidth);
					pProvider->Accept(visitor);
					const tstring& s = visitor.GetResult();
					sResult += s;
					++i;
				}
				else
				{
					sResult += chr;
				}
				break;
			}
		}

		return sResult;
	}

	void log_to_file(const IQuotesProvider* pProvider,
					 HANDLE hContact,
					 double dRate,
					 const tstring& rsLogFileName,
					 const tstring& rsFormat)
	{	
// 		USES_CONVERSION;
// 		const char* pszPath = CT2A(rsLogFileName.c_str());
		std::string sPath = quotes_t2a(rsLogFileName.c_str());

		std::string::size_type n = sPath.find_last_of("\\/");
		if(std::string::npos != n)
		{
			sPath.erase(n);
		}
		DWORD dwAttributes = ::GetFileAttributesA(sPath.c_str());
		if((0xffffffff == dwAttributes) || (0 == (dwAttributes&FILE_ATTRIBUTE_DIRECTORY)))
		{
			CallService(MS_UTILS_CREATEDIRTREE,0,reinterpret_cast<LPARAM>(sPath.c_str()));
		}

		tofstream file(rsLogFileName.c_str(),std::ios::app|std::ios::out);
		file.imbue(GetSystemLocale());
		if(file.good())
		{
			tstring s = format_rate(pProvider,hContact,rsFormat,dRate);
			file << s;
		}
	}

	void log_to_history(const IQuotesProvider* pProvider,
						HANDLE hContact,
						double dRate,
						time_t nTime,
						const tstring& rsFormat)
	{
		tstring s = format_rate(pProvider,hContact,rsFormat,dRate);

		DBEVENTINFO dbei = {0};
		dbei.cbSize = sizeof(dbei);
		dbei.szModule = QUOTES_PROTOCOL_NAME;
		dbei.timestamp = static_cast<DWORD>(nTime);
		dbei.flags = DBEF_READ|DBEF_UTF;
		dbei.eventType = EVENTTYPE_MESSAGE;

		char* psz =  mir_utf8encodeT(s.c_str());
		dbei.cbBlob = ::lstrlenA(psz)+1;
		dbei.pBlob = reinterpret_cast<PBYTE>(psz);

		CallService(MS_DB_EVENT_ADD,reinterpret_cast<WPARAM>(hContact),reinterpret_cast<LPARAM>(&dbei));
		mir_free(psz);
	}

	bool do_set_contact_extra_icon(HANDLE hContact,double dCurrRate,double dPrevRate)
	{
		bool bResult = false;
		if(true == IsWithinAccuracy(dCurrRate,dPrevRate))
		{
			bResult = CExtraImages::GetInstance().SetContactExtraImage(hContact,CExtraImages::eiNotChanged);			
		}
		else if(dCurrRate > dPrevRate)
		{
			bResult = CExtraImages::GetInstance().SetContactExtraImage(hContact,CExtraImages::eiUp);
		}
		else if(dCurrRate < dPrevRate)
		{
			bResult = CExtraImages::GetInstance().SetContactExtraImage(hContact,CExtraImages::eiDown);
		}

		return bResult;
	}

	bool show_popup(const IQuotesProvider* pProvider,
				    HANDLE hContact,
				    double dRate,
					double dPrevRate,
					bool bValidPrevRate,
				    const tstring& rsFormat,
					const CPopupSettings& ps)
	{
		if(1 == ServiceExists(MS_POPUP_ADDPOPUPT))
		{
			POPUPDATAT ppd;
			ZeroMemory((void *)&ppd, sizeof(ppd));
			ppd.lchContact = hContact;
			if((true == bValidPrevRate))
			{
				if(true == IsWithinAccuracy(dRate,dPrevRate))
				{
					ppd.lchIcon = Quotes_LoadIconEx(ICON_STR_QUOTE_NOT_CHANGED);
				}
				else if(dRate > dPrevRate)
				{
					ppd.lchIcon = Quotes_LoadIconEx(ICON_STR_QUOTE_UP);
				}
				else
				{
					ppd.lchIcon = Quotes_LoadIconEx(ICON_STR_QUOTE_DOWN);
				}
			}

			CQuotesProviderVisitorFormater visitor(hContact,_T('s'),0);
			pProvider->Accept(visitor);
			const tstring& sTitle = visitor.GetResult();
			lstrcpyn(ppd.lptzContactName,sTitle.c_str(),MAX_CONTACTNAME);

			safe_string<TCHAR> ss(variables_parsedup((TCHAR*)rsFormat.c_str(), 0, hContact));
			tstring sText = format_rate(pProvider,hContact,ss.m_p,dRate);
			lstrcpyn(ppd.lptzText,sText.c_str(),MAX_SECONDLINE);

			if(CPopupSettings::colourDefault == ps.GetColourMode())
			{
				ppd.colorText = CPopupSettings::GetDefColourText();
				ppd.colorBack = CPopupSettings::GetDefColourBk();
			}
			else
			{
				ppd.colorText = ps.GetColourText();
				ppd.colorBack = ps.GetColourBk();
			}

			switch(ps.GetDelayMode())
			{
			default:
				assert(!"Unknown popup delay mode");
			case CPopupSettings::delayFromPopup:
				ppd.iSeconds = 0;
				break;
			case CPopupSettings::delayPermanent:
				ppd.iSeconds = -1;
				break;
			case CPopupSettings::delayCustom:
				ppd.iSeconds = ps.GetDelayTimeout();
				break;
			}

			LPARAM lp = 0;
			if(false == ps.GetHistoryFlag())
			{
				lp |= 0x08;
			}

			return (0 == CallService(MS_POPUP_ADDPOPUPT,reinterpret_cast<WPARAM>(&ppd),lp));
		}
		else
		{
			return false;
		}
	}
}

void CQuotesProviderBase::WriteContactRate(HANDLE hContact,double dRate,const tstring& rsSymbol/* = ""*/)
{
	time_t nTime = ::time(NULL);

	if(false == rsSymbol.empty())
	{
		DBWriteContactSettingTString(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_SYMBOL,rsSymbol.c_str());
	}

	double dPrev = 0.0;
	bool bValidPrev = Quotes_DBReadDouble(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_CURR_VALUE,dPrev);
	if(true == bValidPrev)
	{
		Quotes_DBWriteDouble(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_PREV_VALUE,dPrev);
	}

	Quotes_DBWriteDouble(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_CURR_VALUE,dRate);
	DBWriteContactSettingDword(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_FETCH_TIME,nTime);

	tstring sSymbol = rsSymbol;

	tostringstream oNick;
	oNick.imbue(GetSystemLocale());
	if(false == m_sContactListFormat.empty())
	{
		tstring s = format_rate(this,hContact,m_sContactListFormat,dRate);
		oNick << s;
	}
	else
	{
		if(true == sSymbol.empty())
		{
			sSymbol = Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_SYMBOL);
		}
		oNick << std::setfill(_T(' ')) << std::setw(10) << std::left << sSymbol << std::setw(6) << std::right << dRate;
	}

	if(true == bValidPrev)
	{
		enum{Smart = 0,Always = 1,Never = 2};
		int nForceToAddArrow = DBGetContactSettingByte(NULL,QUOTES_PROTOCOL_NAME,"ForceToAddArrowToNick",Smart);
		bool bResult = do_set_contact_extra_icon(hContact,dRate,dPrev);
		if(((Always == nForceToAddArrow) || ((false == bResult) && (Smart == nForceToAddArrow))) && (false == IsWithinAccuracy(dRate,dPrev)))
		{
			if(dRate > dPrev)
			{
#ifdef _UNICODE
				oNick << L" ↑";
#else
				oNick << " +";
#endif
			}
			else if(dRate < dPrev)
			{
#ifdef _UNICODE
				oNick << L" ↓";
#else
				oNick << " -";
#endif
			}
		}
	}

	DBWriteContactSettingTString(hContact,LIST_MODULE_NAME,CONTACT_LIST_NAME,oNick.str().c_str());

	tstring sStatusMsg = format_rate(this,hContact,m_sStatusMsgFormat,dRate);
	if(false == sStatusMsg.empty())
	{
		DBWriteContactSettingTString(hContact,LIST_MODULE_NAME,STATUS_MSG_NAME,sStatusMsg.c_str());
	}
	else
	{
		DBDeleteContactSetting(hContact,LIST_MODULE_NAME,STATUS_MSG_NAME);
	}

	bool bUseContactSpecific = (DBGetContactSettingByte(hContact,QUOTES_PROTOCOL_NAME,DB_STR_CONTACT_SPEC_SETTINGS,0) > 0);

	CAdvProviderSettings global_settings(this);

	WORD dwMode = (bUseContactSpecific) 
		? DBGetContactSettingWord(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_LOG,static_cast<WORD>(lmDisabled))
		: global_settings.GetLogMode();
	if(dwMode&lmExternalFile)
	{
		bool bAdd = true;
		bool bOnlyIfChanged = (bUseContactSpecific)
			? (DBGetContactSettingWord(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_LOG_FILE_CONDITION,1) > 0)
			: global_settings.GetLogOnlyChangedFlag();
		if(true == bOnlyIfChanged)
		{
			bAdd = ((false == bValidPrev) || (false == IsWithinAccuracy(dRate,dPrev)));
		}
		if(true == bAdd)
		{
			tstring sLogFileName =  (bUseContactSpecific)
				? Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_LOG_FILE,global_settings.GetLogFileName().c_str())
				: global_settings.GetLogFileName();

			if(true == sSymbol.empty())
			{
				sSymbol = Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_SYMBOL);
			}

			sLogFileName = GenerateLogFileName(sLogFileName,sSymbol);

			tstring sFormat = global_settings.GetLogFormat();
			if(bUseContactSpecific)
			{
				CQuotesProviderVisitorDbSettings visitor;
				Accept(visitor);
				sFormat = Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_FORMAT_LOG_FILE,visitor.m_pszDefLogFileFormat);
			}

			log_to_file(this,hContact,dRate,sLogFileName,sFormat);
		}
	}
	if(dwMode&lmInternalHistory)
	{
		bool bAdd = true;
		bool bOnlyIfChanged = (bUseContactSpecific) 
			? (DBGetContactSettingWord(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_HISTORY_CONDITION,1) > 0)
			: global_settings.GetHistoryOnlyChangedFlag();

		if(true == bOnlyIfChanged)
		{
			bAdd = ((false == bValidPrev) || (false == IsWithinAccuracy(dRate,dPrev)));
		}
		if(true == bAdd)
		{
			tstring sFormat =  (bUseContactSpecific)
				? Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_FORMAT_HISTORY,global_settings.GetHistoryFormat().c_str())
				: global_settings.GetHistoryFormat();

			log_to_history(this,hContact,dRate,nTime,sFormat);
		}
	}

	if(dwMode&lmPopup)
	{
		bool bOnlyIfChanged = (bUseContactSpecific) 
			? (1 == DBGetContactSettingByte(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_POPUP_CONDITION,1) > 0)
			: global_settings.GetShowPopupIfValueChangedFlag();
		if((false == bOnlyIfChanged) 
			|| ((true == bOnlyIfChanged) && (true == bValidPrev) && (false == IsWithinAccuracy(dRate,dPrev))))
		{
			tstring sFormat =  (bUseContactSpecific)
				? Quotes_DBGetStringT(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_FORMAT_POPUP,global_settings.GetPopupFormat().c_str())
				: global_settings.GetPopupFormat();

			CPopupSettings ps = *(global_settings.GetPopupSettingsPtr());
			ps.InitForContact(hContact);
			show_popup(this,hContact,dRate,dPrev,bValidPrev,sFormat,ps);
		}
	}


	if((true == IsOnline()))
	{
		SetContactStatus(hContact,ID_STATUS_ONLINE);
	}
}

HANDLE CQuotesProviderBase::CreateNewContact(const tstring& rsName)
{
	HANDLE hContact = reinterpret_cast<HANDLE>(CallService(MS_DB_CONTACT_ADD,0,0));
	if(hContact)
	{
		if(0 == CallService(MS_PROTO_ADDTOCONTACT,reinterpret_cast<WPARAM>(hContact),(LPARAM)QUOTES_PROTOCOL_NAME))
		{
			tstring sProvName = GetInfo().m_sName;
			DBWriteContactSettingTString(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_PROVIDER,sProvName.c_str());
			DBWriteContactSettingTString(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_SYMBOL,rsName.c_str());
			DBWriteContactSettingTString(hContact,LIST_MODULE_NAME,CONTACT_LIST_NAME,rsName.c_str());

			{// for CCritSection
				CGuard<CLightMutex> cs(m_cs);
				m_aContacts.push_back(hContact);
			}
		}
		else
		{
			CallService(MS_DB_CONTACT_DELETE,reinterpret_cast<WPARAM>(hContact),0);
			hContact = NULL;
		}
	}

	return hContact;
}

namespace
{
	DWORD get_refresh_timeout_miliseconds(const CQuotesProviderVisitorDbSettings& visitor)
	{
		assert(visitor.m_pszDbRefreshRateType);
		assert(visitor.m_pszDbRefreshRateValue);

		int nRefreshRateType = DBGetContactSettingWord(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbRefreshRateType,RRT_MINUTES);
		if(nRefreshRateType < RRT_SECONDS || nRefreshRateType > RRT_HOURS)
		{
			nRefreshRateType = RRT_MINUTES;
		}

		DWORD nTimeout = DBGetContactSettingWord(NULL,QUOTES_MODULE_NAME,visitor.m_pszDbRefreshRateValue,1);
		switch(nRefreshRateType)
		{
		default:
		case RRT_SECONDS:
			if(nTimeout < 1 || nTimeout > 60)
			{
				nTimeout = 1;
			}
			nTimeout *= 1000;
			break;
		case RRT_MINUTES:
			if(nTimeout < 1 || nTimeout > 60)
			{
				nTimeout = 1;
			}
			nTimeout *= 1000*60;
			break;
		case RRT_HOURS:
			if(nTimeout < 1 || nTimeout > 24)
			{
				nTimeout = 1;
			}
			nTimeout *= 1000*60*60;
			break;
		}

		return nTimeout;
	}
}

namespace
{
	class CBoolGuard
	{
	public:
		CBoolGuard(bool& rb) : m_b(rb){m_b = true;}
		~CBoolGuard(){m_b = false;}

	private:
		bool m_b;
	};
}

void CQuotesProviderBase::Run()
{
	CQuotesProviderVisitorDbSettings visitor;
	Accept(visitor);

	DWORD nTimeout = get_refresh_timeout_miliseconds(visitor);
	m_sContactListFormat = Quotes_DBGetStringT(NULL,QUOTES_PROTOCOL_NAME,visitor.m_pszDbDisplayNameFormat,visitor.m_pszDefDisplayFormat);
	m_sStatusMsgFormat = Quotes_DBGetStringT(NULL,QUOTES_PROTOCOL_NAME,visitor.m_pszDbStatusMsgFormat,visitor.m_pszDefStatusMsgFormat);

	enum{
		STOP_THREAD = 0,
		SETTINGS_CHANGED = 1,
		REFRESH_CONTACT = 2,
		COUNT_SYNC_OBJECTS = 3
	};

	HANDLE anEvents[COUNT_SYNC_OBJECTS];
	anEvents[STOP_THREAD] = g_hEventWorkThreadStop;
	anEvents[SETTINGS_CHANGED] = m_hEventSettingsChanged;
	anEvents[REFRESH_CONTACT] = m_hEventRefreshContact;

	TContracts anContacts;
	{
		CGuard<CLightMutex> cs(m_cs);
		anContacts = m_aContacts;
	}

	bool bGoToBed = false;

	while(false == bGoToBed)
	{		
		{
			CBoolGuard bg(m_bRefreshInProgress);
// 			LogIt(Info,_T("Begin contacts refreshing"));
			RefreshQuotes(anContacts);
// 			LogIt(Info,_T("End contacts refreshing"));
		}
		anContacts.clear();

		DWORD dwBegin = ::GetTickCount();
		DWORD dwResult = ::WaitForMultipleObjects(COUNT_SYNC_OBJECTS,anEvents,FALSE,nTimeout);
		switch(dwResult)
		{
		case WAIT_FAILED:
			assert(!"WaitForMultipleObjects failed");
			bGoToBed = true;
			break;
		case WAIT_ABANDONED_0+STOP_THREAD:
		case WAIT_ABANDONED_0+SETTINGS_CHANGED:
		case WAIT_ABANDONED_0+REFRESH_CONTACT:
			assert(!"WaitForMultipleObjects abandoned");
		case WAIT_OBJECT_0+STOP_THREAD:
			bGoToBed = true;
			break;
		case WAIT_OBJECT_0+SETTINGS_CHANGED:
			nTimeout = get_refresh_timeout_miliseconds(visitor);
			m_sContactListFormat = Quotes_DBGetStringT(NULL,QUOTES_PROTOCOL_NAME,visitor.m_pszDbDisplayNameFormat,visitor.m_pszDefDisplayFormat);
			m_sStatusMsgFormat = Quotes_DBGetStringT(NULL,QUOTES_PROTOCOL_NAME,visitor.m_pszDbStatusMsgFormat,visitor.m_pszDefStatusMsgFormat);
			{
				CGuard<CLightMutex> cs(m_cs);
				anContacts = m_aContacts;
			}
			break;
		case WAIT_OBJECT_0+REFRESH_CONTACT:
			{
				DWORD dwTimeRest = ::GetTickCount()-dwBegin;
				if(dwTimeRest < nTimeout)
				{
					nTimeout -= dwTimeRest;
				}
				{
					CGuard<CLightMutex> cs(m_cs);
					anContacts = m_aRefreshingContacts;
					m_aRefreshingContacts.clear();
				}
			}
			break;
		case WAIT_TIMEOUT:
			nTimeout = get_refresh_timeout_miliseconds(visitor);
			{
				CGuard<CLightMutex> cs(m_cs);
				anContacts = m_aContacts;
			}
			break;
		default:
			assert(!"What is the hell?");
		}
	}

	OnEndRun();
}

void CQuotesProviderBase::OnEndRun()
{
	TContracts anContacts;
	{// for CCritSection
		CGuard<CLightMutex> cs(m_cs);
		anContacts = m_aContacts;
		m_aRefreshingContacts.clear();
	}

	CBoolGuard bg(m_bRefreshInProgress);
	std::for_each(anContacts.begin(),anContacts.end(),boost::bind(&SetContactStatus,_1,ID_STATUS_OFFLINE));
}

void CQuotesProviderBase::Accept(CQuotesProviderVisitor& visitor)const
{
	visitor.Visit(*this);
}

void CQuotesProviderBase::RefreshAll()
{
	BOOL b = ::SetEvent(m_hEventSettingsChanged);
	assert(b && "Failed to set event");
}

void CQuotesProviderBase::RefreshContact(HANDLE hContact)
{
	{// for CCritSection
		CGuard<CLightMutex> cs(m_cs);
		m_aRefreshingContacts.push_back(hContact);
	}

	BOOL b = ::SetEvent(m_hEventRefreshContact);
	assert(b && "Failed to set event");
}

void CQuotesProviderBase::SetContactExtraIcon(HANDLE hContact)const
{
// 	tstring s = DBGetStringT(hContact,LIST_MODULE_NAME,CONTACT_LIST_NAME);
// 	tostringstream o;
// 	o << "Request on " << s << " refreshing\nIs online " << IsOnline() << ", is in progress " << m_bRefreshInProgress << "\n";

	bool bResult = false;
	if(true == IsOnline() && (false == m_bRefreshInProgress))
	{
		double dCurrRate = 0.0;
		double dPrevRate = 0.0;
		if((true == Quotes_DBReadDouble(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_CURR_VALUE,dCurrRate))
			&& (true == Quotes_DBReadDouble(hContact,QUOTES_PROTOCOL_NAME,DB_STR_QUOTE_PREV_VALUE,dPrevRate))
			&& (false == m_bRefreshInProgress))
		{
// 			o << "Curr rate = " << dCurrRate << ", prev rate " << dPrevRate << "\n";
			bResult = do_set_contact_extra_icon(hContact,dCurrRate,dPrevRate);
		}
	}

// 	o << "Result is " << bResult;
// 	LogIt(Info,o.str());
}
