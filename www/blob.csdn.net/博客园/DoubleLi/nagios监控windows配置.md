# nagios监控windows配置 - DoubleLi - 博客园






1、下载并安装windows插件 http://sourceforge.net/projects/nscplus/NSCP-0.4.1.73-x64.msi
2、windows端配置 nsclient.ini文件
    [/modules]
    ;CheckWMI = 1   #注释掉该行
    [/settings/default]
    allowed hosts = *
    ;password =
     port = 12489   #该行为手工添加
3、服务器端配置commands.cfg
 define command{
        command_name    check_nt
        command_line    $USER1$/check_nt -H $HOSTADDRESS$ -p 12489  -v $ARG1$ $ARG2$
        }
      命令行参数可参考： /usr/lib/nagios/plugins/check_nt -h
4、配置hosts.cfg
   define host {
      host_name                  aaa
     alias                       aaa
     address                      192.168.1.190
     contact_groups             ITServices
     check_command           check-host-alive
      max_check_attempts    5
       notification_interval      10
      notification_period        24x7
      notification_options       d,u,r
      }
5、配置services.cfg
#host ---aaa
define service{
        host_name               aaa
        service_description     check-host-alive
        check_command           check-host-alive
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups           ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description     CPU Load
        check_command           check_nt!CPULOAD!-l 5,80,90
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups           ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description     Memory Usage
        check_command           check_nt!MEMUSE!-w 80 -c 90
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups           ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description     OnlineTime
        check_command           check_nt!UPTIME
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups          ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description      C:\ Drive Space
        check_command           check_nt!USEDDISKSPACE!-l c -w 80 -c 90
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups          ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description      D:\ Drive Space
        check_command           check_nt!USEDDISKSPACE!-l d -w 80 -c 90
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups          ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description     W3SVC
        check_command           check_nt!SERVICESTATE!-d SHOWALL -l W3SVC
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups           ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description     Explorer
        retry_check_interval    2
        contact_groups          ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description      C:\ Drive Space
        check_command           check_nt!USEDDISKSPACE!-l c -w 80 -c 90
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups          ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description      D:\ Drive Space
        check_command           check_nt!USEDDISKSPACE!-l d -w 80 -c 90
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups          ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
define service{
        host_name               aaa
        service_description     W3SVC
        check_command           check_nt!SERVICESTATE!-d SHOWALL -l W3SVC
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups           ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
   }
define service{
        host_name               aaa
        service_description     Explorer
        check_command           check_nt!PROCSTATE!-d SHOWALL -l Explorer.exe
        check_period            24x7
        max_check_attempts      5
        normal_check_interval   3
        retry_check_interval    2
        contact_groups           ITServices
        notification_interval   10
        notification_period     24x7
        notification_options    w,u,c,r
        }
5、重启nagios和httpd服务
6、遇见的错误：
  windows下nsclient.log里有“source\nscp\trunk\include\check_nt/server/protocol.hpp:65: Rejected connection from:”，检查nsclient.ini与commands.cfg 接口是否一致









