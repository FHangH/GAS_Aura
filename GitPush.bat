@echo off
setlocal enabledelayedexpansion

:: 设置控制台编码为UTF-8
chcp 65001

:: 等待用户输入内容
echo 请输入提交消息: 
set /p message=

:: 执行git add .
git add .

:: 执行git commit -m "用户输入的内容"
git commit -m "%message%"

:: 执行git push
git push origin master

:: 等待用户按回车键
pause
echo 提交完成

:: 结束
endlocal