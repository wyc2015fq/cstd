# 使用FFmpeg新解码API解封装解码音视频(代码实例)-T-Bagwell-ChinaUnix博客






[使用FFmpeg新解码API解封装解码音视频(代码实例)](/uid-11344913-id-5780368.html)

****
分类： C/C++

2017-11-29 18:21:52






						在ffmpeg的源代码中，有新旧版本的编解码接口调用示例，但是demux、mux然后decode、encode的联动起来的接口调用实例并没有，在使用旧版本的编解码接口在编译时会报接口弃用告警信息，所以最好尽快把原有的调用方式切换到新的编解码接口调用方式，告警信息如下：





			点击(此处)折叠或打开
		


- liuqideMBP:xxx liuqi$ make doc/examples/demuxing_decoding

- 
				CC  doc/examples/demuxing_decoding.o

- 
				src/doc/examples/demuxing_decoding.c:73:15: warning:'avcodec_decode_video2' is deprecated [-Wdeprecated-declarations]

- ret= avcodec_decode_video2(video_dec_ctx, frame, got_frame, &pkt);

- 
				              ^

- 
				src/libavcodec/avcodec.h:4631:1: note:'avcodec_decode_video2' has been explicitly marked deprecated here

- 
				attribute_deprecated

- 
				^

- 
				src/libavutil/attributes.h:94:49: note: expanded from macro'attribute_deprecated'

- 
				#    define attribute_deprecated __attribute__((deprecated))

- 
				                                                ^

- 
				src/doc/examples/demuxing_decoding.c:111:15: warning:'avcodec_decode_audio4' is deprecated [-Wdeprecated-declarations]

- ret= avcodec_decode_audio4(audio_dec_ctx, frame, got_frame, &pkt);

- 
				              ^

- 
				src/libavcodec/avcodec.h:4582:1: note:'avcodec_decode_audio4' has been explicitly marked deprecated here

- 
				attribute_deprecated

- 
				^

- 
				src/libavutil/attributes.h:94:49: note: expanded from macro'attribute_deprecated'

- 
				#    define attribute_deprecated __attribute__((deprecated))

- 
				                                                ^

- 
				2 warnings generated.

- 
				LD  doc/examples/demuxing_decoding_g

- 
				ld: warning: directory not found foroption'-Llibavresample'

- 
				STRIP   doc/examples/demuxing_decoding
			


为了修改方便，而网上又没有只管举例的相关完整的实例，所以在这里写一个例子，供大伙参考



- /*

- * Copyright (c) 2017 bbs.chinaffmpeg.com 孙悟空

- *

- * Permission is hereby granted, free of charge,to any person obtaining a copy

- * of this software and associated documentation files (the "Software"),to deal

- *in the Software without restriction, including without limitation the rights

- *to use, copy, modify, merge, publish, distribute, sublicense,and/or sell

- * copies of the Software,andto permit persons to whom the Software is

- * furnished todo so, subject to the following conditions:

- *

- * The above copyright notice and this permission notice shall be included in

- * all copies or substantial portions of the Software.

- *

- * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

- * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

- * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL

- * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

- * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

- * OUT OF ORIN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN

- * THE SOFTWARE.

- */

- 

- /**

- * @file

- * Demuxing and decoding example.

- *

- * Show how to use the libavformat and libavcodec API to demux and

- * decode audio and video data.

- * @example demuxing_decoding.c

- */

- 

- 
			#include <libavutil/imgutils.h>

- 
			#include <libavutil/samplefmt.h>

- 
			#include <libavutil/timestamp.h>

- 
			#include <libavformat/avformat.h>

- 

- 
			static AVFormatContext *fmt_ctx =NULL;

- 
			static AVCodecContext *video_dec_ctx =NULL,*audio_dec_ctx;

- 
			static int width, height;

- 
			static enum AVPixelFormat pix_fmt;

- 
			static AVStream *video_stream =NULL,*audio_stream =NULL;

- 
			static const char *src_filename =NULL;

- 
			static const char *video_dst_filename =NULL;

- 
			static const char *audio_dst_filename =NULL;

