
import sys
from you_get import common as you_get #导入you-get库 

import clip
directory = r'D:\\VDownload\\' #设置下载目录
    
url = 'https://www.bilibili.com/video/av47504269/' #需要下载的视频地址
url = flvpath = clip.gettext().decode('gbk').trip('\r\n')
sys.argv = ['you-get','-o',directory,url] #sys传递参数执行下载，就像在命令行一样 
you_get.main()
