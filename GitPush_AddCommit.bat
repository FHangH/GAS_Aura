@echo off
setlocal enabledelayedexpansion

REM 启动PowerShell脚本来居中控制台窗口，并重定向输出到nul以隐藏输出
powershell -ExecutionPolicy Bypass -File "%~dp0CenterConsole.ps1" >nul 2>&1

:: 设置控制台编码为UTF-8
chcp 65001

:: 执行git add .
echo 正在添加所有更改...
git add .
echo 添加完成!

:: 等待用户输入内容
echo 请输入提交消息: 
set /p message=

:: 获取所有分支
echo 正在获取所有分支...
for /f "tokens=*" %%a in ('git branch') do (
    set "branches=!branches! %%a"
)

:: 显示所有分支
echo 可用分支:
echo %branches%

:: 等待用户输入分支名
echo 请输入要提交的分支名 (按回车使用默认分支 master):
set /p branch=

:: 如果用户没有输入分支名，则使用默认分支 master
if "%branch%" == "" (
    set branch=master
)

:: 执行git commit -m "用户输入的内容"
echo 正在提交更改...
git commit -m "%message%"

:: 执行git push
echo 正在推送更改到%branch%分支...
git push origin %branch%

:: 等待用户按回车键
echo 提交完成--按回车键退出...
pause

:: 结束
endlocal