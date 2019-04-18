# weblogic的安装和部署的【注意事项】 - z69183787的专栏 - CSDN博客
2013年07月08日 10:01:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3677
我是第一次安装weblogic，谈谈自己遇到的问题，并不是写出全部细节
weblogic的安装，每一步都阅读以下，
首先是选择 【不要更新】，之后的就是 选择安装路径之类的，正常选择，一直 【下一步】
最后  勾选  qucikstart
第一个问题 是 选择 【开发模式】  还是  【生产模式】
生产模式 比 开发模式  更有效率，其实按照名字想想就能知道原因，我觉得不用太纠结
选择 生产模式
然后  第二个问题  jdk 的选择
这里推荐使用weblogic自带的jdk，Jrockit，据说效果更好，也不易安装出错
如果使用自己的jdk，有两点说明，第一个，weblogic的是32位的还是64位的 看的是 你的jdk而不是操作系统的，jdk的 检测 使用 java -version即可，如果是64位的，会写着64的，如果是32的，会写着client vm，第二个，jdk的安装路径中不能出现空格等特殊符号，否则安装时失败。
 注：weblogic自带的jdk ， jrockit 都是32 位的
还有其他相关的
1.注册成系统的服务项，网上很多，步骤简单
2.部署项目时，最好不要使用war包部署，war包部署，会有一个log4j的异常，写着root为null，这是weblogic 的bug，在读取路径时有问题，使用文件夹部署即可
3.注意weblogic10.3在linux下时，运行程序时可能出现ServletImpl.setEncodingChar之类的异常，这是weblogic的bug，建议更换版本
4.验证码不显示时，在验证码页面的最上面加上，response.reset()就可以了
5.IO流，response already commit，这个异常，out对象不要关闭，就好了
