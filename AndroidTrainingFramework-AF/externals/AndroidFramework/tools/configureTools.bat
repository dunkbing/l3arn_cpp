@echo off
rem **************************************************************************
rem **************************************************************************
rem ************ DO NOT EDIT THIS FILE !!!!!! ********************************
rem **************************************************************************
rem **************************************************************************
rem **************************************************************************
rem USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE
rem **************************************************************************
rem Usage: configureTools.bat [platform] [pause at end] [force check]
rem [platform]      = can be Win32, Android, iOS or combination. Ex: Win32+Android+iOS
rem [pause at end]  = use 'pause_at_end' if you want to pause at end.
rem [svn password]  = use 'forced_check' if tou want to check even if the tools was already checked and are ok.
rem **************************************************************************
rem USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE USAGE
rem **************************************************************************

 

rem ***********************************************
::setlocal ENABLEDELAYEDEXPANSION

::replace the '"' from the PATH environment variable. The '"' creates an error.
set PATH=%PATH:"=%
rem @echo PATH=%PATH%
if not defined CECHO_TOOL (    
    if not exist %~dp0\cecho\cecho.exe (
	echo.
        echo The file %~dp0cecho\cecho.exe does not exist.
	echo.
    )
    set CECHO_TOOL=1
    set "SAVE_PATH=%PATH%"
)
::endlocal


if not "%SAVE_PATH%"=="" (
    set "PATH=%~dp0\cecho\;%SAVE_PATH%;"
)


call cecho {white}{\n}
call cecho {aqua}_______________________________________________________________________________{\n}
call cecho {aqua}																				{\n}
call cecho {aqua}                 Checking tools	                     				{\n}
call cecho {aqua}_______________________________________________________________________________{\n}
call cecho {white}{\n}
call cecho {white}{\n}


set GLLEGACY_NOTEPAD=%~dp0\Notepad\notepad++.exe
set GLLEGACY_SVNCLIENT_HOME=%~dp0\svn18
set GLLEGACY_SVNCLIENT_BIN=%~dp0\svn18


rem load the tools 
call:loadTheTools

set FORCED_CHECK=forced_check
set PAUSE_AT_THE_END=%1

rem reset the errorlevel by calling ver
ver > NUL


if "%FORCED_CHECK%"=="forced_check" (
    set CHECK_THE_TOOLS=true
)

if "%CHECK_THE_TOOLS%"=="false" (
    goto already_checked
)

call cecho {teal}Checking tools...
call cecho {white} {\n}
	
call "%~dp0GLLegacyInstaller\bin\GLLegacyInstaller.exe" %~dp0GLLegacyInstaller\bin\InstallerSetup.xml --console

if errorlevel 1 (
		set HAS_ERRORS=1
        set CHECK_THE_TOOLS=true
        call cecho {teal}Start the GLLegacyInstaller to fix the missing paths for tools... {\n}
		call "%~dp0GLLegacyInstaller\bin\GLLegacyInstaller.exe" %~dp0GLLegacyInstaller\bin\InstallerSetup.xml --only-check-tools %CHECK_TOOLS_FOR_PLATFORM%
        
        rem reset the errorlevel by calling ver
        ver > NUL

        rem load the tools again
        call:loadTheTools


        call cecho {teal}Check Again all the needed tools... {\n}
        call "%~dp0GLLegacyInstaller\bin\GLLegacyInstaller.exe" %~dp0GLLegacyInstaller\bin\InstallerSetup.xml --console  %CHECK_TOOLS_FOR_PLATFORM%


        if errorlevel 1 (
			call cecho {white}{\n}
			call cecho {red}_______________________________________________________________________________{\n}
			call cecho {white}{\n}
			call cecho {red}	Please install / configure the required tools.
			call cecho {red}_______________________________________________________________________________{\n}
			call cecho {white}{\n}
			call cecho {white}{\n}
			
			set HAS_ERRORS=1
            set CHECK_THE_TOOLS=true
            goto eof
        )
) else (
    rem The tools are ok, so do not check them again!
    set CHECK_THE_TOOLS=false
    goto eof
) 

:already_checked
call cecho {green}All the tools are already checked. {\n}
goto :eof

:loadTheTools
    call cecho {green}Loading all tool variables...{\n}
    if exist "%TEMP%\GLLegacyDistccENV.bat" (
        rem WARNING: do not change GLLegacyDistccENV.bat by hand. This is generated by GLLegacyInstaller
        rem WARNING: do not not use other local files instead this!
        call "%TEMP%\GLLegacyDistccENV.bat"
    ) else (
        call cecho {yellow}WARNING: "%TEMP%\GLLegacyDistccENV.bat" does not exists! {\n}
    )

	if exist "%TEMP%\GLLegacyInstallerENV.bat" (
        rem WARNING: do not change GLLegacyInstallerENV.bat by hand. This is generated by GLLegacyInstaller!
        rem WARNING: do not not use other local files instead of this!
        call "%TEMP%\GLLegacyInstallerENV.bat"
    ) else (
        call cecho {yellow}WARNING: "%TEMP%\GLLegacyInstallerENV.bat" does not exists! {\n}
    )

goto :eof


:eof

call cecho {white}{\n}
echo Done cheching all tools.

if "%PAUSE_AT_THE_END%"=="pause_at_end" (
	pause
)

rem if "%PAUSE_CT_AT_END%"=="pause_at_end" pause
rem **************************************************************************
rem **************************************************************************
rem ************ DO NOT EDIT THIS FILE !!!!!! ********************************
rem **************************************************************************
rem **************************************************************************