# git 更改远程仓库 - Arthur的随笔 - CSDN博客
2013年04月03日 10:55:02[largetalk](https://me.csdn.net/largetalk)阅读数：1160
basic from: [http://blog.csdn.net/zbunix/article/details/8613648](http://blog.csdn.net/zbunix/article/details/8613648)
1). 从原地址克隆一份裸版本库
git clone --bare git://github.com/username/test.git
2). 然后到新的 Git 服务器上创建一个新项目
**如果是自己服务器，使用git init --bare**
3). 以镜像推送的方式上传代码到 新的 服务器上
cd test.git
git push --mirror git@gitcafe.com/username/test.git
4). 删除本地代码
cd ..
rm -rf test.git
5). 到新服务器上找到 Clone 地址，直接 Clone 到本地
git clone git@gitcafe.com/username/test.git
