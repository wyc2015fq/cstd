# 常见bug - qq_30006593的博客 - CSDN博客





2017年11月14日 10:05:30[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：755








THCUNN/ClassNLLCriterion.cu:57: void cunn_ClassNLLCriterion_updateOutput_kernel：class num 设置不对 

RuntimeError: some of the strides of a given numpy array are negative. This is currently not supported, but will be added in future releases.：transpose后应该返回copy（）



