# 开车请别玩手机、别弹吉他，MIT这套系统在盯着你！ - 知乎
# 



> 晓查 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

无人驾驶汽车的美好愿望是减少车祸的发生。本来减少司机的负担是很好初衷，但司机过度依赖AI产生的懈怠更可怕，甚至会引发严重的车祸。

比如2017年，一名特斯拉Model S车主在开启Autopilot后居然在车内看起了电影，最后装上大卡车导致车毁人亡。

今年Uber无人驾驶车上路时，由于安全员在玩手机，没有看路况，导致撞死一名骑自行车的路人。



![](https://pic2.zhimg.com/v2-957c73e81010f08b90000735cba2f96d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='732'></svg>)



**△ **Uber无人车今年曾撞死一名行人，图为事发车辆

这些车祸发生的原因，都在于车辆没有监控司机的状态，及时提醒司机。


最近，MIT的AI科学家Flex Fridman研发了一套**“以人为本”的自动驾驶系统**，可以实时、高分辨率地观察驾驶员在做什么，形成汽车和人的互动。在“真”无人驾驶到来前，它有望解决人与AI共同管理汽车的问题。

这套系统到底有多精确？请看5个小细节设计。

它能识别到司机手持吉他，甚至是否处在弹奏状态：



![](https://pic4.zhimg.com/v2-74dd1abe690871ce8880021d9b1a5eab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='603'></svg>)



从一个微小动作中，识别司机是否在看路：




![](https://pic4.zhimg.com/v2-c44fbc0a618595469e9e57ad177ecb2f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='918' height='522'></svg>)



正确识别出司机在驾驶过程中看手机：




![](https://pic4.zhimg.com/v2-2bc47d732e02a4bb79bf0cf55350aa17_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='840' height='474'></svg>)



它不仅仅是识别，还能与司机进行互动，如果司机不在看路，会提出风险升高：




![](https://pic3.zhimg.com/v2-567bd0bd67dd71924a952ba38805301a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='607'></svg>)



路边出现行人的时候，也会给司机提示风险：




![](https://pic2.zhimg.com/v2-23a4e4675fca1169c64aca907dea5a45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='605'></svg>)



总之，有了这套系统，司机即使在做别的事情，也能及时收到系统的提醒。在自动驾驶尚未能完全取代人类时，这不失为一种更安全的方案。


## **设计原则**

为了开发这套系统，Lex Fridman提出了设计中的“七大原则”，并且发布了学术论文，供其他自动驾驶汽车设计人员参考：



![](https://pic4.zhimg.com/v2-3a9079272017c745fafbf4bf2d7f27db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='450'></svg>)



文章地址：[https://arxiv.org/abs/1810.01835](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.01835)


- **人车共治**：人也是AI驾驶中的一环
- **从数据中学习**：数据驱动车辆技术
- **人体感应**：理解驾驶员的各种行为
- **共享感知控制**：给司机再添一双眼和手
- **深度个性化**：与驾驶员分享经验
- **不完美设计**：瑕疵也是特征
- **系统级体验**：在系统级别优化安全性和体验

## **初衷&愿景**

在上面的论文中，Lex Fridman谈到了为何要设计这套无人驾驶系统。
目前的自动驾驶系统仍然以AI识别路况为思路，缺乏对司机状态的监控，他希望这套系统能用在从手动驾驶到自动驾驶过渡的过程中，提供一种**半自动驾驶**方案。

另外，自动驾驶系统也能通过对司机驾车行为的监控，使用**模仿学习**向人类学习开车的经验。




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


