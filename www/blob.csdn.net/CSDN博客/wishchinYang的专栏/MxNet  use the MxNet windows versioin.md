# MxNet : use the MxNet windows versioin - wishchinYang的专栏 - CSDN博客
2016年08月01日 15:02:13[wishchin](https://me.csdn.net/wishchin)阅读数：678
个人分类：[ANN/DNN/纤维丛](https://blog.csdn.net/wishchin/article/category/3109825)
The MxNet needs  the following thirdparties:
1. lapack
     complie lapack-3.6.1:
     download the lapack-3.6.1 sourcecode: [http://www.netlib.org/lapack/#_lapack_for_windows](http://www.netlib.org/lapack/#_lapack_for_windows)
  1.2.download intel fortran compiler: [http://tieba.baidu.com/p/2753187458](http://tieba.baidu.com/p/2753187458)
[http://registrationcenter-download.intel.com/akdlm/irc_nas/3651/w_fcompxe_novsshell_online_2013_sp1.1.139.exe](http://registrationcenter-download.intel.com/akdlm/irc_nas/3651/w_fcompxe_novsshell_online_2013_sp1.1.139.exe)
     add the cmake complier :  C:\Program Files (x86)\Intel\Composer XE 2013 SP1    to cmake
     Configure and Generate the VS slution;
     Complie............................
2.**suitesparse**
    A brief Page:    [http://faculty.cse.tamu.edu/davis/suitesparse.html](http://faculty.cse.tamu.edu/davis/suitesparse.html)
    down the **suitesparse** from  [https://github.com/jlblancoc/suitesparse-metis-for-windows](https://github.com/jlblancoc/suitesparse-metis-for-windows)
    replace the libquadmath-0.dll by the new generated **libquadmath-0.dll**;
    compile the **suitesparse By cmaker directly.**
3. complile the MxNet:
    ...................................
4. Run a sample   
      Go to the sample Page : 
﻿﻿
