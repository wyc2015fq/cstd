# OPENFLIXR2.0 你能认真对待的微服务 - qq_42302962的博客 - CSDN博客
2018年06月03日 14:21:52[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：87标签：[微服务																[OpenFLIXR																[KVM																[Docker																[集成化开发](https://so.csdn.net/so/search/s.do?q=集成化开发&t=blog)](https://so.csdn.net/so/search/s.do?q=Docker&t=blog)](https://so.csdn.net/so/search/s.do?q=KVM&t=blog)](https://so.csdn.net/so/search/s.do?q=OpenFLIXR&t=blog)](https://so.csdn.net/so/search/s.do?q=微服务&t=blog)
个人分类：[IT																[c++](https://blog.csdn.net/qq_42302962/article/category/7694890)](https://blog.csdn.net/qq_42302962/article/category/7691858)
现在微服务概念特别火热，最近无意×××看到一个不错的开源项目，OpenFLIXR 2.0，ALL-in-ONE，可以细细体会成熟微服务的项目。之前有个1.0 的版本，就是单一服务。
这个开源的项目，是一个集成的项目，并不仅仅开放源代码，并且直接开放IMG，支持VMware，VirtualBox，KVM等虚拟化，IMG里面直接能跑起来。
这样的做法，免去的很多部署的问题，IMG直接down可以快速部署。OpenFLIXR2.0 所说开源，但是有部分服务是需要收费。
在国内是不能下载的，即使×××速度也是很慢的。我托国外的朋友down了两个IMG。我放在百度云，方便有兴趣的朋友研究。
VMWare与VirtualBox： OpenFLIXR_2.0_VMware_VirtualBox.ova
链接：https://pan.baidu.com/s/1jlEM_iRJxbh7ynpdPoJm7g 密码：h1oj
KVM 的IMG  OpenFLIXR_2.0_KVM_unRAID.img.zip
链接：https://pan.baidu.com/s/1KnFMqJ0PQFH3CkepZUUcww 密码：t7sk
把OpenFLIXR项目用到的开源代码，分享出来，实属强大，即使做其他项目，可借鉴性很强。
|开源项目名称|功能|开发语言|代码位置|
|----|----|----|----|
|htpc|服务器管理软件|python|[https://github.com/Hellowlol/HTPC-Manager](https://github.com/Hellowlol/HTPC-Manager)|
|couchpotato|自动下载器NZB and torrent|python|[https://github.com/CouchPotato/CouchPotatoServer](https://github.com/CouchPotato/CouchPotatoServer)|
|radarr||C#|[https://github.com/Radarr/Radarr](https://github.com/Radarr/Radarr)|
|sickrage||python|[https://github.com/SickRage/SickRage](https://github.com/SickRage/SickRage)|
|headphones|音乐下载器 NZB|python|[https://github.com/rembo10/headphones](https://github.com/rembo10/headphones)|
|sonarr||C#|[https://github.com/Sonarr/Sonarr](https://github.com/Sonarr/Sonarr)|
|mylar|自动CBR/cbz 下载器|python|[https://github.com/evilhero/mylar](https://github.com/evilhero/mylar)|
|librarian|视频作者信息显示模板|python|[https://github.com/DobyTang/LazyLibrarian](https://github.com/DobyTang/LazyLibrarian)|
|Spotweb|usenet协议实现|php|[https://github.com/spotweb/spotweb](https://github.com/spotweb/spotweb)|
|SABnzbd|Binary Newsreader|python|[https://github.com/sabnzbd/sabnzbd](https://github.com/sabnzbd/sabnzbd)|
|nzbget|二进制下载器|C++|[https://github.com/nzbget/nzbget](https://github.com/nzbget/nzbget)|
|qBittorrent|bt客户端|C++/Qt|[https://github.com/qbittorrent/qBittorrent](https://github.com/qbittorrent/qBittorrent)|
|Jackett|开发者帮助|C#|[https://github.com/Jackett/Jackett](https://github.com/Jackett/Jackett)|
|spotmop|前端显示|python|[https://github.com/jaedb/spotmop](https://github.com/jaedb/spotmop)|
|Mopidy|音乐服务器|python|[https://github.com/mopidy/mopidy](https://github.com/mopidy/mopidy)|
|Mopify|Mopdiy客户端|python|[https://github.com/dirkgroenen/mopidy-mopify](https://github.com/dirkgroenen/mopidy-mopify)|
|Moped|HTML5 + JS客户端 Mopdiy|JS|[https://github.com/martijnboland/moped](https://github.com/martijnboland/moped)|
|ComicReader.bundle|Plex Media Server Channel|python|[https://github.com/coryo/ComicReader.bundle](https://github.com/coryo/ComicReader.bundle)|
|Plex|Plex Media Server|deb安装包|[https://www.plex.tv/downloads/](https://www.plex.tv/downloads/)|
|Sub-Zero.bundle|Plex Servr->Subtitle|python|[https://github.com/pannal/Sub-Zero.bundle/](https://github.com/pannal/Sub-Zero.bundle/)|
|RequestChannel.bundle|Plex Channel 管理用户请求|python|[https://github.com/ngovil21/RequestChannel.bundle](https://github.com/ngovil21/RequestChannel.bundle)|
|Webtools.bundle|集成工具Plex Media Server|python|[https://github.com/ukdtom/WebTools.bundle](https://github.com/ukdtom/WebTools.bundle)|
|plexpy|监控，分析，提醒 Plex|python|[https://github.com/Tautulli/Tautulli](https://github.com/Tautulli/Tautulli)|
|Ombi|Plex 前端|C#|[https://github.com/tidusjar/Ombi](https://github.com/tidusjar/Ombi)|
|Home Assistant|Home助手|JS|[https://github.com/home-assistant/home-assistant](https://github.com/home-assistant/home-assistant)|
|Netdata|性能监控|C|[https://github.com/firehol/netdata](https://github.com/firehol/netdata)|
|webmin|系统管理web界面|Perl|[https://github.com/webmin/webmin](https://github.com/webmin/webmin)|
|phpmyadmin|数据库连接工具|php|[https://github.com/phpmyadmin/phpmyadmin](https://github.com/phpmyadmin/phpmyadmin)|
|monit|Unix System 监控工具|C|[https://bitbucket.org/tildeslash/monit/](https://bitbucket.org/tildeslash/monit/)|
|Rtail|日志Web显示工具|CSS/JS|[https://github.com/kilianc/rtail](https://github.com/kilianc/rtail)|
|Pi-hole|网络监控工具（Shell/Web/FTL）|shell/php/C|[https://pi-hole.net/](https://pi-hole.net/)|
|portainer|docker 管理器 UI|CSS/JS/go|[https://portainer.io/](https://portainer.io/)|
|Syncthing|云服务同步|go|[https://syncthing.net/ https://github.com/syncthing/syncthing](https://github.com/syncthing/syncthing)|
|Setup|OpenFLIXR2安装导航|php|[https://github.com/mfrelink/OpenFLIXR2.Wizard](https://github.com/mfrelink/OpenFLIXR2.Wizard)|
|openflixr|OpenFLIXR 多媒体服务器|php|[https://github.com/mfrelink/OpenFLIXR2.LandingPage](https://github.com/mfrelink/OpenFLIXR2.LandingPage)|
|Grav|Web CMS|php|[https://getgrav.org/downloads](https://getgrav.org/downloads)|
![QQ截图20180414215852.png](http://s1.51cto.com/images/20180414/1523714364660281.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![image.png](http://s1.51cto.com/images/20180414/1523714389518222.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
现在微服务概念特别火热，最近无意×××看到一个不错的开源项目，OpenFLIXR 2.0，ALL-in-ONE，可以细细体会成熟微服务的项目。之前有个1.0 的版本，就是单一服务。
这个开源的项目，是一个集成的项目，并不仅仅开放源代码，并且直接开放IMG，支持VMware，VirtualBox，KVM等虚拟化，IMG里面直接能跑起来。
这样的做法，免去的很多部署的问题，IMG直接down可以快速部署。OpenFLIXR2.0 所说开源，但是有部分服务是需要收费。
在国内是不能下载的，即使×××速度也是很慢的。我托国外的朋友down了两个IMG。我放在百度云，方便有兴趣的朋友研究。
VMWare与VirtualBox： OpenFLIXR_2.0_VMware_VirtualBox.ova
链接：https://pan.baidu.com/s/1jlEM_iRJxbh7ynpdPoJm7g 密码：h1oj
KVM 的IMG  OpenFLIXR_2.0_KVM_unRAID.img.zip
链接：https://pan.baidu.com/s/1KnFMqJ0PQFH3CkepZUUcww 密码：t7sk
把OpenFLIXR项目用到的开源代码，分享出来，实属强大，即使做其他项目，可借鉴性很强。
|开源项目名称|功能|开发语言|代码位置|
|----|----|----|----|
|htpc|服务器管理软件|python|[https://github.com/Hellowlol/HTPC-Manager](https://github.com/Hellowlol/HTPC-Manager)|
|couchpotato|自动下载器NZB and torrent|python|[https://github.com/CouchPotato/CouchPotatoServer](https://github.com/CouchPotato/CouchPotatoServer)|
|radarr||C#|[https://github.com/Radarr/Radarr](https://github.com/Radarr/Radarr)|
|sickrage||python|[https://github.com/SickRage/SickRage](https://github.com/SickRage/SickRage)|
|headphones|音乐下载器 NZB|python|[https://github.com/rembo10/headphones](https://github.com/rembo10/headphones)|
|sonarr||C#|[https://github.com/Sonarr/Sonarr](https://github.com/Sonarr/Sonarr)|
|mylar|自动CBR/cbz 下载器|python|[https://github.com/evilhero/mylar](https://github.com/evilhero/mylar)|
|librarian|视频作者信息显示模板|python|[https://github.com/DobyTang/LazyLibrarian](https://github.com/DobyTang/LazyLibrarian)|
|Spotweb|usenet协议实现|php|[https://github.com/spotweb/spotweb](https://github.com/spotweb/spotweb)|
|SABnzbd|Binary Newsreader|python|[https://github.com/sabnzbd/sabnzbd](https://github.com/sabnzbd/sabnzbd)|
|nzbget|二进制下载器|C++|[https://github.com/nzbget/nzbget](https://github.com/nzbget/nzbget)|
|qBittorrent|bt客户端|C++/Qt|[https://github.com/qbittorrent/qBittorrent](https://github.com/qbittorrent/qBittorrent)|
|Jackett|开发者帮助|C#|[https://github.com/Jackett/Jackett](https://github.com/Jackett/Jackett)|
|spotmop|前端显示|python|[https://github.com/jaedb/spotmop](https://github.com/jaedb/spotmop)|
|Mopidy|音乐服务器|python|[https://github.com/mopidy/mopidy](https://github.com/mopidy/mopidy)|
|Mopify|Mopdiy客户端|python|[https://github.com/dirkgroenen/mopidy-mopify](https://github.com/dirkgroenen/mopidy-mopify)|
|Moped|HTML5 + JS客户端 Mopdiy|JS|[https://github.com/martijnboland/moped](https://github.com/martijnboland/moped)|
|ComicReader.bundle|Plex Media Server Channel|python|[https://github.com/coryo/ComicReader.bundle](https://github.com/coryo/ComicReader.bundle)|
|Plex|Plex Media Server|deb安装包|[https://www.plex.tv/downloads/](https://www.plex.tv/downloads/)|
|Sub-Zero.bundle|Plex Servr->Subtitle|python|[https://github.com/pannal/Sub-Zero.bundle/](https://github.com/pannal/Sub-Zero.bundle/)|
|RequestChannel.bundle|Plex Channel 管理用户请求|python|[https://github.com/ngovil21/RequestChannel.bundle](https://github.com/ngovil21/RequestChannel.bundle)|
|Webtools.bundle|集成工具Plex Media Server|python|[https://github.com/ukdtom/WebTools.bundle](https://github.com/ukdtom/WebTools.bundle)|
|plexpy|监控，分析，提醒 Plex|python|[https://github.com/Tautulli/Tautulli](https://github.com/Tautulli/Tautulli)|
|Ombi|Plex 前端|C#|[https://github.com/tidusjar/Ombi](https://github.com/tidusjar/Ombi)|
|Home Assistant|Home助手|JS|[https://github.com/home-assistant/home-assistant](https://github.com/home-assistant/home-assistant)|
|Netdata|性能监控|C|[https://github.com/firehol/netdata](https://github.com/firehol/netdata)|
|webmin|系统管理web界面|Perl|[https://github.com/webmin/webmin](https://github.com/webmin/webmin)|
|phpmyadmin|数据库连接工具|php|[https://github.com/phpmyadmin/phpmyadmin](https://github.com/phpmyadmin/phpmyadmin)|
|monit|Unix System 监控工具|C|[https://bitbucket.org/tildeslash/monit/](https://bitbucket.org/tildeslash/monit/)|
|Rtail|日志Web显示工具|CSS/JS|[https://github.com/kilianc/rtail](https://github.com/kilianc/rtail)|
|Pi-hole|网络监控工具（Shell/Web/FTL）|shell/php/C|[https://pi-hole.net/](https://pi-hole.net/)|
|portainer|docker 管理器 UI|CSS/JS/go|[https://portainer.io/](https://portainer.io/)|
|Syncthing|云服务同步|go|[https://syncthing.net/ https://github.com/syncthing/syncthing](https://github.com/syncthing/syncthing)|
|Setup|OpenFLIXR2安装导航|php|[https://github.com/mfrelink/OpenFLIXR2.Wizard](https://github.com/mfrelink/OpenFLIXR2.Wizard)|
|openflixr|OpenFLIXR 多媒体服务器|php|[https://github.com/mfrelink/OpenFLIXR2.LandingPage](https://github.com/mfrelink/OpenFLIXR2.LandingPage)|
|Grav|Web CMS|php|[https://getgrav.org/downloads](https://getgrav.org/downloads)|
![QQ截图20180414215852.png](http://s1.51cto.com/images/20180414/1523714364660281.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![image.png](http://s1.51cto.com/images/20180414/1523714389518222.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)

