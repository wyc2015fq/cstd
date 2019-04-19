# 《Team Geek》前言（中文，自己翻译的） - SHINING的博客 - CSDN博客
2012年10月02日 00:34:03[snsn1984](https://me.csdn.net/snsn1984)阅读数：4288
Introduction
前言
“Engineeringis easy. People are hard.”
——BillCoughran, former senior vice
presidentof engineering at Google
“做工程容易，做人难。”
——BillCoughran，谷歌工程前高级副总裁
Life isfull of unexpected twists, and the two of us never imagined
we’dsomeday write a book about software engineering.
人生充满了意想不到的纠结，我们两个人从来没有想到有一天我们会写一本关于软件工程的书。
Like most computer geeks, we discovered that our hobby and
passion—playingwith computers—was a great way to make a living
aftergraduating college. And like most hackers of our generation,
wespent the mid-1990s building PCs out of spare parts, installing
prereleaseversions of Linux from piles of diskettes, and learning to
administerUnix machines. We worked as sysadmins, and then at
thedawn of the dot-com bubble, became programmers in smaller
companies.After the bubble burst, we started working for surviving
SiliconValley companies (such as Apple) and later were hired by a
startup(CollabNet) to work full time on designing and writing an
opensource version control application called Subversion.
我们像大多数计算机极客一样，大学毕业之后发现我们的爱好和激情（玩计算机）是一种很好的谋生方式。我们也和我们那一代的大多数骇客一样，在20世纪90年代中期，用一些多余的部件组装个人计算机，用一堆磁盘安装Linux的预发布版本，学习如何管理Unix机器。我们做过系统管理员，在互联网泡沫的前夜我们成为了一家小公司的程序员。互联网泡沫破裂之后，迫于生计我们开始为活下来的硅谷公司（比如苹果）工作，后来我们被一家新创办的公司（CollabNet公司）全职雇佣去设计和开发一个叫 Subversion的开源版本控制软件。
Butsomething unexpected happened between 2000 and 2005.
Whilewe were creating Subversion, our job responsibilities slowly
changed.We weren’t just writing code all day in a vacuum; we
wereleading an open source project. This meant hanging in a
chatroom all day with a dozen other volunteer programmers and
payingattention to what they were doing. It meant coordinating
newfeatures almost entirely through an email list. Along the way,
wediscovered that the key to a project’s success wasn’t just writing
greatcode: the way in which people collaborated toward the end
goalmattered just as much.
但是2000年到2005年期间，发生了一些预想不到的事情。当我们在开发 Subversion的过程中，我们的工作职责在慢慢的变化。我们不再两耳不闻窗外事的整天写代码，我们开始带一个开源项目。这意味着我们要整天和一堆程序员志愿者泡在聊天室里，关注他们都正在做什么。这意味着要协调一个新功能，完全要依靠电子邮件列表。在这个过程中，我们发现，一个项目成功的关键不只是写出伟大的代码。在朝着最终目标努力的过程中，人与人之间的合作方式也事关重大。
In 2005 we started Google’s Chicago engineering office and
continuedour careers as programmers. At this point we were already
deeplyinvolved with the open source world—not just Subversion,
but theApache Software Foundation (ASF) too. We ported
Subversion to Google’s BigTable infrastructure and launched an
opensource project hosting service (similar to SourceForge) under
thebanner of Google Code. We began attending—then speaking
at—developer-centricconferences such as OSCON, ApacheCon,
PyCon, and eventually Google I/O. We discovered that by working
in bothcorporations and open source projects we had accidentally
pickedup a trove of wisdom and anecdotes about how real software
engineeringteams work. What began as a series of humorous talks
aboutdysfunctional development processes (“Subversion Worst
Practices”)eventually turned into talks about protecting teams from
jerks(“How Open Source Projects Survive Poisonous People”).
Larger and larger crowds gathered at our presentations in what
canonly be described as “group therapy” for software developers.
Everyonecould relate to the sorts of problems we talked about and
wantedto gripe about these problems as a group.
2005年，我们成立了谷歌芝加哥工程办公室继续我们的程序员生涯。这时候我们已经很深的介入了开源的世界，不只是 Subversion项目，还有Apache软件基金会（ASF，ApacheSoftware Foundation）。我们把Subversion移植到谷歌的 BigTable基础架构之上，在谷歌代码旗下开启了一个托管服务（类似SourceForge）的开源项目。我们开始参加开发者中心的一些会议，比如：OSCON、ApacheCon、PyCon，甚至是GoogleI/O，我们还在这些会议上做了发言。
 在公司和开源项目的工作经历，使我们收集了一系列关于一个真正的软件工程团队如何工作的聪明想法和事例。这些材料开始于一系列的关于功能失调的开发过程（Subversion最差的实践），到了最后却变成了怎么保护团队远离笨蛋（开源项目如何在别有用心的人的干扰下存活）。大批的人群拥挤在我们的演示前面，这只能被称为是软件开发人员的集体疗伤。每个人都可能遇到我们谈到的那些各种各样的问题，每个人都想把这些问题一并抓住。
And sohere we are, six years later, with a pile of standing-room-
onlytalks about the social challenges of software development. Our
editorat O’Reilly Media, Mary Treseler, pointed out that we should
convertthese talks into a new book. The rest is history.
六年之后，我们挤在这里谈论软件开发的社会挑战。我们的 O’Reilly媒体的编辑 MaryTreseler指出我们应该将这些会谈写成一本书。上面的这些内容，就是这本书的历史。
Tryingto write great software? This book is for you.
想开发出伟大的软件吗？这本书就是为你量身订做的。
Who IsThis Book For?
这本书适合什么样的读者？
Thisbook is squarely written for software developers—for those
who aretrying to advance their careers and ship great software.
It’snot particularly aimed at CEOs, psychologists, “management,”
computerscience theoreticians, or people soldering breadboards
(thoughthose folks may enjoy it too). As our reader, we’re assuming
twoimportant things about you:
•   You work on a team with other programmers.Either you work
in acorporate environment, or perhaps you’re part of an open
sourceor school project.
•   You enjoy software engineering and believeit should be a
rewardingand fun activity. If you’re only turning 1s into 0s and
0s into1s in order to pay off the debt collector, you probably
aren’tinterested in self-actualization or career fulfillment.
这本书适合努力发展他们的事业并试图开发出伟大的软件的开发者。这本书不适合首席技术官、心理学家、“管理人员”、计算机理论科学家或者电路板焊接人员（或许他们很喜欢这本书）。作为我们的读者，我们假定你具有以下两个重要特征：
•   你和其他程序员在一个团队里一起工作。不管你是在一个公司，还是在一个开源项目或者学校项目之中。
•   你喜欢软件工程，并且坚信它是一个既有趣又有益的一个活动。如果你只是为了还清债务而把1转化为0、把0转化为1，你应该不会对自我实现或者职业发展有兴趣。
In theprocess of discussing how engineers best “play well
withothers,” we end up touching on a number of subjects that
(superficially)may seem to be out of a programmer’s job description.
Atdifferent points we discuss how to lead a team effectively,
navigatean organization, and build a healthy relationship with
theusers of your software. On the surface these chapters may
seemspecifically directed toward “people managers” or “product
managers”—butwe assure you that at some point in your software
engineeringcareer you’ll find yourself accidentally wearing those
hats.Suspend your disbelief and keep reading! Everything in this
book isultimately relevant to software engineers.
在探讨软件工程师怎么样和他人进行很好的合作的过程中，我们最终遇到了一些看起来已经超出了一个程序员的工作范畴之外的议题。在不同的阶段，我们探讨怎么样更好的带领一个团队，怎么样削减一个组织，怎么样和软件使用者建立一个良好的关系。表面上这些章节似乎是和“人力管理”或“产品管理”直接相关的，但是我们可以肯定在你的软件工程师生涯的某个阶段，你会用到这些知识。把怀疑放到一边，继续阅读吧！这本书的所有内容最终都是和软件工程师相关的。
Warning:This Is Not a Technical Manual
警告：这不是一本技术手册
Beforewe start, we need to set your expectations. Motivated
programmerslove to read books that lay out domain-specific
problemsin a perfect mathematical presentation; each problem is
typicallypaired with a prescribed procedural solution.
开始之前，我们需要修正下你对这本书的预期。目的性明确的程序员喜欢读那些针对特定领域的问题有完美数学解决方案的书，这样的书里面的每个问题都搭配一个规定的解决方案。
This isnot such a book.
这本书不是那样的书。
