# librtmp接收flv流中提取h264码流:根据多个资料汇总 - DoubleLi - 博客园






rtmpdump可以下载rtmp流并保存成flv文件。
如果要对流中的音频或视频单独处理，需要根据flv协议分别提取。
简单修改rtmpdump代码，增加相应功能。
1 提取音频:
rtmpdump程序在Download函数中循环下载:
....
 do
 {
....
nRead = RTMP_Read(rtmp, buffer, bufferSize);
....
}while(!RTMP_ctrlC && nRead > -1 && RTMP_IsConnected(rtmp) && !RTMP_IsTimedout(rtmp));
....

原程序是收到后写文件，生成flv。
现在，在写之前分别提取音视频，提取音频比较简单，直接分析buffer(参考RTMP_Write函数里的方法).
注意的是，rtmpdump里用的是RTMP_Read来接收，注意它的参数。为了方便，也可以直接用RTMP_ReadPacket。后面的视频使用RTMP_ReadPacket来接收并处理。

int RTMP_Write2(RTMP *r, const char *buf, int size)
{
  RTMPPacket *pkt = &r->m_write;
  char *pend, *enc;
  int s2 = size, ret, num;


 if (size < 11) {
   /* FLV pkt too small */
   return 0;
 }


 if (buf[0] == 'F' && buf[1] == 'L' && buf[2] == 'V')
   {
     buf += 13;
     s2 -= 13;
   }


 pkt->m_packetType = *buf++;
 pkt->m_nBodySize = AMF_DecodeInt24(buf);
 buf += 3;
 pkt->m_nTimeStamp = AMF_DecodeInt24(buf);
 buf += 3;
 pkt->m_nTimeStamp |= *buf++ << 24;
 buf += 3;
 s2 -= 11;


 if (((pkt->m_packetType == RTMP_PACKET_TYPE_AUDIO
                || pkt->m_packetType == RTMP_PACKET_TYPE_VIDEO) &&
            !pkt->m_nTimeStamp) || pkt->m_packetType == RTMP_PACKET_TYPE_INFO)
   {
     pkt->m_headerType = RTMP_PACKET_SIZE_LARGE;
     if (pkt->m_packetType == RTMP_PACKET_TYPE_INFO)
      pkt->m_nBodySize += 16;
   }
 else
   {
     pkt->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
   }


BYTE outbuf2[640];
int nLen2 = 640;


AVManager::GetInstance()->Decode((BYTE*)(pkt->m_body+1), pkt->m_nBodySize-1, outbuf2, nLen2);
//实际音频内容为pkt->m_body+1，大小是pkt->m_nBodySize-1。这里的声音是speex编码。
为什么跳过第一字节，可以参考：http://bbs.rosoo.net/thread-16488-1-1.html

evt_OnReceivePacket((char*)outbuf2, nLen2);//回调出来



RTMPPacket_Free(pkt);
pkt->m_nBytesRead = 0;

2 
视频处理
可以参考rtmpsrv.c
把nRead = RTMP_Read(rtmp, buffer, bufferSize);改成：

RTMPPacket pc = { 0 }, ps = { 0 };
 bool bFirst = true;
while (RTMP_ReadPacket(rtmp, &pc))
{
if (RTMPPacket_IsReady(&pc))
 {
     if (pc.m_packetType == RTMP_PACKET_TYPE_VIDEO && RTMP_ClientPacket(rtmp, &pc))
    {
        bool bIsKeyFrame = false;
     if (result == 0x17)//I frame
    {
        bIsKeyFrame = true;
    }
    else if (result == 0x27)
    {
        bIsKeyFrame = false;
    }
static unsigned char const start_code[4] = {0x00, 0x00, 0x00, 0x01};
fwrite(start_code, 1, 4, pf );
//int ret = fwrite(pc.m_body + 9, 1, pc.m_nBodySize-9, pf);


if( bFirst) {


//AVCsequence header


//ioBuffer.put(foredata);


//获取sps


int spsnum = data[10]&0x1f;


int number_sps = 11;


int count_sps = 1;


while (count_sps<=spsnum){


int spslen =(data[number_sps]&0x000000FF)<<8 |(data[number_sps+1]&0x000000FF);


number_sps += 2;


fwrite(data+number_sps, 1, spslen, pf );
fwrite(start_code, 1, 4, pf );


//ioBuffer.put(data,number_sps, spslen);
//ioBuffer.put(foredata);


number_sps += spslen;


count_sps ++;


}


//获取pps


int ppsnum = data[number_sps]&0x1f;


int number_pps = number_sps+1;


int count_pps = 1;


while (count_pps<=ppsnum){


int ppslen =(data[number_pps]&0x000000FF)<<8|data[number_pps+1]&0x000000FF;


number_pps += 2;


//ioBuffer.put(data,number_pps,ppslen);


//ioBuffer.put(foredata);


fwrite(data+number_pps, 1, ppslen, pf );
fwrite(start_code, 1, 4, pf );


number_pps += ppslen;


count_pps ++;


}


bFirst =false;


} else {


//AVCNALU


int len =0;


int num =5;


//ioBuffer.put(foredata);


while(num<pc.m_nBodySize) 
{


len =(data[num]&0x000000FF)<<24|(data[num+1]&0x000000FF)<<16|(data[num+2]&0x000000FF)<<8|data[num+3]&0x000000FF;


num = num+4;


//ioBuffer.put(data,num,len);


//ioBuffer.put(foredata);


fwrite(data+num, 1, len, pf );
fwrite(start_code, 1, 4, pf );


num = num + len;


}


}       




  }
}



具体视频分析的见: http://blog.csdn.net/cssmhyl/article/details/8128478







http://blog.chinaunix.net/uid-15063109-id-4273162.html









