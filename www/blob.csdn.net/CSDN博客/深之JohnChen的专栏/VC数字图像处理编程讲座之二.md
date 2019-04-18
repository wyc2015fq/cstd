# VC数字图像处理编程讲座之二 - 深之JohnChen的专栏 - CSDN博客

2005年11月24日 15:28:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：7555标签：[图像处理																[编程																[colors																[存储																[null																[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=colors&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
个人分类：[图像处理与识别](https://blog.csdn.net/byxdaz/article/category/144582)


                                                                            VC数字图像处理编程讲座之二

前 言 

　　数字图像处理技术与理论是计算机应用的一个重要领域，许多工程应用都涉及到图像处理，一直有一个强烈的愿望，想系统的写一个关于数字图像处理的讲座，由于工作学习很忙，时至今日才得以实现。 

　　"图"是物体透射光或反射光的分布，"像"是人的视觉系统对图的接收在大脑中形成的印象或认识。图像是两者的结合。人类获取外界信息是靠听觉、视觉、触觉、嗅觉、味觉等，但绝大部分（约80%左右）来自视觉所接收的图像信息。图像处理就是对图像信息进行加工处理，以满足人的视觉心理和实际应用的需要。简单的说，依靠计算机对图像进行各种目的的处理我们就称之为数字图像处理。早期的数字图像处理的目的是以人为对象，为了满足人的视觉效果而改善图像的质量，处理过程中输入的是质量差的图像，输出的是质量好的图像，常用的图像处理方法有图像增强、复原等。随着计算机技术的发展，有一类图像处理是以机器为对象，处理的目的是使机器能够自动识别目标，这称之为图像的识别，因为这其中要牵涉到一些复杂的模式识别的理论，所以我们后续的讲座只讨论其中最基本的内容。由于在许多实际应用的编程中往往都要涉及到数字图像处理，涉及到其中的一些算法，这也是许多编程爱好者感兴趣的一个内容，我们这个讲座就是讨论如何利用微软的Visual C++开发工具来实现一些常用的数字图像处理算法，论述了图像处理的理论，同时给出了VC实现的源代码。本讲座主要的内容分为基础篇、中级篇和高级篇，具体包含的主要内容有： 

　　1． 图像文件的格式； 

　　2． 图像编程的基础-操作调色板； 

　　3． 图像数据的读取、存储和显示、如何获取图像的尺寸等； 

　　4． 利用图像来美化界面； 

　　5． 图像的基本操作：图像移动、图像旋转、图像镜像、图像的缩放、图像的剪切板操作； 

　　6． 图像显示的各种特技效果； 

　　7． 图像的基本处理：图像的二值化、图像的亮度和对比度的调整、图像的边缘增强、如何得到图像的直方图、图像直方图的修正、图像的平滑、图像的锐化等、图像的伪彩色、彩色图像转换为黑白图像、物体边缘的搜索等等； 

　　8． 二值图像的处理：腐蚀、膨胀、细化、距离变换等； 

　　9． 图像分析：直线、圆、特定物体的识别； 

　　10．JEPG、GIF、PCX等格式文件相关操作； 

　　11．图像文件格式的转换； 

　　12．图像的常用变换：付利叶变换、DCT变换、沃尔什变换等； 

　　13．AVI视频流的操作； 

　　图像处理技术博大精深，不仅需要有很强的数学功底，还需要熟练掌握一门计算机语言，在当前流行的语言中，我个人觉的Visual C++这个开发平台是图像开发人员的首选工具。本讲座只是起到抛砖引玉的作用，希望和广大读者共同交流。 VC数字图像处理编程讲座之二 

第一节 图像的文件格式 

　　要利用计算机对数字化图像进行处理，首先要对图像的文件格式要有清楚的认识，因为我们前面说过，自然界的图像以模拟信号的形式存在，在用计算机进行处理以前，首先要数字化，比如摄像头（CCD）摄取的信号在送往计算机处理前，一般情况下要经过数模转换，这个任务常常由图像采集卡完成，它的输出一般为裸图的形式；如果用户想要生成目标图像文件，必须根据文件的格式做相应的处理。随着科技的发展，数码像机、数码摄像机已经进入寻常百姓家，我们可以利用这些设备作为图像处理系统的输入设备来为后续的图像处理提供信息源。无论是什么设备，它总是提供按一定的图像文件格式来提供信息，比较常用的有BMP 格式、JPEG格式、GIF格式等等，所以我们在进行图像处理以前，首先要对图像的格式要有清晰的认识，只有在此基础上才可以进行进一步的开发处理。 

　　在讲述图像文件格式前，先对图像作一个简单的分类。除了最简单的图像外，所有的图像都有颜色，而单色图像则是带有颜色的图像中比较简单的格式，它一般由黑色区域和白色区域组成，可以用一个比特表示一个像素，"1"表示黑色，"0"表示白色，当然也可以倒过来表示，这种图像称之为二值图像。我们也可以用8个比特（一个字节）表示一个像素，相当于把黑和白等分为256个级别，"0"表示为黑，"255"表示为白，该字节的数值表示相应像素值的灰度值或亮度值，数值越接近"0"，对应像素点越黑，相反，则对应像素点越白，此种图像我们一般称之为灰度图像。单色图像和灰度图像又统称为黑白图像，与之对应存在着彩色图像，这种图像要复杂一些，表示图像时，常用的图像彩色模式有RGB模式、CMYK模式和HIS模式，一般情况下我们只使用RGB模式，R 对应红色，G对应绿色，B对应蓝色，它们统称为三基色，这三中色彩的不同搭配，就可以搭配成各种现实中的色彩，此时彩色图像的每一个像素都需要3个样本组成的一组数据表示，其中每个样本用于表示该像素的一个基本颜色。 

　　对于现存的所有的图像文件格式，我们在这里主要介绍BMP图像文件格式，并且文件里的图像数据是未压缩的，因为图像的数字化处理主要是对图像中的各个像素进行相应的处理，而未压缩的BMP图像中的像素数值正好与实际要处理的数字图像相对应，这种格式的文件最合适我们对之进行数字化处理。请读者记住，压缩过的图像是无法直接进行数字化处理的，如JPEG、GIF等格式的文件，此时首先要对图像文件解压缩，这就要涉及到一些比较复杂的压缩算法。后续章节中我们将针对特殊的文件格式如何转换为BMP格式的文件问题作专门的论述，经过转换，我们就可以利用得到的未压缩的BMP文件格式进行后续处理。对于JPEG、GIF等格式，由于涉及到压缩算法，这要求读者掌握一定的信息论方面的知识，如果展开的话，可以写一本书，限于篇幅原因，我们只作一般性的讲解，有兴趣的朋友可以参考相关书籍资料。 

一、BMP文件结构 

　　1. BMP文件组成 

　　BMP文件由文件头、位图信息头、颜色信息和图形数据四部分组成。文件头主要包含文件的大小、文件类型、图像数据偏离文件头的长度等信息；位图信息头包含图象的尺寸信息、图像用几个比特数值来表示一个像素、图像是否压缩、图像所用的颜色数等信息。颜色信息包含图像所用到的颜色表，显示图像时需用到这个颜色表来生成调色板，但如果图像为真彩色，既图像的每个像素用24个比特来表示，文件中就没有这一块信息，也就不需要操作调色板。文件中的数据块表示图像的相应的像素值，需要注意的是：图像的像素值在文件中的存放顺序为从左到右，从下到上，也就是说，在BMP文件中首先存放的是图像的最后一行像素，最后才存储图像的第一行像素，但对与同一行的像素，则是按照先左边后右边的的顺序存储的；另外一个需要读者朋友关注的细节是：文件存储图像的每一行像素值时，如果存储该行像素值所占的字节数为4的倍数，则正常存储，否则，需要在后端补0，凑足4的倍数。 

　　2. BMP文件头 

　　BMP文件头数据结构含有BMP文件的类型、文件大小和位图起始位置等信息。其结构定义如下: 

typedef struct tagBITMAPFILEHEADER 
{ 
WORD bfType; // 位图文件的类型，必须为"BM" 
DWORD bfSize; // 位图文件的大小，以字节为单位 
WORD bfReserved1; // 位图文件保留字，必须为0 
WORD bfReserved2; // 位图文件保留字，必须为0 
DWORD bfOffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位 
} BITMAPFILEHEADER；该结构占据14个字节。 

　　3. 位图信息头 

　　BMP位图信息头数据用于说明位图的尺寸等信息。其结构如下： 

typedef struct tagBITMAPINFOHEADER{ 
DWORD biSize; // 本结构所占用字节数 
LONG biWidth; // 位图的宽度，以像素为单位 
LONG biHeight; // 位图的高度，以像素为单位 
WORD biPlanes; // 目标设备的平面数不清，必须为1 
WORD biBitCount// 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一 
DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一 
DWORD biSizeImage; // 位图的大小，以字节为单位 
LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数 
LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数 
DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数 
DWORD biClrImportant;// 位图显示过程中重要的颜色数 
} BITMAPINFOHEADER；该结构占据40个字节。 

　　注意：对于BMP文件格式，在处理单色图像和真彩色图像的时候，无论图象数据多么庞大，都不对图象数据进行任何压缩处理，一般情况下，如果位图采用压缩格式，那么16色图像采用RLE4压缩算法，256色图像采用RLE8压缩算法。 

　　4. 颜色表 

　　颜色表用于说明位图中的颜色，它有若干个表项，每一个表项是一个RGBQUAD类型的结构，定义一种颜色。RGBQUAD结构的定义如下: 

typedef struct tagRGBQUAD { 
BYTErgbBlue;// 蓝色的亮度(值范围为0-255) 
BYTErgbGreen; // 绿色的亮度(值范围为0-255) 
BYTErgbRed; // 红色的亮度(值范围为0-255) 
BYTErgbReserved;// 保留，必须为0 
} RGBQUAD; 

　　颜色表中RGBQUAD结构数据的个数由BITMAPINFOHEADER 中的biBitCount项来确定，当biBitCount=1,4,8时，分别有2,16,256个颜色表项，当biBitCount=24时，图像为真彩色，图像中每个像素的颜色用三个字节表示，分别对应R、G、B值，图像文件没有颜色表项。位图信息头和颜色表组成位图信息，BITMAPINFO结构定义如下: 

typedef struct tagBITMAPINFO { 
BITMAPINFOHEADER bmiHeader; // 位图信息头 
RGBQUAD bmiColors[1]; // 颜色表 
} BITMAPINFO; 

　　注意：RGBQUAD数据结构中，增加了一个保留字段rgbReserved，它不代表任何颜色，必须取固定的值为"0"，同时， RGBQUAD结构中定义的颜色值中，红色、绿色和蓝色的排列顺序与一般真彩色图像文件的颜色数据排列顺序恰好相反，既：若某个位图中的一个像素点的颜色的描述为"00，00，ff，00"，则表示该点为红色，而不是蓝色。 

　　5. 位图数据 

　　位图数据记录了位图的每一个像素值或该对应像素的颜色表的索引值，图像记录顺序是在扫描行内是从左到右,扫描行之间是从下到上。这种格式我们又称为Bottom_Up位图，当然与之相对的还有Up_Down形式的位图，它的记录顺序是从上到下的，对于这种形式的位图，也不存在压缩形式。位图的一个像素值所占的字节数：当biBitCount=1时，8个像素占1个字节；当biBitCount=4时，2个像素占1个字节；当 biBitCount=8时，1个像素占1个字节；当biBitCount=24时,1个像素占3个字节，此时图像为真彩色图像。当图像不是为真彩色时，图像文件中包含颜色表，位图的数据表示对应像素点在颜色表中相应的索引值，当为真彩色时，每一个像素用三个字节表示图像相应像素点彩色值，每个字节分别对应R、G、B分量的值，这时候图像文件中没有颜色表。上面我已经讲过了，Windows规定图像文件中一个扫描行所占的字节数必须是4的倍数(即以字为单位),不足的以0填充，图像文件中一个扫描行所占的字节数计算方法： 

DataSizePerLine= (biWidth* biBitCount+31)/8；// 一个扫描行所占的字节数 

　　位图数据的大小按下式计算(不压缩情况下)： 

　　DataSize= DataSizePerLine* biHeight。 

　　上述是BMP文件格式的说明，搞清楚了以上的结构，就可以正确的操作图像文件，对它进行读或写操作了。 

二、GIF图像文件格式 

　　GIF图象格式的全称为Graphics Interchange Format，从这个名字可以看出，这种图像格式主要是为了通过网络传输图像而设计的。GIF文件不支持24位真彩色图像，最多只能存储256色的图像或灰度图像；GIF格式文件也无法存储CMY和HIS模型的图像数据；另外，GIF图像文件的各种数据区域一般没有固定的数据长度和存储顺序，所以为了方便程序寻找数据区，将数据区中的第一个字节作为标志符；最后需要读者注意的是GIF文件存储图像数据是有二种排列顺序：顺序排列或交叉排列。交叉排列的方式适合网络传输，这样一来允许用户在不完全掌握图像数据之前，获取当前图像的轮廓数据。 

　　GIF文件格式分为87和89两个版本，对于87这个版本，该文件主要是有五个部分组成，它，们是按顺序出现的：文件头块、逻辑屏幕描述块、可选择的调色板块、图像数据块、最后是标志文件结束的尾块，该块总是取固定的值3BH。其中第一和第二两个块用GIF图像文件头结构描述： 

GIFHEADER：{ 
DB Signature; //该字段占六个字节，为了用于指明图像为GIF格式，前三个字符必须为"GIF"，后三字符用于指定是哪个版本，87或89。 
DW ScreenWidth;// 
DW ScreenDepth;//占两个字节，以像素为单位表示图像的宽、高 
DB GlobalFlagByte;//该字节的各个位用于调色版的描述 
DB BackGroundColor;//代表图象的背景颜色的索引 
DB AspectRatio;图像的长宽比 
} 

　　GIF格式中的调色板有通用调色板和局部调色板之分，因为GIF格式允许一个文件中存储多个图像，因此有这两种调色板，其中通用调色板适于文件中的所有图像，而局部调色板只适用于某一个图像。格式中的数据区域一般分为四个部分，图像数据识别区域，局部调色板数据，采用压缩算法得到的图象数据区域和结束标志区域。 

　　在GIF89版本中，它包含七个部分，分别是文件头、通用调色板数据、图像数据区和四个补充数据区，它们主要是用于提示程序如何处理图像的。 

　　三、JEPG图像文件 

　　JEPG简称为联合摄影专家小组，作为一种技术，主要用于数字化图像的标准编码，JPEG主要采用有损的压缩编码方式，它比GIF、BMP图像文件要复杂的多，这不是短短的几页篇幅可以将清楚的，万幸的是，我们可以通过一些别的方法将该格式转化为BMP格式。读者需要知道的是在对JEPG文件格式编码时，通常需要分为以下四步：颜色转化、DCT变换、量化、编码。 

　　以上介绍了一些常用的图像文件，对比较复杂的格式，如GIF和JEPG，仅仅作了极其浮浅的介绍，后文我们会和它们作进一步的接触。实际应用中，还有许多图像格式，文章中都没有提到，读者如果需要做进一步的研究，还需要参考一些关于图像格式方面的资料。
VC数字图像处理编程讲座之三 

BMP图像的基本操作 

　　上一讲我们主要介绍了图像的格式，其中重点说明了BMP文件的存储格式，同时对JEPG和GIF等常用格式作了简单的介绍。本节主要讲述如何操作BMP文件，如对其读、写和显示等。 

　　在实现数字图象处理的过程中，主要是通过对图像中的每一个像素点运用各种图像处理算法来达到预期的效果，所以进行图像处理的第一步，也是我们最关心的问题，是如何得到图像中每一个像素点的亮度值；为了观察和验证处理的图像效果，另一个需要解决的问题是如何将处理前后的图像正确的显示出来。我们这章内容就是解决这些问题。 

　　随着科技的发展，图像处理技术已经渗透到人类生活的各个领域并得到越来越多的应用，但是突出的一个矛盾是图像的格式也是越来越多，目前图像处理所涉及的主要的图像格式就有很多种，如TIF、JEMP、BMP等等，一般情况下，为了处理简单方便，进行数字图像处理所采用的都是BMP格式的图像文件（有时也称为DIB格式的图像文件），并且这种格式的文件是没有压缩的。我们通过操作这种格式的文件，可以获取正确显示图像所需的调色板信息，图像的尺寸信息，图像中各个像素点的亮度信息等等，有了这些数据，开发人员就可以对图像施加各种处理算法，进行相应的处理。如果特殊情况下需要处理其它某种格式的图像，如GIF、JEMP等格式的图象文件，可以首先将该格式转换为BMP格式，然后再进行相应的处理。这一点需要读者清楚。 

　　BMP格式的图像文件又可以分为许多种类，如真彩色位图、256色位图，采用RLE(游程编码)压缩格式的BMP位图等等。由于在实际的工程应用和图像算法效果验证中经常要处理的是256级并且是没有压缩的BMP灰度图像，例如通过黑白采集卡采集得到的图像就是这种格式，所以我们在整个讲座中范例所处理的文件格式都是BMP灰度图像。如果读者对这种格式的位图能够作到熟练的操作，那么对于其余形式的BMP位图的操作也不会很困难。 

　　BMP灰度图像作为Windows环境下主要的图像格式之一，以其格式简单，适应性强而倍受欢迎。正如我们在上一讲中介绍过的那样，这种文件格式就是每一个像素用8bit表示，显示出来的图像是黑白效果，最黑的像素的灰度（也叫作亮度）值为"0"，最白的像素的灰度值为"255"，整个图像各个像素的灰度值随机的分布在"0"到"255"的区间中，越黑的像素，其灰度值越接近于"0"，越白（既越亮）的像素，其灰度值越接近于"255"；与此对应的是在该文件类型中的颜色表项的各个RGB分量值是相等的，并且颜色表项的数目是256个。 

　　在进行图像处理时，操作图像中的像素值就要得到图像阵列；经过处理后的图像的像素值需要存储起来；显示图像时要正确实现调色板、得到位图的尺寸信息等。结合这些问题，下面我们针对性的给出了操作灰度BMP图像时的部分函数实现代码及注释。 

一、 BMP位图操作 

　　首先我们回顾一下上讲中的重要信息：BMP位图包括位图文件头结构BITMAPFILEHEADER、位图信息头结构 BITMAPINFOHEADER、位图颜色表RGBQUAD和位图像素数据四部分。处理位图时要根据文件的这些结构得到位图文件大小、位图的宽、高、实现调色板、得到位图像素值等等。这里要注意的一点是在BMP位图中，位图的每行像素值要填充到一个四字节边界，即位图每行所占的存储长度为四字节的倍数，不足时将多余位用0填充。 

　　有了上述知识，可以开始编写图像处理的程序了，关于在VC的开发平台上如何开发程序的问题这里不再赘述，笔者假定读者都具有一定的VC开发经验。在开发该图像处理程序的过程中，笔者没有采用面向对象的方法，虽然面向对象的方法可以将数据封装起来，保护类中的数据不受外界的干扰，提高数据的安全性，但是这种安全性是以降低程序的执行效率为代价的，为此，我们充分利用了程序的文档视图结构，在程序中直接使用了一些API函数来操作图像。在微软的 MSDN中有一个名为Diblook的例子，该例子演示了如何操作Dib位图，有兴趣的读者可以参考一下，相信一定会有所收获。 

　　启动Visual C++，生成一个名为Dib的多文档程序，将CDibView类的基类设为CscrollView类，这样作的目的是为了在显示位图时支持滚动条，另外在处理图像应用程序的文档类（CDibDoc.h）中声明如下宏及公有变量： 

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)//计算图像每行象素所占的字节数目； 
HANDLE m_hDIB;//存放位图数据的句柄； 
CPalette* m_palDIB;//指向调色板Cpalette类的指针； 
CSize m_sizeDoc;//初始化视图的尺寸，该尺寸为位图的尺寸； 

　　最后将程序的字符串表中的字符串资源IDR_DibTYPE修改为："//nDib//nDib//nDib Files(*.bmp;*.dib)//n.bmp//nDib.Document//nDib Document"。这样作的目的是为了在程序文件对话框中可以选择BMP或DIB格式的位图文件。 

　　1、 读取灰度BMP位图 

　　可以根据BMP位图文件的结构，操作BMP位图文件并读入图像数据，为此我们充分利用了VC的文档视图结构，重载了文挡类的 OnOpenDocument（）函数，这样用户就可以在自动生成程序的打开文件对话框中选择所要打开的位图文件，然后程序将自动调用该函数执行读取数据的操作。该函数的实现代码如下所示： 

BOOL CDibDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{ 
LOGPALETTE *pPal;//定义逻辑调色板指针； 
pPal=new LOGPALETTE;//初始化该指针； 
CFile file; 
CFileException fe; 
if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe)) 
{//以"读"的方式打开文件； 
AfxMessageBox("图像文件打不开！"); 
return FALSE; 
} 
DeleteContents();//删除文挡； 
BeginWaitCursor(); 
BITMAPFILEHEADER bmfHeader;//定义位图文件头结构； 
LPBITMAPINFO lpbmi; 
DWORD dwBitsSize; 
HANDLE hDIB; 
LPSTR pDIB;//指向位图数据的指针； 
BITMAPINFOHEADER *bmhdr;//指向位图信息头结构的指针 
dwBitsSize = file.GetLength();//得到文件长度 
if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) !=sizeof(bmfHeader)) 
return FALSE;//读取位图文件的文件头结构信息； 
if (bmfHeader.bfType != 0x4d42) //检查该文件是否为BMP格式的文件； 
return FALSE; 
hDIB=(HANDLE) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, dwBitsSize); 
//为读取图像文件数据申请缓冲区 
if (hDIB == 0) 
{ 
return FALSE; 
} 
pDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB); 
//得到申请的缓冲区的指针； 
if (file.ReadHuge(pDIB, dwBitsSize - sizeof(BITMAPFILEHEADER)) != 
dwBitsSize - sizeof(BITMAPFILEHEADER) ) 
{ 
::GlobalUnlock((HGLOBAL)hDIB); 
hDIB=NULL; 
return FALSE; 
}//此时pDIB数据块中读取的数据包括位图头信息、位图颜色表、图像像素的灰度值； 
bmhdr=(BITMAPINFOHEADER*)pDIB;//为指向位图信息头结构的指针赋值； 
::GlobalUnlock((HGLOBAL)hDIB); 
if ((*bmhdr).biBitCount!=8)//验证是否为8bit位图 
{ 
AfxMessageBox("该文件不是灰度位图格式！"); 
return FALSE; 
} 
m_hDIB=hDIB;//将内部变量数据赋于全局变量； 
//下面是记录位图的尺寸； 
m_sizeDoc.x=bmhdr->biWidth; 
m_sizeDoc.y=bmhdr->biHeight; 
//下面是根据颜色表生成调色板； 
m_palDIB=new Cpalette; 
pPal->palVersion=0x300;//填充逻辑颜色表 
pPal->palNumEntries=256; 
lpbmi=(LPBITMAPINFO)bmhdr; 
for(int i=0;i<256;i++) 
{//每个颜色表项的R、G、B值相等，并且各个值从"0"到"255"序列展开； 
Pal->palPalentry[i].peRed=lpbmi->bmiColors[i].rgbRed; 
pPal->palPalentry[i].peGreen=lpbmi->bmiColors[i].rgbGreen; 
pPal->palPalentry[i].peBlue= lpbmi->bmiColors[i].rgbBlue;; 
pPal->palPalentry[i].peFlags=0; 
} 
m_palDIB->CreatePalette(pPal); 
//根据读入的数据得到位图的宽、高、颜色表; 
if(pPal) 
delete pPal; 
EndWaitCursor(); 
SetPathName(lpszPathName);//设置存储路径 
SetModifiedFlag(FALSE); // 设置文件修改标志为FALSE 
return TRUE; 
} 

