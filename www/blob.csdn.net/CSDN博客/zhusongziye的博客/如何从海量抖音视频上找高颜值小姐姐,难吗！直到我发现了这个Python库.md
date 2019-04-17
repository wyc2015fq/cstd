# 如何从海量抖音视频上找高颜值小姐姐,难吗！直到我发现了这个Python库 - zhusongziye的博客 - CSDN博客





2019年01月13日 20:12:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：291








Python真的是无所不能，有同学说万能的Python,其实就是指的Python的库真的很多很多！坐地铁的时候，经常看到周围人刷抖音的，看短视频。抖音上面的美女小姐姐真的是很多，今天在Github上一逛，发现关于抖音的库好多啊！



![](https://img-blog.csdnimg.cn/20190113201153553.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

**有81个repo**,其中点赞最多的是Douyin-Bot,这个库有近**5000**多颗星，近**1000**个fork,厉害啊！我们一起来看看这个神奇的库！



**抖音机器人**



这个库的作者叫 "**wangshub**",他用 Python 加ADB 做了一个 Python 抖音机器人 Douyin-Bot，有点类似与当年风靡一时用机器人玩**微信跳一跳**游戏。



![](https://img-blog.csdnimg.cn/2019011320124029.gif)



**1.看一些它的原理：**
- 
也是利用ADB工具，让手机和电脑相连

- 
然后打开抖音短视频 App,截取手机上美女小姐姐的图片

- 
然后对图片进行压缩，最厉害的部分来了

- 
作者利用腾讯的开放的AI接口，进行人脸识别，进行图形切割和对比

- 
当颜值大于门限值 `BEAUTY_THRESHOLD`时，点赞并关注；

- 
然后循环下一页，继续第一步；




**2.看看程序获取的脸部图片**

![](https://img-blog.csdnimg.cn/20190113201219585.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**3.研究一下库的入口源码**

```
def main():
   """
   main
   :return:
   """
   print('程序版本号：{}'.format(VERSION))
   print('激活窗口并按 CONTROL + C 组合键退出')
   debug.dump_device_info()
   screenshot.check_screenshot()

   while True:
       next_page()

       time.sleep(1)
       screenshot.pull_screenshot()

       resize_image('autojump.png', 'optimized.png', 1024*1024)

       with open('optimized.png', 'rb') as bin_data:
           image_data = bin_data.read()

       ai_obj = apiutil.AiPlat(AppID, AppKey)
       rsp = ai_obj.face_detectface(image_data, 0)

       major_total = 0
       minor_total = 0

       if rsp['ret'] == 0:
           beauty = 0
           for face in rsp['data']['face_list']:
               print(face)
               face_area = (face['x'], face['y'], face['x']+face['width'], face['y']+face['height'])
               print(face_area)
               img = Image.open("optimized.png")
               cropped_img = img.crop(face_area).convert('RGB')
               cropped_img.save(FACE_PATH + face['face_id'] + '.png')
               # 性别判断
               if face['beauty'] > beauty and face['gender'] < 50:
                   beauty = face['beauty']

               if face['age'] > GIRL_MIN_AGE:
                   major_total += 1
               else:
                   minor_total += 1

           # 是个美人儿~关注点赞走一波
           if beauty > BEAUTY_THRESHOLD and major_total > minor_total:
               print('发现漂亮妹子！！！')
               thumbs_up()
               follow_user()

       else:
           print(rsp)
           continue
```

(程序可以左右滑动)



**思路还是非常清晰的:**
- 
一开始进行初始化的工作，检查Py的版本，检查手机ada 是否链接好了，屏幕是否能截屏

- 
接着开始一个死循环，不断的截屏每一页上的美女的图片，压缩存储！

- 
然后读取图片，调用AI接口进入分析，返回一个ret字典信息，对里面颜值和年龄进行分析

- 
最后对颜值高的美女自动点赞，关注！




**作者真的是很有才，谁说码农比较闷，其实我们是很有趣的程序猿！我已经发邮件给他了，回头看看能否认识一下！**



源码库的链接：https://github.com/wangshub/Douyin-Bot



