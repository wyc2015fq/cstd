
# [RK3399 Android7.1] 开启保存上一次重启前日志 - 嵌入式Linux - CSDN博客

2019年03月07日 14:39:45[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：35



## 保存上一次重启前日志的必要
分析死机的情况需要我们把上一次不能及时抓取的日志拿到是非常关键的，这个方法非常有利于我们分析内核panic和Android 死机的情况。
## 开启的方法
### 内核配置
在dts文件里面添加下面两个节点
`ramoops_mem: ramoops_mem {
reg = <0x0 0x110000 0x0 0xf0000>;
reg-names = "ramoops_mem";
};
ramoops {
compatible = "ramoops";
record-size = <0x0 0x20000>;
console-size = <0x0 0x80000>;
ftrace-size = <0x0 0x00000>;
pmsg-size = <0x0 0x50000>;
memory-region = <&ramoops_mem>;
};`
### 日志说明
130|root@rk3399:/sys/fs/pstore \# ls
dmesg-ramoops-0 上次内核panic后保存的log。
pmsg-ramoops-0 上次用户空间的log，android的log。
ftrace-ramoops-0 打印某个时间段内的function trace。
console-ramoops-0 last_log 上次启动的kernel log，但只保存了优先级比默认log level 高的log。
使用方法：
cat dmesg-ramoops-0
cat console-ramoops-0
logcat -L （pmsg-ramoops-0） 通过logcat 取出来并解析
cat ftrace-ramoops-0

