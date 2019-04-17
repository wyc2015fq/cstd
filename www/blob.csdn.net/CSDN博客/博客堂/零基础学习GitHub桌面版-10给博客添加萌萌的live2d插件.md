# 零基础学习GitHub桌面版-10给博客添加萌萌的live2d插件 - 博客堂 - CSDN博客





2019年03月07日 21:29:15[最小森林](https://me.csdn.net/u012052268)阅读数：90
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)









![](http://ww1.sinaimg.cn/large/006qDjsOgy1g0tdplblplj30ne07q7cc.jpg)

# 零基础学习GitHub桌面版-10给博客添加萌萌的live2d插件




### 文章目录
- [零基础学习GitHub桌面版-10给博客添加萌萌的live2d插件](#GitHub10live2d_2)
- [1 效果预览](#1__5)
- [2 安装过程](#2__12)
- [2.1 安装总插件](#21__15)
- [2.2 安装喜欢的具体模型](#22__21)
- [2.3 移动模型地址](#23__33)
- [2.4 配置使用](#24__41)
- [3 成功](#3__64)




## 1 效果预览

我们希望在网站上显示一个好看的动漫形象，live2d这个库完美的解决了我们的需求。

单个效果预览：![image](https://huaji8.top/img/live2d/miku.gif)

所有动画效果：[https://huaji8.top/post/live2d-plugin-2.0/](https://huaji8.top/post/live2d-plugin-2.0/)

## 2 安装过程

### 2.1 安装总插件

在hexo的根目录下执行

```
npm install --save hexo-helper-live2d
```

### 2.2 安装喜欢的具体模型

安装完总插件后，可以安装喜欢的模型

所有动画效果可以在这个网站观看：[https://huaji8.top/post/live2d-plugin-2.0/](https://huaji8.top/post/live2d-plugin-2.0/)

选择一个自己喜欢的，比如tororo（白猫），下载具体的模型：

在根目录下执行：

```
npm install live2d-widget-model-tororo
```

（喜欢别的形象就把tororo换成别的名字）

### 2.3 移动模型地址
- 从hexo的根目录的node_modules中**找到**刚刚安装的模型文件夹，比如我这里安装的是：live2d-widget-model-tororo ；**复制这个文件夹**。
- 在hexo的根目录创建名为live2d_models的文件夹
- 把之前找到的的模型文件夹从node_modules文件夹复制到live2d_models中

### 2.4 配置使用

在hexo根目录下的_config.yml中的最后面添加以下内容

```
live2d:
  enable: true
  scriptFrom: local
  pluginRootPath: live2dw/
  pluginJsPath: lib/
  pluginModelPath: assets/
  tagMode: false
  debug: false
  model:
    use: live2d-widget-model-haruto
  display:
    position: right
    width: 150
    height: 300
  mobile:
    show: true
```

## 3 成功

大功告成，部署博客即可。

hexo -d -g

效果如下：

![](http://ww1.sinaimg.cn/large/006qDjsOly1g0ujitwipjj311y0kgn4f.jpg)



