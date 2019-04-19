# Qt 判断文件或文件夹是否存在及创建文件夹 - xqhrs232的专栏 - CSDN博客
2018年05月14日 17:58:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1173
原文地址::[https://blog.csdn.net/lusirking/article/details/51644782](https://blog.csdn.net/lusirking/article/details/51644782)
1. 判断文件夹是不是存在
参数说明：
QString fullPath;//文件夹全路径
/*方法1*/
bool isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    return false;
}
/*方法2*/
bool isDirExist(QString fullPath)
{
    QFileInfo fileInfo(fullPath);
    if(fileInfo.isDir())
    {
      return true;
    }
    return false;
}
2. 判断文件是不是存在
参数说明：
QString fullFileName;//文件全路径(包含文件名)
/*方法1*/
bool isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fileFullName);
    if(fileInfo.isFile())
    {
        return true;
    }
    return false;
}
3、判断文件或文件夹是不是存在(即不确定字符串是文件还是文件夹路径)
参数说明：
QString fullFilePath;//路径名
/*方法1*/
bool isFileExist(QString fullFilePath)
{
    QFileInfo fileInfo(fullFilePath);
    if(fileInfo.exists())
    {
        return true;
    }
    return false;
}
/*方法2*/
bool isFileExist(QString fullFilePath)
{
    QFile file(fullFilePath);
    if(file.exists())
    {
        return true;
    }
    return false;
}
4、判断文件夹是否存在,不存在则创建
/*方法1*/
bool isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    else
    {
       bool ok = dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
       return ok;
    }
}
/*方法1*/
bool isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    else
    {
       bool ok = dir.mkpath(fullPath);//创建多级目录
       return ok;
    }
}
5、以下为摘录的其他网络测试代码
{
    QFileInfo fi("C:/123");                     // 目录存在
    qDebug() << fi.isFile();                    // false
    qDebug() << fi.isDir();                     // true
    qDebug() << fi.exists();                    // true
    qDebug() << fi.isRoot();                    // false
    qDebug() << QFile::exists("C:/123");        // true
    qDebug() << QDir("C:/123").exists();        // true
    fi.setFile("C:/ABC");                       // 目录不存在
    qDebug() << fi.isFile();                    // false
    qDebug() << fi.isDir();                     // false
    qDebug() << fi.exists();                    // false
    qDebug() << fi.isRoot();                    // false
    qDebug() << QFile::exists("C:/ABC");        // false
    qDebug() << QDir("C:/ABC").exists();        // false
    fi.setFile("C:/");                          // 存在的驱动器
    qDebug() << fi.isFile();                    // false
    qDebug() << fi.isDir();                     // true
    qDebug() << fi.exists();                    // true
    qDebug() << fi.isRoot();                    // true
    qDebug() << QFile::exists("C:/");           // true
    qDebug() << QDir("C:/").exists();           // true
    fi.setFile("Z:/");                          // 不存在的驱动器
    qDebug() << fi.isFile();                    // false
    qDebug() << fi.isDir();                     // false
    qDebug() << fi.exists();                    // false
    qDebug() << fi.isRoot();                    // false
    qDebug() << QFile::exists("Z:/");           // false
    qDebug() << QDir("Z:/").exists();           // false
    fi.setFile("C:/123.lnk");                   // 快捷方式存在且指向的文件也存在
    qDebug() << fi.isFile();                    // true
    qDebug() << fi.isDir();                     // false
    qDebug() << fi.exists();                    // true
    qDebug() << fi.isRoot();                    // false
    qDebug() << QFile::exists("C:/123.lnk");    // true
    qDebug() << QDir("C:/123.lnk").exists();    // false
    fi.setFile("C:/456.lnk");                   // 快捷方式存在但指向的文件不存在
    qDebug() << fi.isFile();                    // false
    qDebug() << fi.isDir();                     // false
    qDebug() << fi.exists();                    // false
    qDebug() << fi.isRoot();                    // false
    qDebug() << QFile::exists("C:/456.lnk");    // false
    qDebug() << QDir("C:/456.lnk").exists();    // false
}
可以看到，容易让人感到混乱的是exists方法，这个方法是通用的判断方法，可以看成是这样的表达式
exists() == (isFile() || isDir()) 
也就是说判断文件或文件夹是否存在单纯用exists方法是不严谨的
比如你的本意是判断文件是否存在，但文件不存在，而恰巧有个同名的文件夹，那么exists也会返回true。文件夹也是同理
根据上面的代码作出的一点总结
准确判断文件是否存在
1.用QFileInfo::isFile()方法 
准确判断文件夹是否存在
1.用QFileInfo::isDir()方法
2.用QDir::exists()方法 
不确定字符串是文件还是文件夹路径
1.用QFileInfo::exists()方法
2.用QFile::exists()方法
