/*
Fingerprint Mod+ (client version) icons module for Miranda IM

Copyright � 2006-2007 Artem Shpynov aka FYR, Bio, Faith Healer. 2009-2010 HierOS

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

/************************************************************************/
/* This file contains data about appropriate MirVer values				*/
/************************************************************************/

/*
*	NOTE: Masks can contain '*' or '?' wild symbols
*	Asterics '*' symbol covers 'empty' symbol too e.g WildCompare("Tst","T*st*"), returns TRUE 
*	In order to handle situation 'at least one any sybol' use '?*' combination:
*	e.g WildCompare("Tst","T?*st*"), returns FALSE, but both WildCompare("Test","T?*st*") and
*	WildCompare("Teeest","T?*st*") return TRUE.
*
*	Function is 'dirt' case insensitive (it is ignore 5th bit (0x20) so it is no difference
*	beetween some symbols. But it is faster than valid converting to uppercase.
*
*	Mask can contain several submasks. In this case each submask (including first)
*	should start from '|' e.g: "|first*submask|second*mask".
*
*	ORDER OF RECORDS IS IMPORTANT: system search first suitable mask and returns it.
*	e.g. if MirVer is "Miranda IM" and first mask is "*im*" and second is "Miranda *" the 
*	result will be client associated with first mask, not second!
*	So in order to avoid such situation, place most generalised masks to latest place.
*
*	In order to get "Unknown" client, last mask should be "?*".
*/

