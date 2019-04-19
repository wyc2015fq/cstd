# 对CreateEvent创建事件的再认识 - xqhrs232的专栏 - CSDN博客
2014年01月09日 10:34:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：849
今天发生一件很奇怪的现象，原来是对CreateEvent函数的认识不准造成的！明明事件有信号状态还没置，但莫名等待的线程/循环等到了事件有信号，
原来是上一个循环的事件有信号状态没被线程响应而一直保持着有信号状态直到下一个循环！！！
CreateEvent的手册是这样介绍的
The CreateEvent function creates a named or unnamed event object. 
HANDLE CreateEvent(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
// pointer to security attributes  
    BOOL bManualReset,// flag for manual-reset event 
    BOOL bInitialState,// flag for initial state 
    LPCTSTR lpName // pointer to event-object name  
   );
Parameters
lpEventAttributes
Pointer to a SECURITY_ATTRIBUTES structure that determines whether the returned handle can be inherited by child processes. If lpEventAttributes is NULL, the handle cannot be inherited. 
Windows NT: The lpSecurityDescriptor member of the structure specifies a security descriptor for the new event. If lpEventAttributes is NULL, the event gets a default security descriptor. 
Windows 95: The lpSecurityDescriptor member of the structure is ignored.
bManualReset
Specifies whether a manual-reset or auto-reset event object is created. If TRUE, then you must use the ResetEvent function to manually reset the state to nonsignaled. If FALSE, Windows automatically resets the state to nonsignaled after a single waiting thread
 has been released. 
bInitialState
Specifies the initial state of the event object. If TRUE, the initial state is signaled; otherwise, it is nonsignaled. 
lpName
Points to a null-terminated string specifying the name of the event object. The name is limited to MAX_PATH characters and can contain any character except the backslash path-separator character (\). Name comparison is case sensitive. 
If lpName matches the name of an existing named event object, this function requests EVENT_ALL_ACCESS access to the existing object. In this case, the bManualReset and bInitialState parameters are ignored because they have already been set by the creating process.
 If the lpEventAttributes parameter is not NULL, it determines whether the handle can be inherited, but its security-descriptor member is ignored. 
If lpName is NULL, the event object is created without a name. 
If lpName matches the name of an existing semaphore, mutex, or file-mapping object, the function fails and the GetLastError function returns ERROR_INVALID_HANDLE. This occurs because event, mutex, semaphore, and file-mapping objects share the same name space. 
Return Values
If the function succeeds, the return value is a handle to the event object. If the named event object existed before the function call, the GetLastError function returns ERROR_ALREADY_EXISTS. Otherwise, GetLastError returns zero. 
If the function fails, the return value is NULL. To get extended error information, call GetLastError. 
//==================================================================================================================
备注::
其中bManualReset的解释如下
Specifies whether a manual-reset or auto-reset event object is created. If TRUE, then you must use the ResetEvent function to manually reset the state to nonsignaled. If FALSE, Windows automatically resets the state to nonsignaled after a single waiting thread
 has been released. 
今天就是创建的一个自动复位的事件，在线程已经退出的情况还是会被下一个循环所响应。因为自动复位事件有信号时没被线程响应就还是一直有信号状态，这样就会触发下一次线程的响应，从而产生了莫名的怪现象！！！
