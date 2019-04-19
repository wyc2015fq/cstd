# Qt判断字符串是否是纯英文，纯中文，纯数字，英文或数字，中文或数字 - xqhrs232的专栏 - CSDN博客
2018年08月31日 15:55:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：101
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[https://blog.csdn.net/qq_36391817/article/details/80708590](https://blog.csdn.net/qq_36391817/article/details/80708590)
做项目的时候需要对字符串做下校验，以下是在前辈们的肩膀上并结合自己项目的需要做的一些总结内容。
## 是否是纯英文
```
bool AIFaceDBConfig::IsEnglish(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z'))
        {
        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}
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
## 是否是纯数字
```
bool AIFaceDBConfig::IsNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if(*s>='0' && *s<='9')
        {
        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}
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
## 是否是纯中文
```
bool AIFaceDBConfig::IsNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if(*s>='0' && *s<='9')
        {
        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}
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
## 是否是英文或数字
```
bool AIFaceDBConfig::IsEnglishOrNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z') || (*s>='0' && *s<='9'))
        {
        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}
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
## 是否是中文或数字
```
bool AIFaceDBConfig::IsChineseOrNumber(QString &qstrSrc)
{
    int nCount = qstrSrc.count();
    bool bret = true;
    for(int i=0; i<nCount; ++i)
    {
        QChar cha = qstrSrc.at(i);
        ushort uni = cha.unicode();
        if((uni >= 0x4E00 && uni <= 0x9FA5)  || (uni >= '0' && uni <= '9'))
        {
        }
        else
        {
            bret = false;
            break;
        }
    }
    return bret;
}
```
