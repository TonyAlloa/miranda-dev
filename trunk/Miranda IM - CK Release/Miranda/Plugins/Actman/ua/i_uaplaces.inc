{}

const
  MenuServices:array [tMenuType] of pAnsiChar = (
    'CList/AddMainMenuItem'   {MS_CLIST_ADDMAINMENUITEM   },
    'CList/AddContactMenuItem'{MS_CLIST_ADDCONTACTMENUITEM},
    'CList/AddTrayMenuItem'   {MS_CLIST_ADDTRAYMENUITEM   },
    'CList/AddProtoMenuItem'  {MS_CLIST_ADDPROTOMENUITEM  },
    'CList/AddStatusMenuItem' {MS_CLIST_ADDSTATUSMENUITEM }
  );
type
  tuaMenuRec = record
    hMenuRoot:HMENU;
    position :integer;
  end;
  tuaMenuRecA = array [tMenuType] of tuaMenuRec;

var
  arMenuRec: array of tuaMenuRecA;

//===== Support =====

function ServiceCallWithLParam(wParam:WPARAM; lParam:LPARAM):int_ptr; cdecl;
begin
  result:=CallService(MS_ACT_RUNBYID, lParam, wParam);
end;

procedure SetMTBState(var ActionItem:tMyActionItem);
var
  lflag:integer;
begin
  if ActionItem.hMTBButton=0 then exit;
  if (ActionItem.flags and UAF_2STATE)=0 then exit;

  lflag:=CallService(MS_TB_GETBUTTONSTATE,ActionItem.hMTBButton,0);
  if lflag=TBST_PUSHED then
  begin
    if (ActionItem.flags and (UAF_2STATE+UAF_PRESSED))<>(UAF_2STATE+UAF_PRESSED) then exit;
    lflag:=TBST_RELEASED;
  end
  else
  begin
    if (ActionItem.flags and (UAF_2STATE+UAF_PRESSED))=(UAF_2STATE+UAF_PRESSED) then exit;
    if (ActionItem.flags and UAF_PRESSED)=0 then exit;
    lflag:=TBST_PUSHED;
  end;
  CallService(MS_TB_SETBUTTONSTATE,ActionItem.hMTBButton,lflag);
end;

function IsLocalItem(const UAItem:tMyActionItem):boolean;
begin
  result:=(UAItem.UAMenuItem[main_menu  ].hMenuItem=0) and
          (UAItem.UAMenuItem[tray_menu  ].hMenuItem=0) and
          (UAItem.UAMenuItem[proto_menu ].hMenuItem=0) and
          (UAItem.UAMenuItem[status_menu].hMenuItem=0) and
          (UAItem.hMTBButton=0);
end;

function ServiceCallWithFParam(wParam:WPARAM; lParam:LPARAM; fParam:LPARAM):int_ptr; cdecl;
var
//  local:boolean;
  i:integer;
  setting:array [0..63] of AnsiChar;
  p:pAnsiChar;
begin
  for i:=0 to HIGH(UActionList) do
  begin
    with UActionList[i] do
      if dwActID=cardinal(fParam) then
      begin
        flags:=flags xor UAF_PRESSED;
        // sync buttons/menus
        if IsLocalItem(UActionList[i]) then
        begin
        end
        else
        begin
          // save "pressed" state
          if (flags and UAF_SAVESTATE)<>0 then
          begin
            p:=GetUABranch(setting,dwActID);
            if p<>nil then
            begin
              p:=StrCopyE(p,opt_UA);
              p^:='/'; inc(p);
              StrCopy(p,opt_Flags);
              DBWriteDWord(0,DBBranch,setting,flags and not UAF_REALTIME);
            end;
          end;

          if hMTBButton<>0 then
            SetMTBState(UActionList[i]);

          if (flags and UAF_REGTABB)<>0 then;
        end;

        break;
      end;
  end;

  result:=CallService(MS_ACT_RUNBYID, fParam, wParam);
end;

function AddIcolibIconP(var ActionItem:tMyActionItem):THANDLE;
var
  sid:TSKINICONDESC;
  buf,buf1:array [0..63] of WideChar;
