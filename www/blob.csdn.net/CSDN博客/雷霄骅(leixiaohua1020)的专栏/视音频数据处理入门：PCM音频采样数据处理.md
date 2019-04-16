# 视音频数据处理入门：PCM音频采样数据处理 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2016年01月29日 23:32:13[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：91521标签：[PCM																[音频																[声道																[WAV](https://so.csdn.net/so/search/s.do?q=WAV&t=blog)
个人分类：[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)







=====================================================

视音频数据处理入门系列文章：

[视音频数据处理入门：RGB、YUV像素数据处理](http://blog.csdn.net/leixiaohua1020/article/details/50534150)

[视音频数据处理入门：PCM音频采样数据处理](http://blog.csdn.net/leixiaohua1020/article/details/50534316)

[视音频数据处理入门：H.264视频码流解析](http://blog.csdn.net/leixiaohua1020/article/details/50534369)

[视音频数据处理入门：AAC音频码流解析](http://blog.csdn.net/leixiaohua1020/article/details/50535042)

[视音频数据处理入门：FLV封装格式解析](http://blog.csdn.net/leixiaohua1020/article/details/50535082)

[视音频数据处理入门：UDP-RTP协议解析](http://blog.csdn.net/leixiaohua1020/article/details/50535230)

=====================================================


上一篇文章记录了RGB/YUV视频像素数据的处理方法，本文继续上一篇文章的内容，记录PCM音频采样数据的处理方法。音频采样数据在视频播放器的解码流程中的位置如下图所示。

![](https://img-blog.csdn.net/20160117235313221)

本文分别介绍如下几个PCM音频采样数据处理函数：
分离PCM16LE双声道音频采样数据的左声道和右声道
将PCM16LE双声道音频采样数据中左声道的音量降一半
将PCM16LE双声道音频采样数据的声音速度提高一倍
将PCM16LE双声道音频采样数据转换为PCM8音频采样数据
从PCM16LE单声道音频采样数据中截取一部分数据
将PCM16LE双声道音频采样数据转换为WAVE格式音频数据

注：PCM音频数据可以使用音频编辑软件导入查看。例如收费的专业音频编辑软件[Adobe Audition](http://www.adobe.com/products/audition.html)，或者免费开源的音频编辑软件[Audacity](http://audacityteam.org/)。




## 函数列表


### （1）分离PCM16LE双声道音频采样数据的左声道和右声道
本程序中的函数可以将PCM16LE双声道数据中左声道和右声道的数据分离成两个文件。函数的代码如下所示。

```cpp
/**
 * Split Left and Right channel of 16LE PCM file.
 * @param url  Location of PCM file.
 *
 */
int simplest_pcm16le_split(char *url){
	FILE *fp=fopen(url,"rb+");
	FILE *fp1=fopen("output_l.pcm","wb+");
	FILE *fp2=fopen("output_r.pcm","wb+");

	unsigned char *sample=(unsigned char *)malloc(4);

	while(!feof(fp)){
		fread(sample,1,4,fp);
		//L
		fwrite(sample,1,2,fp1);
		//R
		fwrite(sample+2,1,2,fp2);
	}

	free(sample);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	return 0;
}
```

调用上面函数的方法如下所示。

```cpp
simplest_pcm16le_split("NocturneNo2inEflat_44.1k_s16le.pcm");
```


从代码可以看出，PCM16LE双声道数据中左声道和右声道的采样值是间隔存储的。每个采样值占用2Byte空间。代码运行后，会把NocturneNo2inEflat_44.1k_s16le.pcm的PCM16LE格式的数据分离为两个单声道数据：

> output_l.pcm：左声道数据。
output_r.pcm：右声道数据。

注：本文中声音样值的采样频率一律是44100Hz，采样格式一律为16LE。“16”代表采样位数是16bit。由于1Byte=8bit，所以一个声道的一个采样值占用2Byte。“LE”代表Little Endian，代表2 Byte采样值的存储方式为高位存在高地址中。


下图为输入的双声道PCM数据的波形图。上面的波形图是左声道的图形，下面的波形图是右声道的波形。图中的横坐标是时间，总长度为22秒；纵坐标是取样值，取值范围从-32768到32767。

![](https://img-blog.csdn.net/20160117235701134)


下图为分离后左声道数据output_l.pcm的音频波形图。

![](https://img-blog.csdn.net/20160117235715074)

下图为分离后右声道数据output_r.pcm的音频波形图。

![](https://img-blog.csdn.net/20160117235727770)

### （2）将PCM16LE双声道音频采样数据中左声道的音量降一半
本程序中的函数可以将PCM16LE双声道数据中左声道的音量降低一半。函数的代码如下所示。

```cpp
/**
 * Halve volume of Left channel of 16LE PCM file
 * @param url  Location of PCM file.
 */
int simplest_pcm16le_halfvolumeleft(char *url){
	FILE *fp=fopen(url,"rb+");
	FILE *fp1=fopen("output_halfleft.pcm","wb+");

	int cnt=0;

	unsigned char *sample=(unsigned char *)malloc(4);

	while(!feof(fp)){
		short *samplenum=NULL;
		fread(sample,1,4,fp);

		samplenum=(short *)sample;
		*samplenum=*samplenum/2;
		//L
		fwrite(sample,1,2,fp1);
		//R
		fwrite(sample+2,1,2,fp1);

		cnt++;
	}
	printf("Sample Cnt:%d\n",cnt);

	free(sample);
	fclose(fp);
	fclose(fp1);
	return 0;
}
```

调用上面函数的方法如下所示。

```cpp
simplest_pcm16le_halfvolumeleft("NocturneNo2inEflat_44.1k_s16le.pcm");
```


从源代码可以看出，本程序在读出左声道的2 Byte的取样值之后，将其当成了C语言中的一个short类型的变量。将该数值除以2之后写回到了PCM文件中。下图为输入PCM双声道音频采样数据的波形图。

![](https://img-blog.csdn.net/20160118000021219)


下图为输出的左声道经过处理后的波形图。可以看出左声道的波形幅度降低了一半。

![](https://img-blog.csdn.net/20160118000051676)



### （3）将PCM16LE双声道音频采样数据的声音速度提高一倍
本程序中的函数可以通过抽象的方式将PCM16LE双声道数据的速度提高一倍。函数的代码如下所示。

```cpp
/**
 * Re-sample to double the speed of 16LE PCM file
 * @param url  Location of PCM file.
 */
int simplest_pcm16le_doublespeed(char *url){
	FILE *fp=fopen(url,"rb+");
	FILE *fp1=fopen("output_doublespeed.pcm","wb+");

	int cnt=0;

	unsigned char *sample=(unsigned char *)malloc(4);

	while(!feof(fp)){

		fread(sample,1,4,fp);

		if(cnt%2!=0){
			//L
			fwrite(sample,1,2,fp1);
			//R
			fwrite(sample+2,1,2,fp1);
		}
		cnt++;
	}
	printf("Sample Cnt:%d\n",cnt);

	free(sample);
	fclose(fp);
	fclose(fp1);
	return 0;
}
```

调用上面函数的方法如下所示。

```cpp
simplest_pcm16le_doublespeed("NocturneNo2inEflat_44.1k_s16le.pcm");
```


从源代码可以看出，本程序只采样了每个声道奇数点的样值。处理完成后，原本22秒左右的音频变成了11秒左右。音频的播放速度提高了2倍，音频的音调也变高了很多。下图为输入PCM双声道音频采样数据的波形图。

![](https://img-blog.csdn.net/20160118000214030)


下图为输出的PCM双声道音频采样数据的波形图。通过时间轴可以看出音频变短了很多。

![](https://img-blog.csdn.net/20160118000237272)



### （4）将PCM16LE双声道音频采样数据转换为PCM8音频采样数据

本程序中的函数可以通过计算的方式将PCM16LE双声道数据16bit的采样位数转换为8bit。函数的代码如下所示。



```cpp
/**
 * Convert PCM-16 data to PCM-8 data.
 * @param url  Location of PCM file.
 */
int simplest_pcm16le_to_pcm8(char *url){
	FILE *fp=fopen(url,"rb+");
	FILE *fp1=fopen("output_8.pcm","wb+");

	int cnt=0;

	unsigned char *sample=(unsigned char *)malloc(4);

	while(!feof(fp)){

		short *samplenum16=NULL;
		char samplenum8=0;
		unsigned char samplenum8_u=0;
		fread(sample,1,4,fp);
		//(-32768-32767)
		samplenum16=(short *)sample;
		samplenum8=(*samplenum16)>>8;
		//(0-255)
		samplenum8_u=samplenum8+128;
		//L
		fwrite(&samplenum8_u,1,1,fp1);

		samplenum16=(short *)(sample+2);
		samplenum8=(*samplenum16)>>8;
		samplenum8_u=samplenum8+128;
		//R
		fwrite(&samplenum8_u,1,1,fp1);
		cnt++;
	}
	printf("Sample Cnt:%d\n",cnt);

	free(sample);
	fclose(fp);
	fclose(fp1);
	return 0;
}
```



调用上面函数的方法如下所示。

```cpp
simplest_pcm16le_to_pcm8("NocturneNo2inEflat_44.1k_s16le.pcm");
```


PCM16LE格式的采样数据的取值范围是-32768到32767，而PCM8格式的采样数据的取值范围是0到255。所以PCM16LE转换到PCM8需要经过两个步骤：第一步是将-32768到32767的16bit有符号数值转换为-128到127的8bit有符号数值，第二步是将-128到127的8bit有符号数值转换为0到255的8bit无符号数值。在本程序中，16bit采样数据是通过short类型变量存储的，而8bit采样数据是通过unsigned char类型存储的。下图为输入的16bit的PCM双声道音频采样数据的波形图。

![](https://img-blog.csdn.net/20160118000354979)

下图为输出的8bit的PCM双声道音频采样数据的波形图。注意观察图中纵坐标的取值范围已经变为0至255。如果仔细聆听声音的话，会发现8bit PCM的音质明显不如16 bit PCM的音质。

![](https://img-blog.csdn.net/20160118000339516)


### （5）将从PCM16LE单声道音频采样数据中截取一部分数据
本程序中的函数可以从PCM16LE单声道数据中截取一段数据，并输出截取数据的样值。函数的代码如下所示。

```cpp
/**
 * Cut a 16LE PCM single channel file.
 * @param url        Location of PCM file.
 * @param start_num  start point
 * @param dur_num    how much point to cut
 */
int simplest_pcm16le_cut_singlechannel(char *url,int start_num,int dur_num){
	FILE *fp=fopen(url,"rb+");
	FILE *fp1=fopen("output_cut.pcm","wb+");
	FILE *fp_stat=fopen("output_cut.txt","wb+");

	unsigned char *sample=(unsigned char *)malloc(2);

	int cnt=0;
	while(!feof(fp)){
		fread(sample,1,2,fp);
		if(cnt>start_num&&cnt<=(start_num+dur_num)){
			fwrite(sample,1,2,fp1);

			short samplenum=sample[1];
			samplenum=samplenum*256;
			samplenum=samplenum+sample[0];

			fprintf(fp_stat,"%6d,",samplenum);
			if(cnt%10==0)
				fprintf(fp_stat,"\n",samplenum);
		}
		cnt++;
	}

	free(sample);
	fclose(fp);
	fclose(fp1);
	fclose(fp_stat);
	return 0;
}
```

调用上面函数的方法如下所示。

```cpp
simplest_pcm16le_cut_singlechannel("drum.pcm",2360,120);
```


本程序可以从PCM数据中选取一段采样值保存下来，并且输出这些采样值的数值。上述代码运行后，会把单声道PCM16LE格式的“drum.pcm”中从2360点开始的120点的数据保存成output_cut.pcm文件。下图为“drum.pcm”的波形图，该音频采样频率为44100KHz，长度为0.5秒，一共包含约22050个采样点。

![](https://img-blog.csdn.net/20160118000517972)


下图为截取出来的output_cut.pcm文件中的数据。

![](https://img-blog.csdn.net/20160118000532877)

下面列出了上述数据的采样值。

```
4460,  5192,  5956,  6680,  7199,  6706,  5727,  4481,  3261,  1993,
  1264,   747,   767,   752,  1248,  1975,  2473,  2955,  2952,  2447,
   974, -1267, -4000, -6965,-10210,-13414,-16639,-19363,-21329,-22541,
-23028,-22545,-21055,-19067,-16829,-14859,-12596, -9900, -6684, -3475,
  -983,  1733,  3978,  5734,  6720,  6978,  6993,  7223,  7225,  7440,
  7688,  8431,  8944,  9468,  9947, 10688, 11194, 11946, 12449, 12446,
 12456, 11974, 11454, 10952, 10167,  9425,  8153,  6941,  5436,  3716,
  1952,   236, -1254, -2463, -3493, -4223, -4695, -4927, -5190, -4941,
 -4188, -2956, -1490,   -40,   705,   932,   446,  -776, -2512, -3994,
 -5723, -7201, -8687,-10157,-11134,-11661,-11642,-11168,-10155, -9142,
 -7888, -7146, -6186, -5694, -4971, -4715, -4498, -4471, -4468, -4452,
 -4452, -3940, -2980, -1984,  -752,   257,  1021,  1264,  1032,    31,
```


### （6）将PCM16LE双声道音频采样数据转换为WAVE格式音频数据
WAVE格式音频（扩展名为“.wav”）是Windows系统中最常见的一种音频。该格式的实质就是在PCM文件的前面加了一个文件头。本程序的函数就可以通过在PCM文件前面加一个WAVE文件头从而封装为WAVE格式音频。函数的代码如下所示。

```cpp
/**
 * Convert PCM16LE raw data to WAVE format
 * @param pcmpath      Input PCM file.
 * @param channels     Channel number of PCM file.
 * @param sample_rate  Sample rate of PCM file.
 * @param wavepath     Output WAVE file.
 */
int simplest_pcm16le_to_wave(const char *pcmpath,int channels,int sample_rate,const char *wavepath)
{

	typedef struct WAVE_HEADER{  
		char         fccID[4];        
		unsigned   long    dwSize;            
		char         fccType[4];    
	}WAVE_HEADER;  

	typedef struct WAVE_FMT{  
		char         fccID[4];        
		unsigned   long       dwSize;            
		unsigned   short     wFormatTag;    
		unsigned   short     wChannels;  
		unsigned   long       dwSamplesPerSec;  
		unsigned   long       dwAvgBytesPerSec;  
		unsigned   short     wBlockAlign;  
		unsigned   short     uiBitsPerSample;  
	}WAVE_FMT;  

	typedef struct WAVE_DATA{  
		char       fccID[4];          
		unsigned long dwSize;              
	}WAVE_DATA;  


	if(channels==0||sample_rate==0){
    channels = 2;
    sample_rate = 44100;
	}
	int bits = 16;

    WAVE_HEADER   pcmHEADER;  
    WAVE_FMT   pcmFMT;  
    WAVE_DATA   pcmDATA;  
 
    unsigned   short   m_pcmData;
    FILE   *fp,*fpout;  

	fp=fopen(pcmpath, "rb");
    if(fp == NULL) {  
        printf("open pcm file error\n");
        return -1;  
    }
	fpout=fopen(wavepath,   "wb+");
    if(fpout == NULL) {    
        printf("create wav file error\n");  
        return -1; 
    }        
	//WAVE_HEADER
    memcpy(pcmHEADER.fccID,"RIFF",strlen("RIFF"));                    
    memcpy(pcmHEADER.fccType,"WAVE",strlen("WAVE"));  
    fseek(fpout,sizeof(WAVE_HEADER),1); 
	//WAVE_FMT
    pcmFMT.dwSamplesPerSec=sample_rate;  
    pcmFMT.dwAvgBytesPerSec=pcmFMT.dwSamplesPerSec*sizeof(m_pcmData);  
    pcmFMT.uiBitsPerSample=bits;
    memcpy(pcmFMT.fccID,"fmt ",strlen("fmt "));  
    pcmFMT.dwSize=16;  
    pcmFMT.wBlockAlign=2;  
    pcmFMT.wChannels=channels;  
    pcmFMT.wFormatTag=1;  
 
    fwrite(&pcmFMT,sizeof(WAVE_FMT),1,fpout); 

    //WAVE_DATA;
    memcpy(pcmDATA.fccID,"data",strlen("data"));  
    pcmDATA.dwSize=0;
    fseek(fpout,sizeof(WAVE_DATA),SEEK_CUR);

    fread(&m_pcmData,sizeof(unsigned short),1,fp);
    while(!feof(fp)){  
        pcmDATA.dwSize+=2;
        fwrite(&m_pcmData,sizeof(unsigned short),1,fpout);
        fread(&m_pcmData,sizeof(unsigned short),1,fp);
    }  

    pcmHEADER.dwSize=44+pcmDATA.dwSize;

    rewind(fpout);
    fwrite(&pcmHEADER,sizeof(WAVE_HEADER),1,fpout);
    fseek(fpout,sizeof(WAVE_FMT),SEEK_CUR);
    fwrite(&pcmDATA,sizeof(WAVE_DATA),1,fpout);
	
	fclose(fp);
    fclose(fpout);

    return 0;
}
```

调用上面函数的方法如下所示。

```cpp
simplest_pcm16le_to_wave("NocturneNo2inEflat_44.1k_s16le.pcm",2,44100,"output_nocturne.wav");
```

WAVE文件是一种RIFF格式的文件。其基本块名称是“WAVE”，其中包含了两个子块“fmt”和“data”。从编程的角度简单说来就是由WAVE_HEADER、WAVE_FMT、WAVE_DATA、采样数据共4个部分组成。它的结构如下所示。


|WAVE_HEADER|
|----|
|WAVE_FMT|
|WAVE_DATA|
|PCM数据|



其中前3部分的结构如下所示。在写入WAVE文件头的时候给其中的每个字段赋上合适的值就可以了。但是有一点需要注意：WAVE_HEADER和WAVE_DATA中包含了一个文件长度信息的dwSize字段，该字段的值必须在写入完音频采样数据之后才能获得。因此这两个结构体最后才写入WAVE文件中。

```cpp
typedef struct WAVE_HEADER{
		char fccID[4];
		unsigned long dwSize;
		char fccType[4];
	}WAVE_HEADER;

	typedef struct WAVE_FMT{
		char  fccID[4];
		unsigned long dwSize;
		unsigned short wFormatTag;
		unsigned short wChannels;
		unsigned long dwSamplesPerSec;
		unsigned long dwAvgBytesPerSec;
		unsigned short wBlockAlign;
		unsigned short uiBitsPerSample;
	}WAVE_FMT;

	typedef struct WAVE_DATA{
		char       fccID[4];
		unsigned long dwSize;
	}WAVE_DATA;
```

本程序的函数执行完成后，就可将NocturneNo2inEflat_44.1k_s16le.pcm文件封装成output_nocturne.wav文件。



## 下载

**Simplest mediadata test**


**项目主页**

SourceForge：[https://sourceforge.net/projects/simplest-mediadata-test/](https://sourceforge.net/projects/simplest-mediadata-test/)

Github：[https://github.com/leixiaohua1020/simplest_mediadata_test](https://github.com/leixiaohua1020/simplest_mediadata_test)
开源中国：[http://git.oschina.net/leixiaohua1020/simplest_mediadata_test](http://git.oschina.net/leixiaohua1020/simplest_mediadata_test)



CSDN下载地址：[http://download.csdn.net/detail/leixiaohua1020/9422409](http://download.csdn.net/detail/leixiaohua1020/9422409)



本项目包含如下几种视音频数据解析示例：
 (1)像素数据处理程序。包含RGB和YUV像素格式处理的函数。
 (2)音频采样数据处理程序。包含PCM音频采样格式处理的函数。
 (3)H.264码流分析程序。可以分离并解析NALU。
 (4)AAC码流分析程序。可以分离并解析ADTS帧。
 (5)FLV封装格式分析程序。可以将FLV中的MP3音频码流分离出来。

 (6)UDP-RTP协议分析程序。可以将分析UDP/RTP/MPEG-TS数据包。




**雷霄骅 (Lei Xiaohua)leixiaohua1020@126.comhttp://blog.csdn.net/leixiaohua1020**](https://so.csdn.net/so/search/s.do?q=声道&t=blog)](https://so.csdn.net/so/search/s.do?q=音频&t=blog)](https://so.csdn.net/so/search/s.do?q=PCM&t=blog)




