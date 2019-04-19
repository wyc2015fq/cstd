# npm - 左直拳的马桶_日用桶 - CSDN博客
2014年11月27日 11:40:04[左直拳](https://me.csdn.net/leftfist)阅读数：1728
个人分类：[node.js](https://blog.csdn.net/leftfist/article/category/2626561)
node.js给我的感觉就是短小精悍而工具众多：语言就是javascript，不是什么新语言巨献。旗下各种工具各种包，什么CoffeeScript啦，Express.js啦，bower啦，gulp啦，jade啦，当你刚接触node.js的时候，纷沓繁乱 ，一窝蜂迎面扑来，让人脑袋“嗡”一声，半天回不过神。
这其中，少不了应用 npm。
npm：NodeJs包管理器，就是用来安装各种包的，也可以作项目初始化用途：
**1、安装包**
npm install 啥啥啥
通常，这个命令应该在项目根目录下运行，这样，根目录的node_modules子目录下就会出现指定的包。如果没有node_modules，系统会自动创建一个。
比如，我要安装socket.io，可以打命令：
npm install socket.io
包有版本之分。像socket.io现在的版本已经在1.0以上，但我要使用0.9的，可以这样写：
npm install socket.io@0.9
如果一个项目从git拿过来，可能没有node_modules，因为这可以从网上下载，原作者不会提交这个。对于这种情况，我们可以在项目根目录下，输入：
npm install
那么系统会根据根目录下的package.json里的指定，下载各种包。
所以，package.json很重要。
现在问题来了，package.json从何而来？答曰：
**2、项目初始化**
在项目根目录下，输入：
npm init
系统会问你一系列问题，输入答案，回车即可。不想回答可以一路回车，如此，package.json可得矣。
如果我用新包，或更改包版本，可以在安装的同时，写入package.json，方法为：
npm install 啥啥啥 --save

