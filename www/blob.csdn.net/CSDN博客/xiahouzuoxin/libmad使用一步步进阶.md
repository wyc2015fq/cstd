# libmad使用一步步进阶 - xiahouzuoxin - CSDN博客





2012年08月15日 00:14:19[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4608








原文地址：[http://my.oschina.net/mjRao/blog/57874](http://my.oschina.net/mjRao/blog/57874)

libmad是一个开源mp3解码库，其对mp3解码算法做了很多优化，性能较好，很多播放器如mplayer、xmms等都是使用这个开源库进行解码的；如果要设计mp3播放器而又不想研究mp3解码算法的话，libmad是个不错的选择，可是问题来了：
- libmad配套的相关文档太少，可以说几乎没有，只有一个示例程序minimad.c，但没有一定经验的人根本不知道怎么编译这个minimad.c，就算是编译了也不知道怎么运行、怎么播放mp3；
- 网上讲libmad和minimad.c的文章很多，但能解释清楚的少之又少，大家都是抄来抄去，要么是不懂装懂，要么是懂了一点就自以为精通了，这样一来的结果是：在网上搜两天也弄不明白libmad究竟怎么使用。

所幸手里有Altera公司的一个工程，借助对该工程的分析、minimad.c中少的可怜的注释和网上搜索的Linux音频方面的相关知识，反复思考编码，总算把libmad库用起来了，现记录一下其使用方法，在帮助别人的同时也方便自己回头查询。 在开始之前，最好先把mp3文件格式和Linux音频编程方面的知识先学习一下，不然后面有的东西可能听不懂，还有就是一定要熟悉Linux系统，后面的代码都是在linux系统中用gcc编译的，在Windows下不能用的。 首先看下面几个问题，这也是我一开始最迷惑的，弄明白这几个问题了，也就对libmad库的使用相当熟悉了：
- minimad.c怎么编译？编译后怎么运行？运行时的输入输出分别是什么，或者说运行时什么效果？
- 怎样播放minimad输出的数据？或者说怎么播放解码后的数据？
- minimad运行时，mp3数据来源是标准输入，能不能改为从文件中读入数据？该怎么改？
- minimad运行时首先要将整个mp3文件读入内存，能不能改成边解码边读入的形式，比如每次读入16K，解码完再读入16K，而又不影响播放的连贯性，这样可以节省内存开销，方便在嵌入式系统中使用；
- 怎样用libmad做一个简单的mp3播放器?

一个一个来讲吧。

minimad.c怎么编译？编译后怎么运行？运行时的输入输出分别是什么，或者说运行时什么效果？  在Linux下(我前面说了，本文所有的工作都是在Linux进行)先安装libmad，说白了就是把libmad库导入C标准库，安装方法见libmad-0.15.1b中的README和INSTALL文件。 安装libmad后，新建一个文件夹，将libmad-0.15.1b中的minimad.c和mad.h复制过来，用gcc编译minimad.c，编译命令为(假设要生成的可执行程序为minimad)： 
```
gcc
 -o minimad minimad.c -lmad
```
 minimad程序从标准输入读入mp3文件，然后将解码后的音频数据送到标准输出，我们可以用重定向的方式从文件中读入数据并将结果写至文件，命令如下： `./minimad tmp.pcm`怎样播放minimad输出的数据？或者说怎么播放解码后的数据？  假设你有Linux音频编程方面的基础的话，这个应该不成问题，如果没有也没关系，在Linux的设计理念中，一切皆是文件，音频设备也是文件，只需要打 开/dev/dsp（音频设备)这个文件，然后将解码后的数据写入这个文件即可实现播放，新建pcmplay.c文件，拷入如下代码： 

```
#include 
#include 
#include 
#include 
#include 
#include 

int main(int argc, char *argv[])
{
    int  id, fd, i;
    char buf[1024];
    int  rate;      /*simple rate 44.1KHz*/
    int  format;    /*quatize args*/
    int  channels;  /*sound channel*/

    if(argc != 2)
    {
        fprintf(stderr, "usage : %s \n", argv[0]);
        exit(-1);
    }

    if((fd = open(argv[1], O_RDONLY)) < 0)
    {
        fprintf(stderr, "Can't open sound file!\n");
        exit(-2);
    }

    if((id = open("/dev/dsp", O_WRONLY)) 0)
    {
        write(id, buf, i);
        //printf("i=%d\n", i);
    }

    close(fd);
    close(id);

    exit(0);

}
```

编译pcmplay文件，然后就可以用生成的可执行程序播放第一步中声称的tmp.pcm文件，命令如下： 

```
gcc -o pcmplay pcmplay.c
./minimad tmp.pcm
./pcmplay  tmp.pcm
```

播放时可能会变调，这是因为上面这段代码中将音频设备采样率固定设置为44.1k，而mp3文件不一定是这个采样率，解决方法后面会讲。  minimad运行时，mp3数据来源是标准输入，能不能改为从文件中读入数据？该怎么改？ 当然可以改，而且改起来相当的简单，如果不知道怎么改只能说明自己没仔细看minimad.c，你可能不知道struct stat是什么，也不清楚mmap()函数有什么用，但这些都可以在网上查到的，查了之后稍加分析就会发现原来就是把一片数据放入一块内存并得到它的长度 而已，那改成文件读入的方式也很容易，用fopen打开文件，计算一下文件的长度，然后用fread把数据全部读出来即可，这里就不贴代码了。 minimad运行时首先要将整个mp3文件读入内存，能不能改成边解码边读入的形式，比如每次读入16K，解码完再读入16K，而又不影响播放的连贯性，这样可以节省内存开销，方便在嵌入式系统中使用； 修改input()函数，在调用libmad中的mad_decoder_run()实现播放时，首先检查待解码缓存区中有没有数据，有则 解码，没有则调用input()函数一次以填充数据(填充多少可以自己指定)，然后开始解码，解码后的数据交给output()函数处理，解码过程中，一 旦待解码缓存区中的解码数据不够则再次调用input()函数……
 在这里还要提一下struct buffer这个结构体，这个结构体是在input、output和decoder之间传送数据的载体，可以自行定义，比如我的数据来源是文件，待解码数 据缓存区大小为4K，要传递的私有数据包括文件指针、当前的位置、数据缓冲区、缓冲区的实际大小、文件的总大小等，则我这里定义如下： 

```
struct buffer {
  FILE  *fp;                    /*file pointer*/
  unsigned int  flen;           /*file length*/
  unsigned int  fpos;           /*current position*/
  unsigned char fbuf[BUFSIZE];  /*buffer*/
  unsigned int  fbsize;         /*indeed size of buffer*/
};
typedef struct buffer mp3_file;
```

修改input()函数为如下形式，则每次调用填充BUFSIZE字节的数据： 

```
static
enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  mp3_file *mp3fp;
  int      ret_code;
  int      unproc_data_size;    /*the unprocessed data's size*/
  int      copy_size;

  mp3fp = (mp3_file *)data;
  if(mp3fp->fpos flen)
  {
      unproc_data_size = stream->bufend - stream->next_frame;
      memcpy(mp3fp->fbuf, mp3fp->fbuf+mp3fp->fbsize-unproc_data_size, unproc_data_size);
      copy_size = BUFSIZE - unproc_data_size;
      if(mp3fp->fpos + copy_size > mp3fp->flen)
      {
          copy_size = mp3fp->flen - mp3fp->fpos;
      }
      fread(mp3fp->fbuf+unproc_data_size, 1, copy_size, mp3fp->fp);
      mp3fp->fbsize = unproc_data_size + copy_size;
      mp3fp->fpos  += copy_size;
      
      /*Hand off the buffer to the mp3 input stream*/
      mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);
      ret_code = MAD_FLOW_CONTINUE;
  }
  else
  {
      ret_code = MAD_FLOW_STOP;
  }

  return ret_code;

}
```

注意：在上面的代码中涉及到了断桢问题，即一桢跨了两个BUFSIZE，这时候应该将缓冲区中的剩余数据先移至缓冲区头部，然后再从文件中读出数据填充缓冲区。  怎样用libmad设计一个简单的mp3播放器?  修改output()函数。 我在上面说过了，解码后的数据通过output()函数进行处理，在minimad.c中output()函数直接将解码后的数据送到标准输出，其实只要将这里修改为送到音频设备就可以实现播放了。 还有一点需要说明的是：mp3文件的采样率不是固定不变的，解码后的数据中包括采样率，在播放过程中，一旦采样率发生变化，要重新设置一下音频设备。 新建一个mp3player.c文件，然后将下面的代码复制进去，编译生成mp3player，这就是一个简单的mp3播放器了，可以用./mp3player
 1.mp3命令来播放1.mp3文件。 

```
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include "mad.h"

#define BUFSIZE 8192

/*
 * This is a private message structure. A generic pointer to this structure
 * is passed to each of the callback functions. Put here any data you need
 * to access from within the callbacks.
 */
struct buffer {
  FILE  *fp;                    /*file pointer*/
  unsigned int  flen;           /*file length*/
  unsigned int  fpos;           /*current position*/
  unsigned char fbuf[BUFSIZE];  /*buffer*/
  unsigned int  fbsize;         /*indeed size of buffer*/
};
typedef struct buffer mp3_file;

int soundfd;                 /*soundcard file*/
unsigned int prerate = 0;    /*the pre simple rate*/

int writedsp(int c)
{
    return write(soundfd, (char *)&c, 1);
}

void set_dsp()
{
    int format = AFMT_S16_LE;
    int channels = 2;

    soundfd = open("/dev/dsp", O_WRONLY);
    ioctl(soundfd, SNDCTL_DSP_SETFMT, &format);
    ioctl(soundfd, SNDCTL_DSP_CHANNELS, &channels);
}

/*
 * This is perhaps the simplest example use of the MAD high-level API.
 * Standard input is mapped into memory via mmap(), then the high-level API
 * is invoked with three callbacks: input, output, and error. The output
 * callback converts MAD's high-resolution PCM samples to 16 bits, then
 * writes them to standard output in little-endian, stereo-interleaved
 * format.
 */

static int decode(mp3_file *mp3fp);

int main(int argc, char *argv[])
{
  long flen, fsta, fend;
  int  dlen;
  mp3_file *mp3fp;

  if (argc != 2)
    return 1;

  mp3fp = (mp3_file *)malloc(sizeof(mp3_file));
  if((mp3fp->fp = fopen(argv[1], "r")) == NULL)
  {
      printf("can't open source file.\n");
      return 2;
  }
  fsta = ftell(mp3fp->fp);
  fseek(mp3fp->fp, 0, SEEK_END);
  fend = ftell(mp3fp->fp);
  flen = fend - fsta;
  if(flen fp, 0, SEEK_SET);
  fread(mp3fp->fbuf, 1, BUFSIZE, mp3fp->fp);
  mp3fp->fbsize = BUFSIZE;
  mp3fp->fpos   = BUFSIZE;
  mp3fp->flen   = flen;

  set_dsp();

  decode(mp3fp);

  close(soundfd);
  fclose(mp3fp->fp);

  return 0;
}

/*
 * This is the input callback. The purpose of this callback is to (re)fill
 * the stream buffer which is to be decoded. In this example, an entire file
 * has been mapped into memory, so we just call mad_stream_buffer() with the
 * address and length of the mapping. When this callback is called a second
 * time, we are finished decoding.
 */

static
enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  mp3_file *mp3fp;
  int      ret_code;
  int      unproc_data_size;    /*the unprocessed data's size*/
  int      copy_size;

  mp3fp = (mp3_file *)data;
  if(mp3fp->fpos flen)
  {
      unproc_data_size = stream->bufend - stream->next_frame;
      memcpy(mp3fp->fbuf, mp3fp->fbuf+mp3fp->fbsize-unproc_data_size, unproc_data_size);
      copy_size = BUFSIZE - unproc_data_size;
      if(mp3fp->fpos + copy_size > mp3fp->flen)
      {
          copy_size = mp3fp->flen - mp3fp->fpos;
      }
      fread(mp3fp->fbuf+unproc_data_size, 1, copy_size, mp3fp->fp);
      mp3fp->fbsize = unproc_data_size + copy_size;
      mp3fp->fpos  += copy_size;
      
      /*Hand off the buffer to the mp3 input stream*/
      mad_stream_buffer(stream, mp3fp->fbuf, mp3fp->fbsize);
      ret_code = MAD_FLOW_CONTINUE;
  }
  else
  {
      ret_code = MAD_FLOW_STOP;
  }

  return ret_code;

}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline
signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L <= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample > (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */

static
enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  unsigned int rate;
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */

  rate= pcm->samplerate;
  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];

  /* update the sample rate of dsp*/
  if(rate != prerate)
  {
      ioctl(soundfd, SNDCTL_DSP_SPEED, &rate);
      prerate = rate;
  }

  while (nsamples--) {
    signed int sample;

    /* output sample(s) in 16-bit signed little-endian PCM */

    sample = scale(*left_ch++);
    writedsp((sample >> 0) & 0xff);
    writedsp((sample >> 8) & 0xff);

    if (nchannels == 2) {
      sample = scale(*right_ch++);
      writedsp((sample >> 0) & 0xff);
      writedsp((sample >> 8) & 0xff);
    }
  }

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the error callback function. It is called whenever a decoding
 * error occurs. The error is indicated by stream->error; the list of
 * possible MAD_ERROR_* errors can be found in the mad.h (or stream.h)
 * header file.
 */

static enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
  mp3_file *mp3fp = data;

  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - mp3fp->fbuf);

  /* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */

  return MAD_FLOW_CONTINUE;
}

/*
 * This is the function called by main() above to perform all the decoding.
 * It instantiates a decoder object and configures it with the input,
 * output, and error callback functions above. A single call to
 * mad_decoder_run() continues until a callback function returns
 * MAD_FLOW_STOP (to stop decoding) or MAD_FLOW_BREAK (to stop decoding and
 * signal an error).
 */

static int decode(mp3_file *mp3fp)
{
  struct mad_decoder decoder;
  int result;

  /* configure input, output, and error functions */
  mad_decoder_init(&decoder, mp3fp,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);

  /* start decoding */
  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

  /* release the decoder */
  mad_decoder_finish(&decoder);

  return result;
}
```



