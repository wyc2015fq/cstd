# Ubuntu系统的安装与使用：搜狗输入法安装以及切换 - 心纯净，行致远 - CSDN博客





2019年04月02日 17:59:57[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：13








Ubuntu系统是一款基于Debian的Linux系统桌面发行版，在没有太高的游戏需求的情况下可以满足用户的日常办公需求。Ubuntu系统自带的拼音输入法并不好用，小编带大家看看如何在Ubuntu上安装搜狗输入法。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=fa809f0d0de9390156028d3e4bec54f9/0823dd54564e92585750a1c39082d158ccbf4eb2.jpg)

## 工具/原料
- 
Ubuntu16.04

- 
搜狗输入法deb安装包


## 方法/步骤
- 
启动Ubuntu系统，打开浏览器。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=71d425ced939b6004dce0fb7d9503526/55e736d12f2eb938ff4a5cdfd9628535e5dd6ff3.jpg)

- 
使用百度搜索搜狗输入法，打开官网。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=83142637ca5c1038247ecec28211931c/d4628535e5dde711faab65dbabefce1b9d1661a5.jpg)

- 
点击上方的输入法Linux版，然后选择对应的32/64位。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=0f31a704e7c4b7453494b716fffc1e78/03087bf40ad162d9f7b743311ddfa9ec8a13cd45.jpg)

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=4c00c9d95dda81cb4ee683cd6266d0a4/42a98226cffc1e176becad1c4690f603738de948.jpg)

- 
下载完成后，在"~/下载"中打开，并开启前一个终端。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=58979359e7f81a4c2632ecc9e72b6029/0df431adcbef76091f92b8b222dda3cc7cd99e05.jpg)

- 
输入命令sudo dpkg -i ./sogoupinyin_2.2.0.0102_amd64.deb，回车输入用户密码进行安装。可以看到有安装包的依赖未解决。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=02871fea9c529822053339c3e7cb7b3b/faf2b2119313b07e8e24732500d7912397dd8c05.jpg)

- 
输入sudo apt install -f进行安装包的依赖修复。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=fb6f9aacc71349547e1ee864664f92dd/cc11728b4710b9126d4cd4d3cffdfc0392452209.jpg)

- 
然后再次运行上面的sudo dpkg -i ./sogoupinyin_2.2.0.0102_amd64.deb命令即可解决依赖问题。

![Ubuntu系统的安装与使用：[3]搜狗输入法安装](https://imgsa.baidu.com/exp/w=500/sign=4f0be5ce5b2c11dfded1bf2353266255/500fd9f9d72a605908ffc3372434349b033bba2b.jpg)

- 
现在可以进行搜狗拼音打字了。

- 


为了切换输入法，在命令行输入fcitx-configtool 打开fcitx界面进行设置

![](https://img-blog.csdn.net/20180313161932244)

      需要增减输入法, 可以在Input Method 中进行设置，其中列表下第一项为非激活的。

- 


然后一般保留中英文两种输入法即可。




