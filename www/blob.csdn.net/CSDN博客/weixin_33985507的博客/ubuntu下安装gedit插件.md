# ubuntu下安装gedit插件 - weixin_33985507的博客 - CSDN博客
2016年08月05日 10:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
因为gedit-plugins : 依赖: gir1.2-zeitgeist-2.0
所以首先
sudo apt-get install gir1.2-zeitgeist-2.0
 如果报错可以先
sudo apt-get update
 然后
sudo apt-get install gedit-plugins
 在gedit的插件里面选中嵌入终端、单词补全等插件就行了
