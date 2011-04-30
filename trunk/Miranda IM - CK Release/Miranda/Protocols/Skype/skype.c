/*

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


*/

#include "skype.h"
#include "debug.h"
#include "skypeapi.h"
#include "skypesvc.h"
#include "contacts.h"
#include "utf8.h"
#include "pthread.h"
#include "gchat.h"
#include "m_toptoolbar.h"
#include "voiceservice.h"
#include "msglist.h"
#include "memlist.h"
#include <sys/timeb.h>
#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF
#endif
#ifdef _WIN64
#pragma comment (lib, "bufferoverflowU.lib")
#endif

#pragma warning (disable: 4706) // assignment within conditional expression

struct MM_INTERFACE   mmi; 

POPUPDATAT MessagePopup;

// Exported Globals
HWND hSkypeWnd=NULL, g_hWnd=NULL, hSkypeWndSecondary=NULL;
HANDLE SkypeReady, SkypeMsgReceived, hInitChat=NULL, httbButton=NULL, FetchMessageEvent=NULL;
BOOL SkypeInitialized=FALSE, MirandaShuttingDown=FALSE, PopupServiceExists=FALSE;
BOOL UseSockets=FALSE, bSkypeOut=FALSE, bProtocolSet=FALSE, bIsImoproxy=FALSE;
char skype_path[MAX_PATH], protocol=2, *pszProxyCallout=NULL, g_szProtoName[_MAX_FNAME]="SKYPE";
int SkypeStatus=ID_STATUS_OFFLINE, hSearchThread=-1, receivers=1;
long sendwatchers = 0, rcvwatchers = 0;
UINT ControlAPIAttach, ControlAPIDiscover;
LONG AttachStatus=-1;
HINSTANCE hInst;
HANDLE hProtocolAvatarsFolder;
char DefaultAvatarsFolder[MAX_PATH+1];
DWORD mirandaVersion;

CRITICAL_SECTION RingAndEndcallMutex, QueryThreadMutex, TimeMutex;

// Module Internal Globals
PLUGINLINK *pluginLink;
HANDLE MessagePumpReady;
HANDLE hChatEvent=NULL, hChatMenu=NULL;
HANDLE hEvInitChat=NULL, hBuddyAdded=NULL;
HANDLE hMenuAddSkypeContact=NULL;

DWORD msgPumpThreadId = 0;
#ifdef SKYPEBUG_OFFLN
HANDLE GotUserstatus;
#endif

BOOL bModulesLoaded=FALSE;
char *RequestedStatus=NULL;	// To fix Skype-API Statusmode-bug
char cmdMessage[12]="MESSAGE", cmdPartner[8]="PARTNER";	// Compatibility commands



// Direct assignment of user properties to a DB-Setting
static const settings_map m_settings[]= {
		{"LANGUAGE", "Language1"},
		{"PROVINCE", "State"},
		{"CITY", "City"},
		{"PHONE_HOME", "Phone"},
		{"PHONE_OFFICE", "CompanyPhone"},
		{"PHONE_MOBILE", "Cellular"},
		{"HOMEPAGE", "Homepage"},
		{"ABOUT", "About"}
	};

// Imported Globals
extern status_map status_codes[];

BOOL (WINAPI *MyEnableThemeDialogTexture)(HANDLE, DWORD) = 0;

HMODULE hUxTheme = 0;

// function pointers, use typedefs for casting to shut up the compiler when using GetProcAddress()

typedef BOOL (WINAPI *PITA)();
typedef HANDLE (WINAPI *POTD)(HWND, LPCWSTR);
typedef UINT (WINAPI *PDTB)(HANDLE, HDC, int, int, RECT *, RECT *);
typedef UINT (WINAPI *PCTD)(HANDLE);
typedef UINT (WINAPI *PDTT)(HANDLE, HDC, int, int, LPCWSTR, int, DWORD, DWORD, RECT *);

PITA pfnIsThemeActive = 0;
POTD pfnOpenThemeData = 0;
PDTB pfnDrawThemeBackground = 0;
PCTD pfnCloseThemeData = 0;
PDTT pfnDrawThemeText = 0;

#define FIXED_TAB_SIZE 100                  // default value for fixed width tabs

typedef struct {
	char msgnum[16];
	BOOL getstatus;
	BOOL bIsRead;
	BOOL bDontMarkSeen;
	BOOL QueryMsgDirection;
	TYP_MSGLENTRY *pMsgEntry;
} fetchmsg_arg;

typedef struct {
	HANDLE hContact;
	char szId[16];
} msgsendwt_arg;

/*
 * visual styles support (XP+)
 * returns 0 on failure
 */

int InitVSApi()
{
    if((hUxTheme = LoadLibraryA("uxtheme.dll")) == 0)
        return 0;

    pfnIsThemeActive = (PITA)GetProcAddress(hUxTheme, "IsThemeActive");
    pfnOpenThemeData = (POTD)GetProcAddress(hUxTheme, "OpenThemeData");
    pfnDrawThemeBackground = (PDTB)GetProcAddress(hUxTheme, "DrawThemeBackground");
    pfnCloseThemeData = (PCTD)GetProcAddress(hUxTheme, "CloseThemeData");
    pfnDrawThemeText = (PDTT)GetProcAddress(hUxTheme, "DrawThemeText");
    
    MyEnableThemeDialogTexture = (BOOL (WINAPI *)(HANDLE, DWORD))GetProcAddress(hUxTheme, "EnableThemeDialogTexture");
    if(pfnIsThemeActive != 0 && pfnOpenThemeData != 0 && pfnDrawThemeBackground != 0 && pfnCloseThemeData != 0 && pfnDrawThemeText != 0) {
        return 1;
    }
    return 0;
}

/*
 * unload uxtheme.dll
 */

int FreeVSApi()
{
    if(hUxTheme != 0)
        FreeLibrary(hUxTheme);
    return 0;
}

// Plugin Info
PLUGININFOEX pluginInfo = {
	sizeof(PLUGININFOEX),
	"Skype Protocol",
	PLUGIN_MAKE_VERSION(0,0,0,51),
	"Support for Skype network",
	"leecher - tweety - jls17",
	"leecher@dose.0wnz.at - tweety@user.berlios.de",
	"� 2004-2011 leecher - tweety",
	"http://developer.berlios.de/projects/mgoodies/",
	UNICODE_AWARE,
	0,		//doesn't replace anything built-in
	{ 0xa71f8335, 0x7b87, 0x4432, { 0xb8, 0xa3, 0x81, 0x47, 0x94, 0x31, 0xc6, 0xf5 } } // {A71F8335-7B87-4432-B8A3-81479431C6F5}
};

#define MAPDND	1	// Map Occupied to DND status and say that you support it
//#define MAPNA   1 // Map NA status to Away and say that you support it

/*                           P R O G R A M                                */

void RegisterToDbeditorpp(void)
{
    // known modules list
    if (ServiceExists("DBEditorpp/RegisterSingleModule"))
        CallService("DBEditorpp/RegisterSingleModule", (WPARAM)SKYPE_PROTONAME, 0);
}

void RegisterToUpdate(void)
{
	//Use for the Updater plugin
	if(ServiceExists(MS_UPDATE_REGISTER)) 
	{
		Update update = {0};
		char szVersion[16];

		update.szComponentName = pluginInfo.shortName;
		update.pbVersion = (BYTE *)CreateVersionStringPlugin((PLUGININFO *)&pluginInfo, szVersion);
		update.cpbVersion = (DWORD)strlen((char *)update.pbVersion);

#ifdef _WIN64
#ifdef _UNICODE
		update.szBetaUpdateURL = "http://dose.0wnz.at/miranda/Skype/Skype_protocol_unicode_x64.zip";
#else
		update.szBetaUpdateURL = "http://dose.0wnz.at/miranda/Skype/Skype_protocol_x64.zip";
#endif
		update.szBetaVersionURL = "http://dose.0wnz.at/miranda/Skype/";
		update.pbBetaVersionPrefix = (BYTE *)"SKYPE version ";
		update.szUpdateURL = update.szBetaUpdateURL;	// FIXME!!
		update.szVersionURL = update.szBetaVersionURL; // FIXME
		update.pbVersionPrefix = update.pbBetaVersionPrefix; //FIXME
#else /* _WIN64 */
#ifdef _UNICODE
		update.szBetaUpdateURL = "http://dose.0wnz.at/miranda/Skype/Skype_protocol_unicode.zip";
#else
	    update.szBetaUpdateURL = "http://dose.0wnz.at/miranda/Skype/Skype_protocol.zip";
#endif
		update.szBetaVersionURL = "http://dose.0wnz.at/miranda/Skype/";
		update.pbBetaVersionPrefix = (BYTE *)"SKYPE version ";
#ifdef _UNICODE
		update.szUpdateURL = update.szBetaUpdateURL;	// FIXME!!
		update.szVersionURL = update.szBetaVersionURL; // FIXME
		update.pbVersionPrefix = update.pbBetaVersionPrefix; //FIXME
#else
		update.szUpdateURL = "http://addons.miranda-im.org/feed.php?dlfile=3200";
		update.szVersionURL = "http://addons.miranda-im.org/details.php?action=viewfile&id=3200";
		update.pbVersionPrefix = (BYTE *)"<span class=\"fileNameHeader\">Skype Protocol</span>";
#endif
#endif

		update.cpbVersionPrefix = (DWORD)strlen((char *)update.pbVersionPrefix);
		update.cpbBetaVersionPrefix = (DWORD)strlen((char *)update.pbBetaVersionPrefix);

		CallService(MS_UPDATE_REGISTER, 0, (WPARAM)&update);

	}
}

/*
 * ShowMessage
 *
 * Shows a popup, if the popup plugin is enabled.
 * mustShow: 1 -> If Popup-Plugin is not available/disabled, show Message
 *                in a Messagewindow
 *                If the Popup-Plugin is enabled, let the message stay on
 *                screen until someone clicks it away.
 *           0 -> If Popup-Plugin is not available/disabled, skip message
 * Returns 0 on success, -1 on failure
 *
 */
int ShowMessage(int iconID, TCHAR *lpzText, int mustShow) {



	if (DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "SuppressErrors", 0)) return -1;
	lpzText=TranslateTS(lpzText);

	if (bModulesLoaded && PopupServiceExists && ServiceExists(MS_POPUP_ADDPOPUPT) && DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UsePopup", 0) && !MirandaShuttingDown) {
		BOOL showPopup, popupWindowColor;
		unsigned int popupBackColor, popupTextColor;
		int popupTimeSec;

		popupTimeSec = DBGetContactSettingDword(NULL, SKYPE_PROTONAME, "popupTimeSecErr", 4);
		popupTextColor = DBGetContactSettingDword(NULL, SKYPE_PROTONAME, "popupTextColorErr", GetSysColor(COLOR_WINDOWTEXT));
		popupBackColor = DBGetContactSettingDword(NULL, SKYPE_PROTONAME, "popupBackColorErr", GetSysColor(COLOR_BTNFACE));
		popupWindowColor = ( 0 != DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "popupWindowColorErr", TRUE));
		showPopup = ( 0 != DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "showPopupErr", TRUE));

		MessagePopup.lchContact = NULL;
		MessagePopup.lchIcon = LoadIcon(hInst,MAKEINTRESOURCE(iconID));
		MessagePopup.colorBack = ! popupWindowColor ? popupBackColor : GetSysColor(COLOR_BTNFACE);
		MessagePopup.colorText = ! popupWindowColor ? popupTextColor : GetSysColor(COLOR_WINDOWTEXT);
		MessagePopup.iSeconds = popupTimeSec;
		MessagePopup.PluginData = (void *)1;
		
		lstrcpy(MessagePopup.lptzText, lpzText);

#ifdef _UNICODE
		mbstowcs (MessagePopup.lptzContactName, SKYPE_PROTONAME, strlen(SKYPE_PROTONAME)+1);
#else
		lstrcpy(MessagePopup.lptzContactName, SKYPE_PROTONAME);
#endif

		if(showPopup)
			CallService(MS_POPUP_ADDPOPUPT,(WPARAM)&MessagePopup,0);

		return 0;
	} 
	else {

		if (mustShow==1) MessageBox(NULL,lpzText,_T("Skype Protocol"), MB_OK | MB_ICONWARNING);
		return 0;
	}
}
#ifdef _UNICODE
int ShowMessageA(int iconID, char *lpzText, int mustShow) {
	WCHAR *lpwText;
	int iRet;
	size_t len = mbstowcs (NULL, lpzText, strlen(lpzText));
	if (len == -1 || !(lpwText = calloc(len+1,sizeof(WCHAR)))) return -1;
	mbstowcs (lpwText, lpzText, strlen(lpzText));
	iRet = ShowMessage(iconID, lpwText, mustShow);
	free (lpwText);
	return iRet;
}
#endif

// processing Hooks

int HookContactAdded(WPARAM wParam, LPARAM lParam) {
	char *szProto;

	UNREFERENCED_PARAMETER(lParam);

	szProto = (char*)CallService( MS_PROTO_GETCONTACTBASEPROTO, wParam, 0 );
	if (szProto!=NULL && !strcmp(szProto, SKYPE_PROTONAME))
		add_contextmenu((HANDLE)wParam);
	return 0;
}

int HookContactDeleted(WPARAM wParam, LPARAM lParam) {
	char *szProto;

	UNREFERENCED_PARAMETER(lParam);

	szProto = (char*)CallService( MS_PROTO_GETCONTACTBASEPROTO, wParam, 0 );
	if (szProto!=NULL && !strcmp(szProto, SKYPE_PROTONAME)) {
		DBVARIANT dbv;
		int retval;

		if (DBGetContactSettingString((HANDLE)wParam, SKYPE_PROTONAME, SKYPE_NAME, &dbv)) return 1;
		retval=SkypeSend("SET USER %s BUDDYSTATUS 1", dbv.pszVal);
		DBFreeVariant(&dbv);
		if (retval) return 1;
	}
	return 0;
}

void GetInfoThread(HANDLE hContact) {
	DBVARIANT dbv;
	int i;
	char *ptr;
	BOOL bSetNick = FALSE;
	// All properties are already handled in the WndProc, so we just consume the 
	// messages here to do proper ERROR handling
	// If you add something here, be sure to handle it in WndProc, but let it
	// fall through there so that message gets added to the queue in order to be
	// consumed by SkypeGet
	char *pszProps[] = {
		"BIRTHDAY", "COUNTRY", "SEX", "MOOD_TEXT", "TIMEZONE", "IS_VIDEO_CAPABLE"};


	LOG (("GetInfoThread started."));
	EnterCriticalSection (&QueryThreadMutex);
	if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv)) 
	{
		LOG (("GetInfoThread terminated, cannot find Skype Name for contact %08X.", hContact));
		LeaveCriticalSection (&QueryThreadMutex);
		return;
	}

	if (ptr=SkypeGet ("USER", dbv.pszVal, "DISPLAYNAME")) {
		// WndProc sets Nick accordingly
		if (*ptr) bSetNick = TRUE;
		free (ptr);
	}

	if (ptr=SkypeGet ("USER", dbv.pszVal, "FULLNAME")) {
		if (*ptr && !bSetNick && DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "ShowFullname", 1)) {
			// No Displayname and FULLNAME requested
			SkypeDBWriteContactSettingUTF8String(hContact, SKYPE_PROTONAME, "Nick", ptr);
			bSetNick = TRUE;
		}
		free (ptr);
	}

	if (!bSetNick) {
		// Still no nick set, so use SKYPE Nickname
		DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "Nick", dbv.pszVal);
	}


	for (i=0; i<sizeof(pszProps)/sizeof(pszProps[0]); i++)
		if (ptr=SkypeGet ("USER", dbv.pszVal, pszProps[i])) free (ptr);

	if (protocol >= 7) {
		// Notify about the possibility of an avatar
		ACKDATA ack = {0};
		ack.cbSize = sizeof( ACKDATA );
		ack.szModule = SKYPE_PROTONAME;
		ack.hContact = hContact;
		ack.type = ACKTYPE_AVATAR;
		ack.result = ACKRESULT_STATUS;

		CallService( MS_PROTO_BROADCASTACK, 0, ( LPARAM )&ack );
		//if (ptr=SkypeGet ("USER", dbv.pszVal, "RICH_MOOD_TEXT")) free (ptr);
	}

	for (i=0; i<sizeof(m_settings)/sizeof(m_settings[0]); i++)
		if (ptr=SkypeGet ("USER", dbv.pszVal, m_settings[i].SkypeSetting)) free (ptr);

	ProtoBroadcastAck(SKYPE_PROTONAME, hContact, ACKTYPE_GETINFO, ACKRESULT_SUCCESS, (HANDLE) 1, 0);
	LeaveCriticalSection(&QueryThreadMutex);
	DBFreeVariant(&dbv);
    LOG (("GetInfoThread terminated gracefully."));
}

time_t SkypeTime(time_t *timer) {
	struct _timeb tb;
	
	EnterCriticalSection (&TimeMutex);
	_ftime(&tb);
	if (timer) *timer=tb.time;
	LeaveCriticalSection (&TimeMutex);
	return tb.time;
}


