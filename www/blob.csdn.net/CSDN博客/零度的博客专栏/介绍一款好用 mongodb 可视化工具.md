# 介绍一款好用 mongodb 可视化工具 - 零度的博客专栏 - CSDN博客
2018年04月16日 13:53:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：2107
最近想自己搭建一个个人博客，所以学了下mongodb，mongodb是用命令行输入的，有些人可能不太习惯，我自己找了下mongodb的一些可视化工具，一开始安装的是mongoVUE，mongoVUE页面挺友好的，还不错，但是最新版本的mongodb，mongoVUE已经不支持了，打开后看不到数据，而且是还是收费的。后来发现了一款adminmongo很好用，这里介绍给你们用一下。
![adminMongo collections screen](https://raw.githubusercontent.com/mrvautin/mrvautin.github.io/master/images/adminMongo/adminMongo_collectionview.png)
github地址：[https://github.com/mrvautin/adminMongo](https://github.com/mrvautin/adminMongo)
安装方法：
1、把git仓库克隆到本地
`git clone https://github.com/mrvautin/adminMongo`
2、进入仓库
`cd adminMongo`
3、安装
`npm install`
4、启动
`npm start`
5、访问地址 [http://127.0.0.1:1234](http://127.0.0.1:1234/)
进来之后只需填写数据库名称（随便起一个），和本地ip地址 mongodb://127.0.0.1:27017 即可，然后点击右边的"黑色按钮connect"进入即可
![adminMongo connections screen](https://raw.githubusercontent.com/mrvautin/mrvautin.github.io/master/images/adminMongo/adminMongo_connections.png)
![adminMongo database screen](https://raw.githubusercontent.com/mrvautin/mrvautin.github.io/master/images/adminMongo/adminMongo_dbview.png)
![adminMongo collections screen](https://raw.githubusercontent.com/mrvautin/mrvautin.github.io/master/images/adminMongo/adminMongo_collectionview.png)
可以看了，可以愉快的玩耍mongodb了
