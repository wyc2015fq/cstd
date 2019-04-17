# 首个谷歌TensorFlow安全风险被腾讯找到：攻击成本低、迷惑性强 - 知乎
# 



> 李根 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-9460c449914d27654dd47b63813d6268_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='740' height='413'></svg>)
首个TensorFlow安全风险被找到！


利用该风险，攻击者可以生成Tensorflow的恶意模型文件，对AI研究者进行攻击，对受害者自身的AI应用进行窃取或篡改、破坏。

目前，Google官方已经确认了该漏洞，并表示考虑到TensorFlow在AI的广泛使用，TensorFlow官网将会像大多数重要软件一样在网站创建安全公告页面，及时披露和修复安全漏洞。

Google官方，还对漏洞报告者给于了邮件致谢。

报告者，是来自中国的腾讯安全平台部的安全预研团队。

## **首个TensorFlow安全风险**

腾讯安全预研团队告诉量子位，此次TensorFlow漏洞发现过程并非偶然。

近期，在与部门AI同事碰撞思路后，确定尝试研究Tensorflow模型文件。其后在深入挖掘TensorFlow API文档及源码过程中，发现确认了漏洞。

腾讯团队认为，该风险危害面非常大，一方面攻击成本低，普通攻击者即可实施攻击；另一方面迷惑性强，大部分AI研究者可能毫无防备。

同时因为利用了TensorFlow自身的机制，其在PC端和移动端的最新版本均会受到影响。AI模型被窃取，损失的是开发者的心血；而一旦被篡改，造成AI失控，后果更难以想象。

不过，暂时未有风险造成的损害事件发生，不仅因为是零日漏洞，而且还在第一时间向Google进行了报告，漏洞应该会得到及时修补。
![](https://pic1.zhimg.com/v2-5e3b567ccf37bf1ec2f09e680ec0b5a0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='636'></svg>)
## **谨慎使用外部TensorFlow模型**

值得注意的是，虽然尚未造成攻击事件，但腾讯安全团队所发现的漏洞，不仅存在于最新TensorFlow版本中，TensorFlow老版本也存在该风险，而且存在时间久已，需要早已使用TensorFlow开发的开发者引起重视。

腾讯安全团队对量子位确认，所有TensorFlow版本都有这个问题。

由于AI框架被大范围使用，是近1-2年内的事，截至首个安全风险被报告，业内对此类安全问题关注度还不够。

但TensorFlow可能是目前使用范围最广的AI框架了，除了Google，Ebay、Airbnb、Twitter、Uber都在使用TensorFlow进行AI模型训练和开发。

在中国，小米、中兴、京东等公司也在利用TensorFlow的开源框架。

在TensorFlow中，AI研究者可以复用他人建好的模型来进行AI训练，或直接提供AI服务，大大提高开发效率。这些公开的模型通常以文件形式提供，也普遍被认为是无害的。

但此次漏洞被报告，算是给AI开发者“上了一课”。

未来，可能需要开发者在使用开源框架时“长点心”，比如谨慎使用外部提供的TensorFlow模型。

不比互联网和移动互联网时代，AI时代的风险攻击，可能还在造成线上线下一体化影响，目前利用AI开源框架，越来越多的IoT设备正在被接入网络。

腾讯安全预研团队称，2015年成立之时，聚焦重点之一就是聚焦AI、移动及IoT前沿领域的安全问题研究。

不过，TensorFlow的安全风险，也是首次发现，希望可以引起业内更广泛关注。

“漏洞被成功利用后，可导致AI模型被盗或被恶意篡改，将造成AI失控。若恶意模型文件在网络上被大范围使用，可能导致大量使用者被攻击。”腾讯安全团队强调。

## **依赖库也潜藏风险**

这次腾讯是直接在TensorFlow框架中找到安全风险。

[量子位在此前的报道中也提到](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247491683%26idx%3D1%26sn%3D06d437fe3230cde68e912bca9ac2b904%26chksm%3De8d05711dfa7de07e1745ceb2b5f826d377d83ffd4597acedf8f655c3087f288a768804c4f3b%26scene%3D21%23wechat_redirect)，TensorFlow、Caffe、Torch三个深度学习框架中包括很多第三方软件包，其中同样存在安全风险。
![](https://pic3.zhimg.com/v2-3fb8da3cdef784fb1ce62047da873922_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='220'></svg>)
一项初步研究显示：三个框架中发现了15个漏洞，类型包括：DoS拒绝服务攻击、躲避攻击等。

研究者表示：“深度学习框架是复杂的，重度依赖大量开源软件包。”这些依赖库，也正是漏洞的根源。
![](https://pic3.zhimg.com/v2-730cfbf8660f5860343a6a092fbe56a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='435'></svg>)
— 完 —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


