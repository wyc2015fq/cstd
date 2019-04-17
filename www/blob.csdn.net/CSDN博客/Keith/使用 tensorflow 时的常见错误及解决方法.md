# 使用 tensorflow 时的常见错误及解决方法 - Keith - CSDN博客





2017年04月26日 14:12:45[ke1th](https://me.csdn.net/u012436149)阅读数：2659








记录下使用`tensorflow`时采的坑.
- `Profiling: libcupti.so cannot be loaded`这是使用`run_options = tf.RunOptions(trace_level = tf.RunOptions.FULL_TRACE)`碰到的错误. 
**解决方法:**`Appending /usr/local/cuda/extras/CUPTI/lib64 to LD_LIBRARY_PATH environment variable`



