# Mac React-Native 环境配置（iOS开发） - weixin_33985507的博客 - CSDN博客
2017年03月16日 09:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
1.最新的Xcode
2.安装HomeBlew（OS系统上的一个安装包管理器，安装后可以方便后续安装包的安装）
命令终端：
ruby -e"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
搜索软件：brew search 软件名，如brew search wget
安装软件：brew install 软件名，如brew install wget
卸载软件：brew remove 软件名，如brew remove wget
3.安装Node.js （服务端的JavaScript运行环境）（https://nodejs.org/en/）
4.建议安装WatchMan（React修改source文件的一个工具）
终端命令：
brew install watchman
(5). 安装Flow: 一个JavaScript 的静态类型检查器。
终端命令：
brew install flow
(6). 安装React Native CLI: 用来开发React Native的命令行工具
终端命令：
npm install -g react-native
若安装失败：
npm install -g react-native-cli
npm uninstall -g react-native
配置完成，可以创建新项目进行混合开发了。
参考资料：
1.http://www.cnblogs.com/jianglan/p/5027315.html
2.http://www.cnblogs.com/lzrabbit/p/4032515.html
3.http://reactnative.cn/docs/0.42/getting-started.html
