# VC判断UTF-8与ANSI - 凯奥斯的注释 - CSDN博客





2013年10月24日 16:15:26[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：994








原文地址[http://blog.neohope.com/2012/01/04/utf-8_or_gbk/](http://blog.neohope.com/2012/01/04/utf-8_or_gbk/)




大家知道，如果只有英文的话，UTF-8与ANSI是一样的
但有了中文以后，情况就很不一样了，
在ANSI中，比如GBK，中文占两字节，
在UTF-8中，中文占三字节，
当中英文混合时，情况就更复杂一些了。
下面一段是在以前项目中，先判断是UTF-8还是GBK然后转为UNICODE的代码

|```cpp//要判断内容char *s1="....";//字符编码UINT CodePage=0;//字符串长度int nLen=strlen(s1); //判断是否为UTF-8//至少要3字节if(nLen>=3){    unsigned char U1,U2,U3;    int nNow=0;    while(nNow<nLen)    {        U1=(unsigned)s1[nNow];        if((U1&0x80)==0x80)        {            //中文字符，则要三个字符            if(nLen>nNow+2)            {                U2=(unsigned)s1[nNow+1];                U3=(unsigned)s1[nNow+2];                //中文三字节为0xE0 0xC0 0xC0                if(((U1&0xE0)==0XE0) && ((U2&0xC0)==0x80) && ((U3&0xC0)==0x80))                {                    //有可能是UTF-8                    CodePage=65001;                    nNow=nNow+3;                }                else                {                    //不是UTF-8                    CodePage=0;                    break;                }            }            else            {                //不是UTF-8                CodePage=0;                break;            }        }        else        {            //非中文字符            nNow++;        }    }} DWORD dwNum;dwNum=MultiByteToWideChar(CodePage,0,s1,-1,NULL,0);if(dwNum){    wchar_t *pwText;    pwText=new TCHAR[dwNum];    if(pwText)    {        MultiByteToWideChar(CodePage,0,s1,-1,pwText,dwNum);    }    szPatientName=pwText;    delete []pwText;}```|
|----|




