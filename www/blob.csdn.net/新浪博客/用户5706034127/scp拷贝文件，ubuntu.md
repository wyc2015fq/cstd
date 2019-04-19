# scp拷贝文件，ubuntu_用户5706034127_新浪博客
|||

scp是[Linux](http://lib.csdn.net/base/linux)下的远程拷贝
命令：
（1）将本地文件拷贝到远程：scp 文件名 用户名@计算机IP或者计算机名称:远程路径 
（2）从远程将文件拷回本地：scp 用户名@计算机IP或者计算机名称:文件名
  本地路径
（3）将本地目录拷贝到远程：scp
-r目录名 用户名@计算机IP或者计算机名称:远程路径
（4）从远程将目录拷回本地：scp-r 用户名@计算机IP或者计算机名称:目录名
  本地路径
scp -r
/home/administrator/test/  root@192.168.1.100:/root/
scp /home/administrator/Desktop/old/driver/test/test.txt
  root@192.168.1.100:/root/
scp -r root@192.168.62.10:/root/  
/home/administrator/Desktop/new/
