/*
Extraicons service plugin for
Miranda IM: the free IM client for Microsoft* Windows*

Author
			 Copyright (C) 2009-2012 Ricardo Pescuma Domenecci

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

#include "commons.h"

ExtraIcon::ExtraIcon(const char *name) :
	name(name), slot(-1), position(1000)
{
}

ExtraIcon::~ExtraIcon()
{
}

const char *ExtraIcon::getName() const
{
	return name.c_str();
}

int ExtraIcon::getSlot() const
{
	return slot;
}

void ExtraIcon::setSlot(int slot)
{
	this->slot = slot;
}

int ExtraIcon::getPosition() const
{
	return position;
}

void ExtraIcon::setPosition(int position)
{
	this->position = position;
}

bool ExtraIcon::isEnabled() const
{
	return slot >= 0;
}

void ExtraIcon::applyIcons()
{
	if (!isEnabled())
		return;

	HANDLE hContact = (HANDLE) CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);
	while (hContact != NULL)
	{
		// Clear to assert that it will be cleared
		Clist_SetExtraIcon(hContact, slot, NULL);

		applyIcon(hContact);

		hContact = (HANDLE) CallService(MS_DB_CONTACT_FINDNEXT, (WPARAM) hContact, 0);
	}
}

int ExtraIcon::compare(const ExtraIcon *other) const
{
	if (this == other)
		return 0;

	int ret = getPosition() - other->getPosition();
	if (ret != 0)
		return ret;

	int id = 0;
	if (getType() != EXTRAICON_TYPE_GROUP)
		id = ((BaseExtraIcon*) this)->getID();
	int otherId = 0;
	if (other->getType() != EXTRAICON_TYPE_GROUP)
		otherId = ((BaseExtraIcon*) other)->getID();
	return id - otherId;
}

bool ExtraIcon::operator==(const ExtraIcon & other) const
{
	int c = compare(&other);
	return c == 0;
}

bool ExtraIcon::operator!=(const ExtraIcon & other) const
{
	int c = compare(&other);
	return c != 0;
}

bool ExtraIcon::operator<(const ExtraIcon & other) const
{
	int c = compare(&other);
	return c < 0;
}

bool ExtraIcon::operator<=(const ExtraIcon & other) const
{
	int c = compare(&other);
	return c <= 0;
}

bool ExtraIcon::operator>(const ExtraIcon & other) const
{
	int c = compare(&other);
	return c > 0;
}

bool ExtraIcon::operator>=(const ExtraIcon & other) const
{
	int c = compare(&other);
	return c >= 0;
}
