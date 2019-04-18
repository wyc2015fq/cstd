# ACE文件操作 - 深之JohnChen的专栏 - CSDN博客

2011年12月23日 14:02:52[byxdaz](https://me.csdn.net/byxdaz)阅读数：4218


ACE里的文件操作与平常的Win32里面的文件操作有所不同，这是因为ACE是为网络开发而设计的，里面普遍都遵循了Client/Server模式，这样在操作文件时，ACE把文件看做是一个Socket服务器，而进行文件操作的类(ACE_FILE_IO)被看做是一个Socket Client。

        有了上面的认识，使用ACE的文件操作就容易理解了：

        调用ACE_FILE_IO.send(...)其实就是进行写文件的操作；

        调用ACE_FILE_IO.recv(...)其实就是进行文件的读操作。

       ACE里面的文件操作类主要包括：ACE_IO_SAP、ACE_FILE、ACE_FILE_IO、ACE_FILE_Connector。

       ACE_FILE_Connector是一个用来产生ACE_FILE_IO的类工厂。

      ACE_FILE_IO继承于ACE_FILE，ACE_FILE继承于ACE_IO_SAP。

      ACE_FILE只能够对文件进行一些整体性的操作，如：关闭文件（close）、删除文件（remove/ulink）、获取文件属性（get_info）、设置文件大小（truncate）、定位或获取文件游标位置（seek/position/tell）、获取文件路径（get_local_addr/get_remote_addr）。

      ACE_FILE_IO则能够进行文件的读写操作，如：多个版本的send/recv, send_n/recv_n,  sendv/recvv,  sendv_n/recvv_n

      ACE_FILE_Connector实际是为了使ACE_FILE类族能够符合Connector/Acceptor设计模式而设计的，只是没有相应的Acceptor。

示例代码如下：

#include "ace/OS_main.h"
#include "ace/FILE_Addr.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID(FILE_SAP, client, "client.cpp,v 4.16 2003/11/01 11:15:23 dhinton Exp")

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 3 || argc > 3)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage: %s filename string/n",
                       argv[0]),
                      1);

  ACE_TCHAR *readback = new ACE_TCHAR[ACE_OS::strlen (argv[1]) + 1];
  readback[ACE_OS::strlen (argv[1])] = '/0';

  ACE_TCHAR *filecache = new ACE_TCHAR[1024];

  ACE_FILE_Info fileinfo;
  ACE_FILE_IO cli_file;
  ACE_FILE_IO file_copy;
  ACE_FILE_Connector con;

  if (con.connect (cli_file,
                   ACE_FILE_Addr (argv[1]),
     0,
                   ACE_Addr::sap_any, 0,
     O_RDWR|O_APPEND|O_CREAT,
                   ACE_DEFAULT_FILE_PERMS) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n to %s",
                       "connect",
                       argv[1]),
                      -1);

  if (con.connect ( file_copy,
     ACE_FILE_Addr ("testfile_cpy.bak"),
     0,
     ACE_Addr::sap_any, 0,
     O_RDWR|O_APPEND|O_CREAT,
     ACE_DEFAULT_FILE_PERMS) == -1)
   ACE_ERROR_RETURN ((LM_ERROR,
     "%p/n to %s",
     "connect",
     "testfile_cpy.bak"),
     -1);

  ssize_t len = ACE_OS::strlen (argv[2]) + 1;

  if (cli_file.send (argv[2], len) != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n",
                       "send"),
                      1);

  if (cli_file.get_info (&fileinfo) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n",
                       "get_info"),
                      1);
  else
    ACE_OS::printf ("fileinfo : mode = %o/nno of links = %lu/nsize = %lu/n",
                    (u_int) fileinfo.mode_ & 0777,
                    ACE_static_cast(u_long ,fileinfo.nlink_),
                    (u_long) fileinfo.size_);

  off_t fpos = cli_file.position ();

  if (fpos == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n",
                       "position"),
                      1);
  else
    ACE_OS::printf ("current filepointer is at %ld/n",
                    (long int) fpos);

  if (cli_file.position (0,
                         SEEK_SET) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n",
                       "position"),
                      1);

  unsigned long lfsize = (u_long) fileinfo.size_;
  if (lfsize <= 1024)
  {
  if (cli_file.recv(filecache, lfsize) != lfsize)
   ACE_ERROR_RETURN ((LM_ERROR,
        "%p/n",
        "recv"),
        1);

  if (file_copy.send (filecache, lfsize) != lfsize)
   ACE_ERROR_RETURN ((LM_ERROR,
       "%p/n",
       "send"),
       1);
  }
  else
  {
   unsigned int uiTemp = lfsize;
   while (uiTemp - 1024 >= 0)
   {
   if (cli_file.recv(filecache, 1024) != 1024)
    ACE_ERROR_RETURN ((LM_ERROR,
         "%p/n",
         "recv"),
         1);
   if (file_copy.send (filecache, 1024) != 1024)
    ACE_ERROR_RETURN ((LM_ERROR,
        "%p/n",
        "send"),
        1);
   uiTemp -= 1024;
   }
  }
  if (cli_file.recv (readback, len) != len)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n",
                       "recv"),
                      1);

  ACE_OS::printf ("read back :%s/n",
                  ACE_TEXT_ALWAYS_CHAR(readback));

  if (cli_file.close () == -1 || file_copy.close() == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p/n",
                       "close"),
                      1);
  return 0;
}

