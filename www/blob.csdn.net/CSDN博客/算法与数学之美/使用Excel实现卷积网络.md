# 使用Excel实现卷积网络 - 算法与数学之美 - CSDN博客
2018年08月14日 22:40:39[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：388
【导读】Dave Smith使用Excel电子表格深入浅出地讲解了卷积神经网络（CNN）是如何识别人脸的。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQz2CcB6PRGZDZ7iaX8k2MiaXGLlYCbDFDcIMhHbfJS1kaKfNGnNjgIACDQ/640?)
当你入门的时候，可能觉得机器学习很复杂……甚至很可怕。另一方面，电子表格却很简单。电子表格并不酷炫，但却能避免分散你的注意力，同时帮助你以直观的方式可视化代码后面发生的事情。
我将循序渐进地通过电子表格（你可以通过下面的链接下载）向你展示卷积神经网络（CNN）是如何应用于计算机视觉任务的。其中包括一些数学，不过电子表格中包含了所有的公式。
https://drive.google.com/open?id=1TJXPPQ6Cz-4kVRXTSrbj4u4orcaamtpGvY58yuJbzHk
这一电子表格模型查看一幅图像，分析它的像素，并预测是否是Elon Musk、Jeff Bezos、Jon Snow……显然这三位是天网的最大威胁。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzWgsB3icGlBniaTfNZn1OsNBJ6PwNbvqicvnToB2XZvlfXYj24G5c6My7Q/640?)
终结者视觉——在电子表格中创建卷积神经网络
本文将介绍上图中的9个步骤，每个步骤都会使用类比的方法给你的直觉增压。
本文的目标是使用容易操作的电子表格，提供入门机器学习的简单路径，并向充满好奇心的读者展示尖端AI技术的奥秘。如果本文对你有帮助，请注册我创建的邮件列表，注册后可以收到更多后续的电子表格，帮助你入门机器学习和创建神经网络。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzekc82jq8l2RKfVicHIz9ibvia2q9Jo56YdTeeoibjQHeArZJSaHNF8HBQA/640?)
Facebook面部识别系统、某国奥威尔式的大规模监控系统、你的汽车（不久的将来）背后的基础都是计算机视觉。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQz5nVZt5PO0G7uSo9rYFA7I3XqBbJK8Mt5n3gH9hdfIfk2yRUiaicAacog/640?)
## CNN神探
让我们假装终结者的头脑里住着一个特别的侦探，名叫“夏洛克·卷积·福尔摩斯”。他的工作是仔细查看证据（输入图像），使用敏锐的眼神和推理能力（特征检测），预测图像中的人是谁，从而破案（正确分类图像）。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzLJShwIFC7f6BnWqg2ibJIDl2qe74td1TeQpVT6tbofibgbbYoMpNJJUA/640?)
卷积网络架构
你将学习：
- 
输入：计算机如何看
- 
特征检测：像福尔摩斯一样思考
- 
卷积数学：福尔摩斯的侦探工具
- 
ReLU：非线性模式识别
- 
最大池化：保留最重要的线索
- 
扁平化：排列所有线索
- 
全连接：完成案件拼图
- 
Logit + Softmax：破案
- 
交叉熵损失：福尔摩斯的“正/误”
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzrjBOibv9hxR9mddMN1WsBc9Ycooc4l5VjIptWqNxqwoGmaibXwFbFFcg/640?)
## 输入——一图胜千言
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzjmXmDY2NHgAFUYpn0jJwGB90XibN8fNA3Micoia9zCxMxnjx0edN6EIJw/640?)
天网的最大威胁——Elon Musk
我从上面的图像中看到了一个有远见的人。他一方面不断改善地球这颗星球，另一方面创建逃离地球的火箭，以防终结者试图摧毁地球。和计算机不同，我看不到像素值，也无法辨认出一张图像不过是红光、绿光、蓝光的堆叠：
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzGGV8pNuYncX6dJrQSQRnDSuoonvtlkEVM9qHywKlEqK1ms14zGqccA/640?)
另一方面，一台计算机（比如天网）是瞎的……它只能看到数字。
想象一下由3张电子表格（红、绿、蓝）堆叠构成的电子图像，每张表格是一个数字矩阵。拍照的时候，相机测量到达每个像素的红光、绿光、蓝光的量。接着它在0-255的范围内评估每个像素，然后将其记录到电子表格中。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzbRicsiaoBCdPuVSHdF1ldxc8hq0xt7Id4S0RI8Mz7ZibmpBX96iaN6WcJw/640?)
计算机看到电子表格
上为一张28x28的图像，每个像素由三行（红、蓝、绿）表示，取值范围0-255. 每个像素的颜色对应其数值。
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQzFlXsJrqL5tNvf4TcgypUU0yDmTBffpDXNNibN3gSMXQLrHibfv9XT5zQ/640?)
终结者没有看到眼睛，他看到的是一串数字
将每种颜色单独放到一个矩阵中，我们得到了3个28x28矩阵，也就是我们之后用来训练神经网络的输入：
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/hq0PKaHicMTHdJet93tXFicVdqiaoJwuicQztTV5czbVQ8wtnfjAhyp2Mn1PlwI2f9ckypIcu3rENQsJmqpHWMcknQ/640?)
模型输入
如果你想知道如何将任意图像转换成电子表格，请访问以下网址：
http://think-maths.co.uk/spreadsheet
你将学到如何得到一张“Ex拍”，你的Excel迷朋友们会喜欢的……相信我，在电子表格中找到你的马克杯（或者他们的）会让他们捧腹大笑的            
