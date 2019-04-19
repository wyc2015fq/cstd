# python 错误笔记 - sinat_31425585的博客 - CSDN博客
2019年02月24日 22:26:56[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：43
个人分类：[python](https://blog.csdn.net/sinat_31425585/article/category/6402119)
**1、ImportError: No module named Image**
**解决方法：**
```python
import PIL.Image as Image
```
**参考资料：**
[https://blog.csdn.net/weixin_40013463/article/details/79424403#commentsedit](https://blog.csdn.net/weixin_40013463/article/details/79424403#commentsedit)
**2、ERROR:tensorflow:Exception in QueueRunner: Input to reshape is a tensor with 921600， but request is 196608.....**
**错误原因：**数据在载入时，没有进行缩放，大小为640x480x3 = 921600，导出的时候reshape到256x256x3=196608出错。
**解决方法：**在数据写成二进制文件前，进行缩放操作
```python
image = image.resize((256, 256))
```
参考资料：
[https://blog.csdn.net/sunqimin/article/details/80233591](https://blog.csdn.net/sunqimin/article/details/80233591)
