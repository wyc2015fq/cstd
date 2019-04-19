# do-release-upgrade升级笔记 - =朝晖= - 博客园
# [do-release-upgrade升级笔记](https://www.cnblogs.com/dhcn/p/10586588.html)
### 事后的总结: 后来可能因为阿里云镜像并不是完全底层无关镜像，do-release-upgrade后的18.04版本在经过一次异常的内核版本升级以后，restart失败，因为是虚机还很难处理，不得不直接更换系统盘。
### 第一个问题
Your python3 install is corrupted. Please fix the '/usr/bin/python3' 
参考：[https://askubuntu.com/questions/1104052/your-python3-install-is-corrupted?newreg=5e411af9bb55493a93c5b16f7ba38b93](https://askubuntu.com/questions/1104052/your-python3-install-is-corrupted?newreg=5e411af9bb55493a93c5b16f7ba38b93)
### 第二个问题
- uwsgi-plugin不对,参考:[https://www.cnblogs.com/dhcn/p/7606210.html](https://www.cnblogs.com/dhcn/p/7606210.html)
### 第三个问题
#### postgresql版本升级过程
- 1. db_dump备份数据库
- 2. apt指令卸载旧的postgresql服务
- 3. apt安装新的
- 4. 端口如果变成5433，可以去配置里面改一下
- 5. bash里面用下面指令建用户名
createuser -s dbuser
- 6. bash里面用这个用户名建db：
createdb -O dbuser mydb
- 7. psql进数据库控制台改密码
postgres=#alter user postgres with password 'new password';
- 8. restore恢复数据库

