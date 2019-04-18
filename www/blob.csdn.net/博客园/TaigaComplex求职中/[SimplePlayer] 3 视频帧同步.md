# [SimplePlayer] 3. 视频帧同步 - TaigaComplex求职中 - 博客园







# [[SimplePlayer] 3. 视频帧同步](https://www.cnblogs.com/TaigaCon/p/9819819.html)





# Frame Rate

帧率代表的是每一秒所播放的视频图像数目。通常，视频都会有固定的帧率，具体点地说是每一帧的时间间隔都是一样的，这种情况简称为CFR（Constant Frame Rate）;另外一种情况就是每一帧的时间间隔不一定相同，即可变帧率，简称为[VFR](https://en.wikipedia.org/wiki/Variable_frame_rate)（Variable Frame Rate），现在也有些录像设备支持录制VFR视频了，在录制具有大量静止场景的视频时，采用VFR能降低录制出来的视频的容量大小。





# PTS

通过上文对帧率的描述，我们知道在进行视频播放时，每一帧都应该有自己的播放时刻。不过视频文件中不会直接存放帧的播放时刻，为了保证精度以及节省存储空间，文件中会存储每一帧的PTS（Presentation Time Stamp）以及一个名为time base的参数，PTS是整数，而time base是小数，通过PTS与time base相乘，就能得到视频的播放时刻，单位为秒。大部分的视频格式都支持存放帧的PTS，而在播放的时候，通过对PTS进行计算就能使得视频的每一帧都在合适的时间显示。通常，第一帧的PTS为0。





# 同步流程

视频帧有固定的输出时间的话，意味着在解码完成一帧之后，需要进行等待，等到合适的时间才能进行帧的显示。

![image](https://img2018.cnblogs.com/blog/421096/201810/421096-20181019231244275-471833620.png)

上图为一个视频帧的解码、显示循环。在解码完成一帧后，需要获得当前时间以及当前帧的显示时间，两者的时间差就是需要睡眠的时长。

![image](https://img2018.cnblogs.com/blog/421096/201810/421096-20181019231245259-1501849956.png)

不过由于我们常用的操作系统为非实时系统，因此如果希望执行短时间的休眠，调用sleep通常不会得到非常满意的结果。解决方法就是通过多次休眠更短的时间段，并在被唤醒过后检查是否已经超时，超时即可进行视频帧的输出。

![image](https://img2018.cnblogs.com/blog/421096/201810/421096-20181019231245905-490123218.png)


            //Decode video frame
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);
            if(frameFinished){
                memcpy(Display.YPlane, pFrame->data[0], bufsize);
                memcpy(Display.UPlane, pFrame->data[1], bufsize/4);
                memcpy(Display.VPlane, pFrame->data[2], bufsize/4);

                vs.frame_last_pts = vs.frame_cur_pts;
                vs.frame_cur_pts = pFrame->pts * time_base * 1000000;
                pts_delay = vs.frame_cur_pts - vs.frame_last_pts;
                vs.cur_display_time = vs.last_display_time + pts_delay;
                //vs.last_frame_displayed = 0;

                if(!vs.is_first_frame){
                    time = av_gettime_relative();
                    delay = vs.cur_display_time - time;
                    while(delay > 0){
                        if(delay > 10000)
                            vs.sleep_time = 10000;
                        else
                            vs.sleep_time = delay;
                        av_usleep(vs.sleep_time);
                        time = av_gettime_relative();
                        delay = vs.cur_display_time - time;
                    }
                    vs.last_display_time = time;
                    DisplayFrame(&Display);
                    //vs.last_frame_displayed = 1;
                }else{
                    vs.last_display_time = av_gettime_relative();
                    DisplayFrame(&Display);
                    vs.is_first_frame = 0;
                    //vs.last_frame_displated = 1;
                }

                SDL_PumpEvents();
            }











