/* 
ListeningTo plugin for Miranda IM
==========================================================================
Copyright	(C) 2005-2011 Ricardo Pescuma Domenecci
			(C) 2010-2011 Merlin_de
==========================================================================

in case you accept the pre-condition,
this is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#ifndef __OPTIONS_H__
# define __OPTIONS_H__


#include "commons.h"

#include <windows.h>


#define POPUP_ACTION_DONOTHING 0
#define POPUP_ACTION_CLOSEPOPUP 1
#define POPUP_ACTION_OPENHISTORY 2

#define POPUP_DELAY_DEFAULT 0
#define POPUP_DELAY_CUSTOM 1
#define POPUP_DELAY_PERMANENT 2

#define SET_XSTATUS 0
#define CHECK_XSTATUS 1
#define CHECK_XSTATUS_MUSIC 2
#define IGNORE_XSTATUS 3


struct Options {
	BOOL enable_sending;
	BOOL enable_music;
	BOOL enable_radio;
	BOOL enable_video;
	BOOL enable_others;

	TCHAR templ[1024];
	TCHAR unknown[128];

	BOOL override_contact_template;
	BOOL show_adv_icon;
	int adv_icon_slot;

//	BOOL get_info_from_watrack;	 //not used
	BOOL enable_other_players;
	BOOL enable_code_injection;
	int time_to_pool;

	WORD xstatus_set;
	TCHAR xstatus_name[1024];
	TCHAR xstatus_message[1024];
	TCHAR nothing[128];
};

extern Options opts;


// Initializations needed by options
void InitOptions();

// Deinitializations needed by options
void DeInitOptions();


// Loads the options from DB
// It don't need to be called, except in some rare cases
void LoadOptions();



BOOL IsTypeEnabled(LISTENINGTOINFO *lti);


#endif // __OPTIONS_H__
