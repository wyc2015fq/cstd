# 【GPL和LGPL】【VPlayer不开源】【其他开源的基本都仅是ffmpeg修改部分】 - 三少GG - CSDN博客
2011年06月11日 13:04:00[三少GG](https://me.csdn.net/scut1135)阅读数：4496
 1.   
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2. 以下内容用chrome浏览器打开：
原作者地址： [http://abitno.me/release-android-media-player-divx-real-rmvb](http://abitno.me/release-android-media-player-divx-real-rmvb)
### [发布一Android全能播放器](http://abitno.me/release-android-media-player-divx-real-rmvb)
POSTED AT: 2010-10-10 12:38:43 UTC | POSTED IN:[Android](http://abitno.me/categories/Android)|[83 COMMENTS](http://abitno.me/release-android-media-player-divx-real-rmvb#comment_list)
又一个Android播放器而已，调用了FFmpeg的LGPL代码，所以各种视频封装和编码格式基本都支持，测试过avi mkv rmvb mp4 wmv ogv没有问题。
使用Milestone（CPU 600MHz，RAM 256M，Andriod 2.1）能流畅播放360P高码率视频，480P低码率也基本可以播放，不过要在降低画质的前提下。
先发张截图看看吧，这是一360P的RMVB视频，效果还不错，整个过程声画同步正常，也一直处于比较流畅的状态，画质基本没受影响。
“
"
havlenapetr不是提供了一个demo嘛。。。
开不开放代码是博主的权利
不过即使提供代码 又有多少人能编译出来呢
**arm汇编优化ffmpeg就更不谈了****目前可以看到的ffmpeg player代码基本都停留在JNI这一层**ffmpeg的优化只有rockplayer做了些"
"
目前开源的至少可以找到4套代码
呵呵 然后成功完成编译的好像没多少
感觉代码并不重要，能看明白的自己也就能写了
不然有了代码也还是看不明白的-_-~~~
havlenapetr的思路很好
1.6-2.2的兼容问题基本解决了
注：系统库可以从模拟器直接扒出来，不用学着havlenapetr重新生成
祝博主早日完成优化:)"
"
- 
IrisHe[on 2010-11-17 at 08:39](http://abitno.me/release-android-media-player-divx-real-rmvb#comment_7316)
FFMpeg的许可协议要求使用FFMpeg的软件开源
[](http://abitno.me/release-android-media-player-divx-real-rmvb/comments/new?reply_to=7317)[ABitNo](http://abitno.me/)[on 2010-11-17 at 11:45](http://abitno.me/release-android-media-player-divx-real-rmvb#comment_7317)
[@IrisHe](http://abitno.me/release-android-media-player-divx-real-rmvb#comment_7316)
**哥们，请看LGPL**
"
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
3. 关于上文中谈到的VPlayer 不公布源代码！！！
下面为原作者博客(chrome)打开：  [http://abitno.me/android-ndk-ffmpeg-for-vplayer](http://abitno.me/android-ndk-ffmpeg-for-vplayer)
> 
首先为自己的错误郑重的道歉，第一个错误是声明为”free forever“的VPlayer最后成了收费的了，第二个是没有及时的公布用到的FFmpeg源码。错了就是错了，没有什么值得为自己辩解的，得让实际行动证明一切。
VPlayer使用了[FFmpeg](http://ffmpeg.org/)的代码，基于[LGPLv3](http://www.gnu.org/licenses/lgpl.html)协议，修改过的代码可以[点击这里](http://vplayer.net/projects/vplayer/files)下载。
> 
附注：现在好多Android上的播放器在使用FFmpeg，似乎都公布了FFmpeg的代码，可它们公布的貌似不是自己用到的。简单的验证：按照他们的文档介绍看一下能否正常编译，如果能正常编译，把得到的libffmpeg.so与他们程序里的对比一下大小是否一致，然后替换他们程序里的libffmpeg.so，看看还有没有同样的效果。
- "
![angrycans](http://www.gravatar.com/avatar/9ecc5ac9737045c1a1d08fe64c849aa7?r=g&d=monsterid&s=48)
[>>REPLY<<](http://abitno.me/android-ndk-ffmpeg-for-vplayer/comments/new?reply_to=7916)angrycans[on 2011-02-14 at 08:12](http://abitno.me/android-ndk-ffmpeg-for-vplayer#comment_7916)
博主发表的源代码也是不可以编译的,很简单的几个
在havlenapetr-FFMpeg 中说明的问题都没有修改,是不可能编译过去的.
例如#define restrict 和那几个static 函数的问题.
- 
![ABitNo](http://www.gravatar.com/avatar/7008a3be585decb7b82a3de4f293bfec?r=g&d=monsterid&s=48)
[](http://abitno.me/android-ndk-ffmpeg-for-vplayer/comments/new?reply_to=7929)[ABitNo](http://abitno.me/)[on 2011-02-15 at 02:28](http://abitno.me/android-ndk-ffmpeg-for-vplayer#comment_7929)
[@angrycans](http://abitno.me/android-ndk-ffmpeg-for-vplayer#comment_7916)
。。。。。
你试试是可以的
" 这个问题怎么回事？不管他了，反正不开源也不用这个了。。。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
- 4. 我的出路在哪里？ 难道还是havlenapetr ?
源码还没想开放，我还想怎么着给我赚点钱。。。
不过我会继续写完怎么完成一个播放器的基本东西
# 关于GPL和LGPL
除了GPL和LGPL外，还有BSD和MPL，但是在Linux中GPL和LGPL是最主流的两种。GPL和LGPL都是公众的License，用于主要限定了使用者，以及在上面进行二次开发的开发者权限，即他的权力和义务，而不是去宣称提供者的利益。
　　简单地说：GPL是带有病毒传播性的，你包含了GPL的东西，无论是否修改，你也必须是GPL，LGPL则是你修改了LGPL的内容，你也必须是LGPL。
**GPL（GUN通用公共许可证）**
**保护对象是应用程序**： 例如Kernel, butil tools, gcc,gdb 。
**为开发者与用户提供较高的自由度**，让他们能够在几乎没有限制的情况下使用/修改/发布软件。 
- 只要不更改许可证和版权声明，可随意复制程序。
- GPL对软件提供的许可不包含任何担保，除非该担保是由发行者提供的。为别人提供复制品及担保服务时，可以收取费用。（GPL和LGPL鼓励软件自由分发，但不代表GPL/LGPL软件必须免费，像redhat linux enterprise就是收费软件，只是付费用户拿到软件后还可以再向其他人免费分发，因此GPL/LGPL软件的厂家大多只能收取软件增值服务费）
- 发行程序的二进制副本时，必须随附源码，通常指“本来的”源码。
- 只要修改或者包含受GPL保护的程序或者程序的任何一部分，需遵照GPL的规定，否则不能对外发行。
- 将未经修改的软件包装起来，则不在上面的限制。
**对于执行GPL软件：**无须担心GPL，例如使用gcc编译器
**对于****在原有软件基础上建立衍生作品**- 需遵循GPL规定
- 无论采用静态还是动态链接方式，私有应用在执行期间通过动态链接整合了GPL软件，仍将受到GPL制约
- GPL软件的协议扩散范围仅限于进程空间内，跨进程使用GPL软件不受GPL约束
**对于专利问题：**任何专利必须以允许每个人自由使用为前提，否则就不准许有专利。 如果你重新发布 GPL 软件，你就明文授权了接受方使用此 GPL 软件中包含的你所拥有的专利。
**LGPL （更为宽松的GPL，即Lesser GPL）**
**保护对象是链接库：**例如Glibc, GTK widget toolkit 。
**对于****在原有软件基础上建立衍生作品**- **在程序中使用部分未经修改的LGPL程序，无须考虑LGPL的限制**
- 对于修改的LGPL，必须遵照LGPL规定，否则不能对外发行
- 通过动态或者动态方式连接受到LGPL包括的库是，不受限制，可使用任何许可证发行软件。对于LGPL软件的代替问题，如果静态链接方式，必须给取用软件的人提供应用程序在链接之前的目标码。使用LGPL库，如果导入库的源代码一起编译，即使没修改库代码也要开源，如果采用静态链接（include了库的头文件），不需要发布源代码，但要发布.o（obj）文件，如采用动态链接，就可以完全封闭。
**专利问题**- 一个函数库所能取 得的任何专利，必须与本许可证所声明的“完全自由使用”一致。
**MPL（The Mozilla Public License)**
- **一些重要条款**
- 经MPL许可证发布的源代码的修改也要以MPL许可证的方式再许可出来
- “发布”的定义是“以源代码方式发布的文件”，这就意味着MPL允许一个企业在自己已有的源代码库上加一个接口，除了接口程序的源代码以MPL 许可证的形式对外许可外，源代码库中的源代码就可以不用MPL许可证的方式强制对外许可。
- 允许被许可人将经过MPL许可证获得的源代码同自己其他类型的代码混合得到自己的软件程序
**专利**- 源代码的提供者不能提供已经受专利保护的源代码（除非他本人是 专利权人，并书面向公众免费许可这些源代码），也不能在将这些源代码以开放源代码许可证形式许可后再去申请与这些源代码有关的专利。
　　这些许可证，在美国是有法律保护，在中国如果违反，不清楚是否有问题，因为不清楚这些许可证在中国是否具有法律效应。国内的知识产权法律在开源协议保护方面似乎比较欠缺，国内有很多违反开源协议的企业，但没有哪些受到法律制裁。据说只有开源软件的作者亲自去告违反开源协议的企业才有胜诉的可能。但是从开发者的角度看，GPL和LGPL反对通过专利收费的方式来进行技术的垄断或者霸占，他们提供了公开开放的方式，也要求他们的使用者遵循这个原则。这是网络的共产主义条款，我们应遵循他们，去年green坝在这方面不太厚道，网上有不少研究评论，不凑这个热闹了。
##### 什么是 LGPL
　　LGPL是GPL的变种，**早期称之为“库级-GPL”(Lib-GPL)，**后来称之为“轻型-GPL”(Light-GPL)或“宽松-GPL”(Lesser-GPL)。和严格的、限制商业行为的GPL规定不同，**LGPL允许商业公司以库和链接的方式使用GPL代码。**
**LGPL是GPL的一个为主要为类库使用设计的开源协议**。和GPL要求任何使用/修改/衍生之GPL类库的的软件必须采用GPL协议不同。 LGPL允许商业软件通过类库引用(link)方式使用LGPL类库而不需要开源商业软件的代码。这使得采用LGPL协议的开源代码可以被商业软件作为类库引用并发布和销售。
但是如果修改LGPL协议的代码或者衍生，则所有修改的代码，**涉及修改部分的额外代码和衍生的代码都必须采用LGPL协议**。因此LGPL协议的开源代码很适合作为第三方类库被商业软件**引用**，但不适合希望以LGPL协议代码为基础，通过修改和衍生的方式做二次开发的商业软件采用。
　　GPL/LGPL都保障原作者的知识产权，避免有人利用开源代码复制并开发类似的产品。
