
# arduino 编程基础 - Zhang's Wikipedia - CSDN博客


2018年02月23日 20:32:33[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：524



## 0. setup() 与 loop() 函数
void setup()：
// put your setup code here, to run once:
仅执行一次；
void loop()：
// put your main code here, to run repeatedly
会重复执行多次；
1.  Blink demo
setup：
pinMode(LED_BUILTIN, OUTPUT);// pinMode 设置引脚模式// LED_BUILTIN：arduino 开发板上自带的 LED，将其指定为输出；
loop：
digitalWrite(LED_BUILTIN, HIGH);// turn the LED on (HIGH is the voltage level)delay(3000);// 暂停三秒，也即 LED_BUILTIN 高电平持续三秒，也即亮三秒；digitalWrite(LED_BUILTIN, LOW);// turn the LED off by making the voltage LOWdelay(1000);
2. 常用函数
millis() ⇒ 获取当前时间
unsignedlongcurrentMillis = millis();

