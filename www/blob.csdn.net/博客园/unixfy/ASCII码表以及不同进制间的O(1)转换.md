
# ASCII码表以及不同进制间的O(1)转换 - unixfy - 博客园
# [程序最美（寻路）](https://www.cnblogs.com/unixfy/)
你还在坚持练习你的技术吗？运动员天天训练，音乐家也会演练更难的曲章。你呢？
## [ASCII码表以及不同进制间的O(1)转换](https://www.cnblogs.com/unixfy/p/3333920.html)
ASCII码表以及不同进制间的O(1)转换
**一、ASCII****码表**
ASCII全称为American Standard Code for Information Interchange，美国信息交换标准代码。其值范围为0-127，总共128个字符。
下面我们对着128个字符进行输出，分为五列：二进制、八进制、十进制、十六进制、字符。具体程序如下：
//生成ASCII码表\#include <iostream>\#include<fstream>\#include<string>usingnamespacestd;stringIntTo_(intnum,intradix,string&ret)
{intindex = ret.size() -1;while(num !=0)
    {inttmp = num %radix;if(tmp !=0)
        {if(tmp <10)
            {
                ret[index]='0'+tmp;
            }else{
                ret[index]='A'+ tmp -10;
            }
        }
        num/=radix;--index;
    }returnret;
}stringIntToBinary(intnum)
{stringret(8,'0');returnIntTo_(num,2, ret);
}stringIntToOct(intnum)
{stringret(3,'0');returnIntTo_(num,8, ret);
}stringIntToDec(intnum)
{stringret(3,'0');returnIntTo_(num,10, ret);
}stringIntToHex(intnum)
{stringret(2,'0');returnIntTo_(num,16, ret);
}voidCreateASCII(intbegin,intend)
{if(begin <0|| end >127)//越界{return;
    }
    ofstream fout("result.txt");if(!fout)
    {
        cerr<<"File error!"<<endl;
        exit(1);
    }
    cout<<"二进制\t\t八进制\t十进制\t十六进制\t字符\n";
    fout<<"二进制\t\t八进制\t十进制\t十六进制\t字符\n";for(auto i = begin; i <= end; ++i)
    {
        cout<< IntToBinary(i) <<'\t'<< IntToOct(i) <<'\t'<< IntToDec(i) <<'\t'<< IntToHex(i) <<'\t'<<'\t'<< (char)i <<endl;
        fout<< IntToBinary(i) <<'\t'<< IntToOct(i) <<'\t'<< IntToDec(i) <<'\t'<< IntToHex(i) <<'\t'<<'\t'<< (char)i <<endl;
    }
}intmain()
{
    CreateASCII(0,127);//CreateASCII('A', 'Z');//CreateASCII('a', 'z');//CreateASCII('0', '9');return0;
}
二进制        八进制    十进制    十六进制    字符
00000000    000    000    00         
00000001    001    001    01        
00000010    002    002    02        
00000011    003    003    03        
00000100    004    004    04        
00000101    005    005    05        
00000110    006    006    06        
00000111    007    007    07        
00001000    010    008    08        
00001001    011    009    09            
00001010    012    010    0A        
00001011    013    011    0B        
00001100    014    012    0C        
00001101    015    013    0D        
00001110    016    014    0E        
00001111    017    015    0F        
00010000    020    016    10        
00010001    021    017    11        
00010010    022    018    12        
00010011    023    019    13        
00010100    024    020    14        
00010101    025    021    15        
00010110    026    022    16        
00010111    027    023    17        
00011000    030    024    18        
00011001    031    025    19        
00011010    032    026    1A        
00011011    033    027    1B        
00011100    034    028    1C        
00011101    035    029    1D        
00011110    036    030    1E        
00011111    037    031    1F        
00100000    040    032    20         
00100001    041    033    21        !
00100010    042    034    22        "
00100011    043    035    23        \#
00100100    044    036    24        $
00100101    045    037    25        %
00100110    046    038    26        &
00100111    047    039    27        '
00101000    050    040    28        (
00101001    051    041    29        )
00101010    052    042    2A        *
00101011    053    043    2B        +
00101100    054    044    2C        ,
00101101    055    045    2D        -
00101110    056    046    2E        .
00101111    057    047    2F        /
00110000    060    048    30        0
00110001    061    049    31        1
00110010    062    050    32        2
00110011    063    051    33        3
00110100    064    052    34        4
00110101    065    053    35        5
00110110    066    054    36        6
00110111    067    055    37        7
00111000    070    056    38        8
00111001    071    057    39        9
00111010    072    058    3A        :
00111011    073    059    3B        ;
00111100    074    060    3C<00111101075    061    3D=
00111110076    062    3E>00111111    077    063    3F        ?
01000000    100    064    40        @
01000001    101    065    41        A
01000010    102    066    42        B
01000011    103    067    43        C
01000100    104    068    44        D
01000101    105    069    45        E
01000110    106    070    46        F
01000111    107    071    47        G
01001000    110    072    48        H
01001001    111    073    49        I
01001010    112    074    4A        J
01001011    113    075    4B        K
01001100    114    076    4C        L
01001101    115    077    4D        M
01001110    116    078    4E        N
01001111    117    079    4F        O
01010000    120    080    50        P
01010001    121    081    51        Q
01010010    122    082    52        R
01010011    123    083    53        S
01010100    124    084    54        T
01010101    125    085    55        U
01010110    126    086    56        V
01010111    127    087    57        W
01011000    130    088    58        X
01011001    131    089    59        Y
01011010    132    090    5A        Z
01011011    133    091    5B        [
01011100    134    092    5C        \
01011101    135    093    5D        ]
01011110    136    094    5E        ^
01011111    137    095    5F        _
01100000    140    096    60        `
01100001    141    097    61        a
01100010    142    098    62        b
01100011    143    099    63        c
01100100    144    100    64        d
01100101    145    101    65        e
01100110    146    102    66        f
01100111    147    103    67        g
01101000    150    104    68        h
01101001    151    105    69        i
01101010    152    106    6A        j
01101011    153    107    6B        k
01101100    154    108    6C        l
01101101    155    109    6D        m
01101110    156    110    6E        n
01101111    157    111    6F        o
01110000    160    112    70        p
01110001    161    113    71        q
01110010    162    114    72        r
01110011    163    115    73        s
01110100    164    116    74        t
01110101    165    117    75        u
01110110    166    118    76        v
01110111    167    119    77        w
01111000    170    120    78        x
01111001    171    121    79        y
01111010    172    122    7A        z
01111011    173    123    7B        {
01111100    174    124    7C        |
01111101    175    125    7D        }
01111110    176    126    7E        ~
01111111    177    127    7F        
**二、不同进制间的O(1)****转换**
实现二进制、八进制、十进制、十六进制四种进制之间的O(1)转换
//十六进制、八进制、十进制到二进制的直译\#include <iostream>\#include<fstream>\#include<vector>\#include<map>\#include<string>usingnamespacestd;stringIntTo_(intnum,intradix,string&ret)
{intindex = ret.size() -1;while(num !=0)
    {inttmp = num %radix;if(tmp !=0)
        {if(tmp <10)
            {
                ret[index]='0'+tmp;
            }else{
                ret[index]='A'+ tmp -10;
            }
        }
        num/=radix;--index;
    }returnret;
}stringIntToBinary(intnum)
{stringret(8,'0');returnIntTo_(num,2, ret);
}stringIntToOct(intnum)
{stringret(3,'0');returnIntTo_(num,8, ret);
}stringIntToDec(intnum)
{stringret(3,'0');returnIntTo_(num,10, ret);
}stringIntToHex(intnum)
{stringret(2,'0');returnIntTo_(num,16, ret);
}structFourSystem
{stringbinary;stringoct;stringdec;stringhex;intnum;intindex;
};voidCreateSystem(vector<FourSystem>& System, map<string,int>& binaryMap, map<string,int>&octMap,
                                          map<string,int>& decMap,    map<string,int>&hexMap,
                                          map<int,int>&intMap)
{
    System.clear();
    binaryMap.clear();
    octMap.clear();
    decMap.clear();
    hexMap.clear();
    intMap.clear();
    FourSystem tmp;//System.push_back(tmp);//binaryMap[""];//octMap[""];//decMap[""];//hexMap[""];//intMap[-1];for(auto i =0; i !=128; ++i)
    {
        tmp.binary=IntToBinary(i);
        tmp.oct=IntToOct(i);
        tmp.dec=IntToDec(i);
        tmp.hex=IntToHex(i);
        tmp.num=i;
        tmp.index=i;
        System.push_back(tmp);
        binaryMap[tmp.binary]=i;
        octMap[tmp.oct]=i;
        decMap[tmp.dec]=i;
        hexMap[tmp.hex]=i;
        intMap[tmp.num]=i;
    }
}intmain()
{
    vector<FourSystem>System;
    map<string,int>binaryMap, octMap, decMap, hexMap;
    map<int,int>intMap;
    CreateSystem(System, binaryMap, octMap, decMap, hexMap, intMap);intindex = binaryMap["00010110"];//22cout <<"00010110"<<endl;
    cout<< System[index].binary <<'\t'<< System[index].oct <<'\t'<< System[index].dec <<'\t'<< System[index].hex    <<'\t'<< System[index].num <<endl;
    index= octMap["026"];//22cout <<"026"<<endl;
    cout<< System[index].binary <<'\t'<< System[index].oct <<'\t'<< System[index].dec <<'\t'<< System[index].hex    <<'\t'<< System[index].num <<endl;
    index= decMap["022"];//22cout <<"022"<<endl;
    cout<< System[index].binary <<'\t'<< System[index].oct <<'\t'<< System[index].dec <<'\t'<< System[index].hex    <<'\t'<< System[index].num <<endl;
    index= hexMap["16"];//22cout <<"16"<<endl;
    cout<< System[index].binary <<'\t'<< System[index].oct <<'\t'<< System[index].dec <<'\t'<< System[index].hex    <<'\t'<< System[index].num <<endl;
    index= intMap[22];//22cout <<22<<endl;
    cout<< System[index].binary <<'\t'<< System[index].oct <<'\t'<< System[index].dec <<'\t'<< System[index].hex    <<'\t'<< System[index].num <<endl;
    index= hexMap["7F"];//7Fcout <<"7F"<<endl;
    cout<< System[index].binary <<'\t'<< System[index].oct <<'\t'<< System[index].dec <<'\t'<< System[index].hex    <<'\t'<< System[index].num <<endl;return0;
}
![](https://images0.cnblogs.com/blog/463570/201309/22214117-7c454662284b489a83ee2b803cc65572.jpg)
我们定义了结构体：FourSystem，用于存储一个数字对应的二进制、八进制、十进制、十六进制形式，CreateSystem函数用于初始化System数组，并且对binaryMap、octMap、decMap、hexMap、intMap等从形式到索引的映射进行初始化。预处理之后，针对特定的进制形式，只需根据特定的映射找到对应的索引，即可得到对应的其他进制形式。
这里需要我们针对特定的进制形式，指定特定的映射，当然也可以进行智能识别，比如如果长度为8，则对应于binaryMap；如果长度是2，则对应于hexMap；如果类型为int，则对应于intMap。对于octMap和decMap如果含有大于等于’8’的字符则对应于decMap；如果不含有，则只能我们人工指定其进制形式。当然，我们也可以根据上下文、应用场景进行区分。




posted on2013-09-22 21:42[unixfy](https://www.cnblogs.com/unixfy/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=3333920)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)







### 导航
[博客园](https://www.cnblogs.com/)
[首页](https://www.cnblogs.com/unixfy/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/unixfy)
[订阅](https://www.cnblogs.com/unixfy/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 - 85文章 - 0评论 - 8引用 - 0
公告

Powered by:
博客园
Copyright © unixfy
