# centos6下手工编译vitess - weixin_33985507的博客 - CSDN博客
2017年07月26日 12:28:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
vitess是youtub开源的一款mysql代理，在ubuntu下编译非常方便。可是在centos下且不能訪问google的情况下坑比較多。近期依据其bootstrap.sh脚本手工编译成功。把过程贴处来供有兴趣的朋友參考一下。
升级automake
wget ftp://ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/home:/monkeyiq:/centos6updates/CentOS_CentOS-6/noarch/automake-1.13.4-3.2.noarch.rpm
wget [ftp://ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/home:/monkeyiq:/centos6updates/CentOS_CentOS-6/noarch/autoconf-2.69-12.2.noarch.rpm](http://ftp//ftp.pbone.net/mirror/ftp5.gwdg.de/pub/opensuse/repositories/home:/monkeyiq:/centos6updates/CentOS_CentOS-6/noarch/autoconf-2.69-12.2.noarch.rpm)
rpm -Uvh autoconf-2.69-12.2.noarch.rpm
rpm -Uvh automake-1.13.4-3.2.noarch.rpm
升级python至2.7，參考资料[http://jingyan.baidu.com/article/da1091fb3fb589027849d6d7.html](http://jingyan.baidu.com/article/da1091fb3fb589027849d6d7.html)
wget [https://www.python.org/ftp/python/2.7.10/Python-2.7.10.tgz](https://www.python.org/ftp/python/2.7.10/Python-2.7.10.tgz)
升级python-setuptools  root用户运行 wget https://bootstrap.pypa.io/ez_setup.py -O - | python 假设安装失败手动安装过程例如以下
wget[https://pypi.python.org/packages/source/s/setuptools/setuptools-17.0.tar.gz#md5=fd15d24ed1e4c95e56d8c6e812f3a84f](https://pypi.python.org/packages/source/s/setuptools/setuptools-17.0.tar.gz#md5=fd15d24ed1e4c95e56d8c6e812f3a84f)
tar zxvf setuptools-17.0.tar.gz
cd setuptools-17.0
python ez_setup.py 
升级pip到7.0.1 wget [https://bootstrap.pypa.io/get-pip.py](https://bootstrap.pypa.io/get-pip.py)；python get-pip.py
安装virtualenv13.0.1
 ；pip install virtualenv
ln -s /usr/local/bin/python2.7 /usr/bin/python2.7
-------------------------------------------------------------
. ./dev.env
安装zookeeper
rm -fr $VTROOT/dist/vt-zookeeper-3.3.5
cd $VTTOP/third_party/zookeeper
tar -xjf zookeeper-3.3.5.tbz
mkdir -p $VTROOT/dist/vt-zookeeper-3.3.5/lib
cp zookeeper-3.3.5/contrib/fatjar/zookeeper-3.3.5-fatjar.jar $VTROOT/dist/vt-zookeeper-3.3.5/lib
cd zookeeper-3.3.5/src/c 
./configure --prefix=$VTROOT/dist/vt-zookeeper-3.3.5
make -j3 install
cd $VTTOP/third_party/zookeeper
rm -fr zookeeper-3.3.5
# install protoc and proto python libraries
protobuf_dist=$VTROOT/dist/protobuf
rm -rf  $VTROOT/dist/protobuf
cd $VTROOT
mkdir -p $VTROOT/dist/protobuf/lib/python2.7/site-packages
export PYTHONPATH=$VTROOT/dist/protobuf/lib/python2.7/site-packages:$PYTHONPATH
參考./travis/install_protobuf.sh运行例如以下操作
wget[https://github.com/google/protobuf/archive/v3.0.0-alpha-1.tar.gz](https://github.com/google/protobuf/archive/v3.0.0-alpha-1.tar.gz)
tar zxvf [v3.0.0-alpha-1.tar.gz](https://github.com/google/protobuf/archive/v3.0.0-alpha-1.tar.gz)
cd $VTROOT/protobuf-3.0.0-alpha-1
自带梯子，下载并解压gtest在$VTROOT/protobuf-3.0.0-alpha-1文件夹
./autogen.sh $VTROOT/dist/protobuf
./configure --prefix=$VTROOT/dist/protobuf
cd python/
python setup.py install
# install gRPC C++ base, so we can install the python adapters
grpc_dist=$VTROOT/dist/grpc
  rm -rf $VTROOT/dist/grpc
  mkdir -p $VTROOT/dist/grpc
 參考 ./travis/install_grpc.sh 运行
git clone[https://github.com/grpc/grpc.git](https://github.com/grpc/grpc.git)
cd grpc
git submodule update --init
make install prefix=$VTROOT/dist/grpc
pip install -r src/python/requirements.txt -t $VTROOT/dist/grpc/lib/python2.7/site-packages
CFLAGS=-I$VTROOT/dist/grpc/include LDFLAGS=-L$VTROOT/dist/grpc/lib pip install --upgrade src/python/src -t $VTROOT/dist/grpc/lib/python2.7/site-packages
ln -nfs $VTTOP/third_party/go/launchpad.net $VTROOT/src
go install launchpad.net/gozk/zookeeper
go get -u github.com/golang/protobuf/proto
go get -u github.com/golang/glog
go get -u -a github.com/golang/protobuf/protoc-gen-go
下面go package在墙外须要手工下载编译，golang中国提供对应打包下载[http://www.golangtc.com/download/package](http://www.golangtc.com/download/package)
go get -u golang.org/x/net/context
go get -u golang.org/x/tools/cmd/goimports
go get -u github.com/golang/lint/golint
go get -u github.com/tools/godep
go get -u google.golang.org/grpc
go get -u github.com/modocache/gover
go get -u github.com/mattn/goveralls
ln -snf $VTTOP/config $VTROOT/config
ln -snf $VTTOP/data $VTROOT/data
ln -snf $VTTOP/py $VTROOT/py-vtdb
ln -snf $VTTOP/go/zk/zkctl/zksrv.sh $VTROOT/bin/zksrv.sh
ln -snf $VTTOP/test/vthook-test.sh $VTROOT/vthook/test.sh
echo "$MYSQL_FLAVOR" > $VTROOT/dist/MYSQL_FLAVOR
cp $VTTOP/config/gomysql.pc.tmpl $VTROOT/lib/gomysql.pc
echo "Version:" "$($VT_MYSQL_ROOT/bin/mysql_config --version)" >> $VTROOT/lib/gomysql.pc
echo "Cflags:" "$($VT_MYSQL_ROOT/bin/mysql_config --cflags) -ggdb -fPIC" >> $VTROOT/lib/gomysql.pc
echo "Libs:" "$($VT_MYSQL_ROOT/bin/mysql_config --libs_r)" >> $VTROOT/lib/gomysql.pc
# install bson
cd $VTTOP/third_party/py/bson-0.3.2
python ./setup.py install --prefix=$VTROOT/dist/py-vt-bson-0.3.2
rm -r build
# install cbson
cd $VTTOP/py/cbson
python ./setup.py install --prefix=$VTROOT/dist/py-cbson
# create pre-commit hooks
ln -sf $VTTOP/misc/git/pre-commit $VTTOP/.git/hooks/pre-commit