　　上面的方法是通过CFile类对象的操作来读取位图文件的，它需要分析位图中的文件头信息，从而确定需要读取的图像长度。这种方法相对来说有些繁琐，其实还可以以一种相对简单的方法读取位图数据，首先在程序的资源中定义DIB类型资源，然后添加位图到该类型中，将图像数据以资源的形式读取出来，这时候就可以根据所获取的数据中的位图信息结构来获取、显示图像数据了。下面的函数实现了以资源形式装载图像文件数据，该函数的实现代码如下所示： 

///////////////////////////////////////////////////////////////// 
HANDLE LoadDIB(UINT uIDS, LPCSTR lpszDibType) 
{ 
LPCSTR lpszDibRes =MAKEINTRESOURCE(uIDS);//根据资源标志符确定资源的名字； 
HINSTANCE hInst=AfxGetInstanceHandle();//得到应用程序的句柄； 
HRSRC hRes=：：FindResource(hInst,lpszDibRes, lpszDibType);//获取资源的句柄，这里lpszDibType为资源的名字"DIB"; 
If(hRes==NULL) 
return NULL 
HGLOBAL hData=：：LoadResource(hInst, hRes);//转载资源数据并返回该句柄； 
return hData; 
} 

　　2、 灰度位图数据的存储 

