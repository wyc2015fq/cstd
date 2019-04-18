# 【Linux】扩展swap逻辑卷 - weixin_33985507的博客 - CSDN博客
2017年05月23日 09:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
- 准备好pv，vg；摘掉swap。
```
# swapoff -a
```
- 扩展swap的lv。
```
# lvextend  -l +10%free /dev/mapper/rhel-swap
# resize2fs /dev/rhel/swap
```
- 重新挂载swap
```
# mkswap /dev/mapper/rhel-swap 
# swapon /dev/mapper/rhel-swap 
# swapon -s
```
