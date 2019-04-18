# SERV-U：FTP鲜为人知的技巧 - ljx0305的专栏 - CSDN博客
2008年07月20日 21:52:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：605
#### SERV-U：FTP鲜为人知的技巧
　　现在很多的朋友都用SERV-U做个人FTP的服务器，有关如何使用SERV－U架设服务器的文章很多了，这儿我就不多说了。不过大家不知道注意到了没有，当你登陆很多FTP的时候，会显示一些欢迎信息，比如说显示你的IP，告诉你目前有多少人在使用FTP，带宽是多少等等。。。看起来就比较的专业样子。其实你自己也是可以做的，SERV-U这个软件本身就有这个功能。下面我就说明以下如何在自己的FTP里面加上这些信息。 
　　第一、先建立一个文本文件，随便取一个名字。我们这儿就取message.txt吧。 
　　第二、这个这个文本文件里面加上这些文字 
　　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ 
　　欢迎来到XXX的FTP服务器 
　　你的IP地址是：%IP 
　　目前服务器所在的时间是 %time 
　　已经有 %u24h 个用户在最近24小时访问过本FTP 
　　本FTP服务器已经运行了 %ServerDays 天，%ServerHours 小时 和 %ServerMins 分。 
　　服务器的运行情况： 
　　所有登陆用户数量： %loggedInAll total 
　　当前登陆用户数量： %Unow 
　　已经下载字节数： %ServerKbDown Kb 
　　已经上传字节数： %ServerKbUp Kb 
　　已经下载文件数： %ServerFilesDown 
　　已经上传文件数： %ServerFilesUp 
　　服务器平均带宽： %ServerAvg Kb/sec 
　　服务器当前带宽： %ServerKBps Kb/sec 
　　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ 
　　其中XXX可以改成你的名字。你也可以加上一些你自己认为喜欢的文字，不过要注意的是每行最好不要超过80个字符其中以%开头的都是一些变量，下面是SERV-U能支持的变量时间和日期： 
　　%Time - 显示你的计算机当前时间 
　　%Date - 显示你的计算机当前*期 
　　服务器的统计信息 
　　%ServerDays - 显示服务器已经运行的天数 
　　%ServerHours - 显示服务器已经运行的小时数 
　　%ServerMins - 显示服务器已经运行的分钟数 
　　%ServerSecs - 显示服务器已经运行的秒数 
　　%ServerKbUp - 显示自从服务器运行以来已经上传的字节数 
　　%ServerKbDown - 显示自从服务器运行以来已经下载的字节数 
　　%ServerFilesUp - 显示自从服务器运行以来已经上传的文件数 
　　%ServerFilesDown - 显示自从服务器运行以来已经下载的文件数 
　　%LoggedInAll - 显示自从服务器运行以来已经登陆的用户数 
　　%ServerAvg - 显示服务器的平均带宽 
　　%ServerKBps - 显示服务器的当前带宽 
　　服务器的设定信息 
　　%MaxUsers - 显示服务器能同时登陆的最大用户数量 
　　%MaxAnonymous - 显示服务器能同时登陆的最大匿名用户数量 
　　用户信息 
　　%Name - 显示登陆的用户名 
　　%IP - 显示登陆的用户IP地址 
　　%Dir - 显示登陆的用户的当前目录 
　　%Disk - 显示登陆的用户的当前磁盘 
　　%DFree - 显示登陆的用户的当前磁盘空间，单位是MB 
　　%FUp - 显示登陆的用户上传的文件数量 
　　%FDown - 显示登陆的用户下载的文件数量 
　　%FTot - 显示登陆的用户上传和下载的总的文件数量 
　　%BUp - 显示登陆的用户上传的字节数，单位是KB 
　　%Bdown - 显示登陆的用户下载的字节数，单位是KB 
　　%BTot - 显示登陆的用户上传和下载的总字节数，单位是KB 
　　%TconM - 显示登陆用户连接时间，单位是分钟 
　　%TconS - 显示登陆用户连接时间，单位是秒，要和%TconM一起使用 
　　%RatioUp - 显示登陆用户的上传流量限制 
　　%RatioDown - 显示登陆用户的下载流量限制 
　　%RatioCredit - 显示登陆用户还有多少credit可以上传和下载，这个是针对有些FTP是要上传多少文件，才能下载多少文件而设置的 
　　%QuotaUsed - 显示登陆用户的已经使用了多少空间，单位是KB 
　　%QuotaLeft - 显示登陆用户的还有多少空间可以使用，单位是KB 
　　%QuotaMax - 显示登陆用户的的最大空间，单位是KB 
　　后面3个是针对有磁盘限制的用户设置的 
　　用户数量 
　　%UNow - 显示当前有多少用户连接 
　　%UAll - 显示从服务器运行以来一共有多少用户连接过 
　　%U24h - 显示最近24小时有多少用户 
　　%UAnonAll - 显示当前总的匿名用户数量 
　　%UAnonThisIP - 显示所有匿名登陆的用户数 
　　%UNonAnonAll - 显示所有当前非匿名登陆用户数 
　　%UNonAnonThisIP - 显示所有非匿名登陆用户数 
　　%UThisName - 显示所有使用这个名字登陆的用户数 
　　自己在先建立一个txt文件输入你想要显示的文字，具体参数看上面的内容，然后在Serv_u内的server设置里面，加入这个txt文件就可以了! 
　　下面我已经做了几个！喜欢就把名字等改改OK了！呵呵 
　　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ 
　　欢迎来到＊＊＊A 网＊＊＊的FTP服务器 
　　你的IP地址是：%IP 
　　目前服务器所在的时间是 %time 
　　已经有 %u24h 个用户在最近24小时访问过本FTP 
　　本FTP服务器已经运行了 %ServerDays 天，%ServerHours 小时 和 %ServerMins 分。 
　　服务器的运行情况： 
　　所有登陆用户数量： %loggedInAll total 
　　当前登陆用户数量： %Unow 
　　已经下载字节数： %ServerKbDown Kb 
　　已经上传字节数： %ServerKbUp Kb 
　　已经下载文件数： %ServerFilesDown 
　　已经上传文件数： %ServerFilesUp 
　　服务器平均带宽： %ServerAvg Kb/sec 
　　服务器当前带宽： %ServerKBps Kb/sec 
　　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ 
　　你的信息 
　　用户名：　%Name 
　　IP地址：　%IP 
　　当前目录：　%Dir 
　　当前磁盘：　%Disk 
　　磁盘空间：%DFree 
　　连接时间：　%TconM　分　和　%TconS　秒 
　　上传流量限制：　%RatioUp 
　　下载流量限制：　%RatioDown 
　　－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－ 
　　★★★★★★★★★★★★★★★★★★★★★★★★★ 
　　来自%IP的朋友您好，欢迎来到＋a网＋的个人FTP服务器 
　　本地时间是%Date %Time 
　　服务器已连续工作：%ServerDays天%ServerHours小时%ServerMins分%ServerSecs秒 
　　本站开通以来已接通%UAll位使用者。 
　　过去24小时总共有%U24h次连接。 
　　匿名用户上限为%MaxAnonymous人。每个ip只能开两个进程 
　　目前有%UNow位使用者在线。 
　　其中匿名用户有%UAnonAll人在线。 
　　★★★★★★★★★★★★★★★★★★★★★★★★★ 
　　自本站开通以来共上传/下载 |%ServerKbUpKB/%ServerKbDownKB 
　　自本站开通以来共上传/下载 |%ServerFilesUp个/%ServerFilesDown个 
　　服务器当前流量|%ServerKBps Kb/sec 
　　服务器平均流量|%ServerAvg Kb/sec 
　　★★★★★★★★★★★★★★★★★★★★★★★★★ 
　　你目前的空间容量为 |%QuotaMax/KB 
　　你目前总共使用了 |%QuotaUsed/KB 
　　你目前的空间剩余 |%QuotaLeft/KB 
　　★★★★★★★★★★★★★★★★★★★★★★★★★
