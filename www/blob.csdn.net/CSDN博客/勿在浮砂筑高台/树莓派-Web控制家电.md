# 树莓派-Web控制家电 - 勿在浮砂筑高台 - CSDN博客





2015年04月12日 19:18:59[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：3358








Linux+Raspberry Pi+Python 

   **给学生做的一个智能家居的教程：  

   采用树莓派作为服务器，我们通过访问服务器的方法，控制树莓派的I/O口的高低电平，I/O口接上继电器（实现低压控制高压），继电器上你就可以随意接上你想控制的家用电器啦！在网页页面上就可以控制了！**
**直接点击页面上的“关闭”、“打开”按钮就可以实现通过网络控制家中的灯了**

*下面是效果图*： 
![](https://img-blog.csdn.net/20150412190058464)
# 教程

1.安装FLASK

使用Flask（[http://flask.pocoo.org/](http://flask.pocoo.org/)）这个Python 的Web 框架把Raspberry Pi 变为一个动态Web 服务器。在命令行上输入下面的命令安装：
`   pi@raspberrypi~@sudo pip install flask`
2.编写python程序

```
创建一个名为WebLight.py的文件，写入以下代码：
```

```python
import RPi.GPIO as GPIO
from flask import Flask, render_template, request
app = Flask(__name__)
GPIO.setmode(GPIO.BCM)
pins = {
     24 : {'name' : 'TV', 'state' : GPIO.LOW},
     25 : {'name' : 'Light', 'state' : GPIO.LOW}
    }
for pin in pins:
    GPIO.setup(pin, GPIO.OUT)
    GPIO.output(pin, GPIO.LOW)
@app.route("/")
def main():
     for pin in pins:
         pins[pin]['state'] = GPIO.input(pin)
     templateData = {
        'pins' : pins
      }
     return render_template('main.html', **templateData)
@app.route("/<changePin>/<action>")
def action(changePin, action):
     changePin = int(changePin)
     deviceName = pins[changePin]['name']
     if action == "on":
        GPIO.output(changePin, GPIO.HIGH)
        message = "Turned " + deviceName + " on."
     if action == "off":
        GPIO.output(changePin, GPIO.LOW)
        message = "Turned " + deviceName + " off."
     if action == "toggle":
        GPIO.output(changePin, not GPIO.input(changePin))
        message = "Toggled " + deviceName + "."
     for pin in pins:
        pins[pin]['state'] = GPIO.input(pin)
     templateData = {
        'message' : message,
        'pins' : pins
        }
     return render_template('main.html', **templateData)
if __name__ == "__main__":
app.run(host='0.0.0.0', port=80, debug=True)
```

** 如果我们还想再加一个可以端口控制电器，很简单我们可以这样去添加： 

把

```bash
pins = {
     24 : {'name' : 'TV', 'state' : GPIO.LOW},
     25 : {'name' : 'Light', 'state' : GPIO.LOW}
    }
```

改为

```bash
pins = {
     24 : {'name' : 'TV', 'state' : GPIO.LOW},
     25 : {'name' : 'Light', 'state' : GPIO.LOW}
     26 : {'name' : 'Coffee Maker', 'state' : GPIO.LOW}
    }
```

3.**编写html文件**

    在WebLight.py 所在的目录下，创建一个名为templates 的子目录。在templates 子目录中，创建一个名为 

main.html 的文件并输入以下的代码: 

   这个HTML 页面模板中所有括号的部分会被解析为变量名，在Python 脚本调用render_template 函数时，会被实际的数据所替换。
```
<!DOCTYPE html>
<head>
<title>家居智能控制</title>
</head>
<body>
<h1>家庭自动化项目</h1>
<img src="http://www.5qdd.com/files/all/121207/6-12120G43028.jpg" border="0" widgth="440" height="230"></a>
<h2>学生姓名:   <font size ="6" color="red" >张三</font> </h2>
{% for pin in pins %}
<p>The {{ pins[pin].name }}
{% if pins[pin].state == true %}
 (<a href="/{{pin}}/off">关闭</a>)
{% else %}
 (<a href="/{{pin}}/on">打开</a>)
{% endif %}
</p>
{% endfor %}
{% if message %}
<h3>{{ message }}</h3>
{% endif %}
</body>
</html>
```

4.**启动服务器**

```
pi@raspberrypi~$sudo python WebLight.py
*Running on http://0.0.0.0:80/
*Restarting with reloader
```

最后：把继电器接在控制的I/O上，在局域网内登陆树莓派的IP地址，就实现了网络控制家用电器啦！。。

[参考文章](http://www.dfrobot.com.cn/community/thread-2663-1-1.html%20%20%E6%99%BA%E8%83%BD%E5%AE%B6%E5%B1%85)






