@echo off
setlocal enabledelayedexpansion

REM 启动PowerShell脚本来居中控制台窗口，并重定向输出到nul以隐藏输出
powershell -ExecutionPolicy Bypass -File "%~dp0CenterConsole.ps1" >nul 2>&1

:ask
:: 提示用户是否继续
set /p continue="Do you want to continue (y/n)? "

if /i "%continue%"=="y" (
    goto pull
) else if /i "%continue%"=="n" (
    goto end
) else (
    echo Invalid input. Please enter 'y' or 'n'.
    goto ask
)

:pull
echo Pulling from GitHub...
git pull git@github.com:FHangH/GAS_Aura.git master

:: 拉取完成
echo Pulling completed.
pause

:end
:: 结束
endlocal