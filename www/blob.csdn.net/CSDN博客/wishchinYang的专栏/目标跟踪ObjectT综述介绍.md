# 目标跟踪ObjectT综述介绍 - wishchinYang的专栏 - CSDN博客
2014年07月16日 17:07:31[wishchin](https://me.csdn.net/wishchin)阅读数：1109
            此文也很详细：[http://blog.csdn.net/maochongsandai110/article/details/11530045](http://blog.csdn.net/maochongsandai110/article/details/11530045)
            原文链接：[http://blog.csdn.net/pp5576155/article/details/6962694](http://blog.csdn.net/pp5576155/article/details/6962694)
        图像跟踪是一个不断发展的研究方向，新的方法不断产生，再加上其它学科的方法的引入，因此对于图像跟踪算法的分类没有确定的标准。对于所有的跟踪算法，需要解决两个关键问题：目标建模和目标定位[35]。以下根据目标建模所用的视觉特征和目标定位所用的方法对跟踪算法分类。
一．据视觉特征分类：欲实现目标的准确定位需要以描述目标的视觉特征建立其表观模型。具有良好可分性的视觉特征，是实现对跟踪目标与视场背景精确分割与提取的关键，因此视觉特征的选择是实现鲁棒跟踪前提。若所选视觉特征具有较强可分性，即使简单的跟踪算法也能实现可靠跟踪。反之不然。常用的视觉特征分类如下：
**颜色：**由于颜色特征具有较好的抗击平面旋转、非刚性变形以及部分遮挡的
能力，变形目标跟踪中表现出较强的鲁棒性，因此广泛的应用于视频跟踪的目标特征选择上。文献[56]中的基于颜色直方图跟踪算法(Color Histogram)，采用Mean Shift算法实现对非刚性目标的鲁棒跟踪。此算法的不足表现在目标遮挡和相邻两帧出现较大的目标位移时，由于Mean Shift算法搜索区域局限于局部状态空间，此时会出现跟踪发散。为解决此问题，文献[57,58]中，由Perez等人和Nummiaro等人提出将颜色特征作为粒子滤波观测模型，实现了复杂环境下（目标遮挡）的可靠跟踪。此算法不足在于，当背景出现与目标颜色分布相似干扰物时，易造成粒子发散，因此Birchfield等人[39]提出空间-颜色直方图跟踪算法，充分利用像素点之间的空间关系而不局限于颜色分布，改善了跟踪性能。
**边缘：**虽然颜色特征具有较好的抗目标形变的能力，但是缺乏对目标空间结构的描述，且对光照敏感。因此在光照变化频繁的跟踪视场下，常采用目标边缘特征。文献[40-44]将边缘信息作为目标可分性特征，从而实现可靠跟踪。由于颜色与边缘特征具有互补特性，因此将两种信息融合建立目标特征模型的算法，近年来引起研究者广泛关注[45-47]。上述基于边缘特征的跟踪算法存在计算耗时较长以及形状模型单一的问题，制约了跟踪算法的实时性及可靠性。因此文献[48-50]提出了基于边缘方位直方图特征的跟踪算法，此算法对光照变化不敏感且比单一轮廓边缘特征具有更丰富的信息。
**光流特征：**光流特征通常是采用Lucak-Kande算法计算像素点光流的幅值和方向，文献[51]为利用光流实现人脸跟踪实例。由于光流法运算量较大很难满足实时性要求，并且光流法本身对光照变化和噪声敏感，都限制了光流法的实际应用。
**小波：**由于金字塔可实现在不同角度、尺度上对图像进行描述的功能，这也是实现差分运动估计的基础[52,53]。
**局部特征描述子：**图像的局部区域特征具有对光照、尺度、旋转的不变性。局部区域特征从局部区域内提取特征点，并以相应的描述子对其描述。文献[54，55]分别以局部二元模式和（SIFT）特征实现目标跟踪。
**空间与颜色融合：**颜色特征描述目标全局颜色分布，属于全局信息，虽然具有一定的抗目标形变能力，但由于缺乏对像素间空间结构的描述易受到背景中相似颜色分布区域的干扰，文献[56,57]将空间信息与颜色直方图融合，作为目标特征取得了良好的跟踪效果。
**特征基（Eigen-Basis）：**将图像信息从高维空间映射到低维空间，图像在低维空间成为一个流形，通过一定的几何、统计工具可以提取和分析。PCA、LDA是图像跟踪领域构建子空间广泛采用的方法。特征跟踪（Eigen-Tracking）方法[18,58-61]以Karhunen-Loeve构建表征目标表观的特征基，再通过递增SVD实现对特征基的在线更新。文献[62]以局部线性嵌入流形LLE将跟踪问题映射到非线性低维流型空间去解决。
**模式分类：**利用分类器将跟踪目标从背景中分割出来是以模式分类的方法解决视频跟踪问题。文献[64,65]同时强调目标与背景的重要性，通过特征评价算法建立对目标和背景具有良好可分性的的视觉特征实现跟踪。Avidan[65]以支持向量机SVM离线学习得到目标与背景特征，称为支持向量机跟踪算法（SVM-Tracking）。文献[67]利用集成学习将弱分类器（Adaboost方法训练得到弱分类器）组合成强分类器，由此强分类器实现对视频帧中目标与背景分类，即像素分类置信图（Confidence
 Map），由分类置信图的模式得到当前帧中目标位置，将输出的目标位置反馈，训练出新的强分类器以实现后续的分类。为克服上述文献中由于采用离线学习方法使得跟踪算法不满足实时性的问题,Grabner等人[68]提出了基于在线Adaboost训练分类器的跟踪算法。
                          最新文章：**Multi-Forest：A chameleon in tracking**，真够操CVPR2014 蛋的...
二．依据目标定位所使用的方法分类：目标定位根据历史信息推理当前帧中目标位置信息的过程。依据目标定位方法对跟踪算法分类如下：
**概率跟踪方法：**概率跟踪方法是采用Bayesian滤波理论解决状态估计问题在视频跟踪领域的应用，通过预测和修正过程采用一种递推方式实现时变状态的估计。表征目标位置信息的状态量通常由位置坐标、速度、尺度以及旋转角度构成，状态量通过状态转移模型向前推进即实现状态预测，通过最新观测值以及观测似然模型对状态预测置信度进行评价，从而对预测值做出修正。在模型线性（状态转移模型和观测模型）、系统噪声和观测噪声服从高斯分布时，Kalman滤波能给出Bayesian滤波最优解；对于非线性Bayesian滤波，扩展Kalman(EKF)以及无味Kalman(UKF)
 [69,70]给出了次优解。隐马尔科夫模型(HMM)[44]用于实现状态空间有限、离散情况下的状态估计。对于状态模型和观测模型均为非线性且噪声为非高斯，同时状态分布呈多模态，利用Monte Carlo(MC)方法通过采样估计目标状态后验分布，取得了良好的效果，其中以PF为代表的MC采样方法成为了研究热点。
**确定性跟踪方法：**该类算法的基本思想是由目标检测或者手动设置方式获取目标模板，度量目标模板与备选目标位置的相识度称为评价函数。跟踪的过程，即将备选目标位置与目标模板匹配的过程。以最优化方法计算评价函数最大值，使得评价函数取得最大值时的备选目标位置判断为是目标在当前视频帧中的估计位置[36,56,71]。通常选择颜色直方图距离作为相识度评价函数。该类算法在一定场景下能实现快速可靠的跟踪，但是该类算法的可靠性是建立在目标模板在跟踪过程中不发生变化的假设之上，因此当目标表观模型改变，跟踪结果与实际目标位置会产生较大偏离甚至失跟。针对此问题，文献[72]提出实时更新目标表观直方图的方法，提高了确定性跟踪算法的鲁棒性。由于概率跟踪方法能够解决复杂背景下的目标状态估计，特别是以PF为代表的MC积分法实现对Bayesian滤波的近似已成为跟踪算法的主流。由于PF以一组随机加权样本近似Bayesian滤波，不受模型线性高斯假设的限制，PF已成为解决非线性非高斯模型下的状态估计问题的有力工具[38,41,42]。
**Reference：**
[30]B.Jung and G.S.Sukhatme.A generalized region-based approach for multi-target tracking in outdoor environments.[C]In:Proceedings of IEEE International Conference on
 Robotics and Automation,New Orleans,LA,United States,2004,2004,2189-2195.
[31]B.Jung and G.S.Sukhatme.Detecting moving objects using a single camera on a mobile robot In an outdoor environment.[C]In:Proceedings of International Conference on
 Intelligent Autonomous Systems,2004,980–987.
[32]L.Mun Wai,I.Cohen and J.Soon Ki.Particle filter with analytical inference for human body tracking.[C]In:Proceedings of IEEE Workshop on Motion and VideoComputing,2002,159-165.
[33]H.Z.Ning,T.N.Tan,L.Wang,et al.People tracking based on motion model and motion constraints with automatic initialization.[J]Pattern Recognition,2004,37(7):1423-1440.
[34]T.Xiang and S.G.Gong.Beyond tracking:Modelling activity and understanding behaviour.[JInternational Journal of Computer Vision,2006,67(1):21-51.
[35]C.Shen.Robust visual tracking in image sequences.[D]The University of Adelaide,2005.
[36]D.Comaniciu,V.Ramesh and P.Meer.Kernel-based object tracking.[J]IEEE Transaction on Pattern Analysis and Machine Intelligence,2003,25(5):564-577.
[37]P.Perez,C.Hue,J.Vermaak,et al.Color-based probabilistic tracking.[C]In:Proceedings of 7th European Conference on Computer Vision,2002,661-675.
[38]K.Nummiaro,E.Koller-Meier and L.Van Gool.An adaptive color-based particle filter.[J]Image And Vision Computing,2003,21(1):99-110.
[39]S.T.Birchfield and S.Rangarajan.Spatiograms versus histograms for region-based tracking.[C]In:Proceedings of IEEE Computer Society Conference on Computer Vision and
 Pattern Recognition,San Diego,CA,United States,2005,2,1158-1163.
[40]N.Peterfreund.Robust tracking of position and velocity with Kalman snakes.[J]IEEE Transactions on Pattern Analysis and Machine Intelligence,1999,21(6):564-569.
[41]M.Isard and A.Blake.CONDENSATION-Conditional density propagation for visual
tracking.[J]International Journal of Computer Vision,1998,29(1):5-28.
[42]P.H.Li,T.W.Zhang and A.E.C.Pece.Visual contour tracking based on particle filters.[J]Image and Vision Computing,2003,21(1):111-123.
[43]J.Deutscher and I.Reid.Articulated body motion capture by stochastic search.[J]International Journal of Computer Vision,2005,61(2):185-205.
[44]Y.Chen,Y.Rui and T.S.Huang.JPDAF based HMM for real-time contour tracking.[C]In:Proceedings of IEEE Computer Society Conference on Computer Vision and Pattern Recognition,Kauai,Hawaii,2001,1,543-550.
45]C.Shen,A.van den Hengel and A.Dick.Probabilistic multiple cue integration for particle filter based tracking.[C]In:International Conference on Digital Image Computing:Techniques
 and Applications,Sydney,2003,1,399–408.
46]A.Li,Z.Jing and S.Hu.Particle filter based visual tracking with multi-cue adaptive fusion.[J] Chinese Optics Letters,2005,3(6):326-329.
47]P.Perez,J.Vermaak and A.Blake.Data fusion for visual tracking with particles.[J]Proceedings Of the IEEE,2004,92(3):495-513.
48]E.Maggio,F.Smeraldi and A.Cavallaro.Combining colour and orientation for adaptive Particle filter-based tracking.[C]In:Proceedings of British Machine Vision Conference
 Oxford,UK 2005.
