# Hex文件转Bin文件 - xqhrs232的专栏 - CSDN博客
2017年05月21日 22:05:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：244
原文地址::[http://blog.csdn.net/zf_c_cqupt/article/details/52676716](http://blog.csdn.net/zf_c_cqupt/article/details/52676716)
相关文章
1、bin文件转换为hex格式文件----[http://download.csdn.net/download/ping630480256/2609334](http://download.csdn.net/download/ping630480256/2609334)
在[嵌入式开发](http://lib.csdn.net/base/embeddeddevelopment)中，编译器生成的目标文件一般都是 .hex 文件。 
为什么要转换，直接使用hex文件不行吗，可是我在开发过程中一直都是直接生成hex文件，然后进行下载，也没见出错？ 
在不清楚hex与bin文件的格式时，可能小伙伴会有这样的疑问。需要进行转换的原因是：hex文件中数据记录（record）并不是按照 “起始地址–>终止地址” 这样的顺序进行排列的，由于每行数据都包含起始地址和数据长度，所以hex文件中数据不需要按照地址顺序从低到高进行排列；而Bin文件中的数据则是严格按照地址顺序进行排列的。
首先需要了解hex文件的格式，可以参考官方资料[Intel Hexadecimal Object File Format Specification](http://www.interlog.com/~speff/usefulinfo/Hexfrmt.pdf)，也可以看我的另一篇blog——[HEX文件说明](http://blog.csdn.net/zf_c_cqupt/article/details/52321881) 。
最近做ECU的上位机下载工具，其中一步是将hex文件的内容转成按地址顺序（从低到高）排列的二进制数据（bin文件）。 
于是我找了一个转换工具——[hex2bin](http://hex2bin.sourceforge.net/)，[源码地址](https://sourceforge.net/projects/hex2bin/)。下面我就该工具的整个转换过程进行一些分析。 
整个过程主要分为两步： 
1. 遍历整个hex文件，找出最小地址和最大地址（也就是起始地址和结束地址），算出数据长度（数据长度=结束地址-起始地址），根据得到的数据长度，分配对应大小的内存（开辟一个数组）； 
2. 再次遍历整个hex文件，计算每条数据记录中的起始地址与hex文件起始地址的偏移量，按照偏移量将该条数据记录中的数据部分写入第一步的数组中。（这样就实现了按照从低到高的地址顺序排列整个hex文件的数据）。 
最后只需要将该数组写出到文件中即可。
首先使用`FILE * fopen(const char * path, const char * mode);`打开hex文件，然后是第一次遍历，找出起始地址和数据长度。
```cpp
/* 第一次遍历hex文件，获取地址范围(Lowest_Address和Highest_Address) */
    /* get highest and lowest addresses so that we can allocate the right size */
    do
    {
        unsigned int i;
        /* Read a line from input file. */
        GetLine(Line,Filin);
        Record_Nb++;
        /* Remove carriage return/line feed(回车/换行) at the end of line. */
        i = strlen(Line);
        if (--i != 0)
        {
            if (Line[i] == '\n') Line[i] = '\0';
            /* Scan the first two bytes and nb of bytes.
               The two bytes are read in First_Word since its use depend on the
               record type: if it's an extended address record or a data record.
               */
            /* sscanf() - 从一个字符串中读进与指定格式匹配的数据, 成功则返回参数数目.
               ":%2x%4x%2x%s":格式说明 :冒号开头,2个十六进制数,4个十六进制数, 2个十六进制数,余下的当做字符串  */
            result = sscanf (Line, ":%2x%4x%2x%s",&Nb_Bytes,&First_Word,&Type,Data_Str);
            if (result != 4) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
            p = (char *) Data_Str; //p表示指向数据域(包括checksum)的指针
            /* If we're reading the last record, ignore it. */
            switch (Type)
            {
            /* Data record */
            case 0:
                if (Nb_Bytes == 0)
                    break;
                Address = First_Word;
                if (Seg_Lin_Select == SEGMENTED_ADDRESS)
                {
                    Phys_Addr = (Segment << 4) + Address;
                }
                else
                {
                    /* LINEAR_ADDRESS or NO_ADDRESS_TYPE_SELECTED
                       Upper_Address = 0 as specified in the Intel spec. until an extended address
                       record is read. */
                    Phys_Addr = ((Upper_Address << 16) + Address);
                }
                if (Verbose_Flag) fprintf(stderr,"Physical Address: %08X\n",Phys_Addr);
                /* 获取地址范围(Lowest_Address和Highest_Address) */
                /* Set the lowest address as base pointer. */
                if (Phys_Addr < Lowest_Address)
                    Lowest_Address = Phys_Addr;
                /* Same for the top address. */
                temp = Phys_Addr + Nb_Bytes -1;
                if (temp > Highest_Address)
                {
                    Highest_Address = temp;
                    if (Verbose_Flag) fprintf(stderr,"Highest_Address: %08X\n",Highest_Address);
                }
                break;
            case 1:
                if (Verbose_Flag) fprintf(stderr,"End of File record\n");
                break;
            case 2:
                /* First_Word contains the offset. It's supposed to be 0000 so
                   we ignore it. */
                /* First extended segment address record ? */
                if (Seg_Lin_Select == NO_ADDRESS_TYPE_SELECTED)
                    Seg_Lin_Select = SEGMENTED_ADDRESS;
                /* Then ignore subsequent extended linear address records */
                if (Seg_Lin_Select == SEGMENTED_ADDRESS)
                {
                    result = sscanf (p, "%4x%2x",&Segment,&temp2);
                    if (result != 2) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
                    if (Verbose_Flag) fprintf(stderr,"Extended Segment Address record: %04X\n",Segment);
                    /* Update the current address. */
                    Phys_Addr = (Segment << 4);
                }
                else
                {
                    fprintf(stderr,"Ignored extended linear address record %d\n", Record_Nb);
                }
                break;
            case 3:
                if (Verbose_Flag) fprintf(stderr,"Start Segment Address record: ignored\n");
                break;
            case 4:
                /* First_Word contains the offset. It's supposed to be 0000 so
                   we ignore it. */
                /* First extended linear address record ? */
                if (Seg_Lin_Select == NO_ADDRESS_TYPE_SELECTED)
                    Seg_Lin_Select = LINEAR_ADDRESS;
                /* Then ignore subsequent extended segment address records */
                if (Seg_Lin_Select == LINEAR_ADDRESS)
                {
                    result = sscanf (p, "%4x%2x",&Upper_Address,&temp2);    //取出基地址(Extended Linear Address)和checksum
                    if (result != 2) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
                    if (Verbose_Flag) fprintf(stderr,"Extended Linear Address record: %04X\n",Upper_Address);
                    /* Update the current address. */
                    Phys_Addr = (Upper_Address << 16);
                    if (Verbose_Flag) fprintf(stderr,"Physical Address: %08X\n",Phys_Addr);
                }
                else
                {
                    fprintf(stderr,"Ignored extended segment address record %d\n", Record_Nb);
                }
                break;
            case 5:
                if (Verbose_Flag) fprintf(stderr,"Start Linear Address record: ignored\n");
                break;
            default:
                if (Verbose_Flag) fprintf(stderr,"Unknown record type: %d at %d\n",Type,Record_Nb);
                break;
            }
        }
    }
    while (!feof (Filin)); 
    /*feof()用来侦测是否读取到了文件尾, 参数stream 为fopen()所返回的文件指针. 如果已读到文件尾则返回非零值, 其他情况返回0.*/
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
每次读取一行，循环读取，直到文件尾。
申请指定长度的内存(malloc)，然后进行第二次遍历，这次的目的是将数据按地址顺序进行排列。
```cpp
/* 第二次遍历hex文件, 处理数据 */
    /* Read the file & process the lines. */
    do /* repeat until EOF(Filin) */
    {
        unsigned int i;
        /* Read a line from input file. */
        GetLine(Line,Filin);
        Record_Nb++;
        /* Remove carriage return/line feed at the end of line. */
        i = strlen(Line);
        //fprintf(stderr,"Record: %d; length: %d\n", Record_Nb, i);
        if (--i != 0)
        {
            if (Line[i] == '\n') Line[i] = '\0';
            /* Scan the first two bytes and nb of bytes.
               The two bytes are read in First_Word since its use depend on the
               record type: if it's an extended address record or a data record.
            */
            result = sscanf (Line, ":%2x%4x%2x%s",&Nb_Bytes,&First_Word,&Type,Data_Str);
            if (result != 4) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
            Checksum = Nb_Bytes + (First_Word >> 8) + (First_Word & 0xFF) + Type; //前4个字节累加
            p = (char *) Data_Str;
            /* If we're reading the last record, ignore it. */
            switch (Type)
            {
            /* Data record */
            case 0:
                if (Nb_Bytes == 0)
                {
                    fprintf(stderr,"0 byte length Data record ignored\n");
                    break;
                }
                Address = First_Word;
                if (Seg_Lin_Select == SEGMENTED_ADDRESS)
                    Phys_Addr = (Segment << 4) + Address;
                else
                    /* LINEAR_ADDRESS or NO_ADDRESS_TYPE_SELECTED
                       Upper_Address = 0 as specified in the Intel spec. until an extended address
                       record is read. */
                    if (Address_Alignment_Word)
                        Phys_Addr = ((Upper_Address << 16) + (Address << 1)) + Offset;
                    else
                        Phys_Addr = ((Upper_Address << 16) + Address);
                /* Check that the physical address stays in the buffer's range. */
                if ((Phys_Addr >= Lowest_Address) && (Phys_Addr <= Highest_Address))
                {
                    /* The memory block begins at Lowest_Address */
                    Phys_Addr -= Lowest_Address; /* 计算该条数据记录相对于hex文件起始地址的偏移量 */
                    p = ReadDataBytes(p); /* 根据偏移量将该条记录中的数据写入指定的数组中 */
                    /* Read the Checksum value. */
                    result = sscanf (p, "%2x",&temp2);
                    if (result != 1) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
                    /* Verify Checksum value. */
                    /* 校验和 = 0x100 - 除checksum之外所有字节的累加和 */
                    Checksum = (Checksum + temp2) & 0xFF;
                    VerifyChecksumValue();
                }
                else
                {
                    if (Seg_Lin_Select == SEGMENTED_ADDRESS)
                        fprintf(stderr,"Data record skipped at %4X:%4X\n",Segment,Address);
                    else
                        fprintf(stderr,"Data record skipped at %8X\n",Phys_Addr);
                }
                break;
            /* End of file record */
            case 1:
                /* Simply ignore checksum errors in this line. */
                break;
            /* Extended segment address record */
            case 2:
                /* First_Word contains the offset. It's supposed to be 0000 so
                   we ignore it. */
                /* First extended segment address record ? */
                if (Seg_Lin_Select == NO_ADDRESS_TYPE_SELECTED)
                    Seg_Lin_Select = SEGMENTED_ADDRESS;
                /* Then ignore subsequent extended linear address records */
                if (Seg_Lin_Select == SEGMENTED_ADDRESS)
                {
                    result = sscanf (p, "%4x%2x",&Segment,&temp2);
                    if (result != 2) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
                    /* Update the current address. */
                    Phys_Addr = (Segment << 4);
                    /* Verify Checksum value. */
                    Checksum = (Checksum + (Segment >> 8) + (Segment & 0xFF) + temp2) & 0xFF;
                    VerifyChecksumValue();
                }
                break;
            /* Start segment address record */
            case 3:
                /* Nothing to be done since it's for specifying the starting address for
                   execution of the binary code */
                break;
            /* Extended linear address record */
            case 4:
                /* First_Word contains the offset. It's supposed to be 0000 so
                   we ignore it. */
                if (Address_Alignment_Word) /*默认为false*/
                {
                    sscanf (p, "%4x",&Offset);
                    Offset = Offset << 16;
                    Offset -= Lowest_Address;
                }
                /* First extended linear address record ? */
                if (Seg_Lin_Select == NO_ADDRESS_TYPE_SELECTED)
                    Seg_Lin_Select = LINEAR_ADDRESS;
                /* Then ignore subsequent extended segment address records */
                if (Seg_Lin_Select == LINEAR_ADDRESS)
                {
                    result = sscanf (p, "%4x%2x",&Upper_Address,&temp2);
                    if (result != 2) fprintf(stderr,"Error in line %d of hex file\n", Record_Nb);
                    /* Update the current address. */
                    Phys_Addr = (Upper_Address << 16);
                    /* Verify Checksum value. */
                    Checksum = (Checksum + (Upper_Address >> 8) + (Upper_Address & 0xFF) + temp2)
                               & 0xFF;
                    VerifyChecksumValue();
                }
                break;
            /* Start linear address record */
            case 5:
                /* Nothing to be done since it's for specifying the starting address for
                   execution of the binary code */
                break;
            default:
                fprintf(stderr,"Unknown record type\n");
                break;
            }
        }
    }
    while (!feof (Filin));
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
最后将数组中的内容输出到文件，即可得到bin文件。
按照上面的思路我用[Java](http://lib.csdn.net/base/javase)写了一个[转换类](http://download.csdn.net/detail/zf_c_cqupt/9642578)（用在上面提到的ECU下载工具中），感兴趣的同学可以看一看。 
其中碰到的坑有必要提一下，由于Hex文件中的数据是采用ASC II码的，而Bin文件中的数据是直接使用的二进制（不存在编码），因此在转换过程中涉及到编码转换。另外还有一点需要提一下，由于Java没有无符号型，如果将读到的数据赋给byte型变量，在调试的过程中使用print输出时会看到乱码（数据溢出byte类型的范围）。
