
# GetDriveType函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:31:03[seven-soft](https://me.csdn.net/softn)阅读数：215标签：[disk																](https://so.csdn.net/so/search/s.do?q=disk&t=blog)[磁盘																](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)[任务																](https://so.csdn.net/so/search/s.do?q=任务&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[网络																](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=测试&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=任务&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=任务&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=disk&t=blog)
经常碰到这样的需求，比如你需要保存一个文件到一个目录里去，这个目录或许是用户指定的目录，那么你就需要确保这个目录是否有效的，这样就需要去测试这个目录是否允许写文件？这个目录是否存在？这个目录是否可写的？或者这个目录是否网络上的目录？要完成这个任务，就得使用函数GetDriveType来完成。GetDriveType函数可以获取目录和盘号的属性。
函数GetDriveType声明如下：
WINBASEAPI
UINT
WINAPI
GetDriveTypeA(
__in_opt LPCSTR lpRootPathName
);
WINBASEAPI
UINT
WINAPI
GetDriveTypeW(
__in_opt LPCWSTR lpRootPathName
);
\#ifdef UNICODE
\#define GetDriveType GetDriveTypeW
\#else
\#define GetDriveType GetDriveTypeA
\#endif // !UNICODE
**lpRootPathName**是目录或盘号的名称。
返回值是目录的属性，有如下值：
DRIVE_UNKNOWN
DRIVE_NO_ROOT_DIR
DRIVE_REMOVABLE
DRIVE_FIXED
DRIVE_REMOTE
DRIVE_CDROM
DRIVE_RAMDISK
调用函数的例子如下：
\#001 //获取目录或磁盘的属性。
\#002  //
\#003  void Disk(void)
\#004  {
\#005         //获取C:目录的属性。
\#006         std::wstring strTest(_T("C://WINDOWS//"));
**\#007        UINT nRes = ::GetDriveType(strTest.c_str());**
\#008         switch(nRes)
\#009         {
\#010         case DRIVE_UNKNOWN:
\#011               OutputDebugString(_T("DRIVE_UNKNOWN/r/n"));
\#012               break;
\#013         case DRIVE_NO_ROOT_DIR:
\#014               OutputDebugString(_T("DRIVE_NO_ROOT_DIR/r/n"));
\#015               break;
\#016         case DRIVE_REMOVABLE:
\#017               OutputDebugString(_T("DRIVE_REMOVABLE/r/n"));
\#018             break;
\#019         case DRIVE_FIXED:
\#020               OutputDebugString(_T("DRIVE_FIXED/r/n"));
\#021             break;
\#022         case DRIVE_REMOTE:
\#023               OutputDebugString(_T("DRIVE_REMOTE/r/n"));
\#024               break;
\#025         case DRIVE_CDROM:
\#026               OutputDebugString(_T("DRIVE_CDROM/r/n"));
\#027               break;
\#028         case DRIVE_RAMDISK:
\#029               OutputDebugString(_T("DRIVE_RAMDISK/r/n"));
\#030               break;
\#031         default:
\#032             break;
\#033         }
\#034
\#035         //判断盘号是否光驱。
\#036         strTest = _T("d://");
**\#037        nRes = ::GetDriveType(strTest.c_str());**
\#038         switch(nRes)
\#039         {
\#040         case DRIVE_UNKNOWN:
\#041               OutputDebugString(_T("DRIVE_UNKNOWN/r/n"));
\#042               break;
\#043         case DRIVE_NO_ROOT_DIR:
\#044               OutputDebugString(_T("DRIVE_NO_ROOT_DIR/r/n"));
\#045               break;
\#046         case DRIVE_REMOVABLE:
\#047               OutputDebugString(_T("DRIVE_REMOVABLE/r/n"));
\#048               break;
\#049         case DRIVE_FIXED:
\#050               OutputDebugString(_T("DRIVE_FIXED/r/n"));
\#051               break;
\#052         case DRIVE_REMOTE:
\#053               OutputDebugString(_T("DRIVE_REMOTE/r/n"));
\#054               break;
\#055         case DRIVE_CDROM:
\#056               OutputDebugString(_T("DRIVE_CDROM/r/n"));
\#057               break;
\#058         case DRIVE_RAMDISK:
\#059               OutputDebugString(_T("DRIVE_RAMDISK/r/n"));
\#060               break;
\#061         default:
\#062               break;
\#063         }
\#064  }