void BasicSearchThread(char *nick) {
	PROTOSEARCHRESULT psr={0};
	char *cmd=NULL, *token=NULL, *ptr=NULL, *nextoken;
	time_t st;

    LOG (("BasicSearchThread started."));
	EnterCriticalSection (&QueryThreadMutex);
	SkypeTime(&st);
	if (SkypeSend("SEARCH USERS %s", nick)==0 && (cmd=SkypeRcvTime("USERS", st, INFINITE))) {
		if (strncmp(cmd, "ERROR", 5)) {
			psr.cbSize=sizeof(psr);
			for (token=strtok_r(cmd+5, ", ", &nextoken); token; token=strtok_r(NULL, ", ", &nextoken)) {
				psr.nick=psr.id=token;
				psr.lastName=NULL;
				psr.firstName=NULL;
				psr.email=NULL;
				if (ptr=SkypeGet("USER", token, "FULLNAME")) {
					// We cannot use strtok() to seperate first & last name here,
					// because we already use it for parsing the user list
					// So we use our own function
					if (psr.lastName=strchr(ptr, ' ')) {
						*psr.lastName=0;
						psr.lastName++;
						LOG(("BasicSearchThread: lastName=%s", psr.lastName));
					}
					psr.firstName=ptr;
					LOG(("BasicSearchThread: firstName=%s", psr.firstName));
				}
				ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_SEARCH, ACKRESULT_DATA, (HANDLE)hSearchThread, (LPARAM)(PROTOSEARCHRESULT*)&psr);
				if (ptr) free(ptr);
			}
		} else {
			OUT(cmd);
		}
		free(cmd);
	}
	ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_SEARCH, ACKRESULT_SUCCESS, (HANDLE)hSearchThread, 0);
	free(nick);
	LeaveCriticalSection(&QueryThreadMutex);
    LOG (("BasicSearchThread terminated gracefully."));
	return;
}

INT_PTR SkypeDBWriteContactSettingUTF8String(HANDLE hContact,const char *szModule,const char *szSetting,const char *val)
{
	DBCONTACTWRITESETTING cws;
	INT_PTR iRet;

	// Try to save it as UTF8 sting to DB. If this doesn't succeed (i.e. older Miranda version), we convert
	// accordingly and try to save again.

	cws.szModule=szModule;
	cws.szSetting=szSetting;
	cws.value.type=DBVT_UTF8;
	cws.value.pszVal=(char*)val;
	// DBVT_UTF8 support started with version 0.5.0.0, right...?
	if (mirandaVersion < 0x050000 || (iRet = CallService(MS_DB_CONTACT_WRITESETTING,(WPARAM)hContact,(LPARAM)&cws)))
	{
		// Failed, try to convert and then try again
		cws.value.type=DBVT_TCHAR;
		if (!(cws.value.ptszVal = make_tchar_string((const unsigned char*)val))) return -1;
		iRet = CallService(MS_DB_CONTACT_WRITESETTING,(WPARAM)hContact,(LPARAM)&cws);
		free (cws.value.pszVal);
	}
	return iRet;
}


// added by TioDuke
void GetDisplaynameThread(char *dummy) {
	DBVARIANT dbv;
	char *ptr;

	UNREFERENCED_PARAMETER(dummy);
    
	LOG(("GetDisplaynameThread started."));
	if (DBGetContactSettingString(NULL, SKYPE_PROTONAME, SKYPE_NAME, &dbv)) {
		LOG(("GetDisplaynameThread terminated."));
		return;
	}
	EnterCriticalSection(&QueryThreadMutex);
    if ((ptr=SkypeGet("USER", dbv.pszVal, "FULLNAME"))) {
		if (*ptr) SkypeDBWriteContactSettingUTF8String(NULL, SKYPE_PROTONAME, "Nick", ptr);
		free(ptr);
	}
	DBFreeVariant(&dbv);
	LeaveCriticalSection(&QueryThreadMutex);
    LOG(("GetDisplaynameThread terminated gracefully."));
}


// Starts importing history from Skype
INT_PTR ImportHistory(WPARAM wParam, LPARAM lParam) {
	DBVARIANT dbv;

	UNREFERENCED_PARAMETER(lParam);
	
	if (DBGetContactSettingByte((HANDLE)wParam, SKYPE_PROTONAME, "ChatRoom", 0)) {
		if (DBGetContactSettingString((HANDLE)wParam, SKYPE_PROTONAME, "ChatRoomID", &dbv)) return 0;
		SkypeSend ("GET CHAT %s CHATMESSAGES", dbv.pszVal);
	} else {
		if (DBGetContactSettingString((HANDLE)wParam, SKYPE_PROTONAME, SKYPE_NAME, &dbv)) return 0;
		SkypeSend("SEARCH %sS %s", cmdMessage, dbv.pszVal);
	}
	DBFreeVariant(&dbv);
	return 0;
}

int SearchFriends(void) {
	char *ptr, *token, *pStat, *nextoken;
	int iRet = 0;
	time_t st;

	SkypeTime(&st);
	if (SkypeSend("SEARCH FRIENDS")!=-1 && (ptr=SkypeRcvTime("USERS", st, INFINITE)))
	{
		if (strncmp(ptr, "ERROR", 5)) {
			if (ptr+5) {
				for (token=strtok_r(ptr+5, ", ", &nextoken); token; token=strtok_r(NULL, ", ", &nextoken)) {
					if (!(pStat = SkypeGet("USER", token, "ONLINESTATUS")))
					{
						iRet = -1;
						break;
					}
					free (pStat);
				}
			}
		} else iRet=-1;
		free(ptr);
	} else iRet=-1;
	return iRet;
}

void __cdecl SearchUsersWaitingMyAuthorization(void *dummy) {
	char *cmd, *token, *nextoken;

	UNREFERENCED_PARAMETER(dummy);

	if (SkypeSend("#UWA SEARCH USERSWAITINGMYAUTHORIZATION")) return;
	if (!(cmd=SkypeRcv("#UWA USERS", INFINITE))) return;
	if (!strncmp(cmd, "ERROR", 5)) {
		free(cmd);
		return;
	}

	token=strtok_r(cmd+10, ", ", &nextoken);
	while (token) {
		CCSDATA ccs={0};
		PROTORECVEVENT pre={0};
		HANDLE hContact;
		char *firstname=NULL, *lastname=NULL, *pCurBlob;
		
		LOG(("Awaiting auth: %s", token));
		ccs.szProtoService=PSR_AUTH;
		ccs.hContact=hContact=add_contact(token, PALF_TEMPORARY);
		ccs.wParam=0;
		ccs.lParam=(LPARAM)&pre;
		pre.flags=0;
		pre.timestamp=(DWORD)SkypeTime(NULL);

		/* blob is: */
		//DWORD protocolSpecific HANDLE hContact
		//ASCIIZ nick, firstName, lastName, e-mail, requestReason
		if (firstname=SkypeGet("USER", token, "FULLNAME"))
			if (lastname=strchr(firstname, ' ')) {
				*lastname=0;
				lastname++;
			}
	
		pre.lParam=sizeof(DWORD)+sizeof(HANDLE)+strlen(token)+5;
		if (firstname) pre.lParam+=strlen(firstname);
		if (lastname) pre.lParam+=strlen(lastname);
		if (pre.szMessage  = pCurBlob = (char *)calloc(1, pre.lParam)) {
			pCurBlob+=sizeof(DWORD); // Not used
			memcpy(pCurBlob,&hContact,sizeof(HANDLE));	pCurBlob+=sizeof(HANDLE);
			strcpy((char *)pCurBlob,token);				pCurBlob+=strlen((char *)pCurBlob)+1;
			if (firstname) {
				strcpy((char *)pCurBlob,firstname); 
				if (lastname) {
					pCurBlob+=strlen((char *)pCurBlob)+1;
					strcpy((char *)pCurBlob,lastname);
				}
			}
			CallService(MS_PROTO_CHAINRECV,0,(LPARAM)&ccs);
			free(pre.szMessage);
		}
		if (firstname) free(firstname);
		token=strtok_r(NULL, ", ", &nextoken);
	}
	free(cmd);
	return;
}

void SearchFriendsThread(char *dummy) {
	UNREFERENCED_PARAMETER(dummy);

	if (!SkypeInitialized) return;
    LOG(("SearchFriendsThread started."));
	EnterCriticalSection(&QueryThreadMutex);
	SkypeInitialized=FALSE;
	SearchFriends();
	SkypeInitialized=TRUE;
	LeaveCriticalSection(&QueryThreadMutex);
    LOG(("SearchFriendsThread terminated gracefully."));
}

void __cdecl SearchRecentChats(void *dummy) {
	char *cmd, *token, *nextoken;

	UNREFERENCED_PARAMETER(dummy);

	if (SkypeSend("#RCH SEARCH RECENTCHATS")) return;
	if (!(cmd=SkypeRcv("#RCH CHATS", INFINITE))) return;
	if (!strncmp(cmd, "ERROR", 5)) {
		free(cmd);
		return;
	}

	for (token=strtok_r(cmd+10, ", ", &nextoken); token; token=strtok_r(NULL, ", ", &nextoken)) {
		char *pszStatus = SkypeGet ("CHAT", token, "STATUS");

		if (pszStatus) {
			if (!strcmp(pszStatus, "MULTI_SUBSCRIBED")) {
				// Add chatrooms for active multisubscribed chats
				/*if (!find_chatA(token)) */{
					char *pszTopic;

					EnterCriticalSection (&QueryThreadMutex);
					ChatStart(token, TRUE);
					if (pszTopic = SkypeGet ("CHAT", token, "TOPIC")) {
						TCHAR *psztChatName, *psztTopic;

						if (!*pszTopic) {
							free (pszTopic);
							if (pszTopic = SkypeGet ("CHAT", token, "FRIENDLYNAME"));
						}
						psztChatName = make_nonutf_tchar_string((const unsigned char*)token);
						psztTopic = make_tchar_string((const unsigned char*)pszTopic);
						SetChatTopic (psztChatName, psztTopic, FALSE);
						free_nonutf_tchar_string(psztChatName);
						free (psztTopic);
						free (pszTopic);
					}
					LeaveCriticalSection (&QueryThreadMutex);
				}
			}
			free (pszStatus);
		}
	}
	free(cmd);
	return;
}


void __cdecl SkypeSystemInit(char *dummy) {
	static BOOL Initializing=FALSE;
	DBVARIANT dbv={0};

	UNREFERENCED_PARAMETER(dummy);

    LOG (("SkypeSystemInit thread started."));
	if (SkypeInitialized || Initializing) return;
	Initializing=TRUE;
// Do initial Skype-Tasks
	logoff_contacts(FALSE);
// Add friends

	if (SkypeSend(SKYPE_PROTO)==-1 || !testfor("PROTOCOL", INFINITE) ||
		SkypeSend("GET CURRENTUSERHANDLE")==-1 ||
		SkypeSend("GET PRIVILEGE SKYPEOUT")==-1) {
		Initializing=FALSE;
        LOG (("SkypeSystemInit thread stopped with failure."));
		return;	
	}

	if(DBGetContactSettingString(NULL,SKYPE_PROTONAME,"LoginUserName",&dbv) == 0) 
	{
		if (*dbv.pszVal)
		{
			char *pszUser;

			// Username is set in Plugin, therefore we need to match it
			// against CURRENTUSERHANDLE
			if (pszUser = SkypeRcv ("CURRENTUSERHANDLE", INFINITE))
			{
				memmove (pszUser, pszUser+18, strlen(pszUser+17));
				if (_stricmp(dbv.pszVal, pszUser))
				{
					char szError[256];

					// Doesn't match, maybe we have a second Skype instance we have to take
					// care of? If in doubt, let's wait a while for it to report its hWnd to us.
					LOG (("Userhandle %s doesn't match username %s from settings", pszUser, dbv.pszVal));
					if (!hSkypeWndSecondary) Sleep(5000);
					if (hSkypeWndSecondary) 
					{
						hSkypeWnd = hSkypeWndSecondary;
						hSkypeWndSecondary = NULL;
						Initializing=FALSE;
						while (testfor ("CURRENTUSERHANDLE", 0));
						LOG (("Trying to init secondary Skype instance"));
						SkypeSystemInit(dummy);
					}
					else
					{
						sprintf (szError, "Username '%s' provided by Skype API doesn't match username '%s' in "
							"your settings. Please either remove username setting in you configuration or correct "
							"it. Will not connect!", pszUser, dbv.pszVal);
						OUTPUTA (szError);
						Initializing=FALSE;
					}
				}
				free (pszUser);
			}
		}
		DBFreeVariant(&dbv);
		if (!Initializing) return;
	}

#ifdef SKYPEBUG_OFFLN
    if (!ResetEvent(GotUserstatus) || SkypeSend("GET USERSTATUS")==-1 || 
		WaitForSingleObject(GotUserstatus, INFINITE)==WAIT_FAILED) 
	{
        LOG (("SkypeSystemInit thread stopped with failure."));
		Initializing=FALSE;
		return;
	}
	if (SkypeStatus!=ID_STATUS_OFFLINE)
#endif
	if (SearchFriends()==-1) {
        LOG (("SkypeSystemInit thread stopped with failure."));
		Initializing=FALSE;
		return;	
	}
	if (protocol>=5 || bIsImoproxy) {
		SkypeSend ("CREATE APPLICATION libpurple_typing");
		testfor ("CREATE APPLICATION libpurple_typing", 2000);
	}
	if (protocol>=5) {
		SearchUsersWaitingMyAuthorization(NULL);
		if (DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseGroupchat", 0))
			SearchRecentChats(NULL);
	}
	SkypeSend("SEARCH MISSED%sS", cmdMessage);
	

#ifndef SKYPEBUG_OFFLN
	if (SkypeSend("GET USERSTATUS")==-1)
	{
        LOG (("SkypeSystemInit thread stopped with failure."));
		Initializing=FALSE;
		return;
	}
#endif
	SetTimer (g_hWnd, 1, PING_INTERVAL, NULL);
	SkypeInitialized=TRUE;
	Initializing=FALSE;
	LOG (("SkypeSystemInit thread terminated gracefully."));
	return;
}

void FirstLaunch(char *dummy) {
	int counter=0;

	UNREFERENCED_PARAMETER(dummy);

	LOG (("FirstLaunch thread started."));
	if (!DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "StartSkype", 1) || ConnectToSkypeAPI(skype_path, FALSE)==-1) 
	{
		int oldstatus=SkypeStatus;

		LOG(("OnModulesLoaded starting offline.."));	
		InterlockedExchange((long *)&SkypeStatus, ID_STATUS_OFFLINE);
		ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE) oldstatus, SkypeStatus);
	}
	if (AttachStatus==-1 || AttachStatus==SKYPECONTROLAPI_ATTACH_REFUSED || AttachStatus==SKYPECONTROLAPI_ATTACH_NOT_AVAILABLE) {
		LOG (("FirstLaunch thread stopped because of invalid Attachstatus."));
		return;
	}
	
	// When you launch Skype and Attach is Successfull, it still takes some time
	// until it becomes available for receiving messages.
	// Let's probe this with PINGing
	LOG(("CheckIfApiIsResponding Entering test loop"));
	for ( ;; ) {
		LOG(("Test #%d", counter));
		if (SkypeSend("PING")==-1) counter ++; else break;
		if (counter>=20) {
			OUTPUT(_T("Cannot reach Skype API, plugin disfunct."));
			LOG (("FirstLaunch thread stopped: cannot reach Skype API."));
			return;
		}
		Sleep(500);
	}
	LOG(("CheckIfApiIsResponding: Testing for PONG"));
	testfor("PONG", 2000); // Flush PONG from MsgQueue

	pthread_create(( pThreadFunc )SkypeSystemInit, NULL);
	LOG (("FirstLaunch thread terminated gracefully."));
}

int CreateTopToolbarButton(WPARAM wParam, LPARAM lParam) {
	TTBButton ttb={0};
	
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	ttb.cbSize = sizeof(ttb);
	ttb.dwFlags = TTBBF_VISIBLE|TTBBF_SHOWTOOLTIP|TTBBF_DRAWBORDER;
	ttb.hbBitmapDown = ttb.hbBitmapUp = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_CALL));
	ttb.pszServiceDown = ttb.pszServiceUp = SKYPEOUT_CALL;
	ttb.name=Translate("Do a SkypeOut-call");
	if ((int)(httbButton=(HANDLE)CallService(MS_TTB_ADDBUTTON, (WPARAM)&ttb, 0))==-1) httbButton=0;
	return 0;
}


