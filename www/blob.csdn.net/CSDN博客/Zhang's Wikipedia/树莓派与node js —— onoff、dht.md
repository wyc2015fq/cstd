
# 树莓派与node.js —— onoff、dht - Zhang's Wikipedia - CSDN博客


2018年05月12日 18:16:40[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：343



## 1. 发光二极管
[tm-onoff](https://www.npmjs.com/package/tm-onoff)
```python
var
```
```python
Gpio =
```
```python
require
```
```python
(
```
```python
'tm-onoff'
```
```python
).Gpio,
    led =
```
```python
new
```
```python
Gpio(
```
```python
17
```
```python
,
```
```python
'out'
```
```python
),
    button =
```
```python
new
```
```python
Gpio(
```
```python
18
```
```python
,
```
```python
'in'
```
```python
,
```
```python
'both'
```
```python
);
```
```python
// 'both'：按钮的上升下降沿都应该被处理，即调用如下的函数
```
```python
button.watch(
```
```python
function
```
```python
(err, value)
```
```python
{
```
```python
led.writeSync(value);
});
```
上述代码在ctrl+c退出时，并未释放资源，也未处理错误，一份更完整的处理如下：
```python
var
```
```python
Gpio =
```
```python
require
```
```python
(
```
```python
'tm-onoff'
```
```python
).Gpio,
    led =
```
```python
new
```
```python
Gpio(
```
```python
17
```
```python
,
```
```python
'out'
```
```python
),
    button =
```
```python
new
```
```python
Gpio(
```
```python
18
```
```python
,
```
```python
'in'
```
```python
,
```
```python
'both'
```
```python
);
button.watch(
```
```python
function
```
```python
(err, value)
```
```python
{
```
```python
if
```
```python
(err)
```
```python
exit
```
```python
();
    led.writeSync(value);
});
```
```python
function
```
```python
exit
```
```python
()
```
```python
{
```
```python
led.unexport();
    button.unexport();
    process.
```
```python
exit
```
```python
();
}
process.on(
```
```python
'SIGINT'
```
```python
,
```
```python
exit
```
```python
);
```
通过调用 led、btn 的 unexport 方法以释放其资源；
varonoff =require('onoff');varGpio = onoff.Gpio,
    led =newGpio(4,'out'),
    interval;
interval = setInterval(function(){//\#Cvarvalue = (led.readSync() +1) %2;//\#Dled.write(value,function(){//\#Econsole.log("Changed LED state to: "+ value);
    });
},2000);
process.on('SIGINT',function(){//\#FclearInterval(interval);
    led.writeSync(0);//\#Gled.unexport();
    console.log('Bye, bye!');
    process.exit();
});
## 2. dht：温湿度传感器
[GitHub - momenso/node-dht-sensor: Node.js Humidity and Temperature sensor addon](https://github.com/momenso/node-dht-sensor)
node-dht-sensor 的安装使用如下命令：
$npminstall--savenode-dht-sensor\#如果存在权限问题，使用$sudonpminstall--unsafe-perm--savenode-dht-sensor
```python
var
```
```python
dhtLib =
```
```python
require
```
```python
(
```
```python
'node-dht-sensor'
```
```python
);
dhtLib.initialize(
```
```python
11
```
```python
,
```
```python
12
```
```python
);
```
```python
// 第一个参数表示传感器类型，dht11使用11，dht22使用22
```
```python
// 第二个参数表示GPIO引脚号
```
```python
var
```
```python
interval = setInterval(
```
```python
function
```
```python
()
```
```python
{
```
```python
read();
},
```
```python
2000
```
```python
);
```
```python
function
```
```python
read
```
```python
()
```
```python
{
```
```python
var
```
```python
readout = dhtLib.read();
    console.log(
```
```python
'Temperature: '
```
```python
+ readout.temperature.toFixed(
```
```python
2
```
```python
) +
```
```python
'C, '
```
```python
+
```
```python
'humidity: '
```
```python
+ readout.humidity.toFixed(
```
```python
2
```
```python
) +
```
```python
'%'
```
```python
);
};
process.on(
```
```python
'SIGINT'
```
```python
,
```
```python
function
```
```python
()
```
```python
{
```
```python
clearInterval(interval);
    console.log(
```
```python
'Bye, bye!'
```
```python
);
    process.
```
```python
exit
```
```python
();
});
```

