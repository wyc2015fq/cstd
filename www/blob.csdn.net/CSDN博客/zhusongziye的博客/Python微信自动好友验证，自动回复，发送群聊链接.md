# Python微信自动好友验证，自动回复，发送群聊链接 - zhusongziye的博客 - CSDN博客





2018年06月09日 14:12:18[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3102










引言：






> 
一开始二维码加群，但是呢，这个东西隔一段时间会过期，我需要 每隔一段时间去更新二维码，然后当群人数超过100人了，只能邀请进群， 这个时候就需要别人添加我为好友，然后我通过，打开交流群，添加成员， 然后滚列表，一个个翻，找到那个人，然而我通讯录几百个人...后来我学会 了用标签，虽然可以过滤一部分人，但是还是要翻；还有一个问题就是， 加我的人时间的随机性，早上，下午，或者晚上，甚至还有深夜3,4点 加我的，最尴尬的是，刚拉完一个人，又有一个人加我，我又得拉一次... 还有一点就是不想陌生人窥探自己的朋友圈...





作为一个Pyer，肯定要想办法解决上述这些问题，解放自己的双手啊， 让机器人来帮我做这些事，说干就干，立马分析一波实现流程。




流程分析





大概流程如下：

Step 1.被添加好友，过滤关键词，看是否包含Py，Python 字(pi)眼，如果包含， 通过请求，并说一句欢迎词的骚话；

Step 2.过滤用户发送的关键词，响应相关信息：
- 
1.加群：发送加群链接

- 
2.博客：返回我的掘金主页链接

- 
3.公众号：返回公众号二维码图片

- 
4.打赏：返回一个动图，感谢词，微信付款二维码

- 
5.其他：返回一个黑人问号图片



流程是挺简单的，然而实际实现的时候一堆问题，大部分时间都在试错， 翻issues，谷歌






代码实现

好友验证通过

实现代码如下：




![](https://img-blog.csdn.net/20180609141047990?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





运行截图：




(要出现验证信息的话，需要双方互删！！！，另外如果你自己做机器人的话， 记得去设置 -> 隐私 -> 加我为朋友需要验证，把这个给勾选！！！)



![](https://img-blog.csdn.net/20180609141100341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




关键词回复





`@itchat.msg_register([TEXT])`：监测信息的，里面那个列表代表监测的类型， 这里目前只需要对文本进行过滤，除此之外还可以监听下面这些类型： PICTURE, RECORDING, ATTACHMENT, VIDEO，这个自己翻文档和issues去。 另外，这个方法，可以直接return一个字符串，会直接发送一个文本 给发送这个信息的人。

如果是其他类型的，就需要调用**`send_msg`和`send_image`这类方法 去发送信息了，参数都是两个(字符串/附件文件名，UserName)，这个UserName 可不是直接的'十二'这样的东西，是类似于@abcdefg1234567这样的东西， 这个东西你需要去获取，这里接手别人的信息就很好拿：`msg['FromUserName'])`** 如果是你主动发的话，就需要通过get_friends获得好友列表，或者通过search_friends， 得到一个列表，接着遍历，name等于好友名字的值，然后在拿到UserName。




接着说下发送群聊的

这个可把我坑惨了，试了N久都不发不了，没任何提示， 官方issues也是，看到一个什么超过40人无效之类的，要调用另外一个方法， 然而那个方法根本没有，后来折腾了好久才知道是name参数不对，第一个参数 不是直接传群名，而是和上面那个UserName一样是一个@xxx这样的东西，同样 需要通过get_chatrooms或者search_chatrooms拿到群聊列表，然后遍历列表 拿到你的这个群聊，才是正确的，然后呢**`add_member_into_chatroom`**() 这个函数的第二个参数，不是直接UserName，而是需要自己另外拼接成字典： [{'UserName': msg['FromUserName']}]，还有一个参数是：useInvitation=True， 是否以发送邀请链接的形式拉人，群聊满100人的话需要把这个属性设置为True。






大概细节的东西就这样，接下来写代码实现下，先是定义一个获取群聊ID的函数：

![](https://img-blog.csdn.net/20180609141138347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接着就是编写自动处理函数了，细节那里弄懂了，基本没什么问题：

![](https://img-blog.csdn.net/20180609141149449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180609141202526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




```
# 微信小宇宙助手
import datetime
import re
from threading import Timer
import itchat
from itchat.content import *
# 小宇宙今天新闻匹配正则
xyz_compile = re.compile(r'.*?小宇宙整理.*?%d月%d日.*'
                         % (datetime.datetime.now().month, datetime.datetime.now().day), re.S)
# 验证好友信息正则，关键词中有Python，Py和加群的关键字就可以了
add_friend_compile = re.compile(r'Python|Py|加群')
# 获取用户昵称的正则的
nickname_compile = re.compile(r'NickName\':\'(.*)\'', re.S)
# 小宇宙日报抓取
@itchat.msg_register([TEXT], isGroupChat=True)
def xyz_reply(msg):
    group_list = [u'我是渣渣辉', u'我是轱天乐', u'探挽懒月']
    group_name = []
    for group in group_list:
        chat = itchat.search_chatrooms(name=group)
        if len(chat) > 0:
            group_name.append(chat[0]['UserName'])
    # 过滤小宇宙新闻
    result = xyz_compile.search(msg['Content'])
    print(msg['ActualNickName'] + " : " + msg['Content'])
    if result is not None:
        if result.group() is not None and msg['ActualNickName'] == u'小宇宙':
            for group in group_name:
                itchat.send('%s' % (msg['Content']), toUserName=group)
# 每个半个小时发依次信息貌似能防止掉线
def loop_send():
    global count
    itchat.send('大扎好，我系轱天乐，我四渣嘎辉，探挽懒月，介四里没有挽过的船新版本，'
                '挤需体验三番钟，里造会干我一样，爱像借款游戏。'
                , toUserName=itchat.search_chatrooms(name=u'探挽懒月')[0]['UserName'])
    count += 1
    if count < 10000:
        Timer(1800, loop_send).start()
# 自动通过加好友
@itchat.msg_register(itchat.content.FRIENDS)
def deal_with_friend(msg):
    if add_friend_compile.search(msg['Content']) is not None:
        itchat.add_friend(**msg['Text'])  # 自动将新好友的消息录入，不需要重载通讯录
        itchat.send_msg('嘤嘤嘤，我是智障机器人小Pig，\n很高兴认识你，回复关键字:\n\n 加群，博客，公众号，打赏 \n\n 来继续我们的摔跤♂故事！',
                        msg['RecommendInfo']['UserName'])
        itchat.send_image('welcome.png', msg['RecommendInfo']['UserName'])
# 自动处理信息
# 1.加好友后发送加群信息
# 2.过滤加群信息
# 3.公众号推荐
# 4.打赏
@itchat.msg_register([TEXT])
def deal_with_msg(msg):
    text = msg['Content']
    if text == u'加群':
        itchat.add_member_into_chatroom(get_group_id("小猪的Python学习交流群"),
                                        [{'UserName': msg['FromUserName']}], useInvitation=True)
    elif text == u'博客':
        return 'coder-pig的个人主页-掘金：https://juejin.im/user/570afb741ea493005de84da3'
    elif text == u'公众号':
        itchat.send_image('gzh.jpg', msg['FromUserName'])
    elif text == u'打赏':
        itchat.send_image('ds.gif', msg['FromUserName'])
        itchat.send_msg('您的打赏，会让小猪更有动力肝出\n更Interesting的文章，谢谢支持～', msg['FromUserName'])
        itchat.send_image('wxpay.png', msg['FromUserName'])
    else:
        itchat.send_image('hrwh.png', msg['FromUserName'])
# 获得群聊id
def get_group_id(group_name):
    group_list = itchat.search_chatrooms(name=group_name)
    return group_list[0]['UserName']
if __name__ == '__main__':
    count = 0
    Timer(1800, loop_send).start()
    itchat.auto_login(enableCmdQR=2, hotReload=True)
    itchat.run()
作者：coder-pig
链接：https://juejin.im/post/5abf5d69518825558723af66
```







