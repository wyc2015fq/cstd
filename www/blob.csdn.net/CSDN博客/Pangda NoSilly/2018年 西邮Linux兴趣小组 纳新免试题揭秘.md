# 2018年 西邮Linux兴趣小组 纳新免试题揭秘 - Pangda NoSilly - CSDN博客

2018年05月07日 23:14:59[hepangda](https://me.csdn.net/hepangda)阅读数：765


# 前言

小组2018年的免试题的五位出题人是：小组16级成员刘付杰、李猛、时宇辰、王良、娄泽豪。（此处应有掌声若干秒）

本人虽然参与了出题，但是我对其他关卡知之甚少，于是好奇的我在免试题上线了之后，与大家一起开始了破关之旅。以下以我作为第一视角所写而成的“免试题攻略”，若有错漏，还请多多包涵，与我在评论区进行交流。

# 第一关

目前微信推送中第一关的入口已经下线，想要挑战的同学可以点击[这里的入口](http://m.fujie.bid)进行挑战。

打开本关的入口链接，首先我们看到了一大段英文，根据我们在线翻译级别的英语技术，大约意思是我们要不要继续下去：

![第一关的第一个页面](https://img-blog.csdn.net/20180503232202290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

第一直觉我们肯定选择【Yes！】，不过这一页还有别的信息吗？经过一番尝试，无论是叛逆的选择【No..】或者做其他操作，都没有其他有用的信息。于是我们选择【Yes！】进入第二个页面：

![第一关的第二个页面](https://img-blog.csdn.net/20180503232909497?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

首先最引人瞩目的显然就是中间的看不清楚的字，还有下面“FUJIE”五个字母构成的目前看来意义不明的动画。中间看不清楚的字可以通过选择的方式选中复制出来看：

```
晓伴君王犹未起，
阻避钧衡过十年。
纳降归拟尽公卿，
欣然向我话佳境。
诚知杀身愿如此，
恭谈祖德朵颐开。
```

我的文学水平不怎么高，但是显然这首诗的文学水平实在是一言难尽。不过既然刻意隐藏起来，其中必有玄机，仔细读读之后，发现似乎是一个藏头诗啊——“晓阻纳欣诚恭”，不正是“小组纳新成功”的意思吗？

在刚刚复制字的时候，左右拖动一下，还可以发现网页的动画下边隐藏了这样一些文字：

```
然后，如你所见
Fujie被一股神秘力量肢解了！！！
这实在是太恐怖了！小组的小伙伴们都吓得不知所措了！
希望你能帮我们复原一下Fujie！
```

这里说了“肢解”，可能是一个很重要的信息。不过我们现在手头还没有与之相关的内容，怎么办呢？还是前几年的老方法，按F12在审查元素里面找找吧！

![第一章审查元素](https://img-blog.csdn.net/20180503233943249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里有一个隐藏起来的链接，我们点击进去看看：

![第一关单词表](https://img-blog.csdn.net/20180503234229492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里有以26个字母开头的单词为名的文件各一个。随意的下载几个来看，没什么联系，看来是我们已有的信息派上用场的时候了。联想到“FUJIE”被分开的动画以及提示的“肢解”、“藏头诗”的信息，想到了：有可能是f,u,j,i,e五个字母打头的文件被拆分了。于是下载这五个文件，使用以下命令将他们连接起来：

```matlab
cat understand >> freedom
cat jealousy >> freedom
cat independence >> freedom
cat enjoy >> freedom
```

组合起来之后，根据文件图标和文件头来看，是一个zip的压缩包。打开之后，发现里面有两个文件：

![第一关压缩包](https://img-blog.csdn.net/20180503235002276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

尝试解压这个压缩包，果然是需要密码的。不过压缩文件里既然说“请给我们一句祝福的话（UTF-8）”，那么密码就呼之欲出了，藏头诗的内容“小组纳新成功”即是密码。解压之后，仔细听压缩包中的另一个mp3，在音乐的末尾有机读出下一关的IP地址。那么这一关就算完成了。

# 第二关

从上一关中我们得到了[本关的入口](http://47.100.19.186/)，打开入口界面，我们看到了以下的界面：

![第二关入口](https://img-blog.csdn.net/20180504162831845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用Ctrl+A选择页面内的所有元素，没有发现什么有价值的东西。还是老方法，审查元素：

![第二关审查元素](https://img-blog.csdn.net/20180507165712299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在注释里面说要让我们认真听音乐，然后下面还有一系列文件，在网页上分析太不方便了，干脆统统下载下来好了。下载之后，按照网页上所说的，先听听音乐吧！听了一阵子之后发现有些时候左耳会声音变小，有的时候右耳声音会变小，摘掉另一个耳机，发现有滴滴声。那么我们用频谱分析软件看一看发生了什么吧！这里用的软件是`audacity`。打开之后，发现听的时候三段的声音变小的频谱是这样的：

![第二关频谱1](https://img-blog.csdn.net/20180507170328675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![第二关频谱2](https://img-blog.csdn.net/20180507170336503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![第二关频谱3](https://img-blog.csdn.net/20180507170344575?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

单独播放声音变小的声道的声音，似乎像发电报的声音，搜索摩尔斯电码对照着看，得到了三个单词：`OPEN FREE SHARE`，是小组的文化精神，应该是一个重要的信息。分析完了音乐，再看看其他的东西吧，打开`XiyouLinux.zip`这个压缩包，果然是需要密码，不过刚才我们刚刚得到了三个单词，进行排列组合尝试一下，发现密码就是`openfreeshare`。压缩包中有两个文件：

![第二关压缩包](https://img-blog.csdn.net/2018050720301676?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解压缩之后，运行`a.out`提示`need parameter`，尝试再次输入`openfreeshare`，这次并不能通过。看来还需要别的信息，打开`.pri.key`这个文件：

```
-----BEGIN PRIVATE KEY-----
MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAL4YTFlbLUlGHoW4
bS92RUL0rOn8BnFzNp95FTpeKqUP4gDDTbwflFSZ+1jdhrTsmXHTzTl0rQZ+wNbk
iSX3k8obXFmlO21cj1uvEjmWWtxlR6A375A7iQGH10x3h2Bh9o7dnPXoUvd2wH9x
2p7tD6dPm5GR+dpXiOhByPiuHpahAgMBAAECgYEAnLQZFDw2S8YS2TbcQxjjJbvf
Yw+QHCvW5oWBO1lvHBzIuMpHJYJ+23MIIQyUxEaag5wO/IMzMzyAKWXBrVu3JqGe
Xtu4EGEPxnLG5JNbOeUfo8c25a3Kjm4pAYhEE0cfq0I0QnWKyGfwxuQghT9Fb4xZ
qNd2DIIgOpjhilLj6zUCQQD2HqWesti5VgaqBUk2XZpH0jyFW43ACk5YSuSPhkDp
hnAYFdesNpyJK5M11e/Y3imD1UB8t9s0dpAIp3PwhnpzAkEAxbnjiKUYwwwcF8ts
EZ1ahsLAXYNoBX6YOr+Rxlf2tWKqaF7JAhwb6VNdpH3oi1Jn/Eg99mHHOKgwMjZF
uugBmwJBAIAUOMoadlAUpYkrEQt6sIP5s0cO+vhaJKUr7D+IdRVRwdm23DKhhNqZ
U5VrjNKF4oLZoiKFJ0zo+lGWmu4rfWMCQD7BALYdr/43mbLznRj6GAEtTmBflGQq
CaabpmiNAoAPEIaPjrxcr38eNlo+m8+cF+S6CPBpmBEjUCifkQIBIIECQARkXiTZ
oU7fXHlOW49e1V3ExMah874+XxaZ5BmWoWZASgQ+M+cybPYsQpBKuxwFJMTZdEFo
NY+19KxfsKoFQ2k=
-----END PRIVATE KEY-----

a part of address:(please use public key to deal with)

VkqyowF5O60JThF3tHxgnXlGHDCHHWQG7y8KhlnN9xLOC3eThFPDA/NH9lQIwyiRQQV7XINxOcugiF0xdhKQpzZ9CHdyByUvyvOGyDeA77X3voIF6YivPIOhxEU9vXLLlpFp/2MFbU2D7XX9p4DNCTbn92P8zwx9vcE1FDQkYGE=
```

看到了一个RSA加密算法中所需要的私钥，还有一串让我们使用公钥处理的密文，看来本关与RSA算法有关联，不过我们已经没有更多信息了。思考之后，感觉首页的小组Logo很大（？），而且并没有从外部链接，可能有玄机，使用`vim`打开，跳转到最后，果然别有洞天！

![第二关图片](https://img-blog.csdn.net/20180507204014526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先提示了`#391`，去第391行看看，发现了一个PNG头，删除掉前面内容，得到了一张新的图片： 
![第二关新的图片](https://img-blog.csdn.net/2018050720424832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

图片里直截了当的就是一个某网盘的地址，下载对应文件之后是一个TXT文件，内容是：

```
后半部分：（使用公钥加密）
RWpjbKKOKAyi62rw6F5xVfCWUtDSnzaW1zJVmz2p9we3zWszsbuP1OLgz3bzqGYMHYPFenxx+0eX+qgJk3f5Hb6LmBG4huQp2AH7Mmi8pCTjdS3QX60x4zIObWOzh7ya+eelvQu6UcX0VGI7gGpUVTPoNbxEglb4wCE6XV9Mzhw=
```

看来我们又得到了一部分的内容，先暂且记下，接着分析之前的图片，之前的图片下面又出现了`The parameter`，后面是一堆`&#`开头后面跟着数字的东西，应该是HTML转义字符，对应的字符是`I love Linux && I want to challenge`，将这一串信息输入到刚才的`a.out`中，得到信息：

```
-----BEGIN PUBLIC KEY-----
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC+GExZWy1JRh6FuG0vdkVC9Kzp
/AZxczafeRU6XiqlD+IAw028H5RUmftY3Ya07Jlx0805dK0GfsDW5Ikl95PKG1xZ
pTttXI9brxI5llrcZUegN++QO4kBh9dMd4dgYfaO3Zz16FL3dsB/cdqe7Q+nT5uR
kfnaV4joQcj4rh6WoQIDAQAB
-----END PUBLIC KEY-----

You need to find the ciphertext to get some information !!!
```

看来是获得了公钥，不过实际上之前已经给了我们私钥，即使我们不获得这个信息，也可以通过私钥计算出公钥，这一步理论上可以跳过。

这样我们知道了RSA的公钥和私钥，以及相应的密文，通过在线RSA解密工具，我们得到了下一关的入口：`http://xyl2018.xuejietech.cn/`

# 第三关

进入[第三关](http://xyl2018.xuejietech.cn/)，首先看到的就是一个倒计时：

![第三关首页](https://img-blog.csdn.net/20180507205404461?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

仔细阅读之后，看来是一个迷宫游戏。下面应该是这个迷宫的构造，迷宫很大，岔路很多，而且每次的迷宫在刷新后都不同，应该是随机的。既然是随机的，人品足够应该也能走出来吧！多次尝试之后发现——运气并不能好到这种程度。那么就写一个程序帮助我计算一下路线吧！上网搜索一下迷宫问题的相关算法，有很多种解法，例如DFS、BFS等等，以下给出一个方案：

```
#include <stdio.h>

const int W = 101, H = 101, M = 4;
const char SPACE = '.',WALL = '#', ROUTE = '$';

char board[H][W];

typedef struct pos_t {
    int x, y;
} pos_t;
pos_t movements[] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };

void print_board()
{
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            putchar(board[i][j]);
        }
        putchar('\n');
    }
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            scanf(" %c", &board[i][j]);
        }
    }
    int steps[W * H];
    steps[0] = 0;
    for (int x = 0, y = 1, top = 0;;) {
        pos_t *m = &movements[steps[top]];
        x += m->x; y += m->y;

        if (x == W - 2 && y == H - 1) {
            board[y][x] = ROUTE;
            break;
        } else if (board[y][x] == SPACE && x >= 0 && y >= 0 && x < W && y < H) {
            board[y][x] = ROUTE;
            steps[++top] = 0;
        } else {
            x -= m->x; y -= m->y;
            steps[top]++;
            while (steps[top] >= M) {
                board[y][x] = SPACE;
                if (--top < 0) break;
                m = &movements[steps[top]];
                x -= m->x; y -= m->y;
                steps[top]++;
            }
        }
    }

    print_board();
    return 0;
}
```

按照程序所提示的通关迷宫之后，直接跳转到了[第四关](http://140.143.239.164/)。

# 第四关

从上一关跳转到[本关](http://140.143.239.164/)之后，看到了以下界面：

![第四关界面](https://img-blog.csdn.net/20180507214301256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

一如既往的简洁啊，还是打开审查元素看看都有什么吧！

![第四关审查元素](https://img-blog.csdn.net/20180507214413950?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

还是一如既往的注释剧透系列，这里有一个命令，还有一个压缩包。先运行一下这个命令吧～

![第四关命令结果](https://img-blog.csdn.net/20180507214601634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

得到了字符串`kcabpool`，根据剧本应该是上面那个压缩包的密码了吧！下载压缩包，使用`kcabpool`作为密码解压，压缩包中只有一个音乐：

![第四关压缩包](https://img-blog.csdn.net/20180507214754849?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个音乐的名字说“不仅仅是一个音乐”，看来可能还有别的信息？研究之后~~发现~~猜测是一个`zip`格式的压缩文件。直接使用`unzip`命令解压缩，得到一个文件`hint.txt`，内容如下：

![第四关hint](https://img-blog.csdn.net/20180507215620587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`ﾟωﾟﾉ= /｀ｍ´）ﾉ ~┻━┻   //*´∇｀*/ ['_']; o=(ﾟｰﾟ)  =_=3; c=(ﾟΘﾟ) =(ﾟｰﾟ)-(ﾟｰﾟ); (ﾟДﾟ) =(ﾟΘﾟ)= (o^_^o)/ (o^_^o);(ﾟДﾟ)={ﾟΘﾟ: '_' ,ﾟωﾟﾉ : ((ﾟωﾟﾉ==3) +'_') [ﾟΘﾟ] ,ﾟｰﾟﾉ :(ﾟωﾟﾉ+ '_')[o^_^o -(ﾟΘﾟ)] ,ﾟДﾟﾉ:((ﾟｰﾟ==3) +'_')[ﾟｰﾟ] }; (ﾟДﾟ) [ﾟΘﾟ] =((ﾟωﾟﾉ==3) +'_') [c^_^o];(ﾟДﾟ) ['c'] = ((ﾟДﾟ)+'_') [ (ﾟｰﾟ)+(ﾟｰﾟ)-(ﾟΘﾟ) ];(ﾟДﾟ) ['o'] = ((ﾟДﾟ)+'_') [ﾟΘﾟ];(ﾟoﾟ)=(ﾟДﾟ) ['c']+(ﾟДﾟ) ['o']+(ﾟωﾟﾉ +'_')[ﾟΘﾟ]+ ((ﾟωﾟﾉ==3) +'_') [ﾟｰﾟ] + ((ﾟДﾟ) +'_') [(ﾟｰﾟ)+(ﾟｰﾟ)]+ ((ﾟｰﾟ==3) +'_') [ﾟΘﾟ]+((ﾟｰﾟ==3) +'_') [(ﾟｰﾟ) - (ﾟΘﾟ)]+(ﾟДﾟ) ['c']+((ﾟДﾟ)+'_') [(ﾟｰﾟ)+(ﾟｰﾟ)]+ (ﾟДﾟ) ['o']+((ﾟｰﾟ==3) +'_') [ﾟΘﾟ];(ﾟДﾟ) ['_'] =(o^_^o) [ﾟoﾟ] [ﾟoﾟ];(ﾟεﾟ)=((ﾟｰﾟ==3) +'_') [ﾟΘﾟ]+ (ﾟДﾟ) .ﾟДﾟﾉ+((ﾟДﾟ)+'_') [(ﾟｰﾟ) + (ﾟｰﾟ)]+((ﾟｰﾟ==3) +'_') [o^_^o -ﾟΘﾟ]+((ﾟｰﾟ==3) +'_') [ﾟΘﾟ]+ (ﾟωﾟﾉ +'_') [ﾟΘﾟ]; (ﾟｰﾟ)+=(ﾟΘﾟ); (ﾟДﾟ)[ﾟεﾟ]='\\'; (ﾟДﾟ).ﾟΘﾟﾉ=(ﾟДﾟ+ ﾟｰﾟ)[o^_^o -(ﾟΘﾟ)];(oﾟｰﾟo)=(ﾟωﾟﾉ +'_')[c^_^o];(ﾟДﾟ) [ﾟoﾟ]='\"';(ﾟДﾟ) ['_'] ( (ﾟДﾟ) ['_'] (ﾟεﾟ+(ﾟДﾟ)[ﾟoﾟ]+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+((ﾟｰﾟ) + (ﾟΘﾟ))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (o^_^o)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((o^_^o) +(o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) +(o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((o^_^o) +(o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (c^_^o)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (o^_^o))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((o^_^o) +(o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (c^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((o^_^o) +(o^_^o))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ (ﾟｰﾟ)+ (o^_^o)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ ((ﾟｰﾟ) + (o^_^o))+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟΘﾟ)+ ((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟｰﾟ)+ (ﾟДﾟ)[ﾟεﾟ]+(ﾟｰﾟ)+ ((o^_^o) - (ﾟΘﾟ))+ (ﾟДﾟ)[ﾟεﾟ]+((ﾟｰﾟ) + (ﾟΘﾟ))+ (ﾟΘﾟ)+ (ﾟДﾟ)[ﾟoﾟ]) (ﾟΘﾟ)) ('_');`
哇，好多的颜文字！不过我曾经看到说`Javascript`可以加密为颜文字，这一段是不是一段`Javascript`代码呢？在`Chrome`浏览器自带的`Console`中输入这些内容，得到：

![第四关颜文字](https://img-blog.csdn.net/2018050721592215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其实如果你不知道这一点，也可以上网找到[“颜文字加密”的解密网站](http://utf-8.jp/public/aaencode.html)，也能得到这一串信息。这一串信息是说答案可能与“Robots Exclusion Protocol”相关，那么这是什么呢？搜索一下并仔细阅读阅读：

![REP协议](https://img-blog.csdn.net/20180507220244994?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

仔细看完，感觉只有“根目录下的robots.txt”可能是有效信息，其他的都是一些科普知识。那么我们试着在这个网站下找找`robots.txt`吧！在地址栏中输入本关的网址，后面跟上`/robots.txt`：

![第四关robotstxt](https://img-blog.csdn.net/20180507220559395?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

COOL！我们得到了下一关的地址！

# 第五关

输入网址进入[第五关](http://182.254.130.94:8080)，首先看到的是：

![第五关首页](https://img-blog.csdn.net/20180507220817117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

不同于其他几关，本关的页面中显示的信息量比其他关卡大了很多。不过我们还是尝试尝试老套路吧，按F12打开`Chrome`浏览器的审查元素功能：

![第五关审查元素](https://img-blog.csdn.net/20180507221023347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里的注释居然说以下没有需要查看源码才能得到的信息？！不过我是这关的出题人，我可以向你保证，这里说的真的是真的。尝试在页面中随意点点，发现：点击“官方网站”一项，就真的可以访问小组的官方网站；点击“微信平台”一项，出现的二维码就真的可以关注我们的微信平台；点击“新浪微博”一项，就真的可以跳转到小组的微博；点击“我们在哪”一项，emmm，就真的可以在腾讯地图上找到我们小组的位置。那么关键的信息点在哪里呢？

你可能看到了，页面下面的灰色字体会提示你通过什么系统访问，甚至于通过手机访问的时候：

![第五关安卓访问](https://img-blog.csdn.net/20180507221641921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

不过灰色字体是以逗号结束的，选中之后发现后面还有别的信息，这取决于你使用什么系统来访问的这个页面。如果你和我一样使用`Linux`访问，那么会提示你使用`Windows`访问网页看看；如果你使用`Windows`，那么便提示你使用`Linux`访问尝试一下。对比之后，发现在任务目标一节中，如果你使用`Windows`，那么你看到的就是：

![第五关Windows任务目标](https://img-blog.csdn.net/20180507222032702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果你使用`Linux`访问，那么你看到的则是：

![第五关Linux任务目标](https://img-blog.csdn.net/20180507222111862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

两者之间的区别就是“链接”状图标的位置不同。分别下载这两组文件，`Windows`下，下载的文件是一个压缩包，按照剧本来说应该有密码，不过——这次并没有。解压之后，可执行文件，里面有一个数独游戏。不过即使你解出来也没有用，因为并不能以任何形式输入结果。反复查看之后，发现在文件属性中就得到Key2了：

![第五关Key2](https://img-blog.csdn.net/20180507222412276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在`Linux`下，下载的文件也是一个压缩包，也照例没有密码。解压之后：

![第五关Linux下压缩包](https://img-blog.csdn.net/20180507222631118?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

压缩包东西很多，其中图片说：

![第五关压缩包图片](https://img-blog.csdn.net/20180507222803137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用`vim`打开图片，里面没有任何其他信息。而文本文件的内容是一串base64编码的内容，内容为补充解释图片的意思。两者结合起来大约是要求我们去求`Key1_2018bytes`这个文件中有多少个二进制1，并将这个数字作为结果提交给`pangda`这个程序。那么我们这里可以写一个程序帮我们做这个事情：

```
#include<stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp = fopen("Key1_2018bytes", "rb");
    int res = 0;
    char t;

    fread(&t, sizeof(char), 1, fp);
    while (!feof(fp)) {
        if (t & 0b10000000) res++;
        if (t & 0b01000000) res++;
        if (t & 0b00100000) res++;
        if (t & 0b00010000) res++;
        if (t & 0b00001000) res++;
        if (t & 0b00000100) res++;
        if (t & 0b00000010) res++;
        if (t & 0b00000001) res++;
        fread(&t, sizeof(char), 1, fp);
    }

    printf("result = %d\n", res);
    fclose(fp);
    return 0;
}
```

不过，等等！这些文件名又是什么意思呢？仔细看一下：一字节有八位，2018×8=16144，Key1是2018字节。那么，既然Key1只有2018字节，最终答案必然也在1～16144之内！所以我们还可以通过暴力解答这个问题：

```bash
for ((i=1;i<=16144;i++));  
do
    echo $i > ./temp
    ./pangda < ./temp >> ./out
done
awk '!a[$0]++' out
```

最终结果是8063，提交给程序，得到了`Key1`。

![第五关Key1](https://img-blog.csdn.net/20180507223517250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

还有一个标签没有访问过，点击进去，有一个智力问答游戏。答案是随机的，尝试几次之后点击新出现的提示链接。进入这个页面：

![第五关提示页面](https://img-blog.csdn.net/2018050722380974?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

打开网页源码，发现代码很少，也没有常见的注释提醒。不过，在其中一个`<p>`标签中，有一个名为`tips`即`提示`的属性：

![第五关提示页面源代码](https://img-blog.csdn.net/2018050722393785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

提示内容是`User-Agent`，结合网页上的内容，难道是说让我将`User-Agent`改为`XiyouLinux Group`？在网上查询修改`User-Agent`的方法，重新访问：

![第五关修改UA后](https://img-blog.csdn.net/2018050722413021?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

只有一个链接，点击后下载一个压缩包。解压之后只有一个名为`Key4.elf`的可执行文件，执行一下只提示`See .rodata`。看来我们的另一个Key就藏在`.rodata`段了！使用命令`readelf -p .rodata Key4.elf`，便得到了`Key 4`！

![第五关readelf](https://img-blog.csdn.net/20180507224406233?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，我们已经拿到了Key1、Key2、Key4。离成功只差一步之遥，即——将Key直接填写进文本框中！虽然我们并没有拿到Key3，不过网页提示的任务目标中并不要求我们提交Key3。

![第五关星际选手](https://img-blog.csdn.net/20180507224931876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

提交之后，弹出提示框了一个网页，当我们进入这个网页之后，便进入了这个页面：

![通关撒花](https://img-blog.csdn.net/20180507225018618?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hlcGFuZ2Rh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

呼～～经过好长时间，看来小组2018的免试题终于全部通关了，那么，2018年西邮Linux兴趣小组免试题的揭秘便到此结束！顺便一说，截止今天，小组2018年的免试题仍然在线，大家若有兴趣也可以继续尝试哟～

> 
通关只是目的之一，免试题并不是难住所有做题的人才高兴，希望你能在免试题的旅途上见到很多全新的东西，也许可以引发你的思考，也许能让你兴致盎然。愿你在技术的道路上越走越远～～

参考链接：

[2017 Linux兴趣小组免试题解析](https://blog.csdn.net/tanswer_/article/details/70881447)
[2016 Linux兴趣小组免试题解析](http://blog.csdn.net/yangbodong22011/article/details/51286850)
[2015 Linux兴趣小组免试题解析](http://www.s0nnet.com/archives/xiyoulinux2015)
[2014 Linux兴趣小组免试题解析](http://blog.csdn.net/tutulove1234/article/details/24781255)
[2013 Linux兴趣小组免试题解析](http://www.cnblogs.com/ma6174/archive/2013/05/04/3058889.html)

