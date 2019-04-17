# Ubuntu安装Clang\Clang版本切换 - DumpDoctorWang的博客 - CSDN博客





2018年11月27日 20:22:18[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1118











### 目录
- [1、废话](#1_1)
- [2、安装不同版本的Clang](#2Clang_4)
- [2.1、Ubuntu 16.04 可选版本](#21Ubuntu_1604__24)
- [2.2、Ubuntu 18.04 可选版本](#22Ubuntu_1804__41)
- [2.3、Ubuntu 18.10 可选版本](#23Ubuntu_1810__54)
- [2、Clang不同版本的切换](#2Clang_69)




# 1、废话

clang这种可以直接用apt-get来安装的，居然还有人写了手动编译的教程。。。直接使用apt-get安装不是更好？虽然说使用apt-get可能不能安装不了最新版，但是，最新版的特性也用不上啊。。。

# 2、安装不同版本的Clang

使用apt-get命令安装clang的某个版本

```bash
sudo apt-get install clang-3.8
```

然后通过update-alternatives安装该版本clang的“快捷方式”：

```bash
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 1 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-3.8
```

如果安装的是clang-4.0，就把上面的命令中的“/usr/bin/clang-3.8”（实际的可执行文件名，不同的clang版本是不同的）换成“/usr/bin/clang-4.0”，“/usr/bin/clang++-3.8”换成“/usr/bin/clang++-4.0”，其他版本也是同理。至于update-alternatives，大家可自行查阅其用法。

获取clang版本：

```bash
clang --version
clang++ --version
```

不知道选哪个clang版本就选最新的。

## 2.1、Ubuntu 16.04 可选版本

```bash
# clang-3.5
sudo apt-get install clang-3.5
# clang-3.6
sudo apt-get install clang-3.6
# clang-3.7
sudo apt-get install clang-3.7
# clang-3.8
sudo apt-get install clang-3.8
# clang-3.9
sudo apt-get install clang-3.9
# clang-4.0
sudo apt-get install clang-4.0
```

## 2.2、Ubuntu 18.04 可选版本

```bash
# clang-3.9
sudo apt-get install clang-3.9
# clang-4.0
sudo apt-get install clang-4.0
# clang-5.0
sudo apt-get install clang-5.0
# clang-6.0
sudo apt-get install clang-6.0
```

## 2.3、Ubuntu 18.10 可选版本

```bash
# clang-3.9
sudo apt-get install clang-3.9
# clang-4.0
sudo apt-get install clang-4.0
# clang-5.0
sudo apt-get install clang-5.0
# clang-6.0
sudo apt-get install clang-6.0
# clang-7
sudo apt-get install clang-7
```

# 2、Clang不同版本的切换

update-alternatives可以很方便的完成不同版本的切换，直接来个例子吧。比如说我需要安装clang-3.8和clang-4.0，然后需要在这两个版本之间切换。

先安装clang-3.8和clang-4.0：

```bash
sudo apt-get install clang-3.8 clang-4.0
```

然后使用update-alternatives“安装”两个版本的clang：

```bash
# for clang-3.8
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-3.8 1 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-3.8
# for clang-4.0
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-4.0 2 --slave /usr/bin/clang++ clang++ /usr/bin/clang++-4.0
```

打印clang版本：

```bash
clang --version
clang++ --version
```

输出：

```
clang version 4.0.0-1ubuntu1~16.04.2 (tags/RELEASE_400/rc1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

由于clang-4.0的优先级“2”要比clang-3.8的“1”要高，所以此时clang-4.0是默认的版本。

如果要切换版本，输入如下命令：

```bash
sudo update-alternatives --config clang
```

然后出现如下提示：

```
There are 2 choices for the alternative clang (providing /usr/bin/clang).

  Selection    Path                Priority   Status
------------------------------------------------------------
* 0            /usr/bin/clang-4.0   2         auto mode
  1            /usr/bin/clang-3.8   1         manual mode
  2            /usr/bin/clang-4.0   2         manual mode

Press <enter> to keep the current choice[*], or type selection number:
```

然后在“selection number:”后面输入你要使用的clang版本序号（第一列），加入要切换为clang-3.8，就输入1，然后回车，就切换为clang-3.8了。

打印clang版本：

```bash
clang --version
clang++ --version
```

输出：

```
clang version 3.8.0-2ubuntu4 (tags/RELEASE_380/final)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

update-alternatives也可以用来管理其他软件的版本，比如还常常用于gcc的版本管理，是个很有用的工具。



