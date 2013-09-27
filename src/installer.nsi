;--------------------------------
!include "MUI2.nsh"

; The name of the installer
Name "Langunator"

; The file to write
OutFile "Langunator.exe"

; The default installation directory
InstallDir $PROGRAMFILES\Langunator

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\Langunator" "Install_Dir"

; Request application privileges for Windows Vista and above
RequestExecutionLevel admin

!define MUI_FINISHPAGE_TITLE "Done"
!define MUI_FINISHPAGE_RUN_TEXT "Run Langunator"
!define MUI_FINISHPAGE_RUN "$INSTDIR\Langunator.exe"

;--------------------------------

; Pages

;Page components
!insertmacro MUI_PAGE_DIRECTORY      ;Page directory
!insertmacro MUI_PAGE_INSTFILES      ;Page instfiles
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM      ;UninstPage uninstConfirm
!insertmacro MUI_UNPAGE_INSTFILES    ;UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Langunator (required)"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "$%QTDIR%\bin\icudt51.dll"
  File "$%QTDIR%\bin\icuin51.dll"
  File "$%QTDIR%\bin\icuuc51.dll"
  File "release\Langunator.exe"
  File "$%QTDIR%\bin\Qt5Core.dll"
  File "$%QTDIR%\bin\Qt5Gui.dll"
  File "$%QTDIR%\bin\Qt5Multimedia.dll"
  File "$%QTDIR%\bin\Qt5MultimediaWidgets.dll"
  File "$%QTDIR%\bin\Qt5Network.dll"
  File "$%QTDIR%\bin\Qt5OpenGL.dll"
  File "$%QTDIR%\bin\Qt5PrintSupport.dll"
  File "$%QTDIR%\bin\Qt5Qml.dll"
  File "$%QTDIR%\bin\Qt5Quick.dll"
  File "$%QTDIR%\bin\Qt5Sensors.dll"
  File "$%QTDIR%\bin\Qt5Sql.dll"
  File "$%QTDIR%\bin\Qt5V8.dll"
  File "$%QTDIR%\bin\Qt5WebKit.dll"
  File "$%QTDIR%\bin\Qt5WebKitWidgets.dll"
  File "$%QTDIR%\bin\Qt5Widgets.dll"

  ; For MinGW
  File "$%QTDIR%\bin\libgcc_s_dw2-1.dll"
  File "$%QTDIR%\bin\libstdc++-6.dll"
  File "$%QTDIR%\bin\libwinpthread-1.dll"


  SetOutPath $INSTDIR\sqldrivers
  File "$%QTDIR%\plugins\sqldrivers\qsqlite.dll"

  SetOutPath $INSTDIR\platforms
  File "$%QTDIR%\plugins\platforms\qminimal.dll"
  File "$%QTDIR%\plugins\platforms\qwindows.dll"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\Langunator "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Langunator" "Langunator" "Langunator"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Langunator" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Langunator" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Langunator" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Langunator"
  CreateShortCut "$SMPROGRAMS\Langunator\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Langunator\Langunator.lnk" "$INSTDIR\Langunator.exe" "" "$INSTDIR\Langunator.exe" 0
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Langunator"
  DeleteRegKey HKLM SOFTWARE\Langunator

  ; Remove files and uninstaller
  Delete $INSTDIR\Langunator.exe
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Langunator\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\Langunator"
  RMDir "$INSTDIR"

SectionEnd
