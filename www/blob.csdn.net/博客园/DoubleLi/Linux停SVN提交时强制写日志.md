# Linux停SVN提交时强制写日志 - DoubleLi - 博客园






Linux下SVN提交时强制写日志


SVN默认可以不写注释提交，有时候可能忘记写注释，有的人也没有写注释的习惯，导致翻看history的时候都不知道做了哪些更改，可以依照以下步骤修改SVN配置，强制提交SVN前写注释



步骤：

1.进入svn/code/hooks目录，在svn版本库的hooks文件夹下面，复制模版pre-commit.tmpl
cp pre-commit.tmpl pre-commit
chmod +x pre-commit

2.编辑pre-commit文件：
将：
$SVNLOOK log -t "$TXN" "$REPOS" | \
grep "[a-zA-Z0-9]" > /dev/null || exit 1
commit-access-control.pl "$REPOS" "$TXN" commit-access-control.cfg || exit 1
这几行注释掉（前面加#符号），
3.在此位置添加如下几行：
#必须填写注释且不少于5个字
# Make sure that the log message contains some text.

#下面这句默认是注释掉的，需要打开注释
SVNLOOK=/usr/bin/svnlook
 
LOGMSG=`$SVNLOOK log -t "$TXN" "$REPOS" | grep "[a-zA-Z0-9]" | wc -c` 
if [ "$LOGMSG" -lt 5 ];#要求注释不能少于5个字符，您可自定义 
then 
  echo -e "注释信息不能为空且不少于5个字!" 1>&2 
  exit 1 
fi
exit 0









