/*
Weather protocol for
Miranda IM: the free IM client for Microsoft* Windows*

Authors
			Copyright (C) 2005-2011 Boris Krasnovskiy All Rights Reserved
			Copyright (C) 2002-2005 Calvin Che

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

#define IDI_ICON                        101
#define IDD_USERINFO                    201
#define IDD_EDIT                        202
#define IDI_DISABLED                    203
#define IDD_POPUP                       204
#define IDD_OPTIONS                     205
#define IDI_LOG                         206
#define IDI_UPDATE2                     208
#define IDI_READ                        209
#define IDI_UPDATE                      210
#define IDI_S                           211
#define IDI_MAP                         212
#define IDR_PMENU                       213
#define IDI_POPUP                       214
#define IDI_NOPOPUP                     215
#define IDD_TEXTOPT                     216
#define IDD_BRIEF                       217
#define IDD_SETUP                       218
#define IDR_TMENU                       219
#define IDR_TMMENU                      220
#define IDI_EDIT                        222
#define IDD_INFO                        224
#define IDD_SEARCHCITY                  225
#define IDC_NAME                        2000
#define IDC_ID                          2001
#define IDC_LOG                         2003
#define IDC_UPDATETIME                  2005
#define IDC_CTEXT                       2006
#define IDC_AVATARSIZE                  2006
#define IDC_UPDATE                      2007
#define IDC_BTITLE                      2008
#define IDC_STARTUPUPD                  2008
#define IDC_CHANGE                      2009
#define IDC_BTITLE2                     2009
#define IDC_USEWINCOLORS                2010
#define IDC_BTEXT                       2011
#define IDC_CH                          2013
#define IDC_NTEXT                       2015
#define IDC_DEGREE                      2016
#define IDC_E                           2017
#define IDC_W                           2018
#define IDC_POP1                        2019
#define IDC_XTEXT                       2020
#define IDC_POP2                        2020
#define IDC_PText                       2021
#define IDC_PTitle                      2023
#define IDC_Internal                    2024
#define IDC_ETEXT                       2025
#define IDC_DISCONDICON                 2025
#define IDC_External                    2026
#define IDC_DONOTAPPUNITS               2026
#define IDC_DEFA                        2027
#define IDC_NOFRAC                      2027
#define IDC_HTEXT                       2028
#define IDC_DPop                        2029
#define IDC_DAutoUpdate                 2030
#define IDC_NEWWIN                      2031
#define IDC_IURL                        2032
#define IDC_MURL                        2033
#define IDC_PROTOCOND                   2034
#define IDC_Overwrite                   2035
#define IDC_UPDCONDCHG                  2036
#define IDC_REMOVEOLD                   2037
#define IDC_MAKEI                       2039
#define IDC_BGCOLOUR                    2040
#define IDC_TEXTCOLOUR                  2041
#define IDC_LeftClick                   2042
#define IDC_PREVIEW                     2043
#define IDC_VAR3                        2044
#define IDC_RightClick                  2045
#define IDC_DELAY                       2046
#define IDC_PDEF                        2047
#define IDC_T1                          2048
#define IDC_T2                          2049
#define IDC_W1                          2050
#define IDC_W2                          2051
#define IDC_W3                          2052
#define IDC_W4                          2053
#define IDC_BROWSE                      2054
#define IDC_VIEW1                       2055
#define IDC_RESET1                      2056
#define IDC_VIEW2                       2057
#define IDC_V1                          2058
#define IDC_V2                          2059
#define IDC_RESET2                      2060
#define IDC_SVCINFO                     2061
#define IDC_GETNAME                     2062
#define IDC_P1                          2063
#define IDC_P2                          2064
#define IDC_P3                          2065
#define IDC_P4                          2066
#define IDC_RESET                       2067
#define IDC_D1                          2067
#define IDC_D2                          2068
#define IDC_D3                          2069
#define IDC_INFO1                       2069
#define IDC_INFOICON                    2070
#define IDC_INFO11                      2071
#define IDC_INFO2                       2072
#define IDC_INFO3                       2073
#define IDC_VARLIST                     2074
#define IDC_INFO4                       2075
#define IDC_INFO5                       2076
#define IDC_PD1                         2077
#define IDC_PD2                         2078
#define IDC_PD3                         2079
#define IDC_INFO6                       2079
#define IDC_TM1                         2080
#define IDC_TM2                         2081
#define IDC_TM3                         2082
#define IDC_TM4                         2083
#define IDC_TM5                         2084
#define IDC_TM6                         2085
#define IDC_TM7                         2086
#define IDC_TM8                         2087
#define IDC_INFO7                       2087
#define IDC_TM9                         2088
#define IDC_INFO8                       2089
#define IDC_INFO9                       2090
#define IDC_INFO10                      2091
#define IDC_INFO12                      2092
#define IDC_INFO13                      2093
#define IDC_MORE                        2094
#define IDC_MOREDETAIL                  2095
#define IDC_DATALIST                    2096
#define IDC_MUPDATE                     2097
#define IDC_MFRAME                      2099
#define IDC_MTOGGLE                     2101
#define IDC_MWEBPAGE                    2102
#define IDC_MTEXT                       2103
#define IDC_STEP1                       2107
#define IDC_STEP2                       2108
#define IDC_STEP3                       2109
#define IDC_STEP4                       2110
#define IDC_INFOLIST                    2117
#define IDC_RELOADINI                   2118
#define IDC_MEMUSED                     2119
#define IDC_INICOUNT                    2120
#define IDC_AVATARSPIN                  2124
#define IDC_SEARCHCITY                  2125
#define IDC_HEADERBAR                   2126
#define IDC_E1                          2128
#define IDC_E2                          2129
#define OIC_HAND                        32513
#define OIC_QUES                        32514
#define OIC_BANG                        32515
#define OIC_NOTE                        32516
#define IDM_M1                          40002
#define IDM_M2                          40003
#define IDM_M3                          40004
#define IDM_M4                          40005
#define IDM_M5                          40006
#define IDM_M6                          40007
#define IDM_M7                          40008
#define IDM_M8                          40009
#define ID_T1                           40010
#define ID_T2                           40011
#define ID_MPREVIEW                     40020
#define ID_MRESET                       40021
#define IDC_STATIC                      -1

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NO_MFC                     1
#define _APS_NEXT_RESOURCE_VALUE        226
#define _APS_NEXT_COMMAND_VALUE         40030
#define _APS_NEXT_CONTROL_VALUE         2128
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
