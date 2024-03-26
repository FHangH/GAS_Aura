@echo off
setlocal enabledelayedexpansion

:: 设置控制台编码为UTF-8
chcp 65001

:: 执行git push
git push origin free

:: 等待用户按回车键
echo 提交完成
pause

:: 结束
endlocal