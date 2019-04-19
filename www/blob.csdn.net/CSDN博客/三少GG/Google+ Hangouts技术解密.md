# Google+ Hangouts技术解密 - 三少GG - CSDN博客
2011年10月27日 10:43:12[三少GG](https://me.csdn.net/scut1135)阅读数：1201标签：[google																[h.264																[聊天																[视频会议																[浏览器](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=视频会议&t=blog)](https://so.csdn.net/so/search/s.do?q=聊天&t=blog)](https://so.csdn.net/so/search/s.do?q=h.264&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
[http://www.leiphone.com/google-hangouts-tech.html](http://www.leiphone.com/google-hangouts-tech.html)    还不错的移动互联网网站，值得关注！
从周二Google开始推出[Google+](http://leiphone.com/?s=Google%2B)计划起，很多用户就对它的群组视频聊天功能Hangouts表现出了极大的兴趣。我也是，但是不仅仅是因为它好玩而且容易使用。我真正看重的是它背后的技术。从它的萌芽开始，Hangouts就是一个有趣的云服务。而在不久的将来，它将可能成为一个能够在很多浏览器和所有设备下运行的标准的会议视频方案。
Google对Hangouts的计划保持缄默，也没有透露过多有关这项功能背后的技术。然而，最近几个月的几项关键进展可能可以说明Hangouts是怎么运作和它的发展方向：
**云**
实现群组视频聊天需要很多资源，这就是为什么会出现用P2P来下载视频并在用户间以信号传递的解决方案。像Chatrouletter一样，Skype就是因为这个使用了P2P技术。然而，P2P可能带来一些负面影响，这些影响在你同时和10个人聊天时可能会尤为明显。这就是为什么Google为Hangout寻找另一个途径的原因。
“为了支持Hangouts，我们建立了一个全新的云视频会议平台，”Google实时交流技术的头头Justin Uberti周二在博客中如是说道。他指出，Hangouts运用了主从架构模式来“改变Google基础构架的作用。”
**浏览器整合**
Hangouts在现阶段需要你下载一个和Google Talk里使用的一样的插件来运行。然而，Google正在努力让Hangouts和Google Talk都能在不需要插件的情况下在浏览器中使用。这将会在五月开放源的实时交流的一个被称作WebRTC新架构（读作：文本、声音和视频聊天）下完成。WebRTC是Mozilla和Opera投资的项目，Google在这个月早些时候已经开始将这个架构整合到它的Chrome浏览器中。在它的项目网页上显示，“将Google Talk完全转变成WebRTC的工作已经开始。”
从这个角度上来说，用户在使用Google Talk时可以不再需要任何插件，也就是说Hangouts也一样。Google的发言人通过电子邮件告诉我Google+视频聊天功能和这个架构之间的联系：“Hangouts所运用的许多技术都能用在WebRTC里面，我们为了让WebRTC的界面看起来很好贡献了许多反馈。从这个角度来说，我们的插件和定案是不一样的成果。”他拒绝透露任何未来的计划，但是相信我，这已经很明显了……
**开放编码**
Google Talk和Hangouts在目前都是用其向Vidyo的授权许可来解决视频聊天的问题。视频传输速率是H.264/SVC，并有H.264/AVC和H.263作为后备方案。尽管如此，很明显Google将会最终转向开放编码。
去年，Google开放了VP8视频编解码器的源代码以作为其新WebM视频格式。实时交流是VP8程序员从一开始就希望改进的内容。实际上，VP8已经应用在了Skype的群组视频功能里。Google的WebM项目经理John Luther二月写到，VP8是一个“可以运用在实时交流比如视频会议上尤为好的编解码器。”
那Hangouts什么时候会从H.264过渡到WebM？Google+项目领导人Bradley Horowitz在每周Google（This Week in Google）上指出，周三他的小组已经对可替代的编解码器做了测试。当我问到有关编解码器的事情，一个Google发言人不想谈论太多有关Hangouts的未来计划，但是我们已经有了些线索：WebRTC是基于VP8编解码器的。也就是说，H.264很有可能被替换下来。
By [Janko Roettgers](http://gigaom.com/author/jroettgers/)
**设备整合**
这块非常有趣：Hangout基于云的建造和它未来的浏览器整合将会最终将会为它带来一个对所有设备来说都是最佳的视频聊天体验。桌面用户可以试用全屏视频，手机用户可以实现最优的流畅性来解决带宽问题。Google电视用户很快就可以在他们的电视上看到Hangouts，因为Google电视自带全屏的Chrome浏览器。
有些公司已经开始将多人视频聊天引入手机设备中，但是跨设备的视频会议仍旧只停留在萌芽阶段。Google可以尽早抓住这个机会。当然，Google不希望就Hangouts在手机上的应用发表太多评论，但是Google发言人也并没有完全否认这一计划：
“再次重申，我们不能透露太多我们的未来产品计划，但是Google+在手机产品上的投入不小。我们相信你应该想要能够随时随地分享和交流，不管你是在网上，平板电脑或者电话。”
