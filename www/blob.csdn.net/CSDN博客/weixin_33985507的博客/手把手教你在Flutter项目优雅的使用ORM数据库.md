# 手把手教你在Flutter项目优雅的使用ORM数据库 - weixin_33985507的博客 - CSDN博客
2019年01月21日 23:56:41[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## Flutter ORM数据库介绍
Flutter现在开发上最大的槽点可能就是数据库使用了，Flutter现在只提供了sqflite插件，这表明开发者手动写sql代码，建表、建索引、transation、db线程控制等等繁琐的事情必然接踵而至，这种数据库使用方式是最低效的了。例如IOS平台有coredata、realm等等的框架提供便捷的数据库操作，但来到flutter就又倒退回去裸写sql，这对大部分团队都是重大的成本。
本文将详细介绍一种在Flutter项目中优雅的使用ORM数据库的方法，我们使用的ORM框架是包含在一个Flutter插件[flutter_luakit_plugin](https://pub.dartlang.org/packages/flutter_luakit_plugin)(如何使用可参考[介绍文章](https://juejin.im/post/5c34597651882523d3200c98)）中的其中一个功能，本文只详细介绍这套ORM框架的使用和实现原理。我们给出了一个[demo](https://github.com/williamwen1986/flutter_luakit_demo)。
我们demo中实现了一个简单的功能，从一个天气网站上查询北京的天气信息，解析返回的json然后存数据库，下次启动优先从数据库查数据马上显示，再发请求向天气网站更新天气信息，就这么简单的一个功能。虽然功能简单，但是我们99%日常的业务逻辑也就是由这些简单的逻辑组成的了。下面是demo运行的效果图。
![image](https://image-static.segmentfault.com/182/530/1825306083-5c45c9f3b2e17_articlex)
看完运行效果，我们开始看看ORM数据库的使用。ORM数据库的核心代码都是[lua](https://github.com/williamwen1986/flutter_luakit_demo/tree/master/android/app/src/main/assets/lua),其中[WeatherManager.lua](https://github.com/williamwen1986/flutter_luakit_demo/blob/master/android/app/src/main/assets/lua/WeatherManager.lua)是业务逻辑代码，其他的lua文件是ORM数据库的核心代码，全部是lua实现的，所有代码文件加起来也就120k左右，非常轻量。
针对上面提到的天气信息的功能，我们来设计数据模型，从demo的展示我们看到每天天气信息包含几个信息，城市名、日出日落时间、最高温度、最低温度、风向、风力，然后为了区分是哪一天的数据，我们给每条信息加上个id的属性，作为主键。想好我们就开始定义第一个[ORM数据模型](https://github.com/williamwen1986/flutter_luakit_demo/blob/master/android/app/src/main/assets/lua/orm/dbData.lua),有几个必要的信息，db名，表名，后面的就是我们需要的各个字段了，我们提供IntegerField、RealField、BlobField、TextField、BooleandField。等常用的数据类型。weather 就是这个模型的名字，之后我们weather为索引使用这个数据模型。定义模型代码如下。
```
weather = {
   __dbname__ = "test.db",
    __tablename__ = "weather",
    id = {"IntegerField",{unique = true, null = false, primary_key = true}},
    wind = {"TextField",{}},
    wind_direction = {"TextField",{}},
    sun_info = {"TextField",{}},
    low = {"IntegerField",{}},
    high = {"IntegerField",{}},
    city =  {"TextField",{}},
 },
```
定义好模型后，我们看看[如何使用](https://github.com/williamwen1986/flutter_luakit_demo/blob/master/android/app/src/main/assets/lua/WeatherManager.lua)，我们跟着业务逻辑走，首先网络请求回来我们要生成模型对象存到数据库，分下面几步
- 获取模型对象
```
local Table = require('orm.class.table')
local _weatherTable = Table("weather”)
```
- 准备数据，建立数据对象
```
local t = {}
t.wind = flDict[v.fg]
t.wind_direction = fxDict[v.ff]
t.sun_info = v.fi
t.low = tonumber(v.fd)
t.high = tonumber(v.fc)
t.id = i
t.city = city
local weather = _weatherTable(t)
```
- 保存数据
`weather:save()`- 读取数据
`_weatherTable.get:all():getPureData()`
是不是很简单，很优雅，什么建表、拼sql、transation、线程安全等等都不用考虑，傻瓜式地使用，一个业务就几行代码搞定。这里只演示了简单的存取，更多的select、update、联表等高级用法可参考[db_test demo](https://github.com/williamwen1986/Luakit/blob/master/LuaKitProject/src/Projects/LuaSrc/db_test.lua)。
## Flutter ORM数据库原理详解
好了，上面已经介绍完如何使用了，如果大家仅仅关心使用下面的可以不看了，如果大家想了解这套跨平台的ORM框架的实现原理，下面就会详细介绍，其实了解了实现原理，对大家具体业务使用还是很有好处的，虽然我感觉大家用的时候极少了解原理。
我们把orm框架分为三层接入层，cache层，db操作层，三个层分别处于对应的线程，具体可以参考下图。接入层可以在任意线程发起，接入层也是每次数据库操作的发起点，上面的demo所有操作都是在接入层，cache层，db操作层仅仅是ORM内部划分，对使用者来讲不需要关心cache层和db操作层。我们把所有的操作分成两种，db后续相关的，和db后续无关的。
![image](https://image-static.segmentfault.com/214/962/2149626986-5c45c9f368635_articlex)
db后续无关的操作是从接入层不同的线程进入到cache层的队列，所有操作在这个队列里先同步完成内存操作，然后即可马上返回接入层，异步再到db操作层进行db操作。db后续无关的操作包括 save、update、delete。
db后续相关的操作依赖db操作层操作的结果，这样的话就必须等真实的db操作完成了再返回接入层。db后续相关的操作包括select。
要做到这种数据同步，我们必须先把orm操作接口抽象化，只给几个常用的接口，所有操作都必须通过指定的接口来完成。我们总结了如下基本操作接口。
1、save
2、select where
3、select PrimaryKey
4、update where
5、update PrimaryKey
6、delete where
7、delete PrimaryKey
这七种操作只要在操作前返回前对内存中的cache做相应的处理，即可保证内存cache始终和db保持一致，这样以后我们就可以优先使用cache层的数据了。这七种操作的实现逻辑，这里先说明一下，cache里面的对象都是以主键为key，orm对象为value的形式存储在内存中的，这些控制逻辑是写在[cache.lua](https://github.com/williamwen1986/Luakit/blob/master/LuaKitProject/src/Projects/LuaSrc/orm/cache.lua)里面的。
下面详细介绍七种基本操作的逻辑。
- save操作，同步修改内存cache，然后马上返回接入层，再异步进行db replace into 的操作
![image](https://image-static.segmentfault.com/929/186/929186764-5c45c9f4bec9c_articlex)
- where条件select，这个必须先同步到db线程获取查询结果，再同步修改内存里面的cache值，再返回给接入层
![image](https://image-static.segmentfault.com/250/845/2508451597-5c45c9f1be0dd_articlex)
- select PrimaryKey，就是选一定PrimaryKey值的orm对象，这个操作首先看cache里面是否有primarykey 值的orm对，如果有，直接返回，如果没有，先同步到db线程获取查询结果，再同步修改内存里面的cache值，再返回给接入层
![image](https://image-static.segmentfault.com/301/006/3010068347-5c45c9f0c1e57_articlex)
- update where，先同步到db线程通过where 条件select出需要update的主键值，根据主键值和需要update的内容，同步更新内存cache，然后异步进行db的update操作
![image](https://image-static.segmentfault.com/367/991/3679918669-5c45c9f34bd01_articlex)
- update PrimaryKey，根据PrimaryKey进行update操作，先同步更新内存cache，然后异步进行db的update操作
![image](https://image-static.segmentfault.com/206/903/206903705-5c45c9f43499f_articlex)
- delete where，先同步到db线程通过where 条件select出需要delete的主键值，根据主键值删除内存cache，然后异步进行db的delete操作
![image](https://image-static.segmentfault.com/388/202/3882022748-5c45c9f1b212e_articlex)
- delete PrimaryKey，根据PrimaryKey进行delete操作，先同步删除内存cache，然后异步进行db的delete操作
![image](https://image-static.segmentfault.com/380/499/3804996665-5c45c9f169666_articlex)
只要保证上面七种基本操作逻辑，即可保证cache中的内容和db最终的内容是一致的，这种尽量使用cache的特性可以提升数据库操作的效率，而且保证同一个db的所有操作都在指定的cache线程和db线程里面完成，也可以保证线程安全。
最后，由于我们所有的db操作都集中起来了，我们可以定时的transation 保存，这样可以大幅提升数据库操作的性能。
## 结语
目前Flutter领域最大的痛点就是数据库操作，本文提供了一种优雅使用ORM数据库的方法，大幅降低了使用数据库的门槛。希望这篇文章和flutter_luakit_plugin可以帮到大家更方便的开发Flutter
应用。
