#ifndef _SYS_WIN32_SERIAL_PORT_INL_
#define _SYS_WIN32_SERIAL_PORT_INL_
#include <windows.h>

#define sys_serial_port_errmsg printf

// 枚举所有的串口，返回总串口数
int sys_enum_serial_port(dev_desc* plistComPort, int maxport)
{
  int iPortNum = 0;
  HKEY hKey;
  DWORD dwIndex = 0;
  LONG Status;
  CHAR Name[25];
  char szPortName[80];
  DWORD dwName;
  DWORD dwSizeofPortName;
  DWORD Type;
  LPCSTR data_Set = "HARDWARE\\DEVICEMAP\\SERIALCOMM";
  LONG ret0 = RegOpenKeyExA(HKEY_LOCAL_MACHINE, data_Set, 0, KEY_READ, &hKey);
  if (ret0 != ERROR_SUCCESS) {
    printf("错误：无法打开有关的hKEY");
    //char* ss = sys_errmsg(0);
    iPortNum = 0;
    return 0;
  }
  for (; iPortNum < maxport;) {
    dwName = sizeof(Name);
    dwSizeofPortName = sizeof(szPortName);
    Status = RegEnumValueA(hKey, dwIndex++, Name, &dwName, NULL, &Type,
        (UCHAR*)szPortName, &dwSizeofPortName);
    if ((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA)) {
      strncpy(plistComPort[iPortNum].name, (LPCSTR)szPortName, 32);
      iPortNum++;
    }
    if (!((Status == ERROR_SUCCESS) || (Status == ERROR_MORE_DATA))) {
      break;
    }
  }
  RegCloseKey(hKey);
  return iPortNum;
}
static int sys_win32_close(sys_file_t* f)
{
  HANDLE h = (HANDLE)f->x;
  if (h) {
    CloseHandle(h);
  }
  BZERO1(f);
  return 0;
}
int sys_open(sys_file_t* f, const char* fn, int flag)
{
  DWORD dwDesiredAccess = 0;
  DWORD dwShareMode = 0;
  DWORD dwCreationDisposition = 0;
  DWORD dwFlagsAndAttributes = 0;
#define FFLAGDEF(f2, a, b) if (flag&a) {f2|=b;}
  FFLAGDEF(dwDesiredAccess, FO_RDONLY, GENERIC_READ);
  FFLAGDEF(dwDesiredAccess, FO_WRONLY, GENERIC_WRITE);
  FFLAGDEF(dwCreationDisposition, FO_CREAT, CREATE_NEW);
  FFLAGDEF(dwCreationDisposition, FO_CREAT, CREATE_ALWAYS);
  FFLAGDEF(dwCreationDisposition, FO_EXCL, OPEN_EXISTING);
  FFLAGDEF(dwCreationDisposition, FO_EXCL, OPEN_ALWAYS);
  FFLAGDEF(dwFlagsAndAttributes, FO_NONBLOCK, FILE_FLAG_OVERLAPPED);
#undef FFLAGDEF
  f->x = CreateFileA(fn, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, 0);
  f->close = sys_win32_close;
  return 0;
}
#define MaxSerialPortNum 20 // 有效的串口总个数，不是串口的号 //add by itas109 2014-01-09
typedef struct {
  int m_nComArray[20];
  // thread
  //CWinThread* s->m_Thread;
  HANDLE m_Thread;
  BOOL m_bIsSuspened;// thread监视线程是否挂起
  // synchronisation objects
  CRITICAL_SECTION m_csCommunicationSync;// 临界资源
  BOOL m_bThreadAlive;// 监视线程运行标志
  // handles
  HANDLE m_hShutdownEvent; //stop发生的事件
  HANDLE m_hComm; // 串口句柄
  HANDLE m_hWriteEvent; // write
  // Event array.
  // One element is used for each event. There are two event handles for each s.
  // A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
  // There is a general shutdown when the s is closed.
  // 事件数组，包括一个写事件，接收事件，关闭事件
  // 一个元素用于一个事件。有两个事件线程处理端口。
  // 写事件和接收字符事件位于overlapped结构体（m_ov.hEvent）中
  // 当端口关闭时，有一个通用的关闭。
  HANDLE m_hEventArray[3];
  // structures
  OVERLAPPED m_ov;// 异步I/O
  COMMTIMEOUTS m_CommTimeouts;// 超时设置
  DCB m_dcb;// 设备控制块
  // owner window
  void* m_pOwner;
  recv_message_f recv_message;
  // misc
  UINT m_nPort; //?????
} sys_win32_serial_port_t;

