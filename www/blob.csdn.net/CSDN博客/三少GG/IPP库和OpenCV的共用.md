# IPP库和OpenCV的共用 - 三少GG - CSDN博客
2012年02月27日 13:24:06[三少GG](https://me.csdn.net/scut1135)阅读数：2375
[http://software.intel.com.sixxs.org/sites/products/documentation/hpc/ipp/ippi/ippi_ch11/functn_Mean.html](http://software.intel.com.sixxs.org/sites/products/documentation/hpc/ipp/ippi/ippi_ch11/functn_Mean.html)
# Mean
Computes the mean of image pixel values.
## Syntax
**Case 1: Operation on one-channel integer data**
IppStatus ippiMean_<mod>(const Ipp<datatype>* pSrc,
 int srcStep, IppiSize roiSize,
 Ipp64f* pMean);
Supported values for mod:
|8u_C1R|16u_C1R|16s_C1R|
**Case 2: Operation on one-channel floating-point data**
IppStatus ippiMean_32f_C1R(const Ipp32f* pSrc,
 int srcStep, IppiSize roiSize,
 Ipp64f* pMean, IppHintAlgorithm hint);

相关函数documentation
1.
**[Intel® Integrated Performance Primitives – Documentation](http://software.intel.com/en-us/articles/intel-integrated-performance-primitives-documentation/)**
**2.**
# [Intel® Atom™ Processors support in the Intel® Integrated Performance Primitives (Intel® IPP) Library](http://software.intel.com.sixxs.org/en-us/articles/new-atom-support/?wapkw=ippiSqr_32f_C1R%20software.intel.com)
