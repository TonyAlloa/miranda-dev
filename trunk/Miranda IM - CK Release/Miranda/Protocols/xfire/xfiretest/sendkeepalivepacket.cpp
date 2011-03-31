/*
 *  xfirelib - C++ Library for the xfire protocol.
 *  Copyright (C) 2006 by
 *          Beat Wolf <asraniel@fryx.ch> / http://gfire.sf.net
 *          Herbert Poul <herbert.poul@gmail.com> / http://goim.us
 *    http://xfirelib.sphene.net
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "stdafx.h"

#include "sendkeepalivepacket.h"
#include "variablevalue.h"
#include <string.h>

namespace xfirelib {

  int SendKeepAlivePacket::getPacketContent(char *buf) {
    int index = 0;

    VariableValue val;
    val.setName("value");

    index += val.writeName(buf,index);
    buf[index++] = 2;
    buf[index++] = 0;
    buf[index++] = 0;
    buf[index++] = 0;
    buf[index++] = 0;

    val.setName("stats");

    index += val.writeName(buf,index);
    buf[index++] = 4;
    buf[index++] = 2;
    buf[index++] = 0;
    buf[index++] = 0;

    return index;
  }


};
