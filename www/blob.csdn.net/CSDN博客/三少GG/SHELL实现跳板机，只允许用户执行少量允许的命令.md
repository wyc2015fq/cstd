# SHELL实现跳板机，只允许用户执行少量允许的命令 - 三少GG - CSDN博客
2013年07月19日 15:26:50[三少GG](https://me.csdn.net/scut1135)阅读数：2459
整理自：[http://blog.chinaunix.net/uid-22101889-id-3167454.html](http://blog.chinaunix.net/uid-22101889-id-3167454.html)
**注意：请谨慎使用，到现在为止，还没找到改回去的方法。**
[SHELL实现跳板机，只允许用户执行少量允许的命令](http://blog.chinaunix.net/uid-22101889-id-3167454.html)
两个问题。
第一、很多大公司的服务器都不允许直接登录，而是通过一个跳板机才能登录过去。在跳板机中，通常只能执行几个少数命令(如SSH)，而其他命令是不允许执行的，那么怎样才能实现这个功能呢？
第二、一些小公司，由于服务器比较少，不需要什么跳板机之类的说法，公司的开发运维人员加起来也就那么十几二十人，通常大家都知道root密码，所有人都是直接root登录上去，但是有时有人由于失误，把什么服务弄挂了，这时是肯定抓不到人的，因为所有人都知道密码，那么怎样才能知道谁都执行过什么命令呢？或者干脆只让他们执行少数允许的命令呢？
这两个需求，看似不同，其实原理都相同，只要在用户与shell之间加入个检测机制就可以了，您可以去网上找些开源软件，但是如果您嫌那些软件太复杂，又不能完全满足您的需求，那么就完全可以像我这样，自己用shell写一个简易的模拟终端。
在模拟终端里，你需要做的，就是向用户提供一个看起来像是真的但实际不是真的、的虚拟终端。而你需要做的，就是读取用户输入的命令，并在替用户执行之前进行记录，这样就实现了上面第二个需求中的记录功能。如果你不想让用户执行哪些命令，那么仅仅echo一句“Permission denied”就可以将用户阻挡在真正的shell大门之外了。
如果用户要执行的命令是你允许的，那么在执行之前，记录个log就行了。如果将这个虚拟终端设置成只能执行ssh这个命令，那么不就实现了第一个问题中的跳板机的功能了吗。
所以以上两个需求，都可以总结为一个需求，那就是：拦截用户的输入，在用户与shell之间加一道门。
而脚本的功能，就是根据用户不同的输入采取不同的动作。
脚本的长短也就跟需要的命令的多少有关，需要的命令越多，脚本需要判断的就越多，脚本也就越长。
但实际并不需要为每个命令都写一个action，只要为每个种类的写一个action就行了。
比如：最简单的情形，没有任何参数的命令，如：pwd等。
稍复杂些的，如touch、ls等命令，这时需要处理参数。
再复杂些的，如rm、mv等命令，不只需要处理参数，还要避免用户误删数据。
而以上所有命令，都需要判断用户是否有权限等等。
脚本的复杂程度，完全取决于你的需求了，下面就贴一个我写的虚拟终端。
在该终端内、实现了用户可以执行cd ls rm mv download upload pwd passwd等等几个命令。
如果您只实现ssh命令，那就是跳板机了。
代码如下：
点击(此处)折叠或打开
- 
# ~/virtual terminal
- 
- 
- 
# 将此文件放在/etc/profile.d/目录下，并添加可执行权限。
- 
# 如： -rwxr-xr-x 1 root root 7340 Oct 23 18:12 /etc/profile.d/vt.sh
- 
- 
############################################################################
- 
# 本行直至文件末尾的代码，用于实现用户登录使用的虚拟终端
- 
# 请勿删除或更改
- 
# 如有疑问、请联系维护负责人： xiaoxi227（QQ451914397）
- 
############################################################################
- 
- 
# 该文件存放用户名和密码 文件格式为==> 用户名:密码 （每行一个用户）
- 
passwd_file=/etc/user.password 
- 
- 
function red()
- 
{
- # 以红色显示
- echo -e "\033[31;40m$*\033[0m\n"
- 
}
- 
- 
function green()
- 
{
- # 以绿色显示
- echo -e "\033[32;40m$*\033[0m\n"
- 
}
- 
- 
# 忽略所有信号，以免用户使用Ctrl+C或者Ctrl+Z等操作退出Virtual Terminal(VT)
- 
for signal in `seq 1 64`
- 
do
- trap : $signal &> /dev/null
- 
done
- 
- 
clear
- 
- 
function getchar()# 关闭回显，用于输入密码
- 
{
- stty cbreak -echo
- dd if=/dev/tty bs=1 count=1 2>/dev/null
- stty -cbreak echo
- 
}
- 
- 
###############################################################################
- 
green "登录成功。" # 用户通过unix pam认证以后显示“登陆成功”，随后进行身份验证
- 
while :
- 
do
- read -p "请输入用户名:" username
- echo -n "请输入密码: "
- 
- while :
- do
- ret=$(getchar)
- if [ x"$ret" = x"" ];then
- echo
- break
- fi
- passwd="$passwd$ret"
- echo -n '*'
- done
- correct_passwd=$(gawk -F: "/$username/{ print \$2 }" $passwd_file)
- 
- if [ -z "$username" -o -z "$passwd" ];then
- clear
- red "用户名或密码不能为空"
- continue
- fi
- input_password=$(echo $passwd | md5sum | gawk '{ print $1 }')
- if [ x"$input_password" != x"$correct_passwd" ];then
- clear
- red "用户名或密码错误，请重新输入"
- else
- break
- fi
- 
done
- 
- 
title="
- 
##################################################################################
- 
############################## Virtual Terminal ##################################
- 
##################################################################################
- 
- 
cat | more <<EOF
- 
$title
- 
- 
使用说明:
- 
- 
欢迎使用虚拟终端(Virtual Terminal,以下简称VT).
- 
在此模式下，用户只能执行少量被允许执行的命令，其他命令将会被拒绝执行。
- 
本模式支持的命令及功能如下:
- 
- 
cd <DIR>
进入<DIR>目录，前提是用户有进入该目录的权限。
- 
ls功能和系统中的ls命令相同，但是只能显示有读取权限的目录下的文件。
- 本命令支持参数，系统中ls命令可以使用的选项都可以在VT模式下使用。
- 
mv <SRC> <DST> 将文件<SRC>移至<DST>，如果路径相同则含义为改名。
- 用户必须同时对<SRC>和<DST>有读写权限。
- 
rm <FILE> 
将<FILE>从系统中删除。
- 当<FILE>是目录时，会递归删除所有子目录及子目录下的文件。
- 
pwd 
显示当前的工作路径。
- 
download <FILELIST> 
- 将<FILELIST>下载至本机
- 运行此命令时，用户使用的终端必须支持ZModem协议，如SecureCRT等。
- 下载多个文件需将各文件用空格隔开。
- 如果下载的文件是目录，系统会自动将其打包后再下载。
- 
upload 
向系统中上传文件，运行此命令后，终端会弹出一个对话框，按提示选择文件上传即可。
- 注意：运行此命令时，用户使用的终端必须支持ZModem协议，如SecureCRT等。
- 
passwd 
修改自己的登录口令
- 
- 
如需其他功能，请与管理员联系，VT维护负责人：xiaoxi227(QQ:451914397)
- 
$title
- 
- 
EOF
- 
- 
echo -e "当前登录用户是:\033[32;40m$username\033[0m"
- 
- 
logfile=/var/log/vt.log
- 
- 
function run()# 本函数用于替用户执行命令，并记录log
- 
{
- local command="$@"
- {
- echo -n -e "\033[32;40m$username\033[0m\033[7G -- "
- echo -n -e "\033[31;40m`date '+%Y-%m-%d %H:%M:%S'` -- \033[0m$command"
- echo
- } | sed 's/sz/download/g;s/rz/upload/g' >> $logfile
- $command
- 
}
- 
- 
- 
# 通常开发人员需要部署的程序都在同一目录下，所以为其指定根目录，限定仅在该目录有权限。
- 
ROOT_DIR=/usr# 本例中用/usr目录测试
- 
- 
cd $ROOT_DIR# 进入用户的“根目录”
- 
- 
while :
# 死循环读取用户输入
- 
do
- # 这里的主机名写成“localhost”写死了，您也可以写成$HOSTNAME,这样就会显示主机名了。
- read -e -p "[${username}localhost ${PWD##*/}]$ "
- # 如上语句模拟产生shell提示符（其实是假的）
- 
- # 分隔命令和参数
- COMMAND=$(echo $REPLY | gawk '{ print $1 }')
- OPTION=$(echo $REPLY | sed -r "s/^$COMMAND//g;s/^[[:space:]]+//g")
- 
- # 判断要执行的命令是什么，根据不同命令设置不同的动作。如下是本例中实现的几个命令：
- case $COMMAND in
- cd|ls)
- # 当执行cd命令且参数为空时，默认cd到根目录，行为要与系统中的cd命令保持一致。
- if [ x"$OPTION" = x"" -a x"$COMMAND" = x"cd" ];then
- cd $ROOT_DIR
- continue
- fi
- # 判断参数的第一个字符是不是"/",如果不是，说明想要cd或ls的是相对路径。
- # 也就是当前目录下的文件活目录，此时用户是有权限的。
- first_char=${OPTION:0:1}
- if [ x"$first_char" != x"/" ];then
- run $REPLY
- else
- # 如果是绝对路径，那么判断路径是否以$ROOT_DIR开头，若否则提示用户没权限。
- echo $OPTION | grep -q ^$ROOT_DIR
- if [ $? -eq 0 ];then
- run $REPLY
- else
- echo "$COMMAND: Permission denied." >&2
- fi
- fi
- ;;
- # 其他支持的参数：rm、mv、pwd、passwd等等，就不写注释了，自己理解下吧。
- rm|mv)
- if [ x"$COMMAND" = x"rm" ];then RM='-rf';else RM='';fi
- run='ok'
- for _file in $OPTION
- do
- echo $_file | grep -q '-'
- [ $? -eq 0 ] && { 
- red "本命令在VT模式下不支持参数"; 
- continue;
- }
- 
- echo $_file | grep -q ^$ROOT_DIR
- ret=$?
- first_char=${_file:0:1}
- if [ x"$first_char" = x"/" ];then
- if [ $ret -ne 0 ];then
- run='not ok'
- fi
- fi
- done
- 
- if [ x"$run" = x"ok" ];then
- run $COMMAND $RM $OPTION
- else
- echo "$COMMAND: Permission denied." >&2
- fi
- ;;
- pwd)
- run $REPLY
- ;;
- upload)
- run rz
- ;;
- download)
- for _file in $OPTION
- do
- if [ ! -d $_file ];then
- echo "开始下载文件:$_file"
- run sz $_file
- [ $? -eq 0 ] && green "下载完成" || red "下载失败"
- else
- echo "${_file}是目录，正在将其打包成${_file}.tar"
- tar -cf ${_file}.tar $_file
- [ $? -eq 0 ] && green "打包完成，开始下载${_file}.tar" \
- || { red "打包失败，无法下载";continue; }
- run sz ${_file}.tar
- [ $? -eq 0 ] && {
- green "下载完成"
- echo "正在删除打包文件"
- rm -rf ${_file}.tar
- [ $? -eq 0 ] && green "删除成功" \
- || red "删除失败"
- } || red "下载失败"
- fi
- done
- ;;
- passwd)
- read -e -s -p "请输入旧密码:" old_passwd
- echo '******'
- old_passwd=$(echo $old_passwd | md5sum | gawk '{ print $1 }')
- if [ x"$old_passwd" != x"$correct_passwd" ];then
- red "密码输入错误"
- continue
- else
- read -e -s -p "请输入新密码:" new_passwd1
- echo '******'
- read -e -s -p "请重复新密码:" new_passwd2
- echo '******'
- if [ x"$new_passwd1" != x"$new_passwd2" ];then
- red "两次输入的密码不一致"
- continue
- else
- new_passwd=$(echo $new_passwd1 | md5sum | gawk '{ print $1 }')
- sed -i "/$username/s/.*/$username:$new_passwd/g" $passwd_file
- fi
- fi
- ;;
- "")
- continue
- ;;
- *)
- echo "-bash: $COMMAND: command not found" >&2
- ;;
- esac
- 
- 
done
执行结果如下：
![](http://blog.chinaunix.net/attachment/201204/9/22101889_133396893612v3.jpg)
由于时间仓促，匆匆写完，连语法都没检查，脚本中有些bug没有修复，故本文仅仅提供思想，不保证所有功能都能正常使用。有此需求的朋友可按此方法自己定制脚本。
