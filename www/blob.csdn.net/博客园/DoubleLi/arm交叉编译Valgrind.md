# arm交叉编译Valgrind - DoubleLi - 博客园






1. 

wget http://valgrind.org/downloads/valgrind-3.9.0.tar.bz2

tar xvf valgrind-3.9.0.tar.bz2

cd valgrind-3.9.0

apt-get install automake

./autogen.sh



2. 

修改configure
armv7*) 改成 armv7*|arm)



3. 


./configure --host=arm-linux CC=arm-none-linux-gnueabi-gcc CPP=arm-none-linux-gnueabi-cpp CXX=arm-none-linux-gnueabi-g++ --prefix=./valgrind



4.


make

make install



注意：--prefix=/opt/Valgrind指定的目录要与开发板上放置的目录一致，不然运行valgrind时可能会出现“valgrind: failed to start tool 'memcheck' for platform 'arm-linux': No such file or directory”错误。









