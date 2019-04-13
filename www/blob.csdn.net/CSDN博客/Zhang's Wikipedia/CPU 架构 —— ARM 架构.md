
# CPU 架构 —— ARM 架构 - Zhang's Wikipedia - CSDN博客


2018年05月09日 21:16:59[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：439


linux 系统查看 CPU 架构命令：
```python
$
```
```python
arch
armv7l
```
```python
$
```
```python
uname -m
armv7l
```
```python
# -m：--machine
```
```python
# 进一步查看处理器信息
```
```python
$
```
```python
cat /proc/cpuinfo
```
一般嵌入式设备运行在 ARM 处理器上；
树莓派
一般的 PC：x86、x64 架构；
1. arm 架构处理器的位数
$which bash
/bin/bash$file /bin/bash

