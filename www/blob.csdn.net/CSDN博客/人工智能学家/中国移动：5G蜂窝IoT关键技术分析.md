# 中国移动：5G蜂窝IoT关键技术分析 - 人工智能学家 - CSDN博客
2018年11月20日 22:48:35[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：116
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBUVNCbUGv4B6G2GDibohMFa2m8Df3uzjTn9YwnjEoNBGMFXVuN1to77XkbF1g55gIkHL1jqzcHPXNw/640?wx_fmt=jpeg)
来源：5G
本文讨论了蜂窝物联网的技术现状，针对增强机器类通信和窄带物联网技术标准，提出了2种现网快速部署方案，并进一步指出了C-IoT面向5G的演进路径。该路径充分考虑了5G网络中网络功能虚拟化、软件定义网络、移动边缘计算和大数据分析等关键技术以及面临的安全风险。对关键技术挑战以及解决方案详细分析的结果表明，5G C-IoT可有效扩展C-IoT业务能力。
蜂窝物联网（C-IoT）是以蜂窝网络为主要接入手段的低功率广域物联网，是物联网十年发展后的重要路径，包括已经商用的2G/3G/4G物联网，以及正在商用部署的增强机器类通信（eMTC）和窄带物联网（NB-IoT）。
随着5G网络第1版标准的冻结，5G即将进入商用。5G充分考虑了通信计算融合趋势，引入了软件定义网络（SDN）和络功能虚拟化（NFV）概念，支持移动边缘计算（MEC）和网络切片技术，网络能力更为出色，为C-IoT进一步演进和发展提供了技术方向与基础。
笔者在分析C-IoT现网部署方案的基础上，详细讨论基于5G网络架构，如何将NFV、SDN、MEC等关键技术融合到C-IoT体系架构中，构建5G C-IoT。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY84RxLTCIH57K2qKQTtFURWabZ2V0buLwBJvxZ0pTMJxWGEmV53Yvhw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYWOFa2iaq4bMqk4X2R4A6kIYyKnBfria3aFgoltMyaFsdAmWbneUS3VoQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY0eSDHlDnrm8w7Ou64t0IlnQWlD3yOLArIYXxFqaFgibszEIicLCkGJ6Q/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY6Bu3ZFqyvsnNCuYy5PqqN2iaJdKHG2X1UEOhN1tzCo4Ah9w5qNP5VXA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYqckHEsIL5G5LCrZObuxUuX2rCKQKZepleN8oCeXchFu6XebtdW7E2w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYfOYQIgIEA0MFia2Kp7tibSane4kEo2DHqee6nqah7aMQhmvXNUIdiaYPg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYUf3ljYvHmPic26d1WYiaS8I0lUia89Q4Lt0mDk8ib3rLX71du9ZQ0nY78g/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYicX8qHzNxEo5iaLqMxpibJxwbw560V2mUWSHy86HZokC4UpyfQQibP7ztA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYFW56sQERZKiaR0khJjjuIZuLrReiaHJL6h6Bwicz7kYAFibZgGhQ1T44fg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYXGQribZ0uM9iaATibwgKvhviasNOoTcUibUXQx5wOXLLlRDicicolhe2ILv5Q/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYichuBarNpnhLxPxhcNUoOC0pkaE0KPT7cURhrwWuSawb5xa25Xrau4w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYXF2ZUmDVzPQP7Licuyzs4gj8pdZkvMztzgMm5QwSaGkQoAiallDQtKqg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY1zLqS1dZyxTmhB9OHFWVPft3bkiaGuZicKh1ciazhxV4FMaobIJv4491w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYdhCpYlWt4jYiaFTiaBCuvNEuCg1JkZfMxibzJQNsgiaVB9MXL5bXROxicvA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYU7dkTYfJg3273FUXv89mGCJWzcNZlBX8LGorww5mAQttDbK8BV5sTQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY3oXPsf9RIariaTBhicbRBBBgQJXadl1ibEqAaAVdmh4QRpUt9H0KZibbcA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY2CPeekMbGZKCia7lv5enicib29bFWURxlXJfHmnxcWvu7fkDHveohj57w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYLKTxV7sibS0PJKh4nt563B0LcnM2MWibxic8n849pYps1KHKtC0YEwnRw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQY3XmrFoanSoXnosNSic3PtPWZke1sPW0UJ65RiceZ03lSpt4cyiakmvQnQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYSkKfB5IYDicQtkzcFC8bGI8tcR39eicAHAMe4q6ic5IicOqU8JGeDNic9ibA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYBTEBqge5gB7q1jM5hFNNwUOXicDeAEHTc0KGh3oTSMiaVOkpKdP3Brow/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYLffIBQMV5zpGeoafJX0vial50LvyGicXzRmNvRaunFnNicCxcXGTP3Q5g/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYkNcibvyYSWXianL6xHYrPIWKLjWQUCGZL5muibGMGchlhPqc7Rm2FiciapA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYefLBvy7Yvbw2LAwAIex2Priaic5M5xj9DP7hoZPRfoianqGqJeF35ia99g/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYqseh3MqOC6OEdK6FK3eoMTbVQvtRKs8jZjHw3LM76ypokWnbQZhiaHg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYhYbpDl2CDIfA49OTJCwSwJNlsjIkVrVAsoYwsCIzCr4UWGRGDsNWtw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYhJhekCpMcUxEbkGqywk7tr0vbeXEUOv60TVT8gJzicV1gGiaoqArkODA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYmGR6aCULNVBWSt0WptMgE9YbWTkxuPqg2XAymlAr05YTicqiaBlrPibvw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Jz93sMRHsWoVxKapsPrUuJvb3oIicQGQYDnyoSrrY5IViaS04IQbzy6t0LgayzpgJWSvfHHPAjO0aM5Dicth4mmXQ/640?wx_fmt=jpeg)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