　　为了将图像处理后所得到的像素值保存起来，我们重载了文档类的OnSaveDocument（）函数，这样用户在点击Save或SaveAs子菜单后程序自动调用该函数，实现图像数据的存储。该函数的具体实现如下： 

/////////////////////////////////////////////////////////////////// 
BOOL CDibDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{ 
CFile file; 
CFileException fe; 
BITMAPFILEHEADER bmfHdr; // 位图文件头结构； 
LPBITMAPINFOHEADER lpBI;//指向位图头信息结构的指针； 
DWORD dwDIBSize;； 
if (!file.Open(lpszPathName, CFile::modeCreate |CFile::modeReadWrite | CFile::shareExclusive, &fe)) 
{ 
AfxMessageBox("文件打不开")； 
return FALSE; 
}//以读写的方式打开文件； 
BOOL bSuccess = FALSE; 
BeginWaitCursor(); 
lpBI = (LPBITMAPINFOHEADER) ::GlobalLock((HGLOBAL) m_hDIB); 
if (lpBI == NULL) 
return FALSE; 
dwDIBSize = *(LPDWORD)lpBI + 256*sizeof(RGBQUAD); 
//图像的文件信息所占用的字节数； 
DWORD dwBmBitsSize;//BMP文件中位图的像素所占的字节数 
dwBmBitsSize=WIDTHBYTES((lpBI->biWidth)*((DWORD)lpBI->biBitCount)) 
*lpBI->biHeight;// 存储时位图所有像素所占的总字节数 
dwDIBSize += dwBmBitsSize; //BMP文件除文件信息结构外的所有数据占用的总字节数； 
lpBI->biSizeImage = dwBmBitsSize; // 位图所有像素所占的总字节数 
//以下五句为文件头结构填充值 
bmfHdr.bfType =0x4d42; // 文件为"BMP"类型 
bmfHdr.bfSize = dwDIBSize + sizeof(BITMAPFILEHEADER);//文件总长度 
bmfHdr.bfReserved1 = 0; 
bmfHdr.bfReserved2 = 0; 
bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + lpBI->biSize 
+ 256*sizeof(RGBQUAD); 
//位图数据距离文件头的偏移量； 
file.Write((LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));//向文件中写文件头信息； 
file.WriteHuge(lpBI, dwDIBSize); 
//将位图信息（信息头结构、颜色表、像素数据）写入文件； 
::GlobalUnlock((HGLOBAL) m_hDIB); 
EndWaitCursor()； 
SetModifiedFlag(FALSE)； // 将文档设为"干净"标志，表示此后文档不需要存盘提示； 
return TRUE； 
} 

