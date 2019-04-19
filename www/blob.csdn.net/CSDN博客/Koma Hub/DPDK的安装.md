# DPDK的安装 - Koma Hub - CSDN博客
2018年10月13日 21:44:57[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：84
**目录**
[DPDK网址](#DPDK%E7%BD%91%E5%9D%80)
[下载](#%E4%B8%8B%E8%BD%BD)
[安装](#%E5%AE%89%E8%A3%85)
[1.解压](#1.%E8%A7%A3%E5%8E%8B)
[2.开启pcap](#2.%E5%BC%80%E5%90%AFpcap)
[3.安装依赖](#3.%E5%AE%89%E8%A3%85%E4%BE%9D%E8%B5%96)
[4.编译：](#4.%E7%BC%96%E8%AF%91%EF%BC%9A)
[5.后续](#5.%E5%90%8E%E7%BB%AD)
## DPDK网址
[https://www.dpdk.org/](https://www.dpdk.org/)
## 下载
下载地址：[https://core.dpdk.org/download/](https://core.dpdk.org/download/)
![](https://img-blog.csdn.net/20181013211416362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 安装
### 1.解压
```
$ls
dpdk-18.02.2.tar.xz
$ tar xf dpdk-18.02.2.tar.xz 
$ ls
dpdk-18.02.2.tar.xz  dpdk-stable-18.02.2
$ cd dpdk-stable-18.02.2/
$ ls
app         devtools  examples     license      meson.build        pkg     usertools
buildtools  doc       GNUmakefile  MAINTAINERS  meson_options.txt  README
config      drivers   lib          Makefile     mk                 test
```
### 2.开启pcap
```
$ make config T=x86_64-native-linuxapp-gcc
Configuration done using x86_64-native-linuxapp-gcc
$ sed -ri 's,(PMD_PCAP=).*,\1y,' build/.config
```
### 3.安装依赖
安装依赖numactl，libpcap：
`$sudo yum install numactl-devel*x86_64 ​libpcap*`
### 4.编译：
`$make`
### 5.后续
```
$ sudo mkdir -p /mnt/huge
$ sudo mount -t hugetlbfs nodev /mnt/huge
$ su
# echo 64 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages
```
网址：[http://core.dpdk.org/doc/quick-start/](http://core.dpdk.org/doc/quick-start/)
