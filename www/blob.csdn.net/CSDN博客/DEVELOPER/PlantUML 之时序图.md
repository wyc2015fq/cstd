# PlantUML 之时序图 - DEVELOPER - CSDN博客





2018年06月20日 22:13:29[学术袁](https://me.csdn.net/u012827205)阅读数：1351








# UML Sequence

今晚上借着燥热的天气学习下UML图的使用，然后开始连续3篇，时序图、活动图、类图的学习记录，为对项目开发中混沌的业务逻辑还以清晰.

> 
UML Sequence 
[UML Class](https://blog.csdn.net/junhuahouse/article/details/80767632)

  UML Activity
Unified Modeling Language (UML)又称统一建模语言或标准建模语言，是始于1997年一个OMG标准，它是一个支持模型化和软件系统开发的图形化语言。PlantUML是快速创建UML图形的一个组件。

## Android Studio 安装插件

使用PlantUML很多平台可以，需要你进行安装使用。这里我使用了我的开发工具`Android Studio`，在菜单选项中选中`File/Settings/Plugins/Browse repositories...`输入`PlantUML`搜索，然后找到后点击Install安装重启IDE即可。如果不显示效果图，你需要安装`Graphviz`
![这里写图片描述](https://img-blog.csdn.net/20180620210258294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 安卓app使用token登录逻辑

这里简单使用PlantUML画一个使用token登录的时序图 
![这里写图片描述](https://img-blog.csdn.net/20180620221024865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
@startuml
hide footbox
title login by token
actor "学术袁" as Y #red

Y->"App客户端" as A :启动
activate A
A->A : 检测本地登录信息

alt 有本地记录
    alt token有效
           deactivate A
           A-> "后台服务器" as H :token自动登陆
    else token无效
           A-> Y : 提示登录
           Y-> A : 输入手机号+密码
           A-> H : 手机号+密码登录
           H-> A : 返回新的token
           A-> A : token保存
    end
else 无本地记录
 A-> Y : 提示输入手机号
 A-> H : 请求发送验证码
 H->A : 返回验证码
 Y->A : 输入验证码
 A->H : 请求后台验证
    alt 已注册
        A-> Y : 提示登录
        Y-> A : 输入手机号+密码
        A-> H : 手机号+密码登录
        H-> A : 返回新的token
        A-> A : token保存
    else 未注册
        A-> Y : 提示注册
        Y-> A : 输入两次密码
        A-> H : 请求注册
        H-> A : 返回新的token
        A-> A : token保存
    end
end
@enduml
```

## UML Sequence 介绍

### 【1】图标样式

![这里写图片描述](https://img-blog.csdn.net/20180620212109436?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
@startuml
hide footbox
actor actor #red
boundary boundary #yellow
control control #blue
entity entity #green
database database #EE4000
"学术袁" -> Blog :方形
@enduml
```

### 【2】注解方式

分为行注解使用英文符号 `'+行内容`

多行注解使用英文符号`/' + 内容 +  '/`
![这里写图片描述](https://img-blog.csdn.net/20180620212600644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
@startuml
hide footbox
'actor actor #red
boundary boundary #yellow
control control #blue
entity entity #green
database database #EE4000
/'"学术袁" -> Blog :方形
[o->P:圈圈'/
@enduml
```

### 【3】箭头样式+着色

使用各种箭头来标识消息的方向，并可以加以颜色区分 
![这里写图片描述](https://img-blog.csdn.net/20180620213724725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
@startuml
  hide footbox
  actor "学术袁" #red
  "学术袁" -[#912CEE]> Tencent : message
  Tencent--> Alibaba : message
  Mi<<- Alibaba : message
  Jd<<-- Mi : message
  Jd-\ VIP : message
  Jd--\ VIP : message
  Jd--\\ VIP : message
  Tencent<--> VIP : message
  Tencent<-> VIP : message
  Tencent-[#7FFF00]>o Jd : message
  Tencent-->o User : message
  "媳妇儿"--\o VIP : message
@enduml
```

### 【4】分页+消息序号

![这里写图片描述](https://img-blog.csdn.net/20180621195810271?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
@startuml
autonumber
  title page one
    "产品经理" -> "程序员" : 你应该把这个图标缩小1px
    "程序员" -> "产品经理" : 先问问我手中的大刀

  newpage page two
    autonumber 20
    "产品经理" -> "程序员" : 下班回家吧
    "产品经理" <- "程序员" : 我是有梦想的人

    autonumber 38 10 "<font color=red>Message 0  "
    "产品经理" -> "程序员" : 说笑呢
    "产品经理" <- "程序员" : 我是认真的

  newpage  title for the\nlast page
    autonumber "<b>[100]"
    "程序员" <- "产品经理" : 要不再改下UI
    "程序员" -> "产品经理" : 一起回

    autonumber 18 "<b>(<u>##</u>)"
    "程序员" <- "产品经理" : 你留下来加会班
    "程序员" -> "产品经理" : 好的，那就不请你吃火锅了
@enduml
```

### 【5】消息分组

![这里写图片描述](https://img-blog.csdn.net/20180621201803654?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
@startuml
hide footbox
alt 成功
    "小新" -> "父亲广志": 要零花钱10元
  else 失败
    "小新" <- "父亲广志": 反遭挨骂
    group 继续要钱
      "父亲广志" -> "母亲美伢" : 别给小新零花钱
      loop 30 次
        "小新" -> "父亲广志": 给我零花钱..
      end
      loop 35 次
         "小新" -> "母亲美伢": 给我零花钱..
      end
      "父亲广志" <-> "母亲美伢" : 要不就给他点吧？
    end
  else 失败
    "小新" -> "父亲广志": 我要补习班费
end
@enduml
```

### 【6】消息注解+HTML格式

![这里写图片描述](https://img-blog.csdn.net/20180621202406362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
@startuml
hide footbox
  "小新" as A -> "美伢" as B : hello
  note left: 向母亲打招呼

  B -> A : ok
  note right: 母亲回应

  A -> A : 自忖
  note left
       要零花钱
       应该要多少合适
       找什么借口
  end note
@enduml
```

![这里写图片描述](https://img-blog.csdn.net/20180621203318503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```xml
@startuml
hide footbox
 "小新" as A -> "美芽" as B : A <i>我要零花钱</i> 10元
  note right of  B
    没有 <back:cadetblue><size:18>零钱了</size></back>
    <u>不信</u> 给你看.
  end note
  note left of A
    <u:red>小新</u> 认真 <color #118888>思考</color>
    <b><color purple>不太相信</color> <s:red>母亲的话</strike>哦！</b>
  end note
  note over A, B
    <w:#FF33FF>但事实</w> 是这里出现了图片哦 <img ./src/main/res/mipmap-mdpi/ic_launcher.png>
  end note
@enduml
```

### 经典时序图共赏

![这里写图片描述](https://img-blog.csdn.net/20180626164436361?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
@startuml
hide footbox
actor 学术袁 as A #red
database 后端服务 as B #green
entity 有文化的司机 as C #orange

activate A #red
note over A : 完成登录
A-[#red]>A : 定位起始位置，\n展示规划线路
A-[#red]>B : 打车，呼叫司机
deactivate A
B-[#green]>C : 订单池中派发
activate C #orange
C-[#orange]>C : 提醒司机接单
C-[#orange]>B : 接单
deactivate C
B-[#green]>A : 转发通知，\n司机已接单

activate A #red
A-[#red]>A : 更新已接单状态，\n展示乘客和司机间的距离路线
note over of A : 等待司机到达乘车位置..
note over of C : 接驾驾驶开始
C-[#orange]>B : 行驶中..
B-[#green]>A : 转发通知，\n司机定位行驶状态..
A-[#red]>A : 更新乘客、司机定位状态
...
note over of C : 行驶到接驾地点
C-[#orange]>B : 到达乘车位置
B-[#green]>A : 转发通知，\n司机已到乘车位置
A-[#red]>A : 提示乘客前往用车
deactivate A

activate C #orange
C-[#orange]>B : 已接到乘客
B-[#green]>A : 转发通知，即将启程
activate A #red
A-[#red]>A : 更新定位状态，\n展示前往目的地的线路

note over of C : 前往目的地驾驶开始
deactivate A
C-[#orange]>B : 行驶中..
B-[#green]>A : 转发通知，司机行驶状态..
A-[#red]>A : 更新乘客距离目的地的定位状态
...
note over of C : 行驶到目的地
C-[#orange]>B : 结束行程
deactivate C
B-[#green]>A : 转发通知，\n已到达目的地
activate A #red
A-[#red]>A : 提示支付
note over of A:支付

deactivate A



@enduml
```

![这里写图片描述](https://img-blog.csdn.net/20180621200100631?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
note right: Note on right

B-->A: message
deactivate B
activate A

A->A: message
activate A
deactivate A
deactivate A

A->o C
note right: 箭头样式

A-[#blue]>C
note right: 箭头颜色

alt AD
    A->D: message
else DA
    A->D: message
end
note over D #FFAAAA: 组合消息

D->E:message
note right: Creole和HTML
note right of E
  This is **bold**
  This is //italics//
  This is ""monospaced""
  This is --stroked--
  This is __underlined__
  This is ~~waved~~
end note

== 分隔符 ==

E->F: message

...
note over A:延迟
F->A:message

box "包裹参与者DEF" #LightBlue
    participant D
    participant E
    participant F
end box
@enduml
```

参考内容： 
[https://www.cnblogs.com/Jeson2016/](https://www.cnblogs.com/Jeson2016/p/6837186.html)