二、 调色板的操作 

　　通过上面的操作，我们已经可以获取图像中的数据了，现在的又一个问题是如何在窗口中显示出图像数据。灰度图像要正确显示，必须实现逻辑调色板和系统调色板。首先我们介绍一下逻辑调色板结构LOGPALETTE，该结构定义如下： 

typedef struct tagLOGPALETTE 
{ 
WORD palVersion;//调色板的板本号，应该指定该值为0x300; 
WORD palNumEntries;//调色板中的表项数，对于灰度图像该值为256； 
PALETEENTRY palPalEntry[1];//调色板中的颜色表项，由于该表项的数目不一定，所以这里数组长度定义为1,灰度图像对应的该数组的长度为256; 
}LOGPALETTE; 
颜色表项结构PALETTEENTRY定义了调色板中的每一个颜色表项的颜色和使用方式，定义如下： 
typedef struct tagPALETTEENTRY 
{ 
BYTE peRed; //R分量值； 
BYTE peGreen; //G分量值； 
BYTE peBlue; //B分量值； 
BYTE peFlags; // 该颜色被使用的方式，一般情况下设为"0"； 
}PALETTEENTRY; 

　　Windows系统使用调色板管理器来管理与调色板有关的操作，通常活动窗口的调色板即是当前系统调色板，所有的非活动窗口都必须按照此系统调色板来显示自己的颜色，此时调色板管理器将自动的用系统调色板中的最近似颜色来映射相应的显示颜色。如果窗口或应用程序按自己的调色板显示颜色，就必须将自己的调色板载入到系统调色板中，这种操作叫作实现调色板，实现调色板包括两个步骤，既首先将调色板选择到设备上下文中，然后在设备上下文中实现它。可以通过CDC：：SelectPalette()、CDC：：RealizePalette()或相应的API函数来实现上述的两个步骤。在实现调色板的过程中，通过在主框架类中处理Windows定义的消息WM_QUERYNEWPALETTE 、WM_PALETTECHANGED及视图类中处理自定义消息WM_DOREALIZE（该消息在主框架窗口定义如下：#define WM_REALIZEPAL (WM_USER+101)）来实现调色板的操作。当系统需要处理调色板的变化时，将向程序的主窗口发送WM_QUERYNEWPALETTE 、WM_PALETTECHANGED，例如当某一窗口即将激活时，主框架窗口将收到WM_QUERYNEWPALETTE消息，通知该窗口将要收到输入焦点，给它一次机会实现其自身的逻辑调色板；当系统调色板改变后，主框架窗口将收到WM_PALETTECHANGED消息，通知其它窗口系统调色板已经改变，此时每一窗口都应该实现其逻辑调色板，重画客户区。 

　　由于上述的调色板变更消息是发往主框架窗口的，所以我们只能在主窗口中响应这两个消息，然后由主框架窗口通知各个视窗，使得程序激活时能自动装载自己的调色板。我们定义的用户消息WM_REALIZEPAL用于主框架窗口通知视窗它已经收到调色板变更消息，视窗应该协调其调色板。下面我们给出了各个消息的响应处理函数的具体实现代码和注释： 

////////////////////////////////////////////////////////// 
void CMainFrame::OnPaletteChanged(CWnd* pFocusWnd) 
{//总实现活动视的调色板 
CMDIFrameWnd::OnPaletteChanged(pFocusWnd); 
CMDIChildWnd* pMDIChildWnd = MDIGetActive();//得到活动的子窗口指针； 
if (pMDIChildWnd == NULL) 
return 
CView* pView = pMDIChildWnd->GetActiveView();//得到视图的指针； 
ASSERT(pView != NULL); 
SendMessageToDescendants(WM_DOREALIZE, (WPARAM)pView->m_hWnd); 
//通知所有子窗口系统调色板已改变 
} 
//////////////////////////////////////////////// 
BOOL CMainFrame::OnQueryNewPalette()//提供实现系统调色板的机会 
{ 
// 实现活动视的调色板 
CMDIChildWnd* pMDIChildWnd = MDIGetActive();//得到活动的子窗口指针； 
if (pMDIChildWnd == NULL) 
return FALSE;//no active MDI child frame (no new palette) 
CView* pView = pMDIChildWnd->GetActiveView();//得到活动子窗口的视图指针； 
ASSERT(pView != NULL); 
//通知活动视图实现系统调色板 
pView->SendMessage(WM_DOREALIZE, (WPARAM)pView->m_hWnd); 
return TRUE; 
} 
///////////////////////////////////////////////// 
BOOL CDibView::OnDoRealize(WPARAM wParam, LPARAM)//实现系统调色板 
{ 
ASSERT(wParam != NULL); 
CDibDoc* pDoc = GetDocument(); 
if (pDoc->m_hDIB == NULL) 
return FALSE; // must be a new document 
CPalette* pPal = pDoc->m_palDIB; 
//调色板的颜色表数据在InitDIBData()函数中实现 
if (pPal != NULL) 
{ 
CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;//得到程序的主框架指针； 
ASSERT_KINDOF(CMainFrame, pAppFrame); 
CClientDC appDC(pAppFrame);//获取主框架的设备上下文； 
CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd); 
//只有活动视才可以设为"FALSE"，即根据活动视的调色板设为"前景"调色板； 
if (oldPalette != NULL) 
{ 
UINT nColorsChanged = appDC.RealizePalette();//实现系统调色板 
if (nColorsChanged > 0) 
pDoc->UpdateAllViews(NULL);//更新视图 
appDC.SelectPalette(oldPalette, TRUE); 
//将原系统调色板置为背景调色板 
} 
else 
{ 
TRACE0("//tSelectPalette failed in"); 
} 
return TRUE； 
} 

