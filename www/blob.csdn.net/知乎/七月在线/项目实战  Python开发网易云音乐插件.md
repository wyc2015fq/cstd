# 项目实战 | Python开发网易云音乐插件 - 知乎
# 



**来源：[https://github.com/wu-nerd/dmusic-plugin-NeteaseCloudMusic](https://link.zhihu.com/?target=https%3A//github.com/wu-nerd/dmusic-plugin-NeteaseCloudMusic)**

## **介绍**

深度音乐播放器的网易云音乐插件，可在Linux下播放用户创建、收藏的歌单，以及私人FM。 

1.基于sumary的dmusic-plugin-baidumusic修改而成
2.使用了NetEaseMusicBox中的网易云音乐API
3.非大陆用户可能无法播放音乐，请使用proxychains设置播放器代理或使用VPN

## **特性**
- 支持网易账号及手机号登录，支持新浪微博账号及二维码登录
- 播放创建的歌单、收藏的歌单
- 支持搜索、添加、删除歌曲
- 支持搜索、添加、删除歌单
- 播放私人FM，支持红心、取消红心、删除歌曲
- 支持每日歌曲推荐
- 支持从网易获取歌词及封面
![](https://pic2.zhimg.com/v2-015927539dd8c2537574a095b521616d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='451'></svg>)
## **安装**

安装深度音乐播放器


```
sudo apt-add-repository ppa:noobslab/deepin-sc
sudo apt-get updatesudo apt-get install deepin-music-player
```


这个源有段时间没有更新，因此Ubuntu 15.04可能无法安装。需要修改 /etc/apt/sources.list.d 文件夹下 noobslab-ubuntu-deepin-sc-vivid.list 文件，将其中的的 vivid 修改成 utopic ，然后 

`sudo apt-get updatesudo apt-get install deepin-music-player `

安装Requests库及pycrypto库：


```
sudo apt-get install python-pip
sudo pip install requests
sudo pip install pycrypto
```


对于默认使用Python 3的发行版比如Arch，请安装python2版本的库：


```
sudo pacman -Ss python2-pip
sudo pip2 install requests
sudo pip2 install pycrypto
```


安装网易云音乐插件


```
git clone https://github.com/wu-nerd/dmusic-plugin-NeteaseCloudMusic.git
cd dmusic-plugin-NeteaseCloudMusic
python2 install.py
```


## **使用**

运行深度音乐， 选项设置->附加组件 中启用网易云音乐即可 
![](https://pic3.zhimg.com/v2-50438eb3bbb26288c02825139b35a30a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='451'></svg>)
## **代码**


```
#! /usr/bin/env python

# -*- coding: utf-8 -*-

import os
import shutil
def copytree(src, dst):
    """Recursively copy a directory tree using copy2().
    Modified from shutil.copytree
    """
    base = os.path.basename(src)
    dst = os.path.join(dst, base)
    names = os.listdir(src)    
      if not os.path.exists(dst):
        os.makedirs(dst)    
      for name in names:
        srcname = os.path.join(src, name)        
            try:          
                if os.path.isdir(srcname):
                copytree(srcname, dst)          
                else:
                shutil.copy2(srcname, dst)    
            except Exception ,e:         
               print e          
               raise

def softlink(src, dst):
    link_dst = os.path.join(dst, src.rsplit('/', 1)[-1])    
    if os.path.exists(link_dst):
      os.system("rm -rf %s" % link_dst)    
      
    if not os.path.exists(dst):
       os.makedirs(dst)
    os.system("ln -s %s %s" % (src, dst))
    
if __name__ == "__main__":
    src = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'neteasecloudmusic')
    dst = os.path.join(os.path.expanduser("~"), ".local", "share", "deepin-music-player", "plugins")
    copytree(src, dst)
```


版权声明：转载文章和图片均来自公开网络，版权归作者本人所有，推送文章除非无法确认，我们都会注明作者和来源。如果出处有误或侵犯到原作者权益，请与我们联系删除或授权事宜。 