begin
  if (ActionItem.hIcolibIconPressed=0) or
     (ActionItem.hIcolibIconPressed=ActionItem.hIcolibIcon) then
  begin
    // add icon for action to icolib
    fillChar(sid,SizeOf(sid),0);
    sid.cbSize         :=sizeof(sid);
    sid.szSection    .w:=ICOLIB_ACTSECTION;
    sid.szDefaultFile.w:=szMyPath;
    sid.iDefaultIndex  :=-IDI_ACTION;
    sid.cx             :=16;
    sid.cy             :=16;
    sid.flags          :=SIDF_ALL_UNICODE;
    // icon "off"
    StrCopyW(StrCopyEW(buf,ActionItem.szActDescr),' (pressed)');
    sid.szDescription.w:=@buf;
    StrCopy(StrCopyE(@buf1,ActionItem.szNameID),'_pressed');
    sid.pszName        :=@buf1;
    ActionItem.hIcolibIconPressed:=CallService(MS_SKIN2_ADDICON,0,LPARAM(@sid));
  end;
  result:=ActionItem.hIcolibIconPressed;
end;

function AddIcolibIcon(var ActionItem:tMyActionItem):THANDLE;
var
  sid:TSKINICONDESC;
begin
  if ActionItem.hIcolibIcon=0 then
  begin
    // add icon for action to icolib
    fillChar(sid,SizeOf(sid),0);
    sid.cbSize         :=sizeof(sid);
    sid.szSection    .w:=ICOLIB_ACTSECTION;
    sid.szDefaultFile.w:=szMyPath;
    sid.iDefaultIndex  :=-IDI_ACTION;
    sid.cx             :=16;
    sid.cy             :=16;
    sid.flags          :=SIDF_ALL_UNICODE;
    // icon "on"
    sid.szDescription.w:=ActionItem.szActDescr;
    sid.pszName        :=ActionItem.szNameID;
    ActionItem.hIcolibIcon:=CallService(MS_SKIN2_ADDICON,0,LPARAM(@sid));
  end;
  result:=ActionItem.hIcolibIcon;
end;

procedure DeleteIcolibIconP(var ActionItem:tMyActionItem);
var
  buf1:array [0..63] of WideChar;
begin
  if (ActionItem.hIcolibIconPressed<>0) and
     (ActionItem.hIcolibIconPressed<>ActionItem.hIcolibIcon) then
  begin
    StrCopy(StrCopyE(@buf1,ActionItem.szNameID),'_pressed');
    CallService(MS_SKIN2_REMOVEICON,0,LPARAM(@buf1));
    ActionItem.hIcolibIconPressed:=ActionItem.hIcolibIcon;
  end;
end;

procedure DeleteIcolibIcon(var ActionItem:tMyActionItem);
begin
  DeleteIcolibIconP(ActionItem);
  CallService(MS_SKIN2_REMOVEICON,0,LPARAM(ActionItem.szNameID));
  ActionItem.hIcolibIcon       :=0;
  ActionItem.hIcolibIconPressed:=0;
end;

//===== Really places =====

//----- Hotkeys -----

function AddCoreHotkey(var ActionItem:tMyActionItem):boolean;
var
  hkd:THOTKEYDESC;
begin
  if (ActionItem.flags and UAF_HKREGGED)=0 then
  begin
    FillChar(hkd,SizeOf(hkd),0);
    hkd.cbSize          := SizeOf(hkd); // HOTKEYDESC_SIZE_V1 for pre-0.9
    hkd.dwFlags         := HKD_UNICODE; // since 0.9 only
    hkd.pszName         := ActionItem.szNameID;
    hkd.pszDescription.w:= ActionItem.szActDescr;
    hkd.pszSection    .w:= MODULE_NAME;
    hkd.pszService      := SERVICE_WITH_LPARAM_NAME;
    hkd.lParam          := ActionItem.dwActID;
    result:=CallService(MS_HOTKEY_REGISTER,0,LPARAM(@hkd))<>0;
    if result then
      ActionItem.flags:=ActionItem.flags or UAF_HKREGGED;
  end
  else
    result:=true; //!!
end;

