# 设置ArcGIS的数据源 - 左直拳的马桶_日用桶 - CSDN博客
2017年01月17日 19:39:43[左直拳](https://me.csdn.net/leftfist)阅读数：2406
我从别的地方拿到一份现成的地图文档（*.mxd）,在该服务器上运行得好地地，图文并茂，但用我自己机器的arcMap打开就一片空白，啥都没有。
看左边的各个图层目录，图标上都有个粉红色的惊叹号，醒悟过来，应该是没有数据源。
咋为地图加数据源呢？总结如下：
**1、首先在arcCatalog上加“数据库连接”**
![这里写图片描述](https://img-blog.csdn.net/20170117190914599?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我这里的实例，是在另一台上的，其中orcl22是tnsname的名称；
```
ORCL22 =
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.0.22)(PORT = 1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = orcl)
    )
  )
```
sde为表空间；数据库版本为oracle10g。当然按照帮助文档里的写法：`192.168.0.22/orcl` 也是可以的。
问题是，我机器上装的是oracle 12c 64位的服务器版；而我这个arcgis是32位的（？），它只能用32位的oracle 客户端，否则报 无法加载客户端软件的错误。
有关这个32位的oracle客户端的安装及设置，记录如下： 
1）*客户端安装好后，应在path设置其路径*。比如它装在c:\oracle_client_32，则 
path += c:\oracle_client_32;
2）*设置默认的tnsnames.ora文件*。 
为啥呢？因为我先前已经装了一个oracle服务器版，现在又有了一个客户端，那究竟读那边的tnsnames.ora，才能找到我指明的这个”orcl22”？所以应该指定默认的。方法也是在windows的环境变量里设，比如可以这样：
`TNS_ADMIN= c:\oracle_client_32`
3）*重启arcCatalog即可*
以上说起来好像很轻巧，事实上我费了好大周折。刚开始时死活不行，啥都设置了，后来换了一个客户端就好了。谁能想到客户端也有好有坏。
期间问另一个同事，他曾有类似经历，他说他的解决方案是重装arcgis!因为第一次装时，arcgis是装在默认的c:\programes files(x86)\里了，可能arcgis认不了这个路径，所以导致连不上数据库，除此之外，别无异常。当时我一听顿时哭笑不得。幸亏后来证明根本不是这么回事。
**2、在地图文档上添加该数据源**
右击数据源不对的图层（图标前有粉红色惊叹号），选属性 - 源，找到 arccatalog 里设好的源，记住**一定要双击**，这样才能打开，看到相应的表之类，选中返回即可。
终于，我在我的arcmap上看到了有图有文字的地图，就像我们公司许多会GIS的同事一样。
这是我在GIS开发上迈出的一小步。
