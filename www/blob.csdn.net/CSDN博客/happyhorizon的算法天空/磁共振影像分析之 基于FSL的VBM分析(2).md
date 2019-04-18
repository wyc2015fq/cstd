# 磁共振影像分析之: 基于FSL的VBM分析(2) - happyhorizon的算法天空 - CSDN博客
2018年05月31日 19:52:59[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：1033
# 统计分析
生成好study-specific模板, 下来要做统计分析组间差异了. 如果用fsl提供的vbm方案, 这一步对应的就是`fslvbm_3_proc`这个命令了.
## fslvbm_3_proc
这一步和实验设计和研究问题本身很有关系. 这里讲讲`fslvbm_3_proc`命令的内部过程吧. 
这个文件保存在 `$FSLDIR/bin`中, 可以用vim查看`vim $FSLDIR/bin/fslvbm_3_proc`.  shell脚本的主体如下(省略帮助和说明文字)
```bash
#!/bin/sh
export LC_ALL=C
echo [`date`] [`hostname`] [`uname -a`] [`pwd`] [$0 $@] >> .fslvbmlog
mkdir -p stats
cd struc
echo "Now running the preprocessing steps and the pre-analyses"
/bin/rm -f fslvbm3a
for g in `$FSLDIR/bin/imglob *_struc.*` ; do
  echo $g
  echo "${FSLDIR}/bin/fsl_reg ${g}_GM template_GM ${g}_GM_to_template_GM -fnirt \"--config=GM_2_MNI152GM_2mm.cnf --jout=${g}_JAC_nl\"; \
        $FSLDIR/bin/fslmaths ${g}_GM_to_template_GM -mul ${g}_JAC_nl ${g}_GM_to_template_GM_mod -odt float" >> fslvbm3a
done
chmod a+x fslvbm3a
fslvbm3a_id=`${FSLDIR}/bin/fsl_sub -T 40 -N fslvbm3a -t ./fslvbm3a`
echo Doing registrations: ID=$fslvbm3a_id
cd ../stats
cat <<stage_preproc2 > fslvbm3b
#!/bin/sh
\$FSLDIR/bin/imcp ../struc/template_GM template_GM
\$FSLDIR/bin/fslmerge -t GM_merg     \`\${FSLDIR}/bin/imglob ../struc/*_GM_to_template_GM.*\`
\$FSLDIR/bin/fslmerge -t GM_mod_merg \`\${FSLDIR}/bin/imglob ../struc/*_GM_to_template_GM_mod.*\`
\$FSLDIR/bin/fslmaths GM_merg -Tmean -thr 0.01 -bin GM_mask -odt char
/bin/cp ../design.* .
for i in GM_mod_merg ; do
  for j in 2 3 4 ; do
    \$FSLDIR/bin/fslmaths \$i -s \$j \${i}_s\${j} 
    \$FSLDIR/bin/randomise -i \${i}_s\${j} -o \${i}_s\${j} -m GM_mask -d design.mat -t design.con -V
  done
done
stage_preproc2
chmod a+x fslvbm3b
fslvbm3b_id=`${FSLDIR}/bin/fsl_sub -T 15 -N fslvbm3b -j $fslvbm3a_id ./fslvbm3b`
echo Doing subject concatenation and initial randomise: ID=$fslvbm3b_id
echo "Once this has finished, run randomise with 5000 permutations on the 'best' smoothed 4D GM_mod_merg. We recommend using the -T (TFCE) option. For example:"
echo "randomise -i GM_mod_merg_s3 -o GM_mod_merg_s3 -m GM_mask -d design.mat -t design.con -n 5000 -T -V"
```
这个shell脚本中, 采用cat命令配合EOF符号生成了另两个脚本: fslvbm3a 和fslvbm3b . 值得一提的是, 这里采用`fsl_sub`命令调用并行计算进行计算加速. 但是这个命令是基于Sun grid cluster的, 所以对于普通的台式机, 或者工作站, 建议将这一步去掉, 直接运行fslvbm3a或者fslvbm3b. 
我们再从头看看这个脚本. 首先来说说生成脚本fslvbm3a的部分. 
## fslvbm3a
这一步是对统计分析前的准备工作. 
使用`fsl_reg`将所有灰质图像和study-specific的模板配准, 生成`${g}_GM_to_template_GM`,   之后乘以`Jac_nl`, 生成`${g}_GM_to_template_GM_mod`${g}是指被试样本的名称. 
## fslvbm3b
1) 进入states 文件夹, 拷贝template_GM到该文件夹.
2) 用fslmerge将(../struc中的所有_GM_to_template_GM)图像沿着”时间”轴拼接concatenate到一起, 生成GM_merge 
同时, 将(../struc中所有_GM_to_template_GM_mod)图像沿着”时间”轴concatenate到一起, 生成GM_mod_merg. 
当然, 这里的”时间”轴, 只是一个序号而已. 并不没有时间属性.  以下是fslmerge命令的用法: 
```
Usage: fslmerge <-x/y/z/t/a/tr> <output> <file1 file2 .......> [tr value in seconds]
     -t : concatenate images in time
     -x : concatenate images in the x direction
     -y : concatenate images in the y direction
     -z : concatenate images in the z direction
     -a : auto-choose: single slices -> volume, volumes -> 4D (time series)
     -tr : concatenate images in time and set the output image tr to the final option value
```
3) 对GM_merg 所有图像中信号强度是否0.01做binary(非0即1)的voxel沿着”时间”轴做平均, 设置输出的结果为字符char.  
`fslmaths GM_merg -Tmean -thr 0.01 -bin GM_mask -odt char`
4) 将design.*文件全部拷贝到./stat中. 之后对GM_mod_merg中的每个图片, 
 对每个图片做高斯平滑, 分别选择平滑核宽度为2mm 3mm 和4mm. 然后用randomise命令做非参数统计. 将刚才进行过高斯平滑的图像作为输入, 输出文件名称为`${g}_s${j}`${g}为被试文件名称, ${j}是高斯核宽度. GM_mask 是前一步生成的mask文件, -d后面是设计文件design.mat, -t后面紧跟着的是t检验设置文件,design.con 最后的-V是指在t检验时使用方差平滑.
