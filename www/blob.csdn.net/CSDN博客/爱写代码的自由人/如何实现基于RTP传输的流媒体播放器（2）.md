# 如何实现基于RTP传输的流媒体播放器（2） - 爱写代码的自由人 - CSDN博客





2019年01月16日 18:10:06[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：173








这篇文章给大家介绍怎么打包H264分片到RTP包，并且实现一个播放程序演示接收RTP包和用FFmpeg解码。为了让大家更了解H264打包成RTP的细节，有必要罗嗦一下向大家介绍一些相关的基础知识，下面分三节介绍，其中前面两节是基础知识，第三节重点讲解怎么实现RTP包的发送和接收，以及怎么用FFmpeg解码显示。

**一、H264的组成结构**

H264以NALU（NALunit）为单位来支持编码数据在基于分组交换技术网络中传输的。

NALU的结构是：NAL头+RBSP，实际传输中的数据流如图所示：


![264句法元素的分层结构](http://hi.csdn.net/attachment/201004/9/0_12708080040gID.gif)

NALU头用来标识后面的RBSP是什么类型的数据，他是否会被其他帧参考以及网络传输是否有错误。

NALU头结构

长度：1byte

forbidden_bit(1bit) + nal_reference_bit(2bit) + nal_unit_type(5bit)


1.forbidden_bit：                             禁止位，初始为0，当网络发现NAL单元有比特错误时可设置该比特为1，以便接收方纠错或丢掉该单元。

2.nal_reference_bit：                   nal重要性指示，标志该NAL单元的重要性，值越大，越重要，解码器在解码处理不过来的时候，可以丢掉重要性为0的NALU。

.nal_unit_type：NALU类型取值如下表所示。
|**nal_unit_type**|**NAL类型**|**C**|
|----|----|----|
|0|未使用||
|1|非IDR图像中不采用数据划分的片段|2,3,4|
|2|非IDR图像中A类数据划分片段|2|
|3|非IDR图像中B类数据划分片段|3|
|4|非IDR图像中C类数据划分片段|4|
|5|IDR图像的片|2,3|
|6|补充增强信息单元（SEI）|5|
|7|序列参数集|0|
|8|图像参数集|1|
|9|分界符|6|
|10|序列结束|7|
|11|码流结束|8|
|12|填充|9|
|13..23|保留||
|24..31|不保留（RTP打包时会用到）||

RBSP：RBSP数据是下表中的一种
|RBSP类型|所写|描述|
|----|----|----|
|参数集|PS|序列的全局信息，如图像尺寸，视频格式等|
|增强信息|SEI|视频序列解码的增强信息|
|图像界定符|PD|视频图像的边界|
|编码片|SLICE|编码片的头信息和数据|
|数据分割||DP片层的数据，用于错误恢复解码|
|序列结束符||表明一个序列的结束，下一个图像为IDR图像|
|流结束符||表明该流中已没有图像|
|填充数据||亚元数据，用于填充字节|

NAL的开始和结束


编码器将每个NAL各自独立、完整地放入一个分组，因为分组都有头部，解码器可以方便地检测出NAL的分界，并依次取出NAL进行解码。

每个NAL前有一个起始码 0x00 00 01（或者0x00 00 00 01），解码器检测每个起始码，作为一个NAL的起始标识，当检测到下一个起始码时，当前NAL结束。

同时H.264规定，当检测到0x000000时，也可以表征当前NAL的结束。那么NAL中数据出现0x000001或0x000000时怎么办？H.264引入了防止竞争机制，如果编码器检测到NAL数据存在0x000001或0x000000时，编码器会在最后个字节前插入一个新的字节0x03，这样：

0x000000－>0x00000300

0x000001－>0x00000301

0x000002－>0x00000302

0x000003－>0x00000303

解码器检测到0x000003时，把03抛弃，恢复原始数据（脱壳操作）。解码器在解码时，首先逐个字节读取NAL的数据，统计NAL的长度，然后再开始解码。



**二、RTP封包H264的方式**

**NALU打包成RTP的方式有三种：**

1. 单一 NAL 单元模式

     即一个RTP 包仅由一个完整的 NALU 组成. 这种情况下 RTP NAL 头类型字段和原始的 H.264的

NALU 头类型字段是一样的.

2. 组合封包模式

    即可能是由多个NAL 单元组成一个 RTP 包. 分别有4种组合方式: STAP-A, STAP-B, MTAP16, MTAP24.

那么这里的类型值分别是 24, 25, 26 以及 27.

3. 分片封包模式

    用于把一个NALU 单元封装成多个 RTP 包. 存在两种类型 FU-A 和 FU-B. 类型值分别是 28 和 29.

还记得前面nal_unit_type的定义吧，0~23是给H264用的，24~31未使用，在rtp打包时，如果一个NALU放在一个RTP包里，可以使用NALU的nal_unit_type，但是当需要把多个NALU打包成一个RTP包，或者需要把一个NALU打包成多个RTP包时，就定义新的type来标识。

*Type  Packet      Typename                           ---------------------------------------------------------     0     undefined                                   -     1-23   NAL unit    Single NAL unit packet perH.264       24     STAP-A     Single-timeaggregation packet         25     STAP-B     Single-timeaggregation packet         26     MTAP16    Multi-time aggregationpacket          27     MTAP24    Multi-time aggregationpacket          28     FU-A      Fragmentationunit                    29     FU-B      Fragmentationunit                     30-31 undefined                   *



**三种打包方式的具体格式**

1 .单一 NAL 单元模式

对于 NALU 的长度小于 MTU 大小的包, 一般采用单一 NAL 单元模式.

对于一个原始的 H.264 NALU 单元常由 [Start Code] [NALU Header] [NALU Payload] 三部分组成, 其中 Start Code 用于标示这是一个

NALU 单元的开始, 必须是 "00 00 00 01" 或 "00 00 01", NALU 头仅一个字节, 其后都是 NALU 单元内容.

打包时去除 "00 00 01" 或 "00 00 00 01" 的开始码, 把其他数据封包的 RTP 包即可.

      0                  1                  2                  3

       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      |F|NRI| type  |                                              |

     +-+-+-+-+-+-+-+-+                                              |

     |                                                              |

     |              Bytes 2..n of a Single NALunit                |

     |                                                              |

     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |                              :...OPTIONAL RTP padding        |

      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+



如有一个 H.264 的 NALU 是这样的:

[00 00 00 01 67 42 A0 1E 23 56 0E 2F... ]

这是一个序列参数集 NAL 单元. [00 00 00 01] 是四个字节的开始码,67 是 NALU 头, 42 开始的数据是 NALU 内容.

封装成 RTP 包将如下:

[ RTP Header ] [ 67 42 A0 1E 23 56 0E 2F]

即只要去掉 4 个字节的开始码就可以了.



2 组合封包模式

其次, 当 NALU 的长度特别小时, 可以把几个 NALU 单元封在一个 RTP 包中.



      0                  1                  2                  3

       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1

      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |                         RTP Header                          |

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      |STAP-A NAL HDR|         NALU 1Size           | NALU 1HDR    |

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |                        NALU 1 Data                          |

     :                                                              :

     +              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |              | NALU 2Size                  | NALU 2 HDR    |

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |                        NALU 2 Data                          |

      :                                                              :

     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |                              :...OPTIONAL RTP padding        |

      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+



3 FragmentationUnits (FUs).

而当 NALU 的长度超过 MTU 时, 就必须对 NALU 单元进行分片封包. 也称为 Fragmentation Units (FUs).


      0                  1                  2                  3

       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 01 2 3 4 5 6 7 8 9 0 1

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      | FU indicator |   FUheader  |                              |

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                              |

     |                                                              |

     |                        FU payload                           |

     |                                                              |

     |                              +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

     |                              :...OPTIONAL RTP padding        |

     +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      Figure 14. RTPpayload format for FU-A

**  FU indicator有以下格式：**

     +---------------+

      |0|1|2|3|4|5|6|7|

      +-+-+-+-+-+-+-+-+

      |F|NRI|  Type   |

      +---------------+

   FU指示字节的类型域 Type=28表示FU-A。。NRI域的值必须根据分片NAL单元的NRI域的值设置。



**   FU header的格式如下：**

     +---------------+

      |0|1|2|3|4|5|6|7|

      +-+-+-+-+-+-+-+-+

      |S|E|R|  Type   |

      +---------------+

   S: 1 bit

   当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。

   E: 1 bit

   当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节。当跟随的FU荷载不是分片NAL单元的最后分片,结束位设置为0。

   R: 1 bit

   保留位必须设置为0，接收者必须忽略该位。

   Type: 5 bits



**1、单个NAL包单元**

12字节的RTP头后面的就是音视频数据，比较简单。一个封装单个NAL单元包到RTP的NAL单元流的RTP序号必须符合NAL单元的解码顺序。

**2、FU-A的分片格式**

数据比较大的H264视频包，被RTP分片发送。12字节的RTP头后面跟随的就是FU-A分片：

FU indicator有以下格式：

      +---------------+

      |0|1|2|3|4|5|6|7|

      +-+-+-+-+-+-+-+-+

      |F|NRI|  Type   |

      +---------------+

   FU指示字节的类型域 Type=28表示FU-A。。NRI域的值必须根据分片NAL单元的NRI域的值设置。


   FU header的格式如下：

      +---------------+

      |0|1|2|3|4|5|6|7|

      +-+-+-+-+-+-+-+-+

      |S|E|R|  Type   |

      +---------------+

   S: 1 bit

   当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。

   E: 1 bit

   当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节。当跟随的FU荷载不是分片NAL单元的最后分片,结束位设置为0。

   R: 1 bit

   保留位必须设置为0，接收者必须忽略该位。

   Type: 5 bits

   NAL单元荷载类型定义见下表



表1.  单元类型以及荷载结构总结

      Type  Packet      Typename                      

     ---------------------------------------------------------

      0     undefined                                   -

      1-23   NALunit    Single NAL unit packet per H.264  

      24    STAP-A     Single-time aggregation packet   

      25    STAP-B     Single-time aggregation packet   

      26    MTAP16    Multi-time aggregation packet     

      27    MTAP24    Multi-time aggregation packet     

      28     FU-A     Fragmentation unit               

      29    FU-B      Fragmentationunit                

      30-31 undefined                                   -


**3、拆包和解包**

拆包：当编码器在编码时需要将原有一个NAL按照FU-A进行分片，原有的NAL的单元头与分片后的FU-A的单元头有如下关系：

原始的NAL头的前三位为FU indicator的前三位，原始的NAL头的后五位为FU header的后五位，FUindicator与FU header的剩余位数根据实际情况决定。


解包：当接收端收到FU-A的分片数据，需要将所有的分片包组合还原成原始的NAL包时，FU-A的单元头与还原后的NAL的关系如下：

还原后的NAL头的八位是由FU indicator的前三位加FU header的后五位组成，即：

nal_unit_type = (fu_indicator & 0xe0) | (fu_header & 0x1f)



**三、代码实现RTP发包和解包**

将H264以FU-A打包、发送RTP包的实现：

```
int SendH264File(const char * pszFile)
{
	OpenBitstreamFile((char*)pszFile);//打开264文件，并将文件指针赋给bits,在此修改文件名实现打开别的264文件。
	NALU_t *n;
	char* nalu_payload;
	char sendbuf[1500];

	unsigned short seq_num = 0;
	int	bytes = 0;
	//InitWinsock(); //初始化套接字库

	SOCKET    socket1;
	struct sockaddr_in server;
	int len = sizeof(server);

	float framerate = 25.0f; //帧率
	unsigned int timestamp_increse = 0, ts_current = 0;
	timestamp_increse = (unsigned int)(90000.0 / framerate); //+0.5);  //时间戳，H264的视频设置成90000

	int nFrameDuration = 1000/ framerate; //相邻两帧间隔时间, 单位毫秒

	server.sin_family = AF_INET;
	server.sin_port = htons(m_nDestPort);
	server.sin_addr.s_addr = inet_addr(m_szDestIP);//接收端IP

	socket1 = socket(AF_INET, SOCK_DGRAM, 0); //申请UDP套接字
	int nOK = connect(socket1, (const sockaddr *)&server, len);//连接接收端
	if (nOK < 0)
	{
		TRACE("对方IP:%s 不可访问!!! \n", m_szDestIP);
		return -1;
	}
	
	RTP_FIXED_HEADER        *rtp_hdr;

	NALU_HEADER		*nalu_hdr;
	FU_INDICATOR	*fu_ind;
	FU_HEADER		*fu_hdr;

	n = AllocNALU(8000000);//为结构体nalu_t及其成员buf分配空间。返回值为指向nalu_t存储空间的指针
	
	while (!feof(bits) && g_RTPSendThreadRun)
	{
		GetAnnexbNALU(n);//每执行一次，文件的指针指向本次找到的NALU的末尾，下一个位置即为下个NALU的起始码0x000001
		//dump(n);//输出NALU长度和TYPE

		//TRACE("nalu len: %d, nal_unit_type: %x\n",n->len, n->nal_unit_type);
		
		//（1）一个NALU就是一个RTP包的情况： RTP_FIXED_HEADER（12字节）  + NALU_HEADER（1字节） + EBPS
		//（2）一个NALU分成多个RTP包的情况： RTP_FIXED_HEADER （12字节） + FU_INDICATOR （1字节）+  FU_HEADER（1字节） + EBPS(1400字节)

		memset(sendbuf, 0, 1500);//清空sendbuf；此时会将上次的时间戳清空，因此需要ts_current来保存上次的时间戳值
								 //rtp固定包头，为12字节,该句将sendbuf[0]的地址赋给rtp_hdr，以后对rtp_hdr的写入操作将直接写入sendbuf。
		rtp_hdr = (RTP_FIXED_HEADER*)&sendbuf[0];
		//设置RTP HEADER，
		rtp_hdr->payload = H264;  //负载类型号，
		rtp_hdr->version = 2;  //版本号，此版本固定为2
		rtp_hdr->marker = 0;   //标志位，由具体协议规定其值。
		rtp_hdr->ssrc = htonl(10);    //随机指定为10，并且在本RTP会话中全局唯一

									  //	当一个NALU小于1400字节的时候，采用一个单RTP包发送
		if (n->len <= 1400)
		{
			//设置rtp M 位；
			rtp_hdr->marker = 1;
			rtp_hdr->seq_no = htons(seq_num++); //序列号，每发送一个RTP包增1，htons，将主机字节序转成网络字节序。
												//设置NALU HEADER,并将这个HEADER填入sendbuf[12]
			nalu_hdr = (NALU_HEADER*)&sendbuf[12]; //将sendbuf[12]的地址赋给nalu_hdr，之后对nalu_hdr的写入就将写入sendbuf中；
			nalu_hdr->F = n->forbidden_bit;
			nalu_hdr->NRI = n->nal_reference_idc >> 5;//有效数据在n->nal_reference_idc的第6，7位，需要右移5位才能将其值赋给nalu_hdr->NRI。
			nalu_hdr->TYPE = n->nal_unit_type;

			nalu_payload = &sendbuf[13];//同理将sendbuf[13]赋给nalu_payload
			memcpy(nalu_payload, n->buf + 1, n->len - 1);//去掉nalu头的nalu剩余内容写入sendbuf[13]开始的字符串。

			ts_current = ts_current + timestamp_increse;
			rtp_hdr->timestamp = htonl(ts_current);
			bytes = n->len + 12;	//获得sendbuf的长度,为nalu的长度（包含NALU头但除去起始前缀）加上rtp_header的固定长度12字节
			send(socket1, sendbuf, bytes, 0);//发送rtp包
											 
			//Sleep(nFrameDuration);
			Sleep(1);
		}

		else if (n->len > 1400)  //这里就要分成多个RTP包发送了。
		{
			//得到该nalu需要用多少长度为1400字节的RTP包来发送
			int k = 0, last = 0;
			k = n->len / 1400;//需要k个1400字节的RTP包，这里为什么不加1呢？因为是从0开始计数的。
			last = n->len % 1400;//最后一个RTP包的需要装载的字节数
			int t = 0;//用于指示当前发送的是第几个分片RTP包
			ts_current = ts_current + timestamp_increse;
			rtp_hdr->timestamp = htonl(ts_current);

	
			while (t <= k)
			{
				if (t == k && last == 0)
					break;

				rtp_hdr->seq_no = htons(seq_num++); //序列号，每发送一个RTP包增1
				if (!t)//发送一个需要分片的NALU的第一个分片，置FU HEADER的S位,t = 0时进入此逻辑。
				{
					//设置rtp M 位；
					rtp_hdr->marker = 0;  //最后一个NALU时，该值设置成1，其他都设置成0。
										  //设置FU INDICATOR,并将这个HEADER填入sendbuf[12]
					fu_ind = (FU_INDICATOR*)&sendbuf[12]; //将sendbuf[12]的地址赋给fu_ind，之后对fu_ind的写入就将写入sendbuf中；
					fu_ind->F = n->forbidden_bit;
					fu_ind->NRI = n->nal_reference_idc >> 5;
					fu_ind->TYPE = 28;  //FU-A类型。

										//设置FU HEADER,并将这个HEADER填入sendbuf[13]
					fu_hdr = (FU_HEADER*)&sendbuf[13];
					fu_hdr->E = 0;
					fu_hdr->R = 0;
					fu_hdr->S = 1;
					fu_hdr->TYPE = n->nal_unit_type;

					nalu_payload = &sendbuf[14];//同理将sendbuf[14]赋给nalu_payload
					memcpy(nalu_payload, n->buf + 1, 1400);//去掉NALU头，每次拷贝1400个字节。

					bytes = 1400 + 14;//获得sendbuf的长度,为nalu的长度（除去起始前缀和NALU头）加上rtp_header，fu_ind，fu_hdr的固定长度                                                            14字节
					send(socket1, sendbuf, bytes, 0);//发送rtp包
					t++;

				}
				//发送一个需要分片的NALU的非第一个分片，清零FU HEADER的S位，如果该分片是该NALU的最后一个分片，置FU HEADER的E位
				else if (k == t)//发送的是最后一个分片，注意最后一个分片的长度可能超过1400字节（当 l> 1386时）。
				{

					//设置rtp M 位；当前传输的是最后一个分片时该位置1
					rtp_hdr->marker = 1;
					//设置FU INDICATOR,并将这个HEADER填入sendbuf[12]
					fu_ind = (FU_INDICATOR*)&sendbuf[12]; //将sendbuf[12]的地址赋给fu_ind，之后对fu_ind的写入就将写入sendbuf中；
					fu_ind->F = n->forbidden_bit;
					fu_ind->NRI = n->nal_reference_idc >> 5;
					fu_ind->TYPE = 28;

					//设置FU HEADER,并将这个HEADER填入sendbuf[13]
					fu_hdr = (FU_HEADER*)&sendbuf[13];
					fu_hdr->R = 0;
					fu_hdr->S = 0;
					fu_hdr->TYPE = n->nal_unit_type;
					fu_hdr->E = 1;

					ASSERT(last > 0);

					nalu_payload = &sendbuf[14];//同理将sendbuf[14]的地址赋给nalu_payload
					memcpy(nalu_payload, n->buf + t * 1400 + 1, last - 1);//将nalu最后剩余的l-1(去掉了一个字节的NALU头)字节内容写入sendbuf[14]开始的字符串。
					bytes = last - 1 + 14;		//获得sendbuf的长度,为剩余nalu的长度l-1加上rtp_header，FU_INDICATOR,FU_HEADER三个包头共14字节
					send(socket1, sendbuf, bytes, 0);//发送rtp包
					t++;
					
					Sleep(/*nFrameDuration*/10);
				}
				//既不是第一个分片，也不是最后一个分片的处理。
				else if (t < k && 0 != t)
				{
					//设置rtp M 位；
					rtp_hdr->marker = 0;
					//设置FU INDICATOR,并将这个HEADER填入sendbuf[12]
					fu_ind = (FU_INDICATOR*)&sendbuf[12]; //将sendbuf[12]的地址赋给fu_ind，之后对fu_ind的写入就将写入sendbuf中；
					fu_ind->F = n->forbidden_bit;
					fu_ind->NRI = n->nal_reference_idc >> 5;
					fu_ind->TYPE = 28;

					//设置FU HEADER,并将这个HEADER填入sendbuf[13]
					fu_hdr = (FU_HEADER*)&sendbuf[13];

					fu_hdr->R = 0;
					fu_hdr->S = 0;
					fu_hdr->E = 0;
					fu_hdr->TYPE = n->nal_unit_type;

					nalu_payload = &sendbuf[14];//同理将sendbuf[14]的地址赋给nalu_payload
					memcpy(nalu_payload, n->buf + t * 1400 + 1, 1400);//去掉起始前缀的nalu剩余内容写入sendbuf[14]开始的字符串。
					bytes = 1400 + 14;						//获得sendbuf的长度,为nalu的长度（除去原NALU头）加上rtp_header，fu_ind，fu_hdr的固定长度14字节
					send(socket1, sendbuf, bytes, 0);//发送rtp包
					t++;
				}
			}
		}
	}
	FreeNALU(n);
	return 0;
}
```

在接收端**解包**得到H264数据的实现：

```
rtp_hdr = (RTP_FIXED_HEADER*)&recvbuf[0];
		//TRACE("版本号 : %d\n", rtp_hdr->version);
		//TRACE("结束标志位 : %d\n", rtp_hdr->marker);
		//TRACE("负载类型:%d\n", rtp_hdr->payload);
		//TRACE("包号   : %d \n", htons(rtp_hdr->seq_no));
		//TRACE("时间戳 : %d\n", htonl(rtp_hdr->timestamp));
		//TRACE("同步标识符   : %d\n", htonl(rtp_hdr->ssrc));

		rtpMarkerBit = 0;
		int nBytes = rtp_unpackage_H264((unsigned char*)(recvbuf + 12), receive_bytes - 12, rtpMarkerBit, cFrameType, temp_buffer, temp_size); //重新组装H264的NALU单元
```

```
int rtp_unpackage_H264(unsigned char *payload, int len, int & marker, unsigned char & cFrameType, unsigned char * outbuf, int & total_bytes)
{
	NALU_HEADER * nalu_hdr = NULL;
	NALU_t  nalu_data = { 0 };
	NALU_t * n = &nalu_data;
	FU_INDICATOR	*fu_ind = NULL;
	FU_HEADER		*fu_hdr = NULL;
	total_bytes = 0;

	unsigned int h264_startcode = 0x01000000;

	nalu_hdr = (NALU_HEADER*)&payload[0];                        //网络传输过来的字节序 ，当存入内存还是和文档描述的相反，只要匹配网络字节序和文档描述即可传输正确。
																 //TRACE("forbidden_zero_bit: %d\n",nalu_hdr->F);              //网络传输中的方式为：F->NRI->TYPE.. 内存中存储方式为 TYPE->NRI->F (和nal头匹配)。
	n->forbidden_bit = nalu_hdr->F << 7;                          //内存中的字节序。
																  //TRACE("nal_reference_idc:  %d\n",nalu_hdr->NRI);
	n->nal_reference_idc = nalu_hdr->NRI << 5;
	//TRACE("nal 负载类型:       %d\n",nalu_hdr->TYPE);
	n->nal_unit_type = nalu_hdr->TYPE;


	if (nalu_hdr->TYPE == 0)
	{
		TRACE("这个包有错误，0无定义\n");
	}
	else if (nalu_hdr->TYPE >0 && nalu_hdr->TYPE < 24)  //单包
	{
		//TRACE("当前包为单包\n");

		marker = 1;
		//ASSERT(marker == 1); //单包的Marker为1，但少部分服务器会置为0

		memcpy(outbuf + total_bytes, &h264_startcode, 4);
		total_bytes += 4;

		memcpy(outbuf + total_bytes, nalu_hdr, 1);
		total_bytes += 1;

		memcpy(outbuf + total_bytes, &payload[1], len - 1);
		total_bytes += len - 1;

		int nIDR = (n->nal_unit_type & 0x1f); //帧类型
		if (nIDR == 5)
			cFrameType = 'I';
		else
			cFrameType = 'P';
	}
	else if (nalu_hdr->TYPE == 24)                    //STAP-A   单一时间的组合包
	{
		TRACE("当前包为STAP-A\n");
	}
	else if (nalu_hdr->TYPE == 25)                    //STAP-B   单一时间的组合包
	{
		TRACE("当前包为STAP-B\n");
	}
	else if (nalu_hdr->TYPE == 26)                     //MTAP16   多个时间的组合包
	{
		TRACE("当前包为MTAP16\n");
	}
	else if (nalu_hdr->TYPE == 27)                    //MTAP24   多个时间的组合包
	{
		TRACE("当前包为MTAP24\n");
	}
	else if (nalu_hdr->TYPE == 28)                    //FU-A分片包，解码顺序和传输顺序相同
	{

		fu_ind = (FU_INDICATOR*)&payload[0];
		//printf(("FU_INDICATOR->F     :%d\n",fu_ind->F);
		n->forbidden_bit = fu_ind->F << 7;
		//printf(("FU_INDICATOR->NRI   :%d\n",fu_ind->NRI);
		n->nal_reference_idc = fu_ind->NRI << 5;
		//printf(("FU_INDICATOR->TYPE  :%d\n",fu_ind->TYPE);
		n->nal_unit_type = fu_ind->TYPE;

		fu_hdr = (FU_HEADER*)&payload[1];
		//printf(("FU_HEADER->S        :%d\n",fu_hdr->S);
		//printf(("FU_HEADER->E        :%d\n",fu_hdr->E);
		//printf(("FU_HEADER->R        :%d\n",fu_hdr->R);
		//printf(("FU_HEADER->TYPE     :%d\n",fu_hdr->TYPE);
		n->nal_unit_type = fu_hdr->TYPE;               //应用的是FU_HEADER的TYPE

		int nIDR = (n->nal_unit_type & 0x1f); //帧类型
		if (nIDR == 5)
			cFrameType = 'I';
		else
			cFrameType = 'P';

		if (marker == 1)      //分片包最后一个包
		{
			//printf(("当前包为FU-A分片包最后一个包\n");

			memcpy(outbuf + total_bytes, &payload[2], len - 2);
			total_bytes += len - 2;
		}
		else if (marker == 0)                 //分片包 但不是最后一个包
		{
			if (fu_hdr->S == 1)                        //分片的第一个包
			{
				//printf(("当前包为FU-A分片包第一个包\n");
				unsigned char F;
				unsigned char NRI;
				unsigned char TYPE;
				unsigned char nh;

				memcpy(outbuf + total_bytes, &h264_startcode, 4);
				total_bytes += 4;

				F = fu_ind->F << 7;
				NRI = fu_ind->NRI << 5;
				TYPE = fu_hdr->TYPE;                                            //应用的是FU_HEADER的TYPE
																				//nh = n->forbidden_bit|n->nal_reference_idc|n->nal_unit_type;  //二进制文件也是按 大字节序存储
				nh = F | NRI | TYPE;

				memcpy(outbuf + total_bytes, &nh, 1);
				total_bytes += 1;

				memcpy(outbuf + total_bytes, &payload[2], len - 2);
				total_bytes += len - 2;

			}
			else         //如果不是第一个包
			{
				//printf(("当前包为FU-A分片包\n");

				memcpy(outbuf + total_bytes, &payload[2], len - 2);
				total_bytes += len - 2;

			}
		}
	}
	else if (nalu_hdr->TYPE == 29)                //FU-B分片包，解码顺序和传输顺序相同
	{
		if (marker == 1)                  //分片包最后一个包
		{
			TRACE("当前包为FU-B分片包最后一个包\n");

		}
		else if (marker == 0)             //分片包 但不是最后一个包
		{
			TRACE("当前包为FU-B分片包\n");
		}
	}
	else
	{
		TRACE("这个包有错误，30-31 没有定义\n");
	}

	return total_bytes;
}
```

**四、播放器的实现**

这个播放器的界面如下所示：

![](https://img-blog.csdnimg.cn/20190116182247175.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pob3Vib3RvbmcyMDEy,size_16,color_FFFFFF,t_70)

这个播放器主要演示播放H264（用RTP封装）的网络流的功能。除了接收H264数据，也集成了发送H264的功能，读者从界面上选择一个H264格式的视频文件（必须是H264格式），并且指定发送的目标IP和目标端口号，这里的IP可以选本机IP，然后点击发送，则程序会解析H264文件，从中提取出NALU单元，以FU-A方式打包，然后加上RTP头，通过UDP将数据发送到目标地址。在本程序的接收端，只需要配置接收端口，然后点击“开始接收“按钮则开始接收数据，注意目标端口要和接收端口一致，发送和接收是一对一的。

接收端用到了多线程技术和缓冲队列，用到两条线程，一条用于接收RTP包，从中提取出视频数据；另一条线程用于解码视频，并把视频帧转成RGB格式后显示到窗口中。用到两条线程的好处是：可以并行接收和解码，两个工作相互独立，提高视频帧的处理效率，减少播放延时。而如果用一条线程来做，它既要接收又要解码，线程中处理一个帧的时间就长一些，而这时又不能接收数据，很可能造成后面的数据包丢掉。所以，用双线程的”分工合作“方式处理效率更高。两条线程之间需要维护一个队列，其中一条线程收到数据后放到队列里，然后另外一个线程从队列里读取数据，这是一个典型的”生产者-消费者“的模型，我们需要实现一个先入先出的队列来转运”视频帧“，这个队列的定义如下：

std::list<PacketNode_t>  m_packetList; //包列表

其中，PacketNode_t结构体的定义为：

typedef struct

{

    unsigned length;

    uint8_t *buf;

}PacketNode_t;



在接收端开始接收的时候，创建两个线程：

    g_RTPRecvThreadRun = true;

    g_decoding_thread_run = true;


    DWORD threadID = 0;

    m_hRecvThread   = CreateThread(NULL, 0, RTPRecvThread, this, 0, &threadID);

    m_hDecodeThread = CreateThread(NULL, 0, decoding_thread, this, 0, &threadID);

RTPRecvThread是RTP数据的接收线程，实现方式如下：

```
DWORD WINAPI RTPRecvThread(void* param)
{
	TRACE("RTPRecvThread began! \n");

	CPlayStreamDlg * pThisDlg = (CPlayStreamDlg*)param;

	//int Error;
	//WORD Version = MAKEWORD(2, 2);
	//WSADATA WsaData;
	//Error = WSAStartup(Version, &WsaData);	//Init winsock  
	//if (Error != 0)
	//	return Error;

	char recvbuf[4*1024];
	SOCKET  socket1;
	SOCKADDR_IN client;//分配一个地址结构体
	int len_client = sizeof(client);
	int	receive_bytes = 0;

	socket1 = socket(AF_INET, SOCK_DGRAM, 0);

	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(m_nRecvPort);

	if (bind(socket1, (struct sockaddr*)&client, sizeof(client)) == -1)
	{
		TRACE("Bind to local machine error.\n");
		return -2;
	}
	else
	{
		TRACE("Bind to local machine.\n");
	}

	//设置接收超时，否则如果没有数据，则会一直卡在recvfrom接收函数
	int timeout = 2000; //单位：毫秒
	if (setsockopt(socket1, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR)
	{
		return -3;
	}

	//设置接收缓冲区大小, 设大一点可以减少丢包频率
	int nRecvBuf = 512 * 1024;
	if(setsockopt(socket1, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) == SOCKET_ERROR)
	{
		return -4;
	}

	RTP_FIXED_HEADER * rtp_hdr = NULL;
	int rtpMarkerBit;
	unsigned char cFrameType = '0';

	unsigned char temp_buffer[1500];
	memset(temp_buffer, 0, 1500);
	int temp_size = 0;

	unsigned char * frame_buffer = (unsigned char *)malloc(500*1024); //存储完整的一帧H264数据
	int frame_size = 0;
	int nLastSeqNo = -1;

	while (g_RTPRecvThreadRun)
	{
		receive_bytes = recvfrom(socket1, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&client, &len_client);
		if (receive_bytes <= 0)
		{
			if (WSAGetLastError() == 10060) //接收超时
			{
				Sleep(1);
				continue;
			}
			break;
		}

		rtp_hdr = (RTP_FIXED_HEADER*)&recvbuf[0];
		//TRACE("版本号 : %d\n", rtp_hdr->version);
		//TRACE("结束标志位 : %d\n", rtp_hdr->marker);
		//TRACE("负载类型:%d\n", rtp_hdr->payload);
		//TRACE("包号   : %d \n", htons(rtp_hdr->seq_no));
		//TRACE("时间戳 : %d\n", htonl(rtp_hdr->timestamp));
		//TRACE("同步标识符   : %d\n", htonl(rtp_hdr->ssrc));

		rtpMarkerBit = 0;
		int nBytes = rtp_unpackage_H264((unsigned char*)(recvbuf + 12), receive_bytes - 12, rtpMarkerBit, cFrameType, temp_buffer, temp_size); //重新组装H264的NALU单元
		if (nBytes <= 0)
		{
			TRACE("rtp_unpackage_H264 error!! \n");
			continue;
	    }

		if (rtpMarkerBit == 1)
		{
			memcpy(frame_buffer + frame_size, temp_buffer, temp_size);
			frame_size += temp_size;

#if 0
			Writebuf((char*)pPayData, nLen);
#else			
			pThisDlg->m_cs.Lock();
			//if (pThisDlg->m_packetList.size() < MAX_PACKET_COUNT)
			{
				PacketNode_t  temNode;
				temNode.length = frame_size;
				temNode.buf = new uint8_t[frame_size];
				memcpy(temNode.buf, frame_buffer, frame_size);

				pThisDlg->m_packetList.push_back(temNode); //存包列表
			}
			pThisDlg->m_cs.Unlock();
#endif

			frame_size = 0; //重新将帧大小清零
		}
		else
		{
			memcpy(frame_buffer + frame_size, temp_buffer, temp_size);
			frame_size += temp_size;
		}
	}

	closesocket(socket1);
	free(frame_buffer);

	//WSACleanup(); //释放WinSock资源

	TRACE("RTPRecvThread end! \n");
	return 0;
}
```

上面代码里调用rtp_unpackage_H264对RTP包的负载数据解包，还原真正的NALU单元，这里要注意：rtp_unpackage_H264每次返回可能收到的是NALU单元的部分数据，这时候还不能送给解码器去解码，因为H264解码器需要一个完整的NALU单元才能正常解码，所以我们必须对收到的数据进行缓存并把同一帧的数据“拼装”起来。那怎么知道哪些包是属于同一个NALU的？方法是看MarkerBit位，如果值为1，表示帧的结束包，所以前面收到的包就可以组成一个NALU或一帧（或者通过看RTP包头里面的时间戳也可以知道帧的边界，同一帧的时间戳是相同的）。当组成完整的一个NALU或完整的一帧，就可以放到队列里，然后抛给解码线程去解码了。

解码线程**decoding_thread**，这个线程主要执行流程是：打开一个文件源或URL地址-》从源中读取各个流的信息-》初始化解码器-》解码和显示。因为我们是从网络中收数据，所以是一个网络源，从网络源中读取数据有两种方式：一种是用FFmpeg内置的协议栈的支持，比如RTSP/RTMP/RTP，还有一种方式是我们传数据给FFmpeg，FFmpeg从内存中读取我们送的数据，然后用它的Demuxer和Parser来进行分析，分离出视频和音频。这里程序使用的是第二种方式，即从网络中探测数据，然后送数据给FFmpeg去解析。探测网络数据需要调用FFmpeg的av_probe_input_buffer函数，这个函数要传入一个内存缓冲区地址和一个回调函数指针，其中回调函数是用来从网络中读数据的（即我们放到缓冲队列里的数据包）。

下面的fill_iobuffer就是读数据的回调函数，而pIOBuffer指向用于存放读取数据的缓冲区地址，FFmpeg就是从这里读取数据。

    pIObuffer = (uint8_t*)av_malloc(4096);

    pb = avio_alloc_context(

        pIObuffer,

        4096,

        0,

        param,

        fill_iobuffer,

        NULL,

        NULL);


    if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)//探测从内存中获取到的媒体流的格式

    {

        TRACE("Error: probe format failed\n");

        return -1;

    }

    else {

        TRACE("input format:%s[%s]\n", piFmt->name, piFmt->long_name);


    }

回调函数fill_iobuffer调用了一个ReadBuf的函数：

int fill_iobuffer(void* opaque, uint8_t* buf, int bufSize)

{

    ASSERT(opaque != NULL);

    CPlayStreamDlg* p_CPSDecoderDlg = (CPlayStreamDlg*)opaque;


    //TRACE("ReadBuf----- \n");

    int nBytes = ReadBuf((char*)buf, bufSize, (void*)p_CPSDecoderDlg);

    return (nBytes > 0) ? bufSize : -1;

}

static int ReadBuf(char* data, int len, void* pContext)

{

    CPlayStreamDlg * pThisDlg = (CPlayStreamDlg*)pContext;


    int data_to_read = len;

    char * pReadPtr = data;


    while (g_RTPRecvThreadRun)

    {

        int nRead = pThisDlg->ReadNetPacket((uint8_t*)pReadPtr, data_to_read);

        if (nRead < 0)

        {

            Sleep(10);

            continue;

        }

        pReadPtr += nRead;

        data_to_read -= nRead;

        if (data_to_read > 0)

        {

            Sleep(10);

            continue;

        }

        break;

    }


    return (data_to_read > 0) ? -1 : len;

}

ReadBuf函数的作用就不用解释了，大家一看就明白了。它实现了一个我们前面说的“消费者”，从前面接收线程插入数据进去的缓冲队列中读取数据包，读取之后从队列中删除相应的元素。如果队列不为空，则直接从前面的元素读取；如果无数据，则继续等待。

解码线程的完整实现代码如下：

```
DWORD WINAPI decoding_thread(void* param)
{
	TRACE("decoding_thread began! \n");

	CPlayStreamDlg * pThisDlg = (CPlayStreamDlg*)param;
	HWND hwnd = (HWND)pThisDlg->m_playing_HWND; //预览视频的句柄

	uint8_t* pIObuffer;
	AVIOContext* pb = NULL;
	AVInputFormat* piFmt = NULL;
	AVFormatContext* pFormatContext;
	int m_result;
	enum AVCodecID avCodecID;
	unsigned int video_stream_index;


	av_register_all();
	
	DWORD dwTick1, dwTick2, dwTick3;

	dwTick1 = GetTickCount();

	pIObuffer = (uint8_t*)av_malloc(4096);
	pb = avio_alloc_context(
		pIObuffer,
		4096,
		0,
		param,
		fill_iobuffer,
		NULL,
		NULL);

	if (av_probe_input_buffer(pb, &piFmt, "", NULL, 0, 0) < 0)//探测从内存中获取到的媒体流的格式
	{
		TRACE("Error: probe format failed\n");
		return -1;
	}
	else {
		TRACE("input format:%s[%s]\n", piFmt->name, piFmt->long_name);

	}

	pFormatContext = avformat_alloc_context();
	pFormatContext->pb = pb;

	pFormatContext->max_analyze_duration = 2000000;
	pFormatContext->fps_probe_size = 5;

	m_result = avformat_open_input(&pFormatContext, "", piFmt, NULL);
	if (m_result < 0)
	{
		OutputDebugString(_T("Error: avformat_open_input failed----------------- \n"));
		//ASSERT(0);
		return -1;
	}

	if (g_decoding_thread_run == false)
		return 0;

	dwTick2 = GetTickCount();
	TRACE("avformat_open_input() used time: %ldms \n", dwTick2 - dwTick1);

	m_result = avformat_find_stream_info(pFormatContext, NULL);
	if (m_result < 0)
	{
		OutputDebugString(_T("Error: avformat_find_stream_info failed----------------- \n"));
		ASSERT(0);
		return -1;
	}
	for (video_stream_index = 0; video_stream_index < pFormatContext->nb_streams; video_stream_index++)
	{
		if (AVMEDIA_TYPE_VIDEO == pFormatContext->streams[video_stream_index]->codec->codec_type)
		{
			avCodecID = pFormatContext->streams[video_stream_index]->codec->codec_id;
			break;
		}
	}
	dwTick3 = GetTickCount();
	TRACE("avformat_find_stream_info() used time: %ld ms, CodecID: %x \n", dwTick3 - dwTick2, avCodecID);

	int got_picture;
	AVPacket* pAVPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
	
	AVCodec* pCodec = avcodec_find_decoder(avCodecID);
	AVCodecContext* pCodecCtx = pFormatContext->streams[video_stream_index]->codec;
	AVFrame* pFrame = av_frame_alloc();

	uint8_t* RGB24Data[4];
	int RGB24Linesize[4];
	got_picture = av_image_alloc(RGB24Data, RGB24Linesize, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, 4);
	struct SwsContext* img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	BITMAPINFO bmpinfo;
	memset(&bmpinfo, 0x0, sizeof(BITMAPINFOHEADER));
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biClrImportant = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biHeight = pCodecCtx->height;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biSizeImage = pCodecCtx->width * pCodecCtx->height * 3;
	bmpinfo.bmiHeader.biWidth = pCodecCtx->width;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;

	int PlayingWidth;
	int PlayingHeight;

	RECT rect;
	GetWindowRect(hwnd, &rect);
	PlayingWidth = rect.right - rect.left;
	PlayingHeight = rect.bottom - rect.top;

	HDC hDC = GetDC(hwnd);
	got_picture = SetStretchBltMode(hDC, COLORONCOLOR);

	got_picture = avcodec_open2(pCodecCtx, pCodec, NULL);

	uint8_t* p_uint8_t_temp;

	while (g_decoding_thread_run)
	{
		av_read_frame(pFormatContext, pAVPacket);
		if(pAVPacket->stream_index == video_stream_index)
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, pAVPacket);
			if(got_picture)
			{
				p_uint8_t_temp = pFrame->data[1];
				pFrame->data[1] = pFrame->data[2];
				pFrame->data[2] = p_uint8_t_temp;
				pFrame->data[0] += pFrame->linesize[0] * (pCodecCtx->height - 1);
				pFrame->linesize[0] *= -1;
				pFrame->data[1] += pFrame->linesize[1] * (pCodecCtx->height / 2 - 1);
				pFrame->linesize[1] *= -1;
				pFrame->data[2] += pFrame->linesize[2] * (pCodecCtx->height / 2 - 1);
				pFrame->linesize[2] *= -1;
				got_picture = sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, RGB24Data, RGB24Linesize);
				got_picture = StretchDIBits(hDC, 0, 0, PlayingWidth, PlayingHeight, 0, 0, pCodecCtx->width, pCodecCtx->height, RGB24Data[0], (BITMAPINFO*)&bmpinfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}

		av_free_packet(pAVPacket);
	}


	if (pFormatContext)
	{
		avformat_close_input(&pFormatContext);
		pFormatContext = NULL;
	}

	sws_freeContext(img_convert_ctx);
	av_freep(&RGB24Data[0]);
	av_frame_free(&pFrame);
	//avcodec_close(pCodecCtx);
	//av_free(pIObuffer); //调用了avformat_close_input会自动释放pIObuffer
	ReleaseDC(hwnd, hDC);

	TRACE("decoding_thread end! \n");
	return 0;
}
```



到此为止，一个简单的流媒体播放器的实现过程就介绍完了。

**关于代码优化的地方：**

  用这个播放器程序播放网络流在出图像前会有一定的延迟时间，主要是因为FFmpeg需要缓存一段数据来探测格式，但是因为我们已经知道数据源是H264编码，就不需要探测这个过程，所以av_probe_input_buffer，avformat_open_input和 avformat_find_stream_info这几个比较花时间的函数调用都可以去掉。我们可以直接初始化解码器为H264，然后进入接收循环，FFmpeg解码出第一帧即可以获得视频流的基本信息（比如图像宽、高）。这样实现过程更为简单，并且出图像的速度也会快些。

该播放器的代码下载地址：[https://download.csdn.net/download/zhoubotong2012/10918959](https://download.csdn.net/download/zhoubotong2012/10918959)






