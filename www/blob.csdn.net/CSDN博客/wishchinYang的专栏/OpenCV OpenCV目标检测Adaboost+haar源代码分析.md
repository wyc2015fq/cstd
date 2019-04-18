# OpenCV:OpenCV目标检测Adaboost+haar源代码分析 - wishchinYang的专栏 - CSDN博客
2017年03月21日 15:44:44[wishchin](https://me.csdn.net/wishchin)阅读数：1855
       使用OpenCV作图像检测， Adaboost+haar决策过程，其中一部分源代码如下：
       函数调用堆栈的底层为：
1、使用有序决策桩进行预测
```cpp
template<class FEval>
inline int predictOrderedStump( CascadeClassifier& cascade, Ptr<FeatureEvaluator> &_featureEvaluator, double& sum )
{
    int nodeOfs = 0, leafOfs = 0;
    FEval& featureEvaluator = (FEval&)*_featureEvaluator;
    float* cascadeLeaves = &cascade.data.leaves[0];
    CascadeClassifier::Data::DTreeNode* cascadeNodes = &cascade.data.nodes[0];
    CascadeClassifier::Data::Stage* cascadeStages = &cascade.data.stages[0];
    //每一层进行计算，第一次训练为19层 nstages=19
    //
   int nstages = (int)cascade.data.stages.size();
    for( int stageIdx = 0; stageIdx < nstages; stageIdx++ )
    {
        CascadeClassifier::Data::Stage& stage = cascadeStages[stageIdx];
        sum = 0.0;
        //每一层树的个数
        int ntrees = stage.ntrees;
        for( int i = 0; i < ntrees; i++, nodeOfs++, leafOfs+= 2 )
        {
            CascadeClassifier::Data::DTreeNode& node = cascadeNodes[nodeOfs];
	    //收集累积和//没有显示否定的特性？
            double value = featureEvaluator(node.featureIdx);
            sum += cascadeLeaves[ value < node.threshold ? leafOfs : leafOfs + 1 ];
        }
        if( sum < stage.threshold )
            return -stageIdx;
    }
    return 1;
}
```
2.上层调用：在某个点之处进行计算
```cpp
int CascadeClassifier::runAt( Ptr<FeatureEvaluator>& evaluator, Point pt, double& weight )
{
    CV_Assert( oldCascade.empty() );
    assert( data.featureType == FeatureEvaluator::HAAR ||
            data.featureType == FeatureEvaluator::LBP ||
            data.featureType == FeatureEvaluator::HOG );
    if( !evaluator->setWindow(pt) )
        return -1;
    if( data.isStumpBased )
    {
        //若使用haar特征，则进行haar检测过程 wishchin 
       if( data.featureType == FeatureEvaluator::HAAR )
            return predictOrderedStump<HaarEvaluator>( *this, evaluator, weight );
        else if( data.featureType == FeatureEvaluator::LBP )
            return predictCategoricalStump<LBPEvaluator>( *this, evaluator, weight );
        else if( data.featureType == FeatureEvaluator::HOG )
            return predictOrderedStump<HOGEvaluator>( *this, evaluator, weight );
        else
            return -2;
    }
    else
    {
        if( data.featureType == FeatureEvaluator::HAAR )
            return predictOrdered<HaarEvaluator>( *this, evaluator, weight );
        else if( data.featureType == FeatureEvaluator::LBP )
            return predictCategorical<LBPEvaluator>( *this, evaluator, weight );
        else if( data.featureType == FeatureEvaluator::HOG )
            return predictOrdered<HOGEvaluator>( *this, evaluator, weight );
        else
            return -2;
    }
}
```
3. CascadeClassifierInvoker初始化时产生的 CascadeClassifier，
其中 **每个inVoker继承于并行循环的body**：例如 class CascadeClassifier : public ParallelLoopBody，完成并行计算过程
        其初始化过程，完成检测。
```cpp
void operator()(const Range& range) const
    {
        Ptr<FeatureEvaluator> evaluator = classifier->featureEvaluator->clone();
        Size winSize(cvRound(classifier->data.origWinSize.width * scalingFactor), cvRound(classifier->data.origWinSize.height * scalingFactor));
        int y1 = range.start * stripSize;
        int y2 = min(range.end * stripSize, processingRectSize.height);
        for( int y = y1; y < y2; y += yStep )
        {
            for( int x = 0; x < processingRectSize.width; x += yStep )
            {
                if ( (!mask.empty()) && (mask.at<uchar>(Point(x,y))==0)) {
                    continue;
                }
                double gypWeight;
		//作为起始点检测图像是否为目标！！！ wishchin 2017 03 20
                int result = classifier->runAt(evaluator, Point(x, y), gypWeight);
#if defined (LOG_CASCADE_STATISTIC)
                logger.setPoint(Point(x, y), result);
#endif
                if( rejectLevels )
                {
                    if( result == 1 )
                        result =  -(int)classifier->data.stages.size();
                    if( classifier->data.stages.size() + result < 4 )
                    {
                        mtx->lock();
                        rectangles->push_back(Rect(cvRound(x*scalingFactor), cvRound(y*scalingFactor), winSize.width, winSize.height));
                        rejectLevels->push_back(-result);
                        levelWeights->push_back(gypWeight);
                        mtx->unlock();
                    }
                }
                else if( result > 0 )
                {
                    mtx->lock();
                    rectangles->push_back(Rect(cvRound(x*scalingFactor), cvRound(y*scalingFactor),
                                               winSize.width, winSize.height));
                    mtx->unlock();
                }
                if( result == 0 )
                    x += yStep;
            }
        }
    }
    CascadeClassifier* classifier;
    vector<Rect>* rectangles;
    Size processingRectSize;
    int stripSize, yStep;
    double scalingFactor;
    vector<int> *rejectLevels;
    vector<double> *levelWeights;
    Mat mask;
    Mutex* mtx;
};
```
4.使用多尺度计算过程，对每一层进行单层结果计算
```cpp
bool CascadeClassifier::detectSingleScale( const Mat& image, int stripCount, Size processingRectSize,
                                           int stripSize, int yStep, double factor, vector<Rect>& candidates,
                                           vector<int>& levels, vector<double>& weights, bool outputRejectLevels )
{
    if( !featureEvaluator->setImage( image, data.origWinSize ) )
        return false;
#if defined (LOG_CASCADE_STATISTIC)
    logger.setImage(image);
#endif
    Mat currentMask;
    if (!maskGenerator.empty()) {
        currentMask=maskGenerator->generateMask(image);
    }
    vector<Rect> candidatesVector;
    vector<int>      rejectLevels;
    vector<double>   levelWeights;
    Mutex mtx;
    if( outputRejectLevels )
    {
        parallel_for_(Range(0, stripCount), CascadeClassifierInvoker( *this, processingRectSize, stripSize, yStep, factor,
            candidatesVector, rejectLevels, levelWeights, true, currentMask, &mtx));
        levels.insert( levels.end(), rejectLevels.begin(), rejectLevels.end() );
        weights.insert( weights.end(), levelWeights.begin(), levelWeights.end() );
    }
    else
    {
        //并行处理过程，对每一层初始化一个CascadeClassifierInvoker，完成计算
       parallel_for_(Range(0, stripCount), CascadeClassifierInvoker( *this, processingRectSize, stripSize, yStep, factor,
            candidatesVector, rejectLevels, levelWeights, false, currentMask, &mtx));
    }
    candidates.insert( candidates.end(), candidatesVector.begin(), candidatesVector.end() );
#if defined (LOG_CASCADE_STATISTIC)
    logger.write();
#endif
    return true;
}
```
5. 进行多尺度检测
```cpp
void CascadeClassifier::detectMultiScale( const Mat& image, vector<Rect>& objects,
                                          vector<int>& rejectLevels,
                                          vector<double>& levelWeights,
                                          double scaleFactor, int minNeighbors,
                                          int flags, Size minObjectSize, Size maxObjectSize,
                                          bool outputRejectLevels )
{
    const double GROUP_EPS = 0.2;
    CV_Assert( scaleFactor > 1 && image.depth() == CV_8U );
    if( empty() )
        return;
    if( isOldFormatCascade() )
    {
        MemStorage storage(cvCreateMemStorage(0));
        CvMat _image = image;
        CvSeq* _objects = cvHaarDetectObjectsForROC( &_image, oldCascade, storage, rejectLevels, levelWeights, scaleFactor,
                                              minNeighbors, flags, minObjectSize, maxObjectSize, outputRejectLevels );
        vector<CvAvgComp> vecAvgComp;
        Seq<CvAvgComp>(_objects).copyTo(vecAvgComp);
        objects.resize(vecAvgComp.size());
        std::transform(vecAvgComp.begin(), vecAvgComp.end(), objects.begin(), getRect());
        return;
    }
    objects.clear();
    if (!maskGenerator.empty()) {
        maskGenerator->initializeMask(image);
    }
    if( maxObjectSize.height == 0 || maxObjectSize.width == 0 )
        maxObjectSize = image.size();
    Mat grayImage = image;
    if( grayImage.channels() > 1 )
    {
        Mat temp;
        cvtColor(grayImage, temp, CV_BGR2GRAY);
        grayImage = temp;
    }
    Mat imageBuffer(image.rows + 1, image.cols + 1, CV_8U);
    vector<Rect> candidates;
    for( double factor = 1; ; factor *= scaleFactor )
    {
        Size originalWindowSize = getOriginalWindowSize();
        Size windowSize( cvRound(originalWindowSize.width*factor), cvRound(originalWindowSize.height*factor) );
        Size scaledImageSize( cvRound( grayImage.cols/factor ), cvRound( grayImage.rows/factor ) );
        Size processingRectSize( scaledImageSize.width - originalWindowSize.width, scaledImageSize.height - originalWindowSize.height );
        if( processingRectSize.width <= 0 || processingRectSize.height <= 0 )
            break;
        if( windowSize.width > maxObjectSize.width || windowSize.height > maxObjectSize.height )
            break;
        if( windowSize.width < minObjectSize.width || windowSize.height < minObjectSize.height )
            continue;
        Mat scaledImage( scaledImageSize, CV_8U, imageBuffer.data );
        resize( grayImage, scaledImage, scaledImageSize, 0, 0, CV_INTER_LINEAR );
        int yStep;
        if( getFeatureType() == cv::FeatureEvaluator::HOG )
        {
            yStep = 4;
        }
        else
        {
            yStep = factor > 2. ? 1 : 2;
        }
        int stripCount, stripSize;
        const int PTS_PER_THREAD = 1000;
        stripCount = ((processingRectSize.width/yStep)*(processingRectSize.height + yStep-1)/yStep + PTS_PER_THREAD/2)/PTS_PER_THREAD;
        stripCount = std::min(std::max(stripCount, 1), 100);
        stripSize = (((processingRectSize.height + stripCount - 1)/stripCount + yStep-1)/yStep)*yStep;
        //对每一个尺度进行目标检测 wishchin 2017 03 21 
        if( !detectSingleScale( scaledImage, stripCount, processingRectSize, stripSize, yStep, factor, candidates,
            rejectLevels, levelWeights, outputRejectLevels ) )
            break;
    }
    objects.resize(candidates.size());
    std::copy(candidates.begin(), candidates.end(), objects.begin());
    if( outputRejectLevels )
    {
        groupRectangles( objects, rejectLevels, levelWeights, minNeighbors, GROUP_EPS );
    }
    else
    {
        groupRectangles( objects, minNeighbors, GROUP_EPS );
    }
}
```
以上为objectDetect过程的OpenCV的源代码，外层调用的使用函数接口可以为：
```cpp
// 人眼检测
    m_cascade.detectMultiScale(
        smallImg,
        eyes,
        fAdaBoostScale, // originally 1.1, 4 is faster 
        2, //minNeighbors
        //|CV_HAAR_FIND_BIGGEST_OBJECT
        //|CV_HAAR_DO_ROUGH_SEARCH
        CV_HAAR_DO_CANNY_PRUNING,
        Size(48, 32)
        );
    //cout << "eyes size=:" << eyes.size() << endl;
```
**总结：**
        上述过程即是Haar+Adaboost检测计算大致的函数调用堆栈。
