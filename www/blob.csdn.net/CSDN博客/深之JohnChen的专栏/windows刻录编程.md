# windows刻录编程 - 深之JohnChen的专栏 - CSDN博客

2012年01月31日 09:41:53[byxdaz](https://me.csdn.net/byxdaz)阅读数：5053


**一，使用 Windows XP 内置的刻录功能**

参考
[http://msdn.microsoft.com/msdnmag/issues/04/04/CQA/](http://msdn.microsoft.com/msdnmag/issues/04/04/CQA/)

1，简单文件刻录

**使用 Shell 的 ICDBurn 接口**，其中方法：
        HasRecordableDrive 扫描系统，如果发现可写的CD驱动器则返回TRUE。
        GetRecorderDriveLetter 返回可写驱动器。
        Burn 执行刻录动作，从集结区（staging area）写入 CD-R。

    其中，在执行刻录之前必须先把要刻录的文件复制到集结区，集结区是一个临时目录，位于用户目录 "%USERPROFILE%\Local Settings\Application Data\Microsoft\CD Burning" ，也可以通过下面的Shell API获取：
    SHGetFolderPath(CSIDL_CDBURN_AREA)

    注意这里的方法只能用于单个刻录光驱，如果你有多个刻录光驱就不行了。

2，音乐CD、CD-RW 等更复杂的刻录

    这里**使用的是 IMAPI （映像控制 Image Mastering API），**也是 Windows XP 以上版本支持。IMAPI 可用于查询刻录机、写入数据以及一些叫不出名字的先进功能。
    创建 MSDiscMasterObj 对象，该对象有如下接口：
        -o IDiscMaster
        -o IRedbookDiscMaster
        -o IJolietDiscMaster
    稍为解释一下：CD 一般分为两种：音频CD和数据CD，前者遵循红皮书规范，后者则为ISO-9660规范 （也有其它一些数据规范，Joliet是ISO-9660的扩展。）

    首先获得 IDiscMaster 接口，调用 EnumDiscMasterFormats 列举驱动器支持的媒体格式 （Redbook, Joliet等），根据格式获取 IRedbookDiscMaster 等对应接口。如果要求更高可以调用 Get/SetActiveDiscMasterFormat 以动态更新。

    然后调用 EnumDiscRecorders 列出所有刻录设备，对应 IDiscRecorder 接口，该接口提供诸如设备路径、属性、弹出、擦除 CD-RW 等功能，但不包括写入数据。

    具体的写入数据根据不同格式而不同，对于数据CD，即Joliet 格式，首先调用 AddData(IStorage*) 方法加入文件、目录等，而对于音频CD，即红皮书格式则首先调用 AddAudioTrackBlocks 加入音轨。这些方法都是将数据拷贝到集结区，而不是立即写入。
    当数据都准备好了，最后调用 IDiscMaster 的 RecordDisc 方法就可以了。这个方法有一个 bSimulate 的参数用于“模拟写入”，不过你可能永远不会去用它。

    关于 IMAPI 就介绍到这，它虽然比较简单，不过靠着 Windows XP 这棵大树，对于VB程序员也差不多足够了。

**二，MCDB - Magic CD/DVD Burner (C/C++ Library)**

MCDB 是 Binary Magic 开发的商用 CD/DVD 刻录组件，易用性比较好，有 VCL 和 ActiveX 两大类别，Audio/CD/DVD/VCD 等不同媒介的几个版本，可用于 Delphi, C++ Builder, C#, VB.net, VB, C++ 等开发语言。MCDB支持UDF接口，Unicode字符集，以及多种刻录机接口，可非常容易的制作MPEG VCD、可启动光盘等，**其核心使用ASPI接口(高级SCSI编程接口，也可用于访问IDE接口)。**

编写 MCDB 应用程序非常简单，基本类似于 IMAPI。
首先创建 MCDBurner 对象，属性 DeviceCount 和 DeviceName[] 查得刻录机列表。
通过调用 InsertFile/InsertDir 将如要写入的文件和目录，然后执行写入，有几种不同的写入方法：
    BuildISOImage(filename)  写入ISO映像文件
    BurnISO(isofile)  将 ISO映像文件写入光盘
    Prepare/Burn  直接写入光盘 (也就是著名的 on-the-fly 写盘技术)
其它常用的方法有：
    EraseDisc  擦除 CD-RW 盘片
    DiscIs  判断媒体类型
    MaxWriteSpeed/WriteSpeed  写入速度
    LoadMedium/EjectMedium  装入和弹出光盘

MCDB 的 120M 试用版可在 [www.binarymagics.com](http://www.binarymagics.com/) 上下载，零售价为200-400美元不等，同时有源码出售，价格2000美元左右。

**三，hpCDE**

hpCDE 是 Primo Software 开发的一款高性能CD/DVD刻录组件，目前版本1.78，从特性列表中可以看到它对硬件的支持非常多，在写入中支持 DVD DAO/Incremental、TAO, SAO, DAO, RAW, CD-PLUS 等多种模式，支持 UDF和HFS文件系统，可用于刻录供Mac OS使用的光盘，以及支持飞盘保护如JustLink, SafeBurn, SmartBurn等高级特性。

相较于前面两个，hpCDE明显复杂，没办法~ 强大的东西总是复杂的。

先从CD复制开始，工厂类 hpCDE 提供了其它类的构建方法，大致过程如下：

    初始化：
    hpCDE::CreateEngine     获得引擎对象
    IEngine::Initialize     初始化引擎，其中win 95/98必须使用ASPI，而NT则不用。
    IEngine::GetDevices     获得驱动器列表
        - IDeviceCollection::GetItem    枚举驱动器
        - IDevice::GetWritePossible     判断是否可写，并忽略不可写设备
        - IDevice::GetDriveLetter       查询盘符
        - IDevice::GetDescription       查询卷标
        - IDevice::GetMediaFreeSpace    判断是否盘片是否可用
        - IDevice::GetMediaIsBlank      判断是否为空盘
        - IDevice::GetMaxWriteSpeed     最高写入速度
        - IDevice::GetReWritePossible   刻录机是否支持擦除
        - IDevice::GetMediaIsReWritable 盘片是否可擦除

如果希望CD复制并行进行，因此需要建立两个线程分别用于读取/写入，hpCDE 支持多线程模型。hpCDE 支持SAO/TAO/DAO，这里CD复制使用的是 DAO。对于多段写入，每个段对应于一个ISession，一个Session可以包含多个Track。

    写入过程：
    IDevice::SetCurrentSpeeds   设置写入速度
    IDevice::Eject              弹出盘片

参考
[http://www.hpcde.com/documentation/index.html](http://www.hpcde.com/documentation/index.html)

**四，cdrdao, cdrtools**

参考：
[http://cdrdao.sourceforge.net/](http://cdrdao.sourceforge.net/)
[http://freshmeat.net/projects/cdrecord/](http://freshmeat.net/projects/cdrecord/)
[http://linuxcommand.org/man_pages/cdrecord1.html](http://linuxcommand.org/man_pages/cdrecord1.html)

cdrdao 是一款开源 DAO 模式刻录软件，有 linux 和 windows 两个版本。

cdrdao 仅支持一种模式 DAO 即一次完成写盘，这种模式可以实现一些 pre-gaps 的有趣特性，对于光盘防拷贝，则可以实现 ECC 检查的技巧。因为是专门为 DAO 设计，不是专业人士恐怕无法阅读其源代码。

cdrtools 也是开源的，不过它是全功能的，包括IDE/ATAPI/SCSI, 音频/数据/混合CD，多段刻录，CD-RW，TAO/DAO/RAW 等等，同时支持很多操作系统。

