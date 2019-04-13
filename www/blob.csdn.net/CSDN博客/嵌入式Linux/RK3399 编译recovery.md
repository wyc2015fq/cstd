
# RK3399 编译recovery - 嵌入式Linux - CSDN博客

2018年11月05日 14:03:32[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：229



```python
#!/bin/bash
set -e
. build/envsetup.sh >/dev/null && setpaths
export PATH=$ANDROID_BUILD_PATHS:$PATH
TARGET_PRODUCT=`get_build_var TARGET_PRODUCT`
TARGET_HARDWARE=`get_build_var TARGET_BOARD_HARDWARE`
TARGET_BOARD_PLATFORM=`get_build_var TARGET_BOARD_PLATFORM`
TARGET_DEVICE_DIR=`get_build_var TARGET_DEVICE_DIR`
PLATFORM_VERSION=`get_build_var PLATFORM_VERSION`
PLATFORM_SECURITY_PATCH=`get_build_var PLATFORM_SECURITY_PATCH`
TARGET_BUILD_VARIANT=`get_build_var TARGET_BUILD_VARIANT`
BOARD_SYSTEMIMAGE_PARTITION_SIZE=`get_build_var BOARD_SYSTEMIMAGE_PARTITION_SIZE`
BOARD_USE_SPARSE_SYSTEM_IMAGE=`get_build_var BOARD_USE_SPARSE_SYSTEM_IMAGE`
BOARD_CONFIG=device/rockchip/common/device.mk
KERNEL_SRC_PATH=`grep TARGET_PREBUILT_KERNEL ${BOARD_CONFIG} |grep "^\s*TARGET_PREBUILT_KERNEL *:= *[\w]*\s" |awk  '{print $3}'`
echo TARGET_PRODUCT=$TARGET_PRODUCT
echo TARGET_DEVICE_DIR=$TARGET_DEVICE_DIR
echo KERNEL_SRC_PATH=$KERNEL_SRC_PATH
echo $OUT
IMAGE_PATH=rockdev/Image-recovery-$TARGET_PRODUCT
rm -rf $IMAGE_PATH
mkdir -p $IMAGE_PATH
echo $IMAGE_PATH
FSTYPE=ext4
echo system filesysystem is $FSTYPE

make recovery -j40
echo -n "create recovery.img with kernel and resource... "
[ -d $OUT/recovery/root  ] && \
mkbootfs $OUT/recovery/root | minigzip > $OUT/ramdisk-recovery.img && \
truncate -s "%4" $OUT/ramdisk-recovery.img && \
mkbootimg --kernel $OUT/kernel --ramdisk $OUT/ramdisk-recovery.img --second kernel/resource.img --os_version $PLATFORM_VERSION --os_patch_level $PLATFORM_SECURITY_PATCH --cmdline buildvariant=$TARGET_BUILD_VARIANT --output $OUT/recovery.img && \
cp -a $OUT/recovery.img $IMAGE_PATH/
echo "done."
```
注意kernel的位置，最好看下kernel的位置后再编译

