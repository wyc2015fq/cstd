# mac MySQL的那些坑之卸载MySQL - weixin_33985507的博客 - CSDN博客
2016年11月22日 09:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：21
常用的第一种方法就是把/usr/local/ 文件夹下的有关 mysql 的两个文件夹都删除了. 附图就应该懂啦~:
至于怎么找到的这两个文件也说一下吧:  Finder右键 > 前往文件夹 > /usr/local/
![1142651-5ffd4d06b1c479b0.png](https://upload-images.jianshu.io/upload_images/1142651-5ffd4d06b1c479b0.png)
/usr/local文件夹
还有一种是参照了好几篇文章发现的, 应该也能卸载干净~:
先停止所有mysql有关进程, 在终端里逐条运行下列语句:
```
sudo rm /usr/local/mysql
sudo rm -rf /usr/local/mysql*
sudo rm -rf /Library/StartupItems/MySQLCOM
sudo rm -rf /Library/PreferencePanes/My*
vim /etc/hostconfig  (and removed the line MYSQLCOM=-YES-)
rm -rf ~/Library/PreferencePanes/My*
sudo rm -rf /Library/Receipts/mysql*
sudo rm -rf /Library/Receipts/MySQL*
sudo rm -rf /var/db/receipts/com.mysql.*
```
*注:*  也将会卸载MySQL的DMG格式安装文件.
希望mac MySQL的那些坑这一系列文章能够帮助大家尽快入门~~
