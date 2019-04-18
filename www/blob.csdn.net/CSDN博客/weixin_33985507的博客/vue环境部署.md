# vue环境部署 - weixin_33985507的博客 - CSDN博客
2017年07月06日 01:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
1，brew 安装 node, node自带npm
2, node -v , npm -v进行版本检查
3,安装vue:
sudo npm install -g vue-cli
-g表示程序安装，如果不加-g会安装到当前目录下
4,终端输入vue,测试vue
5.初始化一个项目：
vue init webpack my-first-vue-project
第一个参数webpack是项目类型，第二个参数是工程名字
回答一系列问题，例如工程名字等
6.下载依赖：
sudo npm install (通过package中的依赖)
7.npm run dev(package中有定义)，启动一个本地服务http://localhost:8080
8,可在hello.vue中修改，刷新浏览器，可发现其支持热更新
