
# 基于ftp实现一个类dropbox文件同步程序 - Multiangle's Notepad - CSDN博客


2016年08月31日 00:02:51[multiangle](https://me.csdn.net/u014595019)阅读数：2391


最近要在实验室和宿舍两头跑，同一台电脑上还有win和linux等多个系统，想要在任何一个地点和平台上继续看之前的文献实在有点麻烦，只能拿U盘来拷，但是我的U盘又很容易掉，万一掉了那辛苦工作的成果可就全没了。你说网盘吧，百度网盘又不合我意，同步太麻烦了。最符合我想法的还是dropbox， 只要把文件丢在文件夹里面，就会自动帮你同步。但是dropbox被封了呀， 有人推荐金山快盘，跑去一看已经关了。其他的文件同步工具基本都是在win下的，不符合我双系统的要求。得，正好手上有个腾讯的学生服务器，那既然没有现成的，就自己写一个呗。
主要步骤如下：
获得一个云服务器 （如果只是局域网内用，可选局域网中一台电脑做主机）
配置ssh，免密码登陆（这一步可以跳过，不影响后面的步骤）
搭建ftp服务器，下载vsftpd并进行配置
对本地和ftp中的内容进行遍历
比对两边遍历结果，选择相关的内容进行上传，下载或删除
由于我之前已经有了云服务器，所以获取云服务器的部分就先跳过了，可以选择腾讯云或者阿里云。两者都有学生计划，照着官网来就行了，很容易的。

---1. ssh和sshfs相关配置（可跳过）

---这一部分不是必须的。

---ssh可以让我们远程登陆主机，方便在云主机里的操作

---。如果可以直接接触到主机（例如在局域网中），那就没有必要配置ssh了。

---sshfs是建立的ssh的基础上的。可以将远程主机中的文件映射到本地文件夹

---，这样就可以像操控本地文件一样操控远程主机中的文件了。

---1.1 ssh登陆云主机

---一般来讲，如果没有特别设置过的话，ssh是可以用密码登陆的,  像下面这样。

---ssh user

---@ip

---ssh -p port user

---@ip

----p表示指定登陆的端口。ssh默认的端口是22。  输入以后，会要求你输入密码，如果输入正确的话，就可以进入云主机了。 输入

---exit

---即可退出。

---但是腾讯的云主机默认是不能用密码进行登陆的，进入 /etc/ssh/sshd_config查看，可以发现其PasswordAuthentication 一项是设为no的。即只可以用rsa私钥进行登陆。

![这里写图片描述](https://img-blog.csdn.net/20160830223355934)
---碰到这种情况， 首先从网站上下载对应的私钥， 假如名字为isa_key, 那么在当前文件夹下，可以通过下面这种方法来登陆

---ssh -i isa_key user

---@ip

---还有一种方法，可以直接不需要密码，也不需要指定私钥文件来登陆。首先使用

---ssh

----keygen

----t

---rsa

---来生成一个秘钥对， 秘钥和公钥。私钥一般放在/home/usr/.ssh文件夹内。其中usr为本地用户自己的名字。然后将公钥内容想办法拷到云主机里面，假设名字为id_rsa.pub. 然后执行

---cat id_rsa

---.pub

--->> /home/remote_usr/

---.ssh

---/authorized_keys

---这行代码的意思是，将id_rsa.pub中的内容附加到 authorized_keys 原有内容之后。>> 表示在原有内容之后附加，而 > 表示将原有内容覆盖。在完成上述工作以后，即可运行

---ssh usr@ip

---来进行登陆了，不在需要输入密码或者指定私钥文件。

---1.2 sshfs的使用

---sshfs的功能之前也提到过了，可以将远程的文件映射到本地，可以像操作本地文件一样操作远程文件。

---使用apt-get可以对sshfs进行安装，装完以后，记得先按照上一节的方法配置好ssh，因为sshfs是在ssh基础上的。

---sshfs的使用方法很简单， 使用

---sshfs usr

---@ip

---:/

---/home/tmp

---即可将ip中以用户usr将根目录/下的文件映射到本地的/home/tmp 目录下。其中的路径可以自由更改。挂载到本地目录以后，效果如下所示

![这里写图片描述](https://img-blog.csdn.net/20160830232756490)
---这样，就可以在本地操作远程文件了。非常的方便。

---如果想取消挂载，可以使用

---sudo

---umount /home/tmp

---来完成。


---2. ftp的搭建和配置

---首先在服务器中使用

---sudo apt-get install vsftpd

---来安装ftp服务器。装完以后应该自动运行了，可以查看

---[ftp://ip](ftp://ip)

---来检测。ip即为服务器的ip。当然了，默认来说，ftp服务器是只能下载，不能上传的，想要实现我之前说的文件同步功能，上传功能是必须要实现的，这就要修改vsftpd的配置文件了。

---vsftpd的配置文件在 /etc/vsftpd.conf 中

---。找到

---write_enable =YES

---项，将前面的注释符去掉，就行了。记得先运行

---service vsftpd restart

---将vsftpd重启一下


---3. python中ftplib的使用

---在python3中，ftplib负责与ftp有关的操作。这部分就来讲讲ftplib

---3.1 登陆与编码

---from

---ftp

---import FTP

---ftp

---= FTP(ip)

---ftp

---.login(user=user,passwd=passwd)

---ftp

---.encoding =

---'utf8'

---如代码中所示，先建立ftp，然后进行登陆。值得一提的是下面的encoding.通过看源码可以发现，

---ftp连接默认的编码是latin1,　而这个latin1是一个隐藏的炸弹

---，在上传中文路径的文件时，就会提示无法解析。因此，需要提前将其编码方式换成utf8

---3.2 显示当前文件夹下的文件信息及名字

---显示当前文件夹下有2中方法，一种是使用ftp.nlst()

---from

---ftp

---import FTP

---ftp

---= FTP(ip)

---ftp

---.login(user=user,passwd=passwd)

---ftp

---.encoding =

---'utf8'

---\# 这部分之后不在出现，出现的ftp默认都是由此获得的

---ftp

---.cwd(

---'/home/ftps'

---)

---\# 指定ftp跳转到/home/ftps 目录下

---print(

---ftp

---.nlst())

---\# 获得/home/ftps目录下的

--->>> [

---'syncdir'

---]

---\# 因为此文件夹下只有syncdir目录，所以只返回一个长度为1的数组

---第二种方法，使用ftp.retrlines(‘LIST’)

---ftp.retrlines(

---'LIST'

---)

--->>>

---drwxrwxrwx

---4

---1001

---1001

---4096

---Aug

---30

---15

---:

---21

---syncdir

---这种方法返回的是比较详细的信息，drwxrwxrwx表示文件的权限，跟chmod得到的结果类似。d表示是文件夹，三个rwx分别表示文件拥有者，所属用户组，以及其他用户的读，写和执行权限。

---值得注意的是，通过源码发现，上述方法是可以加回调函数的。如果没有自己指定回调函数，就会使用默认的print_line函数。看名字也知道是输出了，所以上面的内容尽管没有用print函数，但它还是输出在屏幕上了。那么如果想要将这个结果进行进一步的处理怎么办呢？其实也好办，像下面这样

---content = []
ftp.retrlines(

---'LIST'

---, callback=content.

---append

---)

---print

---(content)
>>>[

---'drwxrwxrwx    4 1001     1001         4096 Aug 30 15:21 syncdir'

---]

---其中的Aug 30 就是最近上传的日期的名字

---3.3 上传，下载和删除文件

---上传文件

---使用 ftp.storbinary()方法来实现

---buffsize =

---1024

---local_file =

---open

---(local_file_path,

---'rb'

---)

---ftp

---.storbinary(

---'STOR '

---+ ftp_file_path, local_file, buffsize)

---ftp

---.

---close

---()
local_file.

---close

---()

---其中local_file_path 和 ftp_file_path 分别为本地的文件路径和想要在ftp服务器中存储该文件的路径。这两个在后面也有提到

---下载文件

---使用ftp.retrbinary()方法来实现

---buffsize =

---1024

---local_file =

---open

---(local_file_path,

---'wb'

---)

---ftp

---.retrbinary(

---'RETR '

---+ftp_file_path, local_file.

---write

---, buffsize)

---还有其他的一些必要功能，罗列如下

---ftp

---.rmd(path)

---\# 删除文件夹

---ftp

---.mkd(path)

---\# 新建文件夹

---ftp

---.

---delete

---(path)

---\# 删除文件

---4. 完成的程序

---该程序规模比较小，所以就没写类，由几个函数组成，重要的主要有

---local_iterator(path)

---\# 遍历本地文件夹

---remote_iterator(conn, path)

---\# 遍历ftp端某个文件夹内所有文件

---pull()

---\# 从服务器端同步到本地端

---push

---()

---\# 从本地端同步到服务器端

---实际使用的时候，只要配置好config文件，就可以直接使用pull和push进行同步了。而在config.py文件内，需要手动填几个变量：

---local_dir       =

---'C:\\Users\\multiangle\\Desktop\\paper'  \# 要同步的本地文件夹

---ftp_ip          =

---你的 ftp 服务器ip

---ftp_user        =

---用户名

---ftp_pwd         =

---密码

---ftp_dir         =

---ftp服务器中放置该文件的位置。

---完整的程序如下。当然了，也可以去我的github页面上直接下载repo:

---[https://github.com/multiangle/FileSynScript](https://github.com/multiangle/FileSynScript)

---from

---ftplib

---import

---FTP

---import

---config

---import

---re

---import

---time

---import

---os

---import

---asyncio

---from

---pprint

---import

---pprint

---import

---logging

---@asyncio.coroutine

---def

---local_iterator

---(path=config.local_dir)

---:

---file_paths = os.listdir(path)

---for

---file_path

---in

---file_paths:
        tmppath = os.path.join(path,file_path)

---if

---os.path.isdir(tmppath):

---try

---:

---yield

---local_stat(tmppath)

---yield

---from

---local_iterator(tmppath)

---except

---:

---pass

---else

---:

---try

---:

---yield

---local_stat(tmppath)

---except

---Exception

---as

---e:

---pass

---def

---local_stat

---(tmppath)

---:

---os_stat = os.stat(tmppath)
    st_mtime = os_stat.st_mtime
    rel_path_list = trans_abs_to_rel(config.local_dir,tmppath,os.sep)
    rel_path =

---"~"

---for

---x

---in

---rel_path_list:
        rel_path +=

---'/'

---+ x

---return

---dict(
        abs_path    = tmppath,
        rel_path    = rel_path,
        mtime       = int(st_mtime),
        size        = os_stat.st_size,
        isdir       = os.path.isdir(tmppath)
    )

---@asyncio.coroutine

---def

---remote_iterator

---(conn, path=config.ftp_dir)

---:

---conn.cwd(path)
    lines = []
    abs_path = conn.pwd()
    conn.retrlines(

---"LIST"

---, lines.append)

---for

---line

---in

---lines:
        conn.cwd(path)
        info = parse_ftp_info(line)
        newpath = path +

---'/'

---+ info[

---'name'

---]
        info[

---'abs_path'

---] = newpath
        rel_path_list = trans_abs_to_rel(config.ftp_dir, newpath,

---'/'

---)
        rel_path =

---'~'

---for

---x

---in

---rel_path_list:
            rel_path +=

---'/'

---+ x
        info[

---'rel_path'

---] = rel_path

---if

---info[

---'isdir'

---]:

---yield

---info

---yield

---from

---remote_iterator(conn, path=newpath)

---else

---:

---\# print(path)

---yield

---info

---def

---parse_ftp_info

---(line)

---:

---ret = {}
    current_year =

---2016

---line = re.sub(

---r"\s+"

---,

---' '

---,line)
    cutted_line = line.split(

---' '

---)

---if

---cutted_line.__len__()>

---9

---:

---\# 如果文件中有空格

---name =

---''

---for

---i

---in

---range(

---8

---,cutted_line.__len__()):
            name += cutted_line[i]+

---' '

---name = name[:-

---1

---]

---else

---:
        name = cutted_line[-

---1

---]
    ret[

---'auth'

---]         = cutted_line[

---0

---]
    ret[

---'name'

---]         = name
    ret[

---'time_str'

---]     =

---'{y}-{m}-{d}-{t}'

---.format( y=current_year,
                                                    m=cutted_line[

---5

---],
                                                    d=cutted_line[

---6

---],
                                                    t=cutted_line[

---7

---])
    ret[

---'mtime'

---]    = int(time.mktime(time.strptime(ret[

---'time_str'

---],

---'%Y-%b-%d-%H:%M'

---)))
    ret[

---'isdir'

---]        =

---'d'

---in

---ret[

---'auth'

---]

---return

---ret

---\# ftp.mkd('TEST')

---def

---trans_abs_to_rel

---(par_p,abs_p,sep)

---:

---par_l = par_p.split(sep)
    abs_l = abs_p.split(sep)
    rel_l = abs_l[par_l.__len__():]

---return

---rel_l

---def

---pull

---()

---:

---print(

---'------------------------------------------------'

---)
    print(

---'start pull at {t}\n'

---.format(t=gen_time()))
    ftp = FTP(config.ftp_ip)
    ftp.encoding =

---'utf-8'

---\# 这一步很重要，如果没有会导致ftp无法解析中文文件名

---ftp.login(user=config.ftp_user,passwd=config.ftp_pwd)
    remote_files = [x

---for

---x

---in

---remote_iterator(ftp)]
    local_files = [x

---for

---x

---in

---local_iterator()]

---\# 建立任务列表

---tasks = pull_task_gen(remote_files, local_files)

---\# 根据任务列表进行更新，上传，删除等操作

---buffsize =

---1024

---for

---task

---in

---tasks:

---if

---task[

---'type'

---] ==

---'update'

---or

---\
                (task[

---'type'

---] ==

---'add'

---and

---not

---task[

---'isdir'

---]):

---\# 普通文件的情况

---local_file = open(task[

---'to_addr'

---],

---'wb'

---)
            ftp.retrbinary(

---'RETR '

---+task[

---'from_addr'

---], local_file.write, buffsize)

---\# 写入本地文件

---elif

---task[

---'type'

---] ==

---'add'

---and

---task[

---'isdir'

---] :

---\# 要建立文件夹的情况

---os.mkdir(task[

---'to_addr'

---])

---elif

---task[

---'type'

---] ==

---'del'

---:
            addr = task[

---'addr'

---]

---if

---os.path.exists(addr):

---if

---task[

---'isdir'

---]:
                    os.rmdir(addr)

---else

---:
                    os.remove(addr)

---else

---:

---raise

---RuntimeError(

---'Unknown task types'

---)
    ftp.close()

---def

---pull_task_gen

---(remote_files, local_files)

---:

---tasks = []
    local_left_files = local_files[:]
    remote_rel_list = [x[

---'rel_path'

---]

---for

---x

---in

---remote_files]
    local_rel_list = [x[

---'rel_path'

---]

---for

---x

---in

---local_files]

---for

---remote_file

---in

---remote_files:
        remote_rel_path = remote_file[

---'rel_path'

---]

---if

---remote_rel_path

---in

---local_rel_list:

---\# 如果本地有该文件

---local_left_rel_paths = [x[

---'rel_path'

---]

---for

---x

---in

---local_left_files]
            local_left_files.pop(local_left_rel_paths.index(remote_rel_path))
            local_file = local_files[local_rel_list.index(remote_rel_path)]

---if

---local_file[

---'isdir'

---]:

---continue

---if

---remote_file[

---'mtime'

---] - local_file[

---'mtime'

---] >

---60

---:

---\# 如果ftp端文件比本地要提前N秒，则进行同步

---task = dict(
                    type        =

---'update'

---,
                    from_addr   = remote_file[

---'abs_path'

---],
                    to_addr     = local_file[

---'abs_path'

---]
                )
                tasks.append(task)

---else

---:

---pass

---else

---:

---\# 如果本地没有文件

---rel_path = remote_file[

---'rel_path'

---]
            rel_path_list = rel_path.split(

---'/'

---)
            rel_path_list = rel_path_list[

---1

---:]
            local_abs_path = config.local_dir

---for

---x

---in

---rel_path_list:
                local_abs_path += os.sep + x
            task = dict(
                type        =

---'add'

---,
                from_addr   = remote_file[

---'abs_path'

---],
                to_addr     = local_abs_path,
                isdir       = remote_file[

---'isdir'

---]
            )
            tasks.append(task)

---for

---left_file

---in

---local_left_files[::-

---1

---]:
        task = dict(
            type    =

---'del'

---,
            addr    = left_file[

---'abs_path'

---],
            isdir   = left_file[

---'isdir'

---]
        )
        tasks.append(task)

---return

---tasks

---def

---push

---()

---:

---print(

---'------------------------------------------------'

---)
    print(

---'start push at {t}\n'

---.format(t=gen_time()))
    ftp = FTP(config.ftp_ip)
    ftp.encoding =

---'utf-8'

---ftp.login(user=config.ftp_user,passwd=config.ftp_pwd)
    remote_files = [x

---for

---x

---in

---remote_iterator(ftp)]
    local_files = [x

---for

---x

---in

---local_iterator()]
    tasks = push_task_gen(local_files, remote_files)
    buffsize =

---1024

---for

---task

---in

---tasks:

---if

---task[

---'type'

---] ==

---'update'

---or

---\
                (task[

---'type'

---] ==

---'add'

---and

---not

---task[

---'isdir'

---]):

---\# 普通文件的情况

---local_file = open(task[

---'from_addr'

---],

---'rb'

---)
            print(task[

---'from_addr'

---])
            ftp.storbinary(

---'STOR '

---+ task[

---'to_addr'

---], local_file, buffsize)

---\#  上传文件

---elif

---task[

---'type'

---] ==

---'add'

---and

---task[

---'isdir'

---] :

---\# 要建立文件夹的情况

---ftp.mkd(task[

---'to_addr'

---])

---elif

---task[

---'type'

---] ==

---'del'

---:
            addr = task[

---'addr'

---]

---if

---task[

---'isdir'

---]:
                ftp.rmd(addr)

---else

---:
                print(addr)
                ftp.delete(addr)
    ftp.close()

---def

---push_task_gen

---(local_files, remote_files)

---:

---tasks = []
    remote_left_files = remote_files[:]
    remote_rel_list = [x[

---'rel_path'

---]

---for

---x

---in

---remote_files]

---for

---local_file

---in

---local_files:
        local_rel_path = local_file[

---'rel_path'

---]

---if

---local_rel_path

---in

---remote_rel_list:

---\# 如果ftp端有该文件

---remote_left_rel_paths = [x[

---'rel_path'

---]

---for

---x

---in

---remote_left_files]
            remote_left_files.pop(remote_left_rel_paths.index(local_rel_path))
            remote_file = remote_files[remote_rel_list.index(local_rel_path)]

---if

---remote_file[

---'isdir'

---]:

---continue

---if

---local_file[

---'mtime'

---] - remote_file[

---'mtime'

---] >

---60

---:
                task = dict(
                    type        =

---'update'

---,
                    from_addr   = local_file[

---'abs_path'

---],
                    to_addr     = remote_file[

---'abs_path'

---]
                )
                tasks.append(task)

---else

---:

---pass

---else

---:

---\# 如果ftp端没有文件

---rel_path = local_file[

---'rel_path'

---]
            rel_path_list = rel_path.split(

---'/'

---)
            rel_path_list = rel_path_list[

---1

---:]
            remote_abs_path = config.ftp_dir

---for

---x

---in

---rel_path_list:
                remote_abs_path +=

---'/'

---+ x
            task = dict(
                type        =

---'add'

---,
                from_addr   = local_file[

---'abs_path'

---],
                to_addr     = remote_abs_path,
                isdir       = local_file[

---'isdir'

---]
            )
            tasks.append(task)

---for

---left_file

---in

---remote_left_files[::-

---1

---]:
        task = dict(
            type    =

---'del'

---,
            addr    = left_file[

---'abs_path'

---],
            isdir   = left_file[

---'isdir'

---]
        )
        tasks.append(task)

---return

---tasks

---def

---gen_time

---()

---:

---t = time.localtime(time.time())

---return

---time.strftime(

---'%Y-%m-%d %H:%M:%S'

---, t)

---\# push()

---\# pull()


