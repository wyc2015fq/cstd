#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_
#include <assert.h>
#include <stdio.h>
#include <windows.h>
char HexChar(char c)
{
  if ((c >= '0') && (c <= '9')) {
    return c - 0x30;
  }
  else if ((c >= 'A') && (c <= 'F')) {
    return c - 'A' + 10;
  }
  else if ((c >= 'a') && (c <= 'f')) {
    return c - 'a' + 10;
  }
  else {
    return 0x10;
  }
}
//将一个字符串作为十六进制串转化为一个字节数组，字节间可用空格分隔，
//返回转换后的字节数组长度，同时字节数组长度自动设置。
int Str2Hex(const char* str, char* data)
{
  int i, t, t1;
  int rlen = 0, len = strlen(str);
  //data.SetSize(len/2);
  for (i = 0; i < len;) {
    char l, h = str[i];
    if (h == ' ') {
      i++;
      continue;
    }
    i++;
    if (i >= len) {
      break;
    }
    l = str[i];
    t = HexChar(h);
    t1 = HexChar(l);
    if ((t == 16) || (t1 == 16)) {
      break;
    }
    else {
      t = t * 16 + t1;
    }
    i++;
    data[rlen] = (char)t;
    rlen++;
  }
  return rlen;
}
#ifndef TRACE
#define TRACE printf
#endif // TRACE
#define WM_COMM_BREAK_DETECTED     WM_USER+1  // A break was detected on input.
#define WM_COMM_CTS_DETECTED       WM_USER+2  // The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED       WM_USER+3  // The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED       WM_USER+4  // A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED      WM_USER+5  // A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED      WM_USER+6  // The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR             WM_USER+7  // A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED    WM_USER+8  // The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED   WM_USER+9  // The last character in the output buffer was sent.  
typedef struct serialport {
  int m_nWriteSize;
  // thread
  HANDLE      m_Thread;
  // synchronisation objects
  CRITICAL_SECTION  m_csCommunicationSync;
  BOOL        m_bThreadAlive;
  // handles
  HANDLE        m_hWriteEvent;
  HANDLE        m_hShutdownEvent;
  // Event array.
  // One element is used for each event. There are two event handles for each port.
  // A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
  // There is a general shutdown when the port is closed.
  HANDLE        m_hEventArray[3];
  // structures
  OVERLAPPED      m_ov;
  COMMTIMEOUTS    m_CommTimeouts;
  DCB             m_dcb;
  // owner
  void*        m_pOwner;
  // misc
  UINT         m_nPortNr;
  char*        m_szWriteBuffer;
  DWORD        m_dwCommEvents;
  DWORD        m_nWriteBufferSize;
  HANDLE       m_hComm;
} serialport;
//
// If there is a error, give the right message
//
void ProcessErrorMessage(char* ErrorText)
{
  char Temp[200];
  LPVOID lpMsgBuf;
  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
      NULL,
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf,
      0,
      NULL
  );
  sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, 0);
  MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);
  LocalFree(lpMsgBuf);
}
// Initialize the port. This can be port 1 to 4.
BOOL serialport_open(serialport* seri,
    void* pPortOwner,  // the owner (void) of the port (receives message)
    UINT  portnr,    // portnumber (1..4)
    UINT  baud,      // baudrate
    char  parity,    // parity
    UINT  databits,    // databits
    UINT  stopbits,    // stopbits
    DWORD dwCommEvents,     // EV_RXCHAR, EV_CTS etc
    UINT  writebuffersize)  // size to the writebuffer
{
  BOOL bResult = FALSE;
  char szPort[50] = {0};
  char szBaud[50] = {0};
  // initialize overlapped structure members to zero
  seri->m_nWriteSize = 1;
  assert(portnr > 0 && portnr < 5);
  // if the thread is alive: Kill
  if (seri->m_bThreadAlive) {
    do {
      SetEvent(seri->m_hShutdownEvent);
    }
    while (seri->m_bThreadAlive);
    TRACE("Thread ended\n");
  }
  // create events
  if (seri->m_ov.hEvent != NULL) {
    ResetEvent(seri->m_ov.hEvent);
  }
  else {
    seri->m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  }
  if (seri->m_hWriteEvent != NULL) {
    ResetEvent(seri->m_hWriteEvent);
  }
  else {
    seri->m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  }
  if (seri->m_hShutdownEvent != NULL) {
    ResetEvent(seri->m_hShutdownEvent);
  }
  else {
    seri->m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  }
  // initialize the event objects
  seri->m_hEventArray[0] = seri->m_hShutdownEvent;  // highest priority
  seri->m_hEventArray[1] = seri->m_ov.hEvent;
  seri->m_hEventArray[2] = seri->m_hWriteEvent;
  // initialize critical section
  InitializeCriticalSection(&seri->m_csCommunicationSync);
  // set buffersize for writing and save the owner
  seri->m_pOwner = pPortOwner;
  if (seri->m_szWriteBuffer != NULL) {
    free(seri->m_szWriteBuffer);
  }
  seri->m_szWriteBuffer = (char*)malloc(writebuffersize);
  seri->m_nPortNr = portnr;
  seri->m_nWriteBufferSize = writebuffersize;
  seri->m_dwCommEvents = dwCommEvents;
  // now it critical!
  EnterCriticalSection(&seri->m_csCommunicationSync);
  // if the port is already opened: close it
  if (seri->m_hComm != NULL) {
    CloseHandle(seri->m_hComm);
    seri->m_hComm = NULL;
  }
  // prepare port strings
  sprintf(szPort, "COM%d", portnr);
  sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);
  // get a handle to the port
  seri->m_hComm = CreateFile(szPort,            // communication port string (COMX)
      GENERIC_READ | GENERIC_WRITE,  // read/write types
      0,                // comm devices must be opened with exclusive access
      NULL,              // no security attributes
      OPEN_EXISTING,          // comm devices must use OPEN_EXISTING
      FILE_FLAG_OVERLAPPED,      // Async I/O
      0);              // template must be 0 for comm devices
  if (seri->m_hComm == INVALID_HANDLE_VALUE) {
    return FALSE;
  }
  // set the timeout values
  seri->m_CommTimeouts.ReadIntervalTimeout = 1000;
  seri->m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
  seri->m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
  seri->m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
  seri->m_CommTimeouts.WriteTotalTimeoutConstant = 1000;
  // configure
  if (SetCommTimeouts(seri->m_hComm, &seri->m_CommTimeouts)) {
    if (SetCommMask(seri->m_hComm, dwCommEvents)) {
      if (GetCommState(seri->m_hComm, &seri->m_dcb)) {
        seri->m_dcb.EvtChar = 'q';
        seri->m_dcb.fRtsControl = RTS_CONTROL_ENABLE;    // set RTS bit high!
        if (BuildCommDCB(szBaud, &seri->m_dcb)) {
          if (SetCommState(seri->m_hComm, &seri->m_dcb))
            ; // normal operation... continue
          else {
            ProcessErrorMessage("SetCommState()");
          }
        }
        else {
          ProcessErrorMessage("BuildCommDCB()");
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
  // flush the port
  PurgeComm(seri->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
  // release critical section
  LeaveCriticalSection(&seri->m_csCommunicationSync);
  TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);
  return TRUE;
}
// Write a character.
void serialport_writechar(serialport* seri)
{
  BOOL bWrite = TRUE;
  BOOL bResult = TRUE;
  DWORD BytesSent = 0;
  ResetEvent(seri->m_hWriteEvent);
  // Gain ownership of the critical section
  EnterCriticalSection(&seri->m_csCommunicationSync);
  if (bWrite) {
    // Initailize variables
    seri->m_ov.Offset = 0;
    seri->m_ov.OffsetHigh = 0;
    // Clear buffer
    PurgeComm(seri->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    bResult = WriteFile(seri->m_hComm,  // Handle to COMM Port
        seri->m_szWriteBuffer, // Pointer to message buffer in calling finction
        // strlen((char*)seri->m_szWriteBuffer),  // Length of message to send
        seri->m_nWriteSize,  // Length of message to send
        &BytesSent,          // Where to store the number of bytes sent
        &seri->m_ov);        // Overlapped structure
    // deal with any error codes
    if (!bResult) {
      DWORD dwError = GetLastError();
      switch (dwError) {
      case ERROR_IO_PENDING: {
        // continue to GetOverlappedResults()
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
      LeaveCriticalSection(&seri->m_csCommunicationSync);
    }
  } // end if(bWrite)
  if (!bWrite) {
    bWrite = TRUE;
    bResult = GetOverlappedResult(seri->m_hComm,  // Handle to COMM port
        &seri->m_ov,    // Overlapped structure
        &BytesSent,    // Stores number of bytes sent
        TRUE);       // Wait flag
    LeaveCriticalSection(&seri->m_csCommunicationSync);
    // deal with the error code
    //    if (!bResult)
    {
      // port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
    }
  } // end if (!bWrite)
  //Verify that the data size send equals what we tried to send
  if ((int)BytesSent != seri->m_nWriteSize) { // Length of message to send)
    TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)seri->m_szWriteBuffer));
  }
  //  ::SendMessage((seri->m_pOwner)->seri->m_hWnd, WM_COMM_TXEMPTY_DETECTED, (WPARAM) RXBuff, (LPARAM) seri->m_nPortNr);
  //  ::SendMessage((seri->m_pOwner)->seri->m_hWnd, WM_COMM_TXEMPTY_DETECTED,0,(LPARAM) seri->m_nPortNr);
}
void serialport_WriteToPort(serialport* seri, LPCTSTR string, int n)
{
  assert(seri->m_hComm != 0);
  memset(seri->m_szWriteBuffer, 0, sizeof(seri->m_szWriteBuffer));
  //  strncpy(seri->m_szWriteBuffer, string, n);
  memcpy(seri->m_szWriteBuffer, string, n);
  seri->m_nWriteSize = n;
  // set event for write
  SetEvent(seri->m_hWriteEvent);
}
// Constructor
//
//
// Delete dynamic memory
//
int serialport_close(serialport* seri)
{
  do {
    SetEvent(seri->m_hShutdownEvent);
  }
  while (seri->m_bThreadAlive);
  // if the port is still opened: close it
  if (seri->m_hComm != NULL) {
    CloseHandle(seri->m_hComm);
    seri->m_hComm = NULL;
  }
  // Close Handles
  if (seri->m_hShutdownEvent != NULL) {
    CloseHandle(seri->m_hShutdownEvent);
  }
  if (seri->m_ov.hEvent != NULL) {
    CloseHandle(seri->m_ov.hEvent);
  }
  if (seri->m_hWriteEvent != NULL) {
    CloseHandle(seri->m_hWriteEvent);
  }
  TRACE("Thread ended\n");
  free(seri->m_szWriteBuffer);
  return 0;
}
#if 0
//
//
//
//  The CommThread Function.
//
UINT serialport_CommThread(LPVOID pParam)
{
  // Cast the void pointer passed to the thread back to
  // a pointer of serialport class
  serialport* port = (serialport*)pParam;
  // Set the status variable in the dialog class to
  // TRUE to indicate the thread is running.
  seri->m_bThreadAlive = TRUE;
  // Misc. variables
  DWORD BytesTransfered = 0;
  DWORD Event = 0;
  DWORD CommEvent = 0;
  DWORD dwError = 0;
  COMSTAT comstat;
  BOOL  bResult = TRUE;
  // Clear comm buffers at startup
  if (seri->m_hComm) {  // check if the port is opened
    PurgeComm(seri->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
  }
  // begin forever loop.  This loop will run as long as the thread is alive.
  for (;;) {
    // Make a call to WaitCommEvent().  This call will return immediatly
    // because our port was created as an async port (FILE_FLAG_OVERLAPPED
    // and an seri->m_OverlappedStructerlapped structure specified).  This call will cause the
    // seri->m_OverlappedStructerlapped element seri->m_OverlappedStruct.hEvent, which is part of the seri->m_hEventArray to
    // be placed in a non-signeled state if there are no bytes available to be read,
    // or to a signeled state if there are bytes available.  If this event handle
    // is set to the non-signeled state, it will be set to signeled when a
    // character arrives at the port.
    // we do this for each port!
    bResult = WaitCommEvent(seri->m_hComm, &Event, &seri->m_ov);
    if (!bResult) {
      // If WaitCommEvent() returns FALSE, process the last error to determin
      // the reason..
      switch (dwError = GetLastError()) {
      case ERROR_IO_PENDING: {
        // This is a normal return value if there are no bytes
        // to read at the port.
        // Do nothing and continue
        break;
      }
      case 87: {
        // Under Windows NT, this value is returned for some reason.
        // I have not investigated why, but it is also a valid reply
        // Also do nothing and continue.
        break;
      }
      default: {
        // All other error codes indicate a serious error has
        // occured.  Process this error.
        port->ProcessErrorMessage("WaitCommEvent()");
        break;
      }
      }
    }
    else {
      // If WaitCommEvent() returns TRUE, check to be sure there are
      // actually bytes in the buffer to read.
      //
      // If you are reading more than one byte at a time from the buffer
      // (which this program does not do) you will have the situation occur
      // where the first byte to arrive will cause the WaitForMultipleObjects()
      // function to stop waiting.  The WaitForMultipleObjects() function
      // resets the event handle in seri->m_OverlappedStruct.hEvent to the non-signelead state
      // as it returns.
      //
      // If in the time between the reset of this event and the call to
      // ReadFile() more bytes arrive, the seri->m_OverlappedStruct.hEvent handle will be set again
      // to the signeled state. When the call to ReadFile() occurs, it will
      // read all of the bytes from the buffer, and the program will
      // loop back around to WaitCommEvent().
      //
      // At this point you will be in the situation where seri->m_OverlappedStruct.hEvent is set,
      // but there are no bytes available to read.  If you proceed and call
      // ReadFile(), it will return immediatly due to the async port setup, but
      // GetOverlappedResults() will not return until the next character arrives.
      //
      // It is not desirable for the GetOverlappedResults() function to be in
      // this state.  The thread shutdown event (event 0) and the WriteFile()
      // event (Event2) will not work if the thread is blocked by GetOverlappedResults().
      //
      // The solution to this is to check the buffer with a call to ClearCommError().
      // This call will reset the event handle, and if there are no bytes to read
      // we can loop back through WaitCommEvent() again, then proceed.
      // If there are really bytes to read, do nothing and proceed.
      bResult = ClearCommError(seri->m_hComm, &dwError, &comstat);
      if (comstat.cbInQue == 0) {
        continue;
      }
    }  // end if bResult
    // Main wait function.  This function will normally block the thread
    // until one of nine events occur that require action.
    Event = WaitForMultipleObjects(3, seri->m_hEventArray, FALSE, INFINITE);
    switch (Event) {
    case 0: {
      // Shutdown event.  This is event zero so it will be
      // the higest priority and be serviced first.
      CloseHandle(seri->m_hComm);
      seri->m_hComm = NULL;
      seri->m_bThreadAlive = FALSE;
      // Kill this thread.  break is not needed, but makes me feel better.
      AfxEndThread(100);
      break;
    }
    case 1: { // read event
      GetCommMask(seri->m_hComm, &CommEvent);
      if (CommEvent & EV_RXCHAR)
        // Receive character event from port.
      {
        ReceiveChar(port, comstat);
      }
      if (CommEvent & EV_CTS) {
        ::SendMessage(seri->m_pOwner->seri->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) seri->m_nPortNr);
      }
      if (CommEvent & EV_BREAK) {
        ::SendMessage(seri->m_pOwner->seri->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM) seri->m_nPortNr);
      }
      if (CommEvent & EV_ERR) {
        ::SendMessage(seri->m_pOwner->seri->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM) seri->m_nPortNr);
      }
      if (CommEvent & EV_RING) {
        ::SendMessage(seri->m_pOwner->seri->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) seri->m_nPortNr);
      }
      if (CommEvent & EV_RXFLAG) {
        ::SendMessage(seri->m_pOwner->seri->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) seri->m_nPortNr);
      }
      break;
    }
    case 2: { // write event
      // Write character event from port
      WriteChar(port);
      break;
    }
    } // end switch
  } // close forever loop
  return 0;
}
//
// start comm watching
//
BOOL serialport_StartMonitoring()
{
  if (!(seri->m_Thread = AfxBeginThread(CommThread, this))) {
    return FALSE;
  }
  TRACE("Thread started\n");
  return TRUE;
}
//
// Restart the comm thread
//
BOOL serialport_RestartMonitoring()
{
  TRACE("Thread resumed\n");
  seri->m_Thread->ResumeThread();
  return TRUE;
}
//
// Suspend the comm thread
//
BOOL serialport_StopMonitoring()
{
  TRACE("Thread suspended\n");
  seri->m_Thread->SuspendThread();
  return TRUE;
}
//
// Character received. Inform the owner
//
void serialport_ReceiveChar(serialport* port, COMSTAT comstat)
{
  BOOL  bRead = TRUE;
  BOOL  bResult = TRUE;
  DWORD dwError = 0;
  DWORD BytesRead = 0;
  unsigned char RXBuff;
  for (;;) {
    // Gain ownership of the comm port critical section.
    // This process guarantees no other part of this program
    // is using the port object.
    EnterCriticalSection(&seri->m_csCommunicationSync);
    // ClearCommError() will update the COMSTAT structure and
    // clear any other errors.
    bResult = ClearCommError(seri->m_hComm, &dwError, &comstat);
    LeaveCriticalSection(&seri->m_csCommunicationSync);
    // start forever loop.  I use this type of loop because I
    // do not know at runtime how many loops this will have to
    // run. My solution is to start a forever loop and to
    // break out of it when I have processed all of the
    // data available.  Be careful with this approach and
    // be sure your loop will exit.
    // My reasons for this are not as clear in this sample
    // as it is in my production code, but I have found this
    // solutiion to be the most efficient way to do this.
    if (comstat.cbInQue == 0) {
      // break out when all bytes have been read
      break;
    }
    EnterCriticalSection(&seri->m_csCommunicationSync);
    if (bRead) {
      bResult = ReadFile(seri->m_hComm,    // Handle to COMM port
          &RXBuff,        // RX Buffer Pointer
          1,          // Read one byte
          &BytesRead,      // Stores number of bytes read
          &seri->m_ov);    // pointer to the seri->m_ov structure
      // deal with the error code
      if (!bResult) {
        switch (dwError = GetLastError()) {
        case ERROR_IO_PENDING: {
          // asynchronous i/o is still in progress
          // Proceed on to GetOverlappedResults();
          bRead = FALSE;
          break;
        }
        default: {
          // Another error has occured.  Process this error.
          port->ProcessErrorMessage("ReadFile()");
          break;
        }
        }
      }
      else {
        // ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
        bRead = TRUE;
      }
    }  // close if (bRead)
    if (!bRead) {
      bRead = TRUE;
      bResult = GetOverlappedResult(seri->m_hComm,  // Handle to COMM port
          &seri->m_ov,    // Overlapped structure
          &BytesRead,    // Stores number of bytes read
          TRUE);       // Wait flag
      // deal with the error code
      if (!bResult) {
        port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
      }
    }  // close if (!bRead)
    LeaveCriticalSection(&seri->m_csCommunicationSync);
    // notify parent that a byte was received
    ::SendMessage((seri->m_pOwner)->seri->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) seri->m_nPortNr);
  } // end forever loop
}
//
// Write a string to the port
//
void serialport_WriteToPort(char* string)
{
  assert(seri->m_hComm != 0);
  memset(seri->m_szWriteBuffer, 0, sizeof(seri->m_szWriteBuffer));
  strcpy(seri->m_szWriteBuffer, string);
  seri->m_nWriteSize = strlen(string);
  // set event for write
  SetEvent(seri->m_hWriteEvent);
}
//
// Return the device control block
//
DCB serialport_GetDCB()
{
  return seri->m_dcb;
}
//
// Return the communication event masks
//
DWORD serialport_GetCommEvents()
{
  return seri->m_dwCommEvents;
}
//
// Return the output buffer size
//
DWORD serialport_GetWriteBufferSize()
{
  return seri->m_nWriteBufferSize;
}
void serialport_ClosePort()
{
  SetEvent(seri->m_hShutdownEvent);
}
#endif //
#endif // _SERIALPORT_H_

