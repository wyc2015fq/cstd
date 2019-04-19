# QT中用openssl做rsa加密解密 - xqhrs232的专栏 - CSDN博客
2018年09月07日 16:10:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：210
原文地址::[https://blog.csdn.net/yajunsun/article/details/78909920](https://blog.csdn.net/yajunsun/article/details/78909920)
相关文章
1、Qt5使用openssl实现RSA数字签名----[https://blog.csdn.net/usister/article/details/74390949](https://blog.csdn.net/usister/article/details/74390949)
2、Qt实现的RSA密钥生成器[2048bit]， 附带项目源码和工程文件----[https://download.csdn.net/download/lb1885727/9942689](https://download.csdn.net/download/lb1885727/9942689)
首先，你得安装openssl，去官网下载最新版本就行，
安装完成后用命令行打开到openssl安装目录，先生成私钥，私钥保存到此目录的private.pem中，密钥长度2048位，然后再从私钥中提取公钥，公钥保存到此目录的public.pem中，当然，文件名称自己定。
![](https://blog.csdn.net/yajunsun/article/details/78909920)
按照上述命令生成好私钥公钥文件后，直接编辑打开把密钥复制出来用，或者在程序中直接用公钥私钥文件加载RSA对象。
QT中数据类型转换还是很不好搞的，下面代码中，密钥，明文，密文我都用的QString作为参数传入，分片加密和解密都有，也是我在项目中使用的版本，我刚学QT不就，如果各位发现我写的有问题的地方，请指正。
代码：
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
/**
 * @brief RSASignature::createRSA 载入密钥
 * @param key 密钥
 * @param publi 公钥1 私钥0
 * @return
 */
RSA * RSASignature::createRSA(unsigned char * key,int publi)
{
    RSA *rsa= NULL;
    BIO *keybio ;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio==NULL)
    {
        qDebug()<< "Failed to create key BIO";
        return 0;
    }
    if(publi)
    {
        rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa,NULL, NULL);
    }
    else
    {
        rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa,NULL, NULL);
    }
    if(rsa == NULL)
    {
        qDebug()<< "Failed to create RSA";
    }
    return rsa;
}
/**
 * @brief RSASignature::public_encrypt 公钥加密
 * @param data 待加密数据
 * @param data_len 待加密的数据长度
 * @param key 公钥
 * @param encrypted 加密后的数据
 * @return 加密长度
 */
int RSASignature::public_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    result = RSA_public_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}
/**
 * @brief RSASignature::private_decrypt 私钥解密
 * @param enc_data 待解密数据
 * @param data_len 待解密数据长度
 * @param key 私钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::private_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}
/**
 * @brief RSASignature::private_encrypt 私钥加密
 * @param data 待加密数据
 * @param data_len 待加密数据长度
 * @param key 私钥
 * @param encrypted 加密后数据
 * @return
 */
int RSASignature::private_encrypt(unsigned char * data,int data_len,unsigned char * key, unsigned char *encrypted)
{
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    qDebug()<<RSA_size(rsa);
    int result = RSA_private_encrypt(data_len,data,encrypted,rsa,padding);
    return result;
}
/**
 * @brief RSASignature::public_decrypt 公钥解密
 * @param enc_data 待解密数据
 * @param data_len 待解密数据长度
 * @param key 公钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::public_decrypt(unsigned char * enc_data,int data_len,unsigned char * key, unsigned char *decrypted)
{
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    qDebug()<<RSA_size(rsa);
    int  result = RSA_public_decrypt(data_len,enc_data,decrypted,rsa,padding);
    return result;
}
/**
 * @brief RSASignature::public_encrypt 公钥加密
 * @param data 待加密数据
 * @param keystr 公钥
 * @param encrypted 加密后数据
 * @return
 */
