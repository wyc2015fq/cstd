# MIT科学家Dimitri P. Bertsekas最新2019出版《强化学习与最优控制》(附书稿PDF&讲义)... - 人工智能学家 - CSDN博客
2018年12月17日 21:12:54[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：254
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaz1W5p3gk6ZvdmickarrG3FS7q5LYEe78LbG2SNib3Oeibiawfh7NYIZhAQ/640?wx_fmt=jpeg)
来源：专知
摘要：MIT科学家Dimitri P. Bertsekas今日发布了一份2019即将出版的《强化学习与最优控制》书稿及讲义，该专著目的在于探索这人工智能与最优控制的共同边界，形成一个可以在任一领域具有背景的人员都可以访问的桥梁。
# REINFORCEMENT LEARNING AND OPTIMAL CONTROL
## by Dimitri P. Bertsekas
## Athena Scientific, 2019
> 
这是一本书的草稿，预计在2019年的某个时候定稿，并由Athena Scientific出版社出版。这意味着这份书稿代表“正在进行的工作”，并将定期更新。它很可能有些错误(希望不是严重的错误)，此外，它对文献的引用可能是不完整的。欢迎读者发送邮件（dimitrib@mit.edu） 向作者提出意见和建议。
本书的目的是考虑大型和具有挑战性的多阶段决策问题，这些问题可以通过动态规划和最优控制从原则上解决，但它们的精确解在计算上是难以解决的。我们讨论了依靠近似来产生性能良好的次优策略（suboptimal policies）的求解方法。这些方法统称为强化学习（reinforcement learning），也包括近似动态规划（approximate dynamic programming）和神经动态规划（ neuro-dynamic programming）等替代名称。
我们的学科从最优控制和人工智能的思想相互作用中获益良多。本专著的目的之一是探索这两个领域之间的共同边界，并形成一个可以在任一领域具有背景的人员都可以访问的桥梁。
这本书的数学风格与作者的动态规划书和神经动态规划专著略有不同。我们更多地依赖于直观的解释，而不是基于证据的洞察力。在附录中，我们还对有限和无限视野动态规划理论和一些基本的近似方法作了严格的简要介绍。为此，我们需要一个适度的数学背景：微积分、初等概率和矩阵向量代数等。
实践证明这本书中的方法是有效的，最近在国际象棋和围棋中取得的惊人成就就是一个很好的证明。然而，在广泛的问题中，它们的性能可能不太可靠。这反映了该领域的技术现状：没有任何方法能够保证对所有甚至大多数问题都有效，但有足够的方法来尝试某个具有挑战性的问题，并有合理的机会使其中一个或多个问题最终获得成功。因此，我们的目标是提供一系列基于合理原则的方法，并为其属性提供直觉，即使这些属性不包括可靠的性能保证。 希望通过对这些方法及其变体的充分探索，读者将能够充分解决他/她自己的问题。
**Slides**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliarXvIjibE9Q22tO3aSEBZRVKYDLKPU0qKhLEp9LDkYb1D4ibYP2KiaF0XA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliakJ5wmgxrKAlYLeiaLKbnfNy8WvUo2aDB7CYxUUdphAt3L4aQc4R69Tg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaE7sSF0iaXyFFQEcwEcLuk0MxCsDrtfYe0SJiaRF9gWSrg3GIDSj1XxwQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliajgTFkeRAX0Z1sHWfVUSaAf46KrZMwFt4LsDCh15dAM1hedqRYCknpQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaxLVaInMSr1ibJJc6NftHUAod6BFXibxQpweFN8fRRoWM5K0pmibnDYPDw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaBAZXE0c3Aod2lhiaLmyvj2hvrPk7ejx4qj5vibQFI0iaExtENFp6dhQyA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaCStj1n78LvsGcfVajo9gBMAeZ8zWDCx8UBic1Yaic97ibOC4oyqUViaNSg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliabqrqamel1zblOM3g5iaE6f0jmJldHffI82icuvsBqXBg5HPKe2LWfjicQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliafnXCT8TPiaMaKs7ryRsZSG9OjOC4SmyZRfI1Y1rJmHAViaiaiaXf5bE7BA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliazjhv5jyHiblM1T2icOBW8yNHvgb6WavdfVPotO7aWVuMnpHiaPZocVM7w/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliazEmicj4IN3wktBJ5kINqJGqicaBibtdyib2EqB8hFUbFPQl24ozAicibjOmw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliawBhTcDYtRwYb6zCAZoYZGCDbfm8UibtS1odVPx4jmzbRuCJC6JeZLibA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaYVpEkCAsSQdbyT8QNMo5bvxe3J5hqhibN7gSuEXZmbFica0WpMNmPtNQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaNGJT6cM57TM3eia7FnEnzwhfGs4Kb1qV7ibhBAibWWOT9ibBciaMbcUm6oQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaNGJT6cM57TM3eia7FnEnzwhfGs4Kb1qV7ibhBAibWWOT9ibBciaMbcUm6oQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliarHd5zWW1mDCAsxqCAcHf4cb3hCMnia69qK3zx78c3N4COicxLl0S0ziaA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaLQ2M4qlCFXqAgFQ9BTmrIWOxI9CuC428mWN479SBQu0iclad2qTEWqA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaME4fFjRQj7BhTMtwDzHaCz5EL69OaiadicicJCZVbG0mnjZWyQQyztZnQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaPTf2W7VqD3IpRv009DkRFJhKYEa7BIyicqmIaEVlwwc6NxEPyZRUlEw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaDfRZxFbibXFDpdUxAEc5jz6c2VQzQria44zwQdKfl8PuDYTp5IKGoAGw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXlia068VCCQAxD4yV3kvxzhWLricib1ukeFaXGibI9uzeSB2IZvtgAImNAibAQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliah7sZ4NSiaebh85KAIL9XojibFQ6O22rdqElo37fSNj2fmb4wLPpkG9sA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliah4FIbsfoN4VLnibUN6yVlQbyIsgvdS12ePWMKl54yruSBBANEfugM4Q/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliah4FIbsfoN4VLnibUN6yVlQbyIsgvdS12ePWMKl54yruSBBANEfugM4Q/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliau6Dn4UZrYXEuBz8XmDee8OWz2zibflmB1OEicfnCHDH7Dic4foLpQFylg/640?wx_fmt=png)
**章节**
第一章：精确动态规划 Exact Dynamic Programming
第二章：值空间中的逼近 Approximation in Value Space
第三章：参数近似 Parametric Approximation
第四章：无限视野强化学习  Infinite Horizon Renforcement Learning
第五章：集合 Aggregation
参考文献 Reference
**前言 & 目录**
▼
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXlia506XXPtMLoxwIUJZzdDMqsE2ZEIGHx52r9bhsMOA1cQ2WtZVUvfskw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaVeqFyAjpZAat7Rib0ynVHszjfichH1P8Sh3saL1e9obrrkfAQBiczwjZA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaD8gxH9drokdfaqr2o3jHVgrVDHKqjSZZiaiafe7PIhqdibYADYOjTxvxA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaSV3DTZ15jeSBTrCvUxZU4JPvPIiadUib0KDQlkaKHF29oQfWZWner3zg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliax47N9jZQJb6CLPAns5qOl3UKWFyNgYe8AlibibcE7OR30NFIDlhNoyCw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaPCic6usxOHiaFh9Y3iaicwvmXCwIyXDU5claIwibnRfSm8WzXBeI6DZjhYA/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliam7Laz0Y1M5QeXbZVvYcdJBLMKxJw3Ro70h4Y7o7mh2pQ3bsQEj7rVA/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaA3dC4N8ia7gic1nTVmyib5PvNLJwuV6wqEBPJZSp2EzFibI2eicE1icRoGHg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaFLiaBoza4vCKxLHrLfYAdxDbicHwMHUEJ2AOB0UHnHFpic4jk7p5RBSVw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw2icFibCVQuh0C7TLwF5wdXliaYGZm3BVhW2Z0ZAFRZ0zRN4IXZ1AOJzVI3bicD0WO1c9JJqvhGG3uM7w/640?wx_fmt=png)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
