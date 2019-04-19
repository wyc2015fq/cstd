# ubuntu14.04上面安装ia32-libs - walkabc - 博客园
# [ubuntu14.04上面安装ia32-libs](https://www.cnblogs.com/hitfire/p/4770256.html)
```csharp;gutter
sudo -i
cd /etc/apt/sources.list.d
echo "deb http://old-releases.ubuntu.com/ubuntu/ raring main restricted universe multiverse" >ia32-libs-raring.list
apt-get update
apt-get install ia32-libs
```
