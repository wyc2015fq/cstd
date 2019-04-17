# 读取*.wav音频文件 - xiahouzuoxin - CSDN博客





2012年08月16日 23:59:12[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：20370








1、wav音频文件的格式

wav文件由**文件头**和**采样数据**2部分组成。

文件头又分为RIFF（Resource Interchange File Format）、WAVE文件标识段 和 声音数据格式说明段组成。

各段的起始地址分别由RIFF标识符、WAVE标识符、以及波形格式标识符（FMT）标定。

（1）文件头格式

注意：下面的地址是连续的

![](https://img-my.csdn.net/uploads/201208/09/1344519517_8607.JPG)

（2）数据格式



![](https://img-my.csdn.net/uploads/201208/09/1344519531_6649.JPG)





虽然上图给出的数据标识符起始地址刚好是文件头的末地址+1，但并不代表总是这样。

因此，我们在读取数据时最好是找到数据标识符，该标识符的4个字节刚好是'd'、‘a’、‘t’、‘a’。

2、C语言读取wav文件

首先对一些类型使用了重定义

```cpp
typedef unsigned char  uchar;
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef char           sint8;
typedef short          sint16;
typedef long           sint32;
typedef float          fp32;
typedef double         fp64;

typedef enum BOOLEAN
{
	TRUE  = 1,
	FALSE = 0
} boolean;
```




（1）wav结构体定义

```cpp
//wave文件头
typedef struct WaveHeader
{
	uint8 riff[4];             //资源交换文件标志
	uint32 size;               //从下个地址开始到文件结尾的字节数
	uint8 wave_flag[4];        //wave文件标识
	uint8 fmt[4];              //波形格式标识
	uint32 fmt_len;            //过滤字节(一般为00000010H)
	uint16 tag;                //格式种类，值为1时，表示PCM线性编码
	uint16 channels;           //通道数，单声道为1，双声道为2
	uint32 samp_freq;          //采样频率
	uint32 byte_rate;          //数据传输率 (每秒字节＝采样频率×每个样本字节数)
	uint16 block_align;        //块对齐字节数 = channles * bit_samp / 8
	uint16 bit_samp;           //bits per sample (又称量化位数)
} wave_header_t;

typedef struct WaveStruct
{
	FILE *fp;                  //file pointer
	wave_header_t header;      //header
	uint8 data_flag[4];        //数据标识符
	uint32 length;             //采样数据总数
	uint32 *pData;             //data
} wave_t;
wave_t wave;
```




（2）读取文件头信息

```cpp
/*
 * open *.wav file
 */
void WaveOpen(char *file, int raw, int mono_stereo)
{
	uchar temp = 0;
	uint8 read_bytes = 0;
	char *channel_mappings[] = {NULL,"mono","stereo"};
	uint32 total_time = 0;
	struct PlayTime        //播放时间
	{
		uint8 hour;
		uint8 minute;
		uint8 second;
	} play_time;

	if(NULL == (wave.fp=fopen(file, "rb")))                               /* open file */
	{
		printf("file %s open failure!\n", file);	
	}

	/* read heade information */
	if(4 != fread(wave.header.riff, sizeof(uint8), 4, wave.fp))           /* RIFF chunk */
	{
		printf("read riff error!\n");
		return;
	}
	if(1 != fread(&wave.header.size, sizeof(uint32), 1, wave.fp))         /* SIZE : from here to file end */
	{
		printf("read size error!\n");
		return;
	}
	if(4 != fread(wave.header.wave_flag, sizeof(uint8), 4, wave.fp))      /* wave file flag */
	{
		printf("read wave_flag error!\n");
		return;
	}
	if(4 != fread(wave.header.fmt, sizeof(uint8), 4, wave.fp))             /* fmt chunk */
	{
		printf("read fmt error!\n");
		return;
	}
	if(1 != fread(&wave.header.fmt_len, sizeof(uint32), 1, wave.fp))       /* fmt length */
	{
		printf("read fmt_len error!\n");
		return;
	}
	if(1 != fread(&wave.header.tag, sizeof(uint16), 1, wave.fp))           /* tag : PCM or not */
	{
		printf("read tag error!\n");
		return;
	}
	if(1 != fread(&wave.header.channels, sizeof(uint16), 1, wave.fp))      /* channels */
	{
		printf("read channels error!\n");
		return;
	}
	if(1 != fread(&wave.header.samp_freq, sizeof(uint32), 1, wave.fp))      /* samp_freq */
	{
		printf("read samp_freq error!\n");
		return;
	}
	if(1 != fread(&wave.header.byte_rate, sizeof(uint32), 1, wave.fp))      /* byte_rate : decode how many bytes per second */
	{                                                                       /* byte_rate = samp_freq * bit_samp */
		printf("read byte_rate error!\n");
		return;
	}
	if(1 != fread(&wave.header.block_align, sizeof(uint16), 1, wave.fp))       /* quantize bytes for per samp point */
	{
		printf("read byte_samp error!\n");
		return;
	}
	if(1 != fread(&wave.header.bit_samp, sizeof(uint16), 1, wave.fp))        /* quantize bits for per samp point */
	{                                                                        /* bit_samp = byte_samp * 8 */
		printf("read bit_samp error!\n");
		return;
	}

	/* jump to "data" for reading data */
	do
	{
		fread(&temp, sizeof(uchar), 1, wave.fp);
	}
	while('d' != temp);
	wave.data_flag[0] = temp;
	if(3 != fread(&wave.data_flag[1], sizeof(uint8), 3, wave.fp))                 /* data chunk */
	{
		printf("read header data error!\n");
		return;
	}
	if(1 != fread(&wave.length, sizeof(uint32), 1, wave.fp))                  /* data length */
	{
		printf("read length error!\n");
	}

	/* jduge data chunk flag */
	if(!StrCmp(wave.data_flag, "data", 4))
	{
		printf("error : cannot read data!\n");
		return;
	}

	total_time = wave.length / wave.header.byte_rate;
	play_time.hour = (uint8)(total_time / 3600);
	play_time.minute = (uint8)((total_time / 60) % 60);
	play_time.second = (uint8)(total_time % 60);
	/* printf file header information */
	printf("%s %ldHz %dbit, DataLen: %ld, Rate: %ld, Length: %2ld:%2ld:%2ld\n",
		   channel_mappings[wave.header.channels],             //声道
		   wave.header.samp_freq,                              //采样频率
		   wave.header.bit_samp,                               //每个采样点的量化位数
           wave.length,
		   wave.header.byte_rate,
		   play_time.hour,play_time.minute,play_time.second);


	//fclose(wave.fp);                                               /* close wave file */
}
```


按结构体一点点的读出文件头的信息，请注意 

/* jump to "data" for reading data */

 的那一段，“先识别data标识符，再接着往下读取”。



（3）读数据

在读完数据长度之后就全是数据了，直接使用fread按uint32格式读取数据即可，我这里每次读取1152个数据（即一帧）。

```cpp
/*
 * get wave data 
 */
uint32* GetWave(void)
{
	static uint32 buffer[1152] = {0};	
	uint16 n = 1152;
	uint16 p = 0;

	p = fread(buffer, sizeof(uint32), n, wave.fp);
	if(!p)
	{
		return 0;
	}
	else
	{
		for(; p<n; p++)
		{
			buffer[p] = 0;
		}
		return buffer;
	}
}
```

上面程序中注意几点，

（1）不要定义大容量的局部变量，因为局部变量存放在堆栈中。如果一定要定义，要定义成static类型。

（2）不要返回局部变量的的地址，因为在堆栈中的地址值是不确定的。

（3）对于for(; p<n; p++）指当读出的数据不满一帧的时候，在后面补0到一帧，一般这样方便后期处理

上面的程序返回局部数组的指针，前提是 已经将数据存放在静态数据存储区。

但不管怎样，返回局部变量的地址总是不好的。







