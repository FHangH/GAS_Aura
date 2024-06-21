@echo off
setlocal enabledelayedexpansion

:ask
:: Prompt the user to continue or not
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
echo .Pulling from GitHub...
git pull git@github.com:FHangH/GAS_Aura.git master

:: 拉取完成
echo .Pulling completed.
pause

:end
:: 结束
endlocal