
# 详解GOPATH - 尹成的技术博客 - CSDN博客

2018年11月16日 11:09:16[尹成](https://me.csdn.net/yincheng01)阅读数：74


`File->Settings->Go->GOPATH`**作用**
存放SDK以外的第三方类库
可以是下载的第三方类库
也可以是自己收藏的可复用代码
**目录结构**
GOPATH目录可以指定多个
每一个GOPATH目录下必须有一个src目录
src目录下的文件夹名称就是引用时的包名
**举例**
`import fuck.shit
    shit.EatSome(5)`上述实例成功运行的前提
GOPATH目录之一下存在路径：`src/fuck/shit/`
shit目录下的某个go源文件中有函数定义：`func EatSome(kg int)`
**全局GOPATH**
所有工程可用
全局GOPATH目录也可以以GOPATH环境变量的方式配置
**工程GOPATH**
只有当前工程可用
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611090339.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