- 
			static FILE *video_dst_file =NULL;

- 
			static FILE *audio_dst_file =NULL;

- 

- 
			static uint8_t *video_dst_data[4]={NULL};

- 
			static int video_dst_linesize[4];

- 
			static int video_dst_bufsize;

- 

- 
			static int video_stream_idx =-1, audio_stream_idx =-1;

- 
			static AVFrame *frame =NULL;

- 
			static AVPacket pkt;

- 
			static int video_frame_count = 0;

- 
			static int audio_frame_count = 0;

- 

- /* Enable or disable frame reference counting. You are not supposed to support

- * both paths in your application but pick the one most appropriate to your

- * needs. Look for the use of refcount in this example to see what are the

- * differences of API usage between them.*/

- 
			static int refcount = 0;

- 

- 
			static int decode_packet(int*got_frame,int cached)

- {

- int ret = 0;

- int i = 0;

- int ch = 0;

- int data_size = 0;

- int decoded = pkt.size;

- 

- *got_frame = 0;

- 

- if(pkt.stream_index == video_stream_idx){

- /* decode video frame */

- 
			        ret = avcodec_send_packet(video_dec_ctx,&pkt);

- if(ret < 0){

- 
			            fprintf(stderr,"Error sending a packet for decoding\n");

- exit(1);

- }

- 

- while(ret >= 0){

- 
			            ret = avcodec_receive_frame(video_dec_ctx, frame);

- if(ret == AVERROR(EAGAIN)|| ret == AVERROR_EOF)

- 
			                return ret;

- elseif(ret < 0){

- 
			                fprintf(stderr,"Error during decoding\n");

- exit(1);

- }

- 
			            printf("bbs.chinaffmpeg.com 孙悟空 video_frame%s n:%d coded_n:%d\n",

- 
			                   cached ?"(cached)":"",

- 
			                   video_frame_count++, frame->coded_picture_number);

- 

- /* copy decoded frame to destination buffer:

- * this is required since rawvideo expects non aligned data */

- 
			            av_image_copy(video_dst_data, video_dst_linesize,

- (const uint8_t **)(frame->data), frame->linesize,

- 
			                          pix_fmt, width, height);

- 

- /* write to rawvideo file */

- 
			            fwrite(video_dst_data[0], 1, video_dst_bufsize, video_dst_file);

- 

- 
			            printf("saving frame %3d\n", video_dec_ctx->frame_number);

- 
			            fflush(stdout);

- }

- }elseif(pkt.stream_index == audio_stream_idx){

- /* decode audio frame */

- 
			#if 1

- /* send the packet with the compressed data to the decoder */

- 
			        ret = avcodec_send_packet(audio_dec_ctx,&pkt);

- if(ret < 0){

- 
			            fprintf(stderr,"Error submitting the packet to the decoder\n");

- exit(1);

- }

- 

- /* read all the output frames (in general there may be any number of them */

- while(ret >= 0){

- 
			            ret = avcodec_receive_frame(audio_dec_ctx, frame);

- if(ret == AVERROR(EAGAIN)|| ret == AVERROR_EOF)

- 
			                return ret;

- elseif(ret < 0){

- 
			                fprintf(stderr,"Error during decoding\n");

- exit(1);

- }

- 
			            data_size = av_get_bytes_per_sample(audio_dec_ctx->sample_fmt);

- if(data_size < 0){

- /* This should not occur, checking just for paranoia */

- 
			                fprintf(stderr,"Failed to calculate data size\n");

- exit(1);

- }

- for(i = 0; i < frame->nb_samples; i++)

- for(ch = 0; ch < audio_dec_ctx->channels; ch++)

- 
			                    fwrite(frame->data[ch]+ data_size*i, 1, data_size, audio_dst_file);

- }

- 

- 
			        printf("audio_frame%s n:%d nb_samples:%d pts:%s\n",

- 
			               cached ?"(cached)":"",

- 
			               audio_frame_count++, frame->nb_samples,

- 
			               av_ts2timestr(frame->pts,&audio_dec_ctx->time_base));

- 
			#else

- 
			        ret = avcodec_decode_audio4(audio_dec_ctx, frame, got_frame,&pkt);

- if(ret < 0){

- 
			            fprintf(stderr,"Error decoding audio frame (%s)\n", av_err2str(ret));

- 
			            return ret;

- }

- /* Some audio decoders decode only part of the packet,and have to be

- * called again with the remainder of the packet data.

- * Sample: 转自bbs.chinaffmpeg.com 孙悟空fate-suite/lossless-audio/
- 
			         * luckynight-partial.shn

- * Also, some decoders might over-read the packet.*/

- 
			        decoded = FFMIN(ret, pkt.size);

- 

- if(*got_frame){

- 
			            size_t unpadded_linesize = frame->nb_samples * av_get_bytes_per_sample(frame->format);

- 
			            printf("audio_frame%s n:%d nb_samples:%d pts:%s\n",

- 
			                   cached ?"(cached)":"",

- 
			                   audio_frame_count++, frame->nb_samples,

- 
			                   av_ts2timestr(frame->pts,&audio_dec_ctx->time_base));

- 

- /* Write the raw audio data samples of the first plane. This works

- * fine for packed formats (e.g. AV_SAMPLE_FMT_S16). However,

- * most audio decoders output planar audio, which uses a separate

- * plane of audio samples foreach channel (e.g. AV_SAMPLE_FMT_S16P).

- *In other words, this code will write only the first audio channel

- *in these cases.

- * You should use libswresample or libavfilter to convert the frame

- *to packed data.*/

- 
			            fwrite(frame->extended_data[0], 1, unpadded_linesize, audio_dst_file);

- }

- 
			#endif

- }

- 

- /*If we use frame reference counting, we own the data and need

- *to de-reference it when we don't use it anymore */

- if(*got_frame && refcount)

- 
			        av_frame_unref(frame);

- 

- 
			    return decoded;

- }

