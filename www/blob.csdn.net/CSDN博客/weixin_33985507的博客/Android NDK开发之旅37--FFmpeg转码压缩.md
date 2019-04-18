# Android NDK开发之旅37--FFmpeg转码压缩 - weixin_33985507的博客 - CSDN博客
2018年01月15日 15:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
[Android NDK开发之旅 目录](https://www.jianshu.com/p/0c948db795ab)
#### 前言
Windows下命令行转码压缩形式
```
ffmpeg -i input.mov -b:v 640k output.mp4
```
Android也是利用FFmpeg命令行的形式(混编)，进行视频转码压缩。
#### 1.引入FFmpeg命令行调用库
![1824809-8e8dbb3a922b0583.png](https://upload-images.jianshu.io/upload_images/1824809-8e8dbb3a922b0583.png)
#### 2.Android 核心代码
##### 2.1.jni Java声明
```
//视频转码压缩
    public native void transcodingCompress(int argc,String[] argv);
```
##### 2.2.核心代码ffmpeg_transcoding.c
```
#include "jni.h"
#include "ffmpeg.h"
//视频转码压缩主函数入口
//SDL（main）
//ffmpeg_mod.c有一个FFmpeg视频转码主函数入口
//标记（声明有一个这样的函数提供给我调用）
//参数含义分析
//首先分析：String str = "ffmpeg -i input.mov -b:v 640k output.mp4"
// argc = str.split(" ").length()
// argv = str.split(" ")  字符串数组
//参数一：命令行字符串命令个数
//参数二：命令行字符串数组
int ffmpegmain(int argc, char **argv);
JNIEXPORT void JNICALL Java_com_haocai_ffmpegtest_util_VideoPlayer_transcodingCompress
        (JNIEnv *env, jobject jobj,jint jlen,jobjectArray jobjArray){
    //转码
    //将java的字符串数组转成C字符串
    int argc = jlen;
    //开辟内存空间
    char **argv = (char**)malloc(sizeof(char*) * argc);
    //填充内容
    for (int i = 0; i < argc; ++i) {
        jstring str = (*env)->GetObjectArrayElement(env,jobjArray,i);
        const char* tem = (*env)->GetStringUTFChars(env,str,0);
        argv[i] = (char*)malloc(sizeof(char)*1024);
        strcpy(argv[i],tem);
    }
    //开始转码(底层实现就是只需命令)
    ffmpegmain(argc,argv);
    //释放内存空间
    for (int i = 0; i < argc; ++i) {
        free(argv[i]);
    }
    //释放数组
    free(argv);
}
```
###### 调用ffmpeg_mod.c中ffmpegmain函数入口
```
//ffmpeg主函数入口
int ffmpegmain(int argc, char **argv)
{
    int ret;
    int64_t ti;
    //av_log_set_callback(av_log_callback); 
    register_exit(ffmpeg_cleanup);
    setvbuf(stderr,NULL,_IONBF,0); /* win32 runtime needs this */
    av_log_set_flags(AV_LOG_SKIP_REPEATED);
    parse_loglevel(argc, argv, options);
    if(argc>1 && !strcmp(argv[1], "-d")){
        run_as_daemon=1;
        av_log_set_callback(log_callback_null);
        argc--;
        argv++;
    }
    avcodec_register_all();
#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avfilter_register_all();
    av_register_all();
    avformat_network_init();
    show_banner(argc, argv, options);
    term_init();
    /* parse options and open all input/output files */
    ret = ffmpeg_parse_options(argc, argv);
    if (ret < 0)
    {   ffmpeg_cleanup(1);  return 1;}
    if (nb_output_files <= 0 && nb_input_files == 0) {
        show_usage();
        av_log(NULL, AV_LOG_WARNING, "Use -h to get full help or, even better, run 'man %s'\n", program_name);
        ffmpeg_cleanup(1);
        return 1;
    }
    /* file converter / grab */
    if (nb_output_files <= 0) {
        av_log(NULL, AV_LOG_FATAL, "At least one output file must be specified\n");
        ffmpeg_cleanup(1);
        return 1;
    }
//     if (nb_input_files == 0) {
//         av_log(NULL, AV_LOG_FATAL, "At least one input file must be specified\n");
//         exit_program(1);
//     }
    current_time = ti = getutime();
    if (transcode() < 0)
    {    ffmpeg_cleanup(1);  return 1;}
    ti = getutime() - ti;
     av_log(NULL, AV_LOG_FATAL, "Transcode has Finished\n");
  // if (do_benchmark) {
   //     printf("bench: utime=%0.3fs\n", ti / 1000000.0);
   // }
   // av_log(NULL, AV_LOG_DEBUG, "%"PRIu64" frames successfully decoded, %"PRIu64" decoding errors\n",
    //       decode_error_stat[0], decode_error_stat[1]);
    //if ((decode_error_stat[0] + decode_error_stat[1]) * max_error_rate < decode_error_stat[1])
   // {    exit_program(69); return 69;}
    ///exit_program(received_nb_signals ? 255 : main_return_code);
    ffmpeg_cleanup(0);
    return main_return_code;
}
```
##### 2.3调用程序
```
public void transcodingCompress(){
        final File inputFile;
        final File outputFile;
        final File dic = Environment.getExternalStorageDirectory();
        inputFile = new File(dic,"告白气球.avi");
        outputFile = new File(dic,"告白气球.mp4");
        new Thread(new Runnable() {
            @Override
            public void run() {
                //获取视频文件的路径(sdcard路径)
                // 拼接cmd 指令 ffmpeg -i 告白气球.avi -b:v 640k 告白气球.mp4 (与windows 命令行相同)
                StringBuilder builder = new StringBuilder();
                builder.append("ffmpeg ");
                builder.append("-i ");
                builder.append(inputFile.getAbsolutePath()+" ");
                builder.append("-b:v 640k ");  //码率越大 清晰度越高 码率越小 清晰度越低
                builder.append(outputFile.getAbsolutePath());
                String[] argv = builder.toString().split(" ");
                int argc = argv.length;
                player.transcodingCompress(argc,argv);
            }
        }).start();
    }
```
#### 3.运行结果
##### 3.1Log输出
```
I/hwaps: JNI_OnLoad
I/main: 当前帧：36
I/main: 当前帧：48
I/main: 当前帧：73
I/main: 当前帧：85
I/main: 当前帧：93
I/main: 当前帧：99
I/main: 当前帧：106
I/main: 当前帧：113
太多省略......
```
##### 3.2转码输出
![1824809-71deda884ab6350f.png](https://upload-images.jianshu.io/upload_images/1824809-71deda884ab6350f.png)
### 源码下载
##### Github：[https://github.com/kpioneer123/FFmpegTest](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fkpioneer123%2FFFmpegTest)