// If there is a error, give the right message
// 如果有错误，给出提示
//
void ProcessErrorMessage(const char* ErrorText)
{
  char Temp[200];
  LPVOID lpMsgBuf;
  FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf, 0, NULL );
  _snprintf(Temp, countof(Temp), "WARNING: %s Failed with the following error: \n%s\n", (char*)ErrorText, lpMsgBuf);
  //MessageBox(NULL, Temp, "Application Error!", MB_ICONSTOP);
  printf("%s\n", Temp);
  LocalFree(lpMsgBuf);
}

int sys_win32_serial_port_recv(sys_serial_port_t* ss, void* data, int n)
{
  char* cdata = (char*)data;
  sys_win32_serial_port_t* s = (sys_win32_serial_port_t*)ss->x;
  BOOL bRead = TRUE;
  BOOL bResult = TRUE;
  DWORD dwError = 0;
  DWORD BytesRead = 0;
  COMSTAT comstat;
  unsigned char RXBuff;
  int len = 0;
  for (;len<n;) {
    //add by liquanhai 2011-11-06 防止死锁
    if (WaitForSingleObject(s->m_hShutdownEvent, 0) == WAIT_OBJECT_0) {
      return len;
    }
    // Gain ownership of the comm s critical section.
    // This process guarantees no other part of this program
    // is using the s object.
    EnterCriticalSection(&s->m_csCommunicationSync);
    // ClearCommError() will update the COMSTAT structure and
    // clear any other errors.
    // 更新COMSTAT
    bResult = ClearCommError(s->m_hComm, &dwError, &comstat);
    LeaveCriticalSection(&s->m_csCommunicationSync);
    // start forever loop. I use this type of loop because I
    // do not know at runtime how many loops this will have to
    // run. My solution is to start a forever loop and to
    // break out of it when I have processed all of the
    // data available. Be careful with this approach and
    // be sure your loop will exit.
    // My reasons for this are not as clear in this sample
    // as it is in my production code, but I have found this
    // solutiion to be the most efficient way to do this.
    // 所有字符均被读出，中断循环
    if (comstat.cbInQue == 0) {
      // break out when all bytes have been read
      break;
    }
    EnterCriticalSection(&s->m_csCommunicationSync);
    if (bRead) {
      // 串口读出，读出缓冲区中字节
      bResult = ReadFile(s->m_hComm, &RXBuff, 1, &BytesRead, &s->m_ov); // pointer to the s->m_ov structure
      // deal with the error code
      // 若返回错误，错误处理
      if (!bResult) {
        switch (dwError = GetLastError())
        {
        case ERROR_IO_PENDING: 
          // asynchronous i/o is still in progress
          // Proceed on to GetOverlappedResults();
          // 异步IO仍在进行
          bRead = FALSE;
          break;
        default: 
          // Another error has occured. Process this error.
          ProcessErrorMessage("ReadFile()");
          break;
          //return;// 防止读写数据时，串口非正常断开导致死循环一直执行。add by itas109 2014-01-09 与上面liquanhai添加防死锁的代码差不多
        }
      }
      else { // ReadFile返回TRUE
        // ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
        bRead = TRUE;
      }
    } // close if (bRead)
    // 异步IO操作仍在进行，需要调用GetOverlappedResult查询
    if (!bRead) {
      bRead = TRUE;
      BytesRead = 0;
      bResult = GetOverlappedResult(s->m_hComm, &s->m_ov, &BytesRead, TRUE); // Wait flag
      // deal with the error code
      if (!bResult) {
        ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
      } else if (BytesRead) {
        cdata[len++] = RXBuff;
        //if ('\n'==RXBuff) {          ASSERT(0);          return len;        }
      }
    } // close if (!bRead)
    LeaveCriticalSection(&s->m_csCommunicationSync);
  } // end forever loop
  return len;
}
#if 0
void sys_win32_serial_port_ReceiveChar(sys_serial_port_t* ss)
{
  sys_win32_serial_port_t* s = (sys_win32_serial_port_t*)ss->x;
  BOOL bResult = TRUE;
	BOOL  bRead = TRUE; 
  DWORD dwError = 0;
  COMSTAT comstat;
  char* buf = NULL;
  int pos=0, len=0;
	//unsigned char RXBuff;
  for (;;) {
    DWORD BytesRead = 0;
    //add by liquanhai 2011-11-06 防止死锁
    if (WaitForSingleObject(s->m_hShutdownEvent, 0) == WAIT_OBJECT_0) {
      break;
    }
    EnterCriticalSection(&s->m_csCommunicationSync);
    // 更新COMSTAT
    bResult = ClearCommError(s->m_hComm, &dwError, &comstat);
    LeaveCriticalSection(&s->m_csCommunicationSync);
    // 所有字符均被读出，中断循环
    if (comstat.cbInQue == 0) {
      // break out when all bytes have been read
      break;
    }
    if (bRead) {
      buf = (char*)realloc(buf, comstat.cbInQue+10);
      bResult = ReadFile(s->m_hComm, buf, 1, &BytesRead, &s->m_ov);
      
      if (!bResult)  
      {
        switch (dwError = GetLastError()) 
        { 
        case ERROR_IO_PENDING: 	
          { 
            // asynchronous i/o is still in progress 
            // Proceed on to GetOverlappedResults();
            ///异步IO仍在进行
            bRead = FALSE;
            break;
          }
        default:
          {
            // Another error has occured.  Process this error.
            ProcessErrorMessage("ReadFile()");
            break;
            //return;///防止读写数据时，串口非正常断开导致死循环一直执行。add by itas109 2014-01-09 与上面liquanhai添加防死锁的代码差不多
          } 
        }
      }
      if (!bRead)
      {
		}  // close if (!bRead)
    } // end forever loop
  }
    if (len) {
      buf[len] = 0;
      sys_serial_port_SendMessage(ss, buf, len, 0);
    }

  if (buf) free(buf);
  return ;
}
#endif
// Write a character.
int sys_win32_serial_port_write_buf(sys_win32_serial_port_t* s, const char* pWriteBuffer, DWORD SendLen)
{
  BOOL bWrite = TRUE;
  BOOL bResult = TRUE;
  DWORD BytesSent = 0;
  int pos=0;
  ResetEvent(s->m_hWriteEvent);
  // Gain ownership of the critical section
  EnterCriticalSection(&s->m_csCommunicationSync);
  while(pos<SendLen) {
    if (bWrite) {
      // Initailize variables
      s->m_ov.Offset = 0;
      s->m_ov.OffsetHigh = 0;
      // Clear buffer
      PurgeComm(s->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
      bResult = WriteFile(s->m_hComm, pWriteBuffer+pos, SendLen-pos, &BytesSent, &s->m_ov); // Overlapped structure
      // deal with any error codes
      if (!bResult) {
        DWORD dwError = GetLastError();
        switch (dwError) {
        case ERROR_IO_PENDING: {
          BytesSent = 0;
          bWrite = FALSE;
          break;
                               }
        default: {
          // all other error codes
          ProcessErrorMessage("WriteFile()");
                 }
        }
      }
      else {
        LeaveCriticalSection(&s->m_csCommunicationSync);
      }
    } // end if(bWrite)
    //printf("pos %d\n", pos);
    if (!bWrite) {
      bWrite = TRUE;
      bResult = GetOverlappedResult(s->m_hComm, &s->m_ov, &BytesSent, TRUE); // Wait flag
      LeaveCriticalSection(&s->m_csCommunicationSync);
      pos += BytesSent;
      // deal with the error code
      if (!bResult) {
        ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
      }
    } // end if (!bWrite)
    // Verify that the data size send equals what we tried to send
    if (BytesSent != SendLen /*strlen((char*)s->m_szWriteBuffer)*/) { // add by
      //TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)s->m_szWriteBuffer));
    }
  }
  printf("send %d\n", pos);
  return pos;
}
//
// The CommThread Function.
// 线程函数
// 监视线程的大致流程：
// 检查串口-->进入循环{WaitCommEvent(不阻塞询问)询问事件-->如果有事件来到-->到相应处理(关闭\读\写)}
//
DWORD WINAPI sys_win32_serial_port_CommThread(LPVOID pParam)
{
  // Cast the void pointer passed to the thread back to
  // a pointer of sys_win32_serial_port_t class
  sys_serial_port_t* ss = (sys_serial_port_t*)pParam;
  sys_win32_serial_port_t* s = (sys_win32_serial_port_t*)ss->x;
  // Set the status variable in the dialog class to
  // TRUE to indicate the thread is running.
  // TRUE表示线程正在运行
  // Misc. variables
  DWORD BytesTransfered = 0;
  DWORD Event = 0;
  DWORD CommEvent = 0;
  DWORD dwError = 0;
  COMSTAT comstat;
  BOOL bResult = TRUE;
  char data[1024];
  int maxlen = countof(data), len=0;
  s->m_bThreadAlive = TRUE;
  // Clear comm buffers at startup
  // 开始时清除串口缓冲
  if (s->m_hComm) { // check if the s is opened
    PurgeComm(s->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
  }
  // begin forever loop. This loop will run as long as the thread is alive.
  // 只要线程存在就不断读取数据
  for (;;) {
    // WaitCommEvent函数第3个参数1pOverlapped可以是一个OVERLAPPED结构的变量指针
    // ，也可以是NULL，当用NULL时，表示该函数是同步的，否则表示该函数是异步的。
    // 调用WaitCommEvent时，如果异步操作不能立即完成，会立即返回FALSE，系统在
    // WaitCommEvent返回前将OVERLAPPED结构成员hEvent设为无?藕抛刺鹊讲ㄐ?
    // 事件时，系统将其置有信号
    bResult = WaitCommEvent(s->m_hComm, &Event, &s->m_ov);// 表示该函数是异步的
    if (!bResult) {
      // If WaitCommEvent() returns FALSE, process the last error to determin
      // the reason..
      // 如果WaitCommEvent返回Error为FALSE，则查询错误信息
      switch (dwError = GetLastError()) {
      case ERROR_IO_PENDING: { // 正常情况，没有字符可读
        // This is a normal return value if there are no bytes
        // to read at the s.
        // Do nothing and continue
        break;
      }
      case 87: { // 系统错误
        // Under Windows NT, this value is returned for some reason.
        // I have not investigated why, but it is also a valid reply
        // Also do nothing and continue.
        break;
      }
      default: { // 发生其他错误，其中有串口读写中断开串口连接的错误
        // All other error codes indicate a serious error has
        // occured. Process this error.
        ProcessErrorMessage("WaitCommEvent()");
        break;
      }
      }
    }
    else { // WaitCommEvent()能正确返回
      bResult = ClearCommError(s->m_hComm, &dwError, &comstat);
      if (comstat.cbInQue == 0) {
        continue;
      }
    } // end if bResult
    // 主等待函数，会阻塞线程
    // Main wait function. This function will normally block the thread
    // until one of nine events occur that require action.
    // 等待3个事件：关断/读/写，有一个事件发生就返回
    Event = WaitForMultipleObjects(3, s->m_hEventArray, FALSE, INFINITE);// 超时时间
    switch (Event) {
    case 0: {
      // Shutdown event. This is event zero so it will be
      // the higest priority and be serviced first.
      // 关断事件，关闭串口
      CloseHandle(s->m_hComm);
      s->m_hComm = NULL;
      s->m_bThreadAlive = FALSE;
      // Kill this thread. break is not needed, but makes me feel better.
      //AfxEndThread(100);
      ExitThread(100);
      break;
    }
    case 1: { // read event 将定义的各种消息发送出去
      GetCommMask(s->m_hComm, &CommEvent);
      if (CommEvent & EV_RXCHAR) { //接收到字符，并置于输入缓冲区中
        //sys_win32_serial_port_ReceiveChar(ss);
        int len = sys_win32_serial_port_recv(ss, data, maxlen);
        if (len>0) {
          //printf("%d\n", pos);
          sys_serial_port_SendMessage(ss, data, len, 0);
        }
      }
      if (CommEvent & EV_CTS) { //CTS信号状态发生变化
        //sys_serial_port_SendMessage(s->m_pOwner, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) s->m_nPort);
      }
      if (CommEvent & EV_RXFLAG) { //接收到事件字符，并置于输入缓冲区中
        //sys_serial_port_SendMessage(s->m_pOwner, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) s->m_nPort);
      }
      if (CommEvent & EV_BREAK) { //输入中发生中断
        //sys_serial_port_SendMessage(s->m_pOwner, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM) s->m_nPort);
      }
      if (CommEvent & EV_ERR) { //发生线路状态错误，线路状态错误包括CE_FRAME,CE_OVERRUN和CE_RXPARITY
        //sys_serial_port_SendMessage(s->m_pOwner, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM) s->m_nPort);
      }
      if (CommEvent & EV_RING) { //检测到振铃指示
        //sys_serial_port_SendMessage(s->m_pOwner, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) s->m_nPort);
      }
      break;
    }
    case 2:
      while (ss->m_szWriteBufferList) { // write event 发送数据
        // Write character event from s
        sendbuf_t* sb = ss->m_szWriteBufferList;
        ss->m_szWriteBufferList = sb->next;
        //printf("%d %02x %02x %02x %02x\n", sb->len, sb->data[0], sb->data[1], sb->data[2], sb->data[3]);
        sys_win32_serial_port_write_buf(s, sb->data, sb->len);
        pfree(sb);
      }
      break;
    default: {
      sys_serial_port_errmsg("%s", "接收有问题!");
      break;
    }
    } // end switch
  } // close forever loop
  return 0;
}
// Restart the comm thread
// 复位监视线程
//
//void sys_win32_serial_port_RestartMonitoring(sys_win32_serial_port_t* s){  ResumeThread(s->m_Thread);}
int sys_win32_serial_port_close(sys_serial_port_t* ss)
{
  sys_win32_serial_port_t* s = (sys_win32_serial_port_t*)ss->x;
  //add by liquanhai 防止死锁 2011-11-06
  do {
    SetEvent(s->m_hShutdownEvent);
  }
  while (s->m_bThreadAlive);

  // Suspend the comm thread
  // 挂起监视
  //TRACE("Thread suspended\n");
  //s->m_Thread->SuspendThread();
  SuspendThread(s->m_Thread);

  // if the s is still opened: close it
  if (s->m_hComm != NULL) {
    CloseHandle(s->m_hComm);
    s->m_hComm = NULL;
  }
  // Close Handles
  if (s->m_hShutdownEvent != NULL) {
    CloseHandle(s->m_hShutdownEvent);
  }
  if (s->m_ov.hEvent != NULL) {
    CloseHandle(s->m_ov.hEvent);
  }
  if (s->m_hWriteEvent != NULL) {
    CloseHandle(s->m_hWriteEvent);
  }
  //TRACE("Thread ended\n");
  sendbuf_delall(&ss->m_szWriteBufferList);
  FREE(s);
  BZERO1(ss);
  return 1;
}
int sys_win32_serial_port_send(sys_serial_port_t* ss, const void* data, int n)
{
  sys_win32_serial_port_t* s = (sys_win32_serial_port_t*)ss->x;
  // set event for write
#if 0
  sendbuf_addbuf(&ss->m_szWriteBufferList, data, n);
  SetEvent(s->m_hWriteEvent);
#else
  n = sys_win32_serial_port_write_buf(s, (const char*)data, n);
#endif
  return n;
}
BOOL sys_win32_serial_port_RecvData(sys_serial_port_t* ss, LPTSTR lpszData, const int nSize)
{
  sys_win32_serial_port_t* s = (sys_win32_serial_port_t*)ss->x;
  //接收数据
  DWORD mylen = 0, mylen2 = 0;
  assert(s->m_hComm != 0);
  memset(lpszData, 0, nSize);
  while ((int)mylen < nSize) {
    if (!ReadFile(s->m_hComm, lpszData, nSize, &mylen2, NULL)) {
      return FALSE;
    }
    mylen += mylen2;
  }
  return TRUE;
}
//
// 查询注册表的串口号，将值存于数组中
// 本代码参考于mingojiang的获取串口逻辑名代码
void sys_win32_serial_port_QueryKey1(sys_win32_serial_port_t* s, HKEY hKey)
{
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
  // char achKey[MAX_KEY_LENGTH]; // buffer for subkey name
  // DWORD cbName; // size of name string
  char achClass[MAX_PATH] = (""); // buffer for class name
  DWORD cchClassName = MAX_PATH; // size of class string
  DWORD cSubKeys = 0; // number of subkeys
  DWORD cbMaxSubKey; // longest subkey size
  DWORD cchMaxClass; // longest class string
  DWORD cValues; // number of values for key
  DWORD cchMaxValue; // longest value name
  DWORD cbMaxValueData; // longest value data
  DWORD cbSecurityDescriptor; // size of security descriptor
  FILETIME ftLastWriteTime; // last write time
  DWORD i, retCode;
  char achValue[MAX_VALUE_NAME];
  DWORD cchValue = MAX_VALUE_NAME;
  // Get the class name and the value count.
  retCode = RegQueryInfoKeyA(
      hKey, // key handle
      achClass, // buffer for class name
      &cchClassName, // size of class string
      NULL, // reserved
      &cSubKeys, // number of subkeys
      &cbMaxSubKey, // longest subkey size
      &cchMaxClass, // longest class string
      &cValues, // number of values for this key
      &cchMaxValue, // longest value name
      &cbMaxValueData, // longest value data
      &cbSecurityDescriptor, // security descriptor
      &ftLastWriteTime); // last write time
  for (i = 0; i < 20; i++) { // 存放串口号的数组初始化
    s->m_nComArray[i] = -1;
  }
  // Enumerate the key values.
  if (cValues > 0) {
    for (i = 0, retCode = ERROR_SUCCESS; i < cValues; i++) {
      cchValue = MAX_VALUE_NAME;
      achValue[0] = '\0';
      if (ERROR_SUCCESS == RegEnumValueA(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL)) {
        char* szName = (achValue);
        if (NULL != strstr(szName, "Serial") || NULL != strstr(szName, "VCom")) {
          BYTE strDSName[10];
          DWORD nValueType = 0, nBuffLen = 10;
          memset(strDSName, 0, 10);
          if (ERROR_SUCCESS == RegQueryValueEx(hKey, (LPCTSTR)achValue, NULL, &nValueType, strDSName, &nBuffLen)) {
            int nIndex = -1;
            while (++nIndex < MaxSerialPortNum) {
              if (-1 == s->m_nComArray[nIndex]) {
                s->m_nComArray[nIndex] = atoi((char*)(strDSName + 3));
                break;
              }
            }
          }
        }
      }
    }
  }
  else {
    sys_serial_port_errmsg("本机没有串口.....");
  }
}


//
// Initialize the s. This can be s 1 to MaxSerialPortNum.
// 初始化串口。只能是1-MaxSerialPortNum
//
//parity:
// n=none
// e=even
// o=odd
// m=mark
// s=space
//data:
// 5,6,7,8
//stop:
// 1,1.5,2
int sys_serial_port_open(sys_serial_port_t* ss, UINT port, UINT baud, char parity, UINT databits, UINT stopbits)
{
  sys_win32_serial_port_t* s = NULL;
  DWORD ReadIntervalTimeout = 1000;
  DWORD ReadTotalTimeoutMultiplier = 1000;
  DWORD ReadTotalTimeoutConstant = 1000;
  DWORD WriteTotalTimeoutMultiplier = 1000;
  DWORD WriteTotalTimeoutConstant = 1000;
  BOOL bResult = FALSE;
  int mystop;
  int myparity;
  HANDLE m_hComm = NULL;

  assert(port > 0 && port < 200);

  // if the s is already opened: close it
  // 串口已打开就关掉
  if (ss->x) {
    sys_serial_port_close(ss);
  }
  // stop is index 0 = 1 1=1.5 2=2
  switch (stopbits) {
  case 0:
    mystop = ONESTOPBIT;
    break;
  case 1:
    mystop = ONE5STOPBITS;
    break;
  case 2:
    mystop = TWOSTOPBITS;
    break;
  }
  myparity = 0;
  parity = toupper(parity);
  switch (parity) {
  case 'N':
    myparity = 0;
    break;
  case 'O':
    myparity = 1;
    break;
  case 'E':
    myparity = 2;
    break;
  case 'M':
    myparity = 3;
    break;
  case 'S':
    myparity = 4;
    break;
  }
  //sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, mystop);
  // get a handle to the s
  // 通信程序在CreateFile处指定串口设备及相关的操作属性，再返回一个句柄，
  // 该句柄将被用于后续的通信操作，并贯穿整个通信过程串口打开后，其属性
  // 被设置为默认值，根据具体需要，通过调用GetCommState(hComm,&&dcb)读取
  // 当前串口设备控制块DCB设置，修改后通过SetCommState(hComm,&&dcb)将其写
  // 入。运用ReadFile()与WriteFile()这两个API函数实现串口读写操作，若为异
  // 步通信方式，两函数中最后一个参数为指向OVERLAPPED结构的非空指针，在读
  // 写函数返回值为FALSE的情况下，调用GetLastError()函数，返回值为ERROR_IO_PENDING，
  // 表明I/O操作悬挂，即操作转入后台继续执行。此时，可以用WaitForSingleObject()
  // 来等待结束信号并设置最长等待时间
  for (;port<16; ++port) {
    char szPort[50];
    // prepare s strings
    sprintf(szPort, "\\\\.\\COM%d", port);// 可以显示COM10以上端口//add by itas109 2014-01-09
    m_hComm = CreateFileA(szPort, // communication s string (COMX)
      GENERIC_READ | GENERIC_WRITE, // read/write types
      0, // comm devices must be opened with exclusive access
      NULL, // no security attributes
      OPEN_EXISTING, // comm devices must use OPEN_EXISTING
      FILE_FLAG_OVERLAPPED, // Async I/O
      0); // template must be 0 for comm devices
    // 创建失败
    if (m_hComm != INVALID_HANDLE_VALUE) {
      break;
    }
  }
  if (m_hComm == INVALID_HANDLE_VALUE) {
    // s not found
    return 0;
  }
  
  MYREALLOC(s, 1);
  BZERO1(s);
  s->m_hComm = m_hComm;
  ss->x = s;
  ss->close = sys_win32_serial_port_close;
  ss->send = sys_win32_serial_port_send;
  ss->recv = sys_win32_serial_port_recv;
  // if the thread is alive: Kill
  if (s->m_bThreadAlive) {
    do {
      SetEvent(s->m_hShutdownEvent);
    }
    while (s->m_bThreadAlive);
    //TRACE("Thread ended\n");
  }
  // create events
  if (s->m_ov.hEvent != NULL) {
    ResetEvent(s->m_ov.hEvent);
  }
  else {
    s->m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  }
  if (s->m_hWriteEvent != NULL) {
    ResetEvent(s->m_hWriteEvent);
  }
  else {
    s->m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  }
  if (s->m_hShutdownEvent != NULL) {
    ResetEvent(s->m_hShutdownEvent);
  }
  else {
    s->m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  }
  // initialize the event objects
  // 事件数组初始化，设定优先级别
  s->m_hEventArray[0] = s->m_hShutdownEvent; // highest priority
  s->m_hEventArray[1] = s->m_ov.hEvent;
  s->m_hEventArray[2] = s->m_hWriteEvent;
  // initialize critical section
  // 初始化临界资源
  InitializeCriticalSection(&s->m_csCommunicationSync);
  // 多个线程操作相同的数据时，一般是需要按顺序访问的，否则会引导数据错乱，
  // 无法控制数据，变成随机变量。为解决这个问题，就需要引入互斥变量，让每
  // 个线程都按顺序地访问变量。这样就需要使用EnterCriticalSection和
  // LeaveCriticalSection函数。
  // now it critical!
  EnterCriticalSection(&s->m_csCommunicationSync);
  // set buffersize for writing and save the owner
  s->m_nPort = port;
  // set the timeout values
  // 设置超时
  s->m_CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout * 1000;
  s->m_CommTimeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier * 1000;
  s->m_CommTimeouts.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant * 1000;
  s->m_CommTimeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier * 1000;
  s->m_CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant * 1000;
  // configure
  // 配置
  // 分别调用Windows API设置串口参数
  if (SetCommTimeouts(s->m_hComm, &s->m_CommTimeouts)) { // 设置超时
    // 若对端口数据的响应时间要求较严格，可采用事件驱动方式。
    // 事件驱动方式通过设置事件通知，当所希望的事件发生时，Windows
    // 发出该事件已发生的通知，这与DOS环境下的中断方式很相似。Windows
    // 定义了9种串口通信事件，较常用的有以下三种：
    // EV_RXCHAR:接收到一个字节，并放入输入缓冲区；
    // EV_TXEMPTY:输出缓冲区中的最后一个字符，发送出去；
    // EV_RXFLAG:接收到事件字符(DCB结构中EvtChar成员)，放入输入缓冲区
    // 在用SetCommMask()指定了有用的事件后，应用程序可调用WaitCommEvent()来等待事
    // 件的发生。SetCommMask(hComm,0)可使WaitCommEvent()中止
    if (SetCommMask(s->m_hComm, EV_RXCHAR)) { // 设置通信事件
      if (GetCommState(s->m_hComm, &s->m_dcb)) { // 获取当前DCB参数
        s->m_dcb.EvtChar = 'q';
        s->m_dcb.fRtsControl = RTS_CONTROL_ENABLE; // set RTS bit high!
        s->m_dcb.BaudRate = baud; // add by mrlong
        s->m_dcb.Parity = myparity;
        s->m_dcb.ByteSize = databits;
        s->m_dcb.StopBits = mystop;
        if (!SetCommState(s->m_hComm, &s->m_dcb)) {// 配置DCB
          ProcessErrorMessage("SetCommState()");
        }
      }
      else {
        ProcessErrorMessage("GetCommState()");
      }
    }
    else {
      ProcessErrorMessage("SetCommMask()");
    }
  }
  else {
    ProcessErrorMessage("SetCommTimeouts()");
  }
  // flush the s
  // 终止读写并清空接收和发送
  PurgeComm(s->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
  // release critical section
  // 释放临界资源
  LeaveCriticalSection(&s->m_csCommunicationSync);
  //TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", port);
  
  SetupComm(s->m_hComm,100*1024,100*1024);
  // start comm watching
  // 开启监视线程
  //if (!(s->m_Thread = AfxBeginThread(CommThread, this)))
  if (!(s->m_Thread = CreateThread(NULL, 0, sys_win32_serial_port_CommThread, ss, 0, NULL))) {
    return 0;
  }
  //TRACE("Thread started\n");
  return port+1;
}

#endif // _SYS_WIN32_SERIAL_PORT_INL_
