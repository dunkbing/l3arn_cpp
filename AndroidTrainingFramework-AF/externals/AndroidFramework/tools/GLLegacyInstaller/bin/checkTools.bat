@echo off
rem **************************************************************************
rem **************************************************************************
rem ************ DO NOT EDIT THIS FILE !!!!!! ********************************
rem **************************************************************************
rem **************************************************************************
@echo Executing: %~f0
rem **************************************************************************
rem USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE
rem **************************************************************************
rem Usage: checkTools.bat [platform] [pause at end] [force check]
rem [platform]      = can be Win32, Android, iOS or combination. Ex: Win32+Android+iOS
rem [pause at end]  = use 'pause_at_end' if you want to pause at end.
rem [forced_check]  = use 'forced_check' if you want to check even if the tools was already checked and are ok.
rem IMPROVEMENTS: if you want to use a specific file you can define GLLEGACY_INSTALLER_CONFIG=path_to_config
rem **************************************************************************
rem USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE
rem **************************************************************************



::replace the '"' from the PATH environment variable. The '"' creates an error.
set PATH=%PATH:"=%
if not defined CECHO_TOOL (
    if not exist %~dp0cecho\cecho.exe (
        @echo ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR
        @echo The file %~dp0cecho\cecho.exe DOES NOT EXIST!
        @echo You have to update GLLegacy to the latest revision!    
        @echo ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR
        goto eof
    )
    set CECHO_TOOL=1
    set "SAVE_PATH=%PATH%"
)


if not "%SAVE_PATH%"=="" (
    set "PATH=%~dp0cecho\;%SAVE_PATH%;"
)

call cecho {silver}------------------------------------------------------------------------------------{#}{\n}

:: load the tools 
call:loadTheTools


set PAUSE_CT_AT_END=%2
set FORCED_CHECK=%3

:: reset the errorlevel by calling ver
ver > NUL


if "%FORCED_CHECK%"=="forced_check" (
    set CHECK_THE_TOOLS=true
)

if "%CHECK_THE_TOOLS%"=="false" (
    goto already_checked
)


set CHECK_TOOLS_FOR_PLATFORM=
if not "%1"=="" (
    set CHECK_TOOLS_FOR_PLATFORM=--platforms=%1
)

if not "%CHECK_TOOLS_FOR_PLATFORM%"=="" (
    call cecho {aqua}Checking tools for %CHECK_TOOLS_FOR_PLATFORM% ... {#}{\n}
) else (
    call cecho {aqua}Checking tools ... {#}{\n}
)

if "%GLLEGACY_INSTALLER_CONFIG%"=="" (
    set GLLEGACY_INSTALLER_CONFIG=%~dp0InstallerSetup.xml
)

call "%~dp0GLLegacyInstaller.exe" %GLLEGACY_INSTALLER_CONFIG% --console %CHECK_TOOLS_FOR_PLATFORM%

if errorlevel 1 (
    set CHECK_THE_TOOLS=true
    call cecho {aqua}Start the GLLegacyInstaller to fix the missing paths for tools... {#}{\n}
    call "%~dp0GLLegacyInstaller.exe" %GLLEGACY_INSTALLER_CONFIG% --runAsAdmin --only-check-tools %CHECK_TOOLS_FOR_PLATFORM%
    
    :: reset the errorlevel by calling ver
    ver > NUL

    :: load the tools again
    call:loadTheTools


    call cecho {aqua}Check Again all the needed tools... {#}{\n}
    call "%~dp0GLLegacyInstaller.exe" %GLLEGACY_INSTALLER_CONFIG% --console  %CHECK_TOOLS_FOR_PLATFORM%


    if errorlevel 1 (
        call cecho {red}ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR {#}{\n}
        call cecho {red}ERROR: You didn't fixed the required tools!{#}{\n}
        call cecho {red}TIPS: Run again the Setup.bat and fix the required tools!{#}{\n}
        call cecho {red}ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR-ERROR{#}{\n}

        set CHECK_THE_TOOLS=true
        goto eof
    )
) else (
    :: The tools are ok, so do not check them again!
    set CHECK_THE_TOOLS=false
    goto eof
) 


:already_checked
call cecho {green}All the tools are already checked. {#}{\n}
call cecho {silver}------------------------------------------------------------------------------------{#}{\n}
goto :eof

:loadTheTools
    call cecho {green}Load all tools variables...{#}{\n}
    if exist "%TEMP%\GLLegacyDistccENV.bat" (
        :: WARNING: do not change GLLegacyDistccENV.bat by hand. This is generated by GLLegacyInstaller
        :: WARNING: do not not use other local files instead this!
        call "%TEMP%\GLLegacyDistccENV.bat"
    ) else (
        call cecho {yellow}WARNING: "%TEMP%\GLLegacyDistccENV.bat" does not exists! {#}{\n}
    )

	if exist "%TEMP%\GLLegacyInstallerENV.bat" (
        :: WARNING: do not change GLLegacyInstallerENV.bat by hand. This is generated by GLLegacyInstaller!
        :: WARNING: do not not use other local files instead this!
        call "%TEMP%\GLLegacyInstallerENV.bat"
    ) else (
        call cecho {yellow}WARNING: "%TEMP%\GLLegacyInstallerENV.bat" does not exists! {#}{\n}
    )

goto :eof



:eof

if "%PAUSE_CT_AT_END%"=="pause_at_end" pause
rem **************************************************************************
rem **************************************************************************
rem ************ DO NOT EDIT THIS FILE !!!!!! ********************************
rem **************************************************************************
rem **************************************************************************

