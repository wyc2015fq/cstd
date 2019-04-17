# 如何用Python爬取LOL官网全英雄皮肤 - zhusongziye的博客 - CSDN博客





2018年11月25日 10:42:12[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：103








**今天小编带你爬取LOL官网全英雄皮肤的图片**

不要失望，也不要难过

**接下咱们来讲讲怎么爬取LOL官网**

![](https://img-blog.csdnimg.cn/20181125103737407.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



本次案例使用到的模块

```
import requests
import re
import json
```

安装模块：

```
pip install requests
#re、json都是自带的不需要安装
```



**01 获取JS源代码  获取英雄的ID**

首先让咱们找到每个英雄皮肤的地址，F12打开开发者工具页面选择器，选中图片自动找寻到图片的地址

![](https://img-blog.csdnimg.cn/2018112510381720.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



龙的传人 李青的图片地址：

http://ossweb-img.qq.com/images/lol/web201310/skin/big64003.jpg

神僧 李青图片地址：

http://ossweb-img.qq.com/images/lol/web201310/skin/big64011.jpg



不难发现其中的规律：

big64003.jpg

big64011.jpg

很明显 64是英雄的ID地址 003是皮肤顺序

![](https://img-blog.csdnimg.cn/20181125103905230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20181125103914907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



找到JS源代码，你会发现英雄的ID地址并不是按照顺序排列下来的！

```
def getLOLImages():
    url_js = 'http://lol.qq.com/biz/hero/champion.js'
    #获取JS源代码 str bytrs字节
    res_js = requests.get(url_js).content
    #转码
    html_js = res_js.decode()
    #正则表达
    req = '"keys":(.*?),"data"'
    list_js = re.findall(req,html_js)
    # str → dict
    dict_js = json.loads(list_js[0])
    print(dict_js)
```



![](https://img-blog.csdnimg.cn/20181125103937983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

很多不明白这个正则表达式怎么写，这里简单的说明一下：

![](https://img-blog.csdnimg.cn/20181125103949917.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

前面是 “keys”：后面是，“data” 然后我们需要提取的是中间的部分，那么不管它们是啥，我们就是.?* 通配符全部匹配出来。

后面就不详细讲解了，本篇文章最后，小编会带上详细的视频讲解

**02 拼接URL地址、获取下载图片的地址**

```
pic_list = []
    for key in dict_js:
        #print(key)
        for i in range(20):
            num = str(i)
            if len(num) == 1:
                hreo_num = "00"+num
            elif len(num) == 2:
                hreo_num = "0"+num
            numstr = key+hreo_num
            url = "http://ossweb-img.qq.com/images/lol/web201310/skin/big"+numstr+".jpg"
            print(url)
            pic_list.append(url)

            list_filepath = []
            path = "图片保存地址"
            #print(dict_js.values())
            for name in dict_js.values():
                for i in range(20):
                    file_path = path + name + str(i) + '.jpg'
                    list_filepath.append(file_path)
                    #print(list_filepath)
```

**03 下载图片**

```
n = 0               
    for picurl in pic_list:
        res = requests.get(picurl)
        n+=1

        if res.status_code ==200:

            print("正在下载%s"%list_filepath[n])

            with open(list_filepath[n],'wb') as f:
                f.write(res.content)
```



**04 详细视频学习地址**



链接：https://pan.baidu.com/s/1TbPuMUsKvuk9bqh3hm6wNQ 

密码：ewyk



