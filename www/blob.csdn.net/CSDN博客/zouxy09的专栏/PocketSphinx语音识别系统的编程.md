
# PocketSphinx语音识别系统的编程 - zouxy09的专栏 - CSDN博客


2012年09月14日 10:59:11[zouxy09](https://me.csdn.net/zouxy09)阅读数：28249


**PocketSphinx****语音识别系统的编程**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
关于语音识别的基础知识和sphinx的知识，具体可以参考我的另外的博文：
语音识别的基础知识与CMUsphinx介绍：
http://blog.csdn.net/zouxy09/article/details/7941585
PocketSphinx语音识别系统的编译、安装和使用：
http://blog.csdn.net/zouxy09/article/details/7942784
PocketSphinx语音识别系统语言模型的训练和声学模型的改进:
http://blog.csdn.net/zouxy09/article/details/7949126
PocketSphinx语音识别系统声学模型的训练与使用
http://blog.csdn.net/zouxy09/article/details/7962382
本文主要实现PocketSphinx语音识别系统的编程使用，主要分两个方面，一个是编程解码语音文件（主要参考CMU sphinx的wiki：http://cmusphinx.sourceforge.net/wiki/），二是编程识别麦克风的语音（主要参考PocketSphinx源码包里的pocketsphinx.c文件）。对于后面加入我的人机交互系统的话，采用的是识别麦克风的语音的编程，具体使用时还需要对其进行精简。
**一、编程解码语音文件**
**1****、编程：**
```cpp
#include <pocketsphinx.h>
int main(int argc, char *argv[])
{
	ps_decoder_t *ps;
	cmd_ln_t *config;
	FILE *fh;
	char const *hyp, *uttid;
        int16 buf[512];
	int rv;
	int32 score;
	//1、初始化：创建一个配置对象 cmd_ln_t *
	//cmd_ln_init函数第一个参数是我们需要更新的上一个配置，因为这里是初次创建，所以传入NULL；
	//第二个参数是一个定义参数的数组，如果使用的是标准配置的参数集的话可以通过调用ps_args()去获得。
	//第三个参数是是一个标志，它决定了参数的解释是否严格，如果为TRUE，那么遇到重复的或者未知的参
	//数，将会导致解释失败；
	//MODELDIR这个宏，指定了模型的路径，包括声学模型，语言模型和字典三个文件，是由gcc命令行传入，
	//我们通过pkg-config工具从PocketSphinx的配置中去获得这个modeldir变量
	config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
			     "-lm", MODELDIR "/lm/en/turtle.DMP",
			     "-dict", MODELDIR "/lm/en/turtle.dic",
			     NULL);
	if (config == NULL)
		return 1;
	
	//2、初始化解码器（语言识别就是一个解码过程，通俗的将就是将你说的话解码成对应的文字串）
	ps = ps_init(config);
	if (ps == NULL)
		return 1;
	//3、解码文件流
	//因为音频输入接口（麦克风）受到一些特定平台的影响，不利用我们演示，所以我们通过解码音频文件流
	//来演示PocketSphinx API的用法，goforward.raw是一个包含了一些诸如“go forward ten meters”等用来
	//控制机器人的短语（指令）的音频文件，其在test/data/goforward.raw。把它复制到当前目录
	fh = fopen("/dev/input/event14", "rb");
	if (fh == NULL) {
		perror("Failed to open goforward.raw");
		return 1;
	}
	
	//4、使用ps_decode_raw()进行解码
	
	rv = ps_decode_raw(ps, fh, NULL, -1);
	if (rv < 0)
		return 1;
	
	//5、得到解码的结果（概率最大的字串） hypothesis
	hyp = ps_get_hyp(ps, &score, &uttid);
	if (hyp == NULL)
		return 1;
	printf("Recognized: %s\n", hyp);
	//从内存中解码音频数据
	//现在我们将再次解码相同的文件，但是使用API从内存块中解码音频数据。在这种情况下，首先我们
	//需要使用ps_start_utt()开始说话：
    fseek(fh, 0, SEEK_SET);
	
    rv = ps_start_utt(ps, NULL);
	if (rv < 0)
		return 1;
        while (!feof(fh)) {
    rv = ps_start_utt(ps, NULL);
        if (rv < 0)
                return 1;
		printf("ready:\n");
            size_t nsamp;
            nsamp = fread(buf, 2, 512, fh);
		printf("read:\n");
			//我们将每次从文件中读取512大小的样本，使用ps_process_raw()把它们放到解码器中:
            rv = ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
		printf("process:\n");
        }
		//我们需要使用ps_end_utt()去标记说话的结尾处：
        rv = ps_end_utt(ps);
	if (rv < 0)
		return 1;
		
	//以相同精确的方式运行来检索假设的字符串：
	hyp = ps_get_hyp(ps, &score, &uttid);
	if (hyp == NULL)
		return 1;
	printf("Recognized: %s\n", hyp);
	}
	//6、清理工作：使用ps_free()释放使用ps_init()返回的对象，不用释放配置对象。
	fclose(fh);
        ps_free(ps);
	return 0;
}
```

**2****、编译：**
编译方法：
gcc -o test_ps test_ps.c \
-DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" \
`pkg-config --cflags --libs pocketsphinx sphinxbase`
//gcc的-D选项，指定宏定义，如-Dmacro=defn相当于C语言中的\#define macro=defn那么上面就表示在test_ps.c文件中，新加入一个宏定义：
\#define MODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\"
\表示转义符，把“号转义。
这么做是为什么呢？因为程序中需要指定MODELDIR这个变量，但是因为不同的使用者，这个变量不一样，没办法指定死一个路径，所以只能放在编译时，让用户去根据自己的情况来指定。
pkg-config工具可以获得一个库的编译和连接等信息；
\#pkg-config --cflags --libs pocketsphinx sphinxbase
显示：
-I/usr/local/include/sphinxbase  -I/usr/local/include/pocketsphinx
-L/usr/local/lib -lpocketsphinx -lsphinxbase –lsphinxad
\#pkg-config --variable=modeldir pocketsphinx
显示结果输出：/usr/local/share/pocketsphinx/model
**二、编程解码麦克风的录音**
**1****、编程**
麦克风录音数据的获得主要是用sphinxbase封装了alsa的接口来实现。
```cpp
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <setjmp.h>
#include <sphinxbase/err.h>
//generic live audio interface for recording and playback
#include <sphinxbase/ad.h>
#include <sphinxbase/cont_ad.h>
#include "pocketsphinx.h"
static ps_decoder_t *ps;
static cmd_ln_t *config;
static void print_word_times(int32 start)
{
	ps_seg_t *iter = ps_seg_iter(ps, NULL);
	while (iter != NULL) 
	{
		int32 sf, ef, pprob;
		float conf;
		
		ps_seg_frames (iter, &sf, &ef);
		pprob = ps_seg_prob (iter, NULL, NULL, NULL);
		conf = logmath_exp(ps_get_logmath(ps), pprob);
		printf ("%s %f %f %f\n", ps_seg_word (iter), (sf + start) / 100.0, (ef + start) / 100.0, conf);
		iter = ps_seg_next (iter);
	}
}
/* Sleep for specified msec */
static void sleep_msec(int32 ms)
{
    struct timeval tmo;
    tmo.tv_sec = 0;
    tmo.tv_usec = ms * 1000;
    select(0, NULL, NULL, NULL, &tmo);
}
/*
 * Main utterance processing loop:
 *     for (;;) {
 * 	   wait for start of next utterance;
 * 	   decode utterance until silence of at least 1 sec observed;
 * 	   print utterance result;
 *     }
 */
static void recognize_from_microphone()
{
    ad_rec_t *ad;
    int16 adbuf[4096];
    int32 k, ts, rem;
    char const *hyp;
    char const *uttid;
    cont_ad_t *cont;
    char word[256];
    if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                          (int)cmd_ln_float32_r(config, "-samprate"))) == NULL)
        E_FATAL("Failed top open audio device\n");
    /* Initialize continuous listening module */
    if ((cont = cont_ad_init(ad, ad_read)) == NULL)
        E_FATAL("Failed to initialize voice activity detection\n");
    if (ad_start_rec(ad) < 0)
        E_FATAL("Failed to start recording\n");
    if (cont_ad_calib(cont) < 0)
        E_FATAL("Failed to calibrate voice activity detection\n");
    for (;;) {
        /* Indicate listening for next utterance */
        printf("READY....\n");
        fflush(stdout);
        fflush(stderr);
        /* Wait data for next utterance */
        while ((k = cont_ad_read(cont, adbuf, 4096)) == 0)
            sleep_msec(100);
        if (k < 0)
            E_FATAL("Failed to read audio\n");
        /*
         * Non-zero amount of data received; start recognition of new utterance.
         * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
         */
        if (ps_start_utt(ps, NULL) < 0)
            E_FATAL("Failed to start utterance\n");
        ps_process_raw(ps, adbuf, k, FALSE, FALSE);
        printf("Listening...\n");
        fflush(stdout);
        /* Note timestamp for this first block of data */
        ts = cont->read_ts;
        /* Decode utterance until end (marked by a "long" silence, >1sec) */
        for (;;) {
            /* Read non-silence audio data, if any, from continuous listening module */
            if ((k = cont_ad_read(cont, adbuf, 4096)) < 0)
                E_FATAL("Failed to read audio\n");
            if (k == 0) {
                /*
                 * No speech data available; check current timestamp with most recent
                 * speech to see if more than 1 sec elapsed.  If so, end of utterance.
                 */
                if ((cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
                    break;
            }
            else {
                /* New speech data received; note current timestamp */
                ts = cont->read_ts;
            }
            /*
             * Decode whatever data was read above.
             */
            rem = ps_process_raw(ps, adbuf, k, FALSE, FALSE);
            /* If no work to be done, sleep a bit */
            if ((rem == 0) && (k == 0))
                sleep_msec(20);
        }
        /*
         * Utterance ended; flush any accumulated, unprocessed A/D data and stop
         * listening until current utterance completely decoded
         */
        ad_stop_rec(ad);
        while (ad_read(ad, adbuf, 4096) >= 0);
        cont_ad_reset(cont);
        printf("Stopped listening, please wait...\n");
        fflush(stdout);
        /* Finish decoding, obtain and print result */
        ps_end_utt(ps);
        hyp = ps_get_hyp(ps, NULL, &uttid);
        printf("%s: %s\n", uttid, hyp);
        fflush(stdout);
        /* Exit if the first word spoken was GOODBYE */
        if (hyp) {
            sscanf(hyp, "%s", word);
            if (strcmp(word, "goodbye") == 0)
                break;
        }
        /* Resume A/D recording for next utterance */
        if (ad_start_rec(ad) < 0)
            E_FATAL("Failed to start recording\n");
    }
    cont_ad_close(cont);
    ad_close(ad);
}
static jmp_buf jbuf;
static void sighandler(int signo)
{
    longjmp(jbuf, 1);
}
int main(int argc, char *argv[])
{
	
	config = cmd_ln_init(NULL, ps_args(), TRUE,
			     "-hmm", MODELDIR "/hmm/en_US/hub4wsj_sc_8k",
			     "-lm", MODELDIR "/lm/en/turtle.DMP",
			     "-dict", MODELDIR "/lm/en/turtle.dic",
			     NULL);
	if (config == NULL)
		return 1;
	
	ps = ps_init(config);
	if (ps == NULL)
		return 1;
	signal(SIGINT, &sighandler);
 	if (setjmp(jbuf) == 0) 
	    recognize_from_microphone();
	
        ps_free(ps);
	return 0;
}
```
**2****、编译**
和1.2一样。
至于说后面把PocketSphinx语音识别系统加入我的人机交互系统这个阶段，因为感觉这个系统本身的识别率不是很高，自己做了适应和重新训练声学和语言模型后，提升还是有限，暂时实用性还不是很强，所以暂时搁置下，看能不能通过其他方法去改进目前的状态。希望有牛人指导下。另外，由于开学了，需要上课，所以后续的进程可能会稍微减慢，不过依然期待各位多多交流！呵呵


