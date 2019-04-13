
# Error: file not found: arch-arm-boot-compressed-piggy.lzo - 嵌入式Linux - CSDN博客

2016年03月03日 12:50:35[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1950


**问题：在编译Android 系统kernel源码的时候出现错误**
```python
I am trying
```
```python
to
```
```python
build android project
Getting below
```
```python
error
```
```python
even after setting correct path
```
```python
for
```
```python
toolchains
/bin/sh: lzop:
```
```python
not
```
```python
found
```
```python
AS
```
```python
arch/arm/boot/compressed/piggy.lzo.o
arch/arm/boot/compressed/piggy.lzo.S: Assembler messages:
arch/arm/boot/compressed/piggy.lzo.S:
```
```python
4
```
```python
:
```
```python
Error
```
```python
: file
```
```python
not
```
```python
found: arch/arm/boot/compressed/piggy.lzo
make[
```
```python
2
```
```python
]: *** [arch/arm/boot/compressed/piggy.lzo.o]
```
```python
Error
```
```python
1
```
```python
make[
```
```python
1
```
```python
]: *** [arch/arm/boot/compressed/vmlinux]
```
```python
Error
```
```python
2
```
```python
make: *** [zImage]
```
```python
Error
```
```python
2
```
```python
make: Leaving directory `/home/user/workspace/android/kernel/exynos54xx
```
```python
'
```
```python
build:
```
```python
error
```
```python
: build-kernel failed
```
```python
and
```
```python
die !!!
```
## 解决办法
Here is a solution if you’re using Ubuntu:
$ sudo apt-get install lzop
or look for the lzop package of you OS.

