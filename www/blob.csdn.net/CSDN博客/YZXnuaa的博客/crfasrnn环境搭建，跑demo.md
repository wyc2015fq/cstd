# crfasrnn环境搭建，跑demo - YZXnuaa的博客 - CSDN博客
2018年05月11日 18:15:36[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：69
crfasrnn官网：
https://github.com/torrvision/crfasrnn
按照官网的步骤说明做即可
（1）将下载得到的压缩包解压，得到crfasrnn文件夹，将该文件夹拷贝到Linux下面
（2）安装crfasrnn/caffe/python/requirements.txt中提到的Python包，单个安装执行下列命令：
**[html]**[view plain](https://blog.csdn.net/liuxiabing150/article/details/51636650#)[copy](https://blog.csdn.net/liuxiabing150/article/details/51636650#)
- sudo pip install Cython    
（3）拷贝caffe下的Makefile.config.example，得到Makefile.config，修改该config下的配置路径（使用默认Python是不需要修改的）
**cp Makefile.config.example Makefile.config **
（4）在crfasrnn/caffe目录下，编译：
**[html]**[view plain](https://blog.csdn.net/liuxiabing150/article/details/51636650#)[copy](https://blog.csdn.net/liuxiabing150/article/details/51636650#)
- make all -j4  
**[html]**[view plain](https://blog.csdn.net/liuxiabing150/article/details/51636650#)[copy](https://blog.csdn.net/liuxiabing150/article/details/51636650#)
- make pycaffe  
（5）执行crfasrnn/python-scripts目录下的download_trained_model.sh文件，下载crfasrnn model文件：
**[html]**[view plain](https://blog.csdn.net/liuxiabing150/article/details/51636650#)[copy](https://blog.csdn.net/liuxiabing150/article/details/51636650#)
- sh download_trained_model.sh  
（6）运行crfasrnn/python-scripts目录下的crfasrnn_demo.py，会输出output.png，即分割结果：
**[html]**[view plain](https://blog.csdn.net/liuxiabing150/article/details/51636650#)[copy](https://blog.csdn.net/liuxiabing150/article/details/51636650#)
- python crfasrnn_demo.py  
（7）可以替换crfasrnn_demo.py文件里的input.jpg为自己需要分割的图片，得到使用自己图片的分割结果。         注：自己的图片是有大小限制的，例如：如果使用1024*640的图片作为输入，会报错，将图片尺寸修改为500*312就没有问题了            
