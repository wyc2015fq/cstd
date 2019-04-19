# PostgreSQL使用笔记 - zwvista - 博客园
## [PostgreSQL使用笔记](https://www.cnblogs.com/zwvista/p/9662498.html)
### 下载并安装
注意安装图形界面 pgAdmin
需要输入缺省用户 postgres 的密码
在 Windows 下安装之后注意把 bin文件夹加到 Path 环境变量中。
### 重置密码
使用管理员权限打开 pg_hba.conf (C:\Program Files\PostgreSQL\9.3\data) 文件
将其中的 md5 改为 trust
然后再修改密码
[I forgot the password I entered during postgres installation](https://stackoverflow.com/questions/10845998/i-forgot-the-password-i-entered-during-postgres-installation)
### 修改密码
启动命令行界面，然后输入 \password
[How to change PostgreSQL user password?](https://stackoverflow.com/questions/12720967/how-to-change-postgresql-user-password)
