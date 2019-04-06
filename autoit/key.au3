
;#include <MsgBoxConstants.au3>
;#include <WindowsConstants.au3>
#include <WinAPI.au3>

Local $hWnd = _WinGetHandleByPnmAndCls("firefox.exe", "MozillaWindowClass")

If Not $hWnd Then
	MsgBox("asdf", "fff", "窗口没找到")
	Exit
EndIf

WinActivate($hWnd)
AutoItSetOption("SendKeyDelay", 40)

$file = FileOpen("list.txt", 0)

For $count = 1 To 2
	$line = FileReadLine($file)
Next

For $count = 1 To 1
	; Print the count
	$line = FileReadLine($file)
	;ConsoleWrite($line & Chr(13) )
	If @error = -1 Then ExitLoop
	$strs = StringSplit($line, " ")
	Send("^t")
	ClipPut($strs[1])
	Send("^v")
	Send("{ENTER}")
	Send("{ENTER}")
	Sleep(5000)
	Send($strs[2])
	Send("{ENTER}")
	Send("{ENTER}")
	Sleep(5000)
	MouseClick("left", 810, 261, 1)
	Sleep(1000)
	MouseClick("left", 902, 935, 1)
	MouseClick("left", 902, 835, 1)
	Sleep(3000)
	;Send("^w")
	ConsoleWrite($strs[1] & " " & $strs[2] & Chr(13))
Next
;810 261
FileClose($file)


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

