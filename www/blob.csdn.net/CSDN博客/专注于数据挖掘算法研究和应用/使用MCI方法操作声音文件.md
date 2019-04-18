# 使用MCI方法操作声音文件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2008年05月15日 10:25:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2569








                引入：Winmm.lib      Mmsystem.hBYTE* CWaveDlg::GetData(CString pString) //获取声音文件数据的函数，pString参数指向要打开的声音文件；{ if (pString.IsEmpty())  return NULL; HMMIO file;//定义HMMIO文件句柄； file=mmioOpen(pString.GetBuffer(0),NULL,MMIO_READWRITE);//以读写模式打开所给的WAVE文件； if(file==NULL) {  MessageBox("WAVE文件打开失败！");  return NULL; } char style[4];//定义一个四字节的数据，用来存放文件的类型； mmioSeek(file,8,SEEK_SET);//定位到WAVE文件的类型位置 mmioRead(file,style,4); if(style[0]!='W'||style[1]!='A'||style[2]!='V'||style[3]!='E')//判断该文件是否为"WAVE"文件格式 {  MessageBox("该文件不是WAVE格式的文件！");  return NULL; } PCMWAVEFORMAT format; //定义PCMWAVEFORMAT结构对象，用来判断WAVE文件格式； mmioSeek(file,20,SEEK_SET);//对打开的文件进行定位，此时指向WAVE文件的PCMWAVEFORMAT结构的数据； mmioRead(file,(char*)&format,sizeof(PCMWAVEFORMAT));//获取该结构的数据； if(format.wf.nChannels!=2)//判断是否是立体声声音； {  MessageBox("该声音文件不是双通道立体声文件");  return NULL; } int len=sizeof(PCMWAVEFORMAT); mmioSeek(file,24+sizeof(PCMWAVEFORMAT)+20,SEEK_SET); //获取WAVE文件的声音数据的大小； long size; mmioRead(file,(char*)&size,4); BYTE *pData; pData=(BYTE*)new char[size];//根据数据的大小申请缓冲区； mmioSeek(file,28+sizeof(PCMWAVEFORMAT)+20,SEEK_SET);//对文件重新定位； mmioRead(file,(char*)pData,size);//读取声音数据； mmioClose(file, MMIO_FHOPEN);//关闭WAVE文件； return pData;}//播放用mcivoid CWaveDlg::OnOK() { // TODO: Add extra validation here MCI_OPEN_PARMS mciOpenParms; MCI_PLAY_PARMS PlayParms; mciOpenParms.dwCallback=0; mciOpenParms.lpstrElementName="D://temp//for test//Wave//ent36871.wav"; mciOpenParms.wDeviceID=0; mciOpenParms.lpstrDeviceType="waveaudio"; mciOpenParms.lpstrAlias=" "; PlayParms.dwCallback=0; PlayParms.dwTo=0; PlayParms.dwFrom=0; mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_TYPE|MCI_OPEN_ELEMENT,(DWORD)(LPVOID)&mciOpenParms);//打开音频设备； mciSendCommand(mciOpenParms.wDeviceID,MCI_PLAY,MCI_WAIT,(DWORD)(LPVOID)&PlayParms);//播放WAVE声音文件； mciSendCommand(mciOpenParms.wDeviceID,MCI_CLOSE,NULL,NULL);//关闭音频设备； //CDialog::OnOK();}            


