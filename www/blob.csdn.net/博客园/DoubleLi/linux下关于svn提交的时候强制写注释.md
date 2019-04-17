# linux下关于svn提交的时候强制写注释 - DoubleLi - 博客园






在svn版本库的hooks文件夹下面，复制模版pre-commit.tmpl

```
cp pre-commit.tmpl pre-commit
chmod 777  pre-commit
```
- 1
- 2
- 1
- 2

nano 编辑，如下：

```bash
#!/bin/sh
#必须填写注释且不少于5个字
REPOS="$1"
TXN="$2"

# Make sure that the log message contains some text.
SVNLOOK=/usr/bin/svnlook

LOGMSG=`$SVNLOOK log -t "$TXN" "$REPOS" | grep "[a-zA-Z0-9]" | wc -c` 
if [ "$LOGMSG" -lt 5 ];#要求注释不能少于5个字符，您可自定义 
then 
  echo -e "注释信息不能为空且不少于5个字!" 1>&2 
  exit 1 
fi
exit 0
```









