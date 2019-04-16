# Kaggle问题总结1 - 去向前方的博客 - CSDN博客





2019年03月19日 12:31:03[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：50








# 目录




### 文章目录
- [目录](#_0)
- [前言](#_2)
- [账号验证](#_5)
- [ie失败](#ie_7)
- [电话号码验证](#_22)
- [API数据下载](#API_27)




# 前言

Kaggle是一个国际级的数据科学比赛网站，上面有很多相应的数据比赛可以参加。

但是参加这个比赛不容易，账号注册，验证等等都会遇到很多问题，本文对作者参加比赛过程中遇到的问题进行一个小小的总结，希望能帮助到后来的各位。

# 账号验证

账号验证这一块儿的坑在于,如果你用普通浏览器是不行的.

## ie失败

直接点击邮件里的activate,会这样:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319121048509.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)反馈:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319121539972.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)
复制链接,使用火狐浏览器:

反馈

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319121552973.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)

复制链接使用谷歌浏览器:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319121602519.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)选择完相应的选项后就没有问题了.
# 电话号码验证

输入的时候不能直接输电话号码,而要加上区号

+860手机号

即可:

例如，若手机号为：13275151551，则格式为“+86013275151551“
# API数据下载

为了方便下载Kaggle数据,kaggle开发了这个API,使用方法如下.

首先在cmd窗口下:pip install kaggle.下载kaggle库.

然后找到kaggle.exe
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319122047856.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)双击这个软件，之后，你的电脑文件里就会出现.kaggle这个文件夹。
**注意，在双击这个kaggle.exe之前，你是找不到相应的文件夹的**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319122300993.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)然后就可以找到相应文件夹了。

接下来是找到这个kaggle.json文件

首先点击,my account.

然后点击creat new API Token,就行。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319122423181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319122608706.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)输入：

kaggle competitions download -c quora-insincere-questions-classification

就可以下载文件了

这个数据集的下载命令来自：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319123019649.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==,size_16,color_FFFFFF,t_70)娶你向下的数据集里下载咯。







