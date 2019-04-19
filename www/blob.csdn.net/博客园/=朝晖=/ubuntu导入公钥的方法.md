# ubuntu导入公钥的方法 - =朝晖= - 博客园
# [ubuntu导入公钥的方法](https://www.cnblogs.com/dhcn/p/7100321.html)
导入公钥的办法：
#方法1：
gpg --keyserver subkeys.pgp[.NET](http://lib.csdn.net/base/dotnet) --recv 6E871C4A881574DE
gpg --export --armor 6E871C4A881574DE | sudo apt-key add -
#方法2：
sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 6E871C4A881574DE

