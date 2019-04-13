
# [dts]DTS实例分析 - 嵌入式Linux - CSDN博客

2017年08月05日 11:00:27[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：375


## 2. 通常会碰到的实际问题
到此，问题出现了：
1. 当写一个按键驱动，应该如何在*.dts或者*.dtsi中操作?
2. 当在串口driver中需要使用到某个pin脚作为普通输出IO，该如何操作？
3. 当在串口driver中需要使用某个muxpin脚作为ADC或者其他服用功能该如何操作？
4. 当在串口driver中有可能想使能某个功能而不想写定在driver代码中，该如何操作？
5. 当想向driver中传入一个常数如做delay延时操作等等，该如何操作？
解答：
1. 当写一个按键驱动，应该如何在*.dts或者*.dtsi中操作?
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1gpio-keys {2compatible ="gpio-keys";3pinctrl-names ="default";4pinctrl-0= <&pinctrl_gpio_keys>;56volume-up {7label ="Volume Up";8gpios = <&gpio118GPIO_ACTIVE_LOW>;9linux,code = <KEY_VOLUMEUP>;10};1112volume-down {13label ="Volume Down";14gpios = <&gpio119GPIO_ACTIVE_LOW>;15linux,code = <KEY_VOLUMEDOWN>;16};17};![复制代码](http://common.cnblogs.com/images/copycode.gif)
1pinctrl_gpio_keys: gpio_keysgrp {2fsl,pins = <3MX6SX_PAD_CSI_DATA04__GPIO1_IO_180x170594MX6SX_PAD_CSI_DATA05__GPIO1_IO_190x170595>;6};
2. 当在串口driver中需要使用到某个pin脚作为普通输出IO，该如何操作？
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1sii902x_reset: sii902x-reset {2compatible ="gpio-reset";3reset-gpios = <&gpio3 27 1>;4reset-delay-us = <100000>;5\#reset-cells = <0>;6status ="disabled";7};![复制代码](http://common.cnblogs.com/images/copycode.gif)

最简单的不需要pinctrl来进行pin脚的设置，为什么？？？
3. 当在串口driver中需要使用某个muxpin脚作为ADC或者其他服用功能该如何操作？
这个问题其实上面有提到，就像上述的多个pin-names和多个pinctrl在后面的pinctrl中配置这个复用的管脚即可。
4. 当在串口driver中有可能想使能某个功能而不想写定在driver代码中，该如何操作？
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1&uart5 {2pinctrl-names ="default";3pinctrl-0= <&pinctrl_uart5>;4fsl,uart-has-rtscts;5status ="okay";67};![复制代码](http://common.cnblogs.com/images/copycode.gif)

只需要在driver中使用of函数读取此属性，如果有此字段，可以使能某些操作；如果没有就失能某些操作。
5. 当想向driver中传入一个常数如做delay延时操作等等，该如何操作？
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1&usdhc3 {2pinctrl-names ="default","state_100mhz","state_200mhz";3pinctrl-0= <&pinctrl_usdhc3>;4pinctrl-1= <&pinctrl_usdhc3_100mhz>;5pinctrl-2= <&pinctrl_usdhc3_200mhz>;6bus-width = <8>;7cd-gpios = <&gpio210GPIO_ACTIVE_HIGH>;8wp-gpios = <&gpio215GPIO_ACTIVE_HIGH>;9keep-power-in-suspend;10enable-sdio-wakeup;11vmmc-supply = <&vcc_sd3>;12status ="okay";13};![复制代码](http://common.cnblogs.com/images/copycode.gif)

只需要在driver中读取bus-width字段的值，即可获取到8这个常量，注意在devicetree中命名字段更多使用"-"而非下划线"_".

