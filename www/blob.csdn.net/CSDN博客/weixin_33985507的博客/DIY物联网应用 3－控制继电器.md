# DIY物联网应用 3－控制继电器 - weixin_33985507的博客 - CSDN博客
2016年11月21日 20:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
在上一章[《我的第一个物联网应用 2》](https://link.jianshu.com?t=http://www.liuguangli.win/archives/586)中，我们完成了 Ruff SDK 的安装，开发环境已经准备就绪，这一章我们的目标是：
- 新建一个 Ruff 项目，熟悉项目结构
- 以继电器为例学会添加连接外设，并控制外设
下面有请这一期的主角入场。
- 
Ruff 开发板：
![1344733-3f91bbe37c673087.jpg](https://upload-images.jianshu.io/upload_images/1344733-3f91bbe37c673087.jpg)
主板
- 
继电器
![1344733-3470cada5be12a27.jpg](https://upload-images.jianshu.io/upload_images/1344733-3470cada5be12a27.jpg)
继电器
## 继电器的作用
为了方便理解后面的内容，这里先简单科普一下继电器的工作原理，如果你已经知道继电器的作用可以跳过这段内容。
什么是继电器？继电器是一种通过某种信号变化来自动控制电路开关的电子设备。
![1344733-26451ae82b5e509a.png](https://upload-images.jianshu.io/upload_images/1344733-26451ae82b5e509a.png)
继电器简图
上图是最简单的继电器模型图，输出电路有三个端口:常闭、常开和公共端，默认情况下常闭端和公共端是连通的，控制端根据输入的电信号能控制摆动条向下或向上摆，从而控制公共端和常闭端或者常开端连通。例如下图就是一个基于电磁信号来实现的继电器:
![1344733-293286a2e0782f20.jpg](https://upload-images.jianshu.io/upload_images/1344733-293286a2e0782f20.jpg)
电磁信号继电器
大家回忆一下中学物理课中的电磁感应原理，当线圈通电后，线圈环绕的铁芯会产生磁力，在磁力的吸引作用下铁片和常闭端断开、和常开端联通；当线圈断电，铁芯磁力消失，铁片在弹簧拉力的作用下和常闭端联通。 如果我们用开发板控制继电器来控制风扇，那么电路连接情况可以如下图：
![1344733-456daa2888127e66.png](https://upload-images.jianshu.io/upload_images/1344733-456daa2888127e66.png)
## 创建项目
思路理清楚了，下面我们终于可以写代码了，开干吧！创建一个目录：MyLotApp。进入项目目录。执行命令：
```
rap init
```
接下来命令终端会让你输入项目名称、描述、作者等信息，写过 NodeJs 的同学一定会觉得很亲切，项目初始化完成之后我们来看看工程的目录结构：
![1344733-65b4468787216efc.png](https://upload-images.jianshu.io/upload_images/1344733-65b4468787216efc.png)
工程目录
package.json:
```
{
"name": "MyLotApp",
"version": "0.1.0",
"description": "My first Lot App",
"author": "dalimao",
"main": "src/index.js",
"ruff": {
    "boards": {
        "ruff-mbd-v1": "^4.2.0",
        "*": "*"
    },
    "dependencies": {}
 }
}
```
其中 main 节点指定了程序的入口文件；ruff 节点描述工程的依赖模块，boards 是开发板的基础依赖模块，dependencies 是设备的依赖模块，我们现在还没添加外设备，所以这里是空的。
app.json: 描述设备的逻辑ID（我们在添加外设时自己指定的ID）和驱动依赖包的映射关系。
test 目录：测试代码存放目录。
src 目录：源码目录，我们写的代码就放到这个目录下。
src/index.js: 在package.json 中指定了 index.js 为项目的程序入口。
```
'use strict';
$.ready(function (error) {
    if (error) {
        console.log(error);
        return;
    }
    $('#led-r').turnOn();
});
$.end(function () {
    $('#led-r').turnOff();
});
```
开发板启动完成、加载程序之后会执行ready() 回调函数，关闭之前会执行end()回调函数。这段代码加载后，开发板会点亮开发板上内置的红色led 灯。现在我们让这段初始化生成的代码在开发板上跑起来吧，有请我们的第一位主角－Ruff 开发板出场。
## 启动开发版
用 USB 连接线给 Ruff 开发板 ，开发板随即启动。开发版启动后会启动一个名字以 Ruff 开头的 WI-FI 热点，例如 Ruff0021332 。让开发机（你的电脑）连上这个 WI-FI 热点，然后就可以把代码部署到开发板上了。
## 部署程序
打开终端，进入工程目录，输入命令：
```
rap deploy -s
```
部署成功，观察一下开发板上红色 led 灯是否亮了。
## 添加继电器
关掉开发板电源，WIFI 切回正常使用的 WIFI。先来熟悉下 rap device add <device-id>这个命令，这个命令是用来添加外设的， 后面的参数是逻辑上的设备 ID，你可以自行定义，只要保证和其他设备 ID 不重复就行了。
```
rap device add relay
```
这里我把继电器的 ID 定义为 relay ，回车之后 Ruff 命令行会询问你 Module ID， 这个才是设备真正的 ID ，这个 ID 和这个硬件的驱动程序是对应的，这个 ID 在设备的包装盒上有注明，输入这个 ID 回车之后 Ruff SDK 会根据 ID 在 Ruff 的软件仓库帮你寻找对应的驱动程序供你选择安装，如果这个设备还没有对应的驱动程序，你可以去根据 Ruff 的 API 去为它写驱动程序，当然这是后话了。
输入继电器的 Module  ID ：RELAY-1C，回车选择最新的驱动，然后自动到 Ruff 驱动仓库下载继电器的依赖模块。完成之后观察下项目 package.json 和 app.json 的变化，多了设备的配置信息。
安装上设备驱动之后 rap 命令行会输出设备针脚很开发板管脚的连接布局信息，你也可以通过 rap layout 命令来查询布局信息，你还可以通过 rap layout --visual 这个命令查看图形化的布局信息。
![1344733-7d92ce84ceb19e96.png](https://upload-images.jianshu.io/upload_images/1344733-7d92ce84ceb19e96.png)
布局信息
根据指示完成继电器和主板的连线，连线过程记得关闭主板电源哟。
## 控制继电器
现在我们可以写程序来控制继电器了，怎么控制？这里我们要学会去Ruff 官网查看每个设备驱动提供的 API 和用法。到 Ruff 的软件仓库：[https://rap.ruff.io/](https://link.jianshu.com?t=https://rap.ruff.io/)搜索：
![1344733-a1aac804b90c750c.png](https://upload-images.jianshu.io/upload_images/1344733-a1aac804b90c750c.png)
你可以直接搜索 Module ID，你也可以搜索设备名称。搜索之后结果页面会出现模块的信息：
![1344733-2084caaebbf0b0f6.png](https://upload-images.jianshu.io/upload_images/1344733-2084caaebbf0b0f6.png)
进入驱动页面可以看到设备模块对应的驱动的用法：
![1344733-fe3b08c9d4bbe072.png](https://upload-images.jianshu.io/upload_images/1344733-fe3b08c9d4bbe072.png)
通过 ID，索引设备对象，这里的 ID 就是我们添加设备的时候自己定义的ID，我们定义的 ID 是"relay"，所以我们可以这样来打开继电器（使公共端和常开端连通）：
```
$('#relay').turnOn(function () { 
    // 打开继电器成功的回调函数
    console.log('turn on');
});
```
现在我们把打开、关闭继电器的逻辑放到src/index.js中：
```
'use strict';
$.ready(function (error) {
    if (error) {
        console.log(error);
        return;
    }
  $('#relay').turnOn(function () { 
     // 打开继电器成功的回调函数
       console.log('turn on');
    });
});
$.end(function () {
    $('#relay').turnOff(function () { 
        // 关闭继电器成功的回调函数
       console.log('turn off');
    });
});
```
启动并连接开发板，重新部署程序，部署成功你会听到继电器发出“嗒”的一声，说明继电器打开成功。
至此，我们能创建一个应用、学会如何添加并控制外设了，下一章有请另外两位主角：玩具小风扇和红外接收器。