- 

- 
			static int open_codec_context(int*stream_idx,

- 
			                              AVCodecContext **dec_ctx, AVFormatContext *fmt_ctx, enum AVMediaType type)

- {

- int ret, stream_index;

- 
			    AVStream *st;

- 
			    AVCodec *dec =NULL;

- 
			    AVDictionary *opts =NULL;

- 

- 
			    ret = av_find_best_stream(fmt_ctx, type,-1,-1,NULL, 0);

- if(ret < 0){

- 
			        fprintf(stderr,"Could not find %s stream in input file '%s'\n",

- 
			                av_get_media_type_string(type), src_filename);

- 
			        return ret;

- }else{

- 
			        stream_index = ret;

- 
			        st = fmt_ctx->streams[stream_index];

- 

- /* find decoder for the stream */

- 
			        dec = avcodec_find_decoder(st->codecpar->codec_id);

- if(!dec){

- 
			            fprintf(stderr,"Failed to find %s codec\n",

- 
			                    av_get_media_type_string(type));

- 
			            return AVERROR(EINVAL);

- }

- 

- /* Allocate a codec context for the decoder */

- *dec_ctx = avcodec_alloc_context3(dec);

- if(!*dec_ctx){

- 
			            fprintf(stderr,"Failed to allocate the %s codec context\n",

- 
			                    av_get_media_type_string(type));

- 
			            return AVERROR(ENOMEM);

- }

- 

- /* Copy codec parameters from input stream to output codec context */

- if((ret = avcodec_parameters_to_context(*dec_ctx, st->codecpar))< 0){

- 
			            fprintf(stderr,"Failed to copy %s codec parameters to decoder context\n",

- 
			                    av_get_media_type_string(type));

- 
			            return ret;

- }

- 

- /* Init the decoders, with or without reference counting */

- 
			        av_dict_set(&opts,"refcounted_frames", refcount ?"1":"0", 0);

- if((ret = avcodec_open2(*dec_ctx, dec,&opts))< 0){

- 
			            fprintf(stderr,"Failed to open %s codec\n",

- 
			                    av_get_media_type_string(type));

- 
			            return ret;

- }

- *stream_idx = stream_index;

- }

- 

- 
			    return 0;

- }

