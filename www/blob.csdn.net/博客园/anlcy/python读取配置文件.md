
# python读取配置文件 - anlcy - 博客园






# [python读取配置文件](https://www.cnblogs.com/camilla/p/7234657.html)
配置文件作为一种可读性很好的格式，非常适用于存储程序中的配置数据。 在每个配置文件中，配置数据会被分组（比如“config”和 “cmd”）。 每个分组在其中指定对应的各个变量值。如下：
```python
#  定义config分组
```
```python
[config]
```
```python
platformName=Android
```
```python
appPackage=com.romwe
```
```python
appActivity=com.romwe.SplashActivity
```
```python
#  定义cmd分组
```
```python
[cmd]
```
```python
viewPhone=adb devices
```
```python
startServer=adb start-server
```
```python
stopServer=adb kill-server
```
```python
#  定义log分组
```
```python
[log]
```
```python
log_error=
```
```python
true
```
**基本的读取操作：**
-read(filename)               直接读取文件内容
-sections()得到所有的section，并以列表的形式返回
-options(section)得到该section的所有option
-items(section)得到该section的所有键值对
-get(section,option)得到section中option的值，返回为string类型
-getint(section,option)得到section中option的值，返回为int类型，还有相应的getboolean()和getfloat() 函数。
在对配置文件进行读写操作前，我们需要先进行以下两个操作：
1、实例化ConfigParser对象:
```python
#  实例化configParser对象
```
```python
cf = configparser.ConfigParser()
```
2、读取配置文件
```python
#  读取
```
```python
config
```
```python
.ini文件
```
```python
cf
```
```python
.read(
```
```python
config
```
```python
.ini)
```
然后进行配置文件的读取操作。
以get为例，示例代码如下：
```python
#  定义方法，获取config分组下指定name的值
```
```python
def
```
```python
getConfigValue
```
```python
(self, name)
```
```python
:
    value = self.cf.get(
```
```python
"config", name)
```
```python
return value
```
```python
#  定义方法，获取cmd分组下指定name的值
```
```python
def
```
```python
getCmdValue
```
```python
(self, name)
```
```python
:
    value = self.cf.get(
```
```python
"cmd", name)
```
```python
return value
```
通过get(section, option)方法，可以获取指定分组下指定名称的值，其他方法类似，可参照着尝试。
**基本的写入操作：**
-write(fp)将config对象写入至某个 .init 格式的文件Write an .ini-format representation of the configuration state.
-add_section(section)添加一个新的section
-set( section, option, value对section中的option进行设置，需要调用write将内容写入配置文件
-remove_section(section)删除某个 section
-remove_option(section, option)
以set(section, option, value)为例，示例代码如下：
```python
#  定义方法，修改config分组下指定name的值value
```
```python
def
```
```python
setConfigValue
```
```python
(self, name, value)
```
```python
:
    cfg = self.cf.set(
```
```python
"config", name, value)
    fp = open(
```
```python
r'config.ini',
```
```python
'w')
    cfg.write(fp)
```
其他方法可以自行尝试。
配置文件中的名字是不区分大小写的，如下两个是等价的：
```python
#  不区分大小写,以下两个等价，都获取appActivity的值
```
```python
self.cf.
```
```python
get(
```
```python
"config",
```
```python
"appActivity")
```
```python
self.cf.
```
```python
get(
```
```python
"config",
```
```python
"APPACTIVITY")
```
在解析时，getboolean()方法查找任何可行的值，例如以下几个都是等价的：
```python
#  以下取得的值都是等价的为ture
```
```python
[log]
```
```python
log_error=
```
```python
true
```
```python
log_error=
```
```python
TRUE
```
```python
log_error=
```
```python
1
```
```python
log_error=
```
```python
yes
```
以上就是我初次学习的一点记录，如有错误的地方，希望大家慷慨指出，我会及时改正，如果本篇对你有些帮助，希望给我点个赞哦！接下来我会继续python的学习道路，希望大家于我同行。





