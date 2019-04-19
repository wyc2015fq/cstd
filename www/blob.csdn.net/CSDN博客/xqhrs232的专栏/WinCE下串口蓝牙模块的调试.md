# WinCE下串口蓝牙模块的调试 - xqhrs232的专栏 - CSDN博客
2015年07月02日 14:41:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：695
原文地址::[http://www.cnblogs.com/we-hjb/archive/2010/11/16/1878917.html](http://www.cnblogs.com/we-hjb/archive/2010/11/16/1878917.html)
相关文章
1、蓝牙耳机怎么连接手机 简单四步搞定----[http://www.to8to.com/yezhu/v3334.html](http://www.to8to.com/yezhu/v3334.html)
2、蓝牙耳机怎么用 手机蓝牙的使用方法----[http://www.pc841.com/shoujijiqiao/7144.html](http://www.pc841.com/shoujijiqiao/7144.html)
拿到模块后，先请ZWEI帮忙把模块飞到板子上，最后的情形有点像蜈蚣跟蜘蛛在打仗，而且难分难解。
    飞完线后用串口调试助手发送AT#指令，以确认蓝牙模块和串口都能正常工作。结果，运气不太好，发出去的指令就好像石沉大海了一样。
    又请ZWEI用示波器量了一下RXD和TXD，信号都正常。把TXD和RXD短接，用串口调试助手自发自收，也不成功。串口调试助手，应该是没问题的，久经考验太长时间了。最后，飞线到MCU的另外一个串口上就行了。ZWEI飞来飞去的功夫实在了得。
    系统上电，还没来得及运行原厂提供的测试程序，M8已经能搜索到模块了。否极泰来啊。
    由于之前用的一款蓝牙芯片，很不理想。所以这次在调试时，希望能在前期验证的更全面一些，以免影响产品的整体性能，降低用户体验。
    就目前碰到的问题来说，兼容性、稳定性和功能是调试蓝牙模块时最关键的几个。所以，针对这款蓝牙模块做了一个小工具，以验证上述的三个问题。
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE_1224B/image_thumb.png)
![image](http://images.cnblogs.com/cnblogs_com/we-hjb/WindowsLiveWriter/WinCE_1224B/image_thumb_1.png)
    完成该工具后，先后测试了近10款手机，包括iPhone 4、Nokia E75、M8、乐Phone、联想I908和摩托罗拉E6等，没有发现不兼容的。而这几款手机，也极具代表性。
    设置模块的名称、开机自动连接、模块异常后的自动恢复等功能都没有任何问题，反应速度也很快。
    电话的拨号功能、DTMF和音源切换也都正常工作。个别手机有问题也都是手机自己的问题。如M8手机的蓝牙是没有办法正常拨号的。
    A2DP和AVRCP工作也都正常，只是不同的手机发过来的消息不一样。其中iPhone 4是最完美的，该有的消息都有，不该有的也不乱发。
    电话本功能在iPhone 4、E75和乐Phone上都能工作。模块可以主动请求同步电话本，也可以接收手机发送过来的名片。M8手机是不行的，模块请求同步电话本时总是返回失败。M8发送名片过去，模块收到的全是空白。
    调试该蓝牙模块时唯一需要注意的是，它返回的电话本人名和配对手机的名称是UTF-8编码的，在解析时需要做相应的转换，否则遇到中文就会显示成乱码。