- 

- 
			static int get_format_from_sample_fmt(const char **fmt,

- 
			                                      enum AVSampleFormat sample_fmt)

- {

- int i;

- 
			    struct sample_fmt_entry {

- 
			        enum AVSampleFormat sample_fmt;const char *fmt_be,*fmt_le;

- } sample_fmt_entries[]={

- { AV_SAMPLE_FMT_U8,"u8","u8"},

- { AV_SAMPLE_FMT_S16,"s16be","s16le"},

- { AV_SAMPLE_FMT_S32,"s32be","s32le"},

- { AV_SAMPLE_FMT_FLT,"f32be","f32le"},

- { AV_SAMPLE_FMT_DBL,"f64be","f64le"},

- };

- *fmt =NULL;

- 

- for(i = 0; i < FF_ARRAY_ELEMS(sample_fmt_entries); i++){

- 
			        struct sample_fmt_entry *entry =&sample_fmt_entries[i];

- if(sample_fmt == entry->sample_fmt){

- *fmt = AV_NE(entry->fmt_be, entry->fmt_le);

- 
			            return 0;

- }

- }

- 

- 
			    fprintf(stderr,

- "sample format %s is not supported as output format\n",

- 
			            av_get_sample_fmt_name(sample_fmt));

- 
			    return -1;

- }

- 

- int main (int argc, char **argv)