int OnModulesLoaded(WPARAM wParam, LPARAM lParam) {
	bModulesLoaded=TRUE;

	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	PopupServiceExists = ServiceExists(MS_POPUP_ADDPOPUPEX);

	logoff_contacts(FALSE);

	HookEventsLoaded();
	RegisterToUpdate();
	RegisterToDbeditorpp();
	VoiceServiceModulesLoaded();
	GCInit();

	add_contextmenu(NULL);
	if ( ServiceExists( MS_GC_REGISTER )) 
	{
		GCREGISTER gcr = {0};
		static COLORREF crCols[1] = {0};
		char szEvent[MAXMODULELABELLENGTH];

		gcr.cbSize = sizeof( GCREGISTER );
		gcr.dwFlags = GC_CHANMGR | GC_TCHAR; // |GC_ACKMSG; // TODO: Not implemented yet
        gcr.ptszModuleDispName = _T("Skype protocol");
		gcr.pszModule = SKYPE_PROTONAME;
		if (CallService(MS_GC_REGISTER, 0, (LPARAM)&gcr)) 
		{
			OUTPUT(_T("Unable to register with Groupchat module!"));
		}
		_snprintf (szEvent, sizeof(szEvent), "%s\\ChatInit", SKYPE_PROTONAME);
		hInitChat = CreateHookableEvent(szEvent);
		hEvInitChat = HookEvent(szEvent, ChatInit);

		hChatEvent = HookEvent(ME_GC_EVENT, GCEventHook);
		hChatMenu = HookEvent(ME_GC_BUILDMENU, GCMenuHook);
        CreateServiceFunction (SKYPE_CHATNEW, SkypeChatCreate);
		CreateProtoService (PS_LEAVECHAT, GCOnLeaveChat);
		CreateProtoService (PS_JOINCHAT, GCOnJoinChat);
	}
	// Try folder service first
	hProtocolAvatarsFolder = NULL;
	if (ServiceExists(MS_FOLDERS_REGISTER_PATH))
	{
		char *tmpPath;

		if (!ServiceExists (MS_UTILS_REPLACEVARS) || !(tmpPath = Utils_ReplaceVars("%miranda_avatarcache%")))
			tmpPath = PROFILE_PATH;
		mir_snprintf(DefaultAvatarsFolder, sizeof(DefaultAvatarsFolder), "%s\\%s", tmpPath, SKYPE_PROTONAME);
		hProtocolAvatarsFolder = (HANDLE) FoldersRegisterCustomPath(SKYPE_PROTONAME, "Avatars Cache", DefaultAvatarsFolder);
	}
	
	if (hProtocolAvatarsFolder == NULL)
	{
		// Use defaults
		CallService(MS_DB_GETPROFILEPATH, (WPARAM) MAX_PATH, (LPARAM) DefaultAvatarsFolder);
		mir_snprintf(DefaultAvatarsFolder, sizeof(DefaultAvatarsFolder), "%s\\%s", DefaultAvatarsFolder, SKYPE_PROTONAME);
		CreateDirectoryA(DefaultAvatarsFolder, NULL);
	}

	pthread_create(( pThreadFunc )FirstLaunch, NULL);
	return 0;
}

void FetchMessageThread(fetchmsg_arg *pargs) {
	char *who=NULL, *type=NULL, *chat=NULL, *users=NULL, *msg=NULL, *status=NULL;
	char *ptr, *msgptr, szPartnerHandle[32], szBuf[128];
	int direction=0, msglen = 0;
	DWORD timestamp = 0, lwr=0;
    CCSDATA ccs={0};
    PROTORECVEVENT pre={0};
    HANDLE hContact = NULL, hDbEvent, hChat = NULL;
	DBEVENTINFO dbei={0};
	DBVARIANT dbv={0};
	fetchmsg_arg args;
	BOOL bEmoted=FALSE, isGroupChat=FALSE, bHasPartList=FALSE;
	BOOL bUseGroupChat = DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseGroupchat", 0);

	if (!pargs) return;
	args = *pargs;
	free (pargs);
	
	sprintf (szPartnerHandle, "%s_HANDLE", cmdPartner);
	pre.lParam = strtoul(args.msgnum, NULL, 10);
	if (args.bIsRead) pre.flags |= PREF_CREATEREAD;
	//pEvent = MsgList_FindMessage(pre.lParam);

	// Get Timestamp
	if (!args.pMsgEntry || !args.pMsgEntry->tEdited) {
		if (!(ptr=SkypeGet (cmdMessage, args.msgnum, "TIMESTAMP"))) return;
		if (strncmp(ptr, "ERROR", 5)) timestamp=atol(ptr);
		else timestamp=(DWORD)SkypeTime(NULL);
		free(ptr);
	} else timestamp=(DWORD)(args.pMsgEntry->tEdited);

	__try {
		// Get Chatname (also to determine if we need to relay this to a groupchat)
		if (!(chat=SkypeGetErr (cmdMessage, args.msgnum, "CHATNAME"))) __leave;
		if (hChat = find_chatA(chat)) isGroupChat=TRUE;

		// Get chat status
		if ((status=SkypeGetErr ("CHAT", chat, "STATUS")) &&
			!strcmp(status, "MULTI_SUBSCRIBED")) isGroupChat=TRUE;
		
		// Get chat type
		if (!(type=SkypeGetErr (cmdMessage, args.msgnum, "TYPE"))) __leave;
		bEmoted = strcmp(type, "EMOTED")==0;
		if (strcmp(type, "MULTI_SUBSCRIBED")==0) isGroupChat=TRUE;

		// Group chat handling
		if (strcmp(type, "TEXT") && strcmp(type, "SAID") && strcmp(type, "UNKNOWN") && !bEmoted) {
			if (bUseGroupChat) {
				BOOL bAddedMembers = FALSE;

				if (!strcmp(type,"SAWMEMBERS") || !strcmp(type, "CREATEDCHATWITH")) 
				{
					// We have a new Groupchat
					LOG(("FetchMessageThread CHAT SAWMEMBERS"));
					if (!hChat) ChatStart(chat, FALSE);
					__leave;
				}
				if (!strcmp(type,"KICKED")) 
				{
					GCDEST gcd = {0};
					GCEVENT gce = {0};
					CONTACTINFO ci = {0};

					if (!hChat) __leave;
					gcd.pszModule = SKYPE_PROTONAME;
					gcd.ptszID = make_nonutf_tchar_string((const unsigned char*)chat);
					gcd.iType = GC_EVENT_KICK;
					gce.cbSize = sizeof(GCEVENT);
					gce.pDest = &gcd;
					gce.dwFlags = GCEF_ADDTOLOG | GC_TCHAR;
					gce.time = timestamp;

					if (users=SkypeGetErr (cmdMessage, args.msgnum, "USERS")) {
						ci.hContact = find_contact(users);
						gce.ptszUID= make_nonutf_tchar_string((const unsigned char*)users);
						if (who=SkypeGetErr (cmdMessage, args.msgnum, szPartnerHandle)) {
							gce.ptszStatus= make_nonutf_tchar_string((const unsigned char*)who);
							ci.cbSize = sizeof(ci);
							ci.szProto = SKYPE_PROTONAME;
							ci.dwFlag = CNF_DISPLAY | CNF_TCHAR;
							if (ci.hContact && !CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci)) gce.ptszNick=ci.pszVal; 
							else gce.ptszNick=gce.ptszUID;
    
							CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
							RemChatContact (GetChat(gcd.ptszID), ci.hContact);
							free_nonutf_tchar_string((void*)gce.ptszStatus);
							if (ci.pszVal) miranda_sys_free (ci.pszVal);
						}
						free_nonutf_tchar_string((void*)gce.ptszUID);
					}
					free_nonutf_tchar_string((void*)gcd.ptszID);
					__leave;
				}
				if (!strcmp(type,"SETROLE")) 
				{
					GCDEST gcd = {0};
					GCEVENT gce = {0};
					CONTACTINFO ci = {0};
					gchat_contact *gcContact;
					char *pszRole;

					// FROM_HANDLE - Wer hats gesetzt
					// USERS - Wessen Rolle wurde gesetzt
					// ROLE - Die neue Rolle
					if (!hChat) __leave;
					gcd.pszModule = SKYPE_PROTONAME;
					gcd.ptszID = make_nonutf_tchar_string((const unsigned char*)chat);
					gcd.iType = GC_EVENT_REMOVESTATUS;
					gce.cbSize = sizeof(GCEVENT);
					gce.pDest = &gcd;
					gce.dwFlags = GCEF_ADDTOLOG | GC_TCHAR;
					gce.time = timestamp;

					if (users=SkypeGetErr (cmdMessage, args.msgnum, "USERS")) {
						gce.ptszUID= make_nonutf_tchar_string((const unsigned char*)users);
						if (who=SkypeGetErr (cmdMessage, args.msgnum, szPartnerHandle)) {
							ci.cbSize = sizeof(ci);
							ci.szProto = SKYPE_PROTONAME;
							ci.dwFlag = CNF_DISPLAY | CNF_TCHAR;
							ci.hContact = find_contact(who);
							if (ci.hContact && !CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci)) {
								gce.ptszText=_tcsdup(ci.pszVal); 
								miranda_sys_free (ci.pszVal);
								ci.pszVal = NULL;
							}
							else gce.ptszText=make_tchar_string((const unsigned char*)who);

							ci.hContact = find_contact(users);
							if (ci.hContact && !CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci)) gce.ptszNick=ci.pszVal; 
							else gce.ptszNick=gce.ptszUID;

							if (gcContact = GetChatContact(GetChat(gcd.ptszID), ci.hContact))
							{
								gce.ptszStatus = gcContact->szRole;
								CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
							}
							if (pszRole=SkypeGetErr (cmdMessage, args.msgnum, "ROLE")) {
								gce.ptszStatus = make_nonutf_tchar_string((const unsigned char*)pszRole);
								gcd.iType = GC_EVENT_ADDSTATUS;
								CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
								free_nonutf_tchar_string((void*)gce.ptszStatus);
								free (pszRole);
							}
							free((void*)gce.ptszText);
							if (ci.pszVal) miranda_sys_free (ci.pszVal);
						}
						free_nonutf_tchar_string((void*)gce.ptszUID);
					}
					free_nonutf_tchar_string((void*)gcd.ptszID);
					__leave;
				}
				if (!strcmp(type,"SETTOPIC")) 
				{
					GCDEST gcd = {0};
					GCEVENT gce = {0};
					CONTACTINFO ci = {0};

					LOG(("FetchMessageThread CHAT SETTOPIC"));
					gcd.pszModule = SKYPE_PROTONAME;
					gcd.ptszID = make_nonutf_tchar_string((const unsigned char*)chat);
					gcd.iType = GC_EVENT_TOPIC;
					gce.cbSize = sizeof(GCEVENT);
					gce.pDest = &gcd;
					gce.dwFlags = GCEF_ADDTOLOG | GC_TCHAR;
					gce.time = timestamp;
					if (who=SkypeGetErr (cmdMessage, args.msgnum, szPartnerHandle)) {
						ci.hContact = find_contact(who);
						gce.ptszUID = make_nonutf_tchar_string((const unsigned char*)who);
						ci.cbSize = sizeof(ci);
						ci.szProto = SKYPE_PROTONAME;
						ci.dwFlag = CNF_DISPLAY | CNF_TCHAR;
						if (ci.hContact && !CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci)) gce.ptszNick=ci.pszVal; 
						else gce.ptszNick=gce.ptszUID;

						if (ptr=SkypeGetErr (cmdMessage, args.msgnum, "BODY")) {
							gce.ptszText = make_tchar_string((const unsigned char*)ptr);
							CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
							free ((void*)gce.ptszText);
							free (ptr);
						}
						free_nonutf_tchar_string ((void*)gce.ptszUID);
						if (ci.pszVal) miranda_sys_free (ci.pszVal);
					}
					free_nonutf_tchar_string((void*)gcd.ptszID);
					__leave;
				}
				if (!strcmp(type,"LEFT") || (bAddedMembers = strcmp(type,"ADDEDMEMBERS")==0)) 
				{
					GCDEST gcd = {0};
					GCEVENT gce = {0};
					CONTACTINFO ci = {0};
					char *pszInvited = Translate("invited ");

					LOG(("FetchMessageThread CHAT LEFT or ADDEDMEMBERS"));
					if (bAddedMembers) {
						gcd.pszModule = SKYPE_PROTONAME;
						gcd.ptszID = make_nonutf_tchar_string((const unsigned char*)chat);
						gcd.iType = GC_EVENT_ACTION;
						gce.cbSize = sizeof(GCEVENT);
						gce.pDest = &gcd;
						gce.dwFlags = GCEF_ADDTOLOG | GC_TCHAR;
						gce.time = timestamp;
						if (users=SkypeGetErr (cmdMessage, args.msgnum, "USERS")) {
							// We assume that users buffer has enough room for "invited" string
							memmove (users+strlen(pszInvited), users, strlen(users)+1);
							memcpy (users, pszInvited, strlen(pszInvited));
							gce.ptszText= make_tchar_string((const unsigned char*)users);
							if (who=SkypeGetErr (cmdMessage, args.msgnum, szPartnerHandle)) {
								DBVARIANT dbv;

								if (DBGetContactSettingString(NULL, SKYPE_PROTONAME, SKYPE_NAME, &dbv)==0) {
									gce.bIsMe = strcmp(who, dbv.pszVal)==0;
									DBFreeVariant(&dbv);
								}
								if (!gce.bIsMe) ci.hContact = find_contact(who);
								gce.ptszUID= make_nonutf_tchar_string((const unsigned char*)who);							
								ci.cbSize = sizeof(ci);
								ci.szProto = SKYPE_PROTONAME;
								ci.dwFlag = CNF_DISPLAY | CNF_TCHAR;
								if (!CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci)) gce.ptszNick=ci.pszVal; 
								else gce.ptszNick=gce.ptszUID;
        
								CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
								free_nonutf_tchar_string((void*)gce.ptszUID);
								if (ci.pszVal) miranda_sys_free (ci.pszVal);
							}
							if (gce.ptszText) free ((void*)gce.ptszText);
						}
						free_nonutf_tchar_string ((void*)gcd.ptszID);
					}
					if (!args.QueryMsgDirection) SkypeSend ("GET CHAT %s MEMBERS", chat);
					__leave;
				}
			}
			__leave;
		}

		// Need to get the status?
		if (args.getstatus) {
			char *status;

			if (protocol<4) InterlockedIncrement (&rcvwatchers);
			status=SkypeGetID(cmdMessage, args.msgnum, "STATUS");
			if (protocol<4) InterlockedDecrement (&rcvwatchers);
			if (!status) __leave;
			if (!strcmp(status, "SENT")) direction=DBEF_SENT;
			free(status);
		}

		// Who sent it?
		if (!(who=SkypeGetErr (cmdMessage, args.msgnum, szPartnerHandle))) __leave;

		// Get contact handle
		LOG(("FetchMessageThread Finding contact handle"));
		DBGetContactSettingString(NULL, SKYPE_PROTONAME, SKYPE_NAME, &dbv);
		if (dbv.pszVal && !strcmp (who, dbv.pszVal))
		{
			char *pTok, *nextoken;

			// It's from me.. But to whom?
			// CHATMESSAGE .. USERS doesn't return anything, so we have to query the CHAT-Object
			if (!(ptr=SkypeGetErr ("CHAT", chat, "ACTIVEMEMBERS"))) {
				DBFreeVariant (&dbv);
				__leave;
			}

			for (pTok = strtok_r (ptr, " ", &nextoken); pTok; pTok=strtok_r(NULL, " ", &nextoken)) {
				if (strcmp (pTok, dbv.pszVal)) break; // Take the first dude in the list who is not me
			}

			if (!pTok) {
				free (ptr);
				DBFreeVariant (&dbv);
				__leave; // We failed
			}
			free (who);
			who=memmove (ptr, pTok, strlen(pTok)+1);
			direction = DBEF_SENT;
		}
		DBFreeVariant (&dbv);

		if (!(hContact=find_contact(who))) {
			// Permanent adding of user obsolete, we use the BUDDYSTATUS now (bug #0000005)
			ResetEvent(hBuddyAdded);
			SkypeSend("GET USER %s BUDDYSTATUS", who);
			WaitForSingleObject(hBuddyAdded, INFINITE);
			if (!(hContact=find_contact(who))) {
				// Arrgh, the user has been deleted from contact list.
				// In this case, we add him temp. to receive the msg at least.
				hContact=add_contact(who, PALF_TEMPORARY);			
			}
		}
		// Text which was sent (on edited msg, BODY may already be in queue, check)
		sprintf (szBuf, "GET %s %s BODY", cmdMessage, args.msgnum);
		if (!args.pMsgEntry || !args.pMsgEntry->tEdited || !(ptr=SkypeRcv(szBuf+4, 1000)))
		{
			if (SkypeSend(szBuf)==-1 || !(ptr=SkypeRcv(szBuf+4, INFINITE)))
				__leave;
		}
		if (strncmp(ptr, "ERROR", 5)) {
			msgptr = ptr+strlen(szBuf+4)+1;
			bHasPartList = strncmp(msgptr,"<partlist ",10)==0;
			if (args.pMsgEntry && args.pMsgEntry->tEdited) {
				// Mark the message as edited
				if (!*msgptr && args.pMsgEntry->hEvent != INVALID_HANDLE_VALUE) {
					// Empty message and edited -> Delete event
					if ((int)(hContact = (HANDLE)CallService (MS_DB_EVENT_GETCONTACT, (WPARAM)args.pMsgEntry->hEvent, 0)) != -1) {
						CallService (MS_DB_EVENT_DELETE, (WPARAM)hContact, (LPARAM)args.pMsgEntry->hEvent);
						free (ptr);
						__leave;
					}
				} else {
					msgptr-=9;
					memcpy (msgptr, "[EDITED] ", 9);
				}
			}
			if( bEmoted && !isGroupChat) {
				CONTACTINFO ci = {0};
				int newlen;
				char *pMsg, *pszUTFnick=NULL;
				ci.cbSize = sizeof(ci);
				ci.szProto = SKYPE_PROTONAME;
				ci.dwFlag = CNF_DISPLAY | CNF_TCHAR;
				if (ci.hContact = hContact) {
					CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci);
					if (ci.pszVal) {
#ifdef _UNICODE
						pszUTFnick = (char*)make_utf8_string(ci.pszVal);
#else
						utf8_encode (ci.pszVal, &pszUTFnick);
#endif
						miranda_sys_free (ci.pszVal);
					}
				}
				newlen = strlen(msgptr) + (pszUTFnick?strlen(pszUTFnick):0) + 9;
				if (pMsg = malloc(newlen)) {
					sprintf (pMsg, "** %s%s%s **", (pszUTFnick?pszUTFnick:""),(pszUTFnick?" ":""),(char*)msgptr);
					free (ptr);
					ptr = msgptr = pMsg;
				}
				if (pszUTFnick) free(pszUTFnick);
			}

			if (mirandaVersion >= 0x070000 &&	// 0.7.0+ supports PREF_UTF flag, no need to decode UTF8
				!isGroupChat) {				// I guess Groupchat doesn't support UTF8?
				msg = ptr;
				pre.flags |= PREF_UTF;
			} else {	// Older version has to decode either UTF8->ANSI or UTF8->UNICODE
				// This could be replaced by mir_getUTFI - functions for Miranda 0.5+ builds, but we stay
				// 0.4 compatible for backwards compatibility. Unfortunately this requires us to link with utf8.c
#ifdef _UNICODE
				int wcLen;
#endif

				if (utf8_decode(msgptr, &msg)==-1) {
					free(ptr);
					__leave;
				}
#ifdef _UNICODE
				msglen = strlen(msg)+1;
				msgptr = (char*)make_unicode_string ((const unsigned char*)msgptr);
				wcLen  = (_tcslen((TCHAR*)msgptr)+1)*sizeof(TCHAR);
				msg=realloc(msg, msglen+wcLen);
				memcpy (msg+msglen, msgptr, wcLen);
				free(msgptr);
				pre.flags |= PREF_UNICODE;
#endif
				msgptr = msg;
				free (ptr);
			}
			msglen = strlen(msgptr)+1;
		} else {
			free (ptr);
			__leave;
		}
		// skype sends some xml statics after a call has finished. Check if thats the case and suppress it if necessary...
		if ((DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "SuppressCallSummaryMessage", 1) && 
			bHasPartList) || msgptr[0]==0) __leave;

		if (isGroupChat && bUseGroupChat) {
			GCDEST gcd = {0};
			GCEVENT gce = {0};
			DBVARIANT dbv = {0};
			CONTACTINFO ci = {0};

			LOG(("FetchMessageThread This is a group chat message"));
			if (!hChat) ChatStart(chat, FALSE);
			gcd.pszModule = SKYPE_PROTONAME;
			gcd.ptszID = make_nonutf_tchar_string((const unsigned char*)chat);
			gcd.iType = bEmoted?GC_EVENT_ACTION:GC_EVENT_MESSAGE;
			gce.cbSize = sizeof(GCEVENT);
			gce.pDest = &gcd;
			if ((gce.bIsMe = (direction&DBEF_SENT)?TRUE:FALSE) &&
				DBGetContactSettingString(NULL, SKYPE_PROTONAME, SKYPE_NAME, &dbv)==0)
			{
				free(who);
				who = _strdup(dbv.pszVal);
				DBFreeVariant(&dbv);
			}
			gce.ptszUID = make_nonutf_tchar_string((const unsigned char*)who);
			ci.cbSize = sizeof(ci);
			ci.szProto = SKYPE_PROTONAME;
			ci.dwFlag = CNF_DISPLAY | CNF_TCHAR;
			ci.hContact = !gce.bIsMe?hContact:NULL;
			gce.ptszNick=gce.ptszUID;
			if (!CallService(MS_CONTACT_GETCONTACTINFO,0,(LPARAM)&ci)) gce.ptszNick=ci.pszVal; 
			gce.time = timestamp>0?timestamp:(DWORD)SkypeTime(NULL);				
			gce.pszText = msgptr;
			if (pre.flags & PREF_UNICODE) gce.pszText += msglen;
			gce.dwFlags = GCEF_ADDTOLOG | GC_TCHAR;
			CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
			MsgList_Add (pre.lParam, INVALID_HANDLE_VALUE);	// Mark as groupchat
			if (ci.pszVal) miranda_sys_free (ci.pszVal);
			free_nonutf_tchar_string((void*)gce.ptszUID);
			free_nonutf_tchar_string(gcd.ptszID);

			// Yes, we have successfully read the msg
			if (!args.bDontMarkSeen)
				SkypeSend("SET %s %s SEEN", cmdMessage, args.msgnum);
			__leave;
		}

		if (args.QueryMsgDirection || (direction&DBEF_SENT)) {
			// Check if the timestamp is valid
			dbei.cbSize=sizeof(dbei);
			dbei.cbBlob=0;
			if (hDbEvent=(HANDLE)CallService(MS_DB_EVENT_FINDFIRST,(WPARAM)hContact,0)) {
				CallService(MS_DB_EVENT_GET,(WPARAM)hDbEvent,(LPARAM)&dbei);
				lwr=dbei.timestamp;
			}
			dbei.cbSize=sizeof(dbei);
			dbei.cbBlob=0;
			dbei.timestamp=0;
			if (hDbEvent=(HANDLE)CallService(MS_DB_EVENT_FINDLAST,(WPARAM)hContact,0))
				CallService(MS_DB_EVENT_GET,(WPARAM)hDbEvent,(LPARAM)&dbei);
			LOG(("FetchMessageThread timestamp %ld between %ld and %ld", timestamp, lwr, dbei.timestamp));
			if (timestamp<lwr || (direction&DBEF_SENT)) {
				LOG(("FetchMessageThread Adding event"));
				dbei.szModule=SKYPE_PROTONAME;
				dbei.cbBlob=msglen;
				if (pre.flags & PREF_UNICODE)
					dbei.cbBlob += sizeof(WCHAR)*( (DWORD)wcslen((WCHAR*)&msgptr[dbei.cbBlob])+1);
				dbei.pBlob=(PBYTE)msgptr;
				dbei.timestamp=timestamp>0?timestamp:(DWORD)SkypeTime(NULL);
				dbei.flags=direction;
				if (pre.flags & PREF_CREATEREAD) dbei.flags|=DBEF_READ;
				if (pre.flags & PREF_UTF) dbei.flags|=DBEF_UTF;
				dbei.eventType=EVENTTYPE_MESSAGE;
				MsgList_Add (pre.lParam, (HANDLE)CallServiceSync(MS_DB_EVENT_ADD, (WPARAM)(HANDLE)hContact, (LPARAM)&dbei));
			}
		}


		if (!(direction&DBEF_SENT) && (!args.QueryMsgDirection || (args.QueryMsgDirection && timestamp>dbei.timestamp))) {
			LOG(("FetchMessageThread Normal message add..."));
			// Normal message received, process it
			ccs.szProtoService = PSR_MESSAGE;
			ccs.hContact = hContact;
			ccs.wParam = 0;
			ccs.lParam = (LPARAM)&pre;
			pre.flags |= direction;	
			if(isGroupChat && DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "MarkGroupchatRead", 0))
				pre.flags |= PREF_CREATEREAD;
			pre.timestamp = timestamp>0?timestamp:(DWORD)SkypeTime(NULL);
			pre.szMessage = msgptr;
			CallServiceSync(MS_PROTO_CHAINRECV, 0, (LPARAM) &ccs);

			// Yes, we have successfully read the msg
			if (!args.bDontMarkSeen) SkypeSend("SET %s %s SEEN", cmdMessage, args.msgnum);
		}
	}
	__finally {
		if (status) free(status);
		if (msg) free(msg);
		if (users) free(users);
		if (chat) free(chat);
		if (type) free(type);
		if (who) free (who);
	}

}

