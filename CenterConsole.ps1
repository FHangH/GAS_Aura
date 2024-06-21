# 添加类型定义以便调用Win32 API
Add-Type @"
using System;
using System.Runtime.InteropServices;
public class User32 {
    [DllImport("user32.dll", SetLastError = true)]
    public static extern IntPtr GetForegroundWindow();

    [DllImport("user32.dll", SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    public static extern bool SetWindowPos(IntPtr hWnd, IntPtr hWndInsertAfter, int X, int Y, int cx, int cy, uint uFlags);

    public const int SWP_NOSIZE = 0x0001;
    public const int SWP_NOZORDER = 0x0004;

    [DllImport("user32.dll")]
    public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);

    public struct RECT
    {
        public int Left;
        public int Top;
        public int Right;
        public int Bottom;
    }
}
"@

# 获取当前活动窗口句柄
$hwnd = [User32]::GetForegroundWindow()

# 获取窗口位置和大小
$rect = New-Object User32+RECT
[User32]::GetWindowRect($hwnd, [ref]$rect)

# 获取窗口的宽度和高度
$windowWidth = $rect.Right - $rect.Left
$windowHeight = $rect.Bottom - $rect.Top

# 获取屏幕分辨率
Add-Type -AssemblyName System.Windows.Forms
$screen = [System.Windows.Forms.Screen]::FromHandle($hwnd)
$screenWidth = $screen.Bounds.Width
$screenHeight = $screen.Bounds.Height

# 计算中心位置
$newX = ($screenWidth / 2) - ($windowWidth / 2)
$newY = ($screenHeight / 2) - ($windowHeight / 2)

# 移动窗口到屏幕中心
[User32]::SetWindowPos($hwnd, [IntPtr]::Zero, $newX, $newY, 0, 0, [User32]::SWP_NOSIZE -bor [User32]::SWP_NOZORDER)