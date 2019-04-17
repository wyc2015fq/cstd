# OBS第三方推流直播教程 - DoubleLi - 博客园






第三方推流使用场景



1、当使用YY客户端进行直播遇到问题，暂无解决方法的时候，可以使用第三方直播软件OBS进行推流。



2、对OBS情有独钟的主播。



OBS简介：



OBS是一款比较好用的开源直播软件，目前支持的操作系统包括：win7，win8，苹果的OS X系统，不支持XP系统。目前OBS有两款产品，一款为旧版本的OBS Classic，一款为新版本的OBS Studio，建议使用OBS Studio。



OBS下载：



OBS下载地址：[https://obsproject.com/download](https://obsproject.com/download)



此为新版本的OBS Studio


<ignore_js_op>![1.png](http://xiage.yy.com/data/attachment/forum/201708/29/094355sffuqb4bhuyydbvb.png)
如果想使用旧版本的OBS，点击官网下方的链接：
<ignore_js_op>![2.png](http://xiage.yy.com/data/attachment/forum/201708/29/094421am6bwdf76zbrlfyb.png)

OBS安装：




OBS 安装时直接运行obs的安装源文件，一直下一步就可以了。



FAQ1：

如果是安装的OBS studio有一步是选择插件的，如下图，其中框起来的部分是支持intel 实感摄像头的插件，如果主播没有实感摄像头可以不勾选，如果有intel的实感摄像头可以选择勾选。（其实虎牙直播已经支持intel 实感摄像头了，可以自己查看教程哦！）


<ignore_js_op>![3.png](http://xiage.yy.com/data/attachment/forum/201708/29/094452lyydmztqmqdzsmyq.png)

FAQ2：

如果安装过程中有如下提示，说明电脑缺少必要的组件，选择“是”，下载组件并安装


<ignore_js_op>![4.png](http://xiage.yy.com/data/attachment/forum/201708/29/094541aezeum1fn5iq211e.png)
<ignore_js_op>![5.png](http://xiage.yy.com/data/attachment/forum/201708/29/094541ll8liiwzjhl1awcw.png)

OBS使用步骤



1、打开OBS软件，界面如下


<ignore_js_op>![6.png](http://xiage.yy.com/data/attachment/forum/201708/29/094623f2r34uujqmamtq3t.png)

2、添加场景：在左下角的场景部分，右键，添加场景，输入场景名字，确定即可



3、添加来源：在来源部分，右键，添加，游戏捕获，输入游戏源名称，确定，在弹出的游戏源窗口中，选择“捕捉任何全屏应用程序”，或者选择“捕捉特定窗口”，然后选择游戏，其他设置默认即可（这种方式相当于是虎牙的进程直播，通过识别游戏的进程名抓取游戏）；


<ignore_js_op>![17_253429856489862.png](http://xiage.yy.com/data/attachment/forum/201708/29/094652o6bad45qo37bi4bs.png)
如果是想使用屏幕捕捉的方式，则在来源部分，右键，添加，显示器获取或者窗口捕获，输入名称，确定，在弹出的显示器获取窗口中，勾选“捕获光标”和“多适配器的兼容性”，其他设置默认即可。（这种方式相当于是虎牙的屏幕捕捉，这里注意屏幕捕捉是获取不到全屏的画面，要将游戏设置为无边框或者是窗口全屏化）
<ignore_js_op>![8.png](http://xiage.yy.com/data/attachment/forum/201708/29/094746jz8nn5i55o8yr8ur.png)

其他图层添加：如果想添加摄像头画面或者添加图片，只需要在添加中选择对应的选项即可；



图层显示方法：在添加好的图层中，在图层前面有个眼睛样式的图标，可以点击控制图层是否显示；



图层大小调节：选中一个图层，在obs的预览中该图层出现红框，鼠标拖动红框即可调节图层的显示大小；





OBS直播参数设定



1、点击obs上面菜单栏中“文件”中的“设置”选项，如下图


<ignore_js_op>![9.png](http://xiage.yy.com/data/attachment/forum/201708/29/094815qwmi92pl8xkvpglp.png)



2、串流：这里可以先不用设置，后面会讲到




3、“输出”中的设置，输出模式选择高级



1）编码器中x264相当于虎牙直播中的CPU H.264编码，NVENC H.264相当于虎牙直播中的NVIDIA H.264（推荐选择x264）



2）速率控制：选择CBR就好



3）比特率：既码率，可以使用虎牙直播中的推荐码率，如果不知道怎么选择，可以使用配置中的自动配置功能


<ignore_js_op>![10.png](http://xiage.yy.com/data/attachment/forum/201708/29/094853n1e558r2785t1f77.png)

4）使用自定义缓存大小：勾选



5）缓冲大小：设置为与最大比特率一致



6）关键帧间隔：不要设置为0，建议设置为5，如果这里设置为0，可能会造成黑屏

OBS Studio的设置截图：


<ignore_js_op>![11.png](http://xiage.yy.com/data/attachment/forum/201708/29/094913a32e5gucddzx5ooz.png)
旧版本OBS的设置截图：
<ignore_js_op>![12.png](http://xiage.yy.com/data/attachment/forum/201708/29/095001qui8z6poqoor88no.png)

4、视频中的设置：



1）分辨率：如果希望直播画面比较流畅，网速也可以的话可以选择1920*1080，如果网速比较差，建议选择1280*720



2）FPS：既帧数，可以参考在虎牙直播中的帧数


<ignore_js_op>![13.png](http://xiage.yy.com/data/attachment/forum/201708/29/095019jppl1bx1pj141uhx.png)

5、音频中的设置



1）采样率：使用44.1khz


<ignore_js_op>![14.png](http://xiage.yy.com/data/attachment/forum/201708/29/095100rssovwcwvz1bwoww.png)

2）桌面音频设备：选择自己的扬声器

3）麦克风：选择自己的麦克风即可

其他的参数使用默认值，设置好以后，点击“应用”。在频道中设置好发言模式和伴奏。


<ignore_js_op>![15.png](http://xiage.yy.com/data/attachment/forum/201708/29/095134b37c8b93pq3qjhdq.png)



6、其他的设置项按照默认设置即可，不需要额外设置，全部设置好以后，点击“确定”即可。



第三方推流开播流程



操作步骤：



1、打开OBS软件，进入yy直播频道。



2、在频道中选择“第三方直播软件”，点击“开始直播”，在弹出的框中选择“OBS或XSplit等第三方直播软件”。



注意：使用第三方软件推流以后，YY频道设置中的参数全部无效，使用的是OBS中设定的参数，具体的设定方法参考上面的讲解。


<ignore_js_op>![16.png](http://xiage.yy.com/data/attachment/forum/201708/29/095210vyg6fqwdgqafoyw8.png)
3、选择好直播源以后，出现以下界面
<ignore_js_op>![17.png](http://xiage.yy.com/data/attachment/forum/201708/29/095248jv2juzbksyv8sz87.png)
打开OBS的设置页面，在串流页面中，选择“自定义流媒体服务器”，将频道给出的推流地址复制到OBS中的“URL”，将直播码复制到“流秘钥”，点击“应用”后“确定”即可。
<ignore_js_op>![18.png](http://xiage.yy.com/data/attachment/forum/201708/29/095320pbigxe6g0b1gex5x.png)

4、在OBS中设置好其他的参数，具体设置方法参考上面的“OBS基础使用”



5、在OBS中点击“开始串流”，进入游戏就可以开始直播了









