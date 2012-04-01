/*
Popup Plus plugin for Miranda IM

Copyright	� 2002 Luca Santarelli,
			� 2004-2007 Victor Pavlychko
			� 2010 MPK
			� 2010 Merlin_de

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

===============================================================================

File name      : $HeadURL$
Revision       : $Revision$
Last change on : $Date$
Last change by : $Author$

===============================================================================
*/

#ifndef __avatars_simple_h__
#define __avatars_simple_h__

class SimpleAvatar: public PopupAvatar
{
private:
	avatarCacheEntry *av;
	bool avNeedFree;

public:
	SimpleAvatar(HANDLE hContact, bool bUseBitmap = false);
	virtual ~SimpleAvatar();
	virtual int activeFrameDelay();
	virtual void draw(MyBitmap *bmp, int x, int y, int w, int h, POPUPOPTIONS *options);
};

#endif // __avatars_simple_h__