- {

- int ret = 0, got_frame;

- 

- if(argc != 4 && argc != 5){

- 
			        fprintf(stderr,"usage: %s [-refcount] input_file video_output_file audio_output_file\n"

- "API example program to show how to read frames from an input file.\n"

- "This program reads frames from a file, decodes them, and writes decoded\n"

- "video frames to a rawvideo file named video_output_file, and decoded\n"

- "audio frames to a rawaudio file named audio_output_file.\n\n"

- "If the -refcount option is specified, the program use the\n"

- "reference counting frame system which allows keeping a copy of\n"

- "the data for longer than one decode call.\n"

- "\n", argv[0]);

- exit(1);

- }

- if(argc == 5 &&!strcmp(argv[1],"-refcount")){

- 
			        refcount = 1;

- 
			        argv++;

- }

- 
			    src_filename = argv[1];

- 
			    video_dst_filename = argv[2];

- 
			    audio_dst_filename = argv[3];

- 

- /* register all formats and codecs */

- 
			    av_register_all();

- 

- /* open input file,and allocate format context */

- if(avformat_open_input(&fmt_ctx, src_filename,NULL,NULL)< 0){

- 
			        fprintf(stderr,"Could not open source file %s\n", src_filename);

- exit(1);

- }

- 

- /* retrieve stream information */

- if(avformat_find_stream_info(fmt_ctx,NULL)< 0){

- 
			        fprintf(stderr,"Could not find stream information\n");

- exit(1);

- }

- 

- if(open_codec_context(&video_stream_idx,&video_dec_ctx, fmt_ctx, AVMEDIA_TYPE_VIDEO)>= 0){

- 
			        video_stream = fmt_ctx->streams[video_stream_idx];

- 

- 
			        video_dst_file = fopen(video_dst_filename,"wb");

- if(!video_dst_file){

- 
			            fprintf(stderr,"Could not open destination file %s\n", video_dst_filename);

- 
			            ret = 1;

- 
			            goto end;

- }

- 

- /* allocate image where the decoded image will be put */

- 
			        width = video_dec_ctx->width;

- 
			        height = video_dec_ctx->height;

- 
			        pix_fmt = video_dec_ctx->pix_fmt;

- 
			        ret = av_image_alloc(video_dst_data, video_dst_linesize,

- 
			                             width, height, pix_fmt, 1);

- if(ret < 0){

- 
			            fprintf(stderr,"Could not allocate raw video buffer\n");

- 
			            goto end;

- }

- 
			        video_dst_bufsize = ret;

- }

- 

- if(open_codec_context(&audio_stream_idx,&audio_dec_ctx, fmt_ctx, AVMEDIA_TYPE_AUDIO)>= 0){

- 
			        audio_stream = fmt_ctx->streams[audio_stream_idx];

- 
			        audio_dst_file = fopen(audio_dst_filename,"wb");

- if(!audio_dst_file){

- 
			            fprintf(stderr,"Could not open destination file %s\n", audio_dst_filename);

- 
			            ret = 1;

- 
			            goto end;

- }

- }

- 

- /* dump input information to stderr */

- 
			    av_dump_format(fmt_ctx, 0, src_filename, 0);

- 

- if(!audio_stream &&!video_stream){

- 
			        fprintf(stderr,"Could not find audio or video stream in the input, aborting\n");

- 
			        ret = 1;

- 
			        goto end;

- }

- 

- 
			    frame = av_frame_alloc();

- if(!frame){

- 
			        fprintf(stderr,"Could not allocate frame\n");

- 
			        ret = AVERROR(ENOMEM);

- 
			        goto end;

- }

- 

- /* initialize packet,set data toNULL,let the demuxer fill it */

- 
			    av_init_packet(&pkt);

- 
			    pkt.data =NULL;

- 
			    pkt.size = 0;

- 

- if(video_stream)

- 
			        printf("Demuxing video from file '%s' into '%s'\n", src_filename, video_dst_filename);

- if(audio_stream)

- 
			        printf("Demuxing audio from file '%s' into '%s'\n", src_filename, audio_dst_filename);

- 

- /* read frames from the file */

- while(av_read_frame(fmt_ctx,&pkt)>= 0){

- 
			        AVPacket orig_pkt = pkt;

- do{

- 
			            ret = decode_packet(&got_frame, 0);

- if(ret < 0)

- 
			                break;

- 
			            pkt.data += ret;

- 
			            pkt.size -= ret;

- }while(pkt.size > 0);

- 
			        av_packet_unref(&orig_pkt);

- }

- 

- /* flush cached frames */

- 
			    pkt.data =NULL;

- 
			    pkt.size = 0;

- do{

- 
			        decode_packet(&got_frame, 1);

- }while(got_frame);

- 

- 
			    printf("Demuxing succeeded.\n");

- 

- if(video_stream){

- 
			        printf("Play the output video file with the command:\n"

- "ffplay -f rawvideo -pix_fmt %s -video_size %dx%d %s\n",

- 
			               av_get_pix_fmt_name(pix_fmt), width, height,

- 
			               video_dst_filename);

- }

- 

- if(audio_stream){

- 
			        enum AVSampleFormat sfmt = audio_dec_ctx->sample_fmt;

- int n_channels = audio_dec_ctx->channels;

- const char *fmt;

- 

- if(av_sample_fmt_is_planar(sfmt)){

- const char *packed = av_get_sample_fmt_name(sfmt);

- 
			            printf("Warning: the sample format the decoder produced is planar "

- "(%s). This example will output the first channel only.\n",

- 
			                   packed ? packed :"?");

- 
			            sfmt = av_get_packed_sample_fmt(sfmt);

- 
			            n_channels = 2;

- }

- 

- if((ret = get_format_from_sample_fmt(&fmt, sfmt))< 0)

- 
			            goto end;

- 

- 
			        printf("Play the output audio file with the command:\n"

- "ffplay -f %s -ac %d -ar %d %s\n",

- 
			               fmt, n_channels, audio_dec_ctx->sample_rate,

- 
			               audio_dst_filename);

- }

- 

- end:

- 
			    avcodec_free_context(&video_dec_ctx);

- 
			    avcodec_free_context(&audio_dec_ctx);

- 
			    avformat_close_input(&fmt_ctx);

- if(video_dst_file)

- 
			        fclose(video_dst_file);

- if(audio_dst_file)

- 
			        fclose(audio_dst_file);

- 
			    av_frame_free(&frame);

- 
			    av_free(video_dst_data[0]);

- 

- 
			    return ret < 0;

- }


下面看一下编译的方式，其实我是用的ffmpeg原生的编译方式编译的make doc/examples/demuxing_decoding，在这里贴一下make -n doc/examples/demuxing_decoding的输出








			点击(此处)折叠或打开
		


