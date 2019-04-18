# POP3协议封装类 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:06:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1140


POP3协议封装类

目前互连网上，EMAIL的使用是越来越广泛了。在所有的TCP连接线路中，大概有一半的线路是用来收发EMAIL的。因此，许多网络应用程序要想离开POP3协议是不可能的。 

　　而且，很多EMAIL系统都是使用SMTP协议来作为发送协议，而POP3协议来作为接受协议。关于这2个协议的许多资料，你可以看一下RCF821（关于SMTP的）和RCF1225（关于POP3的）。虽然在WINDOWS操作系统中，有一个叫MAPI应用程序是来管理EMAIL的，但是它的功能还不够强大，不足以直接处理一些EMAIL服务。所以，我写了一个有关于POP3协议的C++类，在这个类中我还使用了CSocket作为一个类成员（这好象有点不可想象），这样的话呢，我们可以在连接的时候使用它。并且，该POP类使用的一些功能类似于POP3协议中的一些命令。下面就代码： 

/*-------------------------------------------------------------------- 
Pop.h : main header file for the POP application 
-----------------------------------------------------------------------*/ 

#if !defined（AFX_POP_H__A44B38B6_697C_11D1_881E_00001C302581__INCLUDED_） 
#define AFX_POP_H__A44B38B6_697C_11D1_881E_00001C302581__INCLUDED_ 

#define CONNECTION_CHECK 0 
#define USER_CHECK 1 
#define PASSWORD_CHECK 2 
#define QUIT_CHECK 3 
#define DELETE_CHECK 4 
#define RSET_CHECK 5 
#define STAT_CHECK 6 
#define NOOP_CHECK 7 
#define LIST_CHECK 8 
#define RETR_CHECK 9 

///////////////////////////////////////////////////////////////////////////// 
class CPop 
{ 
public: 
      BOOL List（）; 
      CWordArray m_SizeOfMsg; 
      CString GetErrorMessage（）; // If there is any error this will return it method 
      CString GetPassword（）; // Getting Password stored in class 
      void SetPassword（CString& Password）; // Setting Password in class 
      CString GetUser（）; // Getting user name stored in class 
      void SetUser（CString& User）; // Setting user name in class 
      CString GetHost（）; // Getting Host name （email server name） stored in class
      void SetHost（CString& Host）; // Setting Host name （email server name） in class 
      BOOL Connect（）; // Connecting to email server 
      int GetTotalMailSize（）; // it returns the Total Mail Size 
      int GetNumberOfMails（）; // It return the number of mails 
      CString GetMsgContents（）; 
      BOOL Statistics（）; // issue the STAT command on email server 
      BOOL Retrieve（int MsgNumber）; // Getting any particular mail message 
      BOOL Reset（）; // issue the reset command on email server 
      int GetMessageSize（int MsgNumber）; // Return a size of any particular mail 
      BOOL Noop（）; // issue the NOOP command on email server 
      BOOL Disconnect（）; // issue the QUIT command on email server 
      BOOL Delete（int& MsgNumber）; // Deleteing a particular message from email server 
      BOOL Connect（CString& Host, CString& User, CString& Password）; 
      CPop（）; 
      virtual ~CPop（）; 

private: 
      CString m_ErrorMessage; 
      BOOL CheckResponse（int ResponseType）; 
      CString m_Password; 
      CString m_User; 
      CString m_Host; 
      CString m_MsgContents; 
      int m_TotalSize; 
      int m_NumberMail; 
      CSocket m_PopServer; 
}; 
/#endif // !defined（AFX_POP_H__A44B38B6_697C_11D1_881E_00001C302581__INCLUDED_） 

/*----------------------------------------------------------------------------------------------- 
// Pop.cpp : Defines the class behaviors for the application. 
---------------------------------------------------------------------------------------------------*/ 

#include "stdafx.h" 
#include "Pop.h" 

#ifdef _DEBUG 
#define new DEBUG_NEW 
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__; 
#endif 

CPop::CPop（） 
{ 
      m_PopServer.Create（）; 
} 
//{4EEC1C91-6BE1-11d1-8824-00001C302581} 

CPop::~CPop（） 
{ 
      m_PopServer.Close（）; 
} 

BOOL CPop::Connect（CString & Host, CString & User, CString & Password） 
{ 
      char buf [512]; 

      if （!m_PopServer.Connect（Host,110）） // 110 Pop3 Port 
      { 
            m_ErrorMessage = _T（"Server cannot be connected"）; 
            return FALSE; 
      } 
      else 
      { 
            if（CheckResponse（CONNECTION_CHECK）==FALSE） 
                  return FALSE; 

            wsprintf （buf, "USER %s/r/n", （LPCSTR） User）; 
            m_PopServer.Send（buf, strlen （buf））; 
            if（CheckResponse（USER_CHECK）==FALSE） 
                  return FALSE; 

            wsprintf （buf, "PASS %s/r/n", （LPCSTR） Password）; 
            m_PopServer.Send（buf, strlen （buf））; 
            if （CheckResponse（PASSWORD_CHECK）==FALSE） 
                  return FALSE; 

            return TRUE; 
      } 
} 

BOOL CPop::Delete（int & MsgNumber） 
{ 
      char buf [512]; 

      wsprintf （buf, "DELE %d/r/n",MsgNumber ）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（DELETE_CHECK）==FALSE） 
            return FALSE; 
      else 
            return TRUE; 
} 

