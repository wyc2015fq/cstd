# kaldi使用的相关问题和解决 - xmdxcsj的专栏 - CSDN博客





2016年03月05日 11:22:06[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1354








一、编译相关

1.libcublas.so.7.5: cannot open shared object file: No such file or directory

原因：使用的kaldi是从GPU机器上面复制来

解决：需要将kaldi在CPU机器上面重新进行编译




2.GPU编译过的kaldi在CPU上面编译报错路径不对

cd tools

rm -rf openfest

二、kws相关

1.安装F4DE

sudo yum install perl-CPAN




sudo perl -MCPAN -e shell

install YAML



