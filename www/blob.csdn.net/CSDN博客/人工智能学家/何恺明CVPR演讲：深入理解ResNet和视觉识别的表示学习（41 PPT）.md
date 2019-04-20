# 何恺明CVPR演讲：深入理解ResNet和视觉识别的表示学习（41 PPT） - 人工智能学家 - CSDN博客
2018年06月30日 20:56:28[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：538
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/X8OUs1wOMWGEe4a6R02cSg3n6v4WwyXCianDU5BLAu6PBibjibYPgibJGiamIF8fow9hicwou64tajvAGzaXibvECWeYQ/640?wx_fmt=jpeg)
来源：专知
摘要：今年CVPR 2018上，刚获得“TPAMI”年轻研究员奖的Facebook的Kaiming He做了一个叫“Learning Deep Representations for Visual Recognition”的讲座。
在今年CVPR 2018上，刚获得“TPAMI”年轻研究员奖的Facebook的Kaiming He做了一个叫“Learning Deep Representations for Visual Recognition”的讲座。内容重点涵盖了他参与发明的ResNet/ResNeXt的细节结构以及一系列重要模型（包括LeNet、AlexNet、GoogleNet）的回顾。
何恺明关于“Learning Deep Representations for Visual Recognition”的最新讲座，内容重点涵盖了他参与发明的ResNet/ResNeXt的细节结构以及一系列重要模型（包括LeNet、AlexNet、GoogleNet）的回顾。整个讲座内容深入浅出，重要的技巧比如Batch Normalization也都有涵盖。
## ![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL38AGh4oKFDFVIHRRgIDKvNQ1QaHTgJZmMfuADSTr1IRowacsqNt3h5w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3jQ2lQFLB3PPfD0orhUaMTIqwy0qyE9LLK8KHYJ1oYGUJONdIGnZeIA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3lsiboE6ynibnWNQ9XcLcus4icEiaSa1q66J1nSQeW0yQtUjcAiavTVjsbuA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3ibicfybXUfFLXNg3v22icNl9qE97XGKUmBlfdgibgIxIc03uoh5M4yfU2Q/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3yNcPd0HuoULTr0nxhIwJkkUw8tAMXn5mUGZVJQcPxPiaU2bpdE9WXXg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3elEsO4XVtibHl5174KYR6XJScVOqPzia83gVRjibbKeyhGo8aHBsVK4Qg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL37h8jykX5eSiazS5xG0CN2YUeN4BUlnft6CDHRR9jpfJVAaRicjUEXPXw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3Yf5EYuLBEffsBJRPBQv32gXr9vMMxHu15vSVy6IOFC9ZKibxqorFJ7w/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL37fbLcVf8vibzxf5WTOTkjcasEsiaP1LnR7WhibibMh9sNOrWlmjM668hLw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3ibcX9cIVicoSLdbL8pKnvVXqoIHLtnv1hACicDKyN5iaTIgQKFl09F7XHw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3V1cOClVV7PtjVHRsnomUNiaWKt935MvwgXrFkPpTvH32SL2gicgqXZcA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL37NJicW4icUUCgg7e7HeibuibBHybyB2ZJLN3u8lFGxlmswuXffWEvuuWNQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3ubFicv5y4bnOMoqeicDYGDVsDFiaw3d4xBbSokWWSyDz98Bc3UHxjp6OQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3UBoFTgicSqHK2AyvhhhQIibQnHic30h9ib8qcUBRe5yNJXZRQfkbtnVqtQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3YShpBwAbNQP2prIFjaeZic5crKefHribAzsVF2N1MTutTGZic18vsll5A/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL35QC2dvV7D6LbmMnmZbVJec5gJZVJKO5V5rSkRpnZ3kw6K6BtWuHaBA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3gHxsHKiaZR1Z2Jq0QMZBmXP6516xl4FeDiaWQnfnaZt2xiaP9FtnE8WicQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3ibKKv3f8r8W1QPb4KVqadfxpRgWjcUV36kEA5lxDkz7rKVNOtfo70qA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3dYM0kOvMLEiasqsOSLkic1owq6UdkqgXqL1x5ZoWrI27DrsQn4szTnIw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3qASXekBFF82XFdpGvHGqsuCMvj5AliaWBxtEYkgL1cWoTiaWIGGuBKug/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3uvt2JHic8TJBAQXibrl0cyOJJQxImATWsR03YYSyHwcjIzzFvhN6FDEA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3iaXuT10tKWK2h5X6FibEwg5p6QKiatxTWEsogR6UFFKKgjibWJqBN7lG8A/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL34prHiamymvKGhBG6nPj2DBfWfsjLa8Wu53Qpa75dqoVRJFNAhwWdVXg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3iavBQzbtlPIU4PyTCOttlA4fflRoicReBAfqHD0RGzvznAAwGPFs1WgA/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3kYD6qQBm0hcaOmDolSlZ8P31At9rBYUsZlyHr02qmsskYAricnjHBvw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3ksdzX0AhoJ16XrWuZhNPPNdB56sltldFuz96BvQibp6x2joRxnvKb3A/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3ddcmFxbbY5drac65oJYicAKXt75BNCvjXb05pOd4j4xbMMqx7e1icaNw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3GGpiaiblic2uCveGcdhuuDv3nHbWjNMf7r6BoRKp9ozzsFEA58DcicV6qw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3006NUIhWz3ZbyMEEUB9lbGaCA1p6mBHW9GvmPwxrSwQYoMv6DichSKQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3L6z3bORqrqBv1oDCNsZwdZgHiaSWbib7gHhv0875HrXgkVIZia1wsUrdQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw0ce6XHfcHZo6iczh9udgkL3RZqwBSKXxbLr3rNj6Whx3zM39DSNgibWfmPwJfPv6h9S1skhgnjxMHA/640?wx_fmt=jpeg)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
