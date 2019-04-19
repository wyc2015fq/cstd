# Yii2在部署新机器时需要注意的环境问题 - walkabc - 博客园
# [Yii2在部署新机器时需要注意的环境问题](https://www.cnblogs.com/hitfire/articles/6368647.html)
　　最近再部署Yii2项目时，遇到一些问题，主要是环境问题
　　1.有些目录没有写入权限
　　　　因为php-fpm在执行时所属的用户时nobody，所以最好是把assets,runtime,web目录分给nobody用户
```bash;gutter
sudo chown -R nobody assets 
sudo chown -R nobody web
sudo chown -R nobody runtime
```
　　执行上面的命令，使得php-fpm拥有这些目录的使用权限
　　2.php的拓展问题，因为开发环境的编译参数和生产环境的编译参数可能不一致，这样在生产环境执行时会导致服务不正常执行
　　所以最好能把一个标准的安装好执行环境的系统镜像化，这样在以后新机器直接安装此镜像即可。腾讯云和阿里云都支持系统的镜像化