49]B.Han,C.Yang,R.Duraiswami,et al.Bayesian filtering and integral image for visual Tracking [C]In:Proceedings of 6th International Worshop on Image Analysis for Multimedia
 Interactive Services Montreux,Switzerland,2005.
50]C.Yang,R.Duraiswami and L.Davis.Fast multiple object tracking via a hierarchical particle filter.[C]In:Proceedings of IEEE International Conference on Computer Vision,Beijing,China,2005,I,212-219.
51]D.DeCarlo and D.Metaxas.Optical flow constraints on deformable models with applications To face tracking.[J]International Journal of Computer Vision,2000,38(2):99-127.
52]A.D.Jepson,D.J.Fleet and T.F.El-Maraghi.Robust online appearance models for visual tracking.[J]IEEE Transactions on Pattern Analysis and Machine Intelligence,2003,25(10):1296-1311.
53]P.Viola and M.J.Jones.Robust real-time face detection.[J]International Journal of Computer Vision,2004,57(2):137-154.
54]T.Ojala,M.Pietikainen and T.Maenpaa.Multiresolution gray-scale and rotation invariant Texture classification with local binary patterns.[J]IEEE Transactions on Pattern
 Analysis and Machine Intelligence,2002,24(7):971-987.
55]C.Wen-Yan,C.Chu-Song and H.Yi-Ping.Discriminative descriptor-based observation model For visual tracking.[C]In:Proceedings of 18th International Conference on Pattern
 Recognition,Hong Kong,2006,3,83-86.
