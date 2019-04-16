# Matlab 日常技巧 ，判断文件存在 - 去向前方的博客 - CSDN博客





2018年11月08日 19:49:09[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：692








# 前言

在写一个大程序的时候需要批量读入文件，这时候就有一个问题了，需要提前判断文件是否存在，避免出漏子。

# 正文

当前目录中包含以下文件及文件夹：

startup.m

win64/

…

判断当前目录中是否存在startup.m文件

if ~exist(‘startup.m’,‘file’)

error(display(‘no startup.m file’));

end

判断当前目录中是否存在win32文件夹，若不存在则创建

if ~exist(‘win32’,‘dir’)

mkdir('win32‘);

end