```
fslmaths \$i -s \$j \${i}_s\${j} 
\$FSLDIR/bin/randomise -i \${i}_s\${j} -o \${i}_s\${j} -m GM_mask -d design.mat -t design.con -V
```
# 附录:
## 1) fslmaths的基本调用格式如下:
`fslmaths [-dt <datatype>] <first_input> [operations and inputs] <output> [-odt <datatype>]`
```
fslmaths 
Usage: fslmaths [-dt <datatype>] <first_input> [operations and inputs] <output> [-odt <datatype>]
Datatype information:
 -dt sets the datatype used internally for calculations (default float for all except double images)
 -odt sets the output datatype ( default is float )
 Possible datatypes are: char short int float double input
 "input" will set the datatype to that of the original image
Binary operations:
  (some inputs can be either an image or a number)
 -add   : add following input to current image
 -sub   : subtract following input from current image
 -mul   : multiply current image by following input
 -div   : divide current image by following input
 -rem   : modulus remainder - divide current image by following input and take remainder
 -mas   : use (following image>0) to mask current image
 -thr   : use following number to threshold current image (zero anything below the number)
 -thrp  : use following percentage (0-100) of ROBUST RANGE to threshold current image (zero anything below the number)
 -thrP  : use following percentage (0-100) of ROBUST RANGE of non-zero voxels and threshold below
 -uthr  : use following number to upper-threshold current image (zero anything above the number)
 -uthrp : use following percentage (0-100) of ROBUST RANGE to upper-threshold current image (zero anything above the number)
 -uthrP : use following percentage (0-100) of ROBUST RANGE of non-zero voxels and threshold above
 -max   : take maximum of following input and current image
 -min   : take minimum of following input and current image
 -seed  : seed random number generator with following number
 -restart : replace the current image with input for future processing operations
 -save : save the current working image to the input filename
Basic unary operations:
 -exp   : exponential
 -log   : natural logarithm
 -sin   : sine function
 -cos   : cosine function
 -tan   : tangent function
 -asin  : arc sine function
 -acos  : arc cosine function
 -atan  : arc tangent function
 -sqr   : square
 -sqrt  : square root
 -recip : reciprocal (1/current image)
 -abs   : absolute value
 -bin   : use (current image>0) to binarise
 -binv  : binarise and invert (binarisation and logical inversion)
 -fillh : fill holes in a binary mask (holes are internal - i.e. do not touch the edge of the FOV)
 -fillh26 : fill holes using 26 connectivity
 -index : replace each nonzero voxel with a unique (subject to wrapping) index number
 -grid <value> <spacing> : add a 3D grid of intensity <value> with grid spacing <spacing>
 -edge  : edge strength
 -tfce <H> <E> <connectivity>: enhance with TFCE, e.g. -tfce 2 0.5 6 (maybe change 6 to 26 for skeletons)
 -tfceS <H> <E> <connectivity> <X> <Y> <Z> <tfce_thresh>: show support area for voxel (X,Y,Z)
 -nan   : replace NaNs (improper numbers) with 0
 -nanm  : make NaN (improper number) mask with 1 for NaN voxels, 0 otherwise
 -rand  : add uniform noise (range 0:1)
 -randn : add Gaussian noise (mean=0 sigma=1)
 -inm <mean> :  (-i i ip.c) intensity normalisation (per 3D volume mean)
 -ing <mean> :  (-I i ip.c) intensity normalisation, global 4D mean)
 -range : set the output calmin/max to full data range
Matrix operations:
 -tensor_decomp : convert a 4D (6-timepoint )tensor image into L1,2,3,FA,MD,MO,V1,2,3 (remaining image in pipeline is FA)
Kernel operations (set BEFORE filtering operation if desired):
 -kernel 3D : 3x3x3 box centered on target voxel (set as default kernel)
 -kernel 2D : 3x3x1 box centered on target voxel
 -kernel box    <size>     : all voxels in a cube of width <size> mm centered on target voxel
 -kernel boxv   <size>     : all voxels in a cube of width <size> voxels centered on target voxel, CAUTION: size should be an odd number
 -kernel boxv3  <X> <Y> <Z>: all voxels in a cuboid of dimensions X x Y x Z centered on target voxel, CAUTION: size should be an odd number
 -kernel gauss  <sigma>    : gaussian kernel (sigma in mm, not voxels)
 -kernel sphere <size>     : all voxels in a sphere of radius <size> mm centered on target voxel
 -kernel file   <filename> : use external file as kernel
Spatial Filtering operations: N.B. all options apart from -s use the default kernel or that _previously_ specified by -kernel
 -dilM    : Mean Dilation of non-zero voxels
 -dilD    : Modal Dilation of non-zero voxels
 -dilF    : Maximum filtering of all voxels
 -dilall  : Apply -dilM repeatedly until the entire FOV is covered
 -ero     : Erode by zeroing non-zero voxels when zero voxels found in kernel
 -eroF    : Minimum filtering of all voxels
 -fmedian : Median Filtering 
 -fmean   : Mean filtering, kernel weighted (conventionally used with gauss kernel)
 -fmeanu  : Mean filtering, kernel weighted, un-normalised (gives edge effects)
 -s <sigma> : create a gauss kernel of sigma mm and perform mean filtering
 -subsamp2  : downsamples image by a factor of 2 (keeping new voxels centred on old)
 -subsamp2offc  : downsamples image by a factor of 2 (non-centred)
Dimensionality reduction operations:
  (the "T" can be replaced by X, Y or Z to collapse across a different dimension)
 -Tmean   : mean across time
 -Tstd    : standard deviation across time
 -Tmax    : max across time
 -Tmaxn   : time index of max across time
 -Tmin    : min across time
 -Tmedian : median across time
 -Tperc <percentage> : nth percentile (0-100) of FULL RANGE across time
 -Tar1    : temporal AR(1) coefficient (use -odt float and probably demean first)
Basic statistical operations:
 -pval    : Nonparametric uncorrected P-value, assuming timepoints are the permutations; first timepoint is actual (unpermuted) stats image
 -pval0   : Same as -pval, but treat zeros as missing data
 -cpval   : Same as -pval, but gives FWE corrected P-values
 -ztop    : Convert Z-stat to (uncorrected) P
 -ptoz    : Convert (uncorrected) P to Z
 -rank    : Convert data to ranks (over T dim)
 -ranknorm: Transform to Normal dist via ranks
Multi-argument operations:
 -roi <xmin> <xsize> <ymin> <ysize> <zmin> <zsize> <tmin> <tsize> : zero outside roi (using voxel coordinates). Inputting -1 for a size will set it to the full image extent for that dimension.
 -bptf  <hp_sigma> <lp_sigma> : (-t in ip.c) Bandpass temporal filtering; nonlinear highpass and Gaussian linear lowpass (with sigmas in volumes, not seconds); set either sigma<0 to skip that filter
 -roc <AROC-thresh> <outfile> [4Dnoiseonly] <truth> : take (normally binary) truth and test current image in ROC analysis against truth. <AROC-thresh> is usually 0.05 and is limit of Area-under-ROC measure FP axis. <outfile> is a text file of the ROC curve (triplets of values: FP TP threshold). If the truth image contains negative voxels these get excluded from all calculations. If <AROC-thresh> is positive then the [4Dnoiseonly] option needs to be set, and the FP rate is determined from this noise-only data, and is set to be the fraction of timepoints where any FP (anywhere) is seen, as found in the noise-only 4d-dataset. This is then controlling the FWE rate. If <AROC-thresh> is negative the FP rate is calculated from the zero-value parts of the <truth> image, this time averaging voxelwise FP rate over all timepoints. In both cases the TP rate is the average fraction of truth=positive voxels correctly found.
Combining 4D and 3D images:
 If you apply a Binary operation (one that takes the current image and a new image together), when one is 3D and the other is 4D,
 the 3D image is cloned temporally to match the temporal dimensions of the 4D image.
e.g. fslmaths inputVolume -add inputVolume2 output_volume
     fslmaths inputVolume -add 2.5 output_volume
     fslmaths inputVolume -add 2.5 -mul inputVolume2 output_volume
     fslmaths 4D_inputVolume -Tmean -mul -1 -add 4D_inputVolume demeaned_4D_inputVolume
```
## 2) randomise 非参数统计命令
randomise的详细使用帮助: [https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/Randomise](https://fsl.fmrib.ox.ac.uk/fsl/fslwiki/Randomise)
```
randomise
Part of FSL (ID: 5.0.10)
randomise v2.9
Usage: 
randomise -i <input> -o <output> -d <design.mat> -t <design.con> [options]
Compulsory arguments (You MUST set one or more of):
    -i <input>  4D input image
    -o <out_root>   output file-rootname
Optional arguments (You may optionally specify one or more of):
    -D      demean data temporally before model fitting ( demean model as well if required )
    -1      perform 1-sample group-mean test instead of generic permutation test
    -m <mask>   mask image
    -d <design.mat> design matrix file
    -t <design.con> t contrasts file
    -f <design.fts> f contrasts file
    -e <design.grp> exchangeability block labels file
    --effective_design <design2.mat>    alternative design for determining valid permutations
    -q      print out how many unique permutations would be generated and exit
    -Q      print out information required for parallel mode and exit
    -n <n_perm> number of permutations (default 5000, set to 0 for exhaustive)
    -x      output voxelwise corrected p-value images
    --fonly     calculate f-statistics only
    -T      carry out Threshold-Free Cluster Enhancement
    --T2        carry out Threshold-Free Cluster Enhancement with 2D optimisation (e.g. for TBSS data); H=2, E=1, C=26
    -c <thresh> carry out cluster-based thresholding
    -C <thresh> carry out cluster-mass-based thresholding
    -F <thresh> carry out f cluster thresholding
    -S <thresh> carry out f cluster-mass thresholding
    -v <std>    use variance smoothing for t-stats (std is in mm)
    -h,--help   display this message
    --quiet     switch off diagnostic messages
    --twopass   carry out cluster normalisation thresholding
    -R      output raw ( unpermuted ) statistic images
    --uncorrp   output uncorrected p-value images
    -P      output permutation vector text file
    -N      output null distribution text files
    --norcmask  don't remove constant voxels from mask
    --seed <seed>   specific integer seed for random number generator
    --tfce_H <H>    TFCE height parameter (default=2)
    --tfce_D <H>    TFCE delta parameter overide
    --tfce_E <E>    TFCE extent parameter (default=0.5)
    --tfce_C <C>    TFCE connectivity (6 or 26; default=6)
    --vxl       list of numbers indicating voxelwise EVs position in the design matrix (list order corresponds to files in vxf option). caution BETA option.
    --vxf       list of 4D images containing voxelwise EVs (list order corresponds to numbers in vxl option). caution BETA option.
    --permuteBlocks permute exchangeability blocks. Caution BETA option
    --glm_output    output glm information for t-statistics ( unpermuted case only )
    --film  output stats to simulate the output of film
```
