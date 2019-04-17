# *** ValueError: some of the strides of a given numpy array are negative. This is currently not suppo - Snoopy_Dream - CSDN博客





2019年01月08日 12:27:23[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：207








内存地址不连续的问题

加一句np.ascontiguousarray就好了

  inputs6=np.ascontiguousarray(inputs6, dtype=np.float32)



