# 搭建 vue 环境 - Ms.Fox的专栏 - CSDN博客
2018年12月29日 10:12:16[lulu-lu](https://me.csdn.net/smbluesky)阅读数：21
## **安装node.js**
- 进入Node.js官网：[https://nodejs.org/en/](https://nodejs.org/en/)，选择下载并安装Node.js。
2.验证Node.js是否安装好，在windows下，win+r召唤出运行窗口，输入cmd打开命令行窗口。输入`node -v`即可得到对应的Node.js版本。
`npm -g install npm`，更新npm至最新版本。
## **安装cnpm**
执行命令 `npm install -g cnpm --registry=https://registry.npm.taobao.org` ,使用npm的国内镜像（npm 国内镜像 [https://npm.taobao.org/](https://npm.taobao.org/)）cnpm 命令代替默认的npm命令，增加依赖包加载速度且避免资源限制。
## **cnpm安装脚手架vue-cli**
在命令行中运行命令 `cnpm install -g vue-cli` 安装脚手架。
## **构建项目**
将vue项目建在F盘的vue-workspace文件夹下，利用命令进入此目录。
在cmd中输入盘符F:回车即可进入F盘，
然后执行命令进入F:workspacesvue-workspace路径目录下，
再输入新建项目命令 `vue init webpack javalsj-vue`，执行后会自动生成vue项目。
可以会出现这样的错误
Sorry, name can no longer contain capital letters.（项目名称不能包含大写字母。）
改掉就好了
![](https://img-blog.csdnimg.cn/20181229101217163)![](https://img-blog.csdnimg.cn/20181229101217163)![](https://img-blog.csdnimg.cn/20181228191810642.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NtYmx1ZXNreQ==,size_16,color_FFFFFF,t_70)
## ![](https://img-blog.csdnimg.cn/20181228193044968.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NtYmx1ZXNreQ==,size_16,color_FFFFFF,t_70)
## **安装项目依赖**
输入命令 `cnpm install`
博主在下载第三方开源项目运行的时候，有时会遇到奇怪的报错，然后通过先执行cnpm rebuild node-sass，后执行cnpm install解决，此步骤不是必须的。
## **运行项目**
项目准备完毕后，现在可以运行vue初始项目看效果了。
在cmd中，注意需要使用命令**先定位到F:workspacesvue-workspacejavalsj-vue目录下**，然后再输入命令`npm run dev`来运行项目。
![](https://img-blog.csdnimg.cn/20181229101043712.png)
就会自动在浏览器中打开当前的项目的网址了~
