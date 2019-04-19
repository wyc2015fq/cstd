# XACT Q&A - 逍遥剑客 - CSDN博客
2009年04月21日 20:32:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2185标签：[compression																[音乐																[web服务																[random																[audio																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=random&t=blog)](https://so.csdn.net/so/search/s.do?q=web服务&t=blog)](https://so.csdn.net/so/search/s.do?q=音乐&t=blog)](https://so.csdn.net/so/search/s.do?q=compression&t=blog)
个人分类：[视频音频](https://blog.csdn.net/xoyojank/article/category/484788)
1. 怎么做声音间切换时的淡入淡出效果?
    (1)对于背景音乐: 把所有背景音乐归为一个分类(catigory), 如Music, 属性设这样:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090421/1_XACT_Music_FadeInOut.JPG)
    (2)对于音效: 建立一个RPC, 在把里面的参数设置成AttackTime/ReleaseTime控制Volume的, 然后根据需要绘制一个音量随时间变化的曲线, 再把需要这个效果的sound全attach到这个RPC上就OK.
2. 怎么压缩音频文件? wav太占地方
    XACT在打包时会把wav(源文件)进行格式转换, 支持ADPCM和xWMA两种压缩格式. 做法: 首先在"Compression Presents"上右键单击, 选择"New Compression Present", 然后在其属性面板上选择相应的压缩格式, 如WMA, 还可以调节质量(quality)参数. 然后, 在需要进行压缩的WaveBank的属性面板中选择刚才新建的Compression Present, 编译工程就可以看到效果了.
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090421/2_XACT_Wavebank_Compression1.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090421/2_XACT_Wavebank_Compression.JPG)
3. 为什么我的3D音效没有距离感? 就是说音量没有近大远小的效果
    关于音量的距离衰减, 这属于RPCs的一部分. 我们需要对所有需要有这个效果的sound指定一个RPC Present. 新建一个RPC Present, 双击打开参照下图进行设置, 再把这个Present拖动到指定sound就OK了: 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090421/3_XACT_Sound_DistanceVolume.JPG)
4. 为什么我的wav文件XACT不识别?
    XACT只支持PCM格式的音频, 建议统一使用PCM 16bit 44100 stereo格式的wav文件做为源文件.
5. 为什么在XACT工具中试听声音时程序会卡死变成无响应?
    试听之前需要先运行"Audio Console", 这相当于一个服务器端. 另外, 由于端口80被占用, 在开了IIS等web服务器机器上也会出现连接失败的现象.
6. 怎样随机播放多个sound中的一个?
    把选中多个sound拖动到一个cue上面, 在cue的右侧列表里已经显示出各个sound的名字和播放概率. 默认的播放方式就是"Random(no immediate repeats), 在程序中只需要一个cue名字就可以随机播放多个声音了. 另外, 也可以通过在sound的"Play Wave"下挂接多个wave来实现. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090421/6_XACT_RandomPlay.JPG)
7. 怎样循环播放一个cue?
    在cue对应的sound属性中选中"Looping"下的"Infinite", 即无限循环.
8. 怎么样在程序运行时调节音量? 
    一种方法是IXACT3Engine::SetVolume来控制整个分类的音量, 另一种方法是建立一个Gobal Variable, 通过RPC来进行控制:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090421/8_XACT_SetVolume.JPG)
    当然, 程序跟XACT进行交互的媒介就是这个"MusicVolume"变量, IXACT3Engine::SetGlobalVariable/IXACT3Engine::GetGlobalVariable
9. 为什么交互式(interactive)cue播放完后不会自动停止?
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20100104/Ee415970_xact_properties_cue_3(en-us,VS_85).jpg)
    interactive cue可以看作有"on/off"两种状态, 从Play开始, 保持"on"状态, 直到"Stop()"结束才变成"off"(即销毁). 在播放完一个sound后, cue进入"(stop)" transaction state, 等待变量的改变, 并不会自动结束自己. 这时候改变相关变量的值会引发一个新的sound播放, 当然在transaction中可以设置更加复杂的声音切换与过渡...
10. 怎样加速播放声音?
    其实, 升高Pitch(音调)就是加速播放. XACT中的音调范围是(-12~12), 经我用秒表计算大约是0.5倍速~2倍速(-_-). 调节音调的方法跟音量一样, 都是通过RPC来完成的. 参见8
11. 为什么流式加载的音乐有时播放不连续?
    如果不定期调用IXACT3Engine::DoWork(), 流式加载的缓冲播放完毕后声音就会断掉. 所以, 把这个调用放入一个独立的线程才不会受制于主循环的调用频率影响.
