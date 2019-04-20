# 人工智能产业发展联盟公布首轮AI芯片基准评测结果，评估工具已开源 - 人工智能学家 - CSDN博客
2019年03月09日 22:24:59[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：104
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewUicIZHNkeX6gFaZTTwICCkibM2C6tseeAERlAlxZibmEh9mjV9fgx5gUg/640?wx_fmt=jpeg)
来源：量子位
3月6日，由国际电信联盟、中国信息通信研究院联合主办，人工智能产业发展联盟（AIIA）和中兴通讯承办的国际论坛“AI in 5G——引领新时代论坛”在深圳召开。
来自全球电信运营商、标准组织、互联网企业、设备厂商、高校等业界各方的专家和嘉宾共聚一堂，探讨AI赋能5G网络智能化以及行业应用创新。
会上，AIIA正式发布“AIIA DNN benchmark V0.5”首轮评估结果，评估基于AIIA权威测试平台，在四大典型应用场景下，客观反映具有深度学习处理能力的处理器或加速器的性能水平。
参与首轮评估的产品分别是：**海思麒麟980**和**瑞芯微RK3399开发板**。
## **国内权威AI芯片基准测试**
AIIA副秘书长王爱华在论坛上介绍了评估的背景情况。
为了切实反映当前AI芯片的能力，聚合更多AI芯片企业参与，推动市场的健康发展，在国家发改委、科技部、工信部、网信办的指导下，2018年10月AIIA联合国内众多具有代表性的应用企业、主流芯片企业及芯片工具企业，正式发布《人工智能端侧芯片基准测试评估方案Version 0.5》，并于2018年12月启动首轮评估工作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewKGtp4Nlyon9NWOhCA42hXgV3oj69nnXd4dKA24ibELzliaxfd6YOjKuA/640?wx_fmt=png)
△ AIIA副秘书长王爱华介绍评估背景
## **首轮评估结果发布**
AIIA DNN benchmark V0.5测试评估主要面向端侧，旨在客观反映具有深度神经网络加速能力的处理器在完成推断任务时的性能。
AIIA总体组组长孙明俊向业界正式发布首轮评估结果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHew31FdLudNJgBJ2lyNqFjfRCdBP6LLWkpj80s8ebfytGIKCu8OFAGT4g/640?wx_fmt=png)
△ AIIA总体组组长孙明俊发布首轮评估结果
孙明俊表示，首轮评估包含四大典型场景（见图3）和两大类评测指标，指标包含速度（fps）和算法性能，如top1 、top5、mAP、mIoU、PSNR等。值得一提的是，这是深度学习处理器领域首次区分整型和浮点对比的Benchmark。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewlDQrGkXHK7DrrLy9nibF0LaZR72ccCEo7MYPib7a7dg6GT0DZxgPNtTQ/640?wx_fmt=png)
△ 评测指标及应用场景
AIIA DNN benchmark V0.5首轮测试对麒麟980和瑞芯微RK3399开发板两款产品进行了评估。
海思麒麟980——适用于手机终端Mate20系列，采用7nm工艺。具体评测结果如图4和图5所示。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewMHibPibky9qaQm75U5ViaoK9Y0OkM2q45Me7lGvrFxaglJxSlkkofL1tg/640?wx_fmt=png)
△ 场景1下海思麒麟980评测结果
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHew4YuUobgNWp16PQOINCL0guBiapUKrbdX4Hhw1FKaz47BpWPWGbAXzHg/640?wx_fmt=png)
△ 场景2/3/4下海思麒麟980评测结果
瑞芯微 RK3399开发板——该芯片在安防等领域有广泛应用，采用28nm工艺。结果显示,在浮点模型不需要定点化重新训练的情况下，int8计算以精度损失最大为1%的代价,达到了相对于浮点计算两倍的性能。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewVf7BlDzqXgmnq4wtAbKF7OicOY8YUaMTam4NCP7cqIga0Evnzhpj1wQ/640?wx_fmt=png)
△ 瑞芯微RK3399开发板评测结果
## **评估工具开源**
AIIA DNN benchmark V0.5测试评估工具采用开源方式，支持安卓系统，易于推广到所有 POSIX 兼容系统中，且用户可以根据项目文档，自行添加一个框架或者一个新模型。
面向未来的市场需求，AIIA DNN benchmark项目将持续更新。
孙明俊介绍，v0.5版本的Benchmark工具可以对基于CPU/GPU/DSP/NPU的终端推断任务进行评测，支持评测的机器学习框架包括TensorFlow/Caffe。
目前已经完成的移动端适配环境包括HiAI/SNPE/Tengine/TensorFlow Lite/MACE/NCNN等。
## **v1.0 版评估方案即将发布**
会上，西安交通大学任鹏举教授对AIIA DNN benchmark V0.5首轮评测的结果进行了解读，并向业界公布了评估方案的后续推进计划，AIIA DNN benchmark项目v1.0 正式版预计将于今年上半年正式发布。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewFnS9aDU1G3d07TW8xLDJ9mHAFLM65HpjeAQoWeZfnhFP3rzDMOvMiag/640?wx_fmt=png)
△ 西安交通大学任鹏举教授解读v0.5版本评估结果
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHew52z9vU5o5ttXibtnPy2Iib7gIrfCibiaGRMSVRF6su2H555dpc4QrjMoeg/640?wx_fmt=png)
△ v0.5版本评测结果分析
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewRaj7CXYBUhPDtuMsyfY8nH8LicOudoZjC3iaAclshWyQUtbNTBFV4sWg/640?wx_fmt=png)
△ v0.5版本评测结果分析
最后，ARM中国AI技术市场经理分享了题为《ARM中国周易开放AIoT应用平台》的演讲。
他表示，ARM人工智能周易平台已顺利对接AIIA DNN Benchmark项目，旨在助力芯片公司为ARM芯片取得客观公正的AI性能评估报告，同时也方便应用算法公司的芯片选型。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWkhbiah19W3GrgAO9YAyHewyV9lmTkSMyhUT5hicEumZajQQKYCq4ANJhd8Dre8qOYD2BB9hoiaYwYg/640?wx_fmt=png)
△ARM中国AI技术市场经理发表演讲
未来，AIIA将顺应行业发展需要，持续迭代评估规范。
AIIA DNN benchmark项目v1.0 正式版的评测对象将进一步丰富，覆盖语音、自动驾驶、安防等应用场景下的AI芯片。
可以预见，基于开放、共赢的产业生态，AIIA DNN benchmark项目将吸引更多企业与开发者加入。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
