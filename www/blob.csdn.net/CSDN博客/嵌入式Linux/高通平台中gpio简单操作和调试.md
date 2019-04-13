
# 高通平台中gpio简单操作和调试 - 嵌入式Linux - CSDN博客

2017年08月08日 17:17:30[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：445


做底层驱动免不了gpio打交道，所以对其操作和调试进行了一下简单的梳理
一、gpio的调试方法
在[Linux](http://lib.csdn.net/base/linux)下，通过sysfs，获取gpio状态，也可以操作gpio。
1、获取gpio状态
cd /sys/kernel/debug/
cat gpio
2、操作gpio（以gpio99为例）
cd /sys/class/gpio/
echo 99 > export
cd gpio99
echo in/out > direction    //设置gpio输入或输出
cat direction                    //获取gpio输入输出状态
echo 'value' > value        //设置gpio寄存器值
cat value                        //获取gpio寄存器的值

二、在kernel代码中操作gpio
在代码中有两种方式操作gpio，一种是一次申请单个gpio，成功后操纵该gpio，另一种是使用pinctrl，通过设备树设置，一次操作多个gpio。
1、方法一
设备树设备节点中添加gpio    //可选
device_node {
...
gpio_name = <&tlmm 99 0>;    //gpio_99
...
}
驱动代码：
intgpio_99=of_get_named_gpio_flags(dev->of_node,"gpio_name",0,NULL);//从设备数节点, 可选
gpio_request(gpio_99,"gpio_name");//通过gpio号申请gpio
gpio_direction_output(gpio_99,1);//设置gpio_99输出，初始值为1
gpio_set_value(gpio_99,0);//设置gpio_99值为0
gpio_free(gpio_99);//gpio_99不再使用后应当释放
注：
gpio的申请和设置都可能会出现失败的情况，应该做好异常处理

2、方法二
设备树代码:
平台pinctrl节点下添加如下子节点：
相关文档和设备树配置详解见kernel/Document/pinctrl.txt和Documentation/devicetree/bindings/pinctrl/下多个文件
gpio_group {
gpio_active: gpio_active {
mux {
pins = "gpio99", "gpio98";    //复用引脚99和98
functions = "gpio";            //引脚功能配置为普通gpio
};
config {
pins = "gpio99", "gpio98";
drive-strength = <8>;    //最大电流限制为8mA
bias-pull-up;                //配置上拉
output-high;                //输出高电平
};
};
gpio_sleep: gpio_sleep {
mux {
pins = "gpio99", "gpio98";    //复用引脚99和98
functions = "gpio";            //引脚功能配置为普通gpio
};
config {
pins = "gpio99", "gpio98";
drive-strength = <2>;    //最大电流限制为2mA
bias-no-pull;                //不上拉也不下拉
output-low;                //输出低电平
};
};

设备节点中引用pinctrl：
device_node {
...
pinctrl-names = "gpio_active", "gpio_sleep";    //分别对用pinctrl-0和pinctrl-1
pinctrl-0 = <&gpio_active>;                               //引用
pinctrl-1 = <&gpio_sleep>;                                //引用
...
};

内核驱动代码：

structpinctrl*pinctrl=devm_pinctrl_get(device);//获取device对应节点下的pinctrl
structpinctrl_state=pinctrl_lookup_state(pinctrl,"gpio_active");//通过pinctrl名获取pinctrl对应状态
pinctrl_select_state(pinctrl,pinctrl_state);//设置pinctrl的状态为'gpio_active
devm_pinctrl_put(pinctrl);//使用完了释放资源

也可以同时使用方法一和方法二，方法一操作简单，但是只能拉高或拉低，而方法二可全面配置GPIO。在一个设备驱动中同时使用方法一和方法二可保证资源不被其他模块操作。

