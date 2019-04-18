# 一种位图缩放的快速算法&PSD格式的开发&PSD格式文件的读取 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:47:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3577


一种位图缩放的快速算法&PSD格式的开发&PSD格式文件的读取

一种位图缩放的快速算法 

　　给定一个位图，如何将它缩放至任意尺寸？很明显，唯一的方法是：放大时，在像素中间添加一些重复像素，使图像拉宽；缩小时，把部分像素删除掉，使图像收缩。但是如何确定哪些像素该重复，哪些像素该删除呢？下面的方法是我自已想的，如果你有更好的方法，请告诉我。我们只考虑水平方向（垂直方向跟水平方向是同样的道理）。下面先从简单的例子来说，最后再推出一个通用的算法：

若要放大1倍，应将每一个像素都重复一次，N个像素变成了2N个像素，图像放大为2倍。这个不难；

若要缩小1/2，应该每隔一个像素删除一个像素，2N个像素变成了N个像素，图像缩小一半。这个也不难；

若要放大1.5倍，怎么办？假设原有2N个像素，现在欲变成3N个像素，须添加N个像素，所以应对原图每隔一个像素添加一个重复像素： 

若要缩小1/3，就是C的逆过程：每隔两个像素删除一个像素。 
　　上面四个例子都是比较容易的特例。现在来考虑通用的算法。在四个例子的缩放过程可以这样理解。假设欲将长度为N1的像素列变成长度为N2的像素列，首先，设立两个指针，一个作为源指针，指向原来的像素列，读取源像素，另一个作为目的指针，指向变换后的像素列，写入读取到的像素。然后，以拉伸后像素列的长度为循环次数，循环N2次，每次循环中由源指针处COPY一个像素到目的指针处，然后目的指针加一，源指针根据每次循环的不同需要增加一定步长（放大时步长是零或一，缩小时步长大于等于一）。
　　算法的框架解决了，但是中心内容仍没有解决：如何确定每次循环里源指针增加的步长？或者说，每次循环里如何更新源指针的位置？容易看出，通过浮点运算很容易解决这个问题：设立一个初值为零的浮点变量，每次循环中，把这个浮点变量加上N1/N2，并将其结果的整数部分作为源指针距离起始位置的偏移量；这样，经过N2次循环，浮点变量的值恰好达到N1，源指针也恰好“走”到原像素列的末尾。
　　这个方法可行，但是太慢。如果能将浮点运算转化成整数运算就快多了。那么如何转化呢？我们可以设立一个值域为N1*N2的整数计数器，每次循环递增N1，并且规定，计数器每增加N2，源指针就前进一个像素。这样，经过N2次循环，计数器共增加了N1*N2，源指针则增加了N1个单元，恰好“走”完全程。实际编程中，我们是用一个值域为N2的整数计数器，超出值域部分取模处理。算法大致如下：

　　void StrechPixels(int N1, int N2, PIXEL src_pixels[], PIXEL dest_pixels[])
　　{
　　　　ASSERT(N1 <= N2);　　// N1 must <= N2
　　　　int p1 = 0, count = 0;
　　　　for (int p2 = 0; p2 < N2; p2++)
　　　　{
　　　　　　dest_pixels[p2] = src_pixels[p1];
　　　　　　count += N1;
　　　　　　while (count >= N2)
　　　　　　{
　　　　　　　　count -= N2;
　　　　　　　　p1++;
　　　　　　}
　　　　}
　　}

　　上面算法只是水平缩放单行像素，对垂直方向也采用同样的算法，便实现了任意比例的位图缩放。经过以上算法的处理，放大时图像出现马赛克，缩小时图像出现闪砾。若要获得高质量的缩放图形，须采用插值、过滤等技术。但是因为这些技术所需计算量太大，在游戏中通常靠硬件加速来实现，不宜软件解决。  

================================================================
PSD格式的开发 