void FetchMessageThreadSync(fetchmsg_arg *pargs) {
	// Secure this thread with a mutex.
	// This is needed to ensure that we get called after an old msg in the queue has
	// been added so that MsgList_FindEntry will find it.
	WaitForSingleObject (FetchMessageEvent, 30000);	// Wait max. 30 sec. for previous message fetch to complete
	if ((pargs->pMsgEntry = MsgList_FindMessage(strtoul(pargs->msgnum, NULL, 10))) && !pargs->pMsgEntry->tEdited) {
		// Better don't do this, as we set the msg as read and with this code, we would 
		// mark messages not opened by user as read which isn't that good
		/*
		if (pargs->bIsRead && pMsgEvent->hEvent != INVALID_HANDLE_VALUE)
		{
			HANDLE hContact;
			if ((int)(hContact = (HANDLE)CallService (MS_DB_EVENT_GETCONTACT, (WPARAM)pMsgEntry->hEvent, 0)) != -1)
				CallService (MS_DB_EVENT_MARKREAD, (WPARAM)hContact, (LPARAM)hDBEvent);
		}
		*/
		free (pargs);
	}
	else FetchMessageThread (pargs);
	SetEvent (FetchMessageEvent);
}

static int MsglCmpProc(const void *pstPElement,const void *pstPToFind)
{
	return strcmp ((char*)((fetchmsg_arg*)pstPElement)->pMsgEntry, (char*)((fetchmsg_arg*)pstPToFind)->pMsgEntry);
}

void MessageListProcessingThread(char *str) {
	char *token, *nextoken, *chat=NULL;
	fetchmsg_arg *args;
	TYP_LIST *hListMsgs = List_Init(32);
	int i, nCount;

	// Frst we need to sort the message timestamps
	for ((token=strtok_r(str, ", ", &nextoken)); token; token=strtok_r(NULL, ", ", &nextoken)) {
		if (args=calloc(1, sizeof(fetchmsg_arg)+sizeof(DWORD))) {
			strncpy (args->msgnum, token, sizeof(args->msgnum));
			args->getstatus=TRUE;
			args->bIsRead=TRUE;
			args->bDontMarkSeen=TRUE;
			args->QueryMsgDirection=TRUE;
			(char*)args->pMsgEntry = SkypeGet ("CHATMESSAGE", token, "TIMESTAMP"); // Bad abuse of pointer
			if (!chat) chat=SkypeGet ("CHATMESSAGE", token, "CHATNAME");
			if (args->pMsgEntry) List_InsertSort (hListMsgs, MsglCmpProc, args);
			else free(args);
		}
	}
	for (i=0, nCount=List_Count(hListMsgs); i<nCount; i++) {
		args = List_ElementAt (hListMsgs, i);
		free (args->pMsgEntry);
		args->pMsgEntry = NULL;
		FetchMessageThreadSync (args);
	}
	if (chat) {
		SkypeSend ("GET CHAT %s MEMBERS", chat);
		free (chat);
	}
	List_Exit (hListMsgs);
	free (str);
}

char *GetCallerHandle(char *szSkypeMsg) {
	return SkypeGet(szSkypeMsg, "PARTNER_HANDLE", "");
}


HANDLE GetCallerContact(char *szSkypeMsg) {
	char *szHandle;
	HANDLE hContact=NULL;

	if (!(szHandle=GetCallerHandle(szSkypeMsg))) return NULL;
	if (!(hContact=find_contact(szHandle))) {
		// If it's a SkypeOut-contact, PARTNER_HANDLE = SkypeOUT number
		DBVARIANT dbv;
		int tCompareResult;

		for (hContact=(HANDLE)CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);hContact != NULL;hContact=(HANDLE)CallService( MS_DB_CONTACT_FINDNEXT, (WPARAM)hContact, 0)) {
			if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, "SkypeOutNr", &dbv)) continue;
			tCompareResult = strcmp(dbv.pszVal, szHandle);
			DBFreeVariant(&dbv);
			if (tCompareResult) continue; else break;
		}
	}
	free(szHandle);
	if (!hContact) {LOG(("GetCallerContact Not found!"));}
	return hContact;
}

LRESULT CALLBACK InCallPopUpProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) 
{
	switch(msg)
	{
		case WM_COMMAND:
			break;

		case WM_CONTEXTMENU:
			SendMessage(hwnd,UM_DESTROYPOPUP,0,0);
			break;			
		case UM_FREEPLUGINDATA:
			//Here we'd free our own data, if we had it.
			return FALSE;
		case UM_INITPOPUP:
			break;
		case UM_DESTROYPOPUP:
			break;
		case WM_NOTIFY:
		default:
			break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

void RingThread(char *szSkypeMsg) {
	HANDLE hContact;
	DBEVENTINFO dbei={0};
	DBVARIANT dbv;
	char *ptr = NULL;

	// We use a single critical section for the RingThread- and the EndCallThread-functions
	// so that only one function is running at the same time. This is needed, because when
	// a initated and unaccepted call (which is still ringing) is hangup/canceled, skype
	// sends two messages. First "CALL xxx STATUS RINGING" .. second "CALL xx STATUS CANCELED".
	// This starts two independend threads (first: RingThread; second: EndCallThread). Now 
	// the two message are processed in reverse order sometimes. This causes the EndCallThread to
	// delete the contacts "CallId" property and after that the RingThread saves the contacts 
	// "CallId" again. After that its not possible to call this contact, because the plugin
	// thinks that there is already a call going and the hangup-function isnt working, because 
	// skype doesnt accept status-changes for finished calls. The CriticalSection syncronizes
	// the threads and the messages are processed in correct order. 
	// Not the best solution, but it works.
	EnterCriticalSection (&RingAndEndcallMutex);

  LOG(("RingThread started."));
	if (protocol >= 5) SkypeSend ("MINIMIZE");
	if (hContact=GetCallerContact(szSkypeMsg)) {
		// Make sure that an answering thread is not already in progress so that we don't get
		// the 'Incoming call' event twice
		if (!DBGetContactSettingString(hContact, SKYPE_PROTONAME, "CallId", &dbv)) {
			DBFreeVariant(&dbv);
            LOG(("RingThread terminated."));
			goto l_exitRT;
		}
		DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "CallId", szSkypeMsg);
	}
	
	if (!(ptr=SkypeGet(szSkypeMsg, "TYPE", ""))) {
        LOG(("RingThread terminated."));
		goto l_exitRT;;
	}

	if (!strncmp(ptr, "INCOMING", 8))
		NofifyVoiceService(hContact, szSkypeMsg, VOICE_STATE_RINGING);
	else
		NofifyVoiceService(hContact, szSkypeMsg, VOICE_STATE_CALLING);

	if (!strncmp(ptr, "INCOMING", 8)) {
		if (!hContact) {
			char *szHandle;
			
			if (szHandle=GetCallerHandle(szSkypeMsg)) {
				if (!(hContact=add_contact(szHandle, PALF_TEMPORARY))) {
					free(szHandle);
					goto l_exitRT;
				}
				DBDeleteContactSetting(hContact, "CList", "Hidden");
				DBWriteContactSettingWord(hContact, SKYPE_PROTONAME, "Status", (WORD)SkypeStatusToMiranda("SKYPEOUT"));
				DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "SkypeOutNr", szHandle);
				free(szHandle);
			} else goto l_exitRT;
		}
	}

	if (HasVoiceService()) {
		// Voice service will handle it
		goto l_exitRT;
	}

	dbei.cbSize=sizeof(dbei);
	dbei.eventType=EVENTTYPE_CALL;
	dbei.szModule=SKYPE_PROTONAME;
	dbei.timestamp=(DWORD)SkypeTime(NULL);
	dbei.pBlob=(unsigned char*)Translate("Phonecall");
	dbei.cbBlob=strlen((const char*)dbei.pBlob)+1;
	if (!strncmp(ptr, "INCOMING", 8)) 
	{
		CLISTEVENT cle={0};
		char toolTip[256];

		if(PopupServiceExists) 
		{
			BOOL showPopup, popupWindowColor;
			unsigned int popupBackColor, popupTextColor;
			int popupTimeSec;
			POPUPDATAT InCallPopup;
			TCHAR * lpzContactName = (TCHAR*)CallService(MS_CLIST_GETCONTACTDISPLAYNAME,(WPARAM)hContact,GCDNF_TCHAR);

			popupTimeSec = DBGetContactSettingDword(NULL, SKYPE_PROTONAME, "popupTimeSec", 4);
			popupTextColor = DBGetContactSettingDword(NULL, SKYPE_PROTONAME, "popupTextColor", GetSysColor(COLOR_WINDOWTEXT));
			popupBackColor = DBGetContactSettingDword(NULL, SKYPE_PROTONAME, "popupBackColor", GetSysColor(COLOR_BTNFACE));
			popupWindowColor = (0 != DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "popupWindowColor", TRUE));
			showPopup = (0 != DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "showPopup", TRUE));

			InCallPopup.lchContact = hContact;
			InCallPopup.lchIcon = LoadIcon(hInst,MAKEINTRESOURCE(IDI_CALL));
			InCallPopup.colorBack = ! popupWindowColor ? popupBackColor : GetSysColor(COLOR_BTNFACE);
			InCallPopup.colorText = ! popupWindowColor ? popupTextColor : GetSysColor(COLOR_WINDOWTEXT);
			InCallPopup.iSeconds = popupTimeSec;
			InCallPopup.PluginWindowProc = (WNDPROC)InCallPopUpProc;
			InCallPopup.PluginData = (void *)1;
			
			lstrcpy(InCallPopup.lptzText, TranslateT("Incoming Skype Call"));

			lstrcpy(InCallPopup.lptzContactName, lpzContactName);

			if(showPopup)
				CallService(MS_POPUP_ADDPOPUPT,(WPARAM)&InCallPopup,0);

		}
		cle.cbSize=sizeof(cle);
		cle.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_CALL));
		cle.pszService=SKYPE_ANSWERCALL;
		dbei.flags=DBEF_READ;
		cle.hContact=hContact;
		cle.hDbEvent=(HANDLE)CallService(MS_DB_EVENT_ADD,(WPARAM)hContact,(LPARAM)&dbei);
		_snprintf(toolTip,sizeof(toolTip),Translate("Incoming call from %s"),(char*)CallService(MS_CLIST_GETCONTACTDISPLAYNAME,(WPARAM)hContact,0));
		cle.pszTooltip=toolTip;
		CallServiceSync(MS_CLIST_ADDEVENT,0,(LPARAM)&cle);
	} 
	else 
	{
		dbei.flags=DBEF_SENT;
		CallService(MS_DB_EVENT_ADD,(WPARAM)hContact,(LPARAM)&dbei);
	}

