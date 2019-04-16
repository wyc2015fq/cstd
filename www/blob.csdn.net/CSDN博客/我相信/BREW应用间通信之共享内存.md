# BREW应用间通信之共享内存 - 我相信...... - CSDN博客





2011年03月17日 13:49:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：890








实际上，事件传递本质上就是一种共享内存的通信方式。ICLIPBOARD可以用于实现 AEE 层中的剪切和粘贴功能，是共享内存通信的另一种重要形式。由于剪贴板数据在应用程序结束后仍可能存在，因而此函数会分配新内存并将内容复制到新的缓冲区，也就是说ICLIPBOARD_Set() 分配的内存在系统内存里，所以可以被其他的应用所使用。如果传递指针为 NULL 或大小为 0 的参数，将会释放现有的剪贴板数据。

目标应用希望从剪贴板获得通信内容的时候，需要使用ICLIPBOARD_EnumInit()初始化剪贴板中各格式的枚举上下文，然后调用ICLIPBOARD_GetData()将指定格式的剪贴板数据复制到目标缓冲区，如果剪贴板中的数据不是目标应用所需要的，需要调用ICLIPBOARD_EnumNext()返回下一个可用/注册的剪贴板格式进一步处理，以此类推。在目标应用处理完毕后，可以调用ICLIPBOARD_Empty()从剪贴板中删除所有格式的所有数据。如果最后一个应用程序没有调用 ICLIPBOARD_Empty 来释放剪贴板，并且 BREW 在手持设备关机后退出，则剪贴板上的所有数据均将丢失。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/09/29/5914155.aspx](http://blog.csdn.net/wireless_com/archive/2010/09/29/5914155.aspx)



