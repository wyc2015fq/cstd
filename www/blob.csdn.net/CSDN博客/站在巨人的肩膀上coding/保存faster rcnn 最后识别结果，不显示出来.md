# 保存faster rcnn 最后识别结果，不显示出来 - 站在巨人的肩膀上coding - CSDN博客





2018年02月01日 01:51:38[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：2253








```cpp
首先需要注意的是，要把demo.py需要检测的图片放到data/demo文件夹下，才能够使之检测出来！！！！！直接把所有测试图像copy到py-faster-rcnn/data/demo文件夹下即可。
```

```cpp
在上面说的做好后，就可以开始下面的更改了：
```

```cpp
for im_name in im_names:  
       print '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'  
       print 'Demo for data/demo/{}'.format(im_name)  
       demo(net, im_name)  
       plt.savefig("../testfigs/" + im_name)  
  
   #plt.show()
```

其实就是增加一句：

```cpp
plt.savefig("../testfigs/" + im_name)
```

 注销最后一句图片显示部分：



```cpp
#plt.show()
```





