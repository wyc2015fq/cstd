# Yolo 9000的darknet实现 - AI之路 - CSDN博客





2017年08月12日 10:25:11[AI之路](https://me.csdn.net/u014380165)阅读数：2314








代码的github地址：[https://github.com/philipperemy/yolo-9000](https://github.com/philipperemy/yolo-9000)

我是Ubuntu 16.04系统

**1、拉取项目**

```php
git clone --recursive https://github.com/philipperemy/yolo-9000.git
```

因为我只能用http的地址，当然你可以跟github里面一样用git地址，结果都是一样的。默认拉取下来的项目名称是yolo9000

**2、生成weight**

```
cd yolo-9000
cat yolo9000-weights/x* > yolo9000-weights/yolo9000.weights
```

**3、设置环境变量**

命令行输入：

```
export PATH=/usr/local/cuda-8.0/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64:$LD_LIBRARY_PATH
```

**4、修改Makefile文件和编译darknet（这里我直接编译GPU版本）**

```bash
cd darknet
vim Makefile # 修改Makefile的前两行为GPU=1和CUDNN=1
make # 编译
```

编译成功后会在yolo-9000/darknet/目录下生成一个可执行文件darknet，后面要用到。

**5、测试图像**

运行下面命令。
`./darknet detector test cfg/combine9k.data cfg/yolo9000.cfg ../yolo9000-weights/yolo9000.weights data/dog.jpg`
这里的darknet就是前面生成的可执行文件，/yolo9000.cfg保存的是网络结构，yolo9000.weights就是前面第2步生成的模型参数，dog.jpg是要测试的图像。 

运行过程中会生成如下信息：在GPU上做的测试，时间为360ms（时间有点长啊，很奇怪）

![这里写图片描述](https://img-blog.csdn.net/20170812102402458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后生成测试结果图predictions.png放在yolo-9000/darknet/目录下，我这里也贴一下吧。

![这里写图片描述](https://img-blog.csdn.net/20170812102420233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



