;NSIS v3.06.1
Unicode True
!include "MUI.nsh"

!define PRODUCT_NAME "QLumEdit"
!define PRODUCT_VERSION "1.0.3"
!define PRODUCT_PUBLISHER "Krzysztof Strugiñski"
!define PRODUCT_WEB_SITE "http:\\sourceforge.net\projects\qlumedit\"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\QLumEdit.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define MODULE_REGISTER_INTERNAL_DESC "QLumEdit.Eulumdat"

!define SHCNE_ASSOCCHANGED 0x8000000
!define SHCNF_IDLIST 0

!define MUI_ABORTWARNING
!define MUI_ICON "..\..\sources\images\socket.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
;!define MUI_WELCOMEFINISHPAGE_BITMAP "images\qt-wizard.bmp"

;LicenseForceSelection checkbox

!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\..\build\x64\LICENSE.rtf"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\QLumEdit.exe"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"
!insertmacro MUI_LANGUAGE "Polish"

!insertmacro MUI_RESERVEFILE_LANGDLL
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-windows-x64-installer.exe"
InstallDir "$PROGRAMFILES64\QLumEdit"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "${PRODUCT_NAME}" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\..\build\x64\Qt6Gui.dll"
  File "..\..\build\x64\Qt6Core.dll"
  File "..\..\build\x64\Qt6Widgets.dll"  
  File "..\..\build\x64\QLumEdit.exe"
  CreateDirectory "$SMPROGRAMS\QLumEdit"
  CreateShortCut "$SMPROGRAMS\QLumEdit\QLumEdit.lnk" "$INSTDIR\QLumEdit.exe"
  CreateShortCut "$DESKTOP\QLumEdit.lnk" "$INSTDIR\QLumEdit.exe"
  File "..\..\build\x64\libgcc_s_seh-1.dll"
  File "..\..\build\x64\libstdc++-6.dll"
  File "..\..\build\x64\libwinpthread-1.dll" 
  CreateDirectory "$INSTDIR\platforms"
  SetOutPath "$INSTDIR\platforms"
  File "..\..\build\x64\platforms\qwindows.dll"   
SectionEnd

LangString DESC_FileAssociations ${LANG_ENGLISH} "Set up associations"
LangString DESC_FileAssociations ${LANG_GERMAN} "Zuordnungen einrichten"
LangString DESC_FileAssociations ${LANG_POLISH} "Ustaw skojarzenia"

SectionGroup $(DESC_FileAssociations)
Section "Eulumdat (*.ldt)" REGISTERUIEXT_SEC01

  ReadRegStr $R0 HKCR ".ldt" ""
  StrCmp $R0 "LDTFile" 0 +2
    DeleteRegKey HKCR "LDTFile"

  WriteRegStr HKCR ".ldt" "" "${MODULE_REGISTER_INTERNAL_DESC}"
  WriteRegStr HKCR "${MODULE_REGISTER_INTERNAL_DESC}" "" "Eulumdat"
  WriteRegStr HKCR "${MODULE_REGISTER_INTERNAL_DESC}\DefaultIcon" "" "$INSTDIR\QLumEdit.exe,0"
  WriteRegStr HKCR "${MODULE_REGISTER_INTERNAL_DESC}\shell" "" "open"
  WriteRegStr HKCR "${MODULE_REGISTER_INTERNAL_DESC}\shell\open\command" "" '$INSTDIR\QLumEdit.exe "%1"'

  System::Call 'Shell32::SHChangeNotify(i ${SHCNE_ASSOCCHANGED}, i ${SHCNF_IDLIST}, i 0, i 0)'

SectionEnd
SectionGroupEnd

Section -AdditionalIcons
  ;CreateShortCut "$SMPROGRAMS\QLumEdit\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\QLumEdit.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\QLumEdit.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

;Function un.onUninstSuccess
;  HideWindow
;  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
;FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
;  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
;  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\QLumEdit.exe"
  Delete "$INSTDIR\Qt6Gui.dll"
  Delete "$INSTDIR\Qt6Core.dll"
  Delete "$INSTDIR\Qt6Widgets.dll"
  Delete "$INSTDIR\libgcc_s_seh-1.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\libwinpthread-1.dll"
  Delete "$INSTDIR\platforms\qwindows.dll"

  ;Delete "$SMPROGRAMS\QLumEdit\Uninstall.lnk"
  Delete "$DESKTOP\QLumEdit.lnk"
  Delete "$SMPROGRAMS\QLumEdit\QLumEdit.lnk"

  RMDir "$SMPROGRAMS\QLumEdit"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR"

  ReadRegStr $R0 HKCR ".ldt" ""
  StrCmp $R0 "${MODULE_REGISTER_INTERNAL_DESC}" 0 +2
    DeleteRegKey HKCR ".ldt"

  DeleteRegKey HKCR "${MODULE_REGISTER_INTERNAL_DESC}"
  
  System::Call 'Shell32::SHChangeNotify(i ${SHCNE_ASSOCCHANGED}, i ${SHCNF_IDLIST}, i 0, i 0)'
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd