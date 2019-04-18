# 核磁数据处理: FSL系列之segmentation工具FAST - happyhorizon的算法天空 - CSDN博客
2018年03月15日 20:27:22[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：514
fsl的segmentation工具: FAST (FMRIB’s Automated Segmentation Tool)将大脑组织的3D影响分解为不同的组织,比如灰质\白质\脑脊液等等, 同时还可以进行spatial intensity variations的修正, 也就是bias field或者RF inhomogeneities.
在终端输入Fast, 打开FAST的GUI界面, 输入fast则是FAST的命令行调用.
![这里写图片描述](https://img-blog.csdn.net/20180315201937490?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
FAST工具采用的算法是隐马尔科夫随机场模型,以及相关的EM算法(Expectation-Maximization).
整个计算过程都是全自动的,输入是经过bias field-corrected 图像, 输入是从概率上和/或者分容积的组织分解.(probabilistic and/or partial volume tissue) .算法的鲁棒性和可靠性都是比较好的,对噪声不太敏感.
fast命令行工具的调用格式为:
```
fast [options] [input_nifti_file] [output_nifti_file]
```
例如:
```
fast -t 1 -n 3 -H 0.1 -I 4 -l 20.0 -o /home/dl/Data/Brain/Liang_test/test_out/brain /home/dl/Data/Brain/Liang_test/test_out/brain
```
生成的segmentation文件有:
-rw-rw-r-- 1 dl dl   321587 Mar 15 19:55 brain_mixeltype.nii.gz
-rw-rw-r-- 1 dl dl   575046 Mar 15 19:55 brain_pve_0.nii.gz
-rw-rw-r-- 1 dl dl   995397 Mar 15 19:55 brain_pve_1.nii.gz
-rw-rw-r-- 1 dl dl   537648 Mar 15 19:55 brain_pve_2.nii.gz
-rw-rw-r-- 1 dl dl   276283 Mar 15 19:55 brain_pveseg.nii.gz
-rw-rw-r-- 1 dl dl   277565 Mar 15 19:55 brain_seg.nii.gz
常用参数:
-S  or --channels= 图像通道数, 通常设为1
-t  or --type= 图像种类, T1的话设置为1, T2设置为2, PD设置为3
-o  or --out= 输出文件的基本名称,生成的文件会自动附上后缀, 例如seg, pve_0, pve_1等.
-n  or --class= 组织类型数.T1的话通常设置为3, (Grey Matter, White Matter and CSF) T2的话通常设置为4. 如果图像很不清晰, 可以设置为2.
-b 输出的估计偏置场(estimated bias field)
-B 输出偏置图像并保存(bias-corrected image)
-l  or –lowpass= 偏置场平滑, m是FWHM, 单位是mm, m ≅ 2 √n
-I 或者 –iter  循环次数
输出的结果用fsleyes打开后,可以看到:
![这里写图片描述](https://img-blog.csdn.net/20180315202632299?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考:
[https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/FAST#fast](https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/FAST#fast)