　　注：在调用API函数显示位图时，不要忘记设置逻辑调色板，即"背景"调色板，否则位图将无法正确显示，读者可以从后面的显示部分的实现看出我们在显示时实现了逻辑调色板。上述的处理相对来说比较繁琐复杂，可能对于初学者来说也比较难于理解，所以如果我们的程序仅仅限于处理灰度图象，可以采用另外一种相对简单的办法，即在文档类的初始化阶段定义一个灰度调色板，然后在设备上下文中实现它，这样作的好处是在度取灰度位图时可以不再考虑文件中的颜色表信息，提高了文件读取速度，笔者在开发一个基于机器视觉的项目时采用的就是这种方法，取的了比较满意的效果。首先定义一个指向逻辑颜色表结构 LOGPALETTE的指针pPal，填充该指针，然后将该指针与调色板指针联系起来，该方法的具体实现如下： 

///////////////////////////////////////////////////////// 
CDibDoc::CDibDoc() 
{ 
............................ 
LOGPALETTE *Pal; 
Pal=new LOGPALETTE; 
m_palDIB=new Cpalette; 
pPal->palVersion=0x300; 
pPal->palNumEntries=256; 
for(int i=0;i<256;i++) 
{//每个颜色表项的R、G、B值相等，并且各个值从"0"到"255"序列展开； 
Pal->palPalentry[i].peRed=i; 
pPal->palPalentry[i].peGreen=i; 
pPal->palPalentry[i].peBlue=i; 
pPal->palPalentry[i].peFlags=0; 
} 
m_palDIB->CreatePalette(pPal); 
....................... 
} 

三、 图像的显示 

　　显示DIB位图数据可以通过设备上下文CDC对象的成员函数CDC：：Bitblt()或CDC：：StretchBlt()来实现，也可以通过API函数SetDIBBitsToDevice()或StretchDIBBits()来实现，函数中具体所用到的各个参数的意义可以参考 MSDN。其中StretchDIBBits()和CDC：：StretchBlt()可以将图像进行放大和缩小显示。当从文档中装入位图文件时， CDIBView类的OnInitialUpdate函数将被调用，因此可以在该函数中实现对视图尺寸的设置，用于正确的显示位图，然后就可以在视图类的 OnDraw（）函数中正确的显示位图了。这两个函数的具体实现代码分别如下所示： 

///////////////////////////////////////////////////////////// 
void CDIBView：：OnInitialUpdate() 
{ 
CscrollView：：OnInitalUpdate(); 
CDIBDoc *pDoc=GetDocument(); 
If(pDoc->m_hDIB==NULL)//如果位图数据为空，设置m_sizeDoc的默认尺寸； 
pDoc->m_sizeDoc.cx=pDoc->m_sizeDoc.cy=100; 
SetScrollSizes(MM_TEXT,pDoc-> m_sizeDoc); 
} 
///////////////////////////////////////////////////////////// 
void CDIBView：：OnDraw(CDC *pDC) 
{ 
BITMAPINFOHEADER *lpDIBHdr;//位图信息头结构指针； 
BYTE *lpDIBBits;//指向位图像素灰度值的指针； 
BOOL bSuccess=FALSE; 
CPalette*OldPal=NULL;//调色板指针； 
HDC hDC=pDC->GetSafeHdc();//获取当前设备上下文的句柄； 
CDIBDoc *pDoc=GetDocument();//获取活动文档的指针； 
If(pDoc->m_hDIB ==NULL) 
{//判断图像数据是否为空； 
AfxMessageBox("图像数据不能为空，请首先读取图像数据！"); 
return; 
} 
lpDIBHdr=( BITMAPINFOHEADER *)GlobalLock(pDoc->m_hDIB);//得到图像的位图头信息 
lpDIBBits=lpDIBHdr+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);//获取保存图像像素值的缓冲区的指针； 
if(pDoc-> m_palDIB) 
{//如果存在调色板信息，实现逻辑调色板； 
OldPal=pDC-> SelectPalette(pDoc-> m_palDIB,TRUE); 
PDC->RealizePalette(); 
} 
else 
{ 
AfxMessageBox("图像的调色板数据不能为空，请首先读取调色板信息！"); 
return ; 
} 
SetStretchBltMode(hDC,COLORONCOLOR); 
//显示图像 
BSuccess=StretchDIBBits(hDC,0,0,pDoc-> m_sizeDoc.cx, pDoc-> m_sizeDoc.cy, 
0, pDoc-> m_sizeDoc.cy,0, pDoc-> m_sizeDoc.cy, 
lpDIBBits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY); 
GlobalUnlock(pDoc->m_hDIB); 
If(OldPal)//恢复调色板； 
PDC->SelectPalette(OldPal,FALSE); 
retrun; 
} 

　　四、 小结 

　　在本期讲座里我们主要介绍了如何操作灰度位图，它具有较强的代表性，同时为后续的图像处理编程的学习作了必要的准备工作，经过学习，对于如何操作其它类型的BMP格式的图像文件，可以达到举一反三的作用。 
VC数字图像处理编程讲座之四 

BMP图像显示的特效操作 

　　上期讲座中我们主要讲述了BMP图像数据的存取、图像的显示和调色板的操作等内容，在上面的学习基础上，我们可以进一步深化，学习并掌握图像特效显示技术。有了这种技术，可以用来在今后的项目开发中美化我们的软件界面，提高软件的视觉效果。在如今的商业软件中，几乎每一幅图像的显示都采用了图像特效显示，例如读者比较熟悉的Windows的屏幕保护程序就采用了各种各样的图像特效显示，使人感到眼花缭乱和耳目一新。专业图像处理软件更是提供了丰富的显示方式供用户使用，可以方便的在程序中实现图像的特效显示，如PhotoShop 、Authorware等。本节主要介绍如何实现图像的浮雕、雕刻、百页窗、旋转、扫描、栅条、马赛克、和渐显渐隐显示等效果。通过这期讲座的学习，读者朋友们也可以自己动手制作拥有特效显示效果的软件了。 

　　图像的显示我们讲过主要有BitBlt()、SetDIBitsToDevice()和StretchDIBits（）等函数。需要读者注意的是，在特效显示时，并不是每个显示函数都适宜，BitBlt()函数主要是用来显示设备无关位图（DDB），后两个函数用来显示设备无关位图（DIB）。由于我们讲座里处理的是设备无关位图，所以我们主要关心的是后两个函数的应用，其中SetDIBitsToDevice()使用起来较死板，远不如StretchDIBits（）用的灵活，并且对大多数的特效显示无能为力，所以为了实现图像的特效显示效果，需要使用StretchDIBits （）函数来显示图像，具体什么原因，我想可能是微软在实现这些函数时使用的方法不同吧。这些函数如何使用，各个参数的含义，可以参考微软的MSDN。 

　　实现图像的特殊效果的显示的基本思路是要么是操作图像的像素，要么是对图像分块按一定的方向或次序，分阶段的显示或擦除对应的图像块。对于第二种显示的思路，其中的要点是：1.划分图像块；2.确定图像块的操作次序；3.显示或清除对应的图像块；4.在两个连续显示的图像块之间插入一个固定的延迟。其中图像块的划分决定了图像的显示方式，图像块的显示顺序决定了显示的方向和细分的依据。不同的效果决定了不同的分块方法和显示次序，我们将在后面的各种特效显示中介绍如何分块和决定次序。为了使图像的显示过程明显的表现出来，实现显示的特效，就需要在图像块的依此显示中插入固定的延迟。也许读者朋友会想到利用sleep()函数或用Settime（）来实现延迟，由于Windows是个基于消息的多任务操作系统，这些方法所产生的延迟时间对于图像的显示来说是不精确的，为了实现与机器无关的更精确的时间延迟，可以采用timeGetTime()函数来产生微秒级的延迟。使用这个函数时为了编译不产生错误，要在连接设置中引入"Winmm.lib"库，并要包含头文件"Mmsystem.h"。这里我们首先给出一个延迟函数，它用来实现固定时间的延迟： 

