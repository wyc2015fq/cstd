#ifndef _WINCOM_H_
#define _WINCOM_H_
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#define IsComOpen(_PCOM)   (INVALID_HANDLE_VALUE!=(_PCOM->hComm))
typedef struct wincom_t {
  OVERLAPPED ovWrite;
  OVERLAPPED ovRead;
  HANDLE hReadThread;
  HANDLE hThreadEnd;
  HANDLE hComm;
}
wincom_t;
static int WINAPI AsynComReadThread(void* p);
//关闭串口TURE 成功,FALSE 失败
int wincom_close(wincom_t* pcom)
{
  if (CloseHandle(pcom->hComm)) {
    pcom->hComm = INVALID_HANDLE_VALUE;
    WaitForSingleObject(pcom->hThreadEnd, 6 * 1000);    //只要发生事件就行了,不需要进行处理
    if (pcom->ovWrite.hEvent != NULL) {
      CloseHandle(pcom->ovWrite.hEvent);
      pcom->ovWrite.hEvent = NULL;
    }
    if (pcom->ovRead.hEvent) {
      CloseHandle(pcom->ovRead.hEvent);
      pcom->ovRead.hEvent = NULL;
    }
    if (pcom->hThreadEnd == NULL) {
      CloseHandle(pcom->hThreadEnd);
      pcom->hThreadEnd = NULL;
    }
    return 1;
  }
  return 0;
}
// 找开COMM口,并设置COMM口的一些参数.
// pPort 端口号, nBaudRate 波特率, nParity 奇偶校验, nByteSize 几位, nStopBits 停止位
// TURE 成功,FALSE 失败
// wincom_open(pcom, "COM1", 115200, NOPARITY, 8, ONESTOPBIT);
int wincom_open(wincom_t* pcom, const TCHAR* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
  DCB dcb;  // 串口控制块
  COMMTIMEOUTS timeouts = {
    // 串口超时控制参数
    100,   // 读字符间隔超时时间: 100 ms
    1,   // 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
    500, // 基本的(额外的)读超时时间: 500 ms
    1,   // 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
    100  // 基本的(额外的)写超时时间: 100 ms
  };
  pcom->hComm = INVALID_HANDLE_VALUE;
  pcom->hReadThread = NULL;
  ZeroMemory(&pcom->ovWrite, sizeof(OVERLAPPED));
  pcom->ovWrite.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  ZeroMemory(&pcom->ovRead, sizeof(OVERLAPPED));
  pcom->ovRead.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  pcom->hThreadEnd = CreateEvent(NULL, FALSE, TRUE, NULL);
  if (pcom->ovRead.hEvent == NULL || pcom->ovWrite.hEvent == NULL || pcom->hThreadEnd == NULL) {
    // "hEvent create fail";
    return 0;
  }
  pcom->hComm = CreateFile(pPort,    // 串口名称
      GENERIC_READ | GENERIC_WRITE,   // 读写方式
      0,   // 共享方式：独占
      NULL,   // 默认的安全描述符
      OPEN_EXISTING,   // 创建方式
      0,   // 不需设置文件属性
      NULL);  // 不需参照模板文件
  if (pcom->hComm == INVALID_HANDLE_VALUE) {
    wincom_close(pcom);
    return 0;  // 打开串口失败
  }
  //取DCB
  if (GetCommState(pcom->hComm, &dcb) == 0) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  dcb.BaudRate = nBaudRate;
  dcb.ByteSize = nByteSize;
  dcb.Parity = nParity;
  dcb.StopBits = nStopBits;
  if (SetCommState(pcom->hComm, &dcb) == 0) {
    //设置DCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  if (SetupComm(pcom->hComm, 4096, 1024) == 0) {
    //设置输入输出缓冲区大小
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  if (SetCommTimeouts(pcom->hComm, &timeouts) == 0) {
    //设置超时
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  //清空COMM的接收缓冲和输出缓冲
  if (!PurgeComm(pcom->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  return 1;
}
//向串口写数据,异步读取
//TURE 成功, FALSE 失败
//由于硬件不停的向串口发送数据, 如果不进行读取,可以将串口堵死
BOOL ComAsynRead(wincom_t* pcom)
{
  pcom->hReadThread = CreateThread(NULL, 0, AsynComReadThread, pcom, 0, NULL);
  if (pcom->hReadThread == INVALID_HANDLE_VALUE) {
    return FALSE;
  }
  else {
    return TRUE;
  }
}
// 找开COMM口,并设置COMM口的一些参数.
// pPort 端口号, nBaudRate 波特率, nParity 奇偶校验, nByteSize 几位, nStopBits 停止位
// TURE 成功,FALSE 失败
int wincom_openex(wincom_t* pcom, const TCHAR* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
  DCB dcb;  // 串口控制块
  COMMTIMEOUTS timeouts = {
    // 串口超时控制参数
    100,   // 读字符间隔超时时间: 100 ms
    1,   // 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
    500,   // 基本的(额外的)读超时时间: 500 ms
    1,   // 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
    100   // 基本的(额外的)写超时时间: 100 ms
  };
  pcom->hComm = INVALID_HANDLE_VALUE;
  pcom->hReadThread = NULL;
  ZeroMemory(&pcom->ovWrite, sizeof(OVERLAPPED));
  pcom->ovWrite.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  ZeroMemory(&pcom->ovRead, sizeof(OVERLAPPED));
  pcom->ovRead.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  pcom->hThreadEnd = CreateEvent(NULL, FALSE, TRUE, NULL);
  if (pcom->ovRead.hEvent == NULL || pcom->ovWrite.hEvent == NULL || pcom->hThreadEnd == NULL) {
    // "hEvent create fail";
    return 0;
  }
  pcom->hComm = CreateFile(pPort,    // 串口名称
      GENERIC_READ | GENERIC_WRITE,   // 读写方式
      0,   // 共享方式：独占
      NULL,   // 默认的安全描述符
      OPEN_EXISTING,   // 创建方式
      FILE_FLAG_OVERLAPPED,   //异步
      NULL);  // 不需参照模板文件
  if (pcom->hComm == INVALID_HANDLE_VALUE) {
    wincom_close(pcom);
    return 0;  // 打开串口失败
  }
  if (GetCommState(pcom->hComm, &dcb) == 0) {
    //取DCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  dcb.BaudRate = nBaudRate;
  dcb.ByteSize = nByteSize;
  dcb.Parity = nParity;
  dcb.StopBits = nStopBits;
  if (SetCommState(pcom->hComm, &dcb) == 0) {
    //设置DCB
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  if (SetupComm(pcom->hComm, 4096, 1024) == 0) {
    //设置输入输出缓冲区大小
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  if (SetCommTimeouts(pcom->hComm, &timeouts) == 0) {
    //设置超时
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  //清空COMM的接收缓冲和输出缓冲
  if (!PurgeComm(pcom->hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  //创建接收数据线程
  if (!ComAsynRead(pcom)) {
    if (pcom->hComm != INVALID_HANDLE_VALUE) {
      CloseHandle(pcom->hComm);
      pcom->hComm = INVALID_HANDLE_VALUE;
    }
    wincom_close(pcom);
    return 0;
  }
  return 1;
}
//向串口写数据,异步读取
//读取数据线程,如果有数据过来先进行同步读取,同步失败在进行异步
int WINAPI AsynComReadThread(void* p)
{
  wincom_t* pThis = (wincom_t*) p;
  DWORD dwEvtMask = 0;
  DWORD dwReadByte = 0;
  DWORD dwBytes = 0;
  char szInBuffer[ 5120 ] = { 0 };
  OVERLAPPED ovEvtMast;
  ZeroMemory(&ovEvtMast, sizeof(OVERLAPPED));
  ovEvtMast.hEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
  if (pThis == NULL) {
    if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
      CloseHandle(ovEvtMast.hEvent);
      ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
    }
    ExitThread(0);
  }
  else {
    while (TRUE) {
      //关闭com,退出线程
      if (!IsComOpen(pThis)) {
        if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
          CloseHandle(ovEvtMast.hEvent);
          ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
          //set event exit process
          SetEvent(pThis->hThreadEnd);
        }
        ExitThread(0);
      }
      //重置事件
      if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
        ResetEvent(ovEvtMast.hEvent);
      }
      //设置事件驱动,等待事件驱动
      SetCommMask(pThis->hComm, EV_RXCHAR | EV_TXEMPTY);
      WaitCommEvent(pThis->hComm, &dwEvtMask, &ovEvtMast);
      if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR) {
        COMSTAT ComStat;
        DWORD dwLength = 0;
        DWORD dwErr = 0;
        ZeroMemory(&ComStat, sizeof(COMSTAT));
        ClearCommError(pThis->hComm, &dwErr, &ComStat);
        dwLength = ComStat.cbInQue;
        if (dwLength > 0) {
          BOOL bReadState = FALSE;
          bReadState = ReadFile(pThis->hComm, &szInBuffer, dwLength, &dwReadByte, &pThis->ovRead);
          if (bReadState == FALSE) {
            if (GetLastError() == ERROR_IO_INCOMPLETE) {
              DWORD dwErrCode = 0;
              while (TRUE) {
                if (!GetOverlappedResult(pThis->hComm, &pThis->ovRead, &dwReadByte, TRUE)) {
                  dwErrCode = GetLastError();
                  if (dwErrCode == ERROR_IO_INCOMPLETE) {
                    Sleep(1);
                    continue;
                  }
                  else {
                    if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
                      CloseHandle(ovEvtMast.hEvent);
                      ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
                    }
                    ExitThread(0);
                  }
                }
                else {
                  dwBytes += dwReadByte;
                  if (dwBytes == dwLength || dwBytes == 5120) {
                    //MessageBox( NULL, "FULL", "msg", MB_OK );
                    ZeroMemory(&szInBuffer, 5120);
                    dwBytes = 0;
                    break;
                  }
                  Sleep(1);
                }
              }
            }
            else {
              if (ovEvtMast.hEvent != INVALID_HANDLE_VALUE) {
                CloseHandle(ovEvtMast.hEvent);
                ovEvtMast.hEvent = INVALID_HANDLE_VALUE;
              }
              ExitThread(0);
            }
          }
          else {
            //以后要读取数据,添加读取回调
            //add function here
            ZeroMemory(&szInBuffer, 5120);
            //MessageBox( NULL, "not overlapped", "msg", MB_OK );
          }
        }
      }
      Sleep(1);
    }
  }
  return 0;
}
//向串口写数据,同步写入.
//pData 写入数据的内存在址, nLength 写入数据的长度.
//-1 失败, >0 写入数据大小
int wincom_write(const wincom_t* pcom, const void* pData, int nLength)
{
  int ret = -1;
  int dwNumWrite = 0;
  if (IsComOpen(pcom)) {
    PurgeComm(pcom->hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    if (WriteFile(pcom->hComm, pData, nLength, &dwNumWrite, NULL) != 0 /*&& dwNumWrite == nLength*/ && dwNumWrite > 0) {
      ret = dwNumWrite;
    }
  }
  return ret;
}
//向串口写数据,异步写入.
//pData 写入数据的内存在址, nLength 写入数据的长度.
//-1 失败, >0 写入数据大小
int wincom_writeex(wincom_t* pcom, void* pData, int dwLength)
{
  int dwNumWrite = 0;
  BOOL bWriteState = FALSE;
  DWORD dwErrCode = 0;
  int dwWriteByte = 0;
  if (IsComOpen(pcom)) {
    bWriteState = WriteFile(pcom->hComm, pData, dwLength, &dwNumWrite, &pcom->ovWrite);
    if (bWriteState == FALSE) {
      if (GetLastError() == ERROR_IO_PENDING) {
        while (TRUE) {
          //获取异步结果
          if (!GetOverlappedResult(pcom->hComm, &pcom->ovWrite, &dwNumWrite, TRUE)) {
            dwErrCode = GetLastError();
            if (dwErrCode == ERROR_IO_INCOMPLETE) {
              continue;
            }
            else {
              return -1;
            }
          }
          else {
            dwWriteByte += dwNumWrite;
            if (dwWriteByte == dwLength) {
              break;
            }
          }
          Sleep(1);
        }
      }
      else {
        return -1;
      }
    }
    else {
      if (dwLength > 0 && dwLength == dwNumWrite) {
        return dwNumWrite;
      }
      else {
        return -1;
      }
    }
  }
  return dwWriteByte;
}
//读取串口的数据, 同步读取.
//pData 读取数据要存取的内存在址, nLength 读取数据的内存长度.
//-1 失败, >0 读取数据大小
int wincom_read(const wincom_t* pcom, void* pData, int nLength)
{
  DWORD dwNumRead = 0;
  if (IsComOpen(pcom)) {
    OVERLAPPED Overlapped = {0};
    if (ReadFile(pcom->hComm, pData, nLength, &dwNumRead, NULL) != 0 /*&& dwNumRead == nLength*/ && dwNumRead >= 0) {
      return dwNumRead;
    }
    else {
      return -1;
    }
  }
  return -1;
}
//读取串口的数据, 同步读取.
//pData 读取数据要存取的内存在址, nLength 读取数据的内存长度.
//-1 失败, >0 读取数据大小
int wincom_readex(const wincom_t* pcom, char* pData, int nLength)
{
  int iTime = 2;
  while (1) {
    //如果串口没有数据返回,ComRead一定返回为-1.
    int len = wincom_read(pcom, pData, nLength);
    if (len > 0) {
      return len;
    }
    iTime--;
    if (iTime <= 0) {
      return 0;
    }
    //Sleep(100);
  }
  return 0;
}
int test_wincom()
{
  int i;
  int ch;
  //控制车闸开启: A2 0B 0A 60 01 01 00 00 00 00 00 77 2A
  //控制车闸关闭: A2 0B 0A 80 01 01 00 00 00 00 00 97 2A
  unsigned char cmd1[] = {0xA2, 0x0B, 0x0A, 0x60, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x2A};
  unsigned char cmd2[] = {0xA2, 0x0B, 0x0A, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97, 0x2A};
  cvNamedWindow("test_wincom", 1);
  for (i = 0;; ++i) {
    ch = cvWaitKey(100);
    ch = '1';
    //if ('1'==ch)
    {
      wincom_t com[1] = {0};
      printf("控制车闸开启\n");
      if (wincom_open(com, "COM2", 19200, NOPARITY, 8, ONESTOPBIT)) {
        wincom_write(com, cmd1, countof(cmd1));
        wincom_close(com);
        wincom_close(com);
      }
    }
  }
  return 0;
}
#endif // _COM_INL_

