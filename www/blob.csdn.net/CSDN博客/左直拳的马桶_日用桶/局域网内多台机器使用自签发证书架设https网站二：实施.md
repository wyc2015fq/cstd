# 局域网内多台机器使用自签发证书架设https网站二：实施 - 左直拳的马桶_日用桶 - CSDN博客
2018年08月05日 19:45:19[左直拳](https://me.csdn.net/leftfist)阅读数：1265
前文再续，书接上一回。上一篇文章说了[局域网内多台机器使用自签发证书架设https网站的原理](https://blog.csdn.net/leftfist/article/details/81414603)，现在接着介绍实现方法和步骤：
```
1、以一台机器作为根证书服务器（其实就是CA），负责生成根证书
2、所有网站的证书，都由这张根证书来签发
3、客户端导入这张根证书到自己的“受信任的根证书颁发机构”里
```
**一、生成根证书**
生成根证书应该有许多种：
1、如果操作系统是server，可以**搭建CA服务器**
参考文章：《[搭建CA服务器](https://blog.csdn.net/Microsoft_wu/article/details/46521023)》
我搭建了，但因为刚开始的时候思路没有整理清晰，乱试一气，没有试成功。但现在想起来，应该也可以的
**2、用makecert.exe**
这是微软为WIN7或以前的操作系统准备的证书生成工具，挺好用的。现在WIN10和win2016 server之后用的是一个叫：New-SelfSignedCertificate 的工具，在power shell里面运行的命令。名字真长，看上去就头大。各种参数也很长，完全不像一个命令行工具。微软的好多东西，真是越做越烂。我因为没有WIN10和win2016 server，所以就没试。其实win2012 server也有这个工具，但参数极少，完全没办法用。
makecert.exe 生成根证书的方法是：
```
makecert -r -pe -n "CN=WebSSLTestRoot" -b 08/01/2018 -e 08/01/2054 -ss root -sr localmachine -len 2048
```
这样就生成了一个名为“WebSSLTestRoot”的根证书，这个根证书有效期为2018-08-01到2054-08-01（注意日期一定要双位，不足则补零，否则语法报错），并且安装到本机“受信任的根证书颁发机构”里去。（详见拙作《[https的证书](https://blog.csdn.net/leftfist/article/details/72672755)》）
但是，makecert也许是版本太旧了，它生成的证书没有 使用者备用名称（subject Alternative Name）这个属性。
缺乏这个属性，IE没有问题，但新版本的谷歌浏览器（Chrome 58 及以上版本）是根据这个属性来判断身份的，没有的话就会说“服务器无法证明它就是 * * *”，因而标为不安全，报NET::ERR_CERT_COMMON_NAME_INVALID 的错。
对根证书来说，没有subject Alternative Name，应该没有影响，但网站证书的话必须有，所以，makecert已经不能再用了，至少不能用于生成网站证书。详见参考资料：
[错误：“主题备用名称缺失”或 NET::ERR_CERT_COMMON_NAME_INVALID 或“您的连接不是私密连接”](https://support.google.com/chrome/a/answer/7391219?hl=zh-Hans)
现在要隆重介绍第三种工具
**3、itisscg.exe**
这是一个用.NET开发的证书生成工具，图形界面。这个工具小巧玲珑，只有200多K，需要.NET4.6的支持。
官方网址：[http://www.itiverba.com/en/software/itisscg.php。](http://www.itiverba.com/en/software/itisscg.php%E3%80%82)
目前官网不可访问，可到CSDN资源页进行下载：
[https://download.csdn.net/download/leftfist/10843701](https://download.csdn.net/download/leftfist/10843701)
生成根证书步骤：
1）输入根证书的名称Subject，存储位置为本机（区别于当前用户）
![这里写图片描述](https://img-blog.csdn.net/20180805184514566?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Subject Alternative Name不用填（由此可知，用makecert.exe来生成根证书应该也是可以的）
2）
![这里写图片描述](https://img-blog.csdn.net/20180805185036408?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3）
![这里写图片描述](https://img-blog.csdn.net/20180805185423456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这样设置，按下生成按钮后，这个根证书，会在本机的个人证书里生成，然后自动拷贝一份到 “受信任的根证书颁发机构” 里。
本来根证书存在于“受信任的根证书颁发机构” 就足够了，不过“本机”的“个人证书”里这份也不要删掉。因为用来签发网站证书的时候要用到。
另外，生成证书的时候，系统会弹出这个证书给我们预览，预览界面下面有个【安装证书】按钮，不用理会，不必再点击。
参考官方指导方法
[9. Example: Create a “CA Certificate”:](http://www.itiverba.com/en/software/itisscg.php#9)
**二、生成网站证书**
有了根证书，就可以用于签发网站证书了。签发的网站证书导入网站服务器，然后客户端导入根证书，则客户端的浏览器就能够信任网站了。就是这么简单。
如上所述，生成网站证书不能用makecert.exe，要用itisscg.exe：
1）输入subject 和 subject alternative name，存储位置为本机（区别于当前用户）
![这里写图片描述](https://img-blog.csdn.net/2018080519121640?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）
选择根证书进行签发
![这里写图片描述](https://img-blog.csdn.net/20180805191353186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3）
我的情况是，网站跟CA不是同一台机，签发的网站证书没必要装在CA这台机器上，因此将证书输出到文件里，同时删掉本机存储的这个证书。估计它的流程是不管3721，都要先生成到本机证书存储区域。
![这里写图片描述](https://img-blog.csdn.net/20180805191648595?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后将这个证书文件，拷到网站服务器，导入到 “本机”的“个人证书”，然后在IIS里与网站的https绑定即可。方法为：
运行 mmc 命令
![这里写图片描述](https://img-blog.csdn.net/20161128182810320)
添加证书管理单元
![这里写图片描述](https://img-blog.csdn.net/20161128182826660)
![这里写图片描述](https://img-blog.csdn.net/20161128182903071)
![这里写图片描述](https://img-blog.csdn.net/20161128182936021)
![这里写图片描述](https://img-blog.csdn.net/20161128182950974)
鼠标右键，导入网站证书。
然后在IIS里绑定
本来双击这个证书文件，也能安装证书。但安装的路径是“当前账号”而不是“本机”，IIS貌似是访问“本机”，“当前账号”下的证书它看不到。
**三、客户端导入根证书**
将根证书文件拷贝到客户端机器，在“本机”的“受信任的根证书颁发机构” 里导入即可。
完成以上3个步骤，已经大功告成。但是，开发阶段，一个网站往往需要多张证书：
**四、网站多张证书**
依我看，开发过程中，一个网站往往有两张以上的证书。为什么这么说呢？我们开发的时候，访问本机的网站，一般都用localhost，而别人访问我们，肯定要用IP。虽然证书里的subject alternative name 支持多个域名和IP，但如果是localhost与IP混在一起的话，谷歌浏览器支持，IE又不干了，它会警告说这张证书是颁布给别的网站的。所以localhost要与IP分开成两张证书。
![这里写图片描述](https://img-blog.csdn.net/201808051941480?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
相应地，https要分开绑定：
![这里写图片描述](https://img-blog.csdn.net/20180805194332154?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180805194440115?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
