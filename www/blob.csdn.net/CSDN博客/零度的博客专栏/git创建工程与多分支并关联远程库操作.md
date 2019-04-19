# git创建工程与多分支并关联远程库操作 - 零度的博客专栏 - CSDN博客
2018年09月04日 12:04:27[零度anngle](https://me.csdn.net/zmx729618)阅读数：198

git初始化工程：
git init       //分支默认为master
git add .
git commit -m '初始化工程'
git remote add origin https://gitee.com/yuntrust/citicpub_zxsy_baidu.git  //关联远程库
git branch --set-upstream-to=origin/master  //设置远程分支流
git pull origin master --allow-unrelated-histories
git push
git checkout -b test
git merge master
git push --set-upstream origin test  //推送当前版本到远程test版本，并且设置流
git checkout -b dev
git merge test
git push --set-upstream origin dev   //推送当前版本到远程test版本，并且设置流
git pull时出现冲突 放弃本地修改，使远程库内容强制覆盖本地代码：
git fetch --all //只是下载代码到本地，不进行合并操作
git reset --hard origin/master  //把HEAD指向最新下载的版本
git push出现冲突了，强制推送本地代码到远程master库：
git push -f origin master  //强制推送本地代码到远程master库
