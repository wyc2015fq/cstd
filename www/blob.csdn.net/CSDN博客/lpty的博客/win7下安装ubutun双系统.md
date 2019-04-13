
# win7下安装ubutun双系统 - lpty的博客 - CSDN博客

2017年01月13日 21:36:25[lpty](https://me.csdn.net/sinat_33741547)阅读数：946


一、工具准备
1、U盘
2、UltraISO软件
3、Ubuntu安装文件（[https://www.ubuntu.com/download/desktop](https://www.ubuntu.com/download/desktop)）
4、EasyBCD软件
二、UtraISO制作启动盘
1、文件>>打开，选择下载好的ubuntu安装文件
2、启动>>写入硬盘映像>>写入
3、等待完成
三、为ubuntu系统分配空间
1、右键 我的电脑>>管理>>存储>>磁盘管理
2、选择一个盘，右键点击 压缩卷，不存储大文件的话30G就够用了
3、压缩完毕，退出即可
四、安装系统
1、将U盘插入电脑，重启
2、启动过程中按F12进入BOOT（不同电脑自行搜索）
3、选择U盘启动
4、进入Ubuntu安装界面，选择中文，安装
5、注意，到安装选项选择.>>其他选项>>继续自行为Ubuntu分区
6、分区数据
（1）swap分区，一般和内存大小一致即可，逻辑分区
（2）/分区，一般在10G-20G即可，逻辑分区
（3）boot分区，200M，主分区
（4）/home分区，其他剩下空间，逻辑分区
7、选择安装启动引导设备，这里一定要选择/boot分区
8、其他自行选择，完成安装
五、使用EasyBCD修改系统引导
1、上述安装完成后重启电脑是找不到我们新安装的Ubuntu系统的，这时候需要新增系统引导
2、打开EasyBCD，点击Add New Entry选择Linux/BSD，Type选择GRUB，Name随意，Drive选择/boot分区，再点击Add Entry
3、重启，选择ubuntu
附：
Windows 764位旗舰版
ed2k://|file|cn_windows_7_ultimate_with_sp1_x64_dvd_u_677408.iso|3420557312|B58548681854236C7939003B583A8078|/
Windows 732位旗舰版
ed2k://|file|cn_windows_7_ultimate_with_sp1_x86_dvd_u_677486.iso|2653276160|7503E4B9B8738DFCB95872445C72AEFB|/


