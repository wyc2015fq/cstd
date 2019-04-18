# 核磁数据处理: FSL系列之bet命令 - happyhorizon的算法天空 - CSDN博客
2018年03月20日 20:12:49[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1737
bet命令用于脑组织的提取.有GUI和命令行两种调用方式. 
下图是bet的GUI界面. 在shell中输入命令Bet就可以启动bet的GUI界面了. 界面非常简单,只需要制定输入图像,设置图像密度阈值f, 选择bet命令执行的方式:Run standard brain extraction using bet2 或者其他. 点击Go就可以运行得到结果了.
![图 bet_GUI](https://img-blog.csdn.net/20180320200338373?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
命令行调用bet最典型的格式为:
```xml
bet <input> <output> [options]
```
例如: 
`bet -m sub3m0.nii.gz`
运行后生成两个文件,一个是sub3m0_brain.nii.gz, 还有一个是对应的mask文件sub3m0_brain_mask.nii.gz 
选项 -f 可以指定图像密度阈值, 默认是0.5. 下图显示了f设置为0.2和f设置为0.8的对比. 
![这里写图片描述](https://img-blog.csdn.net/20180320200939130?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图中灰色的是f=0.2的结果,蓝色是f=0.8的结果.
如果图像有很大的FOV或者还有substantial bias field, 在提取脑组织图像时很容易出现一些问题, 这时需要用到一个小技巧, 就是bet的-g 选项. 
这里看一个例子. 在fsl的上手教程中数据文件intro中,有一个sub3m0.nii.gz文件. 
图 bet_sub3m0 
首先用bet提取脑组织, `bet -m sub3m0.nii.gz`, 可以得到: 
图 bet_sub3mo_brain 
可以看到边缘还有很多没有去除的痕迹. 可以用bet的-g( the gradient threshold)选项来修正.  
`bet -m -f 0.3 -g 0.2 sub3m0.nii.gz`
另外,还需要一提的是 `bet data.nii.gz -R`的效果比`bet data.nii.gz -m -f 0.5`要好很多. 对比如下图: 
![这里写图片描述](https://img-blog.csdn.net/20180320201115779?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图中红色的部分是`bet data.nii.gz -R`生成的.
