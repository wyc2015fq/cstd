# OpenCV SITF 特征提取 FeatureDetector对象函数detect运行报错解决方案 - 建建的博客 - CSDN博客
2018年09月20日 11:23:31[纪建](https://me.csdn.net/u013898698)阅读数：92
个人分类：[opencv](https://blog.csdn.net/u013898698/article/category/8002288)
本人在使用OpenCV SIFT特征提取算法时，遇到了问题，具体表现为 .exe触发了一个断点错误，经网上查找，发现是 vector 在析构时，造成了内存错误，解决方案由大神在csdn博客中给出，链接地址：
[点击打开链接](http://http//blog.csdn.net/lanbing510/article/details/40585789)
按照大神所提示的 ，在使用opencv 函数之前，先对 vector 进行手动分配内存，分配内存后问题解决；
个人声明：本人在VS2013下调用sift算法，已经将问题解决，当然，具体问题，具体分析，本人适用，但不一定适用所有人，也希望大家对该问题的解决方案进行及时补充；
代码：
- 
`initModule_nonfree();//初始化模块，使用SIFT或SURF时用到 `
- 
- 
`//FeatureDetector::create()函数的参数可选`
- 
`// • "FAST" – FastFeatureDetector`
- 
`// • "STAR" – StarFeatureDetector`
- 
`// • "SIFT" – SIFT(nonfree module)`
- 
`// • "SURF" – SURF(nonfree module)`
- 
`// • "ORB" – ORB`
- 
`// • "BRISK" – BRISK`
- 
`// • "MSER" – MSER`
- 
`// • "GFTT" – GoodFeaturesToTrackDetector`
- 
`// • "HARRIS" – GoodFeaturesToTrackDetector with Harris detector enabled`
- 
`// • "Dense" – DenseFeatureDetector`
- 
`// • "SimpleBlob" – SimpleBlobDetector`
- 
`Ptr<FeatureDetector> detector = FeatureDetector::create("SIFT");//创建SIFT特征检测器 `
- 
- 
`//SiftFeatureDetector detector;`
- 
`//Ptr<FeatureDetector> detector = FeatureDetector::create("SURF");`
- 
`// DescriptorExtractor::create()函数的参数如下：`
- 
`// `
- 
`// • "SIFT" – SIFT`
- 
`// • "SURF" – SURF`
- 
`// • "BRIEF" – BriefDescriptorExtractor`
- 
`// • "BRISK" – BRISK`
- 
`// • "ORB" – ORB`
- 
`// • "FREAK" – FREAK`
- 
`//SiftDescriptorExtractor descriptor_extractor;`
- 
`Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create("SIFT");//创建特征向量生成器 `
- 
`//Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create("SURF");`
- 
`// // DescriptorMatcher::create()函数的参数如下，对应不同的匹配算法：`
- 
`// // – BruteForce(it uses L2)`
- 
`// // – BruteForce - L1`
- 
`// // – BruteForce - Hamming`
- 
`// // – BruteForce - Hamming(2)`
- 
`// // – FlannBased`
- 
`Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create("BruteForce");//创建特征匹配器,暴力匹配(简单匹配) `
- 
`// if (detector.empty() || descriptor_extractor.empty())`
- 
`// cout << "fail to create detector!";`
- 
- 
`//读入图像 `
- 
`Mat img1 = imread("001.jpg", 0);`
- 
`Mat img2 = imread("004.bmp", 0);`
- 
- 
`if (img1.empty() || img1.channels() != 1)`
- 
`MessageBox(L"图像类型错误");`
- 
`//CV_Error(Error::StsBadArg, "image is empty or has incorrect depth (!=CV_8U)");`
- 
- 
`//if (!mask.empty() && mask.type() != CV_8UC1)`
- 
`//CV_Error(Error::StsBadArg, "mask has incorrect type (!=CV_8UC1)");`
- 
- 
`//特征点检测 `
- 
`double t = getTickCount();//当前滴答数 `
- 
`vector<KeyPoint> keypoints1, keypoints2;`
- 
`keypoints1.resize(100);`
- 
`keypoints2.resize(100);`
- 
`detector->detect(img1, keypoints1);//检测img1中的SIFT特征点，存储到keypoints1中 `
- 
`detector->detect(img2, keypoints2);`
- 
`//detector.detect(img1, keypoints1);`
- 
`//detector.detect(img2, keypoints2);`
- 
`// cout << "图像1特征点个数:" << keypoints1.size() << endl;`
- 
`// cout << "图像2特征点个数:" << keypoints2.size() << endl;`
- 
`// `
- 
`// //根据特征点计算特征描述子矩阵，即特征向量矩阵 `
- 
`Mat descriptors1, descriptors2;`
- 
`descriptor_extractor->compute(img1, keypoints1, descriptors1);`
- 
`descriptor_extractor->compute(img2, keypoints2, descriptors2);`
- 
`// t = ((double)getTickCount() - t) / getTickFrequency();`
- 
`// cout << "SIFT算法用时：" << t << "秒" << endl;`
- 
`// `
- 
`// `
- 
`// cout << "图像1特征描述矩阵大小：" << descriptors1.size()`
- 
`// << "，特征向量个数：" << descriptors1.rows << "，维数：" << descriptors1.cols << endl;`
- 
`// cout << "图像2特征描述矩阵大小：" << descriptors2.size()`
- 
`// << "，特征向量个数：" << descriptors2.rows << "，维数：" << descriptors2.cols << endl;`
- 
`// `
- 
`// //画出特征点 `
- 
`Mat img_keypoints1, img_keypoints2;`
- 
`drawKeypoints(img1, keypoints1, img_keypoints1, Scalar::all(-1), 0);`
- 
`drawKeypoints(img2, keypoints2, img_keypoints2, Scalar::all(-1), 0);`
- 
`imshow("Src1",img_keypoints1); `
- 
`imshow("Src2",img_keypoints2); `
- 
`waitKey(0);`
- 
- 
`//特征匹配 `
- 
`vector<DMatch> matches;//匹配结果 `
- 
`matches.resize(100);`
- 
`descriptor_matcher->match(descriptors1, descriptors2, matches);//匹配两个图像的特征矩阵 `
- 
`cout << "Match个数：" << matches.size() << endl;`
- 
- 
`//计算匹配结果中距离的最大和最小值 `
- 
`//距离是指两个特征向量间的欧式距离，表明两个特征的差异，值越小表明两个特征点越接近 `
- 
`double max_dist = 0;`
- 
`double min_dist = 100;`
- 
`for (int i = 0; i < matches.size(); i++)`
- 
`{`
- 
`double dist = matches[i].distance;`
- 
`if (dist < min_dist) min_dist = dist;`
- 
`if (dist > max_dist) max_dist = dist;`
- 
`}`
- 
`cout << "最大距离：" << max_dist << endl;`
- 
`cout << "最小距离：" << min_dist << endl;`
- 
- 
`//筛选出较好的匹配点 `
- 
`vector<DMatch> goodMatches;`
- 
`for (int i = 0; i < matches.size(); i++)`
- 
`{`
- 
`if (matches[i].distance < 0.5 * max_dist)`
- 
`{`
- 
`goodMatches.push_back(matches[i]);`
- 
`}`
- 
`}`
- 
`cout << "goodMatch个数：" << goodMatches.size() << endl;`
- 
- 
`//画出匹配结果 `
- 
`Mat img_matches;`
- 
`//红色连接的是匹配的特征点对，绿色是未匹配的特征点 `
- 
`drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, img_matches,`
- 
`/*Scalar::all(-1)/ **/CV_RGB(255,0,0), CV_RGB(0, 255, 0), Mat(), 2);`
- 
- 
`namedWindow("MatchSIFT", 0);`
- 
`imshow("MatchSIFT", img_matches);`
- 
`waitKey(0);`