void DelayTime(DWORD time) 
{ 
DWORD BeginTime ，EndTime; 
BeginTime=timeGetTime();//得到当前的系统时间、单位为微秒； 
do 
{ 
EndTime=TimeGetTime();//再次得到当前的系统时间； 
} 
while((EndTime-BeginTime) 
} 

一、操作位图的像素实现显示的特效 

　　我们首先介绍直接操作图像中的像素的灰度值来实现图像显示的特效、这里我们主要介绍如何实现图像的浮雕和雕刻效果。经常看电视的朋友们不知注意到没有，有些电视连续剧在每集片头或片尾部分都有显示一些特殊效果的图像，比如前一阵子中央一套放的《长征》和《康熙王朝》，这些特效称为"图像的浮雕效果"和"图像的雕刻效果"，经过这些特效处理后的图像增强了观众们的视觉效果，它们看上去仿佛是使用3D技术作的，这也许就是为什么这种技术那么流行的原因吧。其实，我们完全可以用一些简单的数字图像处理算法来实现这些看似复杂高深的显示效果。下面以一个标准的Lena灰度图像为原图，给出了处理后的效果图，同时给出了VC开发平台上的部分实现源代码。 

　　1．"浮雕"图像 

　　"浮雕"图象效果是指图像的前景前向凸出背景。所谓的"浮雕"概念是指标绘图像上的一个像素和它左上方的那个像素之间差值的一种处理过程，为了使图像保持一定的亮度并呈现灰色，我在处理过程中为这个差值加了一个数值为128的常量。需要读者注意的是，当设置一个像素值的时候，它和它左上方的像素都要被用到，为了避免用到已经设置过的像素，应该从图像的右下方的像素开始处理，下面是实现的源代码： 

void CDibView::OnFDImage() //产生"浮雕"效果图函数 
{ 
　HANDLE data1handle;//用来存放图像数据的句柄； 
　LPBITMAPINFOHEADER lpBi;//图像的信息头结构； 
　CDibDoc *pDoc=GetDocument();//得到文挡指针； 
　HDIB hdib;//用来存放图像数据的句柄； 
　unsigned char *pData;//指向原始图像数据的指针； 
　unsigned char *data;//指向处理后图像数据的指针； 
　hdib=pDoc->m_hDIB;//拷贝存放已经读取的图像文件数据句柄； 
　lpBi=(LPBITMAPINFOHEADER)GlobalLock((HGLOBAL)hdib);//获取图像信息头 
pData=(unsigned char*)FindDIBBits((LPSTR)lpBi); 
//FindDIBBits是我定义的一个函数、根据图像的结构得到位图的灰度值数据、 
pDoc->SetModifiedFlag(TRUE); 
//设置文档修改标志为"真"、为后续的修改存盘作准备； 
data1handle=GlobalAlloc(GMEM_SHARE,WIDTHBYTES(lpBi->biWidth*8)*lpBi->biHeight);　//声明一个缓冲区用来暂存处理后的图像数据； 
data=(unsigned char*)GlobalLock((HGLOBAL)data1handle);//得到该缓冲区的指针； 
AfxGetApp()->BeginWaitCursor(); 
int i,j,buf; 
for( i=lpBi->biHeight; i>=2; i--)//从图像右下角开始对图像的各个像素进行"浮雕"处理； 
　　for( j=lpBi->biWidth; j>=2; j--) 
　　{ 
//浮雕处理 
buf=*(pData+(lpBi->biHeight-i)*WIDTHBYTES(lpBi->biWidth*8)+j)-*(pData+(lpBi->biHeight-i+1)*WIDTHBYTES(lpBi->biWidth*8)+j-1)+128; 
if(buf>255) buf=255; 
if(buf<0)buf=0; *(data+(lpBi->biHeight-i)*WIDTHBYTES(lpBi->biWidth*8)+j)=(BYTE)buf; 
} 
for( j=0; jbiHeight; j++) 
for( i=0; ibiWidth; i++) 
//重新写回原始图像的数据缓冲区; 
*(pData+i*WIDTHBYTES(lpBi->biWidth*8)+j)=*(data+i*WIDTHBYTES(lpBi->biWidth*8)+j);　　　　AfxGetApp()->EndWaitCursor(); 
pDoc->m_hDIB =hdib//将处理过的图像数据写回pDoc中的图像缓冲区； 
GlobalUnlock((HGLOBAL)hdib);//解锁、释放缓冲区； 
GlobalUnlock((HGLOBAL)data1handle); 
GlobalFree((HGLOBAL)hdib); 
GlobalFree((HGLOBAL)data1handle); 
Invalidate(TRUE);//显示图像 
} 

2．"雕刻"图像 

　　上面讲述了通过求一个像素和它左上方像素之间的差值并加上一个常数的方法生成"浮雕"效果的灰度图像，"雕刻"图像与之相反，它是通过取一个像素和它右下方的像素之间的差值并加上一个常数，这里我也取128，经过这样处理，就可以得到"雕刻"图像，这时候图像的前景凹陷进背景之中。同样需要读者注意的是为了避免重复使用处理过的图像像素，处理图像时要从图像的左上方的像素开始处理。实现代码如下： 

void CDibView::OnDKImage() 
{ 
　// TOD Add your command handler code here 
　HANDLE data1handle;//这里的内部变量与前面的含义一致、这里不再赘述； 
　LPBITMAPINFOHEADER lpBi; 
　CDibDoc *pDoc=GetDocument(); 
　HDIB hdib; 
　unsigned char *pData; 
　unsigned char *data; 
　hdib=pDoc->m_hDIB;//拷贝图像数据的句柄； 
　lpBi=(LPBITMAPINFOHEADER)GlobalLock((HGLOBAL)hdib); 
　pData=(unsigned char*)FindDIBBits((LPSTR)lpBi); 
　pDoc->SetModifiedFlag(TRUE); 
　data1handle=GlobalAlloc(GMEM_SHARE,WIDTHBYTES(lpBi->biWidth*8)*lpBi->biHeight);//申请缓冲区； 
　data=(unsigned char*)GlobalLock((HGLOBAL)data1handle);//得到新的缓冲去的指针；　AfxGetApp()->BeginWaitCursor(); 
　int i,j,buf； 
　for( i=0;i<=lpBi->biHeight-2; i++)//对图像的各个像素循环进行"雕刻"处理； 
　　for( j=0;j<=lpBi->biWidth-2; j++) 
　{ 
　　　buf=*(pData+(lpBi->biHeight-i)*WIDTHBYTES(lpBi->biWidth*8)+ j)-*(pData+(lpBi->biHeight-i-1)*WIDTHBYTES(lpBi->biWidth*8)+j+1)+128; //"雕刻"处理； 
　if(buf>255) buf=255; 
　if(buf<0)buf=0; 
　*(data+(lpBi->biHeight-i)*WIDTHBYTES(lpBi->biWidth*8)+j)=(BYTE)buf; 
　} 
　for( j=0; jbiHeight; j++) 
　 for( i=0; ibiWidth; i++)　　//重新将处理后的图像数据写入原始的图像缓冲区内；　　*(pData+i*WIDTHBYTES(lpBi-> biWidth*8)+j)=*(data+i*WIDTHBYTES(lpBi->biWidth*8)+j); 
pDoc->m_hDIB =hdib//将处理过的图像数据写回pDoc中的图像缓冲区； 
GlobalUnlock((HGLOBAL)hdib);//解锁、释放缓冲区； 
GlobalUnlock((HGLOBAL)data1handle); 
GlobalFree((HGLOBAL)hdib); 
GlobalFree((HGLOBAL)data1handle); 
Invalidate(TRUE);//显示图像 
} 

　　3．图像的旋转 

　　根据图像像素的位置来调节该位置的灰度可以实现许多显示的特效，例如图像的镜像、翻转等。灰度图像旋转就是根据这一个思想实现的，它是指把定义的图像绕某一点以逆时针或顺时针方向旋转一定的角度，通常是指绕图像的中心以逆时针方向旋转。首先根据旋转的角度、图像对角线的长度计算旋转后的图像的最大宽度、高度，根据旋转后图象最大的宽度、高度生成新的缓冲区，假设图像的左上角为（left, top），右下角为（right, bottom），则图像上任意点（x, y）绕其中心（xcenter, ycenter）逆时针旋转angle角度后，新的坐标位置（x1, y1）的计算公式为： 

　　xcenter = (width+1)/2+left; 
　　ycenter = (height+1)/2+top; 
　　x1 = (x-xcenter) cosθ - (y - ycenter) sinθ+xcenter; 
　　y1 = (x-xcenter) sinθ+ (y- ycenter) cosθ+ ycenter; 

　　与图像的镜像变换相类似，下一步就是把原图中的(x，y)处象素的灰度值读入新缓冲区的(x1，y1)点处。注意在新缓冲区中与原图没有对应的象素点的值用白色或指定的灰度代替。 

二、图像的分块显示和清除 

　　1． 图像的扫描显示和清除 

　　扫描显示图像是最基本的特效显示方法，它表现为图像一行行（或一列列）地显示出来或从屏幕上清除掉，有种大戏院种的拉幕效果。根据扫描的方向的不同，可以分为上、下、左、右、水平平分和垂直平分等六种扫描。这里以向下移动为例，分别介绍显示和清除的实现。其余的扫描效果可以依次类推。向下扫描显示的实现方法是：从图像的底部开始将图像一行一行的复制到目标区域的顶部。每复制一行后，复制的行数便要增加一行，并加上一些延迟；向下移动清除的实现方法是图像向下移动显示，并在显示区域的上部画不断增高的矩形。 

　　1）扫描显示的代码： 

CdibView::OnImageDownScan() 
{ 
CDibDoc *pDoc=GetDocument(); 
HDIB hdib; 
CClientDC pDC(this); 
hdib=pDoc->m_hDIB;//获取图像数据句柄； 
BITMAPINFOHEADER *lpDIBHdr;//位图信息头结构指针； 
BYTE *lpDIBBits;//指向位图像素灰度值的指针； 
HDC hDC=pDC.GetSafeHdc();//获取当前设备上下文的句柄； 
lpDIBHdr=( BITMAPINFOHEADER *)GlobalLock(hdib);//得到图像的位图头信息； 
lpDIBBits=(BYTE*)lpDIBHdr+sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);//获取指向图像像素值； 
SetStretchBltMode(hDC,COLORONCOLOR); 
//显示图像； 
for(int i=0;ibiHeight;i++) 
{ //每次循环显示图象的"0"到"i"行数据； 
SetDIBitsToDevice (hDC,0,0,lpDIBHdr->biWidth, lpDIBHdr->biHeight, 
0, 0,0, i, 
lpDIBBits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS 
); 
DelayTime(50);//延迟； 
} 
GlobalUnlock(hdib); 
return; 
} 

