# dB与信噪比（SNR） - xiahouzuoxin - CSDN博客





2013年09月02日 21:46:26[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：26625








**信噪比**（Signal-to-noise ratio，缩写为 SNR 或 S/N），也称作**信杂比**或**讯杂比**。




## 信噪比，为有用[信号](http://zh.wikipedia.org/wiki/%E4%BF%A1%E5%8F%B7)[功率](http://zh.wikipedia.org/wiki/%E5%8A%9F%E7%8E%87)(Power of Signal)与[噪声](http://zh.wikipedia.org/wiki/%E9%9B%9C%E8%A8%8A_(%E9%80%9A%E8%A8%8A%E5%AD%B8))[功率](http://zh.wikipedia.org/wiki/%E5%8A%9F%E7%8E%87)(Power of Noise)的比。因此为[幅度](http://zh.wikipedia.org/wiki/%E6%8C%AF%E5%B9%85)（Amplitude）比的平方：
![\mathrm{SNR} = {P_\mathrm{signal} \over P_\mathrm{noise}} = \left ( {A_\mathrm{signal} \over A_\mathrm{noise} } \right )^2](http://upload.wikimedia.org/math/8/7/7/87754bd799d9a5ce5caafaece54005f4.png)

它的单位一般使用[分贝](http://zh.wikipedia.org/wiki/%E5%88%86%E8%B2%9D)，其值为十倍对数信号与噪声功率比：
![\mathrm{SNR (dB)} = 10 \log_{10} \left ( {P_\mathrm{signal} \over P_\mathrm{noise}} \right ) = 20 \log_{10} \left ( {A_\mathrm{signal} \over A_\mathrm{noise}} \right )](http://upload.wikimedia.org/math/7/6/d/76d48b4ff75843979b49f8d71a341195.png)

其中
![P_\mathrm{signal} \,](http://upload.wikimedia.org/math/5/0/e/50edd7d30c66dcbe88f1e5939a5c9384.png) 为信号功率(Power of Signal)。![P_\mathrm{noise}  \,](http://upload.wikimedia.org/math/6/4/7/647150ce179c612505bf9ac64147c0d0.png) 为噪声功率(Power of Noise)。![A_\mathrm{signal} \,](http://upload.wikimedia.org/math/d/1/b/d1bd9e6f1ec1a370b30221e0dacba378.png) 为信号幅度(Amplitude of Signal)。
![A_\mathrm{noise}  \,](http://upload.wikimedia.org/math/4/3/2/432663e586f20363ef0ef53d027bab74.png) 为噪声幅度(Amplitude of Noise)。




还有一个单位**dBm**，可看做是用来衡量能量的一个绝对量，以1mW为基准。

![](http://www.forkosh.com/mathtex.cgi?formdata=%5CLarge+dBm%3D10%5Clog_%7B10%7D%28%5Cfrac%7BPout%7D%7B1mW%7D%29)







## 分贝标准的设定



0分贝的标准设定，是根据听力正常的人所能听到的最小声音而厘定的。每增加10分贝等于强度增为10倍，增加20分贝增为100倍，30分贝则增为1000倍。|**分贝(dB)的级别和一般噪音的危险接触时间**| |
|----|----|
|**分贝指数**|**声音例子**|
|0|人类可听见的最小声音|
|10|正常呼吸|
|20|在5英尺内窃窃私语|
|30|安静的图书馆、轻声细谈|
|40|客厅、安静的办公室、远离路况的卧室|
|50|近距离的轻微路况、冰箱、微风|
|**45-60**|**BabyPlus®胎教系统声频传达至婴儿**|
|60|距离20英尺的冷气机、谈天、缝纫机|
|70|繁忙交通、办公室制表器、吵闹的餐厅。在此分贝水平下，若您经常听到这些嘈杂的声音，或许会影响您的听觉。|
|**危险地带**| |
|80|地下铁、闹市繁忙交通、距离2英尺的闹钟、介于85至115分贝的汽艇、工厂吵杂声。若您常接触这些噪音超过8小时都会对身体不利。|
|90|铲车、介于95至120分贝的摩托车、99分贝的雪地摩托车、家用电器吵杂声、工具、割草机。当噪音声量不断提高，“安全性”的接触时间也会相继降低；损伤可在少于8小时内发生。|
|**95**|**子宫内血脉持续的声音水平**|
|100|锯链、介于105至120分贝的立体声耳机、风钻。即使只持续2个小时接触100分贝都会带来伤害，此外，**每增每5分贝，“安全时间”将会减少一半。**|
|110|娱乐中心可达至110分贝。|
|120|站在摇滚乐队演唱会喇叭前、健身俱乐部、有氧运动室、抛砂、高达118分贝的电影院、高达127分贝的体育活动、雷声。此危险性伤害会立即发生：接触120分贝可损伤您的耳朵。|
|130|儿童玩具所发出的声音高达135至150分贝之间、交响乐队、赛车、电钻、电动钻机。|
|140|介于150至167分贝的枪声、飞机起飞、140分贝或以上的“奏着音乐的车辆”、自行车警笛。接触的时间长短都会带来伤害；140分贝的声音可导致耳朵立即感觉疼痛。|
|150|喷气发动机起飞、介于125至155分贝的炮竹声，平均距离为10英尺、玩具手枪、气球爆裂。|
|160|烟火(距离3 英尺内)、步枪、手枪|
|170|猎枪|
|180|火箭发射台。若在没有护耳的情况下，此噪音水平可造成无法逆转的的损伤：丧失听力是无可避免了。|

**此资料于1983年，由美国华盛顿耳鼻喉与头颈部外科协会和重听联盟(League for the Hard of Hearing)于1996年至2003年提供。**







