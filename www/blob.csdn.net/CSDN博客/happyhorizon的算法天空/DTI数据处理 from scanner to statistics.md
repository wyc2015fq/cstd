# DTI数据处理: from scanner to statistics - happyhorizon的算法天空 - CSDN博客
2018年02月23日 15:46:35[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：4233
# 安装准备
## 工具: FSL, MRIcron
## 准备工作:
### MRIcron安装
MRIcron的下载地址: 
[http://www.mccauslandcenter.sc.edu/mricro/mricron/dcm2nii.html](http://www.mccauslandcenter.sc.edu/mricro/mricron/dcm2nii.html)
### FSL安装:
FSL是一个FMRI, MRI和DTI数据的的分析库. 支持OSX和linux系统, windows需要在虚拟机运行. 所有的命令可以在命令行调用,也可以通过GUI调用.
下载网址 : [https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/FslInstallation](https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/FslInstallation)
下载fslinstaller.py, 这个安装文件是基于python2.7版本编写的, 且运行时需要root用户权限. 
`sudo fslinstaller.py`
建议不要使用默认安装路径, fsl非常大, 大约1.5G, 如果/根目录空间不大的话,请不要轻易尝试. 
安装过程中可能由于网速等原因会导致下载超时,多按几次回车刷新. 
安装完成后, 需要配置系统环境变量. 在~/.bashrc中添加:
```bash
FSLDIR=/usr/local/fsl
. ${FSLDIR}/etc/fslconf/fsl.sh
PATH=${FSLDIR}/bin:${PATH}
export FSLDIR PATH
```
然后source命令更新一下系统的环境变量,搞定.
`source ~/.bashrc`
## 数据预处理:
核磁共振设备厂家主要有西门子,GE和飞利浦: 
西门子/GE核磁共振数据格式是 DICOM 飞利浦的是: PAR/REC格式.  
DICOM格式最为常见. 首先需要从DICOM格式转换成NIfTI格式.
**1 查看数据文件 **
`ls /your_raw_data_dir/$subject_nr/dicoms`
**2 从DICOM格式转化为NifTI格式(.nii) **
原始DICOM格式(.dcm) , 首先可以用MRIcrom中的工具dcm2nii转换成NifTI(.nii）。安装后, 在终端中输入: 
`dcm2nii raw_data_dir output_dir`
转换完成后, 将生成三个文件: 压缩后的原文件、o开头、co开头的文件。其中o开头的文件主要是进行了reorient的，而co是经过切割了neck的。一般用于空间normalize都选用co开头的文件。
FSL提供了NifTI格式处理工具查看数据结构和可视化工具，例如fslinfo可以用来查看转换后NifTI的基本信息:
```
dl@DL:~/Data/Brain/testing_data$ fslinfo 052212_s09_dti.nii
data_type      INT16
dim1           256
dim2           256
dim3           67
dim4           49
datatype       4
pixdim1        1.000000
pixdim2        1.000000
pixdim3        2.000000
pixdim4        8000.000000   ?
cal_max        0.0000
cal_min        0.0000
file_type      NIFTI-1+
```
其中dim1, dim2, dim3分别是核磁图像的3个维度, 256x256x67. Dim4是volumns的个数 49, pixdim1, pixdim2, pixdim3分别是体素voxel的维度 1x1x2mm。 
**fsleview** 用于查看NifTI图像: 
`fslview 052212_s09_dti.nii`
## 失真修正和弥散张量生成
对DTI数据做失真修正和数据处理的整体流程如下图：
![这里写图片描述](https://img-blog.csdn.net/20180222170921824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 失真修正
失真有很多种 主要是 eddy currents distortion, EPI/fieldmap distortion. 
试验期间的测量原理\或者被试状态等等形成了多种可能的误差,比如头动, 比如核磁涡流等等. 
**1.0 registration 对齐**
对齐修正对于structural MRI, functional MRI和diffusionMRI来说都非常重要.如果registration不准确,那么后面对结构核磁的统计分析或者group level的分析都是不准确的. 
FSL的Bet命令可以进行简单registration修正 
input: image, options 
output: brain_extraced.nii.gz, brain_mask.nii.gz …
** 1.1 Eddy current correction(可选)**
FSL提供了专门的eddy correction工具, 使用终端命令:
```
dl@DL:~/Data/Brain/testing_data$ eddy_correct 052212_s09_dti.nii 052212_s09_dti_eddy.nii 0
processing 052212_s09_dti_eddy_tmp0000
processing 052212_s09_dti_eddy_tmp0001
processing 052212_s09_dti_eddy_tmp0002
processing 052212_s09_dti_eddy_tmp0003
processing 052212_s09_dti_eddy_tmp0004
processing 052212_s09_dti_eddy_tmp0005
...
```
**1.2 EPI correction**
EPI失真是由于磁场不均匀引起的, 比eddy失真更明显. EPI失真如下图所示, 图中绿色的是真实的大脑, 灰色是EPI失真造成的变形. 
![这里写图片描述](https://img-blog.csdn.net/20180222171445753?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
FSL有非常棒的工具可以用来处理EPI失真, 分别为: fslmerge (这里需要的是fmap文件) 和topup(acqparams.txt) 
(1)fslmerge: 
fslmerge处理流程分为两步: 首先用raw phase和磁场数据进行融合, 生成field map, 之后用field map 生成diffusion image. 
这类失真是需要结合磁场数据处理的.a real field map image数据名称一般带有fmap.例如: 052212_s10_fmap.nii 
【待续】 
(2)topup 
topup是另外一种方法.这种方法是将正扫描数据和反扫描数据,配合acqparams.txt中记录的扫描信息实现融合处理. acqparams.txt里面是关于扫描方向和扫描时间的信息。
```
0 -1 0 0.0665
0 1 0 0.066
```
这里-1代表AP方向，1代表PA方向。最后一列代表扫描信息读取时间。 
在终端输入:
```
fslroi dwidata nodif 0 1 // 生成nodif文件是AP方向的
fslmerge -t AP_PA_b0 nodif nodif_PA // nodif_PA是PA方向扫描得到的. 这句生成AP_PA_b0
```
之后就可以调用topup命令, 对核磁数据进行EPI修正：
```
topup --imain=AP_PA_b0 \
      --datain=acqparams.txt \
      --config=b02b0.cnf \
      --out=topup_AP_PA_b0 \
      --iout=topup_AP_PA_b0_iout \
      --fout=topup_AP_PA_b0_fout
```
### 2 提取脑组织并做eddy修正
下面就需要将大脑和头骨以及其他的头部组织分开, 以便后续只针对大脑进行分析.FSL自动大脑提取工具bet可以实现目标. 
在提取之前, 应该把经过上述修正后得到的正确的b0 map做平均, 得到一个类似与”模板”的文件.
`Fslmaths topup_AP_PA_b0_iout -Tmean hifi_nodif`
hifi_nodif就是那个”模板” 
脑组织核磁数据提取: 
`bet  hifi_nodif.nii.gz hifi_nodif_brain.nii.gz -m -f 0.2`
bet命令参数:
```
-f option is to set a fractional intensity threshold which determines where the edge of the final segmented brain is located. -f 后面的数字取值大小直接影响了输出结果的精细程度.
-g option is to cause a gradient change to be applied to the previous threshold. Vertical gradient.
-m generate binary brain mask
-R estimate the center
```
这样生成了两个文件,一个是brain mask, 一个是brain本身. 
![这里写图片描述](https://img-blog.csdn.net/2018022217193687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
freesurfer比bet的处理效果要好一些. 如果bet的结果不够满意, 可以用freesurfer试一试. 
fsl的eddy工具可以实现涡流矫正。输入参数和输出参数如下图： 
![这里写图片描述](https://img-blog.csdn.net/20180222172208290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由于eddy运算量很大, 官方提供了cuda和openmp支持. 这里的cuda是cuda7.5 . 如果显卡不支持cuda7.5驱动, 可以用eddy_openmp.
```
eddy_openmp:
    --imain File containing all the images to estimate distortions for
    --mask  Mask to indicate brain
    --index File containing indices for all volumes in --imain into --acqp and –topup
    --acqp  File containing acquisition parameters
    --bvecs File containing the b-vectors for all volumes in --imain
    --bvals File containing the b-values for all volumes in --imain
    --out   Basename for output
--data_is_shelled flag is set to avoid the automatic checking on whether the data has been acquired using a multi-shell protocol (we already know that is indeed the case for this dataset).
```
查看index.txt: 
`cat index.txt`
全部都是1, 每个1 都代表了dwidata中的一个volume, 意思是所有的volume和acqparams.txt 中的参数eddy处理中都要用到
调用eddy_openmp进行涡流矫正:
`eddy_openmp --imain=dwidata.nii.gz --mask=hifi_nodif_brain_mask --index=index.txt --acqp=acqparams.txt --bvecs=bvecs --bvals=bvals --out=eddy_unwarped_images --data_is_shelled`
运行完成后,会生成9个文件: 
eddy_unwarped_images.nii.gz 
eddy_unwarped_images.rotated_bvecs 
eddy_unwarped_images.eddy_parameters 
…
**3 生成神经纤维张量**
扫描仪的梯度信息用来计算tensor模型的.  FSL的DTIFIT用来解决这个问题. DTIFIT有三种方式调用: 前两种都是用GUI, 也可以直接用命令行。 
You can run DTIFIT (the FSL diffusion tensor fitting program) in one of three ways, all described below. Choose one of these approaches: 
1: Run DTIFIT on a FDT directory 
2: Run DTIFIT by manually specifying the input files 
3: Call dtifit from the command line
` dtifit --data=eddy_unwarped_images --mask=hifi_nodif_brain_mask --bvecs=bvecs --bvals=bvals --out=dti`
![这里写图片描述](https://img-blog.csdn.net/20180223153356291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
运行后生成如下文件: 
dti_FA.nii.gz  :  the anisotropy map 各项异性图 
dti_L1.nii.gz      ? 
dti_L2.nii.gz      ? 
dti_L3.nii.gz      ? 
dti_MD.nii.gz   : Mean Diffusivity map 
dti_MO.nii.gz 
dti_S0.nii.gz 
dti_V1.nii.gz    the principal eigenvector map 
dti_V2.nii.gz     secondary eigenvalue 
dti_V3.nii.gz     third eigenvalue
用fsleyes打开dti_FA.nii.gz,如下: 
![这里写图片描述](https://img-blog.csdn.net/20180223153545854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
打开 主特征向量图 principal eigenvector map: 得到下面的三维彩色图, 颜色表示了Diffusion direction . 
![这里写图片描述](https://img-blog.csdn.net/20180223153507620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
调整一下两附图片的对比度和透明度, 显示出叠加效果. 
![这里写图片描述](https://img-blog.csdn.net/20180223153741115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
选择工具栏上的modulate by 为 dti_FA, 可以得到下面的显示效果: 
![这里写图片描述](https://img-blog.csdn.net/20180223153642245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
设置overlay data type为3-direction vector (Line) , 显示tensor vector图, 单击图片后ctrl+鼠标滚轮可以放大缩小.
Overlay data type to 3D/4D volume 可以查看三个坐标轴方向的矢量坐标大小(图像的形式)
**4 张量拟合** tensor fitting - TBSS analysis 为例
**数据快视**
快速查看一下待处理的数据: 
`slicesdir *.nii.gz`
生成所有NifTI文件的图片摘要, 并用html展示.
**统计分析前的预处理**
首先用tbss_b_preproc  工具略微预处理一下, 去掉brain_edge伪迹,  去掉最后的slice(没有什么信息了), 从diffusion tensor fitting中去掉可能的outliers. 
`tbss_1_preproc *.nii.gz`
这个命令将创建两个文件夹(FA, origdata )处理结果放到FA文件夹下, 原始文件放到origdata文件夹下.
**非线性registration**
将所有被测对象的FA结果registrate to FMRIB58_FA 模板. 这步非常耗时. 可以用计算机cluster加速。This process can take a long time, as each registration takes around 10 minutes. You can easily speed this up if you have multiple computers running cluster software such as SGE (Sun Grid Engine) 
在FA文件夹的上一层文件夹中运行命令: 
`tbss_2_reg -T -n`
-T 就将FA结果registrate 到默认的FMRI58_FA模板上. 运行后生成:
```
1260_FA_to_target.mat
1260_FA_to_target_warp_inv.nii.gz
1260_FA_to_target_warp.msf
1260_FA_to_target_warp.nii.gz
…
target.nii.gz
```
至此,完成了所有核磁数据像模板的”订正”.
**post-registration 处理**
tbss_2_reg生成了所有slice所有volume的订正文件, 命令tbss_3_postreg将所有订正后的数据固定到1x1x1mm的标准空间, 之后所有在标准空间的体素合并到4D的图像文件all_FA, 保存到新创建的文件夹stats中. 
FA均值图像mean_FA作为输入将在FA skeletonisation 程序中生成mean_FA_skeleton 
`tbss_3_postreg -S`
运行结束后, 进入stats文件夹查看生成的结果:
```
cd stats
fsleyes -std1mm mean_FA -cm red-yellow -dr 0.2 0.6 &
```
fsleyes的命令参数： -dr: display range ;  -cm: color map 
上述命令用fsleyes打开all_FA和mean_FA_skeleton, 将mean_FA_skeleton的颜色设置为绿色, 将显示范围设置为0.2-0.6
之后将所有对齐后的数据投影到skeleton上. 运行: 
`tbss_4_prestats 0.3`
其中0.3 是一个阈值, 用于设置binary skele mask. 
最后就可以做体素的统计分析了. 进入到stats文件夹, 运行Glm. 设置t检验参数: 
![这里写图片描述](https://img-blog.csdn.net/20180223154448561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
保存(名为design), 生成统计检验的设置文件. 后续主要需要用到的是design.mat 和design.con 这两个文件.
运行randomise命令,进行统计分析:
```
randomise -i all_FA_skeletonised -o tbss \
  -m mean_FA_skeleton_mask -d design.mat -t design.con -c 1.5
```
查看结果:
```
fsleyes -std1mm mean_FA_skeleton -cm green -dr .3 .7 \
  tbss_tstat1 -cm red-yellow -dr 1.5 3 \
  tbss_clustere_corrp_tstat1 -cm blue-lightblue -dr 0.949 1 &
```
参考： 
1、FSL 帮助文档：sample 
2、[http://www.diffusion-imaging.com/2015/10/dti-tutorial-2-normalization-and.html](http://www.diffusion-imaging.com/2015/10/dti-tutorial-2-normalization-and.html)
