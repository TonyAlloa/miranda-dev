/*

Facebook plugin for Miranda Instant Messenger
_____________________________________________

Copyright � 2009-11 Michal Zelinka

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

File name      : $HeadURL: http://eternityplugins.googlecode.com/svn/trunk/facebook/avatars.cpp $
Revision       : $Revision: 91 $
Last change by : $Author: n3weRm0re.ewer $
Last change on : $Date: 2011-01-08 11:10:34 +0100 (so, 08 1 2011) $

*/

#include "common.h"

void FacebookProto::UpdateAvatarWorker(void *p)
{
	if(p == NULL)
		return;

	std::auto_ptr<update_avatar> data( static_cast<update_avatar*>(p) );
	DBVARIANT dbv;

	if (data->url.empty())
		return;
	
	if( !DBGetContactSettingString(data->hContact,m_szModuleName,FACEBOOK_KEY_ID,&dbv) )
	{
		std::string new_url = data->url;
		std::string ext = new_url.substr(new_url.rfind('.'));

		std::string::size_type pos = new_url.rfind( "_q." );
		if (pos != std::string::npos)
			new_url = new_url.replace( pos, 3, "_s." );

		std::string filename = GetAvatarFolder() + '\\' + dbv.pszVal + ext;
		DBFreeVariant(&dbv);

		PROTO_AVATAR_INFORMATION ai = {sizeof(ai)};
		ai.hContact = data->hContact;
		ai.format = ext_to_format(ext);
		strncpy(ai.filename,filename.c_str(),MAX_PATH);

		ScopedLock s( avatar_lock_ );
		LOG("***** Updating avatar: %s",data->url.c_str());
		if (CallService(MS_SYSTEM_TERMINATED,0,0))
		{
			LOG("***** Terminating avatar update early: %s",data->url.c_str());
		} else {
			if(facy.save_url(new_url,filename))
			{
				LOG("***** Done avatar: %s to %s",data->url.c_str(), filename.c_str());
				if (data->hContact != NULL)
					ProtoBroadcastAck(m_szModuleName,data->hContact,ACKTYPE_AVATAR, ACKRESULT_SUCCESS,&ai,0);
				else
					CallService(MS_AV_REPORTMYAVATARCHANGED, (WPARAM)this->m_szModuleName, 0);
			}
			else
			{
				LOG("***** Failed avatar: %s to %s",data->url.c_str(), filename.c_str());
				if (data->hContact != NULL)
					ProtoBroadcastAck(m_szModuleName,data->hContact,ACKTYPE_AVATAR, ACKRESULT_FAILED, &ai,0);
			}		
		}
	}
}

std::string FacebookProto::GetAvatarFolder()
{
	char path[MAX_PATH];
	if ( hAvatarFolder_ && FoldersGetCustomPath(hAvatarFolder_,path,sizeof(path), "") == 0 )
		return path;
	else
		return def_avatar_folder_;
}

int FacebookProto::GetAvatarCaps(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int FacebookProto::GetAvatarInfo(WPARAM wParam, LPARAM lParam)
{
	if (!lParam)
		return -3;

	PROTO_AVATAR_INFORMATION* AI = ( PROTO_AVATAR_INFORMATION* )lParam;
	DBVARIANT dbv;

	if ( !DBGetContactSettingString( AI->hContact,m_szModuleName,FACEBOOK_KEY_AV_URL,&dbv ) )
	{
		if ( strlen( dbv.pszVal ) == 0 )
			return GAIR_NOAVATAR;

		std::string avatar_url = dbv.pszVal;
		DBFreeVariant(&dbv);

		if ( !DBGetContactSettingString( AI->hContact,m_szModuleName,FACEBOOK_KEY_ID,&dbv ) )
		{
			std::string ext = avatar_url.substr(avatar_url.rfind('.'));
			std::string file_name = GetAvatarFolder() + '\\' + dbv.pszVal + ext;
			DBFreeVariant(&dbv);

			if (!_access((char*)file_name.c_str(), 0))
			{
				LOG("***** Giving AvatarInfo: %s",file_name.c_str());
				AI->format = ext_to_format(ext);
				strncpy((char*)AI->filename, file_name.c_str(), (int)AI->cbSize);
				return GAIR_SUCCESS;
			}
		}
	}
	return GAIR_NOAVATAR;
}

int FacebookProto::GetMyAvatar(WPARAM wParam, LPARAM lParam)
{
	if (!wParam)
		return -3;

	char* buf = ( char* )wParam;
	int  size = ( int )lParam;

	DBVARIANT dbv;
	std::string avatar_url;

	if ( !getString( FACEBOOK_KEY_AV_URL,&dbv ) )
	{
		if ( strlen( dbv.pszVal ) == 0 )
			return -2; // No avatar set

		std::string avatar_url = dbv.pszVal;
		DBFreeVariant(&dbv);

		if ( !getString( FACEBOOK_KEY_ID,&dbv ) )
		{
			std::string ext = avatar_url.substr(avatar_url.rfind('.'));
			std::string file_name = GetAvatarFolder() + '\\' + dbv.pszVal + ext;
			DBFreeVariant(&dbv);

			if (!_access((char*)file_name.c_str(), 0))
			{
				LOG("***** Giving MyAvatar: %s",file_name.c_str());
				strncpy((char*)wParam, file_name.c_str(), (int)lParam);
				return 0; // Avatar file exists
			}
			
			return -1; // Avatar file doesn't exist
		}
	}
	return -2; // No avatar set
}

bool FacebookProto::AvatarExists(facebook_user* fbu)
{
	if (!fbu->image_url.empty())
	{
		std::string ext = fbu->image_url.substr(fbu->image_url.rfind('.'));
		std::string file_name = GetAvatarFolder();
		file_name += '\\' + fbu->user_id + ext;

		if (!_access(file_name.c_str(), 0))
			return true;
	}
	return false;
}