56]C.Yang,R.Duraiswami and L.Davis.Efficient mean-shift tracking via a new similarity measure.[C]In:Proceedings of IEEE Computer Society Conference on Computer Vision
 and Pattern Recognition,San Diego,CA,United States,2005,I,176-183.
[57]M.S.Arulampalam,S.Maskell,N.Gordon,and T.Clapp,A tutorial on particle filters for online nonlinear/non-Gaussian Bayesian tracking[J],IEEE Proceedings on Signal Processing,vol.50,no.2:174–188,2002.
[58]Djuric,P.M,Kotecha,J.H,Jianqui Zhang,Yufei Huang,Ghirmai,T,Bugallo,M.F,Miguez, J,Particle filtering[J].IEEE Signal Processing Magazine,2003Vol.20(No.5):19-38
[59]J.S.Liu and R.Chen,Sequential Monte Carlo methods for dynamic systems[J],Journal of the American Statistical Association,vol.93,no.443:1032–1044,1998.
[60]H.Bersini and F.Varela.Hints for Adaptive Problem Solving Gleaned from Immune Network.[J]In Parallel Problem Solving from Nature,H.P.Schwefel and H.M‘hlenbein(Eds.),Publisher-Springer-Verlag,1990:343–354
[61]De Castro,L.N.&Von Zuben,F.J.(Eds.).Recent Developments in Biologically Inspired Computing[M].Idear Group Inc.2004
[62]L.Booker,S.Forrest,M.Mitchell,and R.Riolo.Perspectives on Adaptation in Natural and Artificial Systems[M].Oxford University Press.2003
[63]L.N.de Castro and J.Timmis,Artificial Immune Systems:A New Computational Intelligence Approach[M],Springer 2002
[64]De CASTROLN,Von ZUBEN F J.learning and optimization using the clonal selection principle[J].IEEE Tran on Evolutionary computation,special Issue on Artificial Immune systems,2002.6.(3):239-251.
[65]R.Douc,O.Cappe,and E.Moulines,Comparison of resampling schemes for particlefiltering[C],Proc.of Image and Signal Processing and Analysis(ISPA),Sept.2005,pp.64–69.
[66]Jeroen D.Hol,Thomas B.Sch?n and Fredrik Gustafsson,On Resampling Algorithms for Particle Filters[C].Nonlinear Statistical Signal Processing Workshop,Cambridge,United Kingdom,September
 2006.
