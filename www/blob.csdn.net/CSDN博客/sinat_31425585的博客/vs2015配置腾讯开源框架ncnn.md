# vs2015配置腾讯开源框架ncnn - sinat_31425585的博客 - CSDN博客
2018年09月20日 00:08:39[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：704
    腾讯ncnn官方github下载地址为：[https://github.com/Tencent/ncnn](https://github.com/Tencent/ncnn)
    github上有一个配置好的vs2015版本ncnn：[https://github.com/guozhongluo/ncnn-vs2015-examples-demo](https://github.com/guozhongluo/ncnn-vs2015-examples-demo)
**    1、框架编译**
    可以直接下载这个vs2015版本的ncnn，下载完之后打开。
    这里需要你提前配置好opencv，那个版本无所谓，然后用vs2015打开ncnn-vs2015-examples-demo-master工程，打开之后会报一堆错误，因为你引入的头文件路径不对，右键项目，右键单击属性，如图1所示。
![](https://img-blog.csdn.net/20180919235658827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                                              图1
    然后，点击C/C++  =>  常规  => 附加包含目录，如图2所示。
![](https://img-blog.csdn.net/20180919235952192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                                    图2
    点击下拉三角形，修改为如下内容，如图3所示。
![](https://img-blog.csdn.net/20180920000135402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                                  图3
    最后，删除掉链接器 ==> 常规 ==> 附加库目录中内容，如图4所示。
![](https://img-blog.csdn.net/2018092000042033?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                                  图4
点击编译运行，会出现一个错误：
```cpp
严重性代码说明项目文件行错误C4716“load_labels”: 必须返回一个值
```
解决方法：在static int load_labels(string path, vector<string>& labels) 函数增加一个返回值，return 1；
再运行，就可以得到结果，如图5所示。
![](https://img-blog.csdn.net/20180920000653232?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                          图7
**    2、使用过程：**
![](https://img-blog.csdn.net/20181017211044164?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考资料：
[1] [https://blog.csdn.net/fuwenyan/article/details/76105574](https://blog.csdn.net/fuwenyan/article/details/76105574)
[2] [https://github.com/Tencent/ncnn](https://github.com/Tencent/ncnn)
[3] [https://github.com/guozhongluo/ncnn-vs2015-examples-demo](https://github.com/guozhongluo/ncnn-vs2015-examples-demo)
