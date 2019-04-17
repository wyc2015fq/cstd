# vc实现简单的ftp上传和下载功能 - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年12月21日 10:11:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3945








#include <afxinet.h>
void CFtpHandleDlg::OnOK() 
{
 // TODO: Add extra validation here
 CInternetSession *m_pInternetSession=new CInternetSession(AfxGetAppName(),1,PRE_CONFIG_INTERNET_ACCESS);
 CFtpConnection *m_pFtpConnection;
 try{
  m_pFtpConnection=m_pInternetSession->GetFtpConnection("127.0.0.1","123456","123456");
  if(m_pFtpConnection != NULL)//链接成功
  {
   CFtpFileFind FtpFinder( m_pFtpConnection ) ;
   CString FtpPath="/123/123.xls";
   CString LocalPath ="D://456//456.xls";
   if( !( m_pFtpConnection->GetFile( FtpPath, LocalPath, true, FILE_ATTRIBUTE_NORMAL , FTP_TRANSFER_TYPE_BINARY , 1) ) )
   {
    DWORD dErr = GetLastError() ;
    CString sErr;
    sErr.Format( "下载失败: ERROR %d" ,dErr ) ;
    AfxMessageBox(sErr);
   } 
   FtpPath="/123/456.xls";
   if( !( m_pFtpConnection->PutFile( LocalPath, FtpPath, FTP_TRANSFER_TYPE_BINARY, 1) ) )
   {
    DWORD dErr = GetLastError() ;
    CString sErr;
    sErr.Format( "上传失败: ERROR %d" ,dErr ) ;
    AfxMessageBox(sErr);
   }

  }
 }catch(CInternetException *pEx){
  TCHAR szError[1024];
  if(pEx->GetErrorMessage(szError,1024))
   AfxMessageBox(szError);
  else
   AfxMessageBox("There was an exception");
  pEx->Delete();
  m_pFtpConnection=NULL;
 }
 if(m_pFtpConnection != NULL)
 {
  m_pFtpConnection->Close();
  delete m_pFtpConnection;
 }
 delete m_pInternetSession;
 //CDialog::OnOK();
}