[67]叶其孝，沈永欢.实用数学手册[M],科学出版社，2006.1.
[68]张贤达，现代信号处理[M],清华大学出版社，第二版，2002年.
[69]张贤达，时间序列分析―高阶统计量方法[M]，清华大学出版社，1996
[70]V.M.Zolotarev.One-dimensional Stable Distributions.Amer.Math.Soc.Tkansl.of Math.Monographs[M],Vol.65.Amer.Math.Soc.Providence,R.I.(Transl.of the original1983 Russian).
[71]W.Feller.An Introduction to Probability Theory and its Applications[M].Vol.2,2nd edn. Wiley,New York,1971.
[72]G.Samorodnitsky and M.Taqqu.Stable Non-Gaussian Random Processes:Stochastic Models with Infinite Variance[M].Chapman and Hall,NewYork,London,1994.
[73]M.Shao,C.L.Nikias.Signal processing with fractional lower order moments:stable processes and their applications[C].Proceedings of the IEEE,July 1993,81(7):986-1010.
[74]J.M.Chambers,C.L.Mallows and B.W.Stuck,A Method for Simulating Stable Random Variables[J],JASA,1976,Vol.71,No.354:340-344
[75]Aleksander Weron and Rafal Weron,Computer Simulation of Levy alpha-Stable Variables and Processes[J],Lec.Notes in Physics,1995,457:379-392
