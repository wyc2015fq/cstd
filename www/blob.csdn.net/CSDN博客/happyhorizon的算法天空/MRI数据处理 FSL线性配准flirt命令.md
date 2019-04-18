# MRI数据处理: FSL线性配准flirt命令 - happyhorizon的算法天空 - CSDN博客
2018年03月21日 20:36:22[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1209
flirt 是FSL的线性修正工具. 官方提供了非常丰富的调用方式:
- 命令Flirt可以启动FLIRT的GUI + InvertXFM, ApplyXFM, ConcatXFM
- 命令flirt 是命令行工具
- misc flirt utilities – flirt附属的命令行工具.
flirt是进行仿射变换的主要程序.参数包括了 -in 指定的输入图像,  -ref 指定的参考模板,  -mat 指定了仿射变换(将输入图像配准到ref模板上用的)4*4的仿射矩阵.  -out 指定的是完成仿射变换后的输出, 除此之外, flirt的参考模板也可以是一个某个NifTI文件,要求NifTI文件和输入文件的体素大小\图像尺寸相同.
**要点:**
**1) 目标函数/代价函数加权(cost function weighting).**
参数-refweight, -inweight可以用来指定目标函数或者代价函数的加权,使得代价函数在每个体素上有不同的加权值,当weight=0,就可以去除掉一些不感兴趣的区域, 或者对感兴趣的区域设置weight取值比较大. 注意weight和mask是不一样的, 是因为mask可能会引入边界伪迹,但是weight不会.
**2) 自由度 (degrees of freedom)**
参数-dof是指自由度, 可以设置为6或者7,或者9.
dof自由度指标更灵活的设置方式参考$FSLDIR/etc/flirtsch.
**3) 插值算法(interpolation methods)**
最近邻算法, 一族基于正弦曲线的插值算法(三种window类型:矩形窗口, 汉宁窗口或者Blackman窗口,窗口宽度可设置),还有样条插值方法. 插值只有在最后的变换中有用(applyxfm),而不是在registration计算中.
**4) 目标函数/代价函数(cost function)**
within-modality函数的构造采用了附加正则项的最小二乘法, between-modality函数默认为Correction Ratio, 另外还有互信息(Mutual Information)和正则互信息(Normalised Mutual Information). 除此之外,还有BBR代价函数, 采用了模板图像的segmentation来定义边界, 代价函数用输入图像和模板边界两侧的密度区别intensity difference作为优化目标.
下面给出了一条简单的调用flirt的命令行语句:
`flirt -in invol -ref refvol -out outvol -omat invol2refvol.mat -dof 6`
这里invol是输入的T1图像, refvol应该是registration的模板,例如MNI152, invol2refvol.mat 是registration计算中用到的仿射变换矩阵, dof是指自由度(degrees of freedom), 可以设置为6或者7,或者9.
`flirt -in newvol -ref refvol -out outvol -init invol2refvol.mat -applyxfm`
还可以将之前的仿射变换矩阵作为输入设置, 提供给flirt 命令. -init 选项就可以.
下面这条命令得到的结果在fsleyes中打开, 可以看到
`flirt -in brain_1203.nii.gz -ref MNI152_T1_1mm_brain.nii.gz -out brain_registrated.nii.gz -omat invol2refvol.mat`
![这里写图片描述](https://img-blog.csdn.net/20180321203520469?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180321203506780?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
brain_1203 绿色
brain_registrated 红色
MNI152 灰色
