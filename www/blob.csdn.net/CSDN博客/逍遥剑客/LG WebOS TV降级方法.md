# LG WebOS TV降级方法 - 逍遥剑客 - CSDN博客
2015年03月06日 13:24:25[xoyojank](https://me.csdn.net/xoyojank)阅读数：15661
LG官方的固件更新从来不加什么更新日志, 如果手贱更新了发现少了什么功能就只能认栽
虽然也可以通过USB进行更新, 但是只会识别更高的固件版本号
经过搜索, 发现已经有大神破解出了LG固件的在线更新流程, 大概是这样的:
- 请求snu.lge.com, 提交电视机产品代号
- 根据型号返回对应的固件更新版本数据(XML格式)
- 根据返回的版本数据里的版本号和下载链接下载固件
- 进行系统固件的更新, 重启生效
所以, 想强制更新自定义的版本, 需要伪造一个更新服务器, 大神们提供了个LgDTVUpDater工具, 使用方法如下:
- 下载zip包, 解压到C盘根目录
- 下载你需要更新的固件, 解压到C:/LgDTVUpDater/htdocs目录(假设是xxxx.epk)
- 打到C:/LgDTVUpDater/dns/AcrylicHost.txt文件, 把snu.lge.com改成你本机的IP(下文假设是192.168.1.111)
- 执行C:/LgDTVUpDater/startDNS.bat和C:/LgDTVUpDater/startHTTP.bat
- 在LG电视的网络高级设置中, 把DNS改成你电脑的IP
- 在LG设置中"检查更新", 这时不管什么提示都不要更新
- 打开C:/LgDTVUpDater/htdocs/in_dump.txt, 里面的MODEL_NM-tag就是你的电视机产品代号, 类似于**HE_DTV_GP4H_AFAAABAA**这样的格式
- 在C:/LgDTVUpDater/htdocs/看看有没有以产品代号命名的文件, 没有就把别的复制一份改名
- 打开这个文件(XML格式), 修改这几个字段
- **IMAGE_URL**: 改成http://snu.lge.com
- **IMAGE_NAME**: 你需要更新固件的文件名(xxxx.epk)
- **CDN_URLIP**: 改成http://snu.lge.com/xxxx.epk
- IMAGE_SIZE: 固件的文件大小(可选操作)
- UPDATE_MAJOR_VER/UPDATE_MINOR_VER: 也可以改成对相应的版本号, 但是文件中也有版本号(可选操作)
- **FORCE_FLAG**: 一定要是Y. 意思是不管是什么版本, 强制更新
打开C:/LgDTVUpDater/htdocs/CheckSWManualUpdate.laf, 在里面加入你的产品代号的if分支(如果不会, 8,9步就改EMPTY文件)关闭startDNS.bat和startHTTP.bat, 重新执行在电视机上重新检查更新, 然后就有提示了! 剩下的全是自动化的了, 注意过程中不要让电脑睡眠(时间较长)更新完后建议还原出厂设置
![](https://img-blog.csdn.net/20150306132214845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20150306132127874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20150306132138435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 参考资料
[http://blog.techflaws.org/2014/05/26/firmware-downgrade-for-lg-smart-tvs/](http://blog.techflaws.org/2014/05/26/firmware-downgrade-for-lg-smart-tvs/)
[https://www.avforums.com/threads/4k-lg-ub850v-and-ub950v.1878217/page-79](https://www.avforums.com/threads/4k-lg-ub850v-and-ub950v.1878217/page-79)
[https://www.youtube.com/watch?v=0RmMuJRSttU](https://www.youtube.com/watch?v=0RmMuJRSttU)
[https://www.youtube.com/watch?v=qgPKkYJL9UE](https://www.youtube.com/watch?v=qgPKkYJL9UE)
**LgDTVUpDater**:
[https://yadi.sk/d/UDK5P-kuchH4P](https://yadi.sk/d/UDK5P-kuchH4P)
[https://mega.co.nz/#!jUZWzSBI!fSTwHV_dIfoqFNFtO1-4vXRXwwbVUxRxDzgbnOJbKnA](https://mega.co.nz/#!jUZWzSBI!fSTwHV_dIfoqFNFtO1-4vXRXwwbVUxRxDzgbnOJbKnA)
PS: 猜想可以通过这个方法刷国外的系统固件, 没敢实验...
