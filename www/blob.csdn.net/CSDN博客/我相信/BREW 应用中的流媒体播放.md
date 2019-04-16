# BREW 应用中的流媒体播放 - 我相信...... - CSDN博客





2010年10月20日 10:35:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：931








流式播放的数据源来自ISource 接口对象，可以来自于网络，内存或者文件。流式媒体有两种内容：一种是格式化的媒体，该媒体包含了头，编码规范和原始数据的起始位置，例如.mp3 或者 .wav 的文件；另一种是原始数据，其编解码方法由使用者单独提供。流式播放需要一个ISource的具体实现，由应用创建 ISource 接口, 并保持在IMedia 接口的整个生命周期内有效。下面是一个简单的例子，对一个wav文件实现流式播放。

static void MyApp_SetupSource(MyApp * pme){

AEEMediaDataEx md;

IFileMgr *pfm; ISourceUtil *psu;

// 步骤#1: 创建IMedia PCM 对象，处于 IDLE 状态

ISHELL_CreateInstance(pme->e.m_pIShell,AEECLSID_MEDIAPCM,(void**)&pme->m_pIMedia);

// 步骤#2: 创建具体ISource对象

ISHELL_CreateInstance(pme->e.m_pIShell, AEECLSID_FILEMGR, (void **)&pfm))

pme->m_pFile = IFILEMGR_OpenFile(pfm, "sample.wav", _OFM_READ);

IFILEMGR_Release(pfm);

ISHELL_CreateInstance(pme->e.m_pIShell, AEECLSID_SOURCEUTIL, (void **)&psu))

ISOURCEUTIL_SourceFromAStream(psu, (IAStream*)pme->m_pFile, &pme->m_pISource);

ISOURCEUTIL_Release(psu);

// 步骤 #3: 以ISource 初始化AEEMediaDataEx

md.clsData = MMD_ISOURCE;

md.pData = (void *)pme->m_pISource;

md.dwSize = 0;

md.dwStructSize = sizeof(md); // AEEMediaDataEx 数据结构的大小

md.dwCaps = 0;.

md.bRaw = FALSE; // 是否是原始数据? FALSE代表不是

md.dwBufferSize = 0; // 内部的缓存大小， 0 代表使用默认值.

md.pSpec = NULL; // 只对原始数据格式有限

md.dwSpecSize = 0; //只对原始数据格式有限

// 步骤#4: 设置媒体数据，IMedia 对象处于 READY状态

IMEDIA_SetMediaDataEx(pme->m_pIMedia, &md, 1);

}

对于原始数据的流式播放，由于没有媒体播放的终止符，需要在播放中准确的调用IMEDIA_Stop()。在AEEMediaDataEx 数据结构中，需要将bRaw 设为TRUE，将pSpec 设为指定的编解码方法。

流媒体播放就是数据源来自网络的流式播放，采用流式传输的方式,不需要用户将多媒体数据全部下载,而是采取边下载边播放的方式, 仅仅将最初的一些数据先下载到本地缓冲区,只要数据积累到可以连续播放的要求后就开始播放,后面的数据会根据请求不断进入本地缓冲区,从而使播放片断形成一个完整的数据流，如最常用的网络电视PPLIVE 就是采用这种技术。由于无线网络的限制,移动流媒体一般采用单播的播放方式,每个接收端与流媒体服务器建立起一对一的连接关系,每个用户单独向服务器发出数据请求,并由服务器向该用户发送单独的数据拷贝。

       由于目前的API不支持对H. 264 或者MPEG- 4/ H. 263 格式的接口,因此需要移植相应的解码器到BREW平台上。移植主要使用BREW的接口来替代解码器中的C 语言函数,并用整数计算或定点计算代替浮点运算,尤其需要解决的是H.264 和Xvid 参考源程序所使用的栈空间超过BREW手机的限制问题(如将数组改为动态分配内存,将全局的数组改为函数域中),最终将视频帧解码为位图显示在手机屏幕上。手机播放视频流的一个重要问题是解决音视频的同步。服务器端传送的数据包中包含了音频和视频的播放持续时间,这样可以采取以音频播放时间为基准,校正视频播放. 假设ha 是当前音频的播放时间, hv是当前视频帧的播放时间. 如果hv < ha ,表示视频滞后于音频,则丢弃此帧,立刻转向下一帧的解码;如果hv > ha ,表示视频超前于音频,则此帧暂时不显示,等待音频播放hv - ha 的时间后再显示.



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/07/21/5751897.aspx](http://blog.csdn.net/wireless_com/archive/2010/07/21/5751897.aspx)



