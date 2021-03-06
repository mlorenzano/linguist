; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Elco Linguist"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Elco Elettronica"
#define MyAppURL "http://www.elcoelettronica.it/en/home/"
#define MyAppExeName "ElcoLinguist.exe"

; Percorso d'installazione delle librerie Qt.
#define QtSetupDir          "C:\Qt\5.9.5\"

; Percorso dell'eseguibile del compilatore.
#define MSVCBinPath        "C:\Qt\5.9.5\msvc2017_64\"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppID={{8BA80FA7-E854-4F95-AE21-FB1B5FFDAE2E}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf64}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=installer
OutputBaseFilename=ElcoLinguist-{#MyAppVersion}
Compression=lzma/Max
CompressionThreads=auto
SolidCompression=true
PrivilegesRequired=none
VersionInfoCompany=Elco Elettronica S.N.C.
VersionInfoProductName=Elco Linguist
DisableWelcomePage=no
DisableDirPage=no
DisableProgramGroupPage=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[InstallDelete]
Type: files; Name: "{app}\*.dll"
Type: files; Name: "{app}\*.qm"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}";
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; OnlyBelowVersion: 0,6.1

[Dirs]
Name: "{app}\platforms"

[Files]
; DLL
Source: {#MSVCBinPath}\bin\Qt5Core.dll; DestDir: {app}; Flags: replacesameversion;
Source: {#MSVCBinPath}\bin\Qt5Gui.dll; DestDir: {app}; Flags: replacesameversion;
Source: {#MSVCBinPath}\bin\Qt5Widgets.dll; DestDir: {app}; Flags: replacesameversion;
Source: {#MSVCBinPath}\plugins\platforms\*; DestDir: {app}\platforms; Flags: replacesameversion;

; DLL interne
Source: libs-3rdparty\libCSV\libCSV.dll; DestDir: {app}; Flags: replacesameversion;
Source: libs-3rdparty\libXLNT\xlnt.dll; DestDir: {app}; Flags: replacesameversion;

Source: release\{#MyAppExeName}; DestDir: {app}; Flags: replacesameversion;

;Translations 
Source: translations\*.qm; DestDir: {app}\translations\; Flags: replacesameversion;

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}";
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; WorkingDir: "{app}";

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, "&", "&&")}}"; Flags: nowait postinstall skipifsilent