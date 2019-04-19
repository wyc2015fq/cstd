# ffmpeg  RTP  over   TCP - 建建的博客 - CSDN博客
2017年03月16日 17:48:27[纪建](https://me.csdn.net/u013898698)阅读数：535
个人分类：[ffmpeg学习](https://blog.csdn.net/u013898698/article/category/6692312)
char filepath[]="rtsp://admin:jjchen12345678@192.168.3.25:554/h264/ch1/main/av_stream";
AVDictionary *optionsDict = NULL;
av_register_all();
avformat_network_init();
pFormatCtx = avformat_alloc_context();
av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0);                //采用tcp传输
av_dict_set(&optionsDict, "stimeout", "2000000", 0);                  //如果没有设置stimeout，那么把ipc网线拔掉，
av_read_frame会阻塞（时间单位是微妙）
if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
printf("无法打开文件\n");
return -1;
}
