/*
IEView Plugin for
Miranda IM: the free IM client for Microsoft* Windows*

Author 
			Copyright (C) 2005-2010  Piotr Piastucki

Copyright 2000-2012 Miranda IM project,
all portions of this codebase are copyrighted to the people
listed in contributors.txt.

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

File name      : $HeadURL: 
Revision       : $Revision: 
Last change on : $Date: 
Last change by : $Author:
$Id$		   : $Id$:

===============================================================================
*/

class HistoryHTMLBuilder;

#ifndef HISTORYHTMLBUILDER_INCLUDED
#define HISTORYHTMLBUILDER_INCLUDED

#include "TemplateHTMLBuilder.h"

class HistoryHTMLBuilder:public TemplateHTMLBuilder
{
protected:
	void loadMsgDlgFont(const char *, LOGFONTA * lf, COLORREF * colour, COLORREF * bkgColour);
	char *timestampToString(DWORD dwFlags, time_t check);
	DWORD startedTime;
	bool isDbEventShown(DBEVENTINFO * dbei);
	void appendEventNonTemplate(IEView *, IEVIEWEVENT *event);
	const char *getTemplateFilename(ProtocolSettings *);
	const char *getTemplateFilenameRtl(ProtocolSettings *);
	int getFlags(ProtocolSettings *);
public:
	HistoryHTMLBuilder();
	void buildHead(IEView *, IEVIEWEVENT *event);
	void appendEvent(IEView *, IEVIEWEVENT *event);
};

#endif