　　2）清除代码： 

.......................................//由于篇幅的限制，省略了与上面的相同代码 
Cbrush brush(crWhite);//定义一个"白色"的刷子； 
Cbrush *oldbrush=pDC->SelectObject(&brush); 
for(int i=0;i < lpDIBHdr->biHeight ;i++) 
{//每次循环将目标区域中的"0"到"i"行刷成"白色"； 
pDC->Rectangle(0，0，lpDIBHdr->biWidth，lpDIBHdr->biHeight); 
DelayTime(50); 
} 
....................................... 

　　2． 百页窗效果 

　　所谓百页窗显示效果，就如同关闭和开启百页窗一样，图像被分为一条条或一列列地分别显示或清除掉，根据显示时以行或列为单位可以将该效果分为垂直或水平两种方式。以垂直百页窗为例来说明如何实现这种特效显示。实现垂直百页窗显示时，需要将图像垂直等分为n部分由上向下扫描显示，其中每一部分包括m个条、这个n可以根据具体应用时的需要来决定、m既为图像的高度除n。扫描显示时，依照差值进行扫描显示，即第k次显示k-1、k*m-1、...k*n -1条扫描线。同样，垂直百页窗清除的实现与垂直百页窗的显示相似，不同的是将绘制位图换成画矩形而已。在下面的例子中，我将图像的分成8份。 

