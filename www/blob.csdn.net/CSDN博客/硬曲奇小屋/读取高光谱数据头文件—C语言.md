# 读取高光谱数据头文件—C语言 - 硬曲奇小屋 - CSDN博客





2017年10月10日 11:03:01[HardCookies](https://me.csdn.net/james_616)阅读数：830








做高光谱数据处理，第一步当然是读取头文件。有过相关经验的人都知道，头文件是以*.hdr*结尾的文本文件，主要是每行表示相关的数据。以下是ENVI自带的**cup95eff**数据的头文件。

```
ENVI
description = {
  1995 AVIRIS "Effort" Corrected ATREM [Thu Apr 25 00:52:03 1996]}
samples = 400
lines   = 350
bands   = 50
header offset = 0
file type = ENVI Standard
data type = 2
interleave = bil
sensor type = Unknown
byte order = 0
x start = 215
y start = 295
default bands = {12,22,36}
wavelength units = Micrometers
reflectance scale factor = 1000
band names = {
 Band 172, Band 173, Band 174, Band 175, Band 176, Band 177, Band 178, 
 Band 179, Band 180, Band 181, Band 182, Band 183, Band 184, Band 185, 
 Band 186, Band 187, Band 188, Band 189, Band 190, Band 191, Band 192, 
 Band 193, Band 194, Band 195, Band 196, Band 197, Band 198, Band 199, 
 Band 200, Band 201, Band 202, Band 203, Band 204, Band 205, Band 206, 
 Band 207, Band 208, Band 209, Band 210, Band 211, Band 212, Band 213, 
 Band 214, Band 215, Band 216, Band 217, Band 218, Band 219, Band 220, 
 Band 221}
wavelength = {
 1.990800, 2.000900, 2.010900, 2.020900, 2.030900, 2.040900, 2.050900,
 2.060900, 2.071000, 2.081000, 2.091000, 2.101000, 2.111000, 2.121000,
 2.130900, 2.140900, 2.150900, 2.160900, 2.170900, 2.180900, 2.190800,
 2.200800, 2.210800, 2.220800, 2.230700, 2.240700, 2.250600, 2.260600,
 2.270600, 2.280500, 2.290400, 2.300400, 2.310400, 2.320300, 2.330200,
 2.340200, 2.350100, 2.360000, 2.370000, 2.379900, 2.389800, 2.399700,
 2.409600, 2.419600, 2.429500, 2.439400, 2.449300, 2.459200, 2.469100,
 2.479000}
```

一般情况下，我们所关心的是行数 lines, 列数 samples, 波段数 bands以及数据格式 interleave。下面我们编写程序read_hdr，读取头文件中前三个值。

程序的基本思路是将头文件名作为参数给程序，然后分别调用三个函数readlines, readsamples, readbands，分别返回lines, samples, bands三个整数值输出。程序代码如下：

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 20

/* read_hdr函数：读取高光谱数据的头文件 */
int main(int argc, char *argv[])
{
    FILE *fp;
    char *prog = argv[0];       /* 记下程序名 */
    char *hdr_file = argv[1];   /* 记下头文件名 */

    int readsamples(FILE *);
    int readlines(FILE *);
    int readbands(FILE *);

    int samples, lines, bands;  /* 定义需要读取的变量 */

    if (argc == 1)  /* 如果没有命令行参数，则返回错误 */
    {
        printf("no hdr file to read\n");
        exit(1);
    }
    else if (argc == 2)
    {
        if ((fp = fopen(hdr_file, "r")) == NULL)
        {
            fprintf(stderr, "%s: can't open %s\n", prog, hdr_file);
        }
        else
        {
            printf("Reading...\n");
            samples = readsamples(fp);
            lines = readlines(fp);
            bands = readbands(fp);
            fclose(fp);
            printf("samples: %d\n", samples);
            printf("lines: %d\n", lines);
            printf("bands: %d\n", bands);
        }
    }
    else
    {
        printf("only recept 1 hdr file\n");
        exit(2);
    }

    return 0;
}


/* readsamples函数：读取头文件中samples的值 */
int readsamples(FILE *ifp)
{
    int samples = 0;                // 初始化返回值
    char line[MAXLINE];             // 读取的一行
    char item[MAXLINE];             // 读取的属性名称
    int value;                      // 读取的属性值

    while ((fgets(line, MAXLINE, ifp)) != 0)
    {
        sscanf(line, "%[^=]", item);                // 读取等号之前的内容
        sscanf(line, "%*[^=]=%d", &value);          // 读取等号之后的内容，这里注意参数类型为指针需要‘&’
        if ((strcmp(item, "samples")) >= 0)
            break;
    }
    samples = value;
    return samples;
}

/* readlines函数：读取头文件中samples的值 */
int readlines(FILE *ifp)
{
    int lines = 0;                  // 初始化返回值
    char line[MAXLINE];             // 读取的一行
    char item[MAXLINE];             // 读取的属性名称
    int value;                      // 读取的属性值

    while ((fgets(line, MAXLINE, ifp)) != 0)
    {
        sscanf(line, "%[^=]", item);                // 读取等号之前的内容
        sscanf(line, "%*[^=]=%d", &value);          // 读取等号之后的内容，这里注意参数类型为指针需要‘&’
        if ((strcmp(item, "lines")) >= 0)
            break;
    }
    lines = value;
    return lines;
}

/* readbands函数：读取头文件中samples的值 */
int readbands(FILE *ifp)
{
    int bands = 0;                  // 初始化返回值
    char line[MAXLINE];             // 读取的一行
    char item[MAXLINE];             // 读取的属性名称
    int value;                      // 读取的属性值

    while ((fgets(line, MAXLINE, ifp)) != 0)
    {
        sscanf(line, "%[^=]", item);                // 读取等号之前的内容
        sscanf(line, "%*[^=]=%d", &value);          // 读取等号之后的内容，这里注意参数类型为指针需要‘&’
        if ((strcmp(item, "bands")) >= 0)
            break;
    }
    bands = value;
    return bands;
}
```

编译运行，结果如下图所示。 
![这里写图片描述](https://img-blog.csdn.net/20171010110158686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



