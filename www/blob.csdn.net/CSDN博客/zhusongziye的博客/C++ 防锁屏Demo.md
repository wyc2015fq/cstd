# C++ 防锁屏Demo - zhusongziye的博客 - CSDN博客





2018年03月26日 21:23:13[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：719








     一般公司里面电脑都有锁屏程序，只要不操作电脑一段时间，就会自动锁屏。有段时间经常需要走开一小会，电脑就老是锁屏，每次都要输入密码，感觉比较麻烦，就想写一个防锁屏的程序。

自动锁屏程序原理应该是检测系统在规定时间内是否接收到消息，若未收到消息就自动锁屏。开始时用PostMessage试了一下，果然不行，看来锁屏程序检测的应该系统消息，而不是应用程序消息，所以只要间隔地给发送系统消息就可以防止自动锁屏了。上网搜索了一下相关API，找到一个SendInput函数，模拟键盘事件的，看了下函数说明就可以编写Demo了。





**[csharp]**[view plain](https://blog.csdn.net/LIFEXX/article/details/8594993#)[copy](https://blog.csdn.net/LIFEXX/article/details/8594993#)



- #include "stdafx.h"
- #include <Windows.h>
- #include <process.h>
- #include <iostream>
- #include <stdlib.h>
- 
- usingnamespace std;  
- unsigned __stdcall InkeyBoardThread(void *param);  
- 
- int main(int argc, char* argv[])  
- {  
-     TCHAR consoleName[12] = __TEXT("MyHookTest");  
-     SetConsoleTitle(__TEXT("MyHookTest"));  
- 
-     HANDLE hThread = (HANDLE)_beginthreadex(NULL, NULL, InkeyBoardThread, (void*)consoleName, NULL, NULL);  
-     printf("Hello World!\n");  
-     CloseHandle(hThread);  
- 
- char b;  
-     cin>>b;  
- return 0;  
- }  
- 
- unsigned __stdcall InkeyBoardThread(void *param)  
- {  
-     INPUT myInput[4];  
-     myInput[0].type = INPUT_KEYBOARD;  
-     myInput[0].ki.dwFlags = 0;  
-     myInput[0].ki.wVk = VK_CAPITAL;  
- 
-     myInput[1].type = INPUT_KEYBOARD;  
-     myInput[1].ki.dwFlags = KEYEVENTF_KEYUP;  
-     myInput[1].ki.wVk = VK_CAPITAL;  
- 
-     myInput[2].type = INPUT_KEYBOARD;  
-     myInput[2].ki.dwFlags = 0;  
-     myInput[2].ki.wVk = VK_CAPITAL;  
- 
-     myInput[3].type = INPUT_KEYBOARD;  
-     myInput[3].ki.dwFlags = KEYEVENTF_KEYUP;  
-     myInput[3].ki.wVk = VK_CAPITAL;  
- 
- while (TRUE)  
-     {  
-         SendInput(1, &myInput[0], sizeof(INPUT));  
-         SendInput(1, &myInput[1], sizeof(INPUT));  
-         SendInput(1, &myInput[2], sizeof(INPUT));  
-         SendInput(1, &myInput[3], sizeof(INPUT));  
- 
-         Sleep(300000);  
-     }  
- }  





上面的Demo主要是周期性地模拟Caps Lock键盘事件，选取Caps Lock事件的原因在于该按键不会对其他应用程序产生干扰。如果使用其他键盘事件，比如说‘a’，若用户打开一个txt编辑框，那么该编辑框中会输入‘a’字符。当然我们可以用HOOK把该次按键截获，但是还要加处理函数，比较麻烦，这里就不写了。上面Demo里面模拟事件的线程想要正常退出的话不要用Sleep,用WaitForSingleObject, 开个事件或信号量就OK了，不再啰嗦了。。。