l_exitRT:
	if (ptr) free (ptr);
	free(szSkypeMsg);
	LeaveCriticalSection (&RingAndEndcallMutex);
}

void EndCallThread(char *szSkypeMsg) {
	HANDLE hContact=NULL, hDbEvent;
	DBEVENTINFO dbei={0};
	DBVARIANT dbv;
	int tCompareResult;

	// We use a single critical section for the RingThread- and the EndCallThread-functions
	// so that only one function is running at the same time. This is needed, because when
	// a initated and unaccepted call (which is still ringing) is hangup/canceled, skype
	// sends two messages. First "CALL xxx STATUS RINGING" .. second "CALL xx STATUS CANCELED".
	// This starts two independend threads (first: RingThread; second: EndCallThread). Now 
	// the two message are processed in reverse order sometimes. This causes the EndCallThread to
	// delete the contacts "CallId" property and after that the RingThread saves the contacts 
	// "CallId" again. After that its not possible to call this contact, because the plugin
	// thinks that there is already a call going and the hangup-function isnt working, because 
	// skype doesnt accept status-changes for finished calls. The CriticalSection syncronizes
	// the threads and the messages are processed in correct order. 
	// Not the best solution, but it works.
	EnterCriticalSection (&RingAndEndcallMutex);

  LOG(("EndCallThread started."));
	if (szSkypeMsg) {
		for (hContact=(HANDLE)CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);hContact != NULL;hContact=(HANDLE)CallService( MS_DB_CONTACT_FINDNEXT, (WPARAM)hContact, 0)) {
			if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, "CallId", &dbv)) continue;
			tCompareResult = strcmp(dbv.pszVal, szSkypeMsg);
			DBFreeVariant(&dbv);
			if (tCompareResult) continue; else break;
		}
	}
	if (hContact)
	{
		NofifyVoiceService(hContact, szSkypeMsg, VOICE_STATE_ENDED);

		DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "CallId");

		if (!HasVoiceService()) {
			dbei.cbSize=sizeof(dbei);
			hDbEvent=(HANDLE)CallService(MS_DB_EVENT_FINDFIRSTUNREAD,(WPARAM)hContact,0);
			while(hDbEvent) {
				dbei.cbBlob=0;
				CallService(MS_DB_EVENT_GET,(WPARAM)hDbEvent,(LPARAM)&dbei);
					if (!(dbei.flags&(DBEF_SENT|DBEF_READ)) && dbei.eventType==EVENTTYPE_CALL) {
					CallService(MS_DB_EVENT_MARKREAD,(WPARAM)hContact,(LPARAM)hDbEvent);
					CallService(MS_CLIST_REMOVEEVENT,(WPARAM)hContact,(LPARAM)hDbEvent);
				}
				if (dbei.pBlob) free(dbei.pBlob);
				hDbEvent=(HANDLE)CallService(MS_DB_EVENT_FINDNEXT,(WPARAM)hDbEvent,0);
			}
		}

		if (!DBGetContactSettingString(hContact, SKYPE_PROTONAME, "SkypeOutNr", &dbv)) {
			DBFreeVariant(&dbv);
			if (!strcmp((char *)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM)hContact, 0), SKYPE_PROTONAME) && 
				DBGetContactSettingByte(hContact, "CList", "NotOnList", 0)
			   )
					CallService(MS_DB_CONTACT_DELETE, (WPARAM)hContact, 0);
		}
	}
	free(szSkypeMsg);
	LeaveCriticalSection (&RingAndEndcallMutex);
}

void HoldCallThread(char *szSkypeMsg) {
	HANDLE hContact;

	LOG(("HoldCallThread started"));
    if (!szSkypeMsg) {
		LOG(("HoldCallThread terminated."));
		return;
	}
	if (hContact=GetCallerContact(szSkypeMsg)) {
		DBWriteContactSettingByte(hContact, SKYPE_PROTONAME, "OnHold", 1);
		NofifyVoiceService(hContact, szSkypeMsg, VOICE_STATE_ON_HOLD);
	}
	free(szSkypeMsg);
	LOG(("HoldCallThread terminated gracefully"));
}

void ResumeCallThread(char *szSkypeMsg) {
	HANDLE hContact;

	LOG(("ResumeCallThread started"));
	if (!szSkypeMsg) {
		LOG(("ResumeCallThread terminated."));
		return;
	}
	if (hContact=GetCallerContact(szSkypeMsg)) {
		DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "OnHold");
		NofifyVoiceService(hContact, szSkypeMsg, VOICE_STATE_TALKING);
	}
	free(szSkypeMsg);
    LOG(("ResumeCallThread terminated gracefully."));
}

int SetUserStatus(void) {
   if (RequestedStatus && AttachStatus!=-1) {
	if (SkypeSend("SET USERSTATUS %s", RequestedStatus)==-1) return 1;
   }
   return 0;
}

void LaunchSkypeAndSetStatusThread(void *newStatus) {

/*	   if (!DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UnloadOnOffline", 0)) {
		   logoff_contacts();
		   return 1;
	   }
*/
	int oldStatus=SkypeStatus;

	LOG (("LaunchSkypeAndSetStatusThread started."));
	InterlockedExchange((long *)&SkypeStatus, (int)ID_STATUS_CONNECTING);
	ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE) oldStatus, SkypeStatus);
	   
	if (ConnectToSkypeAPI(skype_path, TRUE)!=-1) {
		pthread_create(( pThreadFunc )SkypeSystemInit, NULL);
		InterlockedExchange((long *)&SkypeStatus, (int)newStatus);
		ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE) oldStatus, SkypeStatus);
		SetUserStatus();
	}

	LOG (("LaunchSkypeAndSetStatusThread terminated gracefully."));
}

LONG APIENTRY WndProc(HWND hWndDlg, UINT message, UINT wParam, LONG lParam) 
{ 
    PCOPYDATASTRUCT CopyData; 
	char *ptr, *szSkypeMsg=NULL, *nick, *buf;
	static char *onlinestatus=NULL;
	static BOOL RestoreUserStatus=FALSE;
	int sstat, oldstatus, flag;
	HANDLE hContact;
	fetchmsg_arg *args;

    switch (message) 
    { 
        case WM_COPYDATA: 
		 LOG(("WM_COPYDATA start"));
		 if(hSkypeWnd==(HWND)wParam) { 
			char *pData;
			CopyData=(PCOPYDATASTRUCT)lParam;
			pData = (char*)CopyData->lpData;
			while (*pData==' ') pData++;
			szSkypeMsg=_strdup((char*)pData);
			ReplyMessage(1);
			LOG(("< %s", szSkypeMsg));

 			if (!strncmp(szSkypeMsg, "CONNSTATUS", 10)) {
				if (!strncmp(szSkypeMsg+11, "LOGGEDOUT", 9)) {
					SkypeInitialized=FALSE;
					ResetEvent(SkypeReady);
					AttachStatus=-1;
					sstat=ID_STATUS_OFFLINE;
				    if (g_hWnd) KillTimer (g_hWnd, 1);
					logoff_contacts(TRUE);
				} else 
					sstat=SkypeStatusToMiranda(szSkypeMsg+11);

				if (sstat) {
					oldstatus=SkypeStatus;
					InterlockedExchange((long*)&SkypeStatus, sstat);
					ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE) oldstatus, SkypeStatus);
					if (sstat!=ID_STATUS_OFFLINE) {
						if (sstat!=ID_STATUS_CONNECTING && (oldstatus==ID_STATUS_OFFLINE || oldstatus==ID_STATUS_CONNECTING)) {

							SkypeInitialized=FALSE;
							pthread_create(( pThreadFunc )SkypeSystemInit, NULL);
						}
						if (DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "KeepState", 0)) RestoreUserStatus=TRUE;
					}

//					if (SkypeStatus==ID_STATUS_ONLINE) SkypeSend("SEARCH MISSEDMESSAGES");
				}
//				break;
			}
			if (!strncmp(szSkypeMsg, "USERSTATUS", 10)) {
//				if ((sstat=SkypeStatusToMiranda(szSkypeMsg+11)) && SkypeStatus!=ID_STATUS_CONNECTING) {
				if ((sstat=SkypeStatusToMiranda(szSkypeMsg+11))) {				
						if (RestoreUserStatus && RequestedStatus) {
							RestoreUserStatus=FALSE;
							SkypeSend ("SET USERSTATUS %s", RequestedStatus);
						}
						oldstatus=SkypeStatus;
						InterlockedExchange((long*)&SkypeStatus, sstat);
						ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE) oldstatus, sstat);
#ifdef SKYPEBUG_OFFLN
						if ((oldstatus==ID_STATUS_OFFLINE || oldstatus==ID_STATUS_CONNECTING) && 
							SkypeStatus!=ID_STATUS_CONNECTING && SkypeStatus!=ID_STATUS_OFFLINE) 
							pthread_create(( pThreadFunc )SearchFriendsThread, NULL);
#endif
				}
#ifdef SKYPEBUG_OFFLN
				SetEvent(GotUserstatus);
