# 不同linux系统添加开机启动程序的命令 - Arthur的随笔 - CSDN博客
2011年12月28日 16:38:47[largetalk](https://me.csdn.net/largetalk)阅读数：1586
                
see http://phpcj.org/blog/%E4%B8%8D%E5%90%8Clinux%E7%B3%BB%E7%BB%9F%E6%B7%BB%E5%8A%A0%E5%BC%80%E6%9C%BA%E5%90%AF%E5%8A%A8%E7%A8%8B%E5%BA%8F%E7%9A%84%E5%91%BD%E4%BB%A4/
不同linux系统添加开机启动程序的命令
作者: 清风 日期: 2011 年 9 月 16 日 发表评论 (0) 查看评论
gentoo:
rc-update add nginx default
centos/fedora:
chkconfig nginx on
ubuntu:
update-rc.d -f nginx defaults
=======================================================================================================
各个运行级的详细解释：
0 为停机，机器关闭。
1 为单用户模式，就像Win9x 下的安全模式类似。
2 为多用户模式，但是没有NFS 支持。
3 为完整的多用户模式，是标准的运行级。
4 一般不用，在一些特殊情况下可以用它来做一些事情。
例如在笔记本电脑的电池用尽时，可以切换到这个模式来做一些设置。
5 就是 X11 ，进到 X Window 系统了。
6 为重启，运行 init 6 机器就会重启。
=======================================================================================================
- [載入 BIOS 的硬體資訊與進行自我測試，並依據設定取得第一個可開機的裝置；](http://linux.vbird.org/linux_basic/0510osloader.php#process_1)
- [讀取並執行第一個開機裝置內 MBR 的 boot Loader (亦即是 grub, spfdisk 等程式)；](http://linux.vbird.org/linux_basic/0510osloader.php#process_2)
- [依據 boot loader 的設定載入 Kernel ，Kernel 會開始偵測硬體與載入驅動程式；](http://linux.vbird.org/linux_basic/0510osloader.php#process_3)
- [在硬體驅動成功後，Kernel 會主動呼叫 init 程式，而 init 會取得 run-level 資訊；](http://linux.vbird.org/linux_basic/0510osloader.php#startup_init)
- [init 執行 /etc/rc.d/rc.sysinit 檔案來準備軟體執行的作業環境 (如網路、時區等)；](http://linux.vbird.org/linux_basic/0510osloader.php#startup_sysinit)
- [init 執行 run-level 的各個服務之啟動 (script 方式)；](http://linux.vbird.org/linux_basic/0510osloader.php#startup_runlevel)
- [init 執行 /etc/rc.d/rc.local 檔案；](http://linux.vbird.org/linux_basic/0510osloader.php#startup_local)
- [init 執行終端機模擬程式 mingetty 來啟動 login 程式，最後就等待使用者登入啦；](http://linux.vbird.org/linux_basic/0510osloader.php#startup_term)
--来自鸟哥的linux私房菜
