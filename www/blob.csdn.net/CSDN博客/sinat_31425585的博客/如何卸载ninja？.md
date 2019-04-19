# 如何卸载ninja？ - sinat_31425585的博客 - CSDN博客
2016年10月17日 18:44:57[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：520
前段时间，跑了一下谷歌开源的Cartographer，安装了ninja，现在想删除，找了一圈，终于找到资源了，参考[卸载ninja](https://installion.co.uk/ubuntu/vivid/main/n/ninja-build/uninstall/index.html)
1. 仅仅卸载ninja-build:
```cpp
$ sudo apt-get remove ninja-build
```
2. 卸载ninjia-build和它的依赖项：
```cpp
$ sudo apt-get remove --auto-remove ninja-build
```
3. 删除ninjia-build的配置文件：
```cpp
$ sudo apt-get purge ninja-build
```
或者：
```cpp
$ sudo apt-get purge --auto-remove ninja-build
```
