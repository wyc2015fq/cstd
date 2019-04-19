# XACT基本概念 - 逍遥剑客 - CSDN博客
2008年11月14日 15:49:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1759
XACT工程
- Wave Banks
- Sound Banks
- Global Settings
Wave Banks
一个wave bank是包含多个wave文件的集合的文件, 扩展名.xwb
比如界面音效一个文件, 技能音效一个文件
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081114/WaveBank.JPG)
Wave Bank模式
- In-Memory
在内存中, 适用于小音频资源. 如界面音效
- Streaming
通过一个内存缓冲读取, 适用于大音频资源. 如背景音乐
**Waves**
音频数据文件. XACT支持以下几种音频格式:
Windows audio waves(WAV)
Xbox 360's native format(XMA), 基于Windows Media Audio(WMA)
Audio interchanged file format(AIFF)
(xoyojank: 推荐使用44kHz 16bit PCM .wav文件(CD音质)作为音频源, 然后在XACT中压缩成xWMA存储)
Sound Banks
一个sound bank是一些sound和cue的逻辑集合. 文件扩展名.xsb
Sound bank不包含wave数据, 只是引用了wave bank中的wave, 并保存怎么播放它们的信息
组成结构:
- Sounds
- Cues
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081114/SoundBank.JPG)
**Sounds**
一个sound指定了怎样去播放一个或多个wave, 如音量和音调
Sound由track和event组成
**Tracks**
一个track是一组event, 一个sound由一个或多个track组成. 
每个track都可以包含一个Play Wave event, 这样一个sound就可以播放多个wave
Events
每个track至少包含一个event, 最简单的就是Play Wave event.
还有其它一些event:
- Stop 
设置停止整个sound的时刻, 如循环播放时
- Marker
当播放到标记的位置时会给应用程序一个通知. 这样可以让程序同步动作和音效
- Set Volume
可以用来弄淡入淡出音量
- Set Pitch
同上, 音调
- Set Variable
暂不支持
- Branck/Wait
暂不支持
Cues
Cue是程序员播放sound使用的. 一个cue可以由多个sound组成.
一个cue可以关联到一个playlist, 可以设置顺序/乱序/随机播放
Cue的好处是可以不用更改程序就能更换对应的sound
Global Settings
一些设置不是局限于一个sound bank或wave bank的, 这些全局设置保存在XGS文件中(工程build后生成). 有以下几种:
- Categories
- Variables
- Run-Time Parameter Controls
- DSP Effect Path Presets
- Compression Presets
**Categories**
可以设置sound的一些规则, 如音量, 最大实例个数等. 
**Variables**
用于RPCs. 范围: global/cue instance. 可见性: public/private
**Run-Time Parameter Controls**
程序可以控制sound播放时的参数. 如汽车的引擎声音声调随转速提高
**DSP Effect Path Presets**
Digital signal processing效果, 如reverb
**Compression Presets**
这里设置音频文件的编码格式和压缩率
