// ---------------------------------------------------------------------------80
//                ICQ plugin for Miranda Instant Messenger
//                ________________________________________
//
// Copyright � 2000,2001 Richard Hughes, Roland Rabien, Tristan Van de Vreede
// Copyright � 2001,2002 Jon Keating, Richard Hughes
// Copyright � 2002,2003,2004 Martin �berg, Sam Kothari, Robert Rainwater
// Copyright � 2004,2005,2006,2007 Joe Kucera
// Copyright � 2006,2007 [sss], chaos.persei, [sin], Faith Healer, Theif, nullbie
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// -----------------------------------------------------------------------------
//
// File name      : $Source$
// Revision       : $Revision: 36 $
// Last change on : $Date: 2007-08-05 03:45:10 +0300 (Вс, 05 авг 2007) $
// Last change by : $Author: sss123next $
//
// DESCRIPTION:
//
//  Describe me here please...
//
// -----------------------------------------------------------------------------

#include "icqoscar.h"



extern HANDLE hServerConn;


void handlePingChannel(unsigned char* buf, WORD datalen)
{
    NetLog_Server("Warning: Ignoring server packet on PING channel");
}



static unsigned __stdcall icq_keepAliveThread(void* arg)
{
    serverthread_info* info = (serverthread_info*)arg;
    icq_packet packet;
    DWORD dwInterval = getSettingDword(NULL, "KeepAliveInterval", KEEPALIVE_INTERVAL);

    NetLog_Server("Keep alive thread starting.");

    info->hKeepAliveEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    for(;;)
    {

        DWORD dwWait;
        if(info)
            dwWait = WaitForSingleObjectEx(info->hKeepAliveEvent, dwInterval, TRUE);
        else
            break;

        if (dwWait == WAIT_OBJECT_0) break; // we should end
        else if (dwWait == WAIT_TIMEOUT)
        {
            // Send a keep alive packet to server
            packet.wLen = 0;
            write_flap(&packet, ICQ_PING_CHAN);
            if (hServerConn) // connection lost, end
                sendServPacket(&packet);
            else
                break;
        }
        else if (dwWait == WAIT_IO_COMPLETION)
            // Possible shutdown in progress
            if (Miranda_Terminated()) break;
    }

    NetLog_Server("Keep alive thread shutting down.");

    CloseHandle(info->hKeepAliveEvent);
    info->hKeepAliveEvent = NULL;
    return 0;
}



void StartKeepAlive(serverthread_info* info)
{
    if (info->hKeepAliveEvent) // start only once
        return;

    if (getSettingByte(NULL, "KeepAlive", 0))
    {
        info->hKeepAliveThread = ICQCreateThreadEx(icq_keepAliveThread, info, NULL);
    }
}



void StopKeepAlive(serverthread_info* info)
{
    // finish keep alive thread
    if (info->hKeepAliveEvent)
    {
        SetEvent(info->hKeepAliveEvent);

        // wait for the thread to finish
        WaitForSingleObjectEx(info->hKeepAliveThread, INFINITE, FALSE);
        CloseHandle(info->hKeepAliveThread);
        info->hKeepAliveThread = NULL;
    }
}
