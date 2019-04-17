# 【Julia】Linux下配置Julia交互开发环境（Julia notebook） - CD's Coding - CSDN博客





2018年08月18日 16:28:17[糖果天王](https://me.csdn.net/okcd00)阅读数：1391








## 0x00 前言

近期Julia这门语言突然被拉入了大众视野，我也是被一句 “Walk as Python，Run as C” 深深的吸引住了， 

如果它没有在吹牛，那么这门语言说不定确实是我所想要的，那么，学学看吧？

本篇文章主要用于介绍如何搭建便于学习与测试的REPL环境，先在Linux上试试水， 

毕竟一旦Jupyter Notebook搭建起来之后，网页版的交互式环境就已经可以不受操作系统限制了。

**Julia？**

+ 要像**C语言**一般快速而有拥有如同**Ruby**的动态性； 

+ 要具有**Lisp**般真正的同像性（Homoiconicity）  

+ 而又有**Matlab**般熟悉的数学记号； 

+ 要像**Python**般通用、 

+ 像**R**般在统计分析上得心应手、 

+ 像**Perl**般自然地处理字符串、  

+ 像**Matlab**般具有强大的线性代数运算能力、  

+ 像**shell**般胶水语言的能力，易于学习而又不让真正的黑客感到无聊；  

+ 还有，它应该是**交互式**的，同时又是**编译型**的……
![ipykernel](https://img-blog.csdn.net/2018081816255458?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

emmmm，那还说啥呢，学学看呗（不就是配个环境么，配起来！）

## 0x01 相关资料
- 官方主页 
[https://julialang.org/downloads/](https://julialang.org/downloads/)
- 官方Github 
[https://github.com/JuliaLang/julia](https://github.com/JuliaLang/julia)
- 官方文档 
[https://docs.julialang.org/en/stable/manual/getting-started/](https://docs.julialang.org/en/stable/manual/getting-started/)

## 0x02 配置过程

**我的环境**
`Anaconda2`下直接new了一个virtual env，叫做`cd`
- Linux RedHat
- Python 3.6
- pip 18.0
- jupyter notebook

**Jupyter Notebook**

一种可交互界面化变成的解决方案，Python和Julia的学习中比较推荐使用
- 预先准备 notebook 
`(cd) [chend@c9 ~]$ pip install notebook`- 如需使用 jupyterlab (功能更多，界面较好)

```
(cd) [chend@c9 ~]$ pip install jupyterlab
(cd) [chend@c9 ~]$ jupyter serverextension enable --py jupyterlab
```

**下载**
- 可以自己下载自己解压，见 `0x01 相关资料` 中的官方主页
`wget https://julialang-s3.julialang.org/bin/linux/x64/1.0/julia-1.0.0-linux-x86_64.tar.gz`- 也可以直接去官方的 Github 上 git clone

```php
git clone git://github.com/JuliaLang/julia.git
```
- git clone 不下来的话需要绕过的，可以全局替换一下变成ipv6通道

```php
git config --global url."https://".insteadOf git://
```

**获取 cmake**

编译需要 cmake，如果没有或者版本不对的话，可以进到刚才下载的目录里面 `cd julia`

使用自带的 cmake 下载脚本：

```
(cd) [chend@c9 julia]$ contrib/download_cmake.sh
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   479  100   479    0     0    227      0  0:00:02  0:00:02 --:--:--   429
  1 29.2M    1  492k    0     0   2443      0  3:29:18  0:03:26  3:25:52     0
curl: (28) Operation too slow. Less than 1 bytes/sec transferred the last 15 seconds
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 29.2M  100 29.2M    0     0   421k      0  0:01:11  0:01:11 --:--:--  450k

cmake-3.7.1-Linux-x86_64.tar.gz: OK
```

**安装**

我这里想要安装刚 Release 的较为稳定的 v1.0.0版本，所以我先 `git co`到这个分支
`git checkout v1.0.0`
然后直接 `make` 就可以编译了，但考虑到东西较多速度不够快，也可以采取多线程编译 `make -j48`

```
(cd) [chend@c9 julia]$ make
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100   465  100   465    0     0    180      0  0:00:02  0:00:02 --:--:--   476
  1 24.3M    1  288k    0     0   7013      0  1:00:45  0:00:42  1:00:03 10783
```

## 0x03 基本使用

在当前目录下直接 `./julia` 就可以调出类似 `IPython`  的交互式界面了

```
(cd) [chend@c9 julia]$ ./julia
               _
   _       _ _(_)_     |  Documentation: https://docs.julialang.org
  (_)     | (_) (_)    |
   _ _   _| |_  __ _   |  Type "?" for help, "]?" for Pkg help.
  | | | | | | |/ _` |  |
  | | |_| | | | (_| |  |  Version 1.0.0 (2018-08-08)
 _/ |\__'_|_|_|\__'_|  |  
|__/                   |

julia>
```

如需在任何位置都可以直接调用 `julia` 命令，可以直接在 `~/.bashrc` 中加入这行

```bash
echo "alias julia='/path/to/install/folder/bin/julia'" >> ~/.bashrc && source ~/.bashrc
```

例如我直接获取目录位置 `pwd` 是 `/home/chend/julia/`，那么我就输入下面这行即可。

```bash
echo "alias julia='/home/chend/julia/julia'" >> ~/.bashrc && source ~/.bashrc
```

## 0x04 配置 Julia Notebook

配置 Jupyter 相关博客： 
[https://www.analyticsvidhya.com/blog/2017/10/comprehensive-tutorial-learn-data-science-julia-from-scratch/](https://www.analyticsvidhya.com/blog/2017/10/comprehensive-tutorial-learn-data-science-julia-from-scratch/)

我的调试过程：

```
julia> using IJulia 
ERROR: ArgumentError: Package IJulia not found in current path:
- Run `Pkg.add("IJulia")` to install the IJulia package.

Stacktrace:
 [1] require(::Module, ::Symbol) at ./loading.jl:817

julia> Pkg.add("IJulia")
ERROR: UndefVarError: Pkg not defined
Stacktrace:
 [1] top-level scope at none:0

# Pkg not defined 错误处理
julia> using Pkg

# 再次尝试调用 IJulia
julia> using IJulia
[ Info: Precompiling IJulia [7073ff75-c697-5162-941a-fcdaad2a7d2a]
ERROR: LoadError: Blosc not installed properly, run Pkg.build("ZMQ"), restart Julia and try again

# 错误处理
julia> Pkg.build("ZMQ")

# 再次尝试调用 IJulia
julia> using IJulia
[ Info: Precompiling IJulia [7073ff75-c697-5162-941a-fcdaad2a7d2a]
ERROR: LoadError: MbedTLS not installed properly, run Pkg.build("MbedTLS"), restart Julia and try again

# 错误处理
julia> Pkg.build("MbedTLS")

# 再次尝试调用 Ijulia
julia> using IJulia
[ Info: Precompiling IJulia [7073ff75-c697-5162-941a-fcdaad2a7d2a]
julia> notebook()

# 完成，直接开个浏览器看看 localhost:8888，如果自己设定过默认端口则改为你自己的端口。
```

![interface](https://img-blog.csdn.net/20180818162703362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L29rY2QwMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




