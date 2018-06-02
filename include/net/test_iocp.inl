#include "mynet.h"
#include "cstd.h"
#include "IOCPModel.inl"
int test_iocp()
{
  CIOCPModel m_IOCP; // 主要对象，完成端口模型
  // 初始化Socket库
  if (false == m_IOCP.LoadSocketLib()) {
    printf(_T("加载Winsock 2.2失败，服务器端无法运行！"));
  }
  // 设置本机IP地址
  printf("IP: %s PORT: %d\n", m_IOCP.GetLocalIP(), DEFAULT_PORT);
  // 绑定主界面指针(为了方便在界面中显示信息 )
  m_IOCP.SetMainDlg(0);
  // 开始监听
  if (false == m_IOCP.Start()) {
    printf(_T("服务器启动失败！"));
    return 0;
  }
  m_IOCP.Stop();
  return 0;
}

