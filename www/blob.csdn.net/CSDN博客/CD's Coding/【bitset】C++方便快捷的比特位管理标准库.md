# 【bitset】C++方便快捷的比特位管理标准库 - CD's Coding - CSDN博客





2018年08月09日 17:04:19[糖果天王](https://me.csdn.net/okcd00)阅读数：108








## 0x00 前言

C++的STL向来是一个非常厉害且好用的东西， 

提供了大量近乎优化到极致的解决方案，在日常编程或是竞赛中， 

STL使用的娴熟可以省去大量的时间和脑力得出非常优质的解法， 

这回是我突然看见了bitset这个我不太常使用的类库，用于状态压缩非常巧妙， 

于是决定相见即是有缘，就好好的学习一下吧——
通常的解释为：

> 
**bitset**

  C++语言的一个类库，用来方便地管理一系列的bit位而不用程序员自己来写代码。 

  bitset除了可以访问指定下标的bit位以外，还可以把它们作为一个整数来进行某些统计。
## 0x01 测试代码

**SOURCE**

```cpp
/*==========================================================================
#   Copyright (C) 2018 All rights reserved.
#
#   filename : test_bitset.cpp
#   author   : chendian / okcd00@qq.com
#   date     : 2018-08-08
# ==========================================================================*/

#include <bitset>
#include <string> 
#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;

int initial(bool flag=false)
{
    string s = "00010111";
    string t = "1111111100010111";
    string r = "11101000";
    bitset<8> z;  // init with all-zero
    bitset<8> a(s);  // init with string object
    bitset<8> b(23);  // init with integer
    bitset<8> c(0x17); // init with hex
    bitset<8> d(t, 8, 8);  // init with substring
    bitset<8> e(string(r.rbegin(), r.rend())); // init with reversed string
    z[4] = z[2] = z[1] = z[0] = 1;  // assignment

    if(flag) cout<<a<<endl<<b<<endl<<c<<endl<<d<<endl<<e<<endl<<z<<endl;
    // All above are the same.
    return 0;
}

int check(bitset<8> bs)
{
    cout<<"this is: ["<<bs.to_ulong()<<"] " <<bs.to_string()<<endl;
    // bs.to_ullong() can only be used after c++11
    cout<<"=================================="<<endl;
    cout<<"bitset size: "<<bs.size()<<"\t| "   <<bs.to_string()<<endl;
    cout<<"has value 1: "<<bs.any()<<"\t| "    <<bs.to_string()<<endl;
    cout<<"all value 0: "<<bs.none()<<"\t| "   <<bs.to_string()<<endl;
    cout<<"has 1 count: "<<bs.count()<<"\t| "  <<bs.to_string()<<endl;
    cout<<"pos[3] is 1: "<<bs.test(3)<<"\t| "  <<bs.to_string()<<endl;
    cout<<"pos[3] to 1: "<<bs.set(3)<<"\t| "   <<bs.to_string()<<endl;
    cout<<"pos[3] to 0: "<<bs.reset(3)<<"\t| " <<bs.to_string()<<endl;
    cout<<"neg pos[3]:  "<<bs.flip(3)<<"\t| "  <<bs.to_string()<<endl;
    cout<<"=================================="<<endl;
    cout<<"this is: ["<<bs.to_ulong()<<"] " <<bs.to_string()<<endl;
    return 0;   
}

int operators()
{
    bitset<4> foo (string("1001"));
    bitset<4> bar (string("0011"));

    cout << (foo^=bar) << endl;       // 1010 (XOR,assign)
    cout << (foo&=bar) << endl;       // 0010 (AND,assign)
    cout << (foo|=bar) << endl;       // 0011 (OR,assign)

    cout << (foo<<=2) << endl;        // 1100 (SHL,assign)
    cout << (foo>>=1) << endl;        // 0110 (SHR,assign)

    cout << (~bar) << endl;           // 1100 (NOT)
    cout << (bar<<1) << endl;         // 0110 (SHL)
    cout << (bar>>1) << endl;         // 0001 (SHR)

    cout << (foo==bar) << endl;       // false (0110==0011)
    cout << (foo!=bar) << endl;       // true  (0110!=0011)

    cout << (foo&bar) << endl;        // 0010
    cout << (foo|bar) << endl;        // 0111
    cout << (foo^bar) << endl;        // 0101   
    return 0;
}


int usage()
{
    // init with all zeros.
    string cname[6] = {"red", "yellow", "green", "blue", "white", "black"};
    enum Color{red, yellow, green, blue, white, black, numColors};
    bitset<numColors> usedColors;

    // something happens
    usedColors.set(red);
    usedColors.set(blue);

    cout << "bitfield of used colors: " << usedColors << endl;
    cout << "bitfield of unused colors: " << ~usedColors << endl;

    // process if any color is used 
    if (usedColors.any())
    {
        for (int c = 0; c < numColors; ++c)
        {
            if (usedColors[Color(c)])
                cout<<cname[Color(c)]<<" is used"<<endl;
            else
                cout<<cname[Color(c)]<<" is not used"<<endl;
        }
    }
}

int main()
{
    bitset<8> bs(23);
    initial();
    check(bs);  
    //operators();
    usage();
    return 0;
}
```

**OUTPUT**

```
this is: [23] 00010111
==================================
bitset size: 8  | 00010111
has value 1: 1  | 00010111
all value 0: 0  | 00010111
has 1 count: 4  | 00010111
pos[3] is 1: 0  | 00010111
pos[3] to 1: 00011111   | 00010111
pos[3] to 0: 00010111   | 00011111
neg pos[3]:  00011111   | 00010111
==================================
this is: [31] 00011111
bitfield of used colors: 001001
bitfield of unused colors: 110110
red is used
yellow is not used
green is not used
blue is used
white is not used
black is not used
```

## 0x02 Reference

[1] [《C++基础——简单而强大的bitset》](https://blog.csdn.net/lanchunhui/article/details/49644373)

[2] [baike.baidu.com](https://baike.baidu.com/item/BitSet/5816051)






