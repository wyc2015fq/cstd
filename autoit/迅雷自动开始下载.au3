
;#include <MsgBoxConstants.au3>
;#include <WindowsConstants.au3>
#include <WinAPI.au3>

AutoItSetOption("SendKeyDelay", 40)

For $count = 1 To 1000
	Local $hWnd = _WinGetHandleByPnmAndCls("Thunder.exe", "XLUEFrameHostWnd")

	If Not $hWnd Then
		MsgBox("asdf", "fff", "窗口没找到")
		Exit
	EndIf

	WinActivate($hWnd)

	MouseClick("left", 2395, 1426, 1)
	Sleep(500)
	Send("{F8}")
	MouseClick("left", 2395, 1426, 1)
	Sleep(500)
	Send("{F8}")
	ConsoleWrite("$count=" & $count & @CRLF)
	Sleep(1000000)
	;MouseClick("left", 2002, 624, 1)
Next


; 根据pname和class获取窗口句柄，找不到则返回0
Func _WinGetHandleByPnmAndCls($pname, $class)
	; 根据进程名查找进程id
	Local $pid = ProcessExists($pname)
	; 如果进程存在，继续
	If $pid Then
		Return _WinGetHandleByPidAndCls($pid, $class)
	Else
		Return 0
	EndIf
EndFunc   ;==>_WinGetHandleByPnmAndCls

; 根据pid和class获取窗口句柄，找不到则返回0
Func _WinGetHandleByPidAndCls($pid, $class)
	; 这里使用枚举所有顶层窗口方法，WinList方法会返回大量隐藏窗口
	Local $winArr = _WinAPI_EnumWindowsTop()
	; 遍历所有窗口,进程id与指定进程id比较
	For $i = 1 To $winArr[0][0]
		If $pid = WinGetProcess($winArr[$i][0]) And $winArr[$i][1] = $class Then
			; 一个进程会有多个窗口，所以要用class来筛选
			Return $winArr[$i][0]
		EndIf
	Next
	Return 0
EndFunc   ;==>_WinGetHandleByPidAndCls

