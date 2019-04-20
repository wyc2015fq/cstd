# 独家对话英伟达首席科学家：解码AI芯片战局 - 人工智能学家 - CSDN博客
2018年01月30日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：143
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/z7ZD1WagSLiaNnz53ia6Ocn7yzCVicictrOzzZSuJbK9Fc4YyTko3EWQCI5RycMR4Pjs0icNu6C1HuC6YCJiaic5kAm6Q/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：智东西*
*概**要：独家对话Bill Dally博士，深度解读四大AI热点话题。*
从2017年1月底以来，英伟达的股价又从108美元一路飙升至240多美元，翻了一倍有多。而英伟达股价一飞冲天的背后，正是人工智能技术不断兴盛崛起、发展壮大的缩影。
英伟达成功的原因有很多，及时布局CUDA、积极推进人工智能、不断完善打造软硬件生态等，这些一个个具体业务的背后，核心是对世界科技进展的战略性前瞻与决策；只有及时预判到了下一个技术热点，才能在所有人反应过来之前积极抢占先机，乃至最终引领行业。
这一切的背后，英伟达CEO黄仁勋自然是一个重要决策者；而除了他之外，另一个角色也扮演着至关重要的位置——英伟达首席科学家兼NVIDIA Research高级副总裁，Bill Dally博士。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/z7ZD1WagSLiaNnz53ia6Ocn7yzCVicictrOzzZSuJbK9Fc4YyTko3EWQCI5RycMR4Pjs0icNu6C1HuC6YCJiaic5kAm6Q/640?wx_fmt=jpeg)
*(英伟达首席科学家兼NVIDIA Research高级副总裁，Bill Dally博士)*
在EmTech China大会结束后，还与Bill Dally博士进行了一场一对一的深度专访，不仅详细解读了英伟达在云与端方面的战略布局，还聊到了目前火热的AI芯片话题——Dally博士认为，目前云端AI芯片巨头地位难以撼动，终端侧的AI芯片是各类初创公司的机会所在。此外，他还提到了英伟达的几个重点关注AI项目、以及中美两国科技力量的相似与差异性等。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWnZQrn7ibibeaNyqqickt22nibicxTicLLOxSN5kPicOzNG7BYe8hJgx1TkSl6Gvw6w9icELzh3dIvwwTicsA/640?wx_fmt=png)
*（智东西记者与Bill Dally博士）*
除了担任英伟达首席科学家、负责把控公司整体技术战略发展外，Dally博士同时还是美国国家工程院院士、美国艺术与科学学院院士、IEEE和ACM的会员；在2009年就任英伟达之前，他还曾担任过MIT、斯坦福大学的教授。
在这众多光环萦绕之下，是一个睿智、幽默、思维极其敏捷、并且语速飞快的科学家——语速飞快是什么概念呢？当天速记的小姐姐拜托我千万要让Dally博士慢点说，因为实在记不住……
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7pFR0j3pib9T80DoiaBMEzBnQicBhcziaGqvTSkT9cntnXNDibhGaHEg4AdA/640?wx_fmt=png)
## **云端芯片格局难以撼动，端智能AI芯片成机会所在**
最近半年以来，人工智能的发展重心逐渐从云端向终端转移，相伴而生的是人工智能芯片产业的全面崛起。已历经数月对人工智能芯片全产业链上下近百间核心企业进行报道，范围不仅涵盖英伟达、谷歌、英特尔等科技巨头，还有众多如寒武纪、地平线、深鉴科技、Graphcore等国内外AI芯片初创。
按照应用场景，AI芯片可以简单地分为用于云端服务器机房等地的云AI芯片，以及用于端智能设备、IoT设备的端AI芯片。
Dally博士认为，现在云端AI芯片发展遇到的最大挑战在于，在提供高性能计算的同时，让芯片保持处理新兴AI模型和算法的灵活性，这些任务需要一个通用的可编程平台和专门的指令来完成。
而在端侧AI芯片发展的过程中，这种计算的灵活性并不那么重要，但终端侧所带来的效率、功耗等制约因素是更大的挑战。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWnZQrn7ibibeaNyqqickt22nibL8JWy1IZic9WM0PEaOcsGlVsBZe5ic9tpMTzufQk7aUT8u3QMq9G9GQg/640?wx_fmt=jpeg)
在云方面，英伟达以Volta系列为首的GPU产品专为云服务机房打造，并且推出了cuDNN，TensorRT等软件服务完善AI生态；目前英伟达的GPU支持如TensorFlow、Caffe等所有主流的深度学习框架。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWnZQrn7ibibeaNyqqickt22nibCz2UpOz9Tm2LUJZ64fDXYNMpPcwrhYKBCnFib5t53QovGVibq9jy7MsA/640?wx_fmt=jpeg)
在端方面，英伟达则推出了16nm的AI芯片XAVIER、自动驾驶平台DRIVE PX，并且开源了DLA深度学习加速器项目，厂商可以免费下载这个专为IoT设备设计的AI架构，自己打造低功耗的AI芯片。
Dally博士说，人工智能的一个重要应用就是落地到各种终端设备上，比如汽车、电器、机器人、甚至是灯泡上，让它们变得智能起来。端智能的大势所趋自然毋庸置疑，但云智能也不会被取代，未来AI的发展趋势将会是云+端共生的系统。
而当谈论到如深鉴科技、Graphcore等这些国内外新兴AI芯片初创企业是否会与英伟达形成竞争关系时，Dally博士坦然表示，“是的，他们是竞争对手，但是竞争是健康的，英伟达正在努力保持其领先地位。”
不过他同时也提到，目前用于云端数据中心的芯片市场已经较为成熟，各类巨头纷纷有着扎实的积累，较难撼动格局。AI芯片初创公司（及AI初创公司）更适宜将重心放在终端应用上，尤其是未来的IoT设备，不仅种类繁多而且数量巨大，是初创企业的机会所在——当然，在这些领域里，如何收集到足够的可用数据成了初创企业所面临的最大挑战之一。因为大公司往往凭借着自身平台积累下大量数据，这些是初创公司所不具备的优势。
人工智能对于很多公司、很多行业都具备颠覆性的力量，大公司如果反应不够及时、又或是做错了某些决定，则很可能在这一波技术浪潮中落后、甚至失败。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7NHvWWGfxjWEk4ybhb9ddw81errHd3N5glhaSicHibO85fKNFGvEdxJ1w/640?wx_fmt=png)
## **三大重点关注AI项目：自动驾驶、新一代芯片、神经网络简化**
作为首席科学家，Dally博士与英伟达公司中的众多项目都有不同程度的参与。当询问到最近最关注的是哪几个项目时，Dally博士给出了几个答案：
1）自动驾驶平台
2）新一代深度学习加速器
3）神经网络的压缩与简化
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWnZQrn7ibibeaNyqqickt22nibbqib8MR4SaZqTjdIVSzr6Sgfu0hn7Wbvs1362MFpq7251piathC5A26Q/640?wx_fmt=png)
关于自动驾驶，Dally博士用了一个巧妙的比喻——“自动驾驶其实就像一场游戏，但与AlphaGO这种一输一赢的游戏不同，在这个游戏里你要保证所有车都不能输，但与此同时你也要和别人竞争，谁最快到达终点。”
与此同时，机器还要像德扑游戏一样学会“察言观色”，准确地给对面的玩家（司机）分类，这个人究竟是激进的还是保守的？是刻薄的还是友好的？如果我超车的话他是会退让还是不让？等等。
至于新一代深度学习加速器项目，则包括了Dally博士及团队为人工智能算法处理所设计的一个新型架构，这一项目目前进展非常好，但是涉及新一代产品的发布，不方便透露更多。
神经网络的压缩与简化则是一个学术界与工程界都在研究讨论的重要问题。目前的深度神经网络普遍较大，无论是在云端还是在终端，都会影响网络速度，增大功耗。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWnZQrn7ibibeaNyqqickt22nibYwYrBUxPzXZ18Bt1VFdm70KybJDyGRoOHo94rPcP1TQ4NdoSy0uehQ/640?wx_fmt=png)
在演讲与采访中Dally博士都提到了这一领域的两大研究方向：一个是降低计算精度（比如从32比特到8比特），另一个则是剪枝（Purne）先构造好整个算法网络，然后再尝试消除多余的节点，压缩网络大小。
此外，Dally博士还提到，英伟达的机器人团队前阵子开始了一个机器人协作工作室，在一个厨房大小的空间里，让机器人和人类协作，目标是最后成功地做出一顿饭来。
不过，目前机器要达到人类希望的标准还有很长的路要走。很多对人类来说很简单的动作——比如打开抽屉——对于机器来说有困难，因为他们往往缺乏常识判断，当面前有个水瓶时，它们不会像人类一样下意识地绕开这个瓶子。
除了自动驾驶、机器人、深度学习这类业务外，对于一些吸引人注意的新兴项目，Dally博士也会带领着一小队英伟达科学家组成“特工小组”来研究这个问题，比如前阵子他们就组了一个专攻量子计算的科学家小分队，一群科学家花了六个月时间读论文、解方程、拜访业内人士，力求全面了解这一领域。
研究得出的最后结论是量子计算离我们还有比较长的路要走，至少接下来10年内不会大规模应用，现在机器的量子比特太不稳定且噪声太大。智东西也在不久前的（量子计算机有多可怕 一秒破译全世界所有密码！）一文中介绍了目前世界范围内量子计算的发展脚步。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7w2M9yjpArMeUKpITwBZXMbI4Qw6icuooFF0Bkq9ktyQzcdHf9hwcgxg/640?wx_fmt=png)
## **中美两国的AI芯片初创企业非常相似**
正如上文提到，目前国内外的AI芯片领域正在逐渐升温，众多AI芯片初创企业开始陆续涌现——这些国内外不同企业的创始人们有很多都是Dally博士或是其好友的学生，与他都保持着良好的联系，这次来中国Dally博士也去了不少企业与学校演讲参观。
对于中美AI及AI芯片初创企业，Dally博士认为他们有着非常高的相似之处，一间硅谷创业公司与一间北京创业公司很可能在用着相似的方法在解决同一个问题。而这一情况在中美各个科技巨头上也是如此。
Dally博士说，他第一次来中国是2000年的时候，那时中美间的科技力量差异还是较大。但是随着这十几年的发展，在人工智能这些前沿科技上，中美之间的技术差异已经非常之小。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWnZQrn7ibibeaNyqqickt22nib9YyOz5LpxzuNzYjxZEcQEmCThGUnmRrvCFPDibTjlibCwXiaT5GnXpQeg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7LzPRgPuKzUXaU1wEsibPVq4fADWG94UP8o92sdWwlcPnbqyEcQozTWQ/640?wx_fmt=png)
## **学术领域与工业领域研究的三大区别**
此外，Dally博士除了身为英伟达首席科学家外，还身任美国国家工程院院士、美国艺术与科学学院院士、斯坦佛大学的教授等学术职位。在智东西问到学术研究与工业领域研究最大的区别有哪几点时，Dally博士给出了三个答案：研究领域、研究深度、以及资源。
在行业中，研究人员通常要将研究的领域限制在对公司有利的领域，而较少探究对公司“无用”的科技，比如宇宙起源。不过对于Dally博士来说，他感兴趣的研究领域正好与英伟达密切相关，这其实也是一开始Dally博士选中英伟达，而没有接受来自谷歌等其他科技巨头的offer的重要原因之一。
其次，在学术研究领域，很多学者研究一个东西的最终目的是发表论文，可论文即使发表了，这项技术也不一定能应用在实际，因为很多问题其实都没有得到解决。而在行业中，研究人员需要跟进和解决这些问题，使技术成熟到足以落地应用。
第三则是资源配置，在美国，至少30多年来，学术研究经费一直在不断下降；而科技公司们拥有更好的资源——人才、计算机、仪器等。
不过，AI人才招募是一个如今产业界都在头疼事情，英伟达也不例外。对于产业界来说，吸引这些AI人才的绝不仅仅的薪资待遇，有时更重要的是为他们提供一个良好的研究环境，并且保证软硬件研究资源的齐全，让他们能顺利地研究并解决问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7ZZymRoPAWGyVfpJu9uwA0IyRfbF0F8jic0ibYEBR3PVlA17tMpaiacpsg/640?wx_fmt=png)
## **结语：人工智能竞争进入白热化，2018拼市场**
在人工智能领域，英伟达绝不仅仅是一个提供芯片的硬件玩家。在过去的十年间，以GPU为代表的硬件为深度学习提供了海量的计算能力，随后英伟达又通过cuDNN、TensorRT等一些列AI软件完善生态，进而推动了这一波人工智能热潮的全面兴起——无论是英伟达的股价还是市场声量都体现了这一点。
对于中国而言，人工智能更是缩短中西科技差异的罕有机会。除了国家积极推出响应政策外，各类市场、资本、初创、巨头也纷纷应声而起，占据各类AI应用赛道——这其中，AI芯片又扮演了一个人工智能“从软到硬”落地的重要环节。
随着2018年的开始，人工智能的竞争进入白热化阶段，各大公司已经从一开始的拼技术、拼履历、拼大牛，陆续进入到拼行业、拼落地的实干阶段了。初创公司们的第一批人工智能芯片也将在今年面世，人工智能将进入市场验证阶段，抢占市场成了2018年AI企业们的关键目标。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
