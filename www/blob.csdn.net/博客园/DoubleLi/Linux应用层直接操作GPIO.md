# Linux应用层直接操作GPIO - DoubleLi - 博客园






### Linux应用层直接操作GPIO

在一个老手的指导下，应用层可以直接操作GPIO，具体指设置GPIO的输入输出以及输出电平高或者低。这个大大地提高了灵活性，官方的文档有[GPIO Sysfs Interface for Userspace](https://www.kernel.org/doc/Documentation/gpio/sysfs.txt)，[GPIO Sysfs](http://developer.toradex.com/knowledge-base/gpio-%28linux%29)。

这里我记录一下使用过例子：

```bash
# RK30_PIN0_PD4
# A0-7 -> 0-7 / B0-7 -> 8-15 / C0-7 -> 16-23 / D0-7 -> 24-31
echo 28 > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio28/direction
echo 1 > /sys/class/gpio/gpio28/value
```
- 1
- 2
- 3
- 4
- 5

以上代码作用是：将`RK30_PIN0_PD4`设置为输出，将输出电平设置为高电平。

OTG的PIN脚为`RK30_PIN3_PD5`。

能有老手指点真好，以前想像过有没有办法直接在用户层控制GPIO的输入／输出并设置输出电平，不过最终没有了结果。

这个问题的背景是出在了对主板上的USB HOST口供电控制，在移植最新的内核版本到该主板上时，USB口没有供电输出，进行手动使能供电解决了问题所在。这个是手动控制，以后要添加到DTS文件中。