KN_FP_MASK def_kn_fp_mask[] =
{//	IconName							Mask										Icon caption							NULL ICON_RESOURCE_ID
//#########################################################################################################################################
//######################################################### MirandaIM clients #############################################################
//#########################################################################################################################################
	{ "client_Miranda_666",				_T("Miranda*6.6.6*"),							_T("Miranda evil"),						_T("ClientIcons_General"),	IDI_MIRANDA_666, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_010U",			_T("|Miranda IM*0.10*Unicode*")
										_T("|Miranda IM*Unicode*0.10*"),				_T("Miranda v0.10 Unicode"),			_T("ClientIcons_General"),	IDI_MIRANDA_010U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_09U",				_T("|Miranda IM*0.9*Unicode*")
										_T("|Miranda IM*Unicode*0.9*"),					_T("Miranda v0.9 Unicode"),				_T("ClientIcons_General"),	IDI_MIRANDA_09U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_08U",				_T("|Miranda IM*0.8*Unicode*")
										_T("|Miranda IM*Unicode*0.8*"),					_T("Miranda v0.8 Unicode"),				_T("ClientIcons_General"),	IDI_MIRANDA_08U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_07U",				_T("|Miranda IM*0.7*Unicode*")
										_T("|Miranda IM*Unicode*0.7*"),					_T("Miranda v0.7 Unicode"),				_T("ClientIcons_General"),	IDI_MIRANDA_07U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_06U",				_T("|Miranda IM*0.6*Unicode*")
										_T("|Miranda IM*Unicode*0.6*"),					_T("Miranda v0.6 Unicode"),				_T("ClientIcons_General"),	IDI_MIRANDA_06U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_05U",				_T("|Miranda IM*0.5*Unicode*")
										_T("|Miranda IM*Unicode*0.5*"),					_T("Miranda v0.5 Unicode"),				_T("ClientIcons_General"),	IDI_MIRANDA_05U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_U",				_T("*Miranda IM*Unicode*"),						_T("Miranda Unicode"),					_T("ClientIcons_General"),	IDI_MIRANDA_U, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_010",				_T("*Miranda IM*0.10*"),						_T("Miranda v0.10"),					_T("ClientIcons_General"),	IDI_MIRANDA_010, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_09",				_T("*Miranda IM*0.9*"),							_T("Miranda v0.9"),						_T("ClientIcons_General"),	IDI_MIRANDA_09, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_08",				_T("*Miranda IM*0.8*"),							_T("Miranda v0.8"),						_T("ClientIcons_General"),	IDI_MIRANDA_08, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_07",				_T("*Miranda IM*0.7*"),							_T("Miranda v0.7"),						_T("ClientIcons_General"),	IDI_MIRANDA_07, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_06",				_T("*Miranda IM*0.6*"),							_T("Miranda v0.6"),						_T("ClientIcons_General"),	IDI_MIRANDA_06, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_05",				_T("*Miranda IM*0.5*"),							_T("Miranda v0.5"),						_T("ClientIcons_General"),	IDI_MIRANDA_05, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_04",				_T("*Miranda IM*0.4*"),							_T("Miranda v0.4"),						_T("ClientIcons_General"),	IDI_MIRANDA_04, INVALID_HANDLE_VALUE, MIRANDA_CASE },
	{ "client_Miranda_Unknown",			_T("|*Miranda*|*Mira*"),						_T("Miranda IM"),						_T("ClientIcons_General"),	IDI_MIRANDA_UNKNOWN, INVALID_HANDLE_VALUE, MIRANDA_CASE },

//#########################################################################################################################################
//###################################################### Multi-protocol clients ###########################################################
//#########################################################################################################################################
	{ "client_SIM_Win32_old",			_T("|^*SIM*0.9.4*|^*SIM*0.9.5*")
										_T("|^*SIM*0.9.6*|^*SIM*0.9.7*")
										_T("|*SIM*Win32"),								_T("SIM (Win32) old"),					_T("ClientIcons_Multiprotocols"),	IDI_SIM_WIN_OLD, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_SIM_MacOS_old",			_T("|^*SIM*0.9.4*|^*SIM*0.9.5*")
										_T("|^*SIM*0.9.6*|^*SIM*0.9.7*")
										_T("|*SIM*MacOS"),								_T("SIM (MacOS) old"),					_T("ClientIcons_Multiprotocols"),	IDI_SIM_MACOSX_OLD, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_SIM_Linux_old",			_T("SIM*?.8.?"),								_T("SIM (Linux) old"),					_T("ClientIcons_Multiprotocols"),	IDI_SIM_LINUX_OLD, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_SIM_old",					_T("|^*SIM*0.9.4*|^*SIM*0.9.5*")
										_T("|^*SIM*0.9.6*|^*SIM*0.9.7*|^SIM")
										_T("|*SIM*Simp*|^*Simp*|*SIM*"),				_T("SIM (others) old"),					_T("ClientIcons_Multiprotocols"),	IDI_SIM_OLD, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },

	{ "client_SIM_Win32",				_T("|*SIM*Win32"),								_T("SIM (Win32)"),						_T("ClientIcons_Multiprotocols"),	IDI_SIM_WIN, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_SIM_MacOSX",				_T("|*SIM*MacOS"),								_T("SIM (MacOS)"),						_T("ClientIcons_Multiprotocols"),	IDI_SIM_MACOSX, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_SIM_Linux",				_T("SIM*?.9.?"),								_T("SIM (Linux)"),						_T("ClientIcons_Multiprotocols"),	IDI_SIM_LINUX, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_SIM",						_T("|*SIM*"),									_T("SIM (others)"),						_T("ClientIcons_Multiprotocols"),	IDI_SIM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },

	{ "client_BayanICQ",				_T("|*BayanICQ*|*barobin*"),					_T("BayanICQ"),							_T("ClientIcons_Multiprotocols"),	IDI_BAYANICQ, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },

	{ "client_Adium",					_T("Adium*"),									_T("Adium"),							_T("ClientIcons_Multiprotocols"),	IDI_ADIUM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Trillian_Astra",			_T("Trillian*Astra*"),							_T("Trillian Astra"),					_T("ClientIcons_Multiprotocols"),	IDI_TRILLIAN_ASTRA, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Trillian_Pro",			_T("Trillian*Pro*"),							_T("Trillian Pro"),						_T("ClientIcons_Multiprotocols"),	IDI_TRILLIAN_PRO, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Trillian",				_T("|Trillian*|http://trillian.im/*"),			_T("Trillian"),							_T("ClientIcons_Multiprotocols"),	IDI_TRILLIAN, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Gaim",					_T("*gaim*"),									_T("Gaim (libgaim)"),					_T("ClientIcons_Multiprotocols"),	IDI_GAIM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Pidgin",					_T("|*Pidgin*|libpurple*|Purple*"),				_T("Pidgin (libpurple) (ex-Gaim)"),		_T("ClientIcons_Multiprotocols"),	IDI_PIDGIN, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Pigeon",					_T("*PIGEON*"),									_T("PIGEON!"),							_T("ClientIcons_Multiprotocols"),	IDI_PIGEON, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Kopete",					_T("|Kopete*|http://kopete.kde.org*"),			_T("Kopete"),							_T("ClientIcons_Multiprotocols"),	IDI_KOPETE, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_IM2",						_T("IM2*"),										_T("IM2"),								_T("ClientIcons_Multiprotocols"),	IDI_IM2, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },

	{ "client_CenterICQ",				_T("Centericq*"),								_T("Centericq"),						_T("ClientIcons_Multiprotocols"),	IDI_CENTERICQ, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Meebo",					_T("Meebo*"),									_T("Meebo"),							_T("ClientIcons_Multiprotocols"),	IDI_MEEBO, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },

	{ "client_AgileMessenger",			_T("*Agile Messenger*"),						_T("Agile Messenger"),					_T("ClientIcons_Multiprotocols"),	IDI_AGILE, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Prelude",					_T("Prelude*"),									_T("Prelude"),							_T("ClientIcons_Multiprotocols"),	IDI_PRELUDE, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_IMPlus",					_T("|IM+*|IMPLUS*"),							_T("IM+"),								_T("ClientIcons_Multiprotocols"),	IDI_IMPLUS, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_uIM",						_T("uIM*"),										_T("uIM"),								_T("ClientIcons_Multiprotocols"),	IDI_UIM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Inlux",					_T("*Inlux*"),									_T("Inlux Messenger"),					_T("ClientIcons_Multiprotocols"),	IDI_INLUX, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Qnext",					_T("QNext*"),									_T("Qnext"),							_T("ClientIcons_Multiprotocols"),	IDI_QNEXT, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_mChat",					_T("|mChat*|gsICQ*|http://mchat.mgslab.com*"),	_T("mChat"),							_T("ClientIcons_Multiprotocols"),	IDI_MCHAT, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_natSiemens",				_T("|SieJC*|NatICQ*|Siemens*Client*"),			_T("Siemens Native ICQ/Jabber Client"),	_T("ClientIcons_Multiprotocols"),	IDI_NATSIEMENS, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Fring",					_T("*fring*"),									_T("Fring"),							_T("ClientIcons_Multiprotocols"),	IDI_FRING, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_eBuddy",					_T("|*eBuddy*|*eMessenger*"),					_T("eBuddy"),							_T("ClientIcons_Multiprotocols"),	IDI_EBUDDY, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Meetro",					_T("Meetro*"),									_T("Meetro"),							_T("ClientIcons_Multiprotocols"),	IDI_MEETRO, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_EasyMessage",				_T("Easy*Message*"),							_T("Easy Message"),						_T("ClientIcons_Multiprotocols"),	IDI_EASYMESSAGE, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_BitlBee",					_T("BitlBee*"),									_T("BitlBee"),							_T("ClientIcons_Multiprotocols"),	IDI_BITLBEE, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Proteus",					_T("*Proteus*"),								_T("Proteus"),							_T("ClientIcons_Multiprotocols"),	IDI_PROTEUS, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_iChat",					_T("|iChat*")
										_T("|http://www.apple.com/ichat/caps*"),		_T("iChat"),							_T("ClientIcons_Multiprotocols"),	IDI_ICHAT, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_EKG2",					_T("*ekg*2*"),									_T("EKG2"),								_T("ClientIcons_Multiprotocols"),	IDI_EKG2, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Ayttm",					_T("*Ayttm*"),									_T("Ayttm"),							_T("ClientIcons_Multiprotocols"),	IDI_AYTTM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_qutIM",					_T("*qutIM*"),									_T("qutIM"),							_T("ClientIcons_Multiprotocols"),	IDI_QUTIM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_YamiGo",					_T("YamiGo*"),									_T("YamiGo"),							_T("ClientIcons_Multiprotocols"),	IDI_YAMIGO, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_QIP_Infium",				_T("|QIP Infium*|http://qip* 9???"),			_T("QIP Infium"),						_T("ClientIcons_Multiprotocols"),	IDI_QIP_INFIUM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_QIP_2010",				_T("|QIP 2010*|http://qip* 3???")
										_T("|http://2010.qip.ru*"),						_T("QIP 2010"),							_T("ClientIcons_Multiprotocols"),	IDI_QIP_2010, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_QIP_PDA",					_T("|QIP PDA*|http://pda.qip.ru/caps*")
										_T("|QIP Mobile*"),								_T("QIP Mobile"),						_T("ClientIcons_Multiprotocols"),	IDI_QIP_PDA, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_CitronIM",				_T("*Citron IM*"),								_T("Citron IM"),						_T("ClientIcons_Multiprotocols"),	IDI_CITRON, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Palringo",				_T("|*Palringo*|http://palringo.com/caps*"),	_T("Palringo"),							_T("ClientIcons_Multiprotocols"),	IDI_PALRINGO, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_ImoIm",					_T("|*imo.im*"),								_T("Imo.im"),							_T("ClientIcons_Multiprotocols"),	IDI_IMOIM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_OneTeam",					_T("*OneTeam*"),								_T("OneTeam"),							_T("ClientIcons_Multiprotocols"),	IDI_ONETEAM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Python",					_T("Py*t"),										_T("Python transport clients"),			_T("ClientIcons_Multiprotocols"),	IDI_PYTHON, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Instantbird",				_T("*Instantbird*"),							_T("Instantbird"),						_T("ClientIcons_Multiprotocols"),	IDI_INSTANTBIRD, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },
	{ "client_Jimm",					_T("|*Jimm*|mobicq*"),							_T("Jimm"),								_T("ClientIcons_Multiprotocols"),	IDI_JIMM, INVALID_HANDLE_VALUE, MULTIPROTOCOL_CASE },

//#########################################################################################################################################
//########################################################### Other ICQ clients ###########################################################
//#########################################################################################################################################
	{ "client_QIP",						_T("QIP*"),										_T("QIP"),								_T("ClientIcons_ICQ"),	IDI_QIP, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_&RQ",						_T("&RQ*"),										_T("&RQ"),								_T("ClientIcons_ICQ"),	IDI_ANDRQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_R&Q",						_T("R&Q*"),										_T("R&Q"),								_T("ClientIcons_ICQ"),	IDI_RANDQ_NEW, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_TICQ",					_T("*TICQ*Client*"),							_T("TICQ (Delphi lib)"),				_T("ClientIcons_ICQ"),	IDI_DELPHI, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_ICAT",					_T("IC@*"),										_T("IC@"),								_T("ClientIcons_ICQ"),	IDI_ICAT, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_StrICQ",					_T("StrICQ*"),									_T("StrICQ"),							_T("ClientIcons_ICQ"),	IDI_STRICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_mICQ",					_T("mICQ*"),									_T("mICQ"),								_T("ClientIcons_ICQ"),	IDI_MICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_VmICQ",					_T("VmICQ*"),									_T("VmICQ"),							_T("ClientIcons_ICQ"),	IDI_VMICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_vICQ",					_T("vICQ*"),									_T("vICQ"),								_T("ClientIcons_ICQ"),	IDI_VICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_YSM",						_T("YSM*"),										_T("YSM"),								_T("ClientIcons_ICQ"),	IDI_YSM, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_MIP",						_T("*MIP*"),									_T("MIP"),								_T("ClientIcons_ICQ"),	IDI_MIP, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_D[i]Chat",				_T("D[i]Chat*"),								_T("D[i]Chat"),							_T("ClientIcons_ICQ"),	IDI_DICHAT, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Corepager",				_T("Core*Pager*"),								_T("Core Pager"),						_T("ClientIcons_ICQ"),	IDI_COREPAGER, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_LocID",					_T("LocID*"),									_T("LocID"),							_T("ClientIcons_ICQ"),	IDI_LOCID, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Slick",					_T("Slick*"),									_T("Slick"),							_T("ClientIcons_ICQ"),	IDI_SLICK, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Smaper",					_T("*SmapeR*"),									_T("SmapeR"),							_T("ClientIcons_ICQ"),	IDI_SMAPER, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Anastasia",				_T("Anastasia*"),								_T("Anastasia"),						_T("ClientIcons_ICQ"),	IDI_ANASTASIA, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_JICQ",					_T("*JICQ*"),									_T("JICQ"),								_T("ClientIcons_ICQ"),	IDI_JICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_ICQ_2X",					_T("ICQ*2.x*"),									_T("ICQ 2.x"),							_T("ClientIcons_ICQ"),	IDI_ICQ2X, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_nICQ",					_T("nICQ*"),									_T("nICQ"),								_T("ClientIcons_ICQ"),	IDI_NICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_kxICQ",					_T("kxICQ*"),									_T("KxICQ2"),							_T("ClientIcons_ICQ"),	IDI_KXICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_NanoICQ",					_T("*Nano*ICQ*"),								_T("NanoICQ"),							_T("ClientIcons_ICQ"),	IDI_NANOICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Yeemp",					_T("Yeemp*"),									_T("Yeemp"),							_T("ClientIcons_ICQ"),	IDI_YEEMP, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_KM_Aska",					_T("*KM*Aska*"),								_T("KM Aska"),							_T("ClientIcons_ICQ"),	IDI_KMASKA, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_1und1",					_T("|*Pocket*Web*|*1&1*|*1*und*1")
										_T("|*1*and*1"),								_T("1&1"),								_T("ClientIcons_ICQ"),	IDI_1UND1, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_WebICQ",					_T("WebICQ*"),									_T("WebICQ"),							_T("ClientIcons_ICQ"),	IDI_WEBICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_alicq",					_T("alicq*"),									_T("Alicq"),							_T("ClientIcons_ICQ"),	IDI_ALICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Licq",					_T("Licq*"),									_T("Licq"),								_T("ClientIcons_ICQ"),	IDI_LICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_GnomeICU",				_T("GnomeICU*"),								_T("GnomeICU"),							_T("ClientIcons_ICQ"),	IDI_GNOMEICU, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_SmartICQ",				_T("SmartICQ*"),								_T("SmartICQ (via mIRC)"),				_T("ClientIcons_ICQ"),	IDI_SMARTICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Sticq",					_T("stICQ*"),									_T("stICQ"),							_T("ClientIcons_ICQ"),	IDI_STICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_GlICQ",					_T("GlICQ*"),									_T("GlICQ"),							_T("ClientIcons_ICQ"),	IDI_GLICQ, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_libicq2k",				_T("libicq2*"),									_T("LibICQ/JIT"),						_T("ClientIcons_ICQ"),	IDI_LIBICQ2000, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_IMadering",				_T("*IMadering*"),								_T("IMadering"),						_T("ClientIcons_ICQ"),	IDI_IMADERING, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_SpamBot",					_T("*Spam*Bot*"),								_T("Spam Bot"),							_T("ClientIcons_ICQ"),	IDI_SPAMBOT, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },
	{ "client_Virus",					_T("*Virus*"),									_T("Virus (Stration worm)"),			_T("ClientIcons_ICQ"),	IDI_VIRUS, INVALID_HANDLE_VALUE, ICQ_UNOFF_CASE },

//#########################################################################################################################################
//############################################## Official ICQ clients #####################################################################
//#########################################################################################################################################
	{ "client_ICQ_Rambler",				_T("ICQ*5*Rambler*"),							_T("ICQ Rambler (Lite v5)"),			_T("ClientIcons_ICQ"),	IDI_ICQ_RAMBLER, INVALID_HANDLE_VALUE, ICQ_OFF_CASE, TRUE },
	{ "client_ICQ_BigMir",				_T("ICQ*bigmir*"),								_T("ICQ bigmir.net (Lite v5)"),			_T("ClientIcons_ICQ"),	IDI_ICQ_BIGMIR, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ_NetVigator",			_T("ICQ*5*Netvigator*"),						_T("ICQ Netvigator (Lite v5)"),			_T("ClientIcons_ICQ"),	IDI_ICQ_NETVIGATOR, INVALID_HANDLE_VALUE, ICQ_OFF_CASE, TRUE },
	{ "client_ICQ_Pro7",				_T("ICQ*Pro7*"),								_T("ICQ ProSieben aka Pro7 (Lite v5)"),	_T("ClientIcons_ICQ"),	IDI_ICQ_PRO7, INVALID_HANDLE_VALUE, ICQ_OFF_CASE, TRUE },
	{ "client_ICQ_Abv",					_T("ICQ*Abv*"),									_T("ICQ ABV (Lite v5)"),				_T("ClientIcons_ICQ"),	IDI_ICQ_ABV, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ_Lite4",				_T("ICQ Lite v4*"),								_T("ICQ Lite v4"),						_T("ClientIcons_ICQ"),	IDI_ICQ_LITE4, INVALID_HANDLE_VALUE, ICQ_OFF_CASE, TRUE },
	{ "client_ICQ_Lite7",				_T("ICQ Lite v7*"),								_T("ICQ Lite v7"),						_T("ClientIcons_ICQ"),	IDI_ICQ_LITE7, INVALID_HANDLE_VALUE, ICQ_OFF_CASE, TRUE },
	{ "client_ICQ_6",					_T("|ICQ*6*|ICQ*7*"),							_T("ICQ v6,ICQ v7"),					_T("ClientIcons_ICQ"),	IDI_ICQ_6, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ_98-99",				_T("|^*Transport*|ICQ*98*|ICQ*99*|ICQ*2000*")
										_T("|ICQ*2001*|ICQ*2002*|ICQ*200?a*")
										_T("|ICQ*2003b*|ICQ*5*|ICQ Lite*|ICQ2Go!*"),	_T("ICQ/ICQ2Go!"),						_T("ClientIcons_ICQ"),	IDI_ICQ, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ComPad",					_T("*ComPad*"),									_T("ICQ ComPad"),						_T("ClientIcons_ICQ"),	IDI_ICQ_COMPAD, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ4Mac",					_T("ICQ for Mac"),								_T("ICQ for Mac"),						_T("ClientIcons_ICQ"),	IDI_ICQ4MAC, INVALID_HANDLE_VALUE, ICQ_OFF_CASE, TRUE },
	{ "client_PocketPC",				_T("ICQ*Pocket*PC*"),							_T("ICQ for PocketPC"),					_T("ClientIcons_ICQ"),	IDI_ICQ_POCKETPC, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },

//#########################################################################################################################################
//#########################################################��#### Jabber BOTS #�###########################################################
//#########################################################################################################################################
	{ "client_juick",					_T("*juick*"),									_T("Juick"),							_T("ClientIcons_Jabber"),	IDI_JUICK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_jrudevels",				_T("*jrudevels*"),								_T("Jrudevels"),						_T("ClientIcons_Jabber"),	IDI_JRUDEVELS, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_whoisbot",				_T("whoisbot"),									_T("Swissjabber Whois Bot"),			_T("ClientIcons_Jabber"),	IDI_WHOISBOT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_pjc",						_T("|*PJC*|http://pjc.googlecode.com/*"),		_T("PHP Jabber Client"), 			    _T("ClientIcons_Jabber"),	IDI_PJC, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_jdisk",					_T("|*jdisk*|JabberDisk*"),						_T("Jabber disk"),						_T("ClientIcons_Jabber"),	IDI_JDISK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Teabot",					_T("|*teabot*|http://teabot.org/bot*"),			_T("Teabot"),							_T("ClientIcons_Jabber"),	IDI_TEABOT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Psto",					_T("*Psto*"),									_T("Psto.net"),							_T("ClientIcons_Jabber"),	IDI_PSTO, INVALID_HANDLE_VALUE, JABBER_CASE },

//#########################################################################################################################################
//############################################################# Jabber clients ############################################################
//#########################################################################################################################################
	{ "client_Psi",						_T("*Psi*"),									_T("PSI"),								_T("ClientIcons_Jabber"),	IDI_PSI, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_GTalk_Gadget",			_T("*TalkGadget*"),								_T("GTalk Gadget"),						_T("ClientIcons_Jabber"),	IDI_GTALK_GADGET, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_orkut",					_T("orkut*"),									_T("orkut"),							_T("ClientIcons_Jabber"),	IDI_ORKUT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_iGoogle",					_T("iGoogle*"),									_T("iGoogle"),							_T("ClientIcons_Jabber"),	IDI_IGOOGLE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_GMail",					_T("|*gmail.*|GMail*"),							_T("GMail"),							_T("ClientIcons_Jabber"),	IDI_GMAIL, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_GTalk",					_T("|*Talk.v*|*Google*Talk*|*Gtalk*|*-iPhon*"),	_T("GoogleTalk aka GTalk"),				_T("ClientIcons_Jabber"),	IDI_GTALK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Talkonaut",				_T("*Talkonaut*"),								_T("Talkonaut"),						_T("ClientIcons_Jabber"),	IDI_TALKONAUT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Jajc",					_T("|*Jajc*|Just Another Jabber Client"),		_T("JAJC"),								_T("ClientIcons_Jabber"),	IDI_JAJC, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Tkabber",					_T("|*Tkabber*|http://tkabber*"),				_T("Tkabber"),							_T("ClientIcons_Jabber"),	IDI_TKABBER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Exodus",					_T("*Exodus*"),									_T("Exodus"),							_T("ClientIcons_Jabber"),	IDI_EXODUS, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Pandion",					_T("*Pandion*"),								_T("Pandion"),							_T("ClientIcons_Jabber"),	IDI_PANDION, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Gush",					_T("*Gush*"),									_T("Gush"),								_T("ClientIcons_Jabber"),	IDI_GUSH, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Gabber",					_T("*Gabber*"),									_T("Gabber"),							_T("ClientIcons_Jabber"),	IDI_GABBER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Gajim",					_T("*Gajim*"),									_T("Gajim"),							_T("ClientIcons_Jabber"),	IDI_GAJIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Spark",					_T("*Spark*"),									_T("Spark"),							_T("ClientIcons_Jabber"),	IDI_SPARK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Beejive",					_T("*Beejive*"),								_T("Beejive"),							_T("ClientIcons_Jabber"),	IDI_BEEJIVE, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Jabbin",					_T("*Jabbin*"),									_T("Jabbin"),							_T("ClientIcons_Jabber"),	IDI_JABBIN, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Papla",					_T("*Papla*"),									_T("Papla"),							_T("ClientIcons_Jabber"),	IDI_PAPLA, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_JabberZilla",				_T("*Zilla*"),									_T("JabberZilla"),						_T("ClientIcons_Jabber"),	IDI_JABBERZILLA, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_BombusQD",				_T("|*Bombusmod-qd*|*bombusqd*"),				_T("BombusQD"),							_T("ClientIcons_Jabber"),	IDI_BOMBUSQD, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_BombusMod",				_T("|*Bombus*mod*|*bombusmod*"),				_T("BombusMod"),						_T("ClientIcons_Jabber"),	IDI_BOMBUSMOD, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Bombus",					_T("*Bombus*"),									_T("Bombus"),							_T("ClientIcons_Jabber"),	IDI_BOMBUS, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Wildfire",				_T("Wildfire*"),								_T("Wildfire"),							_T("ClientIcons_Jabber"),	IDI_WILDFIRE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_M8Jabber",				_T("*M8Jabber*"),								_T("M8Jabber"),							_T("ClientIcons_Jabber"),	IDI_M8JABBER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_EMess",					_T("*EMess*"),									_T("EMess"),							_T("ClientIcons_Jabber"),	IDI_EMESS, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Colibry",					_T("Colibry*"),									_T("Colibry"),							_T("ClientIcons_Jabber"),	IDI_COLIBRY, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_GoTalkMobile",			_T("*Go*Talk*Mobile*"),							_T("GoTalkMobile"),						_T("ClientIcons_Jabber"),	IDI_GOTALKMOBILE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_MGTalk",					_T("Mobile?\?\?\?\?\?\?\?"),					_T("MGTalk"),							_T("ClientIcons_Jabber"),	IDI_MGTALK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Colloquy",				_T("Colloquy*"),								_T("Colloquy"),							_T("ClientIcons_Jabber"),	IDI_COLLOQUY, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_WTW",						_T("|Smok MK2*@ WTW*|*wtw*"),					_T("WTW"),								_T("ClientIcons_Jabber"),	IDI_WTW, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Spik",					_T("Spik*"),									_T("Spik"),								_T("ClientIcons_Jabber"),	IDI_SPIK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Konnekt",					_T("Konnekt*"),									_T("Konnekt"),							_T("ClientIcons_Jabber"),	IDI_KONNEKT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Vacuum",					_T("*Vacuum*"),									_T("Vacuum IM"),						_T("ClientIcons_Jabber"),	IDI_VACUUM, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Neos",					_T("Neos*"),									_T("Neos"),								_T("ClientIcons_Jabber"),	IDI_NEOS, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_GOIM",					_T("*GOIM*"),									_T("GOIM"),								_T("ClientIcons_Jabber"),	IDI_GOIM, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_SoapBox",					_T("SoapBox*"),									_T("SoapBox"),							_T("ClientIcons_Jabber"),	IDI_SOAPBOX, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_MozillaChat",				_T("*Mozilla*Chat*"),							_T("MozillaChat"),						_T("ClientIcons_Jabber"),	IDI_MOZILLACHAT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_WannaChat",				_T("Wanna*Chat*"),								_T("WannaChat"),						_T("ClientIcons_Jabber"),	IDI_WANNACHAT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Nitro",					_T("Nitro*"),									_T("Nitro"),							_T("ClientIcons_Jabber"),	IDI_NITRO, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_JBother",					_T("*JBother*"),								_T("JBother"),							_T("ClientIcons_Jabber"),	IDI_JBOTHER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_gYaber",					_T("gYaber*"),									_T("gYaber"),							_T("ClientIcons_Jabber"),	IDI_GYABER, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_myJabber",				_T("myJabber*"),								_T("myJabber"),							_T("ClientIcons_Jabber"),	IDI_MYJABBER, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_JabberMSNGR",				_T("Jabber Messenger*"),						_T("Jabber Messenger"),					_T("ClientIcons_Jabber"),	IDI_JABBER_MESSENGER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Jabberwocky",				_T("Jabberwocky*"),								_T("Jabberwocky (Amiga)"),				_T("ClientIcons_Jabber"),	IDI_JABBERWOCKY, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_TipicIM",					_T("Tipic*"),									_T("TipicIM"),							_T("ClientIcons_Jabber"),	IDI_TIPICIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Triple",					_T("Triple*"),									_T("TripleSoftwareIM (TSIM)"),			_T("ClientIcons_Jabber"),	IDI_TRIPLE_SOFTWARE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Leaf",					_T("*Leaf*"),									_T("Leaf Messenger"),					_T("ClientIcons_Jabber"),	IDI_LEAF, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Chatopus",				_T("*Chatopus*"),								_T("Chatopus"),							_T("ClientIcons_Jabber"),	IDI_CHATOPUS, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Akeni",					_T("*Akeni*"),									_T("Akeni"),							_T("ClientIcons_Jabber"),	IDI_AKENI, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Coccinella",				_T("*Coccinella*"),								_T("Coccinella"),						_T("ClientIcons_Jabber"),	IDI_COCCINELLA, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_WhisperIM",				_T("*Whisper*"),								_T("WhisperIM"),						_T("ClientIcons_Jabber"),	IDI_WHISPERIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Wija",					_T("*wija*"),									_T("Wija"),								_T("ClientIcons_Jabber"),	IDI_WIJA, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_JabberFoX",				_T("*fox*"),									_T("JabberFoX"),						_T("ClientIcons_Jabber"),	IDI_JABBERFOX, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Gossip",					_T("*Gossip*"),									_T("Gossip"),							_T("ClientIcons_Jabber"),	IDI_GOSSIP, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Fire",					_T("*Fire*"),									_T("Fire"),								_T("ClientIcons_Jabber"),	IDI_FIRE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Mercury",					_T("*Mercury*"),								_T("Mercury Messenger"),				_T("ClientIcons_Jabber"),	IDI_MERCURY_MESSENGER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_BitWise",					_T("*BitWise*"),								_T("BitWise"),							_T("ClientIcons_Jabber"),	IDI_BITWISE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Iruka",					_T("*Iruka*"),									_T("Iruka"),							_T("ClientIcons_Jabber"),	IDI_IRUKA, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Importal",				_T("*Importal*"),								_T("Importal"),							_T("ClientIcons_Jabber"),	IDI_IMPORTAL, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_kf",						_T("*kf*"),										_T("kf jabber"),						_T("ClientIcons_Jabber"),	IDI_KF, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_JClaim",					_T("*JClaim*"),									_T("JClaim"),							_T("ClientIcons_Jabber"),	IDI_JCLAIM, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Nostromo",				_T("*USCSS*Nostromo*"),							_T("USCSS Nostromo"),					_T("ClientIcons_Jabber"),	IDI_NOSTROMO, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_LLuna",					_T("LLuna*"),									_T("LLuna"),							_T("ClientIcons_Jabber"),	IDI_LLUNA, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_ChitChat",				_T("*Chit*Chat*"),								_T("ChitChat"),							_T("ClientIcons_Jabber"),	IDI_CHITCHAT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_WinJab",					_T("WinJab*"),									_T("WinJab"),							_T("ClientIcons_Jabber"),	IDI_WINJAB, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_JMC",						_T("JMC*"),										_T("JMC (Jabber Mix Client)"),			_T("ClientIcons_Jabber"),	IDI_JMC, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_BlackBerry",				_T("|*Berry*|*ICS?\?\?\?\?\?\?\?"),				_T("BlackBerry"),						_T("ClientIcons_Jabber"),	IDI_BLACKBERRY, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_mobber",					_T("*mobber*"),									_T("mobber"),							_T("ClientIcons_Jabber"),	IDI_MOBBER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Jeti",					_T("*Jeti*"),									_T("Jeti"),								_T("ClientIcons_Jabber"),	IDI_JETI, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Gnome",					_T("*Gnome*"),									_T("Gnome"),							_T("ClientIcons_Jabber"),	IDI_GNOME, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_JabberBeOS",				_T("Jabber*BeOS*"),								_T("Jabber (BeOS)"),					_T("ClientIcons_Jabber"),	IDI_JABBER_BEOS, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_JabberApplet",			_T("Jabber*Applet*"),							_T("JabberApplet"),						_T("ClientIcons_Jabber"),	IDI_JABBER_APPLET, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_JabberNaut",				_T("*Jabber*Naut*"),							_T("JabberNaut"),						_T("ClientIcons_Jabber"),	IDI_JABBERNAUT, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_moJab",					_T("moJab*"),									_T("moJab"),							_T("ClientIcons_Jabber"),	IDI_MOJAB, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Chikka",					_T("*Chikka*"),									_T("Chikka"),							_T("ClientIcons_Jabber"),	IDI_CHIKKA, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_GreenThumb",				_T("gReeNtHumB*"),								_T("GreenThumb"),						_T("ClientIcons_Jabber"),	IDI_GREENTHUMB, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_BellSouth",				_T("*BellSouth*"),								_T("BellSouth"),						_T("ClientIcons_Jabber"),	IDI_BELLSOUTH, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_IMCom",					_T("*IMCom*"),									_T("IMCom"),							_T("ClientIcons_Jabber"),	IDI_IMCOM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_BuddySpace",				_T("Buddy*Space*"),								_T("BuddySpace"),						_T("ClientIcons_Jabber"),	IDI_BUDDYSPACE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Kadu",					_T("*Kadu*"),									_T("Kadu"),								_T("ClientIcons_Jabber"),	IDI_KADU, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Xiffian",					_T("Xiffian*"),									_T("Xiffian"),							_T("ClientIcons_Jabber"),	IDI_XIFFIAN, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Mango",					_T("Mango*"),									_T("Mango"),							_T("ClientIcons_Jabber"),	IDI_MANGO, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Sky_Messager",			_T("Sky*Mess*"),								_T("Sky Messager"),						_T("ClientIcons_Jabber"),	IDI_SKYMESSAGER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_QTJim",					_T("*QTJim*"),									_T("QTJim"),							_T("ClientIcons_Jabber"),	IDI_QTJIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Tapioca",					_T("*Tapioca*"),								_T("Tapioca"),							_T("ClientIcons_Jabber"),	IDI_TAPIOCA, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Telepathy",				_T("*Telepathy*"),								_T("Telepathy"),						_T("ClientIcons_Jabber"),	IDI_TELEPATHY, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Landell",					_T("*Landell*"),								_T("Landell"),							_T("ClientIcons_Jabber"),	IDI_LANDELL, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Octro",					_T("*Octro*"),									_T("Octro"),							_T("ClientIcons_Jabber"),	IDI_OCTRO, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Desyr",					_T("*Desyr*"),									_T("Desyr Messenger"),					_T("ClientIcons_Jabber"),	IDI_DESYR, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_LinQ",					_T("*LinQ*"),									_T("LinQ"),								_T("ClientIcons_Jabber"),	IDI_LINQ, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Xeus",					_T("*Xeus*"),									_T("Xeus"),								_T("ClientIcons_Jabber"),	IDI_XEUS, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_OM",						_T("OM*"),										_T("OM aka Online Messenger"),			_T("ClientIcons_Jabber"),	IDI_OM, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Interaction",				_T("*Interaction*"),							_T("Interaction"),						_T("ClientIcons_Jabber"),	IDI_INTERACTION, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_JWGC",					_T("|*JWGC*|Jabber *Gram*"),					_T("JWGC (Jabber WindowGram Client)"),	_T("ClientIcons_Jabber"),	IDI_JWGC, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_SIP_Comm",				_T("*SIP*Communicator*"),						_T("SIP Communicator"),					_T("ClientIcons_Jabber"),	IDI_SIP_COMM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Bowline",					_T("*Bow*line*"),								_T("Bowline"),							_T("ClientIcons_Jabber"),	IDI_BOWLINE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Vayusphere",				_T("*Vayusphere*"),								_T("Vayusphere"),						_T("ClientIcons_Jabber"),	IDI_VAYUSPHERE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_TransactIM",				_T("*Transact*"),								_T("TransactIM"),						_T("ClientIcons_Jabber"),	IDI_TRANSACTIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_laffer",					_T("*laffer*"),									_T("Laffer"),							_T("ClientIcons_Jabber"),	IDI_LAFFER, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_InstanT",					_T("*Instan-t*"),								_T("Instan-t"),							_T("ClientIcons_Jabber"),	IDI_INSTANT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_GCN",						_T("*GCN*"),									_T("GCN"),								_T("ClientIcons_Jabber"),	IDI_GCN, INVALID_HANDLE_VALUE, JABBER_CASE },

	{ "client_Conference",				_T("Conference*"),								_T("Conference Bot (GMail)"),			_T("ClientIcons_Jabber"),	IDI_CONFERENCE, INVALID_HANDLE_VALUE, JABBER_CASE, TRUE },

	{ "client_Paltalk",					_T("*Paltalk*"),								_T("Paltalk"),							_T("ClientIcons_Jabber"),	IDI_PALTALK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Twitter",					_T("*Twitter*"),								_T("Twitter"),							_T("ClientIcons_Jabber"),	IDI_TWITTER, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_SMTPTransport",			_T("smtp*transport*"),							_T("SMTP Transport"),					_T("ClientIcons_Jabber"),	IDI_SMTP_TRANSPORT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_studivz",					_T("*studivz*"),								_T("StudiVZ"),							_T("ClientIcons_Jabber"),	IDI_STUDIVZ, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_schuelervz",				_T("*schuelervz*"),								_T("SchulerVZ"),						_T("ClientIcons_Jabber"),	IDI_SCHULERVZ, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_meinvz",					_T("*meinvz*"),									_T("MeinVZ"),							_T("ClientIcons_Jabber"),	IDI_MEINVZ, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_jabbim",					_T("*jabbim*"),									_T("Jabbim"),							_T("ClientIcons_Jabber"),	IDI_JABBIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_yaonline",				_T("|*yandex*|*�.������*|*������*|*yaonline*"),	_T("YA online"),						_T("ClientIcons_Jabber"),	IDI_YAONLINE, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_jabiru",					_T("|*Jabiru*|http://jabiru*"),					_T("Jabiru"),							_T("ClientIcons_Jabber"),	IDI_JABIRU, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_SworIM",					_T("*SworIM*"),									_T("SworIM"),							_T("ClientIcons_Jabber"),	IDI_SWORIM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Yambi",					_T("*Yambi*"),									_T("Yambi"),							_T("ClientIcons_Jabber"),	IDI_YAMBI, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Glu",						_T("|*glu*|http://glu.net/*"),					_T("Glu"),								_T("ClientIcons_Jabber"),	IDI_GLU, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Facebook",				_T("*Facebook*"),								_T("Facebook"),							_T("ClientIcons_Jabber"),	IDI_FACEBOOK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_WebEx",					_T("|http://webex.com/connect /*"),				_T("Cisco WebEx Connect"),				_T("ClientIcons_Jabber"),	IDI_WEBEX, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_JWChat",					_T("|jwchat*"),									_T("JWChat"),							_T("ClientIcons_Jabber"),	IDI_JWCHAT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Smack",					_T("|http://www.igniterealtime.org/projects/smack/*|*smack*"),	_T("Smack"),			_T("ClientIcons_Jabber"),	IDI_SMACK, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Android",					_T("Android *"),								_T("Android"),							_T("ClientIcons_Jabber"),	IDI_ANDROID, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_PyICQ-t",					_T("|*PyICQ-t*|*pyICQ*|ICQ*Transport*"),		_T("PyICQ-t Jabber Transport"),			_T("ClientIcons_Jabber"),	IDI_PYICQT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_PyAIM-t",					_T("|*PyAIM-t*|*pyAIM*"),						_T("PyAIM-t Jabber Transport"),			_T("ClientIcons_Jabber"),	IDI_PYAIMT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_PyMRA-t",					_T("|*PyMRA-t*|*pyMRA*|*DRQZ00fz5WP*")
										_T("|http://svn.xmpp.ru/repos/mrim*|Mrim*"),	_T("PyMRA-t Jabber Transport"),			_T("ClientIcons_Jabber"),	IDI_PYMRAT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_PyMSN-t",					_T("|*PyMSN-t*|*pyMSN*"),						_T("PyMSN-t Jabber Transport"),			_T("ClientIcons_Jabber"),	IDI_PYMSNT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_PyIRC-t",					_T("|*PyIRC-t*|*pyIRC*"),						_T("PyIRC-t Jabber Transport"),			_T("ClientIcons_Jabber"),	IDI_PYIRCT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_PyYIM-t",					_T("|*PyYIM-t*|*pyYIM*"),						_T("PyYahoo-t Jabber Transport"),		_T("ClientIcons_Jabber"),	IDI_PYYAHOOT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_GnuGadu",					_T("Gnu Gadu*"),								_T("GNU Gadu"),							_T("ClientIcons_Jabber"),	IDI_GNUGADU, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Eyeball",					_T("*Eye*ball*"),								_T("Eyeball Chat"),						_T("ClientIcons_Jabber"),	IDI_EYEBALL_CHAT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_iMov",					_T("*imov*"),									_T("iMov"),								_T("ClientIcons_Jabber"),	IDI_IMOV, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_iMeem",					_T("iMeem*"),									_T("iMeem"),							_T("ClientIcons_Jabber"),	IDI_IMEEM, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_AnothRSSBot",				_T("|*Anothrbot*|*Anothr Rss Bot*"),			_T("Anothr Rss Bot"),					_T("ClientIcons_Jabber"),	IDI_ANOTHRSSBOT, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_Imified",					_T("*someresource*"),							_T("Imified"),							_T("ClientIcons_Jabber"),	IDI_IMIFIED, INVALID_HANDLE_VALUE, JABBER_CASE },
	{ "client_RenRen",					_T("|*WTalkProxy0_0*|*http://talk.xiaonei.com*"),	_T("RenRen"),						_T("ClientIcons_Jabber"),	IDI_RENREN, INVALID_HANDLE_VALUE, JABBER_CASE },
	
//#########################################################################################################################################
//############################################################### VoIP ####################################################################
//#########################################################################################################################################
	{ "client_Gizmo",					_T("*Gizmo*"),									_T("Gizmo"),							_T("ClientIcons_VoIP"),	IDI_GIZMO, INVALID_HANDLE_VALUE, VOIP_CASE },
	{ "client_Zoep",					_T("*Zoep*"),									_T("Zoep"),								_T("ClientIcons_VoIP"),	IDI_ZOEP, INVALID_HANDLE_VALUE, VOIP_CASE },
	{ "client_WengoPhone",				_T("*Wengo*"),									_T("WengoPhone"),						_T("ClientIcons_VoIP"),	IDI_WENGOPHONE, INVALID_HANDLE_VALUE, VOIP_CASE },

//#########################################################################################################################################
//################################################################## Tlen ##################################################################
//#########################################################################################################################################
	{ "client_Tlen",					_T("Smok MK2*@ tlen.pl*"),						_T("Tlen.pl"),							_T("ClientIcons_Tlen"),	IDI_TLEN, INVALID_HANDLE_VALUE, TLEN_CASE },

//#########################################################################################################################################
//################################################################## IRC ##################################################################
//#########################################################################################################################################
	{ "client_KVIrc",					_T("*KVIrc*"),									_T("KVIrc"),							_T("ClientIcons_IRC"),	IDI_KVIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_mIRC",					_T("*mIRC*"),									_T("mIRC"),								_T("ClientIcons_IRC"),	IDI_MIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Opera",					_T("*Opera*"),									_T("Opera"),							_T("ClientIcons_IRC"),	IDI_OPERA, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_ChatZilla",				_T("*ChatZilla*"),								_T("ChatZilla"),						_T("ClientIcons_IRC"),	IDI_CHATZILLA, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_KSirc",					_T("*ksirk*"),									_T("KSirc"),							_T("ClientIcons_IRC"),	IDI_KSIRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_xBitch",					_T("*xBitch*"),									_T("xBitch"),							_T("ClientIcons_IRC"),	IDI_XBITCH, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_xChat",					_T("*xChat*"),									_T("xChat"),							_T("ClientIcons_IRC"),	IDI_XCHAT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Pirch",					_T("*Pirch*"),									_T("Pirch"),							_T("ClientIcons_IRC"),	IDI_PIRCH, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_pIRC",					_T("*pIRC*"),									_T("pIRC"),								_T("ClientIcons_IRC"),	IDI_PIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_psyBNC",					_T("*psyBNC*"),									_T("psyBNC"),							_T("ClientIcons_IRC"),	IDI_PSYBNC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_SysReset",				_T("*Sys*Reset*"),								_T("SysReset"),							_T("ClientIcons_IRC"),	IDI_SYSRESET, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_FChat",					_T("*FChat*"),									_T("FChat"),							_T("ClientIcons_IRC"),	IDI_FCHAT, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_Konversation",			_T("*Konversation*"),							_T("Konversation"),						_T("ClientIcons_IRC"),	IDI_KONVERSATION, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Conversation",			_T("*Conversation*"),							_T("Conversation"),						_T("ClientIcons_IRC"),	IDI_CONVERSATION, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_aMule",					_T("*aMule*"),									_T("aMule"),							_T("ClientIcons_IRC"),	IDI_AMULE, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_eMuleMorphXT",			_T("eMule*MorphXT*"),							_T("eMule MorphXT"),					_T("ClientIcons_IRC"),	IDI_EMULE_MORPHXT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_eMuleNeo",				_T("eMule*Neo*"),								_T("eMule Neo"),						_T("ClientIcons_IRC"),	IDI_EMULE_NEO, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_eMuleXtreme",				_T("eMule*Xtreme*"),							_T("eMule Xtreme"),						_T("ClientIcons_IRC"),	IDI_EMULE_XTREME, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_eMulePlus",				_T("|eMule*plus*|eMule*+*"),					_T("eMule+"),							_T("ClientIcons_IRC"),	IDI_EMULE_PLUS, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_eMule",					_T("*eMule*"),									_T("eMule"),							_T("ClientIcons_IRC"),	IDI_EMULE, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_EggdropRacBot",			_T("*Eggdrop*RacBot*"),							_T("Eggdrop RacBot"),					_T("ClientIcons_IRC"),	IDI_EGGDROP_RACBOT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Eggdrop",					_T("*Eggdrop*"),								_T("Eggdrop"),							_T("ClientIcons_IRC"),	IDI_EGGDROP, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_AmIRC",					_T("*AmIRC*"),									_T("AmIRC"),							_T("ClientIcons_IRC"),	IDI_AMIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_BersIRC",					_T("*BersIRC*"),								_T("BersIRC"),							_T("ClientIcons_IRC"),	IDI_BERSIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_cbirc",					_T("*cbirc*"),									_T("cbirc"),							_T("ClientIcons_IRC"),	IDI_CBIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_dIRC",					_T("*dIRC*"),									_T("dIRC"),								_T("ClientIcons_IRC"),	IDI_DIRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_GDPChat",					_T("*GDPChat*"),								_T("GDP Web Chat"),						_T("ClientIcons_IRC"),	IDI_GDPCHAT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_IceChat7",				_T("*Ice*Chat*7*"),								_T("IceChat 7"),						_T("ClientIcons_IRC"),	IDI_ICECHAT7, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_IceChat5",				_T("*Ice*Chat*5*"),								_T("IceChat 5"),						_T("ClientIcons_IRC"),	IDI_ICECHAT5, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_IceChat",					_T("*Ice*Chat*"),								_T("IceChat"),							_T("ClientIcons_IRC"),	IDI_ICECHAT, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_piorun",					_T("*piorun*"),									_T("Piorun"),							_T("ClientIcons_IRC"),	IDI_PIORUN, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_ProChat",					_T("*Pro*Chat*"),								_T("ProChat"),							_T("ClientIcons_IRC"),	IDI_PROCHAT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_SmartIRC",				_T("*Smart*IRC*"),								_T("SmartIRC"),							_T("ClientIcons_IRC"),	IDI_SMART_IRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_GoPowerTools",			_T("*Go*PowerTools*"),							_T("GoPowerTools"),						_T("ClientIcons_IRC"),	IDI_GOPOWERTOOLS, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_HydraIRC",				_T("*Hydra*IRC*"),								_T("HydraIRC"),							_T("ClientIcons_IRC"),	IDI_HYDRA_IRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_ircle",					_T("*ircle*"),									_T("ircle"),							_T("ClientIcons_IRC"),	IDI_IRCLE, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_IRCXpro",					_T("*IRCXpro*"),								_T("IRCXpro"),							_T("ClientIcons_IRC"),	IDI_IRCXPRO, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_irssi",					_T("*irssi*"),									_T("irssi"),							_T("ClientIcons_IRC"),	IDI_IRSSI, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_jircii",					_T("*jircii*"),									_T("jircii"),							_T("ClientIcons_IRC"),	IDI_JIRCII, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_jmIrc",					_T("*jmIrc*"),									_T("jmIrc"),							_T("ClientIcons_IRC"),	IDI_JMIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Klient",					_T("*Klient*"),									_T("Klient"),							_T("ClientIcons_IRC"),	IDI_KLIENT, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_zsIRC",					_T("*zsIRC*"),									_T("zsIRC"),							_T("ClientIcons_IRC"),	IDI_ZSIRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_NeoRaTrion",				_T("*NeoRa*Trion*"),							_T("NeoRa Trion"),						_T("ClientIcons_IRC"),	IDI_NEORATRION, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Nettalk",					_T("*Nettalk*"),								_T("Nettalk"),							_T("ClientIcons_IRC"),	IDI_NETTALK, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_NoNameScript",			_T("*NoName*Script*"),							_T("NoNameScript"),						_T("ClientIcons_IRC"),	IDI_NONAME_SCRIPT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_PJIRC",					_T("*PJIRC*"),									_T("PJIRC"),							_T("ClientIcons_IRC"),	IDI_PJIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Snak",					_T("*Snak*"),									_T("Snak"),								_T("ClientIcons_IRC"),	IDI_SNAK, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_VircaIRC",				_T("*VircaIRC*"),								_T("VircaIRC"),							_T("ClientIcons_IRC"),	IDI_VIRCAIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_VisionIRC",				_T("*VisionIRC*"),								_T("VisionIRC"),						_T("ClientIcons_IRC"),	IDI_VISIONIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_VisualIRC",				_T("*VisualIRC*"),								_T("VisualIRC"),						_T("ClientIcons_IRC"),	IDI_VISUALIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_VortecIRC",				_T("*VortecIRC*"),								_T("VortecIRC"),						_T("ClientIcons_IRC"),	IDI_VORTECIRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_WeeChat",					_T("*WeeChat*"),								_T("WeeChat"),							_T("ClientIcons_IRC"),	IDI_WEECHAT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_WLIrc",					_T("*WLIrc*"),									_T("WLIrc"),							_T("ClientIcons_IRC"),	IDI_WLIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_wmIRC",					_T("*wmIRC*"),									_T("wmIRC"),							_T("ClientIcons_IRC"),	IDI_WMIRC, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_XChatAqua",				_T("*X*Chat*Aqua*"),							_T("X-Chat Aqua"),						_T("ClientIcons_IRC"),	IDI_XCHATAQUA, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_ZipTorrent",				_T("*ZipTorrent*"),								_T("ZipTorrent"),						_T("ClientIcons_IRC"),	IDI_ZIPTORRENT, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_Babbel",					_T("*Babbel*"),									_T("Babbel"),							_T("ClientIcons_IRC"),	IDI_BABBEL, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_Xirc",					_T("*Xirc*"),									_T("Xirc"),								_T("ClientIcons_IRC"),	IDI_XIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_savIRC",					_T("*savIRC*"),									_T("savIRC"),							_T("ClientIcons_IRC"),	IDI_SAVIRC, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_XiRCON",					_T("*XiRCON*"),									_T("XiRCON"),							_T("ClientIcons_IRC"),	IDI_XIRCON, INVALID_HANDLE_VALUE, IRC_CASE },

	{ "client_MP3Script",				_T("*MP3*Script*"),								_T("MP3 Script for mIRC"),				_T("ClientIcons_IRC"),	IDI_MP3_SCRIPT, INVALID_HANDLE_VALUE, IRC_CASE },
	{ "client_IRCUnknown",				_T("*IRC*"),									_T("Unknown IRC client"),				_T("ClientIcons_IRC"),	IDI_IRC_UNKNOWN, INVALID_HANDLE_VALUE, IRC_CASE },

//#########################################################################################################################################
//################################################################## MSN ##################################################################
//#########################################################################################################################################
	{ "client_WLM11",					_T("WLM*2011*"),								_T("Windows Live 11"),					_T("ClientIcons_MSN"),	IDI_WLM_10, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_WLM10",					_T("WLM*2010*"),								_T("Windows Live 10"),					_T("ClientIcons_MSN"),	IDI_WLM_10, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_WLM9",					_T("WLM*9*"),									_T("Windows Live v9.x"),				_T("ClientIcons_MSN"),	IDI_WLM_9, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_MSN8",					_T("|WLM*8.*|MSN*8.*"),							_T("Windows Live v8.x"),				_T("ClientIcons_MSN"),	IDI_MSN_8, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_MSN7x",					_T("MSN*7.*"),									_T("MSN Messenger v7.x"),				_T("ClientIcons_MSN"),	IDI_MSN_7, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_MSN6x",					_T("MSN*6.*"),									_T("MSN Messenger v6.x"),				_T("ClientIcons_MSN"),	IDI_MSN_6, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_MSN45",					_T("MSN*4.x-5.x"),								_T("MSN Messenger v4.x-5.x"),			_T("ClientIcons_MSN"),	IDI_MSN_45, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_MSN4Mac",					_T("MSN*Mac*"),									_T("MSN for Mac"),						_T("ClientIcons_MSN"),	IDI_MSN_MAC, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_aMSN",					_T("*aMSN*"),									_T("aMSN"),								_T("ClientIcons_MSN"),	IDI_AMSN, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_WebMessenger",			_T("WebMessenger*"),							_T("WebMessenger"),						_T("ClientIcons_MSN"),	IDI_WEBMESSENGER, INVALID_HANDLE_VALUE, MSN_CASE },
	{ "client_MSN",						_T("|WLM*|MSN*|Windows Live*"),					_T("MSN"),								_T("ClientIcons_MSN"),	IDI_MSN, INVALID_HANDLE_VALUE, MSN_CASE, TRUE },

//#########################################################################################################################################
//################################################################## AIM ##################################################################
//#########################################################################################################################################
	{ "client_AIM_Triton",				_T("*AIM*Triton*"),								_T("AIM Triton"),						_T("ClientIcons_AIM"),	IDI_AIM_TRITON, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_Aim4",					_T("AIM*4.*"),									_T("AIM v4.x"),							_T("ClientIcons_AIM"),	IDI_AIM_4, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_Aim5",					_T("AIM*5.*"),									_T("AIM v5.x"),							_T("ClientIcons_AIM"),	IDI_AIM_5, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_AimMac",					_T("AIM*Mac*"),									_T("AIM for MacOS"),					_T("ClientIcons_AIM"),	IDI_AIM_MAC, INVALID_HANDLE_VALUE, AIM_CASE },

	{ "client_AimMobile",				_T("|AIM*Mobile*|AIM*gprs*|Aim*sms*"),			_T("AIM Mobile"),						_T("ClientIcons_AIM"),	IDI_AIM_MOBILE, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_Naim",					_T("*naim*"),									_T("Naim"),								_T("ClientIcons_AIM"),	IDI_NAIM, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_miniaim",					_T("*miniaim*"),								_T("miniaim"),							_T("ClientIcons_AIM"),	IDI_MINIAIM, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_TerraIM",					_T("*Terra*"),									_T("TerraIM"),							_T("ClientIcons_AIM"),	IDI_TERRAIM, INVALID_HANDLE_VALUE, AIM_CASE },
	{ "client_Aim",						_T("AIM*"),										_T("AIM"),								_T("ClientIcons_AIM"),	IDI_AIM, INVALID_HANDLE_VALUE, AIM_CASE },

//#########################################################################################################################################
//################################################################# Yahoo #################################################################
//#########################################################################################################################################
	{ "client_libyahoo2",				_T("*libyahoo*"),								_T("libyahoo2"),						_T("ClientIcons_Yahoo"),	IDI_LIBYAHOO2, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo5",					_T("*Yahoo*5*"),								_T("Yahoo v5.x"),						_T("ClientIcons_Yahoo"),	IDI_YAHOO5, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo6",					_T("*Yahoo*6*"),								_T("Yahoo v6.x"),						_T("ClientIcons_Yahoo"),	IDI_YAHOO6, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo7",					_T("*Yahoo*7*"),								_T("Yahoo v7.x"),						_T("ClientIcons_Yahoo"),	IDI_YAHOO7, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo8",					_T("*Yahoo*8*"),								_T("Yahoo v8.x"),						_T("ClientIcons_Yahoo"),	IDI_YAHOO8, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo9",					_T("*Yahoo*9*"),								_T("Yahoo v9.x"),						_T("ClientIcons_Yahoo"),	IDI_YAHOO8, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo10",					_T("*Yahoo*10*"),								_T("Yahoo v10.x"),						_T("ClientIcons_Yahoo"),	IDI_YAHOO10, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_YahooGoMobile",			_T("|Yahoo*Go*|Yahoo*Mobile*"),					_T("Yahoo! Go Mobile"),					_T("ClientIcons_Yahoo"),	IDI_YAHOO_GO_MOBILE, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_PingBox",					_T("*Yahoo*PingBox*"),							_T("Yahoo PingBox"),					_T("ClientIcons_Yahoo"),	IDI_PINGBOX, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_YahooWebMessenger",		_T("*Yahoo*Web*Messenger"),						_T("Yahoo Web Messenger"),				_T("ClientIcons_Yahoo"),	IDI_YAHOO10, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo",					_T("*Yahoo*"),									_T("Yahoo"),							_T("ClientIcons_Yahoo"),	IDI_YAHOO, INVALID_HANDLE_VALUE, YAHOO_CASE },

//#########################################################################################################################################
//##################################################### Gadu-Gadu clients #################################################################
//#########################################################################################################################################
	{ "client_Gadu_Gadu_7",				_T("Gadu-Gadu*7*"),								_T("Gadu-Gadu v7.x"),					_T("ClientIcons_Gadu"),	IDI_GADU_GADU_7, INVALID_HANDLE_VALUE, GADU_CASE },
	{ "client_Gadu_Gadu_6",				_T("Gadu-Gadu*6*"),								_T("Gadu-Gadu v6.x"),					_T("ClientIcons_Gadu"),	IDI_GADU_GADU_6, INVALID_HANDLE_VALUE, GADU_CASE },

//#########################################################################################################################################
//###################################################### Mail.Ru clients ##################################################################
//#########################################################################################################################################
	{ "client_MailRu_official",			_T("|magent*|Mail.ru Agent*official*|Mail.ru Agent*PC*"),	_T("Mail.Ru Agent (official client)"),	_T("ClientIcons_MRA"),	IDI_MAIL_RU_OFFICIAL, INVALID_HANDLE_VALUE, 14 },
	{ "client_MailRu_Mobile",			_T("|wmagent*|MobileAgent*|sagent*|Mail.ru Agent*Symbian*")										
										_T("|jagent*"),												_T("Mail.Ru Mobile Agent"),				_T("ClientIcons_MRA"),	IDI_MAIL_RU_MOBILE, INVALID_HANDLE_VALUE, 14 },
	{ "client_MailRu_web_official",		_T("Web Agent*"),											_T("Mail.Ru Web Agent"),				_T("ClientIcons_MRA"),	IDI_MAIL_RU_WEBAGENT, INVALID_HANDLE_VALUE, 14 },
	{ "client_MailRu_unknown",			_T("Mail.ru Agent*"),										_T("Mail.Ru (unknown client)"),			_T("ClientIcons_MRA"),	IDI_MAIL_RU_UNKNOWN, INVALID_HANDLE_VALUE, 14 },

//#########################################################################################################################################
//####################################################### Skype clients ###################################################################
//#########################################################################################################################################
	{ "client_Skype3",					_T("*Skype*3.*"),								_T("Skype v3.x"),						_T("ClientIcons_Skype"),	IDI_SKYPE3, INVALID_HANDLE_VALUE, SKYPE_CASE },
	{ "client_Skype2",					_T("*Skype*2.*"),								_T("Skype v2.x"),						_T("ClientIcons_Skype"),	IDI_SKYPE2, INVALID_HANDLE_VALUE, SKYPE_CASE },
	{ "client_Skype1",					_T("*Skype*"),									_T("Skype v1.x"),						_T("ClientIcons_Skype"),	IDI_SKYPE1, INVALID_HANDLE_VALUE, SKYPE_CASE },

//#########################################################################################################################################
//###################################################### Weather clients ##################################################################
//#########################################################################################################################################
	{ "client_accu",					_T("*accuweather*"),							_T("AccuWeather"),						_T("ClientIcons_Weather"),	IDI_ACCU, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_gismeteo",				_T("*gismeteo*"),								_T("GisMeteo"),							_T("ClientIcons_Weather"),	IDI_GISMETEO, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_intelli",					_T("*intellicast*"),							_T("Intellicast"),						_T("ClientIcons_Weather"),	IDI_INTELLI, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_meteogid",				_T("|*meteo-gid*|*meteogid*|*MeteoGid*"),		_T("Meteo-Gid"),						_T("ClientIcons_Weather"),	IDI_METEOGID, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_meteonovosti",			_T("*meteonovosti*"),							_T("Meteonovosti"),						_T("ClientIcons_Weather"),	IDI_METEONOVOSTI, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_noaa",					_T("*noaa*"),									_T("NOAA Weather"),						_T("ClientIcons_Weather"),	IDI_NOAA, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_real",					_T("*realmeteo*"),								_T("RealMeteo"),						_T("ClientIcons_Weather"),	IDI_REALMETEO, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_under",					_T("Weather Underground*"),						_T("Weather Underground"),				_T("ClientIcons_Weather"),	IDI_UNDERGROUND, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_weatherxml",				_T("*WeatherXML*"),								_T("WeatherXML"),						_T("ClientIcons_Weather"),	IDI_WEATHERXML, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_wetter",					_T("*wetter*"),									_T("Wetter"),							_T("ClientIcons_Weather"),	IDI_WETTER, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_yweather",				_T("*Yahoo Weather*"),							_T("Yahoo Weather"),					_T("ClientIcons_Weather"),	IDI_YWEATHER, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_weather_cn",				_T("*weather.com.cn*"),							_T("Weather CN"),						_T("ClientIcons_Weather"),	IDI_WEATHER_CN, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },
	{ "client_weather",					_T("*weather*"),								_T("Weather"),							_T("ClientIcons_Weather"),	IDI_WEATHER, INVALID_HANDLE_VALUE, WEATHER_CASE, TRUE },

//#########################################################################################################################################
//############################################################ RSS ########################################################################
//#########################################################################################################################################
	{ "client_rss09x",					_T("*RSS*0.9*"),								_T("RSS 0.9x"),							_T("ClientIcons_RSS"),	IDI_RSS09, INVALID_HANDLE_VALUE, RSS_CASE },
	{ "client_rss1",					_T("*RSS*1.*"),									_T("RSS 1"),							_T("ClientIcons_RSS"),	IDI_RSS1, INVALID_HANDLE_VALUE, RSS_CASE },
	{ "client_rss2",					_T("*RSS*2.*"),									_T("RSS 2"),							_T("ClientIcons_RSS"),	IDI_RSS2, INVALID_HANDLE_VALUE, RSS_CASE },
	{ "client_atom1",					_T("*Atom*1.*"),								_T("Atom 1"),							_T("ClientIcons_RSS"),	IDI_ATOM1, INVALID_HANDLE_VALUE, RSS_CASE },
	{ "client_atom3",					_T("*Atom*3.*"),								_T("Atom 3"),							_T("ClientIcons_RSS"),	IDI_ATOM3, INVALID_HANDLE_VALUE, RSS_CASE },

//#########################################################################################################################################
//############################################################ QQ #########################################################################
//#########################################################################################################################################
	{ "client_MobileQQ",				_T("|Mobile*QQ*|iPhone*QQ*|QQ WAP*"),			_T("Mobile QQ"),						_T("ClientIcons_QQ"),	IDI_MOBILEQQ, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_QQConcept",				_T("QQ*Concept*"),								_T("QQ Concept"),						_T("ClientIcons_QQ"),	IDI_QQCONCEPT, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_QQCRM",					_T("QQ*CRM*"),									_T("QQ CRM"),							_T("ClientIcons_QQ"),	IDI_QQCRM, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_QQSilver",				_T("QQ*Silverlight*"),							_T("QQ Silverlight"),					_T("ClientIcons_QQ"),	IDI_QQSILVER, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM2008",					_T("|TM2008*|TM2009*|TM2010*"),					_T("TM 2008 and >"),					_T("ClientIcons_QQ"),	IDI_TM2008, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM",						_T("TM*"),										_T("TM"),								_T("ClientIcons_QQ"),	IDI_TM, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_QQInt",					_T("QQ*International*"),						_T("QQ International"),					_T("ClientIcons_QQ"),	IDI_QQINT, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_YamQQ",					_T("YamQQ*"),									_T("Yam QQ"),							_T("ClientIcons_QQ"),	IDI_YAMQQ, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_WebQQ",					_T("WebQQ*"),									_T("WebQQ"),							_T("ClientIcons_QQ"),	IDI_WEBQQ, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_QQ",						_T("QQ*"),										_T("QQ"),								_T("ClientIcons_QQ"),	IDI_QQ, INVALID_HANDLE_VALUE, QQ_CASE },

	{ "client_Notfound",				_T("Notfound"),									_T("Not founded icon"),					_T(""),	IDI_NOTFOUND, INVALID_HANDLE_VALUE, OTHER_CASE },
	{ "client_Unknown",					_T("*Unknown*"),								_T("Unknown client"),					_T(""),	IDI_UNKNOWN, INVALID_HANDLE_VALUE, OTHER_CASE },
	{ "client_Undetected",				_T("?*"),										_T("Undetected client"),				_T(""),	IDI_UNDETECTED, INVALID_HANDLE_VALUE, OTHER_CASE },
};

#define DEFAULT_KN_FP_MASK_COUNT SIZEOF(def_kn_fp_mask)									// should contain the count of records above
#define UNKNOWN_MASK_NUMBER (DEFAULT_KN_FP_MASK_COUNT - 2)								// second from end
#define NOTFOUND_MASK_NUMBER (DEFAULT_KN_FP_MASK_COUNT - 3)								// third from end

//#########################################################################################################################################
//######################################################### Overlays, layer #1 ############################################################
//#########################################################################################################################################
KN_FP_MASK def_kn_fp_overlays_mask[] =
{//	IconName							Mask										Icon caption							NULL ICON_RESOURCE_ID
//#########################################################################################################################################
//#################################################### Miranda Packs ######################################################################
//#########################################################################################################################################
	{ "client_HIEROS_",			_T("*HierOS*"),								_T("HierOS"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_HIEROS, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_YAOL_",			_T("*yaol*"),								_T("YAOL"),								_T("ClientIcons_Packs"),	IDI_MIRANDA_YAOL, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_KDL_",			_T("|*KDL*|*���*"),							_T("KDL"),								_T("ClientIcons_Packs"),	IDI_MIRANDA_KDL, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_ValeraVi_",		_T("*Valera*Vi*"),							_T("ValeraVi"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_VALERAVI, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Stalker_",		_T("*Stalker*"),							_T("Stalker"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_STALKER, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Kuzzman_",		_T("*kuzzman*"),							_T("Kuzzman"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_KUZZMAN, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_AF_",				_T("*AF*Pack*"),							_T("AF"),								_T("ClientIcons_Packs"),	IDI_MIRANDA_AF, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Kolich_",			_T("*Kolich*"),								_T("Kolich"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_KOLICH, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Umedon_",			_T("*Miranda*Umedon*"),						_T("Umedon"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_UMEDON, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Se7ven_",			_T("|^*sss*|*[S7*pack]*|*[S7]*"),			_T("Se7ven"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_SE7VEN, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_SSS_",			_T("*sss*pack*"),							_T("SSS build"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_SSS_MOD, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Faith_",			_T("*Faith*Pack*"),							_T("Faith"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_FAITH, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_LexSys_",			_T("|*MirandaLS*|*LexSys*"),				_T("LexSys"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_LEXSYS, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_dmikos_",			_T("*dmikos*"),								_T("Dmikos"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_DMIKOS, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Tweety_",			_T("*tweety*"),								_T("Tweety"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_TWEETY, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_ME_",				_T("*[Miranda ME]*"),						_T("Miranda ME"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_ME, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_BRI_",			_T("*bri*edition*"),						_T("Bri edition"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_BRI, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_ALFAMAR_",		_T("*AlfaMaR*"),							_T("AlfaMaR"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_ALFAMAR, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_ZELEBOBA_",		_T("*zeleboba's*"),							_T("zeleboba's"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_ZELEBOBA, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_MD_",				_T("*MDpack*"),								_T("MDpack"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_MD, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_PILOT_",			_T("*Pilot*"),								_T("Pilot"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_PILOT, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_AMATORY_",		_T("*Amatory*"),							_T("Amatory"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_AMATORY, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_RAZUNTER_",		_T("*Razunter's*"),							_T("Razunter's Pk"),					_T("ClientIcons_Packs"),	IDI_MIRANDA_RAZUNTER, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_GHOST_",			_T("*Ghost's*"),							_T("Ghost's Pack"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_GHOST, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_DAR_",			_T("*Dar_veter*"),							_T("Dar_veter Pack"),					_T("ClientIcons_Packs"),	IDI_MIRANDA_DAR, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_MATAES_",			_T("*Mataes*"),								_T("Mataes Pack"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_MATAES, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_FREIZE_",			_T("*Freize*"),								_T("Freize"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_FREIZE, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_FR_",				_T("*FR*Pack*"),							_T("Miranda FR"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_FR, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_Native_",			_T("*Native*"),								_T("Native"),							_T("ClientIcons_Packs"),	IDI_MIRANDA_NATIVE, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_E33_",			_T("*[E33]*"),								_T("E33"),								_T("ClientIcons_Packs"),	IDI_MIRANDA_E33, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_ROBYER_",			_T("*RobyerPack*"),							_T("Robyer Pack"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_ROBYER, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_New Style_",		_T("*New Style*"),							_T("New Style"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_NEW_STYLE, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_SPELLHOWLER_",	_T("*Spellhowler*"),				    	_T("xSpellhowleRx Pack"),				_T("ClientIcons_Packs"),	IDI_MIRANDA_SPELLHOWLER, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_LESTAT_",			_T("*[Lpack]*"),						    _T("Lestat Pack"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_LESTAT, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_DEVILSUITE_",		_T("*6.6.6*"),						    	_T("Devil Suite"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_DEVIL, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_FINAL",			_T("*[FinalPack]*"),						_T("Final Pack"),						_T("ClientIcons_Packs"),	IDI_MIRANDA_FINAL, INVALID_HANDLE_VALUE, PACKS_CASE },
	{ "client_CAPPUCCINO",		_T("*[Cappuccino]*"),						_T("HotCoffee mod Pack"),				_T("ClientIcons_Packs"),	IDI_MIRANDA_CAPPUCCINO, INVALID_HANDLE_VALUE, PACKS_CASE },

//#########################################################################################################################################
//#########################################################################################################################################
	{ "client_AimOscar_over",			_T("Miranda*AIM*"),									_T("AIM"),								_T("ClientIcons_General"),	IDI_AIMOSCAR_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },		//FIX
	{ "client_ICQJ_Plus_over",			_T("*ICQ*Plus*"),									_T("ICQJ Plus Mod"),					_T("ClientIcons_General"),	IDI_ICQJ_PLUS_MOD_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_ICQJ_SIN_over",			_T("*ICQ*S!N*"),									_T("ICQ S!N"),							_T("ClientIcons_General"),	IDI_ICQJ_SIN_MOD_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_ICQJ_over",				_T("*IM*ICQ*0.*"),									_T("ICQJ"),								_T("ClientIcons_General"),	IDI_ICQJ_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_ICQ_Unknown_over",		_T("Miranda*ICQ*"),									_T("ICQ unknown"),						_T("ClientIcons_General"),	IDI_ICQJM_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_IRC_over",				_T("Miranda*IRC*"),									_T("IRC"),								_T("ClientIcons_General"),	IDI_IRC_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_JGmail_over",				_T("*IM*JGmail*"),									_T("JGmail"),							_T("ClientIcons_General"),	IDI_GMAIL_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_JGTalk_over",				_T("*IM*JGTalk*"),									_T("JGTalk"),							_T("ClientIcons_General"),	IDI_JGTALK_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_JMeebo_over",				_T("*IM*JMeebo*"),									_T("JMeebo"),							_T("ClientIcons_General"),	IDI_JMEEBO_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Jabber_over",				_T("|*IM*jabber*|*t Jabber*"),						_T("Jabber"),							_T("ClientIcons_General"),	IDI_JABBER_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_MailRu_over",				_T("|*Mail.ru*IM*|*MRA*"),							_T("MailRu Agent"),						_T("ClientIcons_General"),	IDI_MAILRU_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_MSN_over",				_T("Miranda*MSN*"),									_T("MSN"),								_T("ClientIcons_General"),	IDI_MSN_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_QQ_over",					_T("|Miranda*QQ*|MIMQQ*"),							_T("QQ"),								_T("ClientIcons_General"),	IDI_QQ_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Tlen_over",				_T("*tlen.pl"),										_T("Tlen.pl"),							_T("ClientIcons_General"),	IDI_TLEN_OVERLAY, INVALID_HANDLE_VALUE, OVER1_CASE },

	{ "client_Mira_010_over",			_T("*Miranda*0.10*"),								_T("Miranda v0.10"),					_T("ClientIcons_General"),	IDI_MIRANDA_10, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Mira_09_over",			_T("*Miranda*0.9*"),								_T("Miranda v0.9"),						_T("ClientIcons_General"),	IDI_MIRANDA_9, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Mira_08_over",			_T("*Miranda*0.8*"),								_T("Miranda v0.8"),						_T("ClientIcons_General"),	IDI_MIRANDA_8, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Mira_07_over",			_T("*Miranda*0.7*"),								_T("Miranda v0.7"),						_T("ClientIcons_General"),	IDI_MIRANDA_7, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Mira_06_over",			_T("*Miranda*0.6*"),								_T("Miranda v0.6"),						_T("ClientIcons_General"),	IDI_MIRANDA_6, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Mira_05_over",			_T("*Miranda*0.5*"),								_T("Miranda v0.5"),						_T("ClientIcons_General"),	IDI_MIRANDA_5, INVALID_HANDLE_VALUE, OVER1_CASE },
	{ "client_Mira_04_over",			_T("*Miranda*0.4*"),								_T("Miranda v0.4"),						_T("ClientIcons_General"),	IDI_MIRANDA_4, INVALID_HANDLE_VALUE, OVER1_CASE },

	{ "client_ICQ 2000",				_T("ICQ*2000*"),									_T("ICQ 2000"),							_T("ClientIcons_ICQ"),	IDI_CLIENTICQ2000, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ 2001b",				_T("ICQ*2001*"),									_T("ICQ 2001"),							_T("ClientIcons_ICQ"),	IDI_CLIENTICQ2001, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ 2002",				_T("ICQ*2002*"),									_T("ICQ 2002/2003a"),					_T("ClientIcons_ICQ"),	IDI_CLIENTICQ2002, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ 2003",				_T("ICQ*2003*"),									_T("ICQ 2003b"),						_T("ClientIcons_ICQ"),	IDI_CLIENTICQ2003, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ v4",					_T("ICQ*4*"),										_T("ICQ Lite v4"),						_T("ClientIcons_ICQ"),	IDI_CLIENTICQLITE4, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ v5.1",				_T("ICQ*5.1*"),										_T("ICQ v5.1"),							_T("ClientIcons_ICQ"),	IDI_CLIENTICQLITE51, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ v5",					_T("|^*Transport*|ICQ*5*"),							_T("ICQ v5"),							_T("ClientIcons_ICQ"),	IDI_CLIENTICQLITE5, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ v6",					_T("ICQ*6*"),										_T("ICQ v6"),							_T("ClientIcons_ICQ"),	IDI_CLIENTICQ6, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },
	{ "client_ICQ v7",					_T("ICQ*7*"),										_T("ICQ v7"),							_T("ClientIcons_ICQ"),	IDI_CLIENTICQ7, INVALID_HANDLE_VALUE, ICQ_OFF_CASE },

	{ "client_Yahoo v5.x",				_T("*Yahoo 5*"),									_T("Yahoo 5.x"),						_T("ClientIcons_Yahoo"),	IDI_CLIENT_YAHOO5, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo v6.x",				_T("*Yahoo 6*"),									_T("Yahoo 6.x"),						_T("ClientIcons_Yahoo"),	IDI_CLIENT_YAHOO6, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo v7.x",				_T("*Yahoo 7*"),									_T("Yahoo 7.x"),						_T("ClientIcons_Yahoo"),	IDI_CLIENT_YAHOO7, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo v8.x",				_T("*Yahoo 8*"),									_T("Yahoo 8.x"),						_T("ClientIcons_Yahoo"),	IDI_CLIENT_YAHOO8, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo v9.x",				_T("*Yahoo 9*"),									_T("Yahoo 9.x"),						_T("ClientIcons_Yahoo"),	IDI_CLIENT_YAHOO9, INVALID_HANDLE_VALUE, YAHOO_CASE },
	{ "client_Yahoo v10.x",				_T("*Yahoo 10*"),									_T("Yahoo 10.x"),						_T("ClientIcons_Yahoo"),	IDI_CLIENT_YAHOO10, INVALID_HANDLE_VALUE, YAHOO_CASE },

	{ "client_QQ 2000",					_T("QQ2000*"),										_T("QQ 2000"),							_T("ClientIcons_QQ"),	IDI_CLIENT_QQ2000, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM 1",					_T("TM 1.*"),										_T("TM 1.x"),							_T("ClientIcons_QQ"),	IDI_CLIENT_TM1, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM 2",					_T("TM 2.*"),										_T("TM 2.x"),							_T("ClientIcons_QQ"),	IDI_CLIENT_TM2, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TMQQ 2003",				_T("|QQ2003*|TM 3.*"),								_T("TM 3.x, QQ 2003"),					_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2003, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_QQ 2004",					_T("QQ2004*"),										_T("QQ 2004"),							_T("ClientIcons_QQ"),	IDI_CLIENT_QQ2004, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2005",				_T("|QQ2005*|TM2005*"),								_T("TM/QQ 2005"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2005, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2006",				_T("|QQ2006*|TM2006*"),								_T("TM/QQ 2006"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2006, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2007",				_T("|QQ2007*|TM2007*"),								_T("TM/QQ 2007"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2007, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2008",				_T("|QQ2008*|TM2008*"),								_T("TM/QQ 2008"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2008, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2009",				_T("|QQ2009*|TM2009*"),								_T("TM/QQ 2009"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2009, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2010",				_T("|QQ2010*|TM2010*"),								_T("TM/QQ 2010"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2010, INVALID_HANDLE_VALUE, QQ_CASE },
	{ "client_TM_QQ 2011",				_T("|QQ2011*|TM2011*"),								_T("TM/QQ 2011"),						_T("ClientIcons_QQ"),	IDI_CLIENT_TM_QQ2011, INVALID_HANDLE_VALUE, QQ_CASE },

	{ "client_weather_over",			_T("*Weather*"),									_T("Weather"),							_T("ClientIcons_Weather"),	IDI_CLIENT_WEATHER, INVALID_HANDLE_VALUE, WEATHER_CASE },
	
	{ "client_on Win32",				_T("|*Win32*|*windows*|* WM *|wmagent*|*Vista*"),	_T("on Windows"),						_T("ClientIcons_General"),	IDI_CLIENTWIN, INVALID_HANDLE_VALUE, PLATFORM_CASE },
	{ "client_on Mac",					_T("|* Mac *|*IPhone*"),							_T("on MacOS"),							_T("ClientIcons_General"),	IDI_CLIENTMAC, INVALID_HANDLE_VALUE, PLATFORM_CASE },
	{ "client_on Linux",				_T("*Linux*"),										_T("on Linux"),							_T("ClientIcons_General"),	IDI_CLIENTLINUX, INVALID_HANDLE_VALUE, PLATFORM_CASE },
	{ "client_on Flash",				_T("*Flash*"),										_T("on Flash"),							_T("ClientIcons_General"),	IDI_CLIENTFLASH, INVALID_HANDLE_VALUE, PLATFORM_CASE },
	{ "client_on Java",					_T("|*Java*|jagent*|ICQ2Go!*"),						_T("on Java"),							_T("ClientIcons_General"),	IDI_CLIENTJAVA, INVALID_HANDLE_VALUE, PLATFORM_CASE },
	{ "client_on Symbian",				_T("|*Symbian*|sagent*"),							_T("on Symbian"),						_T("ClientIcons_General"),	IDI_CLIENTSYMBIAN, INVALID_HANDLE_VALUE, PLATFORM_CASE },
	{ "client_on Android",				_T("|* Android*|*/Android*"),						_T("on Android"),						_T("ClientIcons_General"),	IDI_CLIENTANDROID, INVALID_HANDLE_VALUE, PLATFORM_CASE },
};

#define DEFAULT_KN_FP_OVERLAYS_COUNT SIZEOF(def_kn_fp_overlays_mask)					//should contain the count of records above

//#########################################################################################################################################
//######################################################### Overlays, layer #2 ############################################################
//#########################################################################################################################################
KN_FP_MASK def_kn_fp_overlays2_mask[] =
{//	IconName							Mask										Icon caption							NULL ICON_RESOURCE_ID
	{ "client_debug_overlay",			_T("*[*debug*]*"),							_T("[Debug]"),							_T("ClientIcons_Jabber"),	IDI_DEBUG_OVERLAY, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_home_overlay",			_T("|*[*home*]*|*[*���*]*"),				_T("[Home]"),							_T("ClientIcons_Jabber"),	IDI_HOME_OVERLAY, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_work_overlay",			_T("|*[*work*]*|*[*������*]*"),				_T("[Work]"),							_T("ClientIcons_Jabber"),	IDI_WORK_OVERLAY, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_office_overlay",			_T("|*[*office*]*|*[*����*]*"),				_T("[Office]"),							_T("ClientIcons_Jabber"),	IDI_OFFICE_OVERLAY, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_mobile_overlay",			_T("|*[*mobile*]*|*[*pda*]*"),				_T("[Mobile]"),							_T("ClientIcons_Jabber"),	IDI_MOBILE_OVERLAY, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_notebook_overlay",		_T("|*[*note*]*|*[*portable*]*|*[*usb*]*")
										_T("|*[*laptop*]*|*[*flash*]*"),			_T("[Portable]"),						_T("ClientIcons_Jabber"),	IDI_NOTEBOOK_OVERLAY, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_010_overlay",		_T("*Miranda*0.10*"),						_T("Miranda v0.10"),					_T("ClientIcons_General"),	IDI_MIRANDA_10s, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_09_overlay",		_T("*Miranda*0.9*"),						_T("Miranda v0.9"),						_T("ClientIcons_General"),	IDI_MIRANDA_9s, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_08_overlay",		_T("*Miranda*0.8*"),						_T("Miranda v0.8"),						_T("ClientIcons_General"),	IDI_MIRANDA_8s, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_07_overlay",		_T("*Miranda*0.7*"),						_T("Miranda v0.7"),						_T("ClientIcons_General"),	IDI_MIRANDA_7s, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_06_overlay",		_T("*Miranda*0.6*"),						_T("Miranda v0.6"),						_T("ClientIcons_General"),	IDI_MIRANDA_6s, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_05_overlay",		_T("*Miranda*0.5*"),						_T("Miranda v0.5"),						_T("ClientIcons_General"),	IDI_MIRANDA_5s, INVALID_HANDLE_VALUE, OVER2_CASE },
	{ "client_Miranda_04_overlay",		_T("*Miranda*0.4*"),						_T("Miranda v0.4"),						_T("ClientIcons_General"),	IDI_MIRANDA_4s, INVALID_HANDLE_VALUE, OVER2_CASE },
};

#define DEFAULT_KN_FP_OVERLAYS2_COUNT SIZEOF(def_kn_fp_overlays2_mask)					//should contain the count of records above
#define DEFAULT_KN_FP_OVERLAYS2_NO_VER_COUNT DEFAULT_KN_FP_OVERLAYS2_COUNT - 7
//#########################################################################################################################################
//######################################################### Overlays, layer #3 ############################################################
//#########################################################################################################################################
KN_FP_MASK def_kn_fp_overlays3_mask[] =
{//	IconName							Mask										Icon caption							NULL ICON_RESOURCE_ID
	{ "client_SecureIM_overlay",		_T("*Secure*IM*"),							_T("+SecureIM"),						_T("ClientIcons_General"),	IDI_SECUREIM_OVERLAY, INVALID_HANDLE_VALUE, SECURITY_CASE },
	{ "client_SimpPro_overlay",			_T("*SimpPro*"),							_T("+SimpPro"),							_T("ClientIcons_General"),	IDI_SIMPPRO_OVERLAY, INVALID_HANDLE_VALUE, SECURITY_CASE },
	{ "client_SimpLite_overlay",		_T("*SimpLite*"),							_T("+SimpLite"),						_T("ClientIcons_General"),	IDI_SIMPLITE_OVERLAY, INVALID_HANDLE_VALUE, SECURITY_CASE },
	{ "client_IMSecure_overlay",		_T("*IMSecure*"),							_T("+IMSecure"),						_T("ClientIcons_General"),	IDI_IMSECURE_OVERLAY, INVALID_HANDLE_VALUE, SECURITY_CASE },
};

#define DEFAULT_KN_FP_OVERLAYS3_COUNT SIZEOF(def_kn_fp_overlays3_mask)					//should contain the count of records above