　　我在做游戏时，因为要用到将PSD格式转换成BMP或者JPG格式的程序，而且，在转换时，要将PSD中的空格转成游戏中约定的透明色，这样的程序，只能自己去写了。所以，我在网上搜了一阵子，找到了“中国游戏开发者”的网站，看到了一篇关于PSD格式的文章（这也是我开始向这个网站投稿的原因，也许这就叫缘）。
　　本来是想偷点懒，省去了自己研究之苦，可以抄一抄别人现在的代码，再自己改改，又能省时间，又能学到东西，何乐而不为呢？可是，在抄下这篇文章的代码之后，发现其运行居然是不能通过的。看来天下没有免费的午餐，我还是得自己研究。一个多小时的苦战之后，终于发现了问题所在，也许这并不是一个问题，只是对于没有这个经验人来说，这确实是个大问题。我现在将这篇文章的一些地方进行改正，望各位朋友在开发PSD格式的读取问题上，不再有麻烦。原文《PSD格式文件的读取》在这里http://cgd.pages.com.cn/cgd/develop/effect/200109/ReadPSD.htm，各位可以看看。我只将我的改正部分写在下面： 

　　1）文件头是4个字节，只能读4个字节。
　　2）Photoshop的PSD格式用的是LIT格式存储。

　　这个LIT格式我以前只是听说过，没想到会被PSD用上。这个格式是将数据的高低位码交换了的，如果直接用ReadFile或者fread函数将其读出来，它的高低位码是被交换了的。例如：640的16进制值是1E0，用DWORD方式存在硬盘里是0001E000，用读文件的函数读出来以后，将变成：00E00100。所以，其高低位码被交换了，解决的方法是用转换函数，代码如下：