int RSASignature::public_encrypt(QString &data,QString &keystr,QString &encrypted)
{
    QByteArray keydata=keystr.toLocal8Bit();
    unsigned char *key= (unsigned char*)strdup(keydata.constData());//密钥
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    free(key);
    int rsasize=RSA_size(rsa);
    int exppadding=rsasize;
    int result=-1;
    QByteArray decdata=QByteArray::fromStdString(data.toStdString()).toBase64(QByteArray::Base64Encoding);
    QByteArray signByteArray;
    int data_len=decdata.length();
    if(data_len>exppadding-11)
        exppadding=exppadding-11;
    int b=0;
    int s=data_len/(exppadding);//片数
    if(data_len%(exppadding))
        s++;
    for(int i=0;i<s;i++)
    {
        QByteArray subdata={0};
        for(int j=0;j<exppadding;j++)
        {
            if(i*exppadding+j>data_len)
                break;
            subdata[j]=decdata[j+i*exppadding];
        }
        unsigned char *smldata=(unsigned char*)strdup(subdata.constData());//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_public_encrypt(exppadding,smldata,smlencrypted,rsa,padding);
        if(b>0)
        {
            QByteArray subarray=QByteArray::fromRawData((const char *)smlencrypted,rsasize);
            signByteArray.append(subarray);
        }
        free(smldata);
    }
    QString str(signByteArray.toHex());
    qDebug()<<str;
    encrypted.append(str);
    result=b;
    return result;
}
/**
 * @brief RSASignature::private_decrypt 私钥解密
 * @param data 待解密数据
 * @param keystr 私钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::private_decrypt(QString &data,QString &keystr,QString &decrypted)
{
    QByteArray keydata=keystr.toLocal8Bit();
    unsigned char *key= (unsigned char*)strdup(keydata.constData());//密钥
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    free(key);
    int rsasize=RSA_size(rsa);
    int result=-1;
    QByteArray encdata=QByteArray::fromHex(QByteArray::fromStdString( data.toStdString()));
    QByteArray signByteArray;
    int data_len=encdata.length();
    int b=0;
    int s=data_len/(rsasize);//片数
    if(data_len%(rsasize))
        s++;
    for(int i=0;i<s;i++)
    {
        QByteArray subdata={0};
        for(int j=0;j<rsasize;j++)
        {
            if(i*rsasize+j>data_len)
                break;
            subdata[j]=encdata[j+i*rsasize];
        }
        unsigned char *smldata=(unsigned char*)subdata.data();//(unsigned char*)strdup(subdata.constData());//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_private_decrypt(rsasize,smldata,smlencrypted,rsa,padding);
        if(b>0)
        {
            QByteArray decdata((char*)smlencrypted);
            signByteArray.append(decdata);
        }
    }
    QByteArray b1= QByteArray::fromBase64(signByteArray,QByteArray::Base64Encoding);
    std::string str=b1.toStdString();
    decrypted.append(QString::fromStdString( str));
    result=b;
    return result;
}
/**
 * @brief RSASignature::private_encrypt 私钥加密
 * @param data 待加密数据
 * @param keystr 私钥
 * @param encrypted 解密后的数据
 * @return
 */
int RSASignature::private_encrypt(QString &data,QString &keystr,QString &encrypted)
{
    QByteArray keydata=keystr.toLocal8Bit();
    unsigned char *key= (unsigned char*)strdup(keydata.constData());//密钥
    RSA * rsa = createRSA(key,0);
    if(rsa==NULL)
        return 0;
    free(key);
    int rsasize=RSA_size(rsa);
    int exppadding=rsasize;
    int result=-1;
    QByteArray decdata=QByteArray::fromStdString(data.toStdString()).toBase64(QByteArray::Base64Encoding);
    QByteArray signByteArray;
    int data_len=decdata.length();
    if(data_len>exppadding-11)//padding占11位
        exppadding=exppadding-11;
    int b=0;
    int s=data_len/(exppadding);//片数
    if(data_len%(exppadding))
        s++;
    for(int i=0;i<s;i++)
    {
        //分片加密
        QByteArray subdata={0};
        for(int j=0;j<exppadding;j++)
        {
            if(i*exppadding+j>data_len)
                break;
            subdata[j]=decdata[j+i*exppadding];
        }
        unsigned char *smldata=(unsigned char*)strdup(subdata.constData());//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_private_encrypt(exppadding,smldata,smlencrypted,rsa,padding);
        if(b>0)
        {
            QByteArray subarray=QByteArray::fromRawData((const char *)smlencrypted,rsasize);
            signByteArray.append(subarray);
        }
        free(smldata);
    }
    QString str(signByteArray.toHex());
    qDebug()<<str;
    encrypted.append(str);
    result=b;
    return result;
}
/**
 * @brief RSASignature::public_decrypt 公钥解密
 * @param data 待解密数据
 * @param keystr 公钥
 * @param decrypted 解密后的数据
 * @return
 */
int RSASignature::public_decrypt(QString &data,QString &keystr,QString &decrypted)
{
    QByteArray keydata=keystr.toLocal8Bit();
    unsigned char *key= (unsigned char*)strdup(keydata.constData());//密钥
    RSA * rsa = createRSA(key,1);
    if(rsa==NULL)
        return 0;
    free(key);
    int rsasize=RSA_size(rsa);
    int result=-1;
    QByteArray encdata=QByteArray::fromHex(QByteArray::fromStdString( data.toStdString()));
    QByteArray signByteArray;
    int data_len=encdata.length();
    int b=0;
    int s=data_len/(rsasize);//片数
    if(data_len%(rsasize))
        s++;
    for(int i=0;i<s;i++)
    {
        QByteArray subdata={0};
        for(int j=0;j<rsasize;j++)
        {
            if(i*rsasize+j>data_len)
                break;
            subdata[j]=encdata[j+i*rsasize];
        }
        unsigned char *smldata=(unsigned char*)subdata.data();//(unsigned char*)strdup(subdata.constData());//数据分片
        unsigned char smlencrypted[1024]={0};//片段加密数据
        b +=RSA_public_decrypt(rsasize,smldata,smlencrypted,rsa,padding);
        if(b>0)
        {
            QByteArray decdata((char*)smlencrypted);
            signByteArray.append(decdata);
        }
    }
    QByteArray b1= QByteArray::fromBase64(signByteArray,QByteArray::Base64Encoding);
    std::string str=b1.toStdString();
    decrypted.append(QString::fromStdString( str));
    result=b;
    return result;
}
请自己提取头文件。
前面那段原始的加密解密方法未提供数据分片，所以只能加密长度小于245的字符数组
