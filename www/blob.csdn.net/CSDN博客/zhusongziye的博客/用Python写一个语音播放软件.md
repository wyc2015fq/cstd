# 用Python写一个语音播放软件 - zhusongziye的博客 - CSDN博客





2018年11月11日 15:40:22[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：168








单位经常使用广播进行临时事项的通知(将文字转换为语音然后通过功放广播)，**但是市面上多数语音播放软件都是收费的**，要么发音失真，要么不够稳定——经常出现莫名其妙的故障，容易给工作带来被动。**学Python这么久不如动手写一款自己的语音广播软件**，即使发生故障也可以自行排除。



1

**界面设计**



在开始动工之前当然要分析需求，我要的核心功能是将一段文字通知输入软件，然后将其转换为语音并播放出来。

这项功能虽然并不复杂，但也需要一个交互式的界面，所以决定使用Tkinter来实现这个功能。



**第一步:建立一个窗体**

设置标题、大小等要素，为了避免显示格式错乱将其设定为不可改变大小，代码如下:

![](https://img-blog.csdnimg.cn/20181111153303116.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**第二步，设置一个控件**

用于接受收入的文字，这里选择带滚动条的Text，代码如下：

![](https://img-blog.csdnimg.cn/20181111153407449.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**第三步，提供选项**

作为一款语音播放软件，最基本的语速、音调等风格设置还是要有的，这里使用Combobox控件提供固定选项，用户可以根据情况选择不同的发音、语速和语调。

![](https://img-blog.csdnimg.cn/20181111153530552.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**第四步**，建立功能事件的触发接口

设置三个Button控件分别用于触发“语音播放”、“文本清除”和“界面退出”功能。

![](https://img-blog.csdnimg.cn/20181111153645974.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**最终界面效果如下：**

![](https://img-blog.csdnimg.cn/20181111153709762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



2

**语音播放**



关于“清除”、“退出”等功能相对比较简单，这里重点对本次的核心功能——语音播放进行详细说明。



**1).语音接口**

文本转换语音推荐使用百度云的REST API 接口，登录网站http://ai.baidu.com/，依次进入控制台——语音技术页面，创建自己的语音应用(下图)，其中AppID 、API Key 、Secret Key 三个参数在代码中会用到。

![](https://img-blog.csdnimg.cn/20181111153753394.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



然后使用 pip install baidu-aip 安装python SDK模块，我们来看一下函数原型:

```
APP_ID = 'XXXXXX'
API_KEY = 'XXXXXXXXXXXXX'
SECRET_KEY = 'XXXXXXXXXXXXXXXXXXXXXX'
client = AipSpeech(APP_ID, API_KEY, SECRET_KEY)
result  = client.synthesis(text, 'zh', 1, {'per':1, 'vol':15, 'pit':9, 'spd':5 })
```
- 
text:需要转换的文字。

- 
per:发音人选择, 0为女声，1为男声，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声。vol:音量，取值0-15，默认为5中音量

- 
pit:音调，取值0-9，默认为5中语调。

- 
spd:语速，取值0-9，默认为5中语速。‘zh’和1分别为语音模式和客户端类型，该两项均为固定值，不支持修改。


可以看到，我们需要的发音、音调、语速三种风格都可以通过修改参数来实现。



**2).功能设计**

解决了语音合成接口的问题，就可以结合界面设置来实现具体功能了。
- 
首先，要将界面中的语音风格选项与语音合成函数参数一一对应起来，这是一个典型的键与值的对应关系，使用字典这种数据结构再合适不过了。

- 
然后对于发音风格，选取了男声、女声、混合三种模式;

- 
最后对于音调和语速，没必要设置过细的精度等级，这里分别选取了三个跨度明显的等级进行区分。


![](https://img-blog.csdnimg.cn/20181111153831645.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



当“播放”按钮点击时，要从Text控件中读取文本，如果文本为空则弹出提示框要求重新输入；若文本不为空则将文本转换为音频文件并使用playsound进行播放。

![](https://img-blog.csdnimg.cn/20181111153847721.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这里**有一个问题需要特别注意**，那就是在软件运行过程中，生成并播放的音频文件不可删除、不可修改、不可覆盖，所以每次转换生成的音频文件名称绝对不能重复，否则在进行多次“播放”操作时，会因为新生成的音频文件无法保存而发生故障。



3

**打包封装**

到目前为止，这个软件的运行要依赖于本地的python开发环境，不能方便地提供给他人使用，**这里推荐Pyinstaller的第三方库来对python程序进行打包**，首先进入刚才的py文件所在目录，执 行以下cmd命令。其中“-w”的作用是是不显示命令窗口，tk_voice是刚才的py文件名称。
`pyinstaller -w tk_voice.py`
这时，在同一目录下会生成一个dist文件夹，这里面就是打包后的程序文件了。我们运行一下其中的.exe文件，就会出现之前设计的程序界面，在文本框中输入一段测试文字：“**全体人员请注意，全体人员请注意，请立即下楼集合开饭。**”，点击“播放”按钮试一下效果：

**插入音频————测试.mp3**

**最后，关于Pyinstaller的使用有几点需要注意的地方：**
- 
该方法仅适用于windows系统，而且对系统版本有比较严格的要求，比如64位系统下打包的程序无法在32位系统下运行。

- 
如果需要打包的程序中调用了外部的一些图片或其他资源文件，则需要手动复制到打包后的文件夹内，因为对于这些文件Pyinstaller不会进行打包。

- 
使用Pyinstaller打包如果出现中途失败的情况，可能会导致原py文件内容丢失，所以打包之前最好先进行备份。

- 
使用import导入其他库的时候尽量有选择性，不要导入整个库，不然打包后的文件会非常庞大。




本文使用python写了一款语音播放软件，主要涉及Tkinter、baidu-aip、playsound、pyinstaller几个库，能够实现基本的语音合成及播放功能，能够脱离python开发环境运行，便于自行维护和扩展，缺点是界面比较简陋、功能比较单一，有兴趣的小伙伴可以自行修改完善。



