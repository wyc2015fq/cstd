# 从冯诺伊曼结构看AI - 我相信...... - CSDN博客





2017年09月17日 22:28:30[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：640标签：[机器学习																[人工智能																[大数据																[ai](https://so.csdn.net/so/search/s.do?q=ai&t=blog)
个人分类：[大数据																[云计算](https://blog.csdn.net/wireless_com/article/category/1292302)](https://blog.csdn.net/wireless_com/article/category/6231064)

所属专栏：[大数据修炼道场](https://blog.csdn.net/column/details/12856.html)





AI, Artificial Intelligence， 人工智能。

就像每个人眼中都有一个自己的哈姆雷特一样，每一个看AI 都是不一样的。作为一个老程序员，也只是一个工作时间长一些的程序员而已，本没有什么资格定义AI，但是面对问题， 还是强作镇定，从一个工程师角度阐述一下，“什么是AI？”以及AI 和 大数据，机器学习，神经网络，自然语言处理等诸多名词到底有什么关系呢？

## 什么是AI？

AI，来自于维基百科的解释是这样的：

> 
Artificial intelligence is intelligence exhibited by machines, rather than humans or other animals . In computer science, the field of AI research defines itself as the study of “intelligent agents”: any device that perceives its environment and takes actions that maximize its chance of success at some goal. Colloquially, the term “artificial intelligence” is applied when a machine mimics “cognitive” functions that humans associate with other human minds, such as “learning” and “problem solving”. —- wikipedia


人工智能中的智能如何定义呢？如何解释智能呢？或许， 用图灵测试来理解人工智能更加方便。1950年， 阿兰图灵提出了图灵测试：如果一台机器能够与人类展开对话（通过电传设备）而不能被辨别出其机器身份，那么称这台机器具有智能。具体地，指测试者与被测试者（一个人和一台机器）隔开的情况下，通过一些装置（如键盘）向被测试者随意提问。进行多次测试后，如果有超过30%的测试者不能确定出被测试者是人还是机器，那么这台机器就通过了测试，并被认为具有人类智能。 
![这里写图片描述](https://img-blog.csdn.net/20170917222129746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因此，人工智能就是让机器具有了和人难以区分的能力，简单的说， AI 是机器所提供的一些能力，这些能力与人的能力类似甚至更高。

## AI 的 结构

AI 的载体是机器，在计算机无所不在的今天（作为一个通信专业的学生，大学读书的时候，老师说过“通信技术计算机”），在计算机网络无所不在的今天（网络对生活的影响巨大，以至于有了“互联网+”等概念），AI 的载体离不开计算机和网络。

谈到计算机的体系结构，就离不开冯诺伊曼体系结构。冯·诺依曼提出的计算机体系结构，奠定了现代计算机的结构理念，即计算机由控制器、运算器、存储器、输入设备、输出设备五部分组成。

![这里写图片描述](https://img-blog.csdn.net/20170917222200977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

冯·诺依曼理论的要点是：数字计算机的数制采用二进制；计算机应该按照程序顺序执行。根据冯·诺依曼体系结构构成的计算机，必须具有如下功能：把需要的程序和数据送至计算机中。必须具有长期记忆程序、数据、中间结果及最终运算结果的能力。能够完成各种算术、逻辑运算和数据传送等数据加工处理的能力。能够根据需要控制程序走向，并能根据指令控制机器的各部件协调操作。能够按照要求将处理结果输出给用户。

冯诺伊曼体系结构真是一个神奇的结构，我理解是自洽的，局部和整体的结构是一致的。小到代码中的一个函数，大到一个计算机乃至整个软件系统都可以用这样一个结构来描述。而网络的存在是将结构中的模块作为功能团在空间上的拉伸， 就是它们可以不在同一个物理空间的点上。

AI 在物理上是基于计算机和网络的一种体系结构。

## AI 的 能力

AI 有了计算机和网络的身体骨骼，表现出来的种种能力。

我们自己面对输入的能力有视觉、听觉、嗅觉、味觉和触觉五种基本能力，相应地，AI 也具有这样的能力：视觉——图像的检测与识别，视频分析等等，人脸识别和指纹识别都是AI视觉能力的一种而已；听觉——声音的检测与识别，超声分析等，语音识别只是AI听觉能力的一种而已，嗅觉、味觉、触觉都可能是具有传感器的，AI通过传感器可以具备这样的能力。

对于输出而言，我们可以是语言，动作，文字等， 相应地，AI也具有声音合成，图像合成，文字合成等输出能力。

存储、计算和控制都在我们的大脑中，相应地，AI 同样可以具有存储、计算和控制的能力，甚至可能比人做的更好。业界有很多“智脑”的项目，以及基于AI的存储、计算和控制能力的应用，例如大名鼎鼎的alpha狗。

因此， AI 基于计算机和网络的能力大概是这样的：

![这里写图片描述](https://img-blog.csdn.net/20170917222619487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## AI 的 技术

AI 是如何具备这些能力的呢？ 这才涉及到当今热门的种种技术。

严格来讲，人工智能和机器学习没有直接关系，只不过目前机器学习的方法被大量的应用于解决人工智能的问题而已。目前机器学习是人工智能的一种实现方式，也是最重要的实现方式之一，例如提供计算机视觉的能力等等。

深度学习是一种实现机器学习的技术，源于人工神经网络的研究。

机器学习都是基于数据的，而所谓的“大数据”技术为机器学习乃至深度学习提供了源泉。

他们之间的关系大致是这样的：

![这里写图片描述](https://img-blog.csdn.net/20170917222635797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

AI 中所采用的各种技术都是为解决某些问题，进而提供或者提高某些能力。

总的来说， 一个老程序员眼中的AI 大致是这样的：

![这里写图片描述](https://img-blog.csdn.net/20170917222653369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=人工智能&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)




