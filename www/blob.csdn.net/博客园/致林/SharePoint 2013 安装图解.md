
# SharePoint 2013 安装图解 - 致林 - 博客园






# [SharePoint 2013 安装图解](https://www.cnblogs.com/bincoding/p/5707587.html)
转自： http://www.cnblogs.com/jianyus/archive/2013/02/01/2889653.html
**介绍：**文章就是SharePoint2013安装过程的图解，包括步骤截图和简单说明还有安装后的小问题，如果你已经安装完成，并没有问题，请跳过本文章，开始你的SharePoint之旅，谢谢。
**安装环境：**
Window Server 2012 DataCenter + Microsoft SQL 2012（sp1） + SharePoint Server 2013
**简单介绍：**
A、安装Windows2012操作系统（VMWare 9.0中）
B、简单配置：改机器名、可以上网、禁用IE增强配置、静态IP、IIS、FM3.5、AD（其间安装DNS）
C、安装SQL2012
D、在线安装必备程序
E、安装SharePoint Server 2013
F、解决安装后遇到的两个小问题
**安装过程：**
1、首先安装操作系统，如下图Windows Server 2012的安装程序
![image](https://images0.cnblogs.com/blog/256729/201302/01224432-1267437433eb4b44ba36ebd26e054b19.png)
2、选择带有GUI（图形化界面）的数据中心版本，这个标准版也可以。
![image](https://images0.cnblogs.com/blog/256729/201302/01224434-91c8b05643874c68b5ad5633e2a1706b.png)
3、复制文件等待安装，耐心等待，安装系统大学半小时多一点。
![image](https://images0.cnblogs.com/blog/256729/201302/01224436-d26566c7930a4e3194582996371205bd.png)
4、安装好了的“服务器管理器”，我们添加角色和功能，都是需要在这里进行添加。
![image](https://images0.cnblogs.com/blog/256729/201302/01224439-58f203626b2047f58c99c49c7e2a2456.png)
5、安装虚拟机工具，我用的是VMWare WorkStation 9.0版本，因为其他版本没有window2012的选项。
![image](https://images0.cnblogs.com/blog/256729/201302/01224442-32a6b03b95b7414a8a5c3498176e6532.png)
6、前期准备，修改机器名，否则搭建好了AD，安装完毕后，修改机器名会遇到一些列问题。
![image](https://images0.cnblogs.com/blog/256729/201302/01224445-fac5ef31a3eb4b4ba5f326d396d2ca99.png)
7、配置网络，使其可以访问外网，我使用的是NAT方式，当然，不一样的环境可以使用Host或者网桥方式，我是通过主机上网，so选择NAT。
![image](https://images0.cnblogs.com/blog/256729/201302/01224449-0ece866895cf41528f61d3a206e0dff0.png)
8、关闭IE增强配置，感觉很无聊，每次上网都需要添加安全区域。
![image](https://images0.cnblogs.com/blog/256729/201302/01224452-999637ad12de4b9ab3ad86966f3ddb31.png)
9、配置通了网络连接，可以访问百度首页（对灯发誓，绝对没有给百度做广告的意思）。
![image](https://images0.cnblogs.com/blog/256729/201302/01224454-923e320a474143fcba522cd814f0d738.png)
10、配置静态IP地址，因为是服务器么，ip地址总变，别人怎么找到你，何况还是AD和Dns。
![image](https://images0.cnblogs.com/blog/256729/201302/01224457-c2275201742f4562aa83144681bbdf51.png)
11、根据IPconfig出来的网络配置，配置静态ip。
![image](https://images0.cnblogs.com/blog/256729/201302/01224501-9d5ad40c0ce649378f4275fedff5fdd3.png)
12、添加fm3.5功能，不要这样添加，可能因为windows2012自带4.5的原因，这样添加会报错，不能添加，我们使用下一条，命令行添加的方式。
![image](https://images0.cnblogs.com/blog/256729/201302/01224504-63d10ba0fc4c41fd958e5be2c63f881c.png)
13、用命令行读取系统盘的文件，添加fm3.5，命令如下，source就是fm3.5安装文件的位置。
dism.exe /online /enable-feature /all /featurename:NetFX3 /Source:e/:source/sxs
![image](https://images0.cnblogs.com/blog/256729/201302/01224507-db17d30329cd42e48ecaf4b6b6526178.png)
14、安装IIS，建议一个个安装，否则安装过程出错，是一件很麻烦的事儿。
![image](https://images0.cnblogs.com/blog/256729/201302/01224510-62236c70cc1b4f41a0decd1e8bd63535.png)
15、我只多勾选了windows身份认证，因为众所周知，sharepoint是需要windows身份认证的。
![image](https://images0.cnblogs.com/blog/256729/201302/01224513-7f6e262ca6d94ee0ae10b284087d312a.png)
16、安装AD环境，添加功能，基本就是下一步。
![image](https://images0.cnblogs.com/blog/256729/201302/01224517-82fa2ffb17d24b54954b65d0ac5b0526.png)
17、准备下一步，就可以了。
![image](https://images0.cnblogs.com/blog/256729/201302/01224520-96cbb6d3bc664f4f89c9533acfcf008d.png)
18、功能安装完成，准备提成为域控。可以直接点蓝字，将此服务器提升为域控制器。
![image](https://images0.cnblogs.com/blog/256729/201302/01224523-15869d7f9302481fb94f6751582fcaab.png)
19、如果上一步，不小心关了，就打开服务器管理器，找到下面地方，进行提升域控操作。当然，我是单一服务器环境，如果你是场的环境，不需要每台提成域控，只要加入AD就可以咯。
![image](https://images0.cnblogs.com/blog/256729/201302/01224526-d1d2279cfb72496e8dc05fd0c4155fb9.png)
20、点击红圈圈的链接就好了。
![image](https://images0.cnblogs.com/blog/256729/201302/01224529-81aff0f0742640f1ad7b2121ad9adedc.png)
21、给我们的AD起个名字，别太长了啊，因为以后我们时常要敲域的名字。
![image](https://images0.cnblogs.com/blog/256729/201302/01224532-1e05b5b5c3ef47738d47a5001413e5b9.png)
22、输入还原密码，将来备份、还原AD的时候，都需要这个密码，要牢记！
![image](https://images0.cnblogs.com/blog/256729/201302/01224534-bd6b3ad8b6b143c99044fe0148c77dc7.png)
23、无法创建该DNS服务器的委派，是因为我们没有提前安装DNS的缘故，没事儿，一会儿会自动帮我们安装DNS的，当然，你提前安装了DNS，就不会出这个提示了。
![image](https://images0.cnblogs.com/blog/256729/201302/01224537-9ce45fb99dfd49aeb679c16912aa75c8.png)
24、NetBIOS名称，填写就行，没啥特别的讲究。
![image](https://images0.cnblogs.com/blog/256729/201302/01224539-f163d5463d514c84850e16899ddf548b.png)
25、先决条件检查，警告是可以忽略的。
![image](https://images0.cnblogs.com/blog/256729/201302/01224543-7cab423a600042a59db57346c167ee06.png)
26、其间自动插入的DNS安装。
![image](https://images0.cnblogs.com/blog/256729/201302/01224546-8d82dd8429a64bf6b5e1b2d53c20be16.png)
27、域控配置OK，我们之间的管理员，就是将来域控的管理员，也就是sp\administartor这个东东。重启即可。
![image](https://images0.cnblogs.com/blog/256729/201302/01224549-25ef8323cd5b436f94d53682f9cae8e2.png)
28、安装sql的过程。
![image](https://images0.cnblogs.com/blog/256729/201302/01224551-4e45f51bd9dc4d2fa8671fc883da6f38.png)
29、输入秘钥，别问我哦，我也不知道。
![image](https://images0.cnblogs.com/blog/256729/201302/01224554-97af979bf36c4815b7555de69a271168.png)
30、勾选，直接下一步，别嫌我啰嗦，我也不想这样。
![image](https://images0.cnblogs.com/blog/256729/201302/01224557-512a42410a204b22978e6ae0a21375c2.png)
31、不建议勾选更新，我就是因为网络速度的原因，更新了N就，花儿都谢了还没更新好，╮(╯▽╰)╭。
![image](https://images0.cnblogs.com/blog/256729/201302/01224600-8eba30b9ab804c318cb61f98fa9fc3ee.png)
32、安装检测，警告也是可以忽略的。呵呵。
![image](https://images0.cnblogs.com/blog/256729/201302/01224603-d30f5607b16f4cc29b5f2f8a2f4f1e9b.png)
33、选择第一个就可以了，sql我没有太深的研究。
![image](https://images0.cnblogs.com/blog/256729/201302/01224607-da1ba251133945acb5c108af2a0cb9f1.png)
34、选择自己需要的，因为还想试试reporting service，我多选了几个，你们，看心情吧。
![image](https://images0.cnblogs.com/blog/256729/201302/01224611-5c7a7bad40104d01906902a3b0aafdf7.png)
35、下一步啊。
![image](https://images0.cnblogs.com/blog/256729/201302/01224615-b8d4b2bf1a9941c781cf514240c0ec6a.png)
36、依然下一步。
![image](https://images0.cnblogs.com/blog/256729/201302/01224617-6dee75e6b77d4aeb8b13c6116df16c79.png)
37、习惯添加Ad账号，因为将来管理，是需要AD账号的。
![image](https://images0.cnblogs.com/blog/256729/201302/01224621-1db636d5789641c58379ef6f0c54637c.png)
38、理由同上。
![image](https://images0.cnblogs.com/blog/256729/201302/01224624-d06a33116fed44d2b5f4e2093a4e2e8b.png)
39、下一步。
![image](https://images0.cnblogs.com/blog/256729/201302/01224631-5c9b7c8f6f234ce4a9816dd9a86cc380.png)
40、点击“安装”，进入漫长的等待。
![image](https://images0.cnblogs.com/blog/256729/201302/01224634-e001abfc561d4b0c9cd107f615a1a8d3.png)
41、安装好的sql，进入我们的sharepoint安装，当然，我们需要使用“sharepoint 2013产品准备工具”，这个需要联网进行安装。
![image](https://images0.cnblogs.com/blog/256729/201302/01224638-923138fb3e8643b9884e9fdb421153b0.png)
42、安装过程，耐心等待，其间会有重启。
![image](https://images0.cnblogs.com/blog/256729/201302/01224639-c39dfb34eb7e45479946f90297b76170.png)
43、安装完毕，重启后，就可以安装sharepoint2013了哦。
![image](https://images0.cnblogs.com/blog/256729/201302/01224643-5882281e9fcb49049878633d5eab5880.png)
44、重启后，运行配置向导。
![image](https://images0.cnblogs.com/blog/256729/201302/01224645-39ca6c8ad2f9458fb82195ec8f4f23ed.png)
45、界面很熟悉是吧。
![image](https://images0.cnblogs.com/blog/256729/201302/01224648-969cd954c1714692871f3d948b2240c0.png)
46、我们创建新的服务器场。
![image](https://images0.cnblogs.com/blog/256729/201302/01224654-975fee778d76445c9e583114ca218f5a.png)
47、填写数据库服务器名称和访问账号密码。
![image](https://images0.cnblogs.com/blog/256729/201302/01224658-f0249211a719430d9cb91f06276c822c.png)
48、填写安全设置密码。
![image](https://images0.cnblogs.com/blog/256729/201302/01224701-f2a74125a1b141dbb7a9bbccd135128c.png)
49、配置端口和安全设置。
![image](https://images0.cnblogs.com/blog/256729/201302/01224705-b117b06c2d7d4849a0a65bd8dae67b97.png)
50、下一步进入进度条。
![image](https://images0.cnblogs.com/blog/256729/201302/01224709-08d899c1d1ff4f328ba8abbef49070ca.png)
51、安装成功。
![image](https://images0.cnblogs.com/blog/256729/201302/01224713-be96f1a9e3b348b0bc596487d18560c9.png)
52、管理中心，配置服务器场，建议不要开启太多服务，否则会很卡。
![image](https://images0.cnblogs.com/blog/256729/201302/01224715-05338215432c4d6180c56d49ee9e64fa.png)
53、创建网站集
![image](https://images0.cnblogs.com/blog/256729/201302/01224717-a6c492cd77b24a4aa551e1b61fdc642e.png)
**备注：**安装完SharePoint以后，可能会出现下面的错误
![image](https://images0.cnblogs.com/blog/256729/201302/01224721-1f9cd863da02492d87e09e920d46a91a.png)
打sql 2012的sp1补丁即可，这个要感谢技术群的朋友。
![image](https://images0.cnblogs.com/blog/256729/201302/01224725-fc1079c67f824005af5b7b2a0103081f.png)
点进去查看问题，我的是关于“Security Token Service”不可用的问题，有说重启iis里的token服务，我是把net改为2.0，就好了，再改回4.0，也米有出现问题，给大家做个参考吧。
180天试用版key：NQTMW-K63MQ-39G6H-B2CH9-FRDWJ
**参考文档：**
http://technet.microsoft.com/zh-cn/library/hh564135(v=office.15)