- liuqideMBP:xxx liuqi$ make -n doc/examples/demuxing_decoding

- 
				printf "CC\t%s\n" doc/examples/demuxing_decoding.o; ccache gcc -I.-Isrc/ -D_ISOC99_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -I/Users/liuqi/multimedia/ffmpeg/compat/dispatch_semaphore -DPIC -DZLIB_CONST -std=c11 -Werror=partial-availability -fomit-frame-pointer -fPIC -pthread -I/usr/local/include-I/usr/local/Cellar/fontconfig/2.12.6/include-I/usr/local/opt/freetype/include/freetype2 -I/usr/local/Cellar/fribidi/0.19.7_1/include/fribidi -I/usr/local/Cellar/glib/2.54.2/include/glib-2.0 -I/usr/local/Cellar/glib/2.54.2/lib/glib-2.0/include-I/usr/local/opt/gettext/include-I/usr/local/Cellar/pcre/8.41/include-I/usr/local/opt/freetype/include/freetype2 -I/usr/local/Cellar/libbluray/1.0.1/include-I/usr/local/include-I/usr/local/Cellar/fontconfig/2.12.6/include-I/usr/local/opt/freetype/include/freetype2 -I/usr/local/opt/freetype/include/freetype2 -I/usr/local/Cellar/speex/1.2rc1/include-I/usr/local/include-I/usr/local/Cellar/x265/2.5_1/include-g -Wdeclaration-after-statement -Wall -Wdisabled-optimization -Wpointer-arith -Wredundant-decls -Wwrite-strings -Wtype-limits -Wundef -Wmissing-prototypes -Wno-pointer-to-int-cast -Wstrict-prototypes -Wempty-body -Wno-parentheses -Wno-switch -Wno-format-zero-length-Wno-pointer-sign -Wno-unused-const-variable -O3 -fno-math-errno -fno-signed-zeros -mstack-alignment=16 -Qunused-arguments -Werror=implicit-function-declaration -Werror=missing-prototypes -Werror=return-type-D_THREAD_SAFE -I/usr/local/include/SDL2 -MMD -MF doc/examples/demuxing_decoding.d -MT doc/examples/demuxing_decoding.o -c -o doc/examples/demuxing_decoding.o src/doc/examples/demuxing_decoding.c

- 
				printf "LD\t%s\n" doc/examples/demuxing_decoding_g; ccache gcc -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavresample -Llibavutil -Llibpostproc -Llibswscale -Llibswresample -Wl,-dynamic,-search_paths_first -Qunused-arguments -o doc/examples/demuxing_decoding_g doc/examples/demuxing_decoding.o -lavdevice -lavfilter -lavformat -lavcodec -lpostproc -lswresample -lswscale -lavutil -framework Foundation -lm -framework AVFoundation -framework CoreVideo -framework CoreMedia -pthread -framework CoreGraphics -L/usr/local/lib -lSDL2 -framework OpenGL -framework OpenGL -pthread -lm -L/usr/local/lib -lass -framework CoreImage -framework AppKit -L/usr/local/Cellar/fontconfig/2.12.6/lib -L/usr/local/opt/freetype/lib -lfontconfig -lfreetype -L/usr/local/opt/freetype/lib -lfreetype -lm -lbz2 -L/usr/local/Cellar/libbluray/1.0.1/lib -lbluray -lz -Wl,-framework,CoreFoundation -Wl,-framework,Security -liconv -lm -llzma -lz -framework AudioToolbox -L/usr/local/lib -lfdk-aac -lmp3lame -L/usr/local/Cellar/speex/1.2rc1/lib -lspeex -L/usr/local/lib -lx264 -L/usr/local/Cellar/x265/2.5_1/lib -lx265 -pthread -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices -lm -lm -lm -pthread -lm -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices

- 
				printf "STRIP\t%s\n" doc/examples/demuxing_decoding; strip -x -o doc/examples/demuxing_decoding doc/examples/demuxing_decoding_g
			