BOOL CPop::Disconnect（） 
{ 
      char buf [512]; 

      wsprintf （buf, "QUIT /r/n"）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（QUIT_CHECK）==FALSE） 
            return FALSE; 
      else 
            return TRUE; 
} 

BOOL CPop::Noop（） 
{ 
      char buf [512]; 

      wsprintf （buf, "NOOP /r/n"）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（NOOP_CHECK）==FALSE） 
            return FALSE; 
      else 
            return TRUE; 
} 

// Return the Msg Size for given msg number 
int CPop::GetMessageSize（int MsgNumber） 
{ 
      if（m_SizeOfMsg.GetSize（） < MsgNumber+1） 
            return 0; 
      else 
            return m_SizeOfMsg[MsgNumber+1]; 
} 

BOOL CPop::Reset（） 
{ 
      char buf [512]; 

      wsprintf （buf, "RSET /r/n"）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（RSET_CHECK）==FALSE） 
            return FALSE; 
      else 
            return TRUE; 
} 

// MsgContents will hold the msg body 
BOOL CPop::Retrieve（int MsgNumber） 
{ 
      char buf [512]; 

      wsprintf （buf, "RETR %d/r/n",MsgNumber ）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（RETR_CHECK）==FALSE） 
            return FALSE; 
      else 
      return TRUE; 
} 

BOOL CPop::Statistics（） 
{ 
      char buf [512]; 

      wsprintf （buf, "STAT /r/n"）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（STAT_CHECK）==FALSE） 
            return FALSE; 
      else 
            return TRUE; 
} 

CString CPop::GetMsgContents（） 
{ 
      return m_MsgContents; 
} 

int CPop::GetNumberOfMails（） 
{ 
      return m_NumberMail; 
} 

int CPop::GetTotalMailSize（） 
{ 
      return m_TotalSize; 
} 

BOOL CPop::Connect（） 
{ 
      Connect（m_Host, m_User, m_Password）; 
      return TRUE; 
} 

void CPop::SetHost（CString & Host） 
{ 
      m_Host = Host; 
} 

CString CPop::GetHost（） 
{ 
      return m_Host; 
} 

void CPop::SetUser（CString & User） 
{ 
      m_User = User; 
} 

CString CPop::GetUser（） 
{ 
      return m_User; 
} 

void CPop::SetPassword（CString & Password） 
{ 
      m_Password = Password; 
} 

CString CPop::GetPassword（） 
{ 
      return m_Password; 
} 

BOOL CPop::CheckResponse（int ResponseType） 
{ 
      char buf [1000]; 

      for （int i=0;i<512;i++） 
      buf[i]='/0'; 

      m_PopServer.Receive（buf, sizeof（buf））; 

      switch （ResponseType） 
      { 
            case CONNECTION_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Bad Connection"）; 
                  return FALSE; 
            } 
            break; 

            case USER_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Bad User Name"）; 
                  return FALSE; 
            } 
            break; 
            case PASSWORD_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Bad Password Name"）; 
                  return FALSE; 
            } 
            break; 
            case QUIT_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during QUIT"）; 
                  return FALSE; 
            } 
            break; 
            case DELETE_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during DELE"）; 
                  return FALSE; 
            } 
            break; 
            case RSET_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during RSET"）; 
                  return FALSE; 
            } 
            break; 
            case STAT_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during STAT"）; 
                  return FALSE; 
            } 
            else 
            { 
                  BOOL EmailNumber = TRUE; 
                  for （char *p = buf; *p != '/0'; p++） 
                  { 
                        if （*p == '/t' || *p == ' '） 
                        { 
                              if（EmailNumber == TRUE） 
                              { 
                                    m_NumberMail = atoi（p）; 
                                    EmailNumber = FALSE; 
                              } 
                              else 
                              { 
                                    m_TotalSize = atoi（p）; 
                                    return TRUE; 
                              } 
                        } 
                  } 
            } 
            break; 

            case NOOP_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during NOOP"）; 
                  return FALSE; 
            } 
            break; 

            case LIST_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during LIST"）; 
                  return FALSE; 
            } 
            else 
            { 
                  m_PopServer.Receive（buf, sizeof（buf））; 

                  for （char *p = buf; *p != '.'; p++） 
                  if （*p == '/t' || *p == ' '） 
                  m_SizeOfMsg.Add（atoi（p））; 
            } 
            break; 
            case RETR_CHECK: 
            if （strnicmp（buf,"-ERR", 4） == 0） 
            { 
                  m_ErrorMessage = _T（"Error occured during RETR"）; 
                  return FALSE; 
            } 
            else 
            { 
                  char temp[9000]; 
                  m_PopServer.Receive（temp, sizeof（temp））; 
                  m_MsgContents = temp; 
            } 
            break; 
      } 
      return TRUE; 
} 

CString CPop::GetErrorMessage（） 
{ 
      return m_ErrorMessage; 
} 

BOOL CPop::List（） 
{ 
      char buf [512]; 

      wsprintf （buf, "LIST /r/n"）; 
      m_PopServer.Send（buf, strlen （buf））; 
      if （CheckResponse（LIST_CHECK）==FALSE） 
            return FALSE; 
      else 
            return TRUE; 
}  

