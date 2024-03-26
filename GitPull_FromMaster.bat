@echo off
setlocal enabledelayedexpansion

echo .Pulling from GitHub...
git pull git@github.com:FHangH/GAS_Aura.git master

:: 拉取完成
echo .Pulling completed.

pause

:: 结束
endlocal