procedure DeleteCoreHotkey(var ActionItem:tMyActionItem);
begin
  if // bCoreHotkeyPresents and
     // (ServiceExists(MS_HOTKEY_UNREGISTER)<>0) and
     ((ActionItem.flags and UAF_HKREGGED)<>0) then
  begin
    CallService(MS_HOTKEY_UNREGISTER,0,LParam(ActionItem.szNameID));
    ActionItem.flags:=ActionItem.flags and not UAF_HKREGGED;
  end;
end;

//----- Common menu functions -----

function AddRootMenuIcon(szPopupName:pWideChar):THANDLE;
var
  sid:TSKINICONDESC;
begin
  FillChar(sid,SizeOf(sid),0);
  //first - register icon for root popup
  sid.cbSize         := sizeof(sid);
  sid.szSection.w    := ICOLIB_MNUSECTION;
  sid.flags          := SIDF_ALL_UNICODE;
  sid.cx             := 16;
  sid.cy             := 16;
  sid.szDescription.w:= szPopupName;
  sid.szDefaultFile.w:= szMyPath;
  sid.iDefaultIndex  := -IDI_ACTION;
  WideToAnsi(szPopupName,sid.pszName);
  result:=CallService(MS_SKIN2_ADDICON,0,LPARAM(@sid));
  mFreeMem(sid.pszName);
end;

procedure DeleteMenuItem(var ActionItem:tMyActionItem;mtype:tMenuType);
var
  i:integer;
  hMenuRoot:THANDLE;
  p:pMyActionItem;
begin
  with ActionItem.UAMenuItem[mtype] do
  begin
    if hMenuItem=0 then exit;
    CallService(MO_REMOVEMENUITEM,hMenuItem,0);
    hMenuItem:=0;
  end;

  hMenuRoot:=ActionItem.UAMenuItem[mtype].hMenuRoot;
  if hMenuRoot<>0 then
  begin
    for i:=0 to HIGH(UActionList) do
    begin
      p:=@UActionList[i];
      // presents somethere else
      if (p<>@ActionItem) and (p.UAMenuItem[mtype].hMenuRoot=hMenuRoot) then
        exit;
    end;
    // menu array cleanup now?
    for i:=0 to HIGH(arMenuRec) do
    begin
      if arMenuRec[i][mtype].hMenuRoot=hMenuRoot then
      begin
        FillChar(arMenuRec[i][mtype],SizeOf(tuaMenuRec),0);
//        arMenuRec[i][mtype].hMenuRoot:=0;
        break;
      end;
    end;
    CallService(MO_REMOVEMENUITEM,hMenuRoot,0);
    ActionItem.UAMenuItem[mtype].hMenuRoot:=0;
  end;
end;

function GetMenuPosition(hMenu:HMENU;mtype:tMenuType;toset:boolean):integer;
var
  i:integer;
begin
  result:=0;
  for i:=0 to HIGH(arMenuRec) do
  begin
    if arMenuRec[i][mtype].hMenuRoot=hMenu then
    begin
      if toset then
        inc(arMenuRec[i][mtype].position,100000);
      result:=arMenuRec[i][mtype].position;
      break;
    end;
  end;
end;

procedure CreateMenuItem(var ActionItem:tMyActionItem;mtype:tMenuType);
var
  i:integer;
  ActItem:pMyActionItem;
  ActMItem,UAMenuItem:pUAMenuItem;
  clmi:TCListMenuItem;
  res:boolean;
  extra:pWideChar;
