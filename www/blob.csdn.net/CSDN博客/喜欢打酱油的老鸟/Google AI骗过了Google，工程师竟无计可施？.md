
# Google AI骗过了Google，工程师竟无计可施？ - 喜欢打酱油的老鸟 - CSDN博客


2019年01月14日 08:18:23[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：38标签：[Google 																](https://so.csdn.net/so/search/s.do?q=Google &t=blog)[AI																](https://so.csdn.net/so/search/s.do?q=AI&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Google &t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)



# Google AI骗过了Google，工程师竟无计可施？
[https://www.toutiao.com/a6643667022901166595/](https://www.toutiao.com/a6643667022901166595/)
2019-01-07 16:21:09
作者 | 若名
出品 | AI科技大本营
如果你通过 Google 搜索购买演唱会门票或者注册论坛账号，系统会提示你必须点击几个图框、音频或者移动鼠标等操作来确认是人类在操作验证而不是机器人。
![Google AI骗过了Google，工程师竟无计可施？](http://p1.pstatp.com/large/pgc-image/eafb8d1db47f4d3f97bc9e14b1f2f3b5)
其背后的验证机制就是 CAPTCHA（验证码），保护互联网服务免受诸如 Sybil 的攻击，这是互联网防止自动创建帐户和滥用服务的第一道防线。而 Google 的 reCaptcha 服务是最受欢迎的验证码系统之一，目前被成千上万的网站用于测试用户是否是人类，以防范机器人的攻击。
reCAPTCHA 发布之后，无论是安全专家还是研究人员都在试图挑战它，找出它的漏洞，比如一些专家就试图通过反向图像搜索、深度学习和“ 实验神经科学数据 ” 来进行攻击。这些方法要么成功率较低或者很快就被 Google 团队迅速补上了漏洞。但其中一种攻击方法让 Google 团队看上去束手无策。
![Google AI骗过了Google，工程师竟无计可施？](http://p1.pstatp.com/large/pgc-image/ff8a501b03874a90a7a6c8a6e1593519)
马里兰大学的研究人员声提出的 unCaptcha 攻击方法 unCaptcha 可以轻松骗过 Google reCaptcha。他们使用来自实时网站的超过 450 次 reCaptcha 挑战来评估 unCaptcha，并证明可以在 5.42 秒内以 85.15％ 的准确率进行破解。
该方法的原理让人万万没想到的是：**在用户请求语音验证码后，只要把收到的内容转发给 Google 语音转文字 API，然后把收到的回复进行提交，最后竟然给轻易破解了。**
古人问：以子之矛，攻子之盾，何如？
Google reCaptcha 团队：其人弗能应也......内心苦，说不出。
微博网友如此调侃：
![Google AI骗过了Google，工程师竟无计可施？](http://p3.pstatp.com/large/pgc-image/9356436bff6f44ea875975199fb4baa1)

![Google AI骗过了Google，工程师竟无计可施？](http://p1.pstatp.com/large/pgc-image/ed7ebe060d074a4ebba56f64f70f87ee)

![Google AI骗过了Google，工程师竟无计可施？](http://p3.pstatp.com/large/pgc-image/a54a8a1975c84485adb5433b4ca5efbc)
怎么会这样呢？
上述研究人员最初在 2017 年开发了 UnCaptcha，他们在发表的论文《unCaptcha: A Low-resource Defeat of reCaptcha's Audio Challenge》中给出了更为详细的解释：**通过 UnCaptcha 下载音频验证码，将音频分成单个数字音频片段，将片段上传到多个其他语音转文本服务（包括 Google），然后将这些服务得出的结果转换为数字编码。随后经过一些同音词猜测后，它会决定哪个语音到文本输出最接近准确值，然后将答案上传到 CAPTCHA 字段。**
![Google AI骗过了Google，工程师竟无计可施？](http://p1.pstatp.com/large/pgc-image/ea4cd48087db45d5984221348d9a78d8)
这还是 unCaptcha 1.0 版本，成功率便达到 85％。在这一版发布后，Google 随即修复了一些漏洞，包括提升浏览器自动检测性能以及将验证方式从数字编码切换为短语音，这成功防止了 unCaptcha 的攻击。
不过，上述研究人员在 2018 年 6 月升级了新方法，unCaptcha v2 依然可以绕过这些防御机制，并且“魔高一丈”，准确性比以前更高，达到了 90％。
这次他们还公布了 v2 版的代码和更详尽的步骤，研究人员称 unCaptcha2 的操作方法很简单：
导航到 Google 的 ReCaptcha 演示网站
导航到 ReCaptcha 的音频挑战
下载音频挑战
将音频挑战提交到 Speech To Text
解析响应和类型答案
按提交并检查是否成功
![Google AI骗过了Google，工程师竟无计可施？](http://p3.pstatp.com/large/pgc-image/08c31999ca9f4862bdfaf35b05a04437)
（过程演示）
由于 unCaptcha2 必须转到屏幕上的特定坐标，因此你需要根据设置更新坐标。这些坐标位于 run.py 的顶部。在 Linux 上，使用命令 xdotool getmouselocation --shell 查找鼠标的坐标可能会有所帮助。
你还需要为选择的任何语音转文本的 API 设置证书。由于 Google，微软和 IBM 的语音转文本系统看起来效果最好，因此这些系统已包含在 queryAPI.py 中。此外，你必须根据需要设置用户名和密码，对于 Google 的 API，必须使用包含 Google 应用程序证书的文件设置环境变量（GOOGLE_APPLICATION_CREDENTIALS）。
最后，使用安装依赖项 pip install -r dependencies.txt，大功告成。
到此你还有疑惑问这难道不是黑客攻击吗？Google 没报警？研究人员的说法是，他们已经联系了 Google ReCaptcha 团队，提醒 Recaptcha 更新后的系统安全性更差了，所以后者完全了解这次攻击的事件，而研究人员非但没有受到任何干扰，而且在破解成功率如此之高的情况下，ReCaptcha 团队甚至允许他们公布代码。
当然，这些研究人员这么努力找 ReCaptcha 的 bug，并善意提醒ReCaptcha 团队基础架构的问题，却没能得到 Google 的奖励，研究人员也特意强调了这一点。
ReCaptcha 团队暂时未给出应对攻击的进一步措施，但研究人员最后发布免责声明称，当 Google 再度更新 ReCaptcha 服务时，他们将不再更新 GitHub 库。因此，他们预计 unCaptcha2 攻击方法未来会失效，项目本身也会随时中断。
> 来源：http://uncaptcha.cs.umd.edu/论文链接：http://uncaptcha.cs.umd.edu/papers/uncaptcha_woot17.pdf代码和数据：https://github.com/ecthros/uncaptchahttps://github.com/ecthros/uncaptcha2

