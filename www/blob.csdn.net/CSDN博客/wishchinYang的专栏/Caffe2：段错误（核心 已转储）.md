# Caffe2：段错误（核心 已转储） - wishchinYang的专栏 - CSDN博客
2018年01月26日 16:53:17[wishchin](https://me.csdn.net/wishchin)阅读数：1444
         测试Caffe的时候，
`cd ~ && python -c 'from caffe2.python import core' 2>/dev/null && echo "Success" || echo "Failure"`
         出现了 段错误（核心 已转储） 错误。
 修改方法：
        使用了387版本的驱动程序，把387版本链接库目录加入路径
        export LD_LIBRARY_PATH=/usr/lib/nvidia-387/:$LD_LIBRARY_PATH
重新测试
       success
添加之后，不再出现：Caffe不支持GPU的错误
**注意事项**：段转储错误是dll文件寻找不到或者找错造成，此方法不一定能完全排除错误，需要根据错误提示具体分析！
