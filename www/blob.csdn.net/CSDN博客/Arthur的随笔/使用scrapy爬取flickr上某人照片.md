# 使用scrapy爬取flickr上某人照片 - Arthur的随笔 - CSDN博客
2013年05月15日 20:41:23[largetalk](https://me.csdn.net/largetalk)阅读数：2596
flickr免费帐号坑爹的只有200张照片的限额，用用就到限制了，准备换个相册。可上面的照片一个一个下下来就麻烦了，正好想用用scrapy，现学现卖，写了一个。代码在
[https://github.com/largetalk/flickr_photo_crawl](https://github.com/largetalk/flickr_photo_crawl)
使用命令是： 
scrapy crawl flickr -a username=largetalk -o scraped_data.json -t json
完了在当前目录下有个 .photos 目录，里面是图片。
==========================================================
flckr改版，后新写了一个，代码在[https://github.com/largetalk/new_flickr_photo_crawl](https://github.com/largetalk/new_flickr_photo_crawl)