#endif
				break;
			}
			if (!strncmp(szSkypeMsg, "APPLICATION libpurple_typing", 28)) {
				char *nextoken, *p;

				if (p=strtok_r(szSkypeMsg+29, " ", &nextoken))
				{
					if (!strcmp (p, "STREAMS")) {
						char *pStr;

						while (p=strtok_r(NULL, " ", &nextoken)) {
							if (pStr = strchr(p, ':')) {
								*pStr=0;
								if (hContact=find_contact(p)) {
									*pStr=':';
									DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "Typing_Stream", p);
								}
							}
						}
					}
					else if (!strcmp (p, "DATAGRAM")) {
						if (p=strtok_r(NULL, " ", &nextoken)) {
							char *pStr;

							if (pStr = strchr(p, ':')) {
								*pStr=0;
								if (hContact=find_contact(p)) {
									*pStr=':';
									DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "Typing_Stream", p);

									if (p=strtok_r(NULL, " ", &nextoken)) {
										LPARAM lTyping = PROTOTYPE_CONTACTTYPING_OFF;

										if (!strcmp(p, "PURPLE_TYPING")) lTyping=PROTOTYPE_CONTACTTYPING_INFINITE;
										CallService(MS_PROTO_CONTACTISTYPING, (WPARAM)hContact, lTyping);
										break;
									}
								}
							}
						}
					}
				}
			}
			if (!strncmp(szSkypeMsg, "USER ", 5)) {
				char *nextoken;

				buf=_strdup(szSkypeMsg+5);
				nick=strtok_r(buf, " ", &nextoken);
				ptr=strtok_r(NULL, " ", &nextoken);

				if (strcmp(ptr, "BUDDYSTATUS")) {
					if (!strcmp(ptr, "RECEIVEDAUTHREQUEST")) {
						pthread_create(( pThreadFunc )SearchUsersWaitingMyAuthorization, NULL);
						free (buf);
						break;
					}

					if (!(hContact=find_contact(nick)) && strcmp(ptr, "FULLNAME")) {
						SkypeSend("GET USER %s BUDDYSTATUS", nick);
						free (buf);
						break;
					} 

					if (!strcmp(ptr, "ONLINESTATUS")) {
						if (SkypeStatus!=ID_STATUS_OFFLINE)
						{
							DBWriteContactSettingWord(hContact, SKYPE_PROTONAME, "Status", (WORD)SkypeStatusToMiranda(ptr+13));
							if((WORD)SkypeStatusToMiranda(ptr+13) != ID_STATUS_OFFLINE)
							{
								LOG(("WndProc Status is not offline so get user info"));
								pthread_create(GetInfoThread, hContact);
							}
						}
					}


					/* We handle the following properties right here in the wndProc, in case that
					 * Skype protocol broadcasts them to us.
					 *
					 * However, we still let them be added to the Message queue im memory, as they
					 * may get consumed by GetInfoThread.
					 * This is necessary to have a proper error handling in case the property is
					 * not supported (i.e. imo2sproxy).
					 *
					 * If one of the property GETs returns an error, the error-message has to be
					 * removed from the message queue, as the error is the answer to the query.
					 * If we don't remove the ERRORs from the list, another consumer may see the ERROR
					 * as a reply to his query and process it.
					 * In case the SKYPE Protocol really broadcasts one of these messages without being
					 * requested by GetInfoThread (i.e. MOOD_TEXT), the garbage collector will take 
					 * care of them and remove them after some time.
					 * This may not be the most efficient way, but ensures that we finally do proper
					 * error handling.
					 */
					if (!strcmp(ptr, "FULLNAME")) {
						char *nm;

						if (nm = strtok_r(NULL, " ", &nextoken))
						{
							SkypeDBWriteContactSettingUTF8String(hContact, SKYPE_PROTONAME, "FirstName", nm);
							if (!(nm=strtok_r(NULL, "", &nextoken))) DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "LastName");
							else 
								SkypeDBWriteContactSettingUTF8String(hContact, SKYPE_PROTONAME, "LastName", nm);
						}
					} else
					if (!strcmp(ptr, "BIRTHDAY")) {
						unsigned int y, m, d;
						if (sscanf(ptr+9, "%04d%02d%02d", &y, &m, &d)==3) {
							DBWriteContactSettingWord(hContact, SKYPE_PROTONAME, "BirthYear", (WORD)y);
							DBWriteContactSettingByte(hContact, SKYPE_PROTONAME, "BirthMonth", (BYTE)m);
							DBWriteContactSettingByte(hContact, SKYPE_PROTONAME, "BirthDay", (BYTE)d);
						} else {
							DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "BirthYear");
							DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "BirthMonth");
							DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "BirthDay");
						}
					} else
					if (!strcmp(ptr, "COUNTRY")) {
						if (ptr[8]) {
							struct CountryListEntry *countries;
							int countryCount, i;

							CallService(MS_UTILS_GETCOUNTRYLIST, (WPARAM)&countryCount, (LPARAM)&countries);
							for (i=0; i<countryCount; i++) {
								if (countries[i].id == 0 || countries[i].id == 0xFFFF) continue;
								if (!_stricmp(countries[i].szName, ptr+8)) 
								{
									DBWriteContactSettingWord(hContact, SKYPE_PROTONAME, "Country", (BYTE)countries[i].id);
									break;
								}
							}
						} else DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "Country");
					} else
					if (!strcmp(ptr, "SEX")) {
						if (ptr[4]) {
							BYTE sex=0;
							if (!_stricmp(ptr+4, "MALE")) sex=0x4D;
							if (!_stricmp(ptr+4, "FEMALE")) sex=0x46;
							if (sex) DBWriteContactSettingByte(hContact, SKYPE_PROTONAME, "Gender", sex);
						} else DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "Gender");
					} else
	/*				if (!strcmp(ptr, "AVATAR" )){
						LOG("WndProc", "AVATAR");
						if (!(hContact=find_contact(nick)))
							SkypeSend("GET USER %s BUDDYSTATUS", nick);
						else
						{
							TCHAR *unicode = NULL;
							
							if(utf8_decode((ptr+9), &Avatar)==-1) break;

							if( ServiceExists(MS_AV_SETAVATAR) )
							{
								CallService(MS_AV_SETAVATAR,(WPARAM) hContact,(LPARAM) Avatar);
							}
							else
							{

								if(DBWriteContactSettingTString(hContact, "ContactPhoto", "File", Avatar)) 
								{
									#if defined( _UNICODE )
										char buff[TEXT_LEN];
										WideCharToMultiByte(code_page, 0, Avatar, -1, buff, TEXT_LEN, 0, 0);
										buff[TEXT_LEN] = 0;
										DBWriteContactSettingString(hContact, "ContactPhoto", "File", buff);
									#endif
								}

							}
														
							
						}
						free(buf);
						break;
					}
					*/
					if (!strcmp(ptr, "MOOD_TEXT")){

						LOG(("WndProc MOOD_TEXT"));
						SkypeDBWriteContactSettingUTF8String (hContact, "CList", "StatusMsg", ptr+10);
					} else
					if (!strcmp(ptr, "TIMEZONE")){
						time_t temp;
						struct tm tms;
						int value=atoi(ptr+9), tz;

						LOG(("WndProc: TIMEZONE %s", nick));

						if (value && !DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "IgnoreTimeZones", 0)) {
							temp = SkypeTime(NULL);
							tms = *localtime(&temp);
							//memcpy(&tms,localtime(&temp), sizeof(tm));
							//tms = localtime(&temp)
							tz=(value >= 86400 )?(256-((2*(atoi(ptr+9)-86400))/3600)):((-2*(atoi(ptr+9)-86400))/3600);
							if (tms.tm_isdst == 1 && DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseTimeZonePatch", 0)) 
							{
								LOG(("WndProc: Using the TimeZonePatch"));
								DBWriteContactSettingByte(hContact, "UserInfo", "Timezone", (BYTE)(tz+2));
							}
							else
							{
								LOG(("WndProc: Not using the TimeZonePatch"));
								DBWriteContactSettingByte(hContact, "UserInfo", "Timezone", (BYTE)(tz+0));
							}
						} else 	{
							LOG(("WndProc: Deleting the TimeZone in UserInfo Section"));
							DBDeleteContactSetting(hContact, "UserInfo", "Timezone");
						}
					} else
					if (!strcmp(ptr, "IS_VIDEO_CAPABLE")){
						if (!_stricmp(ptr + 17, "True"))
							DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "MirVer", "Skype 2.0");
						else
							DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "MirVer", "Skype");
					} else
					if (!strcmp(ptr, "RICH_MOOD_TEXT")) {
						DBWriteContactSettingString(hContact, SKYPE_PROTONAME, "MirVer", "Skype 3.0");
					} else
					if (!strcmp(ptr, "DISPLAYNAME")) {
						// Skype Bug? -> If nickname isn't customised in the Skype-App, this won't return anything :-(
						if (ptr[12]) 
							SkypeDBWriteContactSettingUTF8String(hContact, SKYPE_PROTONAME, "Nick", ptr+12);
					} else	// Other proerties that can be directly assigned to a DB-Value
					{
						int i;
						char *pszProp;

						for (i=0; i<sizeof(m_settings)/sizeof(m_settings[0]); i++) {
							if (!strcmp(ptr, m_settings[i].SkypeSetting)) {
								pszProp = ptr+strlen(m_settings[i].SkypeSetting)+1;
								if (*pszProp)
									SkypeDBWriteContactSettingUTF8String(hContact, SKYPE_PROTONAME, m_settings[i].MirandaSetting, pszProp);
								else
									DBDeleteContactSetting(hContact, SKYPE_PROTONAME, m_settings[i].MirandaSetting);
							}
						}
					}
				} else { // BUDDYSTATUS:
					flag=0;
					switch(atoi(ptr+12)) {
						case 1: if (hContact=find_contact(nick)) CallService(MS_DB_CONTACT_DELETE, (WPARAM)hContact, 0); break;
						case 0: break;
						case 2: flag=PALF_TEMPORARY;
						case 3: add_contact(nick, flag); 
								SkypeSend("GET USER %s ONLINESTATUS", nick);
								break;
					}
					free(buf);
					if (!SetEvent(hBuddyAdded)) TellError(GetLastError());
					break;
				}
				free(buf);
			}
			if (!strncmp(szSkypeMsg, "CURRENTUSERHANDLE", 17)) {	// My username
				DBVARIANT dbv={0};

				if(DBGetContactSettingString(NULL,SKYPE_PROTONAME,"LoginUserName",&dbv) ||
					!*dbv.pszVal || _stricmp (szSkypeMsg+18, dbv.pszVal)==0) 
				{
					DBWriteContactSettingString(NULL, SKYPE_PROTONAME, SKYPE_NAME, szSkypeMsg+18);
					DBWriteContactSettingString(NULL, SKYPE_PROTONAME, "Nick", szSkypeMsg+18);
					pthread_create(( pThreadFunc )GetDisplaynameThread, NULL);
				}
				if (dbv.pszVal) DBFreeVariant(&dbv);
			}
			if (strstr(szSkypeMsg, "AUTOAWAY") || !strncmp(szSkypeMsg, "OPEN ",5) ||
				 (SkypeInitialized && !strncmp (szSkypeMsg, "PONG", 4)) ||
				 !strncmp (szSkypeMsg, "MINIMIZE", 8)) 
			{
				// Currently we do not process these messages  
				break;
			}
			if (!strncmp(szSkypeMsg, "CHAT ", 5)) {
				// Currently we only process these notifications
				if (DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseGroupchat", 0) &&
					(ptr = strchr (szSkypeMsg, ' ')) && (ptr = strchr (++ptr, ' '))) 
				{
					if (strncmp(ptr, " MEMBERS", 8) == 0) {
						LOG(("WndProc AddMembers"));
						pthread_create(( pThreadFunc )AddMembersThread, _strdup(szSkypeMsg));
					} else 
					if (strncmp(ptr, " FRIENDLYNAME ", 14) == 0) {
						// Chat session name
						HANDLE hContact;

						*ptr=0;
						if (hContact = find_chatA(szSkypeMsg+5))
						{
							GCDEST gcd = {0};
							GCEVENT gce = {0};

							if (DBGetContactSettingWord(hContact, SKYPE_PROTONAME, "Status", ID_STATUS_OFFLINE) !=
								ID_STATUS_OFFLINE)
							{
								gcd.pszModule = SKYPE_PROTONAME;
								gcd.ptszID = make_nonutf_tchar_string((const unsigned char*)szSkypeMsg+5);
								gcd.iType = GC_EVENT_CHANGESESSIONAME;
								gce.cbSize = sizeof(GCEVENT);
								gce.pDest = &gcd;
								gce.ptszText = make_tchar_string((const unsigned char*)ptr+14);
								gce.dwFlags = GC_TCHAR;
								if (gce.ptszText) {
									CallService(MS_GC_EVENT, 0, (LPARAM)&gce);
									DBWriteContactSettingTString (hContact, SKYPE_PROTONAME, "Nick", gce.ptszText);
									free((void*)gce.ptszText);
								}
								free_nonutf_tchar_string((void*)gcd.ptszID);
							}
						}
						*ptr=' ';
					} else
					if (strncmp(ptr, " CHATMESSAGES ", 14) == 0) {
						pthread_create(( pThreadFunc )MessageListProcessingThread, _strdup(ptr+14));
						break;
					}
				}
			}
			if (!strncmp(szSkypeMsg, "CALL ",5)) {
				// incoming calls are already processed by Skype, so no need for us
				// to do this.
				// However we can give a user the possibility to hang up a call via Miranda's
				// context menu
				if (ptr=strstr(szSkypeMsg, " STATUS ")) {
					ptr[0]=0; ptr+=8;
					if (!strcmp(ptr, "RINGING") || !strcmp(ptr, "ROUTING")) pthread_create(( pThreadFunc )RingThread, _strdup(szSkypeMsg));
					if (!strcmp(ptr, "FAILED") || !strcmp(ptr, "FINISHED") ||
						!strcmp(ptr, "MISSED") || !strcmp(ptr, "REFUSED")  ||
						!strcmp(ptr, "BUSY")   || !strcmp(ptr, "CANCELLED"))
						pthread_create(( pThreadFunc )EndCallThread, _strdup(szSkypeMsg));
					if (!strcmp(ptr, "ONHOLD") || !strcmp(ptr, "LOCALHOLD") ||
						!strcmp(ptr, "REMOTEHOLD")) pthread_create(( pThreadFunc )HoldCallThread, _strdup(szSkypeMsg));
					if (!strcmp(ptr, "INPROGRESS")) pthread_create(( pThreadFunc )ResumeCallThread, _strdup(szSkypeMsg));
					break;
				} else if ((!strstr(szSkypeMsg, "PARTNER_HANDLE") && !strstr(szSkypeMsg, "FROM_HANDLE"))
							&& !strstr(szSkypeMsg, "TYPE")) break;
			}
			if (!strncmp(szSkypeMsg, "PRIVILEGE SKYPEOUT", 18)) {
				if (!strncmp(szSkypeMsg+19, "TRUE", 4)) {
					if (!bSkypeOut) {
						CLISTMENUITEM mi={0};

						bSkypeOut=TRUE; 
						mi.cbSize=sizeof(mi);
						mi.position=-2000005000;
						mi.flags=0;
						mi.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_CALLSKYPEOUT));
						mi.pszContactOwner=SKYPE_PROTONAME;
						mi.pszName=Translate("Do a SkypeOut-call");
						mi.pszService=SKYPEOUT_CALL;
						CallService(MS_CLIST_ADDMAINMENUITEM, (WPARAM)NULL,(LPARAM)&mi);
					}

				} else {
					bSkypeOut=FALSE;
					if (httbButton) {
						CallService(MS_TTB_REMOVEBUTTON, (WPARAM)httbButton, 0);
						httbButton=0;
					}
				}
				break;
			}
			if (!strncmp(szSkypeMsg, "MESSAGES", 8) || !strncmp(szSkypeMsg, "CHATMESSAGES", 12)) {
				if (strlen(szSkypeMsg)<=(UINT)(strchr(szSkypeMsg, ' ')-szSkypeMsg+1)) 
				{
					LOG(( "%s %d %s %d", szSkypeMsg,(UINT)(strchr(szSkypeMsg, ' ')-szSkypeMsg+1), 
						strchr(szSkypeMsg, ' '), strlen(szSkypeMsg)));
					break;
				}
				LOG(("MessageListProcessingThread launched"));
				pthread_create(( pThreadFunc )MessageListProcessingThread, _strdup(strchr(szSkypeMsg, ' ')+1));
				break;
			}
			if (!strncmp(szSkypeMsg, "MESSAGE", 7) || !strncmp(szSkypeMsg, "CHATMESSAGE", 11)) 
			{
				char *pMsgNum;
				TYP_MSGLENTRY *pEntry;

				if ((pMsgNum = strchr (szSkypeMsg, ' ')) && (ptr = strchr (++pMsgNum, ' ')))
				{
					BOOL bFetchMsg = FALSE;

					if (strncmp(ptr, " EDITED_TIMESTAMP", 17) == 0) {
						ptr[0]=0;
						if (pEntry = MsgList_FindMessage(strtoul(pMsgNum, NULL, 10))) {
							pEntry->tEdited = atol(ptr+18);
						}
						bFetchMsg = TRUE;
					} else bFetchMsg = (strncmp(ptr, " STATUS RE", 10) == 0 && !rcvwatchers) || 
						(strncmp(ptr, " STATUS SENT", 12) == 0 && !sendwatchers);

					if (bFetchMsg) {
						// If new message is available, fetch it
						ptr[0]=0;
						if (!(args=(fetchmsg_arg *)calloc(1, sizeof(*args)))) break;
						strncpy (args->msgnum, pMsgNum, sizeof(args->msgnum));
						args->getstatus=FALSE;
						//args->bIsRead = strncmp(ptr+8, "READ", 4) == 0;
						pthread_create(( pThreadFunc )FetchMessageThreadSync, args);
						break;
					}
				}
			}
			if (!strncmp(szSkypeMsg, "ERROR 68", 8)) {
				LOG(("We got a sync problem :( ->  SendMessage() will try to recover..."));
				break;
			}
			if (!strncmp(szSkypeMsg, "PROTOCOL ", 9)) {
				if ((protocol=(char)atoi(szSkypeMsg+9))>=3) {
					strcpy(cmdMessage, "CHATMESSAGE");
					strcpy(cmdPartner, "FROM");
				}
				bProtocolSet = TRUE;

				if (protocol<5 && !hMenuAddSkypeContact &&
					DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "EnableMenu", 1)) 
				{
					hMenuAddSkypeContact = add_mainmenu();
				}
			}
			LOG(("SkypeMsgAdd launched"));
			SkypeMsgAdd(szSkypeMsg);
			ReleaseSemaphore(SkypeMsgReceived, receivers, NULL);
		}
        break; 

        case WM_TIMER:
			SkypeSend("PING");
			SkypeMsgCollectGarbage(MAX_MSG_AGE);
			MsgList_CollectGarbage();
			if (receivers>1)
			{
				LOG(("Watchdog WARNING: there are still %d receivers waiting for MSGs", receivers));
			}
			break;

		case WM_CLOSE:
			PostQuitMessage (0);
			break;
		case WM_DESTROY:
			KillTimer (hWndDlg, 1);
			break; 

        default: 
		 if(message==ControlAPIAttach) {
				// Skype responds with Attach to the discover-message
				AttachStatus=lParam;
				if (lParam==SKYPECONTROLAPI_ATTACH_SUCCESS) {
					LOG (("AttachStatus success, got hWnd %08X", (HWND)wParam));
					if (hSkypeWnd && (HWND)wParam!=hSkypeWnd && IsWindow(hSkypeWnd))
						hSkypeWndSecondary = (HWND)wParam;
					else {
						hSkypeWnd=(HWND)wParam;	   // Skype gave us the communication window handle
						hSkypeWndSecondary = NULL;
					}
				}
				if (AttachStatus!=SKYPECONTROLAPI_ATTACH_API_AVAILABLE &&
					AttachStatus!=SKYPECONTROLAPI_ATTACH_NOT_AVAILABLE) 
				{
					LOG(("Attaching: SkypeReady fired, Attachstatus is %d", AttachStatus));
					SetEvent(SkypeReady);
				}
				AttachStatus=lParam;
				break;
		 }
		 return (DefWindowProc(hWndDlg, message, wParam, lParam)); 
    }
	LOG(("WM_COPYDATA exit (%08X)", message));
	if (szSkypeMsg) free(szSkypeMsg);
	return 1;
} 

void TellError(DWORD err) {
	LPVOID lpMsgBuf;
	
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err,
					  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);
        MessageBox( NULL, (TCHAR*)lpMsgBuf, _T("GetLastError"), MB_OK|MB_ICONINFORMATION );
        LocalFree( lpMsgBuf );
		return;
}


// SERVICES //
INT_PTR SkypeSetStatus(WPARAM wParam, LPARAM lParam)
{
	int oldStatus;
	BOOL UseCustomCommand, UnloadOnOffline;

	UNREFERENCED_PARAMETER(lParam);

	if (MirandaShuttingDown) return 0;
	UseCustomCommand = DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseCustomCommand", 0);
	UnloadOnOffline = DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UnloadOnOffline", 0);

	//if (!SkypeInitialized && !DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UnloadOnOffline", 0)) return 0;

	// Workaround for Skype status-bug
    if ((int)wParam==ID_STATUS_OFFLINE) logoff_contacts(TRUE);
	if (SkypeStatus==(int)wParam) return 0;
	oldStatus = SkypeStatus;

	if ((int)wParam==ID_STATUS_CONNECTING) return 0;
#ifdef MAPDND
	if ((int)wParam==ID_STATUS_OCCUPIED || (int)wParam==ID_STATUS_ONTHEPHONE) wParam=ID_STATUS_DND;
	if ((int)wParam==ID_STATUS_OUTTOLUNCH) wParam=ID_STATUS_NA;
#endif
#ifdef MAPNA
	if ((int)wParam==ID_STATUS_NA) wParam = ID_STATUS_AWAY;
#endif

   RequestedStatus=MirandaStatusToSkype((int)wParam);

   if (SkypeStatus != ID_STATUS_OFFLINE)
   {
     InterlockedExchange((long*)&SkypeStatus, (int)wParam);
     ProtoBroadcastAck(SKYPE_PROTONAME, NULL, ACKTYPE_STATUS, ACKRESULT_SUCCESS, (HANDLE) oldStatus, SkypeStatus);
   }
   
   if ((int)wParam==ID_STATUS_OFFLINE && UnloadOnOffline) 
   {
			if(UseCustomCommand)
			{
				DBVARIANT dbv;
				if(!DBGetContactSettingString(NULL,SKYPE_PROTONAME,"CommandLine",&dbv)) 
				{
					CloseSkypeAPI(dbv.pszVal);
					DBFreeVariant(&dbv);
				}
			}
			else
			{
				CloseSkypeAPI(skype_path);
			}

   } else if (AttachStatus==-1) 
   {
	   pthread_create(LaunchSkypeAndSetStatusThread, (void *)wParam);
	   return 0;
   }

   return SetUserStatus(); 
}

int __stdcall SendBroadcast( HANDLE hContact, int type, int result, HANDLE hProcess, LPARAM lParam )
{
	ACKDATA ack = {0};
	ack.cbSize = sizeof( ACKDATA );
	ack.szModule = SKYPE_PROTONAME;
	ack.hContact = hContact;
	ack.type = type;
	ack.result = result;
	ack.hProcess = hProcess;
	ack.lParam = lParam;
	return CallService( MS_PROTO_BROADCASTACK, 0, ( LPARAM )&ack );
}

static void __cdecl SkypeGetAwayMessageThread( HANDLE hContact )
{
	DBVARIANT dbv;
	if ( !DBGetContactSettingString( hContact, "CList", "StatusMsg", &dbv )) {
		SendBroadcast( hContact, ACKTYPE_AWAYMSG, ACKRESULT_SUCCESS, ( HANDLE )1, ( LPARAM )dbv.pszVal );
		DBFreeVariant( &dbv );
	}
	else SendBroadcast( hContact, ACKTYPE_AWAYMSG, ACKRESULT_SUCCESS, ( HANDLE )1, ( LPARAM )0 );
}

INT_PTR SkypeGetAwayMessage(WPARAM wParam,LPARAM lParam)
{
	CCSDATA* ccs = ( CCSDATA* )lParam;

	UNREFERENCED_PARAMETER(wParam);

	pthread_create( SkypeGetAwayMessageThread, ccs->hContact );
	return 1;
}

#define POLYNOMIAL (0x488781ED) /* This is the CRC Poly */
#define TOPBIT (1 << (WIDTH - 1)) /* MSB */
#define WIDTH 32

