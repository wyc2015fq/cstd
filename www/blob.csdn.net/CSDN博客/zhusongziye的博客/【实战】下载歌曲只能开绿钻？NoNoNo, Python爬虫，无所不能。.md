# 【实战】下载歌曲只能开绿钻？NoNoNo, Python爬虫，无所不能。 - zhusongziye的博客 - CSDN博客





2018年10月26日 21:58:02[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：91








**引言**

给大家带来绝对福利，之前在某Q音乐上听歌，如果想要下载某首歌曲，总是提示小编开通绿钻，作为一个爬虫爱好者，怎么可能花这冤枉钱呢？所以，办法总是人想的，具体过程就不多说了，最终结果就是通过100行代码，可以下载只有绿钻才能下载的歌曲哦~



**源码**

**链接：https://pan.baidu.com/s/1Ti-ybrquCf3X7qGPcdAEQA 密码：d2zr**



**责任声明**

**本代码只用于学习交流，严谨用于其他用途。所造成的责任影响不归本人负责。**



**实现功能**

通过指定歌曲名称，可以将搜索结果第一页中的歌曲下载完成。



**部分源码**

```
def parse_17(song_name):
    base_url = "https://c.y.qq.com/soso/fcgi-bin/client_search_cp"

    r = requests.get(base_url,params=parames,headers=headers)

    return_data = r.text
    pattern_17 = re.compile('\\((.*)?\\)')
    return_data_json = eval(pattern_17.search(return_data)[0].replace("null","None"))
    data = return_data_json["data"]["song"]["list"]
    for n,d in enumerate(data):
        songmid = (d['songmid'])
        strMediaMid = (d['strMediaMid'])

        parse_17_2 (songmid,strMediaMid,n+1,song_name)
```



```
def parse_17_2(songmid,strMediaMid,n,song_name):

    base_url2 = "https://c.y.qq.com/base/fcgi-bin/fcg_music_express_mobile3.fcg"
    r = requests.get(base_url2,params=params_2,headers=headers)
    return_data = r.text
    pattern_17 = re.compile('\\((.*)?\\)')
    return_data_json = eval(pattern_17.search(return_data)[0].replace("null","None"))
    data = return_data_json["data"]["items"][0]
    filename = data['filename']
    vkey = data['vkey']

    parse_17_3(filename,vkey,n,song_name)
```



```
def parse_17_3(filename,vkey,n,song_name):
    url = "http://dl.stream.qqmusic.qq.com/"
    url = url + filename
    params = {
        'vkey': vkey ,
        'guid': '8208467632' ,
        'uin':'0'  ,
        'fromtag':'66'
    }

    r = requests.get(url,params=params,headers=headers)
    filename = song_name + str(n) + ".m4a"
    with open(filename,"wb") as f :
        f.write(r.content)
    print("第%s首歌曲下载完成"%n)
```



