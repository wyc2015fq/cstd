# 怎么把war包部署到tomcat - xqhrs232的专栏 - CSDN博客
2018年09月29日 15:47:45[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：265
原文地址::[https://jingyan.baidu.com/article/fcb5aff7478311edaa4a712d.html](https://jingyan.baidu.com/article/fcb5aff7478311edaa4a712d.html)
war包的发布是开发人员经常应用的防范，在客户现场用war包发布项目.本文将从war包的制作到发布成功详细讲解.
## 工具/原料
- 
myeclipse，tomcat。
## 方法/步骤
- 
导出war包，右键选择要发布的项目--选择导出
![怎么把war包部署到tomcat](https://imgsa.baidu.com/exp/w=500/sign=e2d5ea11eedde711e7d243f697eecef4/b03533fa828ba61e9f64923b4a34970a304e591f.jpg)
- 
选择java EE下的war file，选择next，选择路径后--finish，完成了war的导出.
![怎么把war包部署到tomcat](https://imgsa.baidu.com/exp/w=500/sign=8b2d74f1a66eddc426e7b4fb09dab6a2/eac4b74543a982264a2571338182b9014a90eb3a.jpg)
- 
下载一个免安装版的tomcat.如图
![怎么把war包部署到tomcat](https://imgsa.baidu.com/exp/w=500/sign=9c927c0049a7d933bfa8e4739d4bd194/dbb44aed2e738bd4203df425aa8b87d6277ff982.jpg)
- 
将war包放入什么webapp中
![怎么把war包部署到tomcat](https://imgsa.baidu.com/exp/w=500/sign=a26a0ee2acefce1bea2bc8ca9f50f3e8/a9d3fd1f4134970a02cb5d3b9ecad1c8a7865d6d.jpg)
- 
修改tomcat先server.xml文件底部添加 如图
<Context path=" " docBase="jcsj" debug="0" privileged="true"/>
path中为war包的位置  docBase中为war文件名称
![怎么把war包部署到tomcat](https://imgsa.baidu.com/exp/w=500/sign=34246fea3dd3d539c13d0fc30a86e927/7aec54e736d12f2e44f6e17444c2d56285356837.jpg)
- 
启动tomcat--bin--startup.bat.启动后就可以直接访问项目了.如图
![怎么把war包部署到tomcat](https://imgsa.baidu.com/exp/w=500/sign=4644ad67a64bd11304cdb7326aaea488/b21c8701a18b87d6a9aee20b0c0828381f30fd32.jpg)
END
## 注意事项
- 
myeclipse10导出war的时候，需要破解才能导出.
