
# TLD（Tracking-Learning-Detection）学习与源码理解之（三） - zouxy09的专栏 - CSDN博客


2012年08月21日 20:18:27[zouxy09](https://me.csdn.net/zouxy09)阅读数：46546


**TLD****（****Tracking-Learning-Detection****）学习与源码理解之（三）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
下面是自己在看论文和这些大牛的分析过程中，对代码进行了一些理解，但是由于自己接触图像处理和机器视觉没多久，另外由于自己编程能力比较弱，所以分析过程可能会有不少的错误，希望各位不吝指正。而且，因为编程很多地方不懂，所以注释得非常乱，还海涵。
从main()函数切入，分析整个TLD运行过程如下：
（这里只是分析工作过程，全部注释的代码见博客的更新）
1、分析程序运行的命令行参数；
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt –r
2、读入初始化参数（程序中变量）的文件parameters.yml；
3、通过文件或者用户鼠标框选的方式指定要跟踪的目标的Bounding Box；
4、用上面得到的包含要跟踪目标的Bounding  Box和第一帧图像去初始化TLD系统，
tld.init(last_gray, box, bb_file); 初始化包含的工作如下：
4.1、buildGrid(frame1, box);
检测器采用扫描窗口的策略：扫描窗口步长为宽高的 10%，尺度缩放系数为1.2；此函数构建全部的扫描窗口grid，并计算每一个扫描窗口与输入的目标box的重叠度；重叠度定义为两个box的交集与它们的并集的比；
4.2、为各种变量或者容器分配内存空间；
4.3、getOverlappingBoxes(box, num_closest_init);
此函数根据传入的box（目标边界框），在整帧图像中的全部扫描窗口中（由上面4.1得到）寻找与该box距离最小（即最相似，重叠度最大）的num_closest_init（10）个窗口，然后把这些窗口归入good_boxes容器。同时，把重叠度小于0.2的，归入bad_boxes容器；相当于对全部的扫描窗口进行筛选。并通过BBhull函数得到这些扫描窗口的最大边界。
4.5、classifier.prepare(scales);
准备分类器，scales容器里是所有扫描窗口的尺度，由上面的buildGrid()函数初始化；
TLD的分类器有三部分：方差分类器模块、集合分类器模块和最近邻分类器模块；这三个分类器是级联的，每一个扫描窗口依次全部通过上面三个分类器，才被认为含有前景目标。这里prepare这个函数主要是初始化集合分类器模块；
集合分类器（随机森林）基于n个基本分类器（共10棵树），每个分类器（树）都是基于一个pixel comparisons（共13个像素比较集）的，也就是说每棵树有13个判断节点（组成一个pixel comparisons），输入的图像片与每一个判断节点（相应像素点）进行比较，产生0或者1，然后将这13个0或者1连成一个13位的二进制码x（有2^13种可能），每一个x对应一个后验概率P(y|x)= \#p/(\#p+\#n) （也有2^13种可能），\#p和\#n分别是正和负图像片的数目。那么整一个集合分类器（共10个基本分类器）就有10个后验概率了，将10个后验概率进行平均，如果大于阈值（一开始设经验值0.65，后面再训练优化）的话，就认为该图像片含有前景目标；
后验概率P(y|x)= \#p/(\#p+\#n)的产生方法：初始化时，每个后验概率都得初始化为0；运行时候以下面方式更新：将已知类别标签的样本（训练样本）通过n个分类器进行分类，如果分类结果错误，那么相应的\#p和\#n就会更新，这样P(y|x)也相应更新了。
pixel comparisons的产生方法：先用一个归一化的patch去离散化像素空间，产生所有可能的垂直和水平的pixel comparisons，然后我们把这些pixel comparisons随机分配给n个分类器，每个分类器得到完全不同的pixel comparisons（特征集合），这样，所有分类器的特征组统一起来就可以覆盖整个patch了。
特征是相对于一种尺度的矩形框而言的，TLD中第s种尺度的第i个特征features[s][i] = Feature(x1, y1, x2, y2);是两个随机分配的像素点坐标（就是由这两个像素点比较得到0或者1的）。每一种尺度的扫描窗口都含有totalFeatures = nstructs * structSize个特征；nstructs为树木（由一个特征组构建，每组特征代表图像块的不同视图表示）的个数；structSize为每棵树的特征个数，也即每棵树的判断节点个数；树上每一个特征都作为一个决策节点；
prepare函数的工作就是先给每一个扫描窗口初始化了对应的pixel comparisons（两个随机分配的像素点坐标）；然后初始化后验概率为0；
4.6、generatePositiveData(frame1, num_warps_init);
此函数通过对第一帧图像的目标框box（用户指定的要跟踪的目标）进行仿射变换来合成训练初始分类器的正样本集。具体方法如下：先在距离初始的目标框最近的扫描窗口内选择10个bounding box（已经由上面的getOverlappingBoxes函数得到，存于good_boxes里面了，还记得不？），然后在每个bounding box的内部，进行±1%范围的偏移，±1%范围的尺度变化，±10%范围的平面内旋转，并且在每个像素上增加方差为5的高斯噪声（确切的大小是在指定的范围内随机选择的），那么每个box都进行20次这种几何变换，那么10个box将产生200个仿射变换的bounding box，作为正样本。具体实现如下：
getPattern(frame(best_box), pEx, mean, stdev);此函数将frame图像best_box区域的图像片归一化为均值为0的15*15大小的patch，存于pEx（用于最近邻分类器的正样本）正样本中（最近邻的box的Pattern），该正样本只有一个。
generator(frame, pt, warped, bbhull.size(), rng);此函数属于PatchGenerator类的构造函数，用来对图像区域进行仿射变换，先RNG一个随机因子，再调用（）运算符产生一个变换后的正样本。
classifier.getFeatures(patch, grid[idx].sidx, fern);函数得到输入的patch的特征fern（13位的二进制代码）；
pX.push_back(make_pair(fern, 1));   //positive ferns <features, labels=1>然后标记为正样本，存入pX（用于集合分类器的正样本）正样本库；
以上的操作会循环 num_warps * good_boxes.size()即20 * 10 次，这样，pEx就有了一个正样本，而pX有了200个正样本了；
4.7、meanStdDev(frame1(best_box), mean, stdev);
统计best_box的均值和标准差，var = pow(stdev.val[0],2) * 0.5;作为方差分类器的阈值。
4.8、generateNegativeData(frame1);
由于TLD仅跟踪一个目标，所以我们确定了目标框了，故除目标框外的其他图像都是负样本，无需仿射变换；具体实现如下：
由于之前重叠度小于0.2的，都归入 bad_boxes了，所以数量挺多，把方差大于var*0.5f的bad_boxes都加入负样本，同上面一样，需要classifier.getFeatures(patch, grid[idx].sidx, fern);和nX.push_back(make_pair(fern, 0));得到对应的fern特征和标签的nX负样本（用于集合分类器的负样本）；
然后随机在上面的bad_boxes中取bad_patches（100个）个box，然后用 getPattern函数将frame图像bad_box区域的图像片归一化到15*15大小的patch，存在nEx（用于最近邻分类器的负样本）负样本中。
这样nEx和nX都有负样本了；（box的方差通过积分图像计算）
4.9、然后将nEx的一半作为训练集nEx，另一半作为测试集nExT；同样，nX也拆分为训练集nX和测试集nXT；
4.10、将负样本nX和正样本pX合并到ferns_data[]中，用于集合分类器的训练；
4.11、将上面得到的一个正样本pEx和nEx合并到nn_data[]中，用于最近邻分类器的训练；
4.12、用上面的样本训练集训练 集合分类器（森林） 和 最近邻分类器：
classifier.trainF(ferns_data, 2); //bootstrap = 2
对每一个样本ferns_data[i] ，如果样本是正样本标签，先用measure_forest函数返回该样本所有树的所有特征值对应的后验概率累加值，该累加值如果小于正样本阈值（0.6* nstructs，这就表示平均值需要大于0.6（0.6* nstructs / nstructs）,0.6是程序初始化时定的集合分类器的阈值，为经验值，后面会用测试集来评估修改，找到最优），也就是输入的是正样本，却被分类成负样本了，出现了分类错误，所以就把该样本添加到正样本库，同时用update函数更新后验概率。对于负样本，同样，如果出现负样本分类错误，就添加到负样本库。
classifier.trainNN(nn_data);
对每一个样本nn_data，如果标签是正样本，通过NNConf(nn_examples[i], isin, conf, dummy);计算输入图像片与在线模型之间的相关相似度conf，如果相关相似度小于0.65 ，则认为其不含有前景目标，也就是分类错误了；这时候就把它加到正样本库。然后就通过pEx.push_back(nn_examples[i]);将该样本添加到pEx正样本库中；同样，如果出现负样本分类错误，就添加到负样本库。
4.13、用测试集在上面得到的 集合分类器（森林） 和 最近邻分类器中分类，评价并修改得到最好的分类器阈值。
classifier.evaluateTh(nXT, nExT);
对集合分类器，对每一个测试集nXT，所有基本分类器的后验概率的平均值如果大于thr_fern（0.6），则认为含有前景目标，然后取最大的平均值（大于thr_fern）作为该集合分类器的新的阈值。
对最近邻分类器，对每一个测试集nExT，最大相关相似度如果大于nn_fern（0.65），则认为含有前景目标，然后取最大的最大相关相似度（大于nn_fern）作为该最近邻分类器的新的阈值。
5、进入一个循环：读入新的一帧，然后转换为灰度图像，然后再处理每一帧processFrame；
6、processFrame(last_gray, current_gray, pts1, pts2, pbox, status, tl, bb_file);逐帧读入图片序列，进行算法处理。processFrame共包含四个模块（依次处理）：跟踪模块、检测模块、综合模块和学习模块；
6.1、跟踪模块：track(img1, img2, points1,points2);
track函数完成前一帧img1的特征点points1到当前帧img2的特征点points2的跟踪预测；
6.1.1、具体实现过程如下：
（1）先在lastbox中均匀采样10*10=100个特征点（网格均匀撒点），存于points1：
bbPoints(points1, lastbox);
（2）利用金字塔LK光流法跟踪这些特征点，并预测当前帧的特征点（见下面的解释）、计算FB error和匹配相似度sim，然后筛选出 FB_error[i] <= median(FB_error) 和 sim_error[i] > median(sim_error) 的特征点（舍弃跟踪结果不好的特征点），剩下的是不到50%的特征点：
tracker.trackf2f(img1, img2, points, points2);
（3）利用剩下的这不到一半的跟踪点输入来预测bounding box在当前帧的位置和大小 tbb：
bbPredict(points, points2, lastbox, tbb);
（4）跟踪失败检测：如果FB error的中值大于10个像素（经验值），或者预测到的当前box的位置移出图像，则认为跟踪错误，此时不返回bounding box：
if (tracker.getFB()>10 || tbb.x>img2.cols ||  tbb.y>img2.rows || tbb.br().x < 1 || tbb.br().y <1)
（5）归一化img2(bb)对应的patch的size（放缩至patch_size = 15*15），存入pattern：
getPattern(img2(bb),pattern,mean,stdev);
（6）计算图像片pattern到在线模型M的保守相似度：
classifier.NNConf(pattern,isin,dummy,tconf);
（7）如果保守相似度大于阈值，则评估本次跟踪有效，否则跟踪无效：
if (tconf>classifier.thr_nn_valid) tvalid =true;
6.1.2、TLD跟踪模块的实现原理和trackf2f函数的实现：
TLD跟踪模块的实现是利用了Media Flow 中值光流跟踪和跟踪错误检测算法的结合。中值流跟踪方法是基于Forward-Backward Error和NNC的。原理很简单：从t时刻的图像的A点，跟踪到t+1时刻的图像B点；然后倒回来，从t+1时刻的图像的B点往回跟踪，假如跟踪到t时刻的图像的C点，这样就产生了前向和后向两个轨迹，比较t时刻中 A点和C点的距离，如果距离小于一个阈值，那么就认为前向跟踪是正确的；这个距离就是FB_error；
bool LKTracker::trackf2f(const Mat& img1, const Mat& img2, vector<Point2f> &points1, vector<cv::Point2f> &points2)
函数实现过程如下：
（1）先利用金字塔LK光流法跟踪预测前向轨迹：
calcOpticalFlowPyrLK( img1,img2, points1, points2, status, similarity, window_size, level, term_criteria, lambda, 0);
（2）再往回跟踪，产生后向轨迹：
calcOpticalFlowPyrLK( img2,img1, points2, pointsFB, FB_status,FB_error, window_size, level, term_criteria, lambda, 0);
（3）然后计算 FB-error：前向与 后向 轨迹的误差：
for( int i= 0; i<points1.size(); ++i )
FB_error[i] = norm(pointsFB[i]-points1[i]);
（4）再从前一帧和当前帧图像中（以每个特征点为中心）使用亚象素精度提取10x10象素矩形（使用函数getRectSubPix得到），匹配前一帧和当前帧中提取的10x10象素矩形，得到匹配后的映射图像（调用matchTemplate），得到每一个点的NCC相关系数（也就是相似度大小）。
normCrossCorrelation(img1, img2, points1, points2);
（5）然后筛选出 FB_error[i] <= median(FB_error) 和 sim_error[i] > median(sim_error) 的特征点（舍弃跟踪结果不好的特征点），剩下的是不到50%的特征点；
filterPts(points1, points2);
6.2、检测模块：detect(img2);
TLD的检测分类器有三部分：方差分类器模块、集合分类器模块和最近邻分类器模块；这三个分类器是级联的。当前帧img2的每一个扫描窗口依次通过上面三个分类器，全部通过才被认为含有前景目标。具体实现过程如下：
先计算img2的积分图，为了更快的计算方差：
integral(frame,iisum,iisqsum);
然后用高斯模糊，去噪：
GaussianBlur(frame,img,Size(9,9),1.5);
下一步就进入了方差检测模块：
6.2.1、方差分类器模块：getVar(grid[i],iisum,iisqsum) >= var
利用积分图计算每个待检测窗口的方差，方差大于var阈值（目标patch方差的50%）的，则认为其含有前景目标，通过该模块的进入集合分类器模块：
6.2.2、集合分类器模块：
集合分类器（随机森林）共有10颗树（基本分类器），每棵树13个判断节点，每个判断节点经比较得到一个二进制位0或者1，这样每棵树就对应得到一个13位的二进制码x（叶子），这个二进制码x对应于一个后验概率P(y|x)。那么整一个集合分类器（共10个基本分类器）就有10个后验概率了，将10个后验概率进行平均，如果大于阈值（一开始设经验值0.65，后面再训练优化）的话，就认为该图像片含有前景目标；具体过程如下：
（1）先得到该patch的特征值（13位的二进制代码）：
classifier.getFeatures(patch,grid[i].sidx,ferns);
（2）再计算该特征值对应的后验概率累加值：
conf = classifier.measure_forest(ferns);
（3）若集合分类器的后验概率的平均值大于阈值fern_th（由训练得到），就认为含有前景目标：
if (conf > numtrees * fern_th)  dt.bb.push_back(i);
（4）将通过以上两个检测模块的扫描窗口记录在detect structure中；
（5）如果顺利通过以上两个检测模块的扫描窗口数大于100个，则只取后验概率大的前100个；
nth_element(dt.bb.begin(), dt.bb.begin()+100, dt.bb.end(),
CComparator(tmp.conf));
进入最近邻分类器：
6.2.3、最近邻分类器模块
（1）先归一化patch的size（放缩至patch_size = 15*15），存入dt.patch[i];
getPattern(patch,dt.patch[i],mean,stdev);
（2）计算图像片pattern到在线模型M的相关相似度和保守相似度：
classifier.NNConf(dt.patch[i],dt.isin[i],dt.conf1[i],dt.conf2[i]);
（3）相关相似度大于阈值，则认为含有前景目标：
if (dt.conf1[i]>nn_th)dbb.push_back(grid[idx]);
到目前为止，检测器检测完成，全部通过三个检测模块的扫描窗口存在dbb中；
6.3、综合模块：
TLD只跟踪单目标，所以综合模块综合跟踪器跟踪到的单个目标和检测器可能检测到的多个目标，然后只输出保守相似度最大的一个目标。具体实现过程如下：
（1）先通过 重叠度 对检测器检测到的目标bounding box进行聚类，每个类的重叠度小于0.5：
clusterConf(dbb, dconf, cbb, cconf);
（2）再找到与跟踪器跟踪到的box距离比较远的类（检测器检测到的box），而且它的相关相似度比跟踪器的要大：记录满足上述条件，也就是可信度比较高的目标box的个数：
if (bbOverlap(tbb, cbb[i])<0.5 && cconf[i]>tconf) confident_detections++;
（3）判断如果只有一个满足上述条件的box，那么就用这个目标box来重新初始化跟踪器（也就是用检测器的结果去纠正跟踪器）：
if (confident_detections==1)  bbnext=cbb[didx];
（4）如果满足上述条件的box不只一个，那么就找到检测器检测到的box与跟踪器预测到的box距离很近（重叠度大于0.7）的所以box，对其坐标和大小进行累加：
if(bbOverlap(tbb,dbb[i])>0.7)  cx += dbb[i].x;……
（5）对与跟踪器预测到的box距离很近的box 和 跟踪器本身预测到的box 进行坐标与大小的平均作为最终的目标bounding box，但是跟踪器的权值较大：
bbnext.x = cvRound((float)(10*tbb.x+cx)/(float)(10+close_detections));……
（6）另外，如果跟踪器没有跟踪到目标，但是检测器检测到了一些可能的目标box，那么同样对其进行聚类，但只是简单的将聚类的cbb[0]作为新的跟踪目标box（不比较相似度了？？还是里面已经排好序了？？），重新初始化跟踪器：
bbnext=cbb[0];
至此，综合模块结束。
6.4、学习模块：learn(img2);
学习模块也分为如下四部分：
6.4.1、检查一致性：
（1）归一化img(bb)对应的patch的size（放缩至patch_size = 15*15），存入pattern：
getPattern(img(bb), pattern, mean, stdev);
（2）计算输入图像片（跟踪器的目标box）与在线模型之间的相关相似度conf：
classifier.NNConf(pattern,isin,conf,dummy);
（3）如果相似度太小了或者如果方差太小了或者如果被被识别为负样本，那么就不训练了；
if (conf<0.5)……或if (pow(stdev.val[0], 2)< var)……或if(isin[2]==1)……
6.4.2、生成样本：
先是集合分类器的样本：fern_examples：
（1）先计算所有的扫描窗口与目前的目标box的重叠度：
grid[i].overlap = bbOverlap(lastbox, grid[i]);
（2）再根据传入的lastbox，在整帧图像中的全部窗口中寻找与该lastbox距离最小（即最相似，重叠度最大）的num_closest_update个窗口，然后把这些窗口归入good_boxes容器（只是把网格数组的索引存入）同时，把重叠度小于0.2的，归入 bad_boxes 容器：
getOverlappingBoxes(lastbox, num_closest_update);
（3）然后用仿射模型产生正样本（类似于第一帧的方法，但只产生10*10=100个）：
generatePositiveData(img, num_warps_update);
（4）加入负样本，相似度大于1？？相似度不是出于0和1之间吗？
idx=bad_boxes[i];
if (tmp.conf[idx]>=1) fern_examples.push_back(make_pair(tmp.patt[idx],0));
然后是最近邻分类器的样本：nn_examples：
if (bbOverlap(lastbox,grid[idx]) < bad_overlap)
nn_examples.push_back(dt.patch[i]);
6.4.3、分类器训练：
classifier.trainF(fern_examples,2);
classifier.trainNN(nn_examples);
6.4.4、把正样本库（在线模型）包含的所有正样本显示在窗口上
classifier.show();
至此，tld.processFrame函数结束。
7、如果跟踪成功，则把相应的点和box画出来：
if (status){
drawPoints(frame,pts1);
drawPoints(frame,pts2,Scalar(0,255,0));  //当前的特征点用蓝色点表示
drawBox(frame,pbox);
detections++;
}
8、然后显示窗口和交换图像帧，进入下一帧的处理：
imshow("TLD", frame);
swap(last_gray, current_gray);
至此，main()函数结束（只分析了框架）。

