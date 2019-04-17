# Docker编写脚本实现应用容器重启 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月12日 09:52:31[boonya](https://me.csdn.net/boonya)阅读数：269








Docker编写脚本试下容器重启：支持windows 和Linux。

### 脚本目录结构

![](https://img-blog.csdnimg.cn/20181112094852456.png)

### Windows bat批处理文件：restart.bat

```
@echo off
echo 重启Docker容器:rtvsweb-publish....
  docker restart rtvsweb-publish
  if errorlevel 0 (echo 执行成功) else echo 执行失败
echo 按任意键结束 自动关闭 restart.bat

pause
exit
```

注意：编辑格式要另存为ANSI格式，否则执行可能会报错。

### Linux sh 脚本文件：restart.sh

```bash
#! /bin/bash
echo "重启Docker容器:rtvsweb-publish"
docker restart rtvsweb-publish
if [[ $? -eq 0 ]]; then
	echo "执行成功!"
else
	echo "执行失败!"
	exit
fi
echo "重启Docker容器:rtvsweb-publish 完毕!"
```







