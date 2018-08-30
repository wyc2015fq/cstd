#/bin/bash
#m libfacefeature.so test_libfacefeature libfacefeature.tar.gz
#gcc  libfacefeatue.c -o3 -fPIC -shared -lm -o libfacefeature.so -I/mnt/hgfs/E/code/cstd/include -I/mnt/hgfs/E/code/cstd
#gcc  test_libfacefeatue.c -lfacefeature -o test_libfacefeature -L. -I/mnt/hgfs/E/code/cstd/include -I/mnt/hgfs/E/code/cstd
#cp test_libfacefeatue.c libfacefeature.so test_libfacefeature /mnt/hgfs/E/pub/cstd/libcd /mnt/hgfs/E/pub/cstd/lib
#cd /mnt/hgfs/E/pub/cstd/lib
#chmod 777 test_libfacefeatue
#./test_libfacefeatue 
#ta -cvf libfacefeature.tar.gz *libfacefeature*.* adfasdf.bmp >nul


CC1=/home/yw/ti-processor-sdk-linux-am437x-evm-02.00.02.11/linux-devkit/sysroots/x86_64-arago-linux/usr/bin
CC=${CC1}  -I/mnt/hgfs/E/code/cstd/include -I/mnt/hgfs/E/code/cstd
echo ${CC}