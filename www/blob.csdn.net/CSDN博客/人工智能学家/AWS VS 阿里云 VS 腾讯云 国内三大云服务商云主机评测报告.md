# AWS VS 阿里云 VS 腾讯云 国内三大云服务商云主机评测报告 - 人工智能学家 - CSDN博客
2018年01月15日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：10483
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTlYpYVzthichZ5icicIVOZo1VFmQRBzY2hUy6yIyfLEQqib4BubCpxVnsicGA/0?wx_fmt=jpeg)
*来源：CloudBest*
*概要：近几年，随着国内公有云市场规模的不断增长，以阿里云、腾讯云为代表的本土云服务商，以及以AWS、微软Azure为代表的海外云服务商，成为公有云市场最大的赢家，市场寡头化趋势已经凸显。*
近几年，随着国内公有云市场规模的不断增长，以阿里云、腾讯云为代表的本土云服务商，以及以AWS、微软Azure为代表的海外云服务商，成为公有云市场最大的赢家，市场寡头化趋势已经凸显。
日前，AWS中国（以下简称“AWS”）宣布继北京区之后，宁夏区也正式投入使用。那么，作为国内公有云市场主流服务商的AWS、阿里云、腾讯云，云主机综合性能如何？作为新起之秀，宁夏区网络可用性如何？对此，国内专业的云资源选型服务平台CloudBest旗下监测实验室，针对亚马逊AWS、阿里云、腾讯云三家云主机产品，进行性能测试与性价比评测。以下为CloudBest监测实验室第一期评测报告：
**一、 测评说明**
本次评测均采用非独享云主机默认配置，选取按量付费按小时计费原则，此处应说明的是阿里云、腾讯云云主机单价含带宽（1M）及存储费用，AWS不含带宽和存储费用。同时考虑到各家磁盘不同，性能测试主要涉及CPU指标，不包含磁盘部分。
**二、 测评方法**
1） 云主机性能共进行3次测试，每次测试时间半小时左右，其中所有的云主机均为新购主机，每完成一次测试空跑8小时以上再进行下一次测试，最终取3次测试的平均值。
2） 网络测试为选择全国范围内多个不同ISP的网络监测节点，同时持续对目标主机进行网络监测。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTlicgAgXupWByULrNQ1ciaVuC6aYhJJyr9OdoFuBEHMuWJtSsGbQUbjdcw/0?wx_fmt=png)
*图1：云主机评测明细*
**说明：**
由于三家云服务商的区域较多，本次测试主要针对阿里云华北2（阿里云华北2地处北京，以下简称“阿里云”），腾讯云北京区（以下简称“腾讯云”），AWS北京、AWS宁夏等区域的云主机产品。由于我们对阿里云、腾讯云、AWS北京、AWS宁夏4个评测对象每个评测对象选取了1核1G、2核4G、4核16G 3种配置的云主机，所以文中涉及到的整体数据，均为3种配置云主机的平均值。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTlEjnpXiaMXRVpJ0O2RnhiaGwrTJuvickSReJeJsXVjicRLVv3Ctjib5PWSDA/0?wx_fmt=png)
*图2：云主机配置及价格概况*
**说明：**
由于不同云服务商的虚拟化方式等的不同，在提供的云主机中，很难找到相同配置的评测对象，故在型号选取上，我们采取大方向上配置一致，细节上选用通用配置。
**三、 评测结果**
在性价比排行上，AWS宁夏1核1G、2核4G，AWS北京1核1G占据此次评测的前三位，性价比=性能得分/单价，得出数据越大，性价比越高。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTl1rJAXic1yl66jxBMw8r9Bc1BPKvzaX44Fm3kIvibomRH1RRd8NnARjGA/0?wx_fmt=png)
*图3：云主机性价比排行（由高到低排列）*
网络时延、丢包率是网络评测中通常会涉及到的指标，本次评测中，我们又将网络时延分为区域时延、时段时延两个维度，丢包率也分为区域丢包率、时段丢包率两个维度。从而根据区域时延、时段时延得出平均时延，根据区域丢包率、时段丢包率得出平均丢包率。同等情况下，平均时延越短，某地访问对应云主机速度相对较快，网络质量越好；同等情况下，平均丢包率越小，某地访问对应云主机的网络连通性越好。以游戏玩家为例，同等情况下，其所在的网络质量越好，网络连通性越好，抢装备的速度越快。
在网络平均时延和平均丢包率评测指标上，AWS宁夏不具备明显优势，AWS北京虽然较AWS宁夏表现稍好，但与阿里云、腾讯云相比稍落后。
腾讯云的总平均时延、总平均丢包率上小胜阿里云；在区域平均网络时延上，腾讯云45ms（中间值）以下覆盖范围较广，平均时段时延上也表现较好；但在区域平均丢包率上，阿里云覆盖范围较广；时段丢包率上，腾讯云、阿里云不同时段各有优劣。
**四、 评测详情**
**1. 整体性能得分**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTlcYCMG1lEmUelpaYgAicppfHNDCZzjRn6ZHImXBVQtGWFejrGHLMtHNQ/0?wx_fmt=png)
*图6：三次测试整体得分数据*
由上图，在1核1G云主机配置下，AWS宁夏云主机得分最高为2035.4，腾讯云分值最低为1614.2；在2核4G云主机配置下，AWS宁夏得分最高为1991.2，阿里云分值最低为1501.5；在4核16G云主机配置下，腾讯云得分最高为4155.0，AWS北京分值最低为3391.7。
AWS宁夏云主机在1核1G、2核4G下得分均最高，AWS北京云主机紧随其后；但在4核16G云主机配置下，腾讯云、阿里云具有明显优势。
**2. 价格**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTlFzH4xHn1KhXbNAgCubqO1VGicbcwEeecHttRFFX5UoicMTKicuyxzjIoQ/0?wx_fmt=png)
*图8：云主机单价对比*
由上图我们可以看出，AWS宁夏在1核1G、2核4G、4核16G配置的云主机价格中均最低，这也与其突出的性价比表现相呼应；AWS北京在1核1G、2核4G配置的云主机价格低于阿里云、腾讯云，但其4核16G价格达到最高，这也对应其在性价比中的表现不太理想；阿里云、腾讯云在这3类云主机价格中各有优势，1核1G云主机阿里云低于腾讯云；2核4G、4核16G云主机，腾讯云低于阿里云。
**3. 区域平均时延**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/7y4F1MEjEIdzraoFHpaFbTaicOsPrzvCuj2BggEj3IqTPcaU7pLdKFUrGZgCYURAia4nZC1v6uH6vojouL2qEu3g/640?wx_fmt=jpeg)
*图9：区域平均时延对比图*
如图9所示，将全国34个省市访问阿里云、腾讯云、AWS北京、AWS宁夏云主机的平均时延划分6个层次为<15、15-30、30-45、45-60、60-75、≥75，单位为毫秒（ms），按照时延长短，颜色由深绿转深红。深绿色代表访问时延最短；深红色代表访问时延最长。由此，AWS宁夏时延45ms以上区域最广，处于相对劣势；腾讯云45ms以下覆盖范围最广。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/7y4F1MEjEIdzraoFHpaFbTaicOsPrzvCu5F7TFc2ckCsFAm0q6TD1hT1pSQ6LPicJm2tgKLIbXxOz3McteyTgpuQ/640?wx_fmt=jpeg)
*图10：区域平均时延柱状图对比*
全国34个省市访问阿里云、腾讯云、AWS北京、AWS宁夏等云主机平均时延对比，其中每个颜色代表一家评测对象，各家在某一区域的柱状图越高代表访问时延越长。以新疆为例，在阿里云、腾讯云、AWS北京、AWS宁夏中，AWS宁夏在新疆柱状图最高，表示该地区访问AWS宁夏云主机时延最长，AWS北京在新疆柱状图最低，表示该区域访问AWS北京云主机时延最短。
**4. 时段平均时延**
在时段时延上，CloudBest选取全天0:00-23:00中的 24个整点，每个整点随机对应一个省市，取四个测评对象3例云主机在相同整点相同城市的平均时段时延，然后再对比不同整点下四个测评对象的时延表现。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTleeZTFicDAshpYLOY86Ngyr6saS2M8Tic8cVYicpO0E2Ra9FUHXb9O1NFg/0?wx_fmt=jpeg)
*图11：时段平均时延评测*
如上图所示，四个评测对象由0:00-23:00各个整点的平均时延数据呈现一致的波动曲线，例如在凌晨2:00-4:00时延最低；晚上20:00-21:00时延最高。其中，腾讯云在24个整点中，平均时延均最低，AWS宁夏在24个整点中，平均时延均最高。
**5. 区域平均丢包率**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/7y4F1MEjEIdzraoFHpaFbTaicOsPrzvCuKXM6ia4SO4iajL3uMmCEybU7rasvOrKbicEWBk89x8OoOMAOVqROAIrwQ/640?wx_fmt=jpeg)
*图12：区域平均丢包率对比*
如上图所示，将全国34个省市访问阿里云北京、腾讯云北京、AWS北京、AWS宁夏云主机的平均丢包率划分为6个层次，136（34*4）个数据分成6等份为<0.6%、0.6%-0.91%、0.91%-1.38%、1.38%-2.48%、2.48%-6.77%、≥6.77%，按照丢包率大小依次排列，颜色深绿转深红表示丢包率由小到大。深绿色代表丢包率最小；深红色代表丢包率最大。由此，AWS宁夏平均丢包率1.38%以上范围较大，处于弱势；阿里云1.38%以下范围较大，处于优势。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/7y4F1MEjEIdzraoFHpaFbTaicOsPrzvCuFWibjtTibACSokU4FXNJ6uI9ibJQEZdxpCrkcD773LJMsGOibKIL536ntw/640?wx_fmt=jpeg)
*图13：区域平均丢包率柱状图对比*
由上图可以看出，四个评测对象在黑龙江、北京、山东、青海、湖北、云南、福建、海南等丢包率差异较大，在吉林、辽宁、宁夏、新疆、湖南、安徽等地平均丢包率差异较小。在34个省份中，宁夏访问阿里云云主机，平均丢包率最低为0.08%，广西平均丢包率最高为14.27%；澳门访问腾讯云云主机，平均丢包率最低为0.08%，广西平均丢包率最高为14.03%；西藏访问AWS北京的平均丢包率最低为0.22%，福建平均丢包率最高为19.17%;澳门访问AWS宁夏平均丢包率最低为0，福建平均丢包率最高为30.41%。
**6. 时段平均丢包率**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBV2yjhicTBLPnIVFkfiaPFuTleO4mLXykw76wxIVm32oHCRRQwjrwdzxTlibSvWngvjGbVFfMZSBq95Q/0?wx_fmt=png)
*图14：时段平均丢包率*
如上图显示的四个评测对象由0:00-23:00各个整点的平均时段丢包率数据情况，阿里云与腾讯云在丢包率上优于AWS北京、AWS宁夏；且在凌晨时段，腾讯云丢包率较多低于阿里云，在晚上时段，阿里云丢包率较多低于腾讯云；除了在正午12点AWS北京平均丢包率数据反差较大外，其他23个整点，AWS北京平均丢包率数据均小于AWS宁夏。
**五、 小结**
阿里云、腾讯云、AWS北京、AWS宁夏的云主机在本次评测中，无论是在性价比方面，还在是平均时延、平均丢包率等方面，各有优劣（见“三、评测结果”），在采购云主机时，可综合考虑以上评测结果及详细数据，并结合采购需求进行选型。
CloudBest是业界领先的云资源选型服务平台，致力于为中小IT规模的企业提供云产品选型采购、统一管理、运维售后等一站式服务。
CloudBest借助AI及大数据技术，帮助企业进行云服务选型、采购、统一管理，平台通过深度挖掘各行业用户的IDC、云计算使用需求，建立标准化的需求自评估模型、产品服务比价模型，同时实现线上线下的服务交易及交付。真正实现在我国互联网＋的进程中，帮助企业选好云产品、用好云产品。CloudBest旗下监测实验室是为数据中心及云计算领域提供全国数据中心基础设施监测、全国网络运行状态监测、云主机性能监测、服务商服务能力监测等服务的平台，为企业和国家的战略部门、职能部门提供决策参考。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