static int GetFileHash(char* filename)
{
	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	int remainder = 0, byte, bit;
	char data[1024];
	DWORD dwRead;

	if(hFile == INVALID_HANDLE_VALUE) return 0;

	do
	{
		// Read file chunk
		dwRead = 0;
		ReadFile(hFile, data, 1024, &dwRead, NULL);

		/* loop through each byte of data */
		for (byte = 0; byte < (int) dwRead; ++byte) {
			/* store the next byte into the remainder */
			remainder ^= (data[byte] << (WIDTH - 8));
			/* calculate for all 8 bits in the byte */
			for ( bit = 8; bit > 0; --bit) {
				/* check if MSB of remainder is a one */
				if (remainder & TOPBIT)
					remainder = (remainder << 1) ^ POLYNOMIAL;
				else
					remainder = (remainder << 1);
			}
		}
	} while(dwRead == 1024);

	CloseHandle(hFile);

	return remainder;
}

static int _GetFileSize(char* filename)
{
	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	int size;

	if(hFile == INVALID_HANDLE_VALUE)
		return 0;
	size = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	return size;
}

/* RetrieveUserAvatar
 * 
 * Purpose: Get a user avatar from skype itself
 * Params : param=(void *)(HANDLE)hContact
 */
void RetrieveUserAvatar(void *param)
{
	HANDLE hContact = (HANDLE) param, file;
	PROTO_AVATAR_INFORMATION AI={0};
	ACKDATA ack = {0};
	DBVARIANT dbv;
	char AvatarFile[MAX_PATH+1], AvatarTmpFile[MAX_PATH+10], *ptr, *pszTempFile;

	if (hContact == NULL)
		return;

	// Mount default ack
	ack.cbSize = sizeof( ACKDATA );
	ack.szModule = SKYPE_PROTONAME;
	ack.hContact = hContact;
	ack.type = ACKTYPE_AVATAR;
	ack.result = ACKRESULT_FAILED;
	
	AI.cbSize = sizeof( AI );
	AI.hContact = hContact;

	// Get skype name
	if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv) == 0) 
	{
		if (dbv.pszVal)
		{
			// Get filename
			FoldersGetCustomPath(hProtocolAvatarsFolder, AvatarFile, sizeof(AvatarFile), DefaultAvatarsFolder);
			mir_snprintf(AvatarTmpFile, sizeof(AvatarTmpFile), "AVATAR 1 %s\\%s_tmp.jpg", AvatarFile, dbv.pszVal);
			pszTempFile = AvatarTmpFile+9;
			mir_snprintf(AvatarFile, sizeof(AvatarFile), "%s\\%s.jpg", AvatarFile, dbv.pszVal);

			// Just to be sure
			DeleteFileA(pszTempFile);
			file = CreateFileA(pszTempFile, 0, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file != INVALID_HANDLE_VALUE)
			{
				CloseHandle(file);
				if (ptr=SkypeGet ("USER", dbv.pszVal, AvatarTmpFile))
				{
					if (strncmp(ptr, "ERROR", 5) && 
						GetFileAttributesA(pszTempFile) != INVALID_FILE_ATTRIBUTES) 
					{
						ack.result = ACKRESULT_SUCCESS;

						// Is no avatar image?
						if (!DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "ShowDefaultSkypeAvatar", 0) 
							&& GetFileHash(pszTempFile) == 0x8d34e05d && _GetFileSize(pszTempFile) == 3751)
						{
							// Has no avatar
							AI.format = PA_FORMAT_UNKNOWN;
							ack.hProcess = (HANDLE)&AI;
							DeleteFileA(AvatarFile);
						}
						else
						{
							// Got it
							MoveFileExA(pszTempFile, AvatarFile, MOVEFILE_REPLACE_EXISTING);
							AI.format = PA_FORMAT_JPEG;
							strcpy(AI.filename, AvatarFile);
							ack.hProcess = (HANDLE)&AI;
						}

					}
					free (ptr);
				}
				DeleteFileA(pszTempFile);
			}

		}
		DBFreeVariant(&dbv);
	}
	CallService( MS_PROTO_BROADCASTACK, 0, ( LPARAM )&ack );
}


/* SkypeGetAvatarInfo
 * 
 * Purpose: Set user avatar in profile
 * Params : wParam=0
 *			lParam=(LPARAM)(const char*)filename
 * Returns: 0 - Success
 *		   -1 - Failure
 */
INT_PTR SkypeGetAvatarInfo(WPARAM wParam,LPARAM lParam)
{

	DBVARIANT dbv;
	PROTO_AVATAR_INFORMATION* AI = ( PROTO_AVATAR_INFORMATION* )lParam;	
	if (AI->hContact == NULL) // User
	{
		if (!DBGetContactSettingString(NULL,SKYPE_PROTONAME, "AvatarFile", &dbv))
		{
			lstrcpynA(AI->filename, dbv.pszVal, sizeof(AI->filename));
			DBFreeVariant(&dbv);
			return GAIR_SUCCESS;
		}
		else
			return GAIR_NOAVATAR;
	}
	else // Contact 
	{
		DBVARIANT dbv;
		char AvatarFile[MAX_PATH+1];

		if (protocol < 7)
			return GAIR_NOAVATAR;

		if (wParam & GAIF_FORCE)
		{
			// Request anyway
			pthread_create(RetrieveUserAvatar, (void *) AI->hContact);
			return GAIR_WAITFOR;
		}

		if (DBGetContactSettingString(AI->hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv)) 
			// No skype name ??
			return GAIR_NOAVATAR;

		if (dbv.pszVal == NULL)
		{
			// No skype name ??
			DBFreeVariant(&dbv);
			return GAIR_NOAVATAR;
		}

		// Get filename
		FoldersGetCustomPath(hProtocolAvatarsFolder, AvatarFile, sizeof(AvatarFile), DefaultAvatarsFolder);
		mir_snprintf(AvatarFile, sizeof(AvatarFile), "%s\\%s.jpg", AvatarFile, dbv.pszVal);
		DBFreeVariant(&dbv);

		// Check if the file exists
		if (GetFileAttributesA(AvatarFile) == INVALID_FILE_ATTRIBUTES)
			return GAIR_NOAVATAR;
		
		// Return the avatar
		AI->format = PA_FORMAT_JPEG;
		strcpy(AI->filename, AvatarFile);
		return GAIR_SUCCESS;
	}
}


/* SkypeGetAvatarCaps
 * 
 * Purpose: Query avatar caps for a protocol
 * Params : wParam=One of AF_*
 *			lParam=Depends on wParam
 * Returns: Depends on wParam
 */
INT_PTR SkypeGetAvatarCaps(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case AF_MAXSIZE:
		{
			POINT *p = (POINT *) lParam;
			if (p == NULL)
				return -1;

			p->x = 96;
			p->y = 96;
			return 0;
		}
		case AF_PROPORTION:
		{
			return PIP_NONE;
		}
		case AF_FORMATSUPPORTED:
		{
			if (lParam == PA_FORMAT_PNG || lParam == PA_FORMAT_JPEG)
				return TRUE;
			else
				return FALSE;
		}
		case AF_ENABLED:
		{
			return TRUE;
		}
		case AF_DONTNEEDDELAYS:
		{
			return FALSE;
		}
	}
	return -1;
}


INT_PTR SkypeGetStatus(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return SkypeStatus;
}

INT_PTR SkypeGetInfo(WPARAM wParam,LPARAM lParam) {
    CCSDATA *ccs = (CCSDATA *) lParam;
	
	UNREFERENCED_PARAMETER(wParam);

	pthread_create(GetInfoThread, ccs->hContact);
	return 0;
}

INT_PTR SkypeAddToList(WPARAM wParam, LPARAM lParam) {
	PROTOSEARCHRESULT *psr=(PROTOSEARCHRESULT*)lParam;

	LOG(("SkypeAddToList Adding API function called"));
	if (psr->cbSize!=sizeof(PROTOSEARCHRESULT) || !psr->nick) return 0;
	LOG(("SkypeAddToList OK"));
    return (INT_PTR)add_contact(psr->nick, wParam);
}

INT_PTR SkypeBasicSearch(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);

	LOG(("SkypeBasicSearch %s", (char *)lParam));
	if (!SkypeInitialized) return 0;
	return (hSearchThread=pthread_create(( pThreadFunc )BasicSearchThread, _strdup((char *)lParam)));
}

void MessageSendWatchThread(msgsendwt_arg *arg) {
	char *str, *err;

	// sendwatchers need to be incremented before starting this thread
	LOG(("MessageSendWatchThread started."));
	str=SkypeRcvMsg(arg->szId, SkypeTime(NULL)-1, arg->hContact, DBGetContactSettingDword(NULL,"SRMsg","MessageTimeout",TIMEOUT_MSGSEND)+1000);
	InterlockedDecrement (&sendwatchers);
	if (str)
	{
		if (!DBGetContactSettingByte(arg->hContact, SKYPE_PROTONAME, "ChatRoom", 0)) {
			if (err=GetSkypeErrorMsg(str)) {
				ProtoBroadcastAck(SKYPE_PROTONAME, arg->hContact, ACKTYPE_MESSAGE, ACKRESULT_FAILED, (HANDLE) 1, (LPARAM)Translate(err));
				free(err);
				free(str);
				free(arg);
				LOG(("MessageSendWatchThread terminated."));
				return;
			}
			ProtoBroadcastAck(SKYPE_PROTONAME, arg->hContact, ACKTYPE_MESSAGE, ACKRESULT_SUCCESS, (HANDLE) 1, 0);
		}
		free(str);
		LOG(("MessageSendWatchThread terminated gracefully."));
	}
	free (arg);
}

INT_PTR SkypeSendMessage(WPARAM wParam, LPARAM lParam) {
    CCSDATA *ccs = (CCSDATA *) lParam;
	DBVARIANT dbv;
	BOOL sendok=TRUE;
    char *msg = (char *) ccs->lParam, *utfmsg=NULL, *mymsgcmd=cmdMessage, szId[16]={0};
	static DWORD dwMsgNum = 0;
	BYTE bIsChatroom = 0 != DBGetContactSettingByte(ccs->hContact, SKYPE_PROTONAME, "ChatRoom", 0);
    
	UNREFERENCED_PARAMETER(wParam);

	if (bIsChatroom)
	{
		if (DBGetContactSettingString(ccs->hContact, SKYPE_PROTONAME, "ChatRoomID", &dbv))
			return 0;
		mymsgcmd="CHATMESSAGE";
	}
	else
	{
		if (DBGetContactSettingString(ccs->hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv))
			return 0;
		mymsgcmd="MESSAGE";
	}
		
	if (ccs->wParam & PREF_UTF) {
		utfmsg = msg;
	} else if (ccs->wParam & PREF_UNICODE) {
		utfmsg = (char*)make_utf8_string((WCHAR*)(msg+strlen(msg)+1));
	} else {
		if (utf8_encode(msg, &utfmsg)==-1) utfmsg=NULL;
	}
	if (protocol>=4) {
		InterlockedIncrement ((LONG*)&dwMsgNum);
		sprintf (szId, "#M%d ", dwMsgNum++);
	}
	InterlockedIncrement (&sendwatchers);
	if (!utfmsg || SkypeSend("%s%s %s %s", szId, mymsgcmd, dbv.pszVal, utfmsg)) sendok=FALSE;
	if (utfmsg && utfmsg!=msg) free(utfmsg);
	DBFreeVariant(&dbv);

	if (sendok) {
		msgsendwt_arg *psendarg = calloc(1, sizeof(msgsendwt_arg));
		
		if (psendarg) {
			psendarg->hContact = ccs->hContact;
			strcpy (psendarg->szId, szId);
			pthread_create(MessageSendWatchThread, psendarg);
		} else InterlockedDecrement (&sendwatchers);
		return 1;
	} else InterlockedDecrement (&sendwatchers);
	if (!bIsChatroom) 
		ProtoBroadcastAck(SKYPE_PROTONAME, ccs->hContact, ACKTYPE_MESSAGE, ACKRESULT_FAILED, (HANDLE) 1, (LPARAM)Translate("Connection to Skype lost"));
	return 0;
}

INT_PTR SkypeRecvMessage(WPARAM wParam, LPARAM lParam)
{
    DBEVENTINFO dbei={0};
    CCSDATA *ccs = (CCSDATA *) lParam;
    PROTORECVEVENT *pre = (PROTORECVEVENT *) ccs->lParam;

	UNREFERENCED_PARAMETER(wParam);

    DBDeleteContactSetting(ccs->hContact, "CList", "Hidden");
    dbei.cbSize = sizeof(dbei);
    dbei.szModule = SKYPE_PROTONAME;
    dbei.timestamp = pre->timestamp;
	if (pre->flags & PREF_CREATEREAD) dbei.flags|=DBEF_READ;
    if (pre->flags & PREF_UTF) dbei.flags|=DBEF_UTF;
    dbei.eventType = EVENTTYPE_MESSAGE;
    dbei.cbBlob = strlen(pre->szMessage) + 1;
	if (pre->flags & PREF_UNICODE)
		dbei.cbBlob += sizeof( wchar_t )*( (DWORD)wcslen(( wchar_t* )&pre->szMessage[dbei.cbBlob] )+1 );
    dbei.pBlob = (PBYTE) pre->szMessage;
    MsgList_Add (pre->lParam, (HANDLE)CallService(MS_DB_EVENT_ADD, (WPARAM)ccs->hContact, (LPARAM)&dbei));
    return 0;
}

INT_PTR SkypeUserIsTyping(WPARAM wParam, LPARAM lParam) {
	DBVARIANT dbv={0};
	HANDLE hContact = (HANDLE)wParam;

	if (protocol<5) return 0;
	if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, "Typing_Stream", &dbv)) {
		if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv) == 0) {
			char szCmd[256];
			_snprintf (szCmd, sizeof(szCmd), 
				"ALTER APPLICATION libpurple_typing CONNECT %s", dbv.pszVal);
			SkypeSend (szCmd);
			DBFreeVariant (&dbv);
			testfor (szCmd, 2000);
			// TODO: We should somehow cache the typing notify result and send it
			// after we got a connection, but in the meantime this notification won't
			// get send on first run
		}
		return 0;
	}

	SkypeSend ("ALTER APPLICATION libpurple_typing DATAGRAM %s %s", dbv.pszVal, 
		(lParam==PROTOTYPE_SELFTYPING_ON?"PURPLE_TYPING":"PURPLE_NOT_TYPING"));
	DBFreeVariant(&dbv);
	return 0;
}


INT_PTR SkypeSendAuthRequest(WPARAM wParam, LPARAM lParam) {
	CCSDATA* ccs = (CCSDATA*)lParam;
	DBVARIANT dbv;
	int retval;

	UNREFERENCED_PARAMETER(wParam);

	if (!ccs->lParam || DBGetContactSettingString(ccs->hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv))
		return 1;
	retval = SkypeSend("SET USER %s BUDDYSTATUS 2 %s", dbv.pszVal, (char *)ccs->lParam);
	DBFreeVariant(&dbv);
	if (retval) return 1; else return 0;
}

INT_PTR SkypeRecvAuth(WPARAM wParam, LPARAM lParam) {
	DBEVENTINFO dbei = {0};
	CCSDATA* ccs = (CCSDATA*)lParam;
	PROTORECVEVENT* pre = (PROTORECVEVENT*)ccs->lParam;

	UNREFERENCED_PARAMETER(wParam);

	DBDeleteContactSetting(ccs->hContact, "CList", "Hidden");

	dbei.cbSize    = sizeof(dbei);
	dbei.szModule  = SKYPE_PROTONAME;
	dbei.timestamp = pre->timestamp;
	dbei.flags     = ((pre->flags & PREF_CREATEREAD)?DBEF_READ:0);
	dbei.eventType = EVENTTYPE_AUTHREQUEST;
	dbei.cbBlob	   = pre->lParam;
	dbei.pBlob     = (PBYTE)pre->szMessage;

	CallService(MS_DB_EVENT_ADD, (WPARAM)NULL, (LPARAM)&dbei);
	return 0;
}

char *__skypeauth(WPARAM wParam) {
	DBEVENTINFO dbei={0};

	if (!SkypeInitialized) return NULL;

	dbei.cbSize = sizeof(dbei);
	if ((dbei.cbBlob = CallService(MS_DB_EVENT_GETBLOBSIZE, wParam, 0))==-1 ||
		!(dbei.pBlob = (unsigned char*)malloc(dbei.cbBlob))) 
	{	return NULL; }

	if (CallService(MS_DB_EVENT_GET, wParam, (LPARAM)&dbei) ||
		dbei.eventType != EVENTTYPE_AUTHREQUEST ||
		strcmp(dbei.szModule, SKYPE_PROTONAME)) 
	{
		free(dbei.pBlob);
		return NULL;
	}
	return (char *)dbei.pBlob;
}

INT_PTR SkypeAuthAllow(WPARAM wParam, LPARAM lParam) {
	char *pBlob;

	UNREFERENCED_PARAMETER(lParam);

	if (pBlob=__skypeauth(wParam))
	{ 
		int retval=SkypeSend("SET USER %s ISAUTHORIZED TRUE", pBlob+sizeof(DWORD)+sizeof(HANDLE));
		free(pBlob);
		if (!retval) return 0;
	}
	return 1;
}

