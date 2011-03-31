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
/* ======================================================================================
����� ���������� ������� ��� ��������� ������� Variables
�����: Mironych
=======================================================================================*/

#include "commonheaders.h"
#define L1 16
#define L2 255

static TCHAR* GetTraffic(ARGUMENTSINFO *ai)
{
	DWORD tmp, tmpsn = 0, tmprn = 0, tmpst = 0, tmprt = 0;
	BYTE ed;
	static TCHAR res[L1];
	
	if (ai->argc != 5) return NULL;

	if (!_tcscmp(ai->targv[1], _T("overall")))
	{
		tmpsn = OverallInfo.CurrentSentTraffic;
		tmprn = OverallInfo.CurrentRecvTraffic;
		tmpst = OverallInfo.TotalSentTraffic;
		tmprt = OverallInfo.TotalRecvTraffic;
	}
	else
	if (!_tcscmp(ai->targv[1], _T("summary")))
	{
		for (ed = 0; ed < NumberOfAccounts; ed++)
			if (ProtoList[ed].Visible)
			{
				tmpsn += ProtoList[ed].CurrentSentTraffic;
				tmprn += ProtoList[ed].CurrentRecvTraffic;
				tmpst += ProtoList[ed].TotalSentTraffic;
				tmprt += ProtoList[ed].TotalRecvTraffic;
			}
	}
	else
	{	// ���� ������ ���������, ����������� ������ ����������
		for (tmp = ed = 0; ed < NumberOfAccounts; ed++)
		{
			TCHAR *buf;
			if (!ProtoList[ed].name) continue;
			buf = mir_a2t(ProtoList[ed].name);
			if (!_tcscmp(buf, ai->targv[1]))
			{
				tmpsn = ProtoList[ed].CurrentSentTraffic;
				tmprn = ProtoList[ed].CurrentRecvTraffic;
				tmpst = ProtoList[ed].TotalSentTraffic;
				tmprt = ProtoList[ed].TotalRecvTraffic;
				tmp = 0xAA; // ������� ����, ��� �������� ��� ������
			}
			mir_free(buf);
		}
		if (tmp != 0xAA) return NULL;
	}

	if (!_tcscmp(ai->targv[2], _T("now")))
	{
		if (!_tcscmp(ai->targv[3], _T("sent"))) tmp = tmpsn;
		else
		if (!_tcscmp(ai->targv[3], _T("recieved"))) tmp = tmprn;
		else
		if (!_tcscmp(ai->targv[3], _T("both"))) tmp = tmprn + tmpsn;
		else return NULL;
	}
	else
	if (!_tcscmp(ai->targv[2], _T("total")))
	{
		if (!_tcscmp(ai->targv[3], _T("sent"))) tmp = tmpst;
		else
		if (!_tcscmp(ai->targv[3], _T("recieved"))) tmp = tmprt;
		else
		if (!_tcscmp(ai->targv[3], _T("both"))) tmp = tmprt + tmpst;
		else return NULL;
	}
	else return NULL;

	if (!_tcscmp(ai->targv[4], _T("b"))) GetFormattedTraffic(tmp, 0, res);
	else
	if (!_tcscmp(ai->targv[4], _T("k"))) GetFormattedTraffic(tmp, 1, res);
	else
	if (!_tcscmp(ai->targv[4], _T("m"))) GetFormattedTraffic(tmp, 2, res);
	else
	if (!_tcscmp(ai->targv[4], _T("d"))) GetFormattedTraffic(tmp, 3, res);
	else return NULL;

	return res;
}

static TCHAR* GetTime(ARGUMENTSINFO *ai)
{
	BYTE ed, flag;
	static TCHAR result3[L2];
	DWORD Duration;

	if (ai->argc != 4) return NULL;

	// ���� ������ ���������, ����������� ������ ����������
	for (flag = ed = 0; ed < NumberOfAccounts; ed++)
	{
		TCHAR *buf;
		if (!ProtoList[ed].name) continue;
		buf = mir_a2t(ProtoList[ed].name);
		if (!_tcscmp(buf, ai->targv[1]))
		{
			flag = 0xAA;
			if (!_tcscmp(ai->targv[2], _T("now")))
				Duration = ProtoList[ed].Session.Timer;
			else if (!_tcscmp(ai->targv[2], _T("total")))
				Duration = ProtoList[ed].Total.Timer;
			else flag = 0;
			break;
		}
		mir_free(buf);
	}
	if ( (flag != 0xAA) && !_tcscmp(ai->targv[1], _T("summary")) )
	{
		flag = 0xAA;
		if (!_tcscmp(ai->targv[2], _T("now")))
			Duration = OverallInfo.Session.Timer;
		else if (!_tcscmp(ai->targv[2], _T("total")))
			Duration = OverallInfo.Total.Timer;
		else flag = 0;
	}
	
	if (flag != 0xAA) return NULL;
	GetDurationFormatM(Duration, ai->targv[3], result3, 255);
	return result3;
}

void RegisterVariablesTokens(void)
{
	TOKENREGISTER trs;
	
	if (!ServiceExists(MS_VARS_REGISTERTOKEN)) return;
		
	ZeroMemory(&trs, sizeof(trs));
	trs.cbSize = sizeof(TOKENREGISTER);

	// �������, ������������ ������
	trs.tszTokenString = _T("tc_GetTraffic");
	trs.parseFunctionT = GetTraffic;
	trs.szHelpText = "Traffic counter\t(A,B,C,D)\tGet traffic counter value. A: <ProtocolName> OR overall OR summary; B: now OR total; C: sent OR recieved OR both; D: b - in bytes, k - in kilobytes, m - in megabytes, d - dynamic";
	trs.flags = TRF_TCHAR | TRF_PARSEFUNC | TRF_FUNCTION;
	CallService(MS_VARS_REGISTERTOKEN, 0, (LPARAM)&trs);
	// �������, ������������ �����
	trs.tszTokenString = _T("tc_GetTime");
	trs.parseFunctionT = GetTime;
	trs.szHelpText = "Traffic counter\t(A,B,C)\tGet time counter value. A: <ProtocolName> OR summary; B: now OR total; C: format";
	CallService(MS_VARS_REGISTERTOKEN, 0, (LPARAM)&trs);
}
