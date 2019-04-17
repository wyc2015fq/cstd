# 我不是机器人：谷歌最新版验证码系统ReCaptcha破解已开源 - 知乎
# 



选自 Github，作者：George Hughey，机器之心编译。

> 每个人都讨厌验证码，这些恼人的图片中包含你必须输入的字符，我们只有正确地填写才能继续访问网站。验证码旨在确认访问者是人还是程序，并防止恶意程序的入侵。然而，随着深度学习和计算机视觉技术的发展，现在这些认证方法可以被我们轻松破解了。

在这一个项目中，作者破解了最新版的 ReCaptcha，并开源了所有代码。不过项目作者声明：「这份代码能够解决最新版的 ReCaptcha，但仅限于在自己网站上使用，且仅用于教育目的。」

项目地址：[https://github.com/ecthros/uncaptcha2](https://link.zhihu.com/?target=https%3A//github.com/ecthros/uncaptcha2)

reCAPTCHA 项目是由卡内基梅隆大学开始构建的系统，这个项目将从书本上扫描下来、无法被 OCR 准确识别的文字显示在 CAPTCHA 问题中，从而判断访问者到底是程序还是人类。自 2014 年以来，reCAPTCHA 改为在「我不是机器人」（I'm not a robot）的方框中打勾，进而完成判别。

这个名为 unCaptcha 的项目最早创建于 2017 年 4 月，并在当时实现了 85% 的 ReCaptcha 对抗率。不过后来谷歌发布了新版本的 ReCaptcha，且新版的 ReCaptcha 主要有以下两个主要改进：
- 更好的浏览器自动检测
- 使用短语语音而不是数字

这些改进最开始成功地防御了第一版 unCaptcha 的攻击，然而到了 18 年 6 月份，这些挑战基本都已被解决。本项目的作者同样与 ReCaptcha 团队取得了联系，他们完全了解这次攻击。此外，ReCaptcha 团队还允许该项目作者发布 unCaptcha2 的代码，尽管它目前仍然能有效破解 ReCaptcha 验证码。

**unCaptcha2 简介**

由于 ReCaptcha 添加了语音形式的验证码识别，破解 ReCaptcha 比以前变得更加容易。因为我们只需要调用一个免费的语音识别 API，对所有验证码的识别准确率就能达到 90% 左右。另外由于 ReCaptcha 会阻止浏览器自动化引擎 Selenium，unCaptcha2 会使用屏幕点击器（clicker）移动到屏幕上特定的像素，从而像人一样浏览网页。当然在模拟人类浏览的过程中，每一个新用户的坐标都需要更新，这个目前并不是太鲁棒。

unCaptcha2 的方法非常简洁：
- 导航至谷歌的 ReCaptcha 演示网站
- 导航至 ReCaptcha 的语音库
- 下载语音
- 使用语音识别 API 识别语音
- 解析 API 返回的参数和识别结果
- 提交识别结果，判断是否成功

如下所示 unCaptcha2 的测试界面：
![](https://pic4.zhimg.com/v2-31576e668ea1c17b5fed69bc1abd56ff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='788' height='445'></svg>)



**如何使用 unCaptcha2 **

因为 unCaptcha2 必须指定屏幕上的特定坐标，因此我们必须基于设置更新坐标。这些坐标更新的代码写在了 run.py 文件的最上面。在 Linux 系统中，使用命令行 xdotool getmouselocation—shell 可以定位鼠标在屏幕上的坐标位置。

我们还需要配置到底使用哪一个语音识别 API，国内的 BAT、讯飞和搜狗等都提供高效的语音识别 API，而国外的谷歌、微软和 IBM 也一样。谷歌、微软和 IBM 的的 API 结构都已经在 queryAPI.py 中，我们只需要配置用户名和密码就行了。如果你要使用谷歌 API，我们需要使用谷歌应用的证书参数配置环境变量 GOOGLE_APPLICATION_CREDENTIAL。

最后，安装剩下的依赖项就能愉快地破解了：pip install -r dependencies.txt。