....................................... 
int m=8; 
int n=lpDIBHdr->biHeight/m;//图像的高度能够整除8； 
for(int l=1;l<=m;l++) 
for(int k=0;k
{ //每次循环依次显示图像中的k-1、k*m-1、...k*n-1行； 
StretchDIBits (hDC,0,4*k+l-1,lpDIBHdr->biWidth,1, 
0, lpDIBHdr->biHeight-4*k-l+1,lpDIBHdr->biWidth,1, 
lpDIBBits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY);//juanlianxiaoguo 

DelayTime(50); 
} 
....................................... 

3.栅条显示特效 

　　栅条特效是移动特效的复杂组合，可以分为垂直栅条和水平栅条两类。它的基本思想是将图像分为垂直或水平的的小条，奇数条向上或向左显示/清除，偶数条向下或向右显示/清除。当然也可以规定进行相反的方向显示/清除。下面的代码是实现垂直栅条的例子： 

....................................... 
int m=8; 
for(int i=0;i<=lpDIBHdr->biHeight;i++) 
for(int j=0;j<=lpDIBHdr->biWidth;j+=m) 
{//向下显示偶数条； 
StretchDIBits (hDC,j,0,m,i,j,lpDIBHdr->biHeight-i, 
m,i, 
lpDIBBits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY);//juanlianxiaoguo 
j=j+m; 
//向上显示奇数条； 
StretchDIBits (hDC,j,lpDIBHdr->biHeight-i,m,i,j,0, 
m,i, 
lpDIBBits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY);// 
DelayTime(20); 
}....................................... 

　　4.马赛克效果 

　　马赛克显示是指图像被分成许多的小块，它们以随机的次序显示出来，直到图像显示完毕。实现马赛克的效果主要解决的问题是如何定义显示随机序列的小方块，这个问题的解决可以在定义过小方块的基础上，用一个数组来记录各个方块的左上角的坐标的位置。显示图像过程中，产生一个随机数来挑选即将显示的小方块，显示后将该方块的位置坐标从数组中剔除。清除过程与之相仿。剔除显示过的方块的位置坐标的方法是将该数组中的最后的一个点的坐标拷贝到当前位置，然后删除数组中的最后点的坐标，经过实现发现这样处理有时显示的图像是不完整的，分析其原因是生成随机数的过程有舍入溢出误差。读者可以采用其它的办法解决这个问题，例如可以生成固定的随机数组或采用一个动态的数组来跟踪未显示的图像方块的坐标等方法。 

....................................... 
int m,n; 
int RectSize=60;//方块的宽、高尺寸为60个像素； 
if(lpDIBHdr->biWidth%RectSize!=0)//得到图像水平方块的个数； 
m= lpDIBHdr->biWidth/RectSize+1; 
else 
m= lpDIBHdr->biWidth/RectSize; 
if(lpDIBHdr->biHeight%RectSize!=0)//得到图像垂直方块的个数； 
n= lpDIBHdr->biHeight/RectSize+1; 
else 
n=lpDIBHdr->biHeight/RectSize; 
POINT *point=new POINT[n*m];//申请一个数组用来记录各个方块的左上角的坐标； 
POINT point1; 
for(int a=0;a
for(int b=0;b
{ 
point1.x=a*RectSize; 
point1.y=b*RectSize; 
*(point+a*b+b)=point1; 
} 
//开始随机的显示各个小方块； 
double fMax=RAND_MAX;//定义Rand()函数的最大值； 
for(int k=m*n-1;k>=0;k--) 
{ 
int c=(int)((double)(m*n)*rand()/fMax); 
int mx=point[c].x; 
int my=point[c].y; 
//显示对应的图像的小块； 
StretchDIBits (hDC,mx,my,RectSize,RectSize, 
mx,lpDIBHdr->biHeight-my,RectSize,RectSize, 
lpDIBBits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY); 
point[c].x=point[k].x; 
point[c].y=point[k].y; 
DelayTime(50); 
} 
....................................... 

　　5．图像的淡入淡出效果 

　　图像的淡入淡出的显示效果被广泛的应用在多媒体娱乐软件中，是一种特别重要的特效显示方法。淡入就是将显示图像的目标区域由本色逐渐过度的图像中的各个像素点的颜色；淡出就是由显示的图像逐渐过度到目标区域的本色。实现图像的淡入淡出有两种办法：一是均匀的改变图像的调色板中的颜色索引值；另一种方法是改变图像像素的灰度值。第一种方法实现起来比较繁琐，第二种方法就比较简单。下面是我们采用第二种方法实现图像淡入效果的代码： 

....................................... 
//申请一个与图像缓冲区相同大小的内存； 
hdibcopy=(HDIB)GlobalAlloc(GMEM_SHARE,lpDIBHdr->biWidth*lpDIBHdr->biHeight); 
lpbits=(BYTE*)GlobalLock(hdibcopy); 
//将缓冲区的数据初始化； 
for(int k=0;kbiWidth*lpDIBHdr->biHeight;k++) 
{ 
*(lpbits+k)=(BYTE)255; 
} 
//显示最初的图像为"白色" 
StretchDIBits (hDC,0,0,lpDIBHdr->biWidth,lpDIBHdr->biHeight,0,0, 
lpDIBHdr->biWidth,lpDIBHdr->biHeight, 
lpbits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY); 
//布尔变量end用来标志何时淡入处理结束； 
BOOL end=false; 
while(!end) 
{ int a=0; 
for(int k=0;kbiWidth*lpDIBHdr->biHeight;k++) 
{ 
//判断是否待显示的像素的灰度值已经小于原始图像对应点的灰度值，如是则计数； 
if(*(lpbits+k)<*(lpDIBBits+k)) 
a++; 
else//否则对应点的灰度值继续减少； 
*(lpbits+k)-=(BYTE)10; 
} 
//显示处理后的图像数据lpbits; 
StretchDIBits (hDC,0,0,lpDIBHdr->biWidth,lpDIBHdr->biHeight,0,0, 
lpDIBHdr->biWidth,lpDIBHdr->biHeight, 
lpbits,(LPBITMAPINFO)lpDIBHdr, 
DIB_RGB_COLORS, 
SRCCOPY); 
//如果所有的点的灰度值的都小于或等于原始图像的像素点的灰度值，则认为图像的淡入处理结束。 
if(a==lpDIBHdr->biWidth*lpDIBHdr->biHeight) 
end=true; DelayTime(50); 
....................................... 

　　本文上面的内容介绍了几种图像的特殊显示效果，代码在Windows2000和Visual C++6.0编程环境下编译通过，运行正常，处理达到了预期的效果。读者可以将上面介绍的显示图像的函数和处理思路结合起来，实现更多效果。 
VC编程实现灰度图像与彩色图像的相互转换 

PhotoShop的图像处理功能很强，其中有一个功能是将灰度图像转换为彩色图像，数字图像处理中，也经常要遇到灰度图像与彩色图像相互转换的问题，如何自己解决这个问题，值得大家探讨，现将我解决这类问题的方法陈述如下： 

　　工程应用中经常要遇到需要把彩色图像到灰度图像的变换的问题，采集卡过来的图像为彩色图像，为加快处理速度，要把彩色图像转换为黑白图象，这个问题比较好解决，一般情况下彩色图像每个像素用三个字节表示，每个字节对应着R、G、B分量的亮度（红、绿、蓝），转换后的黑白图像的一个像素用一个字节表示该点的灰度值，它的值在0~255之间，数值越大，该点越白，既越亮，越小则越黑。转换公式为Gray(i,j)=0.11*R(i,j)+ 0.59*G(i,j)+0.3*B(i,j)，其中Gray(i,j)为转换后的黑白图像在（i,j）点处的灰度值，我们可以观察该式，其中绿色所占的比重最大，所以转换时可以直接使用G值作为转换后的灰度。 

　　至于灰度图像转换为彩色图像，技术上称为灰度图像的伪彩色处理，这是一种视觉效果明显而技术又不是很复杂的图像增强技术。灰度图像中，如果相邻像素点的灰度相差不大，但包含了丰富的信息的话，人眼则无法从图像中提取相应的信息，因为人眼分辨灰度的能力很差，一般只有几十个数量级，但是人眼对彩色信号的分辨率却很强，这样将黑白图像转换为彩色图像人眼可以提取更多的信息量。在转换过程中，经常采用的技术是灰度级-彩色变换，意思就是对黑白图像上的每一个像素点，取得该点的灰度值并送入三个通道经过实施不同的变换，产生相应的R、G、B的亮度值，即所求彩色图像对应像素点的彩色值，具体变换公式很多，我采用的是最常用的一种，变换曲线图如下：

上图中，三个图分别代表了三个变换通道，R、G、B指的是变换后对应点的R、G、B分量值，L指的是各个分量的最大值为255，G（x,y）为相应点的灰度值。理论上就这些，下面是我用VC实现的源代码，图一为我的灰度位图，图二为伪彩色处理后的结果图。我这个实现函数中是如何得到灰度位图的数据的就不多讲了，有兴趣的朋友可参考我在天极网上九月十号发表的《VC灰度位图处理》一文，那里应该讲的很清楚了。需要读者注意的是彩色图像中每个象素中的三个字节分别代表的分量，第一个字节为B，第二个为G值、最后一个为R值，这个顺序不要搞错了。代码实现如下： 
void CDibView::OnMenuchange() file://图像转换实现函数 
{ 
　// TOD Add your command handler code here 
　HANDLE data1handle; 
　LPBITMAPINFOHEADER lpBi; 
　BITMAPINFO *m_pBMI; 
　CDibDoc *pDoc=GetDocument(); 
　HDIB hdib; 
　unsigned char *hData; 
　unsigned char *data; 
　hdib=pDoc->GetHDIB();//得到位图数据的句柄，其中包含图像信息头 
　BeginWaitCursor(); 
　lpBi=(LPBITMAPINFOHEADER)GlobalLock((HGLOBAL)hdib); 
　hData=(unsigned char*)FindDIBBits((LPSTR)lpBi); 
　m_pBMI=new BITMAPINFO;//生成彩色图像的信息头 
　m_pBMI->bmiHeader.biBitCount=24; 
　m_pBMI->bmiHeader.biClrImportant=0; 
　m_pBMI->bmiHeader.biClrUsed=0; 
　m_pBMI->bmiHeader.biCompression=BI_RGB; 
　m_pBMI->bmiHeader.biHeight=lpBi->biHeight; 
　m_pBMI->bmiHeader.biWidth=lpBi->biWidth; 
　m_pBMI->bmiHeader.biPlanes=1; 
　m_pBMI->bmiHeader.biSize=sizeof(BITMAPINFOHEADER); 
　m_pBMI->bmiHeader.biXPelsPerMeter=0; 
　m_pBMI->bmiHeader.biYPelsPerMeter=0; 
　m_pBMI->bmiHeader.biSizeImage=WIDTHBYTES(lpBi->biWidth*8)*lpBi->biHeight*3; 
　file://data=hData; 
　int R,G,B,i,j; 
　data1handle=GlobalAlloc(GMEM_SHARE,WIDTHBYTES(lpBi->biWidth*8)*lpBi->biHeight*3); 
　file://生成存储彩色图象数据的缓冲区 
　data=(unsigned char*)GlobalLock((HGLOBAL)data1handle); 
　for(i=0;ibiHeight;i++)//实现灰度到彩色变换 
　　for(j=0;jbiWidth*8);j++) 
　　{ 
　　　if(*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)<=64) 
　　　{R=0; 
　　　　G=(int)4*(*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)); 
　　　　B=255; 
　　　} 
　　　if(*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)>64 
　　　　&& *(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)<=128) 
　　　　{R=0; 
　　　　　G=255; 
　　　　　B=(int)4*(128-*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)); 
　　　　} 
　　　if(*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)>128 
　　　　　&& *(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)<=192) 
　　　　　{R=(int)4*(*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)-128); 
　　　　　　G=255; 
　　　　　　B=0; 
　　　　　} 
　　　if(*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)>192 
　　　　　&& *(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)<=255) 
　　　　　{R=255; 
　　　　　　G=(int)4*(255-*(hData+i*WIDTHBYTES(lpBi->biWidth*8)+j)); 
　　　　　　B=0; 
　　　　　} 
　　　file://将生成的R、G、B分量存入目标缓冲区 
　　　*(data+i*WIDTHBYTES(lpBi->biWidth*8)*3+j*3)=B; 
　　　*(data+i*WIDTHBYTES(lpBi->biWidth*8)*3+j*3+1)=G; 
　　　*(data+i*WIDTHBYTES(lpBi->biWidth*8)*3+j*3+2)=R; 
　　} 
　　GlobalUnlock((HGLOBAL)hdib); 
　　GlobalUnlock(data1handle); 
　　EndWaitCursor(); 
　　CClientDC pDC(this); 
　　file://显示真彩色图像 
　　StretchDIBits(pDC.GetSafeHdc(),0,0,lpBi->biWidth,lpBi->biHeight,0,0, 
　　　　　　　　　lpBi->biWidth, lpBi->biHeight,data,m_pBMI,DIB_RGB_COLORS, 
　　　　　　　　　SRCCOPY); 
　　delete m_pBMI; 
} 


