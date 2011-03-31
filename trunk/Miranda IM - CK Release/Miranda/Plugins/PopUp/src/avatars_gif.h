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

File name      : $HeadURL: http://svn.miranda.im/mainrepo/popup/trunk/src/avatars_gif.h $
Revision       : $Revision: 1610 $
Last change on : $Date: 2010-06-23 01:55:13 +0400 (Ср, 23 июн 2010) $
Last change by : $Author: Merlin_de $

===============================================================================
*/

#ifndef __avatars_gif_h__
#define __avatars_gif_h__

class GifAvatar: public PopupAvatar
{
protected:
	avatarCacheEntry *av;
	int cachedWidth, cachedHeight;
	int activeFrame;

	HBITMAP hBitmap;
	int *frameDelays;
	int frameCount;
	SIZE frameSize;

public:
	GifAvatar(HANDLE hContact);
	virtual ~GifAvatar();
	virtual int activeFrameDelay();
	virtual void draw(MyBitmap *bmp, int x, int y, int w, int h, POPUPOPTIONS *options);
};

#endif // __avatars_gif_h__
