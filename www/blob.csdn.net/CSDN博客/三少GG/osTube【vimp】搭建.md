# osTube【vimp】搭建 - 三少GG - CSDN博客
2012年06月26日 16:59:56[三少GG](https://me.csdn.net/scut1135)阅读数：3561
## CentOS5.3下osTube搭建
前段时间想搞个类似youku，土豆的视频站点，用于学校里的精品课程、学生的视频娱乐之用。因为要用到上传转码，所以配置起来还是比较复杂的。
这篇文章是台湾一位博主所写，是在fedora上的，可以作为参考：[http://plog.hlps.tc.edu.tw/post/1/3296](http://plog.hlps.tc.edu.tw/post/1/3296)，另可以参考：[http://www.oschina.net/question/90475_16725](http://www.oschina.net/question/90475_16725)
1、前置环境配置
似乎也没有特别复杂的配置，用yum -y install 安装了一些包，当然我的系统是完全安装的，基本上需要的东西大都已经安装了。大致如下：
yum -y install mplayer mencoder flvtool2 php-gd
php升级为php5.2.17，升级方法参照：[http://gblog.hbcf.net/index.php/archives/488](http://gblog.hbcf.net/index.php/archives/488)
当然必须的一些组件是已经安装了的，比如ruby，gd，gcc，libjpeg-devel ，lamp，ffmpeg，ffmpeg-php，php-mysql,php-deve等等。
好在CentOS上有yum，缺什么，只要找好了源，我们就可以直接安装。相对时比较方便的。
注：至于有的地方说要php-zend的支持，后来验证发现，zend在这里并不是必须的。
前置环境安装完毕以后一定要检查一下，是否支持mysql数据库等，否则在安装的时候回出现这样那样不知名的错误。比如，在安装过程中点击Next时，就是不往下一页走，在比如在执行初始化数据库时显示空白页等等。
如果出现了类似的状况，请检查是否将环境都按照要求配置得当，再查看是否其它原因导致。
2、修改php.ini文件
-     upload_max_filesize：   
-     這是上傳單一檔案的大小限制，譬如我若允許上傳超過20MB的影片檔，那這數值就必須至少大於「20M」才可以。   
- 
-     post_max_size：   
-     PHP允許一次上傳檔案的大小限制，若我一次要上傳2個15MB的影片檔，那麼這個數值就必須至少大於「30M」了。  
-     這個 post_max_size 必須大於 upload_max_filesize   
- 
-     memory_limit：   
-     執行每個scripts時使用的memory大小限制   
-     這個 memory_limit 必須大於 post_max_size   
- 
- 
- 這三個數值的大小，必須是 memory_limit > post_max_size > upload_max_filesize ，這不是我說的，是php manual上說的，所以要設多大，就看自己的決定了。（設定好記得要 service httpd restart 來重新啟動）  
3、安装程序
安装osTube的程序，由于osTbue从2.6版本以后就被vimp收购，开始闭源，于是我们就下载2.6版本的吧。以便于以后再应用中对源程序进行修改。安装以后就可以使用强大的后台功能进行相应的设置。
### 日志信息 »
该日志于2012-05-15 02:55由 tiandi 发表在[Linux服务器架设](http://gblog.hbcf.net/index.php/archives/category/linux/linux%E6%9C%8D%E5%8A%A1%E5%99%A8%E6%9E%B6%E8%AE%BE)分类下， 你可以[发表评论](http://gblog.hbcf.net/index.php/archives/514#respond)。除了可以将这个日志以保留[源地址](http://gblog.hbcf.net/index.php/archives/514)及作者的情况下[引用](http://gblog.hbcf.net/index.php/archives/514/trackback)到你的网站或博客，还可以通过[RSS
 2.0](http://gblog.hbcf.net/index.php/archives/514/feed)订阅这个日志的所有评论。 
### [重要] CentOS 的 Mplayer 需要重新加入編碼才能轉 flv 格式
皇天不負苦心人！終於找到一個線索了！![](http://www.tiec.tp.edu.tw/lt/js/tinymce/plugins/emotions/images/44.gif)
![](http://www.tiec.tp.edu.tw/lt/js/tinymce/plugins/emotions/images/6.gif)感謝 samba 組長。
[http://home.csjh.tcc.edu.tw/phpbbinf/viewtopic.php?f=6&t=3581&st=0&sk=t&sd=a&start=15](http://home.csjh.tcc.edu.tw/phpbbinf/viewtopic.php?f=6&t=3581&st=0&sk=t&sd=a&start=15)
參考這篇文章 [http://docs.buyscripts.in/Mplayer](http://docs.buyscripts.in/Mplayer)
我稍微調整了一下，匯入最新的解碼器 all-20071007.tar.bz2
> 
cd /usr/local/src
wget http://www.mplayerhq.hu/MPlayer/releases/codecs/all-20071007.tar.bz2
tar jxvf all-20071007.tar.bz2
mkdir -p /usr/local/lib/codecs
cd all-20071007
cp -f *.* /usr/local/lib/codecs/
chmod -R 755 /usr/local/lib/codecs/
ls -l /usr/local/lib/codecs/
ln -sf /usr/local/lib/codecs /usr/lib/codecs
ln -sf /usr/local/lib/codecs /usr/local/lib/win32
ln -sf /usr/local/lib/codecs /usr/lib/win32
cd /usr/local/src
ldconfig
svn checkout svn://svn.mplayerhq.hu/mplayer/trunk mplayer
cd mplayer
make clean && make distclean
./configure --prefix=/usr --codecsdir=/usr/local/lib/codecs/
make && make install
很有耐心的執行每一行程式！  (要花很長的時間哦！)
奇蹟出現了！轉碼成功 ....![](http://www.tiec.tp.edu.tw/lt/js/tinymce/plugins/emotions/images/28.gif)
[vimp 2.0.6安裝全記錄(原osTube)](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393)
5,207 瀏覽數• 星期三, 一月 12th, 2011
新的伺服器來了之後，考量許久還是選VMware的ESXi 4.1，
原因是在動態記憶體的配置上優於Hyper-V，即使原來的x3650有2008的授權，
原本偏好HP的機器但因沒有優規，後了選擇了IBM的x3650 M3，
這幾家大廠其實和VM的搭配現在都沒什麼問題，
所以想要買的人就考慮看看吧！
除了研究上使用，學校也需要另一個影音平台，
原本有一個測試的osTube，但在尋求新的版本時，
才發現原來已經中止，從2010年10月之後都移到現在的VIMP，
當然安裝過程還是要和往例一樣寫個記錄(其實最近都在碎碎唸)，
除了讓有需要的人參考一下，有錯也可以指正，
同時也讓自己留個記錄可以翻閱！
廢話不多說，就開始囉：
假前提是已經裝好ubuntu加上Apache2、PHP5、MysQL和Tomcat6，
選用ubuntu的原因是過去裝osTube這個是最快的，
我用的是10.10 server edition，以下的安裝步驟是由官網的文件來的，
順便加上我安裝過程遇到的問題。
Server準備
apt-get update
apt-get upgrade
apt-get dist-upgrade
如果有更新到kernel請記得重新啟動server
啟用mod_rewrite
a2enmod rewrite
/etc/init.d/apache2 restart
PHP
apt-get install libapache2-mod-php5
apt-get install php5-cli php5-mysql php5-gd php5-xsl php5-curl
MySQL
apt-get install mysql-server
安裝完MySQL後root的密碼別忘了，等會兒會用到。
Transcoding
touch /etc/apt/source.list.d/debian-multimedia.org.list
vim debian-multideia.org.list
加入下列文字
# Debian Multimedia Repository
deb http://www.debian-multimedia.org.sixxs.org stable main non-free
deb-src http://www.debian-multimedia.org.sixxs.org stable main non-free
deb ftp://ftp.debian-multimedia.org stable main non-free
deb-src ftp://ftp.debian-multimedia.org stable main non-free
安裝keyring of debian-multimedia.org
wget http://www.debian-multimedia.org.sixxs.org/pool/main/d/debian-multimedia-keyring/debian-multimedia-keyring_2010.12.16_all.deb
dpkg -i debian-multimedia-keyring_2010.12.16_all.deb
P.S.原來官網上是安裝2008.10.16的版本
apt-get update
apt-get install mplayer mencoder ffmpeg flvtool2
apt-get install w64codecs(如果是32bit system就改成w32codecs)
在這兒我出現安裝問題："w64codecs has no installation candidate"
後來索性去下載w64codecs_20071007-0medibuntu1_amd64.deb，
wget http://packages.sixxs.org.medibuntu.org/pool/non-free/w/w64codecs/w64codecs_20071007-0medibuntu1_amd64.deb
dpkg -i w64codecs_20071007-0medibuntu1_amd64.deb
在官網文件裡有一段說明，如果是要安裝在子目錄下的就可以直接跳過，
建議是建一個VIMP的virtual host，步驟如下：
mkdir -p /var/www/showvid/data
mkdir -p /var/www/showvid/logs
chown -R www-data:www-data /var/www/showvid
建立virtual host設定
cd /etc/apache2/sites-available
touch showvid
vim showvid
輸入下列內容
<VirtualHost *:80>
DocumentRoot "/var/www/showvid/data/web"
ServerName showvid.com
ServerAlias www.showvid.com
<Directory "/var/www/showvid/data/web">
AllowOverride All
RewriteEngine On Options -Indexes FollowSymLinks
</Directory>
ErrorLog /var/www/showvid/logs/error.log
CustomLog /var/www/showvid/logs/access.log
combined LogLevel error
</VirtualHost>
停止原來的default apache2 virtual host
a2dissite default
啟用新的virtual host
a2ensite showvid
/etc/init.d/apache2 reload
PHP設定
將/etc/php5/apache2/php.ini及/etc/php5/cli/php.ini這兩個檔案，
更改下列設定：
upload_max_filesize: 1024M
post_max_size: 1024M
memory_limit: 512M
/etc/init.d/apache2 restart
開一個專用資料庫給VIMP用，
懶人就用phpmyadmin來建比較快，
同時指定一個使用者給VIMP的資料庫。
安裝vimp 2.0.6
cd /var/www/showvid/data
tar xvzf vimp.framework-2.0.6-r18206-community.tar.gz
rm vimp.framework-2.0.6-r18206-community.tar.gz
chmod +x symfony
./symfony framework:init mysql://資料庫使用者帳號:資料庫使用者密碼@localhost/資料庫名稱
done
接下來就可以登入開始使用啦！
下./symfony時錯誤訊息：
*Fatal error: Call to undefined function sg_load() in /var/www/showvid/data/config/ProjectConfiguration.class.php on line 2*
在裝的時候出現這個狀態，搜尋了一下官網才發現，
沒有裝*SourceGuardian PHP extension，*將這PHP extension解壓開在一個暫存的目錄
mkdir /tmp/loader cd /tmp/loader
tar xfz /var/www/showvid/data/data/loader.tar.gz
接著選擇適檔的版本將他複製到PHP的目錄下
cp Linux_x64-32/ixed.5.3.lin /usr/lib/php5/20090926/
再來需要告訴PHP重新載入這個extension
cd /etc/php5/conf.d/
touch sourceguardian.ini
vim sourceguardian
加入下列文字：
[sourceguardian]
zend_extension=/usr/lib/php5/20090926/ixed.5.3.lin
儲存後重新啟動apache2
/etc/init.d/apache2 restart
就可以進行./symfony的指令了~
Category: [Linux](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?cat=8)  | Tags: [esxi](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?tag=esxi), [osTube](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?tag=ostube), [php5](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?tag=php5), [ubuntu](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?tag=ubuntu), [vimp](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?tag=vimp)
You can follow any responses to this entry through the [RSS 2.0](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?feed=rss2&p=393) feed. You can [leave a response](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393#respond), or [trackback](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/wp-trackback.php?p=393) from your own site. 
31 Responses
- 
![](http://1.gravatar.com.sixxs.org/avatar/78c467aa770b1a0a37716e6f891699c1?s=32&d=http%3A%2F%2F1.gravatar.com%2Favatar%2Fad516503a11cd5ca435acc9bb6523536%3Fs%3D32&r=G)
1
Hrefdog
[](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393#comment-6131)
Thanks a lot!
- 
![](http://1.gravatar.com.sixxs.org/avatar/947006ff667bf1e446f80b87ebcd4c1f?s=32&d=http%3A%2F%2F1.gravatar.com%2Favatar%2Fad516503a11cd5ca435acc9bb6523536%3Fs%3D32&r=G)
2
reona
[](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393#comment-6139)
不客氣，希望有幫忙到，如有錯也歡迎指正！
- 
![](http://1.gravatar.com.sixxs.org/avatar/1c3c539566e494cc38e3d2e017ddc684?s=32&d=http%3A%2F%2F1.gravatar.com%2Favatar%2Fad516503a11cd5ca435acc9bb6523536%3Fs%3D32&r=G)
3
lyiwen
[](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393#comment-7127)
PC机跑这个吃力得紧
- 
![](http://1.gravatar.com.sixxs.org/avatar/947006ff667bf1e446f80b87ebcd4c1f?s=32&d=http%3A%2F%2F1.gravatar.com%2Favatar%2Fad516503a11cd5ca435acc9bb6523536%3Fs%3D32&r=G)
4
reona
[](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393#comment-7326)
不建議使用PC，在轉檔的過程會耗費大量的硬體資源，我使用VMware也覺得有些吃力了，即便是使用IBM x3650 M3，所以能有伺服器等級的硬體來執行會比較適當。
- 
![](http://1.gravatar.com.sixxs.org/avatar/d00877e3325d9e9dcaea0ecca6ad5cc5?s=32&d=http%3A%2F%2F1.gravatar.com%2Favatar%2Fad516503a11cd5ca435acc9bb6523536%3Fs%3D32&r=G)
5
Test
[](http://declaim2.zhps.tp.edu.tw.sixxs.org/wp/?p=393#comment-8130)
後面已經可以安裝了
但是還是跑出一堆錯誤訊息 line25 line778 
不支道友沒有方法解決
# 更改vimp默认语言
发表于 [2012 年 3 月 31 日](http://wp4d.sinaapp.com/2012/03/31/%E6%9B%B4%E6%94%B9vimp%E9%BB%98%E8%AE%A4%E8%AF%AD%E8%A8%80/)由 [dpriest](http://wp4d.sinaapp.com/author/dpriest/)[http://wp4d.sinaapp.com/2012/03/31/%e6%9b%b4%e6%94%b9vimp%e9%bb%98%e8%ae%a4%e8%af%ad%e8%a8%80/](http://wp4d.sinaapp.com/2012/03/31/%E6%9B%B4%E6%94%B9vimp%E9%BB%98%E8%AE%A4%E8%AF%AD%E8%A8%80/)
要将vimp的默认语言改成中文，登陆后台选择默认语言为中文即可。
但是每次admin登陆后，显示的语言又变回英文了，因为已注册用户默认也设置了一个默认语言，要在数据表里修改才生效。
修改user表下的culture为zh，以后登陆就会默认以中文登陆啦~
