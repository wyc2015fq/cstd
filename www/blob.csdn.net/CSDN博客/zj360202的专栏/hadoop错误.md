# hadoop错误 - zj360202的专栏 - CSDN博客





2014年10月11日 18:16:51[zj360202](https://me.csdn.net/zj360202)阅读数：620








put: io.bytes.per.checksum(512) and blockSize(53687091) do not match. blockSize should be a multiple of io.bytes.per.checksum




如果指定的bytesPerChecksum小于1或者blockSize不能[整除](http://cfanz.cn/index.php?c=search&key=%E6%95%B4%E9%99%A4)bytesPerChecksum,




blockSize必须是io.bytes.per.checksum的整数倍




