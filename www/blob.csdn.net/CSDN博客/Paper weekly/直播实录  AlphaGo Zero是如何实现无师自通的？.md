# 直播实录 | AlphaGo Zero是如何实现无师自通的？ - Paper weekly - CSDN博客





2017年10月31日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：224









本文为 10 月 29 日，圣何塞州立大学——刘遥行的论文共读直播分享实录。

Google DeepMind 团队在 Nature 上发表的最新论文，阐述了新版 AlphaGo 是如何从空白状态学起，不使用任何人类棋谱，通过自对弈强化学习，仅用
 3 天训练时间就以 100:0 击败了上一版本的 AlphaGo，并用 21 天达到 AlphaGo Master 水平。




**AlphaGo Zero 的核心在于强化学习下的自我博弈**。本期论文共读，我们邀请到 [深入浅出看懂AlphaGo
 Zero](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247486080&idx=1&sn=cfb5584e3ea0f225558fb1336298a7f5&chksm=96e9d500a19e5c16539132ab2699c968b589adf0635438d2b5897d604d3520623dedef5c22b9&scene=21#wechat_redirect)一文的作者，圣何塞州立大学研究生**刘遥行**，带大家揭秘 AlphaGo 的前世今生。




**>>>>**

** 获取完整PPT**




关注“PaperWeekly”微信公众号，回复“20171029”获取下载链接。




![?wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjogtmjdcqZVjeRlCRUTdJa4dAqd5cvS3TOCdsXqpO5HRwhG2tWjV1JcA/?wxfrom=5&wx_lazy=1)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjow0OaIahEUM5T4JjJLosl9Hrr6CGKySglZR4ErEH2UyxQB1x2OxHdpA/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjo2Bpqibzx36GQNkziap9wNdk1icGPjmibD7emOO5RjZ6686gQyce3I7ib8eA/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoHWZljkZJYchjYjq1wPjYRlDiceHghwL5NWXE0B69pgibFNV4g6CueWjg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoia4HQuFlsK5iaLtjQ6dA7JwuNSKDTSnIlpgK4YUqiaHQvk6ggSbVNibB0Q/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjo2joffIBYibsb4LAicRmtD6RMuOEkpeYONviagRo3YoQMGY8eAKN4BibQ2w/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoCm8QVVSshliaqf2VJrv4EQ3gCe7OER7FPw53or8XKq0w879gutrBE8Q/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoWGjv6WuNiaO37k9HvF6lhDKk7acF8bJ9AfNCNld8XFAdFvaplt2Sv2Q/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoYialZEDGSfv5I90kWByDTQTqpLibK8GYCI6QcEGwyRJDUfNrHVeTtic2Q/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoDRm3I7qu8XrLic9uNNKblqx9DMF7ic68GVH6YktSrVRFMuBOZQCg4AqQ/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjoPmibGPkVaHpl6fzK3FY8M2siaf0tR4gUialSlDd8Gw0eia3aVH2Q6C10wg/?)

![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglcxicmkWde3dDlhlvYYYJjozI9ShzZcdcSXtOMbDmXehfAEd6LLHP83ATSmFDDHicwSHXs5nicBOWSQ/?)







     直播实录     

** AlphaGo Zero是如何实现无师自通的？**



AlphaGo Zero




**论文解读√在线讨论√**




活动形式：语音直播



** 活动时间 **

10 月 29 日（周日）13:00-14:15

45 min 串讲 + 30 min 讨论




**长按识别二维码观看实录**

*报名成功后，即可进入直播间回看实录




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgnAjJtUdHJBsYKhqibiag6ibKrV3LTyAmTVCQHzs2vwgPzaxfte8p3uAVJeibfB5SrBcGWn4TcickLSxIQ/?)










**关于PaperWeekly**



PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入
 PaperWeekly 的交流群里。





![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

▽ 点击 | 阅读原文|
 回看实录




