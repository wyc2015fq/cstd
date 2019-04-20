# Windows开机自动启动SVN服务设置 - h13 - 博客园
　　创建autoRunSVN.bat文件，内容是： sc create MySVNServer binpath= "\"C:\Program FilesSubversion\svnserve\svnserve\" --service -r H:\homerepos" displayname= "SVNService" depend= Tcpip start= auto pause
　　注意之处：
　　（1）sc是windows自带的服务配置程序，MySVNServer 是服务的名称，似乎没什么用。
　　（2）参数binPath表示svnserve可执行文件的安装路径，由于路径中的"Program Files"带有空格，因此整个路径需要用双引号引起来。而双引号本身是个特殊字符，需要进行转移，因此在路径前后的两个双引号都需要写成\" 。
　　（3）--service参数表示以windows服务的形式运行，--r指明svn repository的位置，service参数与r参数都作为binPath的一部分，因此与svnserve.exe的路径一起被包含在一对双引号当中，而这对双引号不需要进行转义。
　　（4）displayname表示在windows服务列表中显示的名字， depend =Tcpip 表示svnserve服务的运行需要tcpip服务，start=auto表示开机后自动运行。安装服务后，svnserve要等下次开机时才会自动运行。
　　（5）binPath的等号前面无空格，等号后面有空格 displayname depend start也都一样 service前面是--，不是- ，而r前面是-
　　（6）若要卸载svn服务，则执行 sc delete svnserve 即可。
　　（7）从“sc”到“auto”是在同一个命令sc，必须写在同一行。 创建成功后，可以在运行中键入service.msc察看系统服务，找到SVNService项，查看这项服务的属性，可以使用Windows提供的界面操作SVNService服务了。
　　也可以使用命令行启动、停止服务。
　　启动服务： C:\>net start svnservice SVNService 服务正在启动 . SVNService 服务已经启动成功。
　　停止服务： C:\>net stop svnservice SVNService 服务正在停止. SVNService 服务已成功停止。
　　删除创建的服务 C:\>sc delete svn svnservice
