# caffe_pb2.py TypeError: __init__() got an unexpected keyword argument 'syntax - Snoopy_Dream - CSDN博客





2018年07月30日 11:48:54[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：589








你会看到提示caffe_pb2.py那些各种xx有点像乱码的错误，其实是编码格式的问题。

This is caused by difference of arguments of FileDescriptor (and other Desctiptors)

主要是protobuf版本的问题。caffe只支持的到2.×版本。

### **1.首先查看版本**
`protobuf --vertion`
如果是3.×，卸载
`sudo pip uninstall protobuf`
再次查看版本 
`protobuf --vertion`
如果还是显示的老版本号，**手动在/usr/local/lib还有/usr/lib下删除protoc文件。**

### **2.下载老版本**

[https://github.com/google/protobuf/releases/tag/v2.6.1](https://github.com/google/protobuf/releases/tag/v2.6.1)

```
tar -zxvf protobuf-2.6.1.tar.gz # 解压

sudo apt-get install build-essential # 不装会报错

cd protobuf-2.6.1/ # 进入目录

./configure # 配置安装文件

make -j8# 编译

make check -j8 # 检测编译安装的环境

sudo make install -j8# 安装
```
- 

## 3. 检查是否安装成功
`protoc --version`- 若成功则显示：
`libprotoc 2.6.1`
可能会出现错误或者还是显示的老版本号，错误原因：protobuf的默认安装路径是/usr/local/lib，而/usr/local/lib不在ubuntu体系默认的LD_LIBRARY_PATH里，所以就找不到lib

**    解决办法：**

    1.打开~/.profile # 打开配置文件
-   在文件最后添加：`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib`

    2.保存退出，
`source ~/.profile`-   执行上面命令

    3.然后，查看版本号
`protoc --version`


### 4.重新编译caffe

```
make -j8
make pycaffe -j8
```



