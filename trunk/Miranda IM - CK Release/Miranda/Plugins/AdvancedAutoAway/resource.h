/*
AdvancedAutoAway plugin for
Miranda IM: the free IM client for Microsoft* Windows*

Author 
			Copyright (C) 2003-2012 P. Boon
			Copyright (C) 2008-2012 George Hazan

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

#define IDD_OPT_KEEPSTATUS              101
#define IDD_OPT_KS_BASIC                101
#define IDD_CONFIRMDIALOG               102
#define IDD_OPT_AUTOAWAY                104
#define IDD_CMDLOPTIONS                 106
#define IDD_ADDPROFILE                  109
#define IDD_OPT_STARTUPSTATUS           110
#define IDB_TTBDOWN                     111
#define IDB_TTBUP                       112
#define IDD_PUOPT_KEEPSTATUS            113
#define IDD_OPT_AUTOAWAYMSG             114
#define IDI_TICK                        117
#define IDI_NOTICK                      118
#define IDD_OPT_GENAUTOAWAY             119
#define IDD_OPT_KS_ACTION               120
#define IDD_SETSTSMSGDIALOG             121
#define IDD_OPT_KS_TRIGGER              122
#define IDD_OPT_STATUSPROFILES          123
#define IDD_OPT_AAATABS                 126
#define IDD_OPT_TABS                    126
#define IDD_OPT_KS_ADV                  127
#define IDD_TRG_AAASTATECHANGE          128
#define IDI_TTBDOWN                     129
#define IDI_TTBUP                       130
#define IDC_PROTOCOL                    1000
#define IDC_STATUS                      1001
#define IDC_PROFILE                     1002
#define IDC_STARTUPLIST                 1008
#define IDC_STATUSMSG                   1008
#define IDC_SETSTATUSONSTARTUP          1010
#define IDC_SETPROFILE                  1010
#define IDC_SETWINDOW                   1011
#define IDC_SETWINSTATE                 1011
#define IDC_WINDOW                      1012
#define IDC_WINSTATE                    1012
#define IDC_CLOSE                       1013
#define IDC_SHOWDIALOG                  1014
#define IDC_OFFLINECLOSE                1016
#define IDC_CONFIRM                     1017
#define IDC_SHOWCMDL                    1018
#define IDC_COPY                        1019
#define IDC_CMDL                        1020
#define IDC_OK                          1021
#define IDC_SHORTCUT                    1022
#define IDC_CHECKCONNECTION             1025
#define IDC_MAXRETRIES                  1026
#define IDC_INITDELAY                   1027
#define IDC_SETSTATUSDELAY              1027
#define IDC_SETPROFILEDELAY             1027
#define IDC_DOCKED                      1028
#define IDC_SETDOCKED                   1029
#define IDC_MAXDELAY                    1031
#define IDC_INCREASEEXPONENTIAL         1032
#define IDC_LNOTHING                    1032
#define IDC_LCLOSE                      1033
#define IDC_LV1STATUS                   1035
#define IDC_LV2STATUS                   1036
#define IDC_STATUSLIST                  1037
#define IDC_LV1AFTERSTR                 1038
#define IDC_SHOWCONNECTIONPOPUPS        1040
#define IDC_ADDPROFILE                  1041
#define IDC_DELPROFILE                  1042
#define IDC_PROFILENAME                 1043
#define IDC_CANCEL                      1044
#define IDC_DEFAULTPROFILE              1045
#define IDC_MONITORMIRANDA              1045
#define IDC_WINCOLORS                   1045
#define IDC_CHKINET                     1045
#define IDC_IGNLOCK                     1045
#define IDC_CONNLOST                    1045
#define IDC_CREATETTBBUTTONS            1046
#define IDC_CREATETTB                   1046
#define IDC_PERPROTOCOLSETTINGS         1050
#define IDC_SAMESETTINGS                1051
#define IDC_RESETSTATUS                 1052
#define IDC_SETSTSMSG                   1053
#define IDC_DEFAULTCOLORS               1057
#define IDC_ONLOCK                      1057
#define IDC_CONNSUCCESS                 1057
#define IDC_LOGINERR                    1057
#define IDC_LV2ONINACTIVE               1062
#define IDC_CNCOTHERLOC                 1062
#define IDC_CONNRETRY                   1062
#define IDC_PUCONNLOST                  1062
#define IDC_DLGTIMEOUT                  1063
#define IDC_AWAYCHECKTIMEINSECS         1063
#define IDC_STSMSG                      1063
#define IDC_PINGHOST                    1063
#define IDC_CONFIRMDELAY                1064
#define IDC_PROTOCOLLIST                1066
#define IDC_LCANCEL                     1068
#define IDC_RNOTHING                    1069
#define IDC_RCLOSE                      1070
#define IDC_RCANCEL                     1071
#define IDC_DELAY                       1072
#define IDC_BGCOLOR                     1074
#define IDC_TEXTCOLOR                   1075
#define IDC_PREV                        1076
#define IDC_LOGINERR_DELAY              1077
#define IDC_PROTOLIST                   1079
#define IDC_ENABLECHECKING              1080
#define IDC_DISABLECHECKING             1081
#define IDC_IDLEWARNING                 1084
#define IDC_DESCRIPTION                 1085
#define IDC_CONNGIVEUP                  1086
#define IDC_CONTCHECK                   1087
#define IDC_BYPING                      1088
#define IDC_CHECKAPMRESUME              1089
#define IDC_PUCONNRETRY                 1090
#define IDC_PURESULT                    1091
#define IDC_PUOTHER                     1092
#define IDC_DELAYFROMPU                 1093
#define IDC_DELAYCUSTOM                 1094
#define IDC_DELAYPERMANENT              1095
#define IDC_LOGINERR_CANCEL             1096
#define IDC_LOGINERR_SETDELAY           1097
#define IDC_PUSHOWEXTRA                 1098
#define IDC_CREATEMMI                   1100
#define IDC_SETWINLOCATION              1104
#define IDC_XPOS                        1105
#define IDC_YPOS                        1106
#define IDC_SETWINSIZE                  1107
#define IDC_WIDTH                       1108
#define IDC_HEIGHT                      1109
#define IDC_OVERRIDE                    1111
#define IDC_MIRANDAMSG                  1112
#define IDC_CUSTOMMSG                   1113
#define IDC_CURWINLOC                   1114
#define IDC_CURWINSIZE                  1115
#define IDC_HOTKEY                      1116
#define IDC_REGHOTKEY                   1117
#define IDC_VARIABLESHELP               1118
#define IDC_AUTODIAL                    1120
#define IDC_AUTOHANGUP                  1121
#define IDC_MONITORKEYBOARD             1122
#define IDC_MONITORMOUSE                1123
#define IDC_IGNSYSKEYS                  1124
#define IDC_IGNALTCOMBO                 1125
#define IDC_FIRSTOFFLINE                1126
#define IDC_INSUBMENU                   1127
#define IDC_MAXCONNECTINGTIME           1128
#define IDC_TABS                        1130
#define IDC_NOLOCKED                    1131
#define IDC_PINGCOUNT                   1132
#define IDC_CNTDELAY                    1133
#define IDC_ENTERFIRST                  1134
#define IDC_ENTERSECOND                 1135
#define IDC_LEAVEFIRST                  1136
#define IDC_LEAVESECOND                 1137
#define IDC_BECOMEACTIVE                1138
#define IDC_OTHERLOC                    1139
#define IDC_LOGINERROR                  1140
#define IDC_SCREENSAVE                  1145
#define IDC_TIMED                       1146
#define IDC_AWAYTIME                    1147
#define IDC_SETNA                       1148
#define IDC_NATIME                      1149
#define IDC_RADUSEMIRANDA               1210
#define IDC_RADUSECUSTOM                1212
#define IDC_SETNASTR                    1568

// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        129
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1141
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
