# Solve fatal error: helper_math.h: No such file or directory - Grandyang - 博客园







# [Solve fatal error: helper_math.h: No such file or directory](https://www.cnblogs.com/grandyang/p/9982126.html)







When the 'fatal error: helper_math.h: No such file or directory' occurs, it means the 'helper_math.h' file is missing.

In fact, 'helper_math.h' locates at '/usr/local/cuda/samples/common/inc/helper_math.h' if you've installed cuda successfully, so just copy the missing file to where the error occurs.



```
cp /usr/local/cuda/samples/common/inc/helper_math.h [Where the error occurs]
```














