# 谷歌Android篡改Linux内核的真相 - 袁萌专栏 - CSDN博客
2011年03月20日 17:19:00[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：3090

	
	
	
	p { margin-bottom: 0.21cm; }
    3月18日，《Brown
Rudnick》律师事务所知识产权律师Edward
Naughton发表一份研究报告，题为“THE
BIONIC LIBRARY: DID GOOGLE WORK AROUND THE GPL?
（PDF
10）”，揭露了谷歌Android篡改Linux内核的事实真相，值得我们大家一读。具体情况是怎样的呢？
    什么是Android？实际上，谷歌的Android只是Linux内核的一个“子集合”，内核的许多功能组件，比如：Cairo、X11、Alsa、FFmpeq、GTK、Pango以及Glibc都被谷歌蓄意篡改或删除，又以Bionic取代Glibc、以Skia取代Cairo、再以opencore取代FFmpeg
，等等，而且，谷歌将Android的硬件驱动程序统统移到userspace，使得Linux
driver与Linux
kernel两者彻底分开，结果，谷歌把Android搞得不伦不类，成了Linux内核的一个怪异“变种”（称为“怪胎”更为确切）。
    大家知道，Linux内核中的Glibc包含了许多极为重要的标准库，比如：标准C函数库、标准数学函数库等，许多应用程序都必须借助Glibc库的支持，否则，系统就要停摆。但是，谷歌为了商业目的，擅自（想当然地）对Glibc大动“手术”，篡改了750个头文件，抹去其中的GPL版权信息（文字、符号、空格），搞了一个谷歌自己发明的“Bionic“文件取而代之，故意让Bionic遵守BSD授权规则，从而使得Android系统不必再遵守Linux内核的GPL规则。由此，谷歌“瞒天过海”，玩弄权术，把全球数十万Android开发大军全套进去了。
     那么，我们要问，Linux的关键文件Glibc到底能不能随意改动呢？早在2003年，Linus
Torvalds就说过一句话：the
GPL licence "very much does NOT say that you can 'use the header
files in any way you want, including building non-GPL'd programs with
them'"，似乎早就看穿了谷歌的这套鬼把戏。当前，谷歌肆意篡改Glibc的手法已经被业界法律人士彻底揭露出来，......说句大实话，已经不好收拾，弄得不好，只会帮倒忙。
今年2月22日，美国法院驳回了谷歌要求法院终止审理甲骨文的诉状，由此可见，谷歌Android侵权（指侵犯甲骨文的Java版权）事实几乎已成为“定局”，请见：Judge
denies Google's bid to end Oracle
case(2011年2月22日)。现在，谷歌篡改Linux内核的事实真相也已经大白于天下，还有什么话好说？
    说明：有人说，我批评谷歌的Android侵权，表明我是惠普的枪手。此言差矣！我敢说，惠普的WebOS绝对不像谷歌那样肆意篡改Linux文件，别出心裁，耍弄数十万Android开发者。总之一句话，谷歌贪婪地追求商业目的是其挺而走险的唯一“动机”，谷歌“不作恶”只是一个幌子罢了。君子爱财，取之有道。我们要问，谷歌的“道”究竟在哪里？......原谷歌(中国)CEO李开复迷惑了不少中国的年轻人，编造了不少谷歌的神话，其流毒至今没有完全肃清。
