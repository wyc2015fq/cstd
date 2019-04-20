# 腾讯AI Lab刷新人脸检测与识别两大测评国际记录，技术日调用超六亿 - 人工智能学家 - CSDN博客
2017年12月21日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：348
*![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXQNlENdvR5XeuJ4CPYjXibFUSyibGSFfG4eZPFDrhoBAlDIAMXYefHPkWgH9BEzPiaIZXhPsWOtVhQw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)*
*来源：腾讯AI实验室*
*概要：人脸检测是让机器找到图像视频中所有人脸并精准定位其位置信息，人脸识别是基于人脸图像自动辨识其身份，两者密切相关，前者是后者的前提和基础。*
腾讯AI Lab在国际最大、最难的人脸检测平台WIDER FACE与最热门权威的人脸识别平台MegaFace的多项评测指标中荣膺榜首，刷新行业纪录，展现其在计算机视觉领域中，特别是人脸技术上的强劲实力。
研究上，目前腾讯AI Lab已通过arXiv平台发表论文公开部分技术细节，促进企业与学界“共享AI”研究成果；应用上，该技术已接入腾讯互联网+公共服务平台等多个应用场景，每日技术调用量超六亿次，未来有望更广泛应用到政务、金融、安防等多个领域，营造安全网络环境，方便百姓远程办事。
人脸检测是让机器找到图像视频中所有人脸并精准定位其位置信息，人脸识别是基于人脸图像自动辨识其身份，两者密切相关，前者是后者的前提和基础。在研究上，由于视角、光照、遮挡、姿态、年龄变化等复杂因素的干扰与影响，真实场景下的人脸检测与识别问题一直是一个极具研究价值与挑战性的国际性难题。而在应用上，其在政务、金融、安防等领域都具有极高价值。
在人脸技术发展过程中，标准评测数据集的重要性不言而喻。参与标准数据集测试时，研究人员可在固定标准下，评估算法性能，并以此为方向推动技术不断发展。
**人 脸 检 测 算 法 Face R-FCN**
**在WIDER FACE全部测试中斩获第一**
在人脸检测领域，WIDER FACE是目前国际最大、最具挑战性的人脸检测评测平台，由香港中文大学发布维护，共有3.2万张图像，39万个标注的人脸，这些人脸在尺寸、姿态、角度和遮挡等有很大变化，吸引了中科院、美国卡耐基梅隆大学和马里兰大学等知名机构参与测评。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWykbW15uovMBHrEOJk6sZT4JwFkhW29WQ1QDYMmx0LSoQDztJMyeiazA/640?wx_fmt=jpeg)
*WIDER FACE人脸图像示例 - 绿框为腾讯AI Lab检测结果，红框为官方标注结果。*
腾讯AI Lab针对人脸在尺度、光线、角度和遮挡上的多维变化，有效改进深度全卷积神经网络，提升人脸检测精度和鲁棒性，推出人脸检测算法Face R-FCN。该算法在WIDER FACE测试平台中使用官方指定训练集，即在完全公平竞争情况下评测不同参赛算法的性能时，在简单、中等及困难模式（Easy、Medium、Hard）的全部三个测试子集中均取得世界第一。目前算法Face R-FCN的部分技术细节已通过论文在arXiv平台上发布。
论文地址：https://arxiv.org/abs/1709.05256
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWLerQdY7C11rsHv0Tf32H0NTt5CLx3LtxpeJxcQAtUjppnBPM20m8BQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWb8yamkEtZnWvGhRdibxT0AYhT1jetaXHoeF1jlKGoSEa9eEagUlf2fw/640?wx_fmt=png)
*WIDER FACE评测结果曲线，第一行是验证集结果，第二行是测试集结果。详情见如下链接：*
*http://mmlab.ie.cuhk.edu.hk/projects/WIDERFace/WiderFace_Results.html*
**人 脸 识 别 算 法 Face CNN**
**在MegaFace Challenge 2所有测试斩获第一**
在人脸识别领域，MegaFace是目前最权威热门的人脸识别评测平台之一，由美国华盛顿大学（University of Washington）发布维护，在百万规模人脸数据下，评定1:N辨识（Face Identification）和1:1验证（Face Verification）两大指标准确率。前者是在N个人数据库中找到1个目标人脸，后者是判断给定的两张人脸是否属于同一身份。基于评测数据规模和评测指标上的优势，MegaFace吸引了Google、俄罗斯著名安防公司Vocord、日本NEC和美国卡耐基梅隆大学等知名机构参与。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWzaJlQvzliaQXoS4GiaLJFcXR8e5c7CggyDq8v3zgVZNqib0z6tGyPlbTw/640?wx_fmt=png)
* MegaFace常规人脸数据样例*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWO8XdMnHj3bUPFpxKNAnYPSdic4LiclGk2I0sny2zgOatnsA3TaKGKRVg/640?wx_fmt=jpeg)
* MegaFace跨年龄人脸数据样例，这是同一人不同年龄时照片*
腾讯AI Lab针对常规人脸识别和跨年龄人脸识别，在网络模型结构、特征学习等方面创新性改进，推出了人脸识别算法Face CNN。该算法在MegaFace测试平台中使用官方指定训练数据，即在完全公平竞争情况下评测不同参赛算法的性能时，在MegaFace Challenge 2（简称为MF2）的所有测试任务均取得世界第一。Face CNN算法的相关技术解读也将在未来陆续公布。
MegaFace含两个Challenge，Challenge 1（MF1）可使用任何外部不限量的人脸数据来训练参赛算法，如谷歌使用亿级数据，其他团队采用百万级数据，较难公平比较算法性能。而Mega Face推出的新版Challenge 2（MF2）要求使用官方固定训练集，能更客观对比各算法的情况。MF2分为常规识别和跨年龄识别两个子任务，分别使用FaceScrub和FGNET测试集，将1:N辨识和1:1验证的准确率视为两项关键评测指标。MF2详细评测结果参阅如下：
http://megaface.cs.washington.edu/results/facescrub_challenge2.html
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWdZBWVibYh4JI3E6tibzXpichEMhlcWNgNcog5yiaELtjpN6ibeYRAUeRVhQ/640?wx_fmt=png)
*MF2常规识别任务的辨识准确率（1:N）排名*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWHEqLlHqE2GsyoJUllykic6l133XRxbadOAY63dfoNXtk5CxAjhRq6TA/640?wx_fmt=png)
*MF2的常规识别任务的验证准确率（1:1）排名*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjWicW13Msia4mJLTaCibibuQODy5dGz5zde3C18FfZZSZplGmfwxcqNwUd3A/640?wx_fmt=png)
*MF2的跨年龄识别任务的辨识准确率（1:N）排名*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/LWHwEYpIANLwcGX7kQFeRIpUjNicoBEjW4OXB1Cxm2rFSXXveHlqxxmibyFxrUS2HfAbjsibK66RyT8eNpnBNOpibA/640?wx_fmt=png)
*MF2的跨年龄任务的验证准确率（1:1）排名*
**研 究 到 应 用 迅 速 落 地**
**走「共享AI」之路**
遵循与学界和行业“共享AI”成果的发展之路，腾讯AI Lab不断推进研究到应用迅速落地，缩短技术迭代时间，小步快跑构建腾讯的AI核心竞争力。目前自研的人脸技术已接入腾讯若干业务场景，每日技术调用量已超过6亿次。一个典型的应用场景是在互联网+公众服务领域，通过人脸验证完成身份自动鉴别的“刷脸办事”场景，方便百姓远程办事，让数据多跑路，百姓少跑腿，通过技术提升了公共服务的品质与效率。
此外，在CVPR、ACL、ICML、NIPS和Nature子刊等衡量AI研究能力的顶级会议和期刊中，腾讯AI Lab今年被收录论文110多篇，位居国内企业前列。在应用上，除了技术已经落到微信、QQ、音乐和视频平台，还围绕游戏、内容和社交等核心业务突破，推出了围棋AI“绝艺”并广泛支持AI+医疗产品“腾讯觅影”。
**关于腾讯AI Lab**
腾讯AI Lab于2016年4月成立，是腾讯的企业级人工智能实验室，专注于AI基础研究与落地应用的结合，借助腾讯丰富应用场景、海量大数据、强大计算能力和一流科技人才等发展优势，为腾讯打造全面的AI能力，向“Make AI Everywhere”（让AI无处不在）的愿景迈步。
腾讯AI Lab主任及第一负责人是机器学习和大数据专家张潼博士，副主任及西雅图AI Lab负责人是语音识别及深度学习专家俞栋博士。目前深圳和西雅图两个实验室共有70余位国际一流的AI科学家及300多位经验丰富的应用工程师。
腾讯AI Lab的基础研究专注机器学习、计算机视觉、语音识别和自然语言处理四大方向，技术应用聚焦于内容、游戏、社交和平台工具型AI四个方向，目前已落地到微信、QQ、天天快报和QQ音乐等上百个腾讯产品。在行业落地上，围棋AI“绝艺”及AI+医疗项目“腾讯觅影”等项目取得了突破性进展。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)