INT_PTR SkypeAuthDeny(WPARAM wParam, LPARAM lParam) {
	char *pBlob;

	UNREFERENCED_PARAMETER(lParam);

	if (pBlob=__skypeauth(wParam))
	{ 
		int retval=SkypeSend("SET USER %s ISAUTHORIZED FALSE", pBlob+sizeof(DWORD)+sizeof(HANDLE));
		free(pBlob);
		if (!retval) return 0;
	}
	return 1;
}


INT_PTR SkypeAddToListByEvent(WPARAM wParam, LPARAM lParam) {
	char *pBlob;

	UNREFERENCED_PARAMETER(lParam);

	if (pBlob=__skypeauth(wParam))
	{ 
		HANDLE hContact=add_contact(pBlob+sizeof(DWORD)+sizeof(HANDLE), LOWORD(wParam));
		free(pBlob);
		if (hContact) return (int)hContact;
	}
	return 0;
}

INT_PTR SkypeRegisterProxy(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);

	if (!lParam) {
		free (pszProxyCallout);
		pszProxyCallout = NULL;
	}
	pszProxyCallout = _strdup((char*)lParam);
	bIsImoproxy = TRUE;
	return 0;
}


void CleanupNicknames(char *dummy) {
	HANDLE hContact;
	char *szProto;
	DBVARIANT dbv, dbv2;

	UNREFERENCED_PARAMETER(dummy);

	LOG(("CleanupNicknames Cleaning up..."));
	for (hContact=(HANDLE)CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);hContact != NULL;hContact=(HANDLE)CallService( MS_DB_CONTACT_FINDNEXT, (WPARAM)hContact, 0)) {
		szProto = (char*)CallService( MS_PROTO_GETCONTACTBASEPROTO, (WPARAM)hContact, 0 );
		if (szProto!=NULL && !strcmp(szProto, SKYPE_PROTONAME) &&
			DBGetContactSettingByte(hContact, SKYPE_PROTONAME, "ChatRoom", 0)==0)	
		{
			if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, SKYPE_NAME, &dbv)) continue;
			if (DBGetContactSettingString(hContact, SKYPE_PROTONAME, "Nick", &dbv2)) {
				DBFreeVariant(&dbv);
				continue;
			}
			DBDeleteContactSetting(hContact, SKYPE_PROTONAME, "Nick");
			GetInfoThread(hContact);
			DBFreeVariant(&dbv);
			DBFreeVariant(&dbv2);
		}
	}
	OUTPUT(_T("Cleanup finished."));
}

/////////////////////////////////////////////////////////////////////////////////////////
// EnterBitmapFileName - enters a bitmap filename

int __stdcall EnterBitmapFileName( char* szDest )
{
	char szFilter[ 512 ];
	OPENFILENAMEA ofn = {0};
	*szDest = 0;

	CallService( MS_UTILS_GETBITMAPFILTERSTRINGS, sizeof szFilter, ( LPARAM )szFilter );
	ofn.lStructSize = sizeof( OPENFILENAME );
	ofn.lpstrFilter = szFilter;
	ofn.lpstrFile = szDest;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = "bmp";
	if ( !GetOpenFileNameA( &ofn ))
		return 1;

	return ERROR_SUCCESS;
}

int MirandaExit(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	MirandaShuttingDown=TRUE;
	return 0;
}

int OkToExit(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

//	logoff_contacts();
	MirandaShuttingDown=TRUE;

	// Trigger all semaphores and events just to be sure that there is no deadlock
	ReleaseSemaphore(SkypeMsgReceived, receivers, NULL);
    SetEvent (SkypeReady);
	SetEvent (MessagePumpReady);
#ifdef SKYPEBUG_OFFLN
	SetEvent(GotUserstatus);
#endif
	SetEvent (hBuddyAdded);

	SkypeFlush ();
	PostMessage (g_hWnd, WM_CLOSE, 0, 0);
	return 0;
}


struct PLUGINDI {
	char **szSettings;
	int dwCount;
};

// Taken from pluginopts.c and modified
int EnumOldPluginName(const char *szSetting,LPARAM lParam)
{
	struct PLUGINDI *pdi=(struct PLUGINDI*)lParam;
	if (pdi && lParam) {
			pdi->szSettings=(char**)realloc(pdi->szSettings,(pdi->dwCount+1)*sizeof(char*));
			pdi->szSettings[pdi->dwCount++]=_strdup(szSetting);
	} 
	return 0;
}

// Are there any Skype users on list? 
// 1 --> Yes
// 0 --> No
int AnySkypeusers(void) 
{
	HANDLE hContact;
	DBVARIANT dbv;
	int tCompareResult;

	// already on list?
	for (hContact=(HANDLE)CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);
		 hContact != NULL;
		 hContact=(HANDLE)CallService( MS_DB_CONTACT_FINDNEXT, (WPARAM)hContact, 0)) 
	{
		// GETCONTACTBASEPROTO doesn't work on not loaded protocol, therefore get 
		// protocol from DB
		if (DBGetContactSettingString(hContact, "Protocol", "p", &dbv)) continue;
        tCompareResult = !strcmp(dbv.pszVal, SKYPE_PROTONAME);
		DBFreeVariant(&dbv);
		if (tCompareResult) return 1;
	}
	return 0;
}


void UpgradeName(char *OldName)
{	
	DBCONTACTENUMSETTINGS cns;
	DBCONTACTWRITESETTING cws;
	DBVARIANT dbv;
	HANDLE hContact=NULL;
	struct PLUGINDI pdi;

	LOG(("Updating old database settings if there are any..."));
	cns.pfnEnumProc=EnumOldPluginName;
	cns.lParam=(LPARAM)&pdi;
	cns.szModule=OldName;
	cns.ofsSettings=0;

	hContact = (HANDLE)CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);

	for ( ;; ) {
		memset(&pdi,0,sizeof(pdi));
		CallService(MS_DB_CONTACT_ENUMSETTINGS,(WPARAM)hContact,(LPARAM)&cns);
		// Upgrade Protocol settings to new string
		if (pdi.szSettings) {
			int i;

			LOG(("We're currently upgrading..."));
			for (i=0;i<pdi.dwCount;i++) {
				if (!DBGetContactSettingString(hContact, OldName, pdi.szSettings[i], &dbv)) {
					cws.szModule=SKYPE_PROTONAME;
					cws.szSetting=pdi.szSettings[i];
					cws.value=dbv;
					if (!CallService(MS_DB_CONTACT_WRITESETTING,(WPARAM)hContact,(LPARAM)&cws))
						DBDeleteContactSetting(hContact,OldName,pdi.szSettings[i]);
					DBFreeVariant(&dbv);
				}		
				free(pdi.szSettings[i]);
			}
			free(pdi.szSettings);
		} 
		// Upgrade Protocol assignment, if we are not main contact
		if (hContact && !DBGetContactSettingString(hContact, "Protocol", "p", &dbv)) {
			if (!strcmp(dbv.pszVal, OldName))
				DBWriteContactSettingString(hContact, "Protocol", "p", SKYPE_PROTONAME);
			DBFreeVariant(&dbv);
		}
		if (!hContact) break;
		hContact = (HANDLE)CallService(MS_DB_CONTACT_FINDNEXT, (WPARAM)hContact, 0);
	}

	DBWriteContactSettingByte(NULL, SKYPE_PROTONAME, "UpgradeDone", (BYTE)1);
	return;
}

void __cdecl MsgPump (char *dummy)
{
  MSG msg;

  WNDCLASS WndClass; 

  UNREFERENCED_PARAMETER(dummy);

  // Create window class
  WndClass.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS; 
  WndClass.lpfnWndProc = (WNDPROC)WndProc; 
  WndClass.cbClsExtra = 0; 
  WndClass.cbWndExtra = 0; 
  WndClass.hInstance =  hInst;
  WndClass.hIcon = NULL; 
  WndClass.hCursor = NULL;
  WndClass.hbrBackground = NULL;
  WndClass.lpszMenuName = NULL; 
  WndClass.lpszClassName = _T("SkypeApiDispatchWindow"); 
  RegisterClass(&WndClass);
  // Do not check the retval of RegisterClass, because on non-unicode
  // win98 it will fail, as it is a stub that returns false() there
	
  // Create main window
  g_hWnd=CreateWindowEx( WS_EX_APPWINDOW|WS_EX_WINDOWEDGE,
		_T("SkypeApiDispatchWindow"), _T(""), WS_BORDER|WS_SYSMENU|WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 128, 128, NULL, 0, (HINSTANCE)WndClass.hInstance, 0);

  LOG (("Created Dispatch window with handle %08X", (long)g_hWnd));
  if (!g_hWnd) {
	OUTPUT(_T("Cannot create window."));
	TellError(GetLastError());
	SetEvent(MessagePumpReady);
    return; 
  }
  ShowWindow(g_hWnd, 0); 
  UpdateWindow(g_hWnd); 
  msgPumpThreadId = GetCurrentThreadId();
  SetEvent(MessagePumpReady);

  LOG (("Messagepump started."));
  while (GetMessage (&msg, g_hWnd, 0, 0) > 0 && !Miranda_Terminated()) {
	  TranslateMessage (&msg);
	  DispatchMessage (&msg);
  }
  UnregisterClass (WndClass.lpszClassName, hInst);
  LOG (("Messagepump stopped."));
}

// DLL Stuff //

__declspec(dllexport) PLUGININFO* MirandaPluginInfo(DWORD mirVersion)
{
	mirandaVersion = mirVersion;

	pluginInfo.cbSize = sizeof(PLUGININFO);
	return (PLUGININFO*) &pluginInfo;
}

__declspec(dllexport) PLUGININFOEX* MirandaPluginInfoEx(DWORD mirVersion)
{
	mirandaVersion = mirVersion;

	return &pluginInfo;
}

static const MUUID interfaces[] = {MUUID_SKYPE_CALL, MIID_LAST};
__declspec(dllexport) const MUUID * MirandaPluginInterfaces(void)
{
	return interfaces;
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(fdwReason);
	UNREFERENCED_PARAMETER(lpvReserved);

	hInst = hinstDLL;
	return TRUE;
}


int PreShutdown(WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	PostThreadMessage(msgPumpThreadId, WM_QUIT, 0, 0);
	return 0;
}

int __declspec(dllexport) Load(PLUGINLINK *link)
{
	PROTOCOLDESCRIPTOR pd;
	DWORD Buffsize;
	HKEY MyKey;
	BOOL SkypeInstalled;
	BOOL UseCustomCommand;
	WSADATA wsaData;
	char path[MAX_PATH];

	pluginLink = link;
	mir_getMMI( &mmi ); 

	GetModuleFileNameA( hInst, path, sizeof( path ));
	_splitpath (path, NULL, NULL, SKYPE_PROTONAME, NULL);
	CharUpperA( SKYPE_PROTONAME );

	InitializeCriticalSection(&RingAndEndcallMutex);
	InitializeCriticalSection(&QueryThreadMutex);
	InitializeCriticalSection(&TimeMutex);


#ifdef _DEBUG
	init_debug();
#endif

	LOG(("Load: Skype Plugin loading..."));

	// We need to upgrade SKYPE_PROTOCOL internal name to Skype if not already done
	if (!DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UpgradeDone", 0))
		UpgradeName("SKYPE_PROTOCOL");

    // Initialisation of Skype MsgQueue must be done because of Cleanup in end and
	// Mutex is also initialized here.
	LOG(("SkypeMsgInit initializing Skype MSG-queue"));
	if (SkypeMsgInit()==-1) {
		OUTPUT(_T("Memory allocation error on startup."));
		return 0;
	}

	// On first run on new profile, ask user, if he wants to enable the plugin in
	// this profile
	// --> Fixing Issue #0000006 from bugtracker.
	if (!DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "FirstRun", 0)) {
		DBWriteContactSettingByte(NULL, SKYPE_PROTONAME, "FirstRun", 1);
		if (AnySkypeusers()==0) // First run, it seems :)
			if (MessageBox(NULL, TranslateT("This seems to be the first time that you're running the Skype protocol plugin. Do you want to enable the protocol for this Miranda-Profile? (If you chose NO, you can always enable it in the plugin options later."), _T("Welcome!"), MB_ICONQUESTION|MB_YESNO)==IDNO) {
				char path[MAX_PATH], *filename;
				GetModuleFileNameA(hInst, path, sizeof(path));
				if (filename = strrchr(path,'\\')+1)
					DBWriteContactSettingByte(NULL,"PluginDisable",filename,1);
				return 0;
			}
	}


	// Check if Skype is installed
	SkypeInstalled=TRUE;
	UseCustomCommand = (BYTE)DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseCustomCommand", 0);
	UseSockets = (BOOL)DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseSkype2Socket", 0);

	if (!UseSockets && !UseCustomCommand) 
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Skype\\Phone"), 0, KEY_READ, &MyKey)!=ERROR_SUCCESS)
		{
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("Software\\Skype\\Phone"), 0, KEY_READ, &MyKey)!=ERROR_SUCCESS)
			{
				SkypeInstalled=FALSE;
			}
		}
		
		Buffsize=sizeof(skype_path);
		
		if (SkypeInstalled==FALSE || RegQueryValueExA(MyKey, "SkypePath", NULL, NULL, (unsigned char *)skype_path,  &Buffsize)!=ERROR_SUCCESS) 
		{
			    //OUTPUT("Skype was not found installed :( \nMaybe you are using portable skype.");
				RegCloseKey(MyKey);
				skype_path[0]=0;
				//return 0;
		}
		RegCloseKey(MyKey);
	}
	WSAStartup(MAKEWORD(2,2), &wsaData);

	// Start Skype connection 
	if (!(ControlAPIAttach=RegisterWindowMessage(_T("SkypeControlAPIAttach"))) || !(ControlAPIDiscover=RegisterWindowMessage(_T("SkypeControlAPIDiscover")))) 
	{
			OUTPUT(_T("Cannot register Window message."));
			return 0;
	}
	
	SkypeMsgReceived=CreateSemaphore(NULL, 0, MAX_MSGS, NULL);
	if (!(SkypeReady=CreateEvent(NULL, TRUE, FALSE, NULL)) ||
		!(MessagePumpReady=CreateEvent(NULL, FALSE, FALSE, NULL)) ||
#ifdef SKYPEBUG_OFFLN
	    !(GotUserstatus=CreateEvent(NULL, TRUE, FALSE, NULL)) ||
#endif
		!(hBuddyAdded=CreateEvent(NULL, FALSE, FALSE, NULL)) ||
		!(FetchMessageEvent=CreateEvent(NULL, FALSE, TRUE, NULL))) {
		 OUTPUT(_T("Unable to create Mutex!"));
		return 0;
	}

	/* Register the module */
	ZeroMemory(&pd, sizeof(pd));
	pd.cbSize = sizeof(pd);
	pd.szName = SKYPE_PROTONAME;
	pd.type   = PROTOTYPE_PROTOCOL;
	CallService(MS_PROTO_REGISTERMODULE, 0, (LPARAM)&pd);

	VoiceServiceInit();

	CreateServices();
	HookEvents();
	InitVSApi();
	MsgList_Init();

	HookEvent(ME_SYSTEM_PRESHUTDOWN, PreShutdown);

	// Startup Message-pump
    pthread_create (( pThreadFunc )MsgPump, NULL);
	WaitForSingleObject(MessagePumpReady, INFINITE);
	return 0;
}



int __declspec( dllexport ) Unload(void) 
{
	BOOL UseCustomCommand = DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "UseCustomCommand", 0);
	BOOL Shutdown = DBGetContactSettingByte(NULL, SKYPE_PROTONAME, "Shutdown", 0);
	
	LOG (("Unload started"));
	
	if ( Shutdown && ((skype_path && skype_path[0]) ||UseCustomCommand) ) {

		if(UseCustomCommand)
		{
			DBVARIANT dbv;
			if(!DBGetContactSettingString(NULL,SKYPE_PROTONAME,"CommandLine",&dbv)) 
			{
				char szAbsolutePath[MAX_PATH];

				TranslateMirandaRelativePathToAbsolute(dbv.pszVal, szAbsolutePath, FALSE);
				_spawnl(_P_NOWAIT, szAbsolutePath, szAbsolutePath, "/SHUTDOWN", NULL);
				LOG (("Unload Sent /shutdown to %s", szAbsolutePath));
				DBFreeVariant(&dbv);
			}
		}
		else
		{
			_spawnl(_P_NOWAIT, skype_path, skype_path, "/SHUTDOWN", NULL);
			LOG (("Unload Sent /shutdown to %s", skype_path));
		}
		
	}
	SkypeMsgCleanup();
	WSACleanup();
	FreeVSApi();
	UnhookEvents();
	UnhookEvent(hChatEvent);
	UnhookEvent (hChatMenu);
	UnhookEvent (hEvInitChat);
	DestroyHookableEvent(hInitChat);
	VoiceServiceExit();
	GCExit();
	MsgList_Exit();

	CloseHandle(SkypeReady);
	CloseHandle(SkypeMsgReceived);
#ifdef SKYPEBUG_OFFLN
	CloseHandle(GotUserstatus);
#endif
	CloseHandle(MessagePumpReady);
	CloseHandle(hBuddyAdded);
	CloseHandle(FetchMessageEvent);

	DeleteCriticalSection(&RingAndEndcallMutex);
	DeleteCriticalSection(&QueryThreadMutex);

	SkypeRegisterProxy (0, 0);
	LOG (("Unload: Shutdown complete"));
#ifdef _DEBUG
	end_debug();
#endif
	DeleteCriticalSection(&TimeMutex);
	return 0;
}

