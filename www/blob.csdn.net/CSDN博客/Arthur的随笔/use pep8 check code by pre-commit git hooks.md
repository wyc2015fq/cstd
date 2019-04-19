# use pep8 check code by pre-commit git hooks - Arthur的随笔 - CSDN博客
2012年12月15日 01:15:39[largetalk](https://me.csdn.net/largetalk)阅读数：887
个人分类：[杂七杂八																[linux](https://blog.csdn.net/largetalk/article/category/727912)](https://blog.csdn.net/largetalk/article/category/823511)
一点小玩意
```
$ cat pre-commit
#!/bin/bash
echo '############################'
echo 'pep8 check before git commit'
git status | grep -E 'modified:|new file:|renamed:' | grep '.py$' | awk '{print $NF}' | xargs pep8
if [ $? -ne 0 ];then
    echo -e "\n\e[1;31mexist unclean code, please modify your code to fit require\nor your can use 'git commit --no-verify' to skip this check\e[0m"
    echo '############################'
    exit 1
else
    echo -e "\e[1;32mclean code, nice job!!!\e[0m"
    echo '############################'
    exit 0
fi
```
