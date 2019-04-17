# 编译Valgrind arm交叉编译 - DoubleLi - 博客园






1. 下载源码：

http://valgrind.org/downloads/valgrind-3.9.0.tar.bz2



2. 加压缩：

mkdir sw

cd sw

tar zxf valgrind-3.9.0.tar.bz2



3. 修改代码：

cd valgrind-3.9.0/

vim coregrind/m_debuginfo/readstabs.c

第57行：注释掉

//#include <-a.out.h>

然后添加：

struct nlist {

  union {

    char *n_name;

    struct nlist*n_next;

    long n_strx;

  } n_un;

  unsigned char n_type;

  char n_other;

  short n_desc;

  unsigned long n_value;

};



解决Valgrind编译时依赖一些列a.out.h头文件问题，

以上代码，是从系统：/usr/include/a.out.h文件中，将nlist结构体拷贝过来。



4. 开始编译：

export PATH=:$PATH

./configure \

  --prefix=/home/nianzhong/sw/install_vg\

  --host=mipsel-linux \

  --build=i686-linux \

  CPPFLAGS=-I 



make 

make install



5. 部署运行：

将install_vg/拷贝到nfs/目录下。

进入嵌入式系统，mount服务器中的nfs目录到嵌入式系统中：

mount -t nfs -o vers=2,nolock /mnt/nfs/

export VALGRIND_LIB=/mnt/nfs/install_vg/lib/valgrind

/mnt/nfs/install_vg/bin/valgrind ls -l



export VALGRIND_LIB用于解决错误:

valgrind: failed to start tool 'memcheck' for platform: nosuch file or directory









