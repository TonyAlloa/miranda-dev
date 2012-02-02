#ifndef commonheaders_h__
#define commonheaders_h__

/*
Traffic Counter plugin for Miranda IM 
Copyright 2007-2011 Mironych.

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

#pragma warning(disable: 4786) // identifier was truncated in the debug information 
#pragma warning(disable: 4127) // constant expression 


#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501		// XP
#endif

/*
#define _WIN32_WINNT 0x0502		// Server 2003
#define _WIN32_WINNT 0x0600		// Win Vista
#define _WIN32_WINNT 0x0601		// Win 7
*/

#define MIRANDA_VER 0x0900
#define MIRANDA_CUSTOM_LP
//#define _USE_32BIT_TIME_T 1

// Standart includes
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <time.h>
#include <tchar.h>

#include <win2k.h>

#include "../resource.h"

// Miranda SDK includes
#include <newpluginapi.h>
#include <m_system.h>
#include <m_netlib.h>
#include <m_langpack.h>
#include <m_clist.h>
#include <m_clistint.h>
#include <m_clui.h>
#include <m_clc.h>
#include <m_options.h>
#include <m_protocols.h>
#include <m_protosvc.h>
#include <m_cluiframes.h>
#include <m_database.h>
#include <m_fontservice.h>
#include <m_popup.h>
#include <m_skin.h>
#include <m_hotkeys.h>

// For Modern Contact List
#include "../Modernb/modern_global_structure.h"
#include "m_skin_eng.h"
#include "m_variables.h"

struct MM_INTERFACE memoryManagerInterface;

#pragma pack(push)
#pragma pack(1)
typedef	struct
{
	BYTE Hour, Day, Month;
	WORD Year;
	DWORD Incoming, Outgoing;
	WORD Time;
} HOURLYSTATS;
#pragma pack(pop)

typedef struct tagTimer
{
	DWORD TimeAtStart; // ����� � ������ ������� ������� - � �������������.
	DWORD Timer; // ���������� ������ �� ������� ������� �������.
} TIMER;

typedef struct
{
	char *name; // ��� ��������.

	TIMER Session; // ������ ������� ������ (�������� � �������).
	TIMER Total; // ������ �����.

	DWORD TotalRecvTraffic, // ����� ������ ��������� (�� ��������� ������)
		  TotalSentTraffic,
		  CurrentRecvTraffic, // ������� ������ ��������� (�� ������)
		  CurrentSentTraffic;
	union
	{
		BYTE Flags;
		struct
		{
			unsigned int Reserv0:1; // ���������� �������� ����� - ���������� ������ �� ���� ���������.
			unsigned int Visible:1; // = 1 - ������� ����� ������������ �� ������ ���������
			unsigned int Enabled:1; // = 1 - ������� ������� � �� ��������
			unsigned int State:1;   // = 1 - ������� ������ ������
			unsigned int Reserv1:3;
		};
	};

	// ��������� � ������ 0.1.1.0.
	DWORD NumberOfRecords; // ���������� ����� � ����� ����������.
	HOURLYSTATS *AllStatistics; // ������ ���������� ������ �� ����������� �������.
	HANDLE hFile; // ���� � ����������� ����������� ������� ���������.

	DWORD StartIndex; // ����� ������ � ����������, ������ ���������� �� ������ �������.
	DWORD StartIncoming; // �������� ��������� ������� �� ������ �������.
	DWORD StartOutgoing; // �������� ���������� ������� �� ������ �������.

	// 0.1.1.5.
	DWORD Shift;	// ����� ������ � ���������� ���������� ���������� ��������,
					// ���� ������� ������������� ������ ���������� ������� ��������.

	// 0.1.1.6
	TCHAR *tszAccountName; // ������������ ��� �������� ��� ������������� � ����������� ����������.
} PROTOLIST;

//---------------------------------------------------------------------------------------------
// ��������� �����
//---------------------------------------------------------------------------------------------
union
{
	DWORD Flags;
	struct
	{
		unsigned int NotifyBySize:1;			//0
		unsigned int DrawCurrentTraffic:1;		//1
		unsigned int DrawTotalTraffic:1;		//2
		unsigned int DrawCurrentTimeCounter:1;	//3
		unsigned int DrawProtoIcon:1;			//4
		unsigned int Reserv0:1;					//5
		unsigned int DrawProtoName:1;			//6
		unsigned int DrawFrmAsSkin:1;			//7
		unsigned int ShowSummary:1;				//8
		unsigned int ShowTooltip:1;				//9
		unsigned int ShowMainMenuItem:1;		//10
		unsigned int ShowOverall:1;				//11
		unsigned int Stat_Units:2;				//12,13
		unsigned int Stat_Tab:3;				//14,15,16
		unsigned int NotifyByTime:1;			//17
		unsigned int Reserv2:1;					//18
		unsigned int PeriodForShow:2;			//19,20
		unsigned int FrameIsVisible:1;			//21
		unsigned int Reserv1:1;					//22
		unsigned int DrawTotalTimeCounter:1;	//23
	};
} unOptions;

PROTOLIST *ProtoList; // ������ ��� ���� ���������.
PROTOLIST OverallInfo; // ��������� ������ �� ������� ���������.
BYTE NumberOfAccounts;
WORD Stat_SelAcc; // ��������� �������� � ���� ����������
HWND TrafficHwnd;
DWORD mirandaVer;

#include "misc.h"
#include "opttree.h"
#include "vars.h"
#include "statistics.h"
#include "TrafficCounter.h"

#endif