begin
{}
  UAMenuItem:=@ActionItem.UAMenuItem[mtype];
  if UAMenuItem.hMenuItem<>0 then exit;

  // create popup menu
{}{}
  res:=true;
  if (UAMenuItem.szMenuPopup<>nil) and (UAMenuItem.szMenuPopup^<>#0) then
  begin
    res:=false;
    for i:=0 to HIGH(UActionList) do
    begin
      // try to find root popup with same name (if we already created one)
      ActItem :=@UActionList[i];
      ActMItem:=@ActItem.UAMenuItem[mtype];

      if (ActMItem.szMenuPopup<>nil) and
         (ActMItem.hMenuRoot<>0) and
        ( (ActItem<>@ActionItem) and
          (StrCmpW(ActMItem.szMenuPopup,UAMenuItem.szMenuPopup)=0) ) then
      begin
        UAMenuItem.hMenuRoot:=ActMItem.hMenuRoot;
        res:=true;
        break;
      end;
    end;
  end;
  // popup menu not found
  if not res then
  begin
    FillChar(clmi,SizeOf(clmi),0);
    clmi.cbSize:=SizeOf(clmi);
    clmi.flags :=CMIF_UNICODE or CMIF_ICONFROMICOLIB;

    if (UAMenuItem.szMenuPopup<>nil) and (UAMenuItem.szMenuPopup^<>#0) then
      clmi.szName.w:=ParseVarString(UAMenuItem.szMenuPopup)
    else
      clmi.szName.w:=ActionItem.szActDescr;

    clmi.hIcon   :=AddRootMenuIcon(clmi.szName.w);
    clmi.position:=ActionItem.wSortIndex*10;

    // position in Root Menu
    inc(clmi.position,GetMenuPosition(0,mtype,
        (UAMenuItem.menu_opt and UAF_MENUSEP)<>0));

    UAMenuItem.hMenuRoot:=CallService(MenuServices[mtype],0,LPARAM(@clmi));
    if clmi.szName.w<>ActionItem.szActDescr then
      mFreeMem(clmi.szName.w);

    for i:=1 to HIGH(arMenuRec) do
    begin
      with arMenuRec[i][mtype] do
        if hMenuRoot=0 then
        begin
//          MenuName :=ActionItem.szActDescr;
          hMenuRoot:=UAMenuItem.hMenuRoot;
          break;
        end;
    end;

  end;
{}{}

 // Now Menu Item preparing
{}{}
  FillChar(clmi,SizeOf(clmi),0);
  clmi.cbSize:=SizeOf(clmi);
  clmi.flags:=CMIF_UNICODE or CMIF_ICONFROMICOLIB;
  if (ActionItem.flags and (UAF_2STATE+UAF_PRESSED))<>(UAF_2STATE+UAF_PRESSED) then
  begin
    clmi.hIcon:=ActionItem.hIcolibIcon;
    extra:='0';
  end
  else
  begin
    clmi.hIcon:=ActionItem.hIcolibIconPressed;
    clmi.flags:=CMIF_UNICODE or CMIF_ICONFROMICOLIB or CMIF_CHECKED;
    extra:='1';
  end;

  with ActionItem.UAMenuItem[mtype] do
  begin
    if (szMenuNameVars<>nil) and (szMenuNameVars^<>#0) then
      clmi.szName.w:=ParseVarString(szMenuNameVars,0,extra)
    else
      clmi.szName.w:=ActionItem.szActDescr;

    if hMenuRoot<>0 then
    begin
      clmi.flags:=clmi.flags or CMIF_ROOTHANDLE;
      clmi.szPopupName.w:=pWideChar(hMenuRoot);
    end;
  end;

  clmi.pszService:=ActionItem.szNameID;
  if ActionItem.hMenuService=0 then
    ActionItem.hMenuService:=PluginLink.CreateServiceFunctionParam(
      clmi.pszService,@ServiceCallWithFParam,ActionItem.dwActID);

  clmi.position:=ActionItem.wSortIndex*10;
{}{}
  inc(clmi.position,GetMenuPosition(UAMenuItem.hMenuRoot,mtype,
      (UAMenuItem.menu_opt and UAF_MENUSEP)<>0));

  UAMenuItem.hMenuItem:=CallService(MenuServices[mtype],0,LPARAM(@clmi));
  if clmi.szName.w<>ActionItem.szActDescr then
    mFreeMem(clmi.szName.w);
{}

end;

function PreBuildMenu(mtype:tMenuType;hContact:THANDLE=0):int;
var
  i:integer;
  mi:TCListMenuItem;
  p,extra:pWideChar;
begin
  result:=0;

  FillChar(mi,SizeOf(mi),0);
  mi.cbSize:=SizeOf(mi);

  for i:=0 to HIGH(UActionList) do
  begin
    mi.flags:=CMIM_FLAGS;
    p:=nil;
    with UActionList[i] do
    begin
      with UAMenuItem[mtype] do
      begin
        if hMenuItem<>0 then // it means, we process that item here
        begin
          mi.szName.w:=nil;
          // Show / hide
          if isVarsInstalled then
          begin
            if (szMenuShowWhenVars<>nil) and (szMenuShowWhenVars^<>#0) then
            begin
              p:=ParseVarString(szMenuShowWhenVars,hContact);
              if p<>nil then
              begin
                if StrCmpW(p,'1')<>0 then
                  mi.flags:=CMIM_FLAGS or CMIF_HIDDEN;
                mFreeMem(p);
              end;
            end;
          end;

          // change if need to show only
          // (popup can be used by many items, keep unchanged)
          if (mi.flags and CMIF_HIDDEN)=0 then
          begin
{
        local:=(UAMenuItem[main_menu].hMenuItem=0) and
               (UAMenuItem[tray_menu].hMenuItem=0) and
               (hMTBButton=0);
if not local then UAF_PRESSED analized
else...
}
            //!!!! icon (check for contact menu)
            mi.flags:=mi.flags or CMIM_ICON or CMIM_FLAGS or CMIF_ICONFROMICOLIB;
            if (flags and (UAF_2STATE+UAF_PRESSED))=(UAF_2STATE+UAF_PRESSED) then
            begin
              mi.flags:=mi.flags or CMIF_CHECKED;
              mi.hIcon:=hIcolibIconPressed;
              extra:='1';
            end
            else
            begin
              mi.hIcon:=hIcolibIcon;
              extra:='0';
            end;

            // new name
            mi.flags:=mi.flags or CMIM_NAME or CMIF_UNICODE;
            if (szMenuNameVars<>nil) and (szMenuNameVars^<>#0) then
              mi.szName.w:=ParseVarString(szMenuNameVars,hContact,extra);

            if mi.szName.w=nil then
              mi.szName.w:=szActDescr;
          end;

          CallService(MS_CLIST_MODIFYMENUITEM,hMenuItem,LPARAM(@mi));
          if mi.szName.w<>szActDescr then
            mFreeMem(mi.szName.w);
        end;
      end;
    end;

  end;
end;

function PreBuildMainMenu(wParam:WPARAM;lParam:LPARAM):int; cdecl;
begin
  result:=PreBuildMenu(main_menu,wParam);
end;

function PreBuildContactMenu(wParam:WPARAM;lParam:LPARAM):int;  cdecl;
begin
  result:=PreBuildMenu(contact_menu,wParam);
end;

function PreBuildTrayMenu(wParam:WPARAM;lParam:LPARAM):int; cdecl;
begin
  result:=PreBuildMenu(tray_menu,wParam);
end;

//----- Modern Toolbar -----

procedure AddMTBButton(var ActionItem:tMyActionItem);
var
  mtButton:TBButton;
  pc,pc1,pc2:pAnsiChar;
begin
  if not NamesArray[uaMTB].enable then exit;

  if ActionItem.hMTBButton=0 then
  begin
    FillChar(mtButton,SizeOf(mtButton),0);
    mtButton.cbSize  :=SizeOf(mtButton);
    mtButton.defPos  :=1000;

    mtButton.pszButtonID   :=ActionItem.szNameID;
    mtButton.pszServiceName:=MTB_SERVICE_NAME;//SERVICE_WITH_LPARAM_NAME;
    mtButton.lParam        :=ActionItem.dwActID;

    mtButton.hPrimaryIconHandle  :=ActionItem.hIcolibIcon;
    mtButton.hSecondaryIconHandle:=ActionItem.hIcolibIconPressed;

    WideToAnsi(ActionItem.szActDescr,pc);

    if (ActionItem.flags and UAF_2STATE)<>0 then
      mtButton.tbbFlags:=TBBF_VISIBLE or TBBF_SHOWTOOLTIP or TBBF_PUSHED
    else
      mtButton.tbbFlags:=TBBF_VISIBLE or TBBF_SHOWTOOLTIP;

    if ActionItem.szMTBTooltip =nil then
      pc1:=pc
    else pc1:=ActionItem.szMTBTooltip;

    if ((ActionItem.flags and UAF_2STATE)=0) or 
       (ActionItem.szMTBTooltipPressed=nil) then
      pc2:=pc1
    else
      pc2:=ActionItem.szMTBTooltipPressed;

    mtButton.pszButtonName :=pc;
    mtButton.pszTooltipUp  :=pc1;
    mtButton.pszTooltipDn  :=pc2;

    ActionItem.hMTBButton:=CallService(MS_TB_ADDBUTTON,0,LPARAM(@mtButton));
    mFreeMem(pc);
  end;
end;

procedure DeleteMTBButton(var ActionItem:tMyActionItem);
begin
  if ActionItem.hMTBButton<>0 then
  begin
    CallService(MS_TB_REMOVEBUTTON,ActionItem.hMTBButton,0);
    ActionItem.hMTBButton:=0;
  end;
end;

function MTBServiceCall(wParam:WPARAM; lParam:LPARAM):int_ptr; cdecl;
var
  i,lflag:integer;
begin
  result:=0;
  for i:=0 to HIGH(UActionList) do
  begin
    if TLPARAM(UActionList[i].dwActID)=lParam then
    begin
      with UActionList[i] do
      begin
        if (flags and UAF_2STATE)<>0 then
        begin
          if CallService(MS_TB_GETBUTTONSTATE,hMTBButton,0)=TBST_PUSHED then
          begin
            lflag:=TBST_RELEASED;
          end
          else
          begin
            lflag:=TBST_PUSHED;
          end;
          CallService(MS_TB_SETBUTTONSTATE,hMTBButton,lflag);
        end;
      end;

      result:=ServiceCallWithFParam(0,0,lParam);
      break;
    end;
  end;
end;

function OnMTBLoaded(wParam:WPARAM;lParam:LPARAM):int; cdecl;
var
  i:integer;
begin
  result:=0;
  for i:=HIGH(UActionList) downto 0 do
  begin
    if (UActionList[i].flags and UAF_REGMTBB)<>0 then
      AddMTBButton(UActionList[i]);
  end;
end;

//----- TabSRMM Toolbar -----

const
  TABTOOLBAR_INITPOS = 350;

procedure AddTabBBButton(var ActionItem:tMyActionItem);
var
  tabb:BBButton;
begin
  if not NamesArray[uaTAB].enable then exit;

  if (ActionItem.flags and UAF_TBREGGED)=0 then
  begin
    FillChar(tabb,SizeOf(tabb),0);
    // register Tab ButtonBar button
    tabb.cbSize       :=SizeOf(tabb);
    tabb.dwButtonID   :=ActionItem.dwActID;
    tabb.pszModuleName:=MODULE_NAME;
    tabb.dwDefPos     :=(TABTOOLBAR_INITPOS+ActionItem.wSortIndex*10) and $7FFF;
    tabb.iButtonWidth :=0;
    tabb.hIcon        :=ActionItem.hIcolibIcon;
    if (ActionItem.flags and UAF_2STATE)<>0 then
      tabb.bbbFlags:=BBBF_ISIMBUTTON or BBBF_ISLSIDEBUTTON or
                     BBBF_ISCHATBUTTON or BBBF_ISPUSHBUTTON
    else
      tabb.bbbFlags:=BBBF_ISIMBUTTON or BBBF_ISLSIDEBUTTON or
                     BBBF_ISCHATBUTTON;

    if ActionItem.szTabBTooltip<>nil then
      tabb.szTooltip.w:=ActionItem.szTabBTooltip
    else
      tabb.szTooltip.w:=ActionItem.szActDescr;

    CallService(MS_BB_ADDBUTTON,0,LPARAM(@tabb));
    ActionItem.flags:=ActionItem.flags or UAF_TBREGGED;
  end;
end;

procedure DeleteTabBBButton(var ActionItem:tMyActionItem);
var
  tabb:BBButton;
begin
  if (ActionItem.flags and UAF_TBREGGED)<>0 then
  begin
    FillChar(tabb,SizeOf(tabb),0);
    tabb.cbSize       :=SizeOf(tabb);
    tabb.dwButtonID   :=ActionItem.dwActID;
    tabb.pszModuleName:=MODULE_NAME;
    CallService(MS_BB_REMOVEBUTTON,0,LPARAM(@tabb));
    ActionItem.flags:=ActionItem.flags and not UAF_TBREGGED;
  end;
end;

procedure SetTABState(var ActionItem:tMyActionItem;state:integer);
var
  tabb:BBButton;
  pc:pWideChar;
begin
  FillChar(tabb,SizeOf(tabb),0);
  tabb.cbSize       :=SizeOf(tabb);
  tabb.dwButtonID   :=ActionItem.dwActID;
  tabb.pszModuleName:=MODULE_NAME;
  CallService(MS_BB_GETBUTTONSTATE,0,TLPARAM(@tabb));

  if (tabb.bbbFlags and BBSF_PUSHED)<>0 then
  begin
    pc:=ActionItem.szTabBTooltipPressed;
    if pc=nil then pc:=ActionItem.szTabBTooltip;
    tabb.hIcon:=ActionItem.hIcolibIconPressed;
  end
  else
  begin
    pc:=ActionItem.szTabBTooltip;
    tabb.hIcon:=ActionItem.hIcolibIcon;
  end;
  if pc=nil then pc:=ActionItem.szActDescr;
  tabb.szTooltip.w:=pc;
  tabb.bbbFlags     :=BBBF_ISIMBUTTON or BBBF_ISLSIDEBUTTON or
                      BBBF_ISCHATBUTTON or BBBF_ISPUSHBUTTON;
  tabb.dwDefPos    :=(TABTOOLBAR_INITPOS+ActionItem.wSortIndex*10) and $7FFF;
  CallService(MS_BB_MODIFYBUTTON,0,TLPARAM(@tabb));
end;

function OnTabButtonPressed(wParam:WPARAM;lParam:LPARAM):int; cdecl;
var
  cbcd:pCustomButtonClickData;
  tabb:BBButton;
  pc:pWideChar;
  i:integer;
begin
  cbcd:=pointer(lParam);
  if StrCmp(cbcd.pszModule,MODULE_NAME)<>0 then
  begin
    result:=0;
    exit;
  end;
  for i:=0 to HIGH(UActionList) do
  begin
    with UActionList[i] do
    begin
      if cbcd.dwButtonId=dwActID then
      begin
        FillChar(tabb,SizeOf(tabb),0);
        tabb.cbSize       :=SizeOf(tabb);
        tabb.dwButtonID   :=cbcd.dwButtonId;
        tabb.pszModuleName:=MODULE_NAME;
        if (flags and UAF_2STATE)<>0 then
        begin
          CallService(MS_BB_GETBUTTONSTATE,cbcd.hContact,TLPARAM(@tabb));
          if (tabb.bbbFlags and BBSF_PUSHED)<>0 then
          begin
            pc:=szTabBTooltipPressed;
            if pc=nil then pc:=szTabBTooltip;
            tabb.hIcon:=hIcolibIconPressed;
          end
          else
          begin
            pc:=szTabBTooltip;
            tabb.hIcon:=hIcolibIcon;
          end;
          if pc=nil then pc:=szActDescr;
          tabb.szTooltip.w:=pc;
          tabb.bbbFlags     :=BBBF_ISIMBUTTON or BBBF_ISLSIDEBUTTON or
                              BBBF_ISCHATBUTTON or BBBF_ISPUSHBUTTON;
        end
        else
        begin
          tabb.hIcon:=hIcolibIcon;
          tabb.szTooltip.w:=szTabBTooltip;
          if tabb.szTooltip.w=nil then tabb.szTooltip.w:=szActDescr;
          tabb.bbbFlags     :=BBBF_ISIMBUTTON or BBBF_ISLSIDEBUTTON or
                              BBBF_ISCHATBUTTON;
        end;
        tabb.iButtonWidth:=0;
        tabb.dwDefPos    :=(TABTOOLBAR_INITPOS+wSortIndex*10) and $7FFF;
        CallService(MS_BB_MODIFYBUTTON,0,TLPARAM(@tabb));

        break;
      end;
    end;
  end;

  ServiceCallWithFParam(cbcd.hContact,0,cbcd.dwButtonId);

  result:=1;
end;

function OnTabBBLoaded(wParam:WPARAM;lParam:LPARAM):int; cdecl;
var
  i:integer;
begin
  result:=0;
  for i:=HIGH(UActionList) downto 0 do
  begin
    if (UActionList[i].flags and UAF_REGTABB)<>0 then
      AddTabBBButton(UActionList[i]);
  end;
end;