　　WORD WORDBIGtoLIT(WORD code)　// 字型的处理
　　{
　　　　return ((a >> 8 & 0xFF) | ((a & 0x00FF) << 8);　// 把高低位码再交换过来
　　}

　　DWORD DWORDBIGtoLIT(DWORD code)　// 双字型的处理
　　{
　　　　WORD HiCode, LowCode;

　　　　HiCode = code & 0xFFFF0000;
　　　　LowCode = code & 0x0000FFFF;
　　　　HiCode = ((HiCode >> 8) & 0xFF) | ((HiCode & 0x00FF) << 8);
　　　　LowCode ((LowCode >> 8) & 0xFF) | ((LowCode & 0x00FF) << 8);
　　　　return MAKELONG((WORD)(LowCode << 16), (WORD)HiCode);
　　}

　　当然，也可以定义成宏形式，如下：

　　#define BIG2LIT(a) (WORD((a >> 8 & 0xFF) | ((a & 0x00FF) << 8)))
　　#define DWORDBIG2LIT(b) MAKELONG(BIG2LIT(HIWORD(b)), BIG2LIT(LOWORD(b)))

　　看起来简单一些，哈哈……。其它的，原文没有什么错误，不过，我建议大家还是最好自己去解决问题，呵呵，因为有一句话说得很好：老师能教你读书写字，但是不能教你做天下文章。 

================================================================
PSD格式文件的读取 

　　PhotoShop，我想没有人会不知道吧。如今最新的版本是6.0，其图象文件*.PSD和5.5相比变化并不太大。以下我就介绍*.PSD文件的读取方法，并提供完整读取函数。其中：m_Rect为目标区域，m_lpDDS7为目标DirectDraw表面，m_pbAlphaMask为目标Aplha通告指针。Read16函数为从指定文件当前位置读取一个WORD，Read32函数为从指定文件当前位置读取一个DWORD。MAX_PSD_CHANNELS为24。以下就是*.PSD文件的读取方法，有兴趣的朋友可以继续深入研究，到时可别忘了发我一份。

　　HRESULT LoadPSD( LPSTR strFilename ) // 读取PSD文件
　　{
　　　　DWORD dwWidth, dwHeight; // 宽高
　　　　long lSurfWidth = m_Rect.right - m_Rect.left;
　　　　long lSurfHeight = m_Rect.bottom - m_Rect.top;
　　　　WORD CompressionType; // 压缩类型
　　　　HDC hDC;
　　　　FILE *fpPSD;
　　　　WORD ChannelCount; // 通道数

　　　　// 打开PSD文件
　　　　if ( ( fpPSD = fopen ( strFilename, "rb" ) ) == NULL ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 头四个字节为"8BPS"
　　　　char signature[5];
　　　　signature[0] = fgetc( fpPSD );
　　　　signature[1] = fgetc( fpPSD );
　　　　signature[2] = fgetc( fpPSD );
　　　　signature[3] = fgetc( fpPSD );
　　　　signature[4] = '/0';
　　　　if ( strcmp( signature,"8BPS" ) != 0 ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 版本必须为1
　　　　if ( Read16( fpPSD ) != 1 ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 跳过一些数据 (总是0)
　　　　Read32( fpPSD );
　　　　Read16( fpPSD );

　　　　// 读取通道数
　　　　ChannelCount = Read16( fpPSD );

　　　　// 确定至少有一个通道
　　　　if ( ( ChannelCount < 0 ) || ( ChannelCount > MAX_PSD_CHANNELS ) ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 读入宽和高
　　　　dwHeight = Read32( fpPSD );
　　　　dwWidth = Read32( fpPSD );
　　　　if ( dwWidth != ( DWORD )lSurfWidth || dwHeight != ( DWORD )lSurfHeight ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 只读入8位通道
　　　　if ( Read16( fpPSD ) != 8 ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 确定模式为RGB.
　　　　// 可能值：
　　　　// 0: 位图
　　　　// 1: 灰阶
　　　　// 2: 索引
　　　　// 3: RGB
　　　　// 4: CMYK
　　　　// 7: Multichannel
　　　　// 8: Duotone
　　　　// 9: Lab
　　　　if ( Read16( fpPSD ) != 3 ) {
　　　　　　return E_FAIL;
　　　　}

　　　　// 跳过数据（如调色板）
　　　　int ModeDataCount = Read32( fpPSD );
　　　　if ( ModeDataCount )
　　　　　　fseek( fpPSD, ModeDataCount, SEEK_CUR );

　　　　// 跳过数据（如：pen tool paths, etc）
　　　　int ResourceDataCount = Read32( fpPSD );
　　　　if ( ResourceDataCount )
　　　　　　fseek( fpPSD, ResourceDataCount, SEEK_CUR );

　　　　// 条过保留数据
　　　　int ReservedDataCount = Read32( fpPSD );
　　　　if ( ReservedDataCount )
　　　　　　fseek( fpPSD, ReservedDataCount, SEEK_CUR );

　　　　// 0: 非压缩
　　　　// 1: RLE压缩
　　　　CompressionType = Read16( fpPSD );
　　　　if ( CompressionType > 1 ) {
　　　　　　return E_FAIL;
　　　　}

　　　　BYTE* PSDPixels = new BYTE[ ( lSurfWidth * lSurfHeight ) * 4 ];

　　　　// 解包数据
　　　　UnPackPSD( fpPSD, lSurfWidth, lSurfHeight, PSDPixels, ChannelCount, CompressionType );

　　　　fclose( fpPSD );

　　　　// 复制信息
　　　　BITMAPINFO BitmapInfo;
　　　　ZeroMemory( &BitmapInfo, sizeof( BitmapInfo ) );
　　　　BitmapInfo.bmiHeader.biSize = sizeof( BitmapInfo.bmiHeader );
　　　　BitmapInfo.bmiHeader.biWidth = lSurfWidth;
　　　　BitmapInfo.bmiHeader.biHeight = -lSurfHeight;
　　　　BitmapInfo.bmiHeader.biPlanes = 1;
　　　　BitmapInfo.bmiHeader.biBitCount = 32;

　　　　m_lpDDS7->GetDC( &hDC );

　　　　int rc = StretchDIBits( hDC,
　　　　　　　　　　　　　　　　0,
　　　　　　　　　　　　　　　　0,
　　　　　　　　　　　　　　　　lSurfWidth,
　　　　　　　　　　　　　　　　lSurfHeight,
　　　　　　　　　　　　　　　　0,
　　　　　　　　　　　　　　　　0,
　　　　　　　　　　　　　　　　lSurfWidth,
　　　　　　　　　　　　　　　　lSurfHeight,
　　　　　　　　　　　　　　　　PSDPixels,
　　　　　　　　　　　　　　　　&BitmapInfo,
　　　　　　　　　　　　　　　　DIB_RGB_COLORS,
　　　　　　　　　　　　　　　　SRCCOPY );

　　　　m_lpDDS7->ReleaseDC( hDC );

　　　　if ( rc == GDI_ERROR ) {
　　　　　　H_ARRAY_DELETE( PSDPixels );

　　#ifdef _DEBUG
　　　　g_pHERR->OutDebugMsg( 3, H2DSERR_INVALID_PSD );
　　#endif
　　　　return E_FAIL;

　　　　}

　　　　// 是否读取Alpha混合通道
　　　　if( ChannelCount > 3 ) {
　　　　　　m_pbAlphaMask = new BYTE[ lSurfWidth * lSurfHeight ];

　　　　for ( int x = 0; x < lSurfWidth; x++ )
　　　　　　for ( int y = 0; y < lSurfHeight; y++ ) {
　　　　　　　　m_pbAlphaMask[ ( y * lSurfWidth ) + x ] =
　　　　　　　　　　　　　　 　PSDPixels[ ( ( ( y * lSurfHeight ) + x ) * 4 ) + 3 ];
　　　　　　}
　　　　}
　　　　else {
　　　　　　m_pbAlphaMask = NULL;
　　　　}

　　　　H_ARRAY_DELETE( PSDPixels );

　　　　return DD_OK;
　　}

　　// PSD文件解包

　　void CHades2DSurface::UnPackPSD( FILE *fp,　　　　　// fp为PSD文件指针，
　　　　　　　　　　　　　　　　　　 DWORD dwWidth,　　 // dwWidth、dwHeight为宽高，
　　　　　　　　　　　　　　　　　　 DWORD dwHeight,
　　　　　　　　　　　　　　　　　　 BYTE* pixels,　　　// pixels为解包目标指针，
　　　　　　　　　　　　　　　　　　 WORD ChannelCnt,　 // ChannelCnt为通道数，
　　　　　　　　　　　　　　　　　　 WORD Compression ) // Compression位压缩类型。 

　　{
　　　　int Default[4] = { 0, 0, 0, 255 };
　　　　int chn[4] = { 2, 1, 0, 3};
　　　　int PixelCount = dwWidth * dwHeight;

　　　　if ( Compression ) {
　　　　　　fseek( fp, dwHeight * ChannelCnt * 2, SEEK_CUR );

　　　　　　for ( int c = 0; c < 4; c++ ) {
　　　　　　　　int pn = 0;
　　　　　　　　int channel = chn[c];

　　　　　　　　if ( channel >= ChannelCnt ) {
　　　　　　　　　　for ( pn=0; pn < PixelCount ;pn++ ) {
　　　　　　　　　　　　pixels[ ( pn * 4 ) + channel ] = Default[ channel ];
　　　　　　　　　　}
　　　　　　　　}
　　　　　　　　else // 非压缩
　　　　　　　　{
　　　　　　　　　　int count = 0;
　　　　　　　　　　while( count < PixelCount ) {
　　　　　　　　　　　　int len = fgetc( fp );
　　　　　　　　　　　　if( len == 128 ) { }
　　　　　　　　　　　　else if ( len < 128 ) // 非RLE
　　　　　　　　　　　　{
　　　　　　　　　　　　　　len++;
　　　　　　　　　　　　　　count += len;
　　　　　　　　　　　　　　while(len) {
　　　　　　　　　　　　　　　　pixels[ ( pn * 4 ) + channel ] = fgetc( fp );
　　　　　　　　　　　　　　　　pn++;
　　　　　　　　　　　　　　　　len--;
　　　　　　　　　　　　　　}
　　　　　　　　　　　　}
　　　 　　　　　　　　 else if ( len > 128 ) // RLE打包
　　　　　　　　　　　　{
　　　　　　　　　　　　　　len ^= 0x0FF;
　　　　　　　　　　　　　　len += 2;
　　　　　　　　　　　　　　unsigned char val = fgetc( fp );
　　　　　　　　　　　　　　count += len;
　　　　　　　　　　　　　　while( len ) {
　　　　　　　　　　　　　　　　pixels[ ( pn * 4 ) + channel ] = val;
　　　　　　　　　　　　　　　　pn++;
　　　　　　　　　　　　　　　　len--;
　　　　　　　　　　　　　　}
　　　　　　　　　　　　}
　　　　　　　　　　}
　　　　　　　　}
　　　　　　}
　　　　}
　　　　else
　　　　{
　　　　　　for ( int c=0; c < 4; c++ ) {
　　　　　　　　int channel = chn[c];
　　　　　　　　if ( channel > ChannelCnt ) {
　　　　　　　　　　for( int pn = 0; pn < PixelCount; pn++ ) {
　　　　　　　　　　　　pixels[ ( pn * 4 ) + channel ] = Default[ channel ];
　　　　　　　　　　}
　　　　　　　　}
　　　　　　　　else {
　　　　　　　　　　for( int n = 0; n < PixelCount; n++ ) {
　　　　　　　　　　　　pixels[ ( n * 4 ) + channel ] = fgetc( fp );
　　　　　　　　　　}
　　　　　　　　}
　　　　　　}
　　　　}
　　}  

