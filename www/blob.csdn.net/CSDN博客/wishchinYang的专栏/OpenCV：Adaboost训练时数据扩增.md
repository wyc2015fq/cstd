# OpenCV：Adaboost训练时数据扩增 - wishchinYang的专栏 - CSDN博客
2017年04月06日 10:12:56[wishchin](https://me.csdn.net/wishchin)阅读数：674
        更准确的模型需要更多的数据，对于传统非神经网络机器学习方法，不同的特征需要有各自相符合的数据扩增方法。
1.   在使用opencv_traincascade.exe 过程中，图像读取在
         classifier.train ->  updateTrainingSet( requiredLeafFARate, tempLeafFARate )->fillPassedSamples( 0, numPos, true, 0, posConsumed )->imgReader.getPos( img ) :  imgReader.getNeg( img )过程中。
2. 在进行数据增强的过程中，从createSample开始，保证vecFile和样本数目长度一致。
修改代码段依次为：
```cpp
//int cvCreateTrainingSamplesFromInfoRf(const char* infoname, const char* vecfilename,
int cvCreateSamplesPlus(const char* infoname, const char* vecfilename, int num,
	int showsamples,
	int winwidth, int winheight)
{
	CvEnhanseData      enhanser;
    char fullname[PATH_MAX];
    char* filename;
    FILE* info;
    FILE* vec;
    //IplImage* src = 0;
    //IplImage* sample;
    cv::Mat src, sample;
    int line;
    int error;
    int i;
    int x, y, width, height;
    int total;
    assert(infoname != NULL);
    assert(vecfilename != NULL);
    total = 0;
    if (!icvMkDir(vecfilename))
    {
#if CV_VERBOSE
        fprintf(stderr, "Unable to create directory hierarchy: %s\n", vecfilename);
#endif /* CV_VERBOSE */
        return total;
    }
    info = fopen(infoname, "r");
    if (info == NULL)
    {
#if CV_VERBOSE
        fprintf(stderr, "Unable to open file: %s\n", infoname);
#endif /* CV_VERBOSE */
        return total;
    }
    vec = fopen(vecfilename, "wb");
    if (vec == NULL)
    {
#if CV_VERBOSE
        fprintf(stderr, "Unable to open file: %s\n", vecfilename);
#endif /* CV_VERBOSE */
        fclose(info);
        return total;
    }
    //sample = cvCreateImage(cvSize(winwidth, winheight), IPL_DEPTH_8U, 1);
    icvWriteVecHeader(vec, num, winwidth, winheight);
    if (showsamples)
    {
        cvNamedWindow("Sample", CV_WINDOW_AUTOSIZE);
    }
    strcpy(fullname, infoname);
    filename = strrchr(fullname, '\\');
    if (filename == NULL)
    {
        filename = strrchr(fullname, '/');
    }
    if (filename == NULL)
    {
        filename = fullname;
    }
    else
    {
        filename++;
    }
    for (line = 1, error = 0, total = 0; total < num; line++)
    {
        int count;
        error = (fscanf(info, "%s %d", filename, &count) != 2);
        if (!error)
        {
            //src = cvLoadImage(fullname, 0);
            //error = (src == NULL);
            src = cv::imread(fullname, 0);
            error = (src.data == NULL);
            if (error)
            {
#if CV_VERBOSE
                fprintf(stderr, "Unable to open image: %s\n", fullname);
#endif /* CV_VERBOSE */
            }
        }
        for (i = 0; (i < count) && (total < num); i++, total++)
        {
            error = (fscanf(info, "%d %d %d %d", &x, &y, &width, &height) != 4);
            if (error) break;
            //cvSetImageROI(src, cvRect(x, y, width, height));
            //cvResize(src, sample, width >= sample->width &&height >= sample->height ? CV_INTER_AREA : CV_INTER_LINEAR);
            cv::resize(src, sample, cv::Size(winwidth,winheight));
            //if (showsamples)
            //{
            //    cvShowImage("Sample", sample);
            //    if (cvWaitKey(0) == 27)
            //    {
            //        showsamples = 0;
            //    }
            //}
            //icvWriteVecSample(vec, sample);
            {
                int extNum = 7;
                //IplImage* sample2 = cvCreateImage(cvGetSize(sample), IPL_DEPTH_8U, sample->nChannels);
                //cvCopyImage(sample, sample2);
                std::vector<cv::Mat > imgLIst(extNum);
                cv::Mat inMat(sample);//此句导致占用释放错误
                enhanser.EnhanceData(inMat, extNum, 1, imgLIst);
                for (int i = 0; i < extNum; ++i)
                {//把Mat 生成移除来//已不必要
                    //IplImage* sampleT = nullptr;// = nullptr;
                    //*sampleT = IplImage(imgLIst[i]);
                    //icvWriteVecSample(vec, sampleT);
                    icvWriteVecSamplePlus(vec,imgLIst[i]);
                    //if (sampleT)
                    //{
                    //    cvReleaseImage(&sampleT);
                    //}
                }
            }
        }
        //if (src)
        //{
        //    cvReleaseImage(&src);
        //}
        if (error)
        {
#if CV_VERBOSE
            fprintf(stderr, "%s(%d) : parse error", infoname, line);
#endif /* CV_VERBOSE */
            break;
        }
    }
    //if (sample)
    //{
    //    cvReleaseImage(&sample);
    //}
    fclose(vec);
    fclose(info);
    return total;
}
```
修改函数：void icvWriteVecSample
```cpp
//对每个图像写入正样本Vec
void icvWriteVecSamplePlus(FILE* file, cv::Mat &sample)
{
	//CvMat* mat, stub;
	int r, c;
	short tmp;
	uchar chartmp;
	//mat = cvGetMat(sample, &stub);
	chartmp = 0;
	fwrite(&chartmp, sizeof(chartmp), 1, file);
	for (r = 0; r < sample.rows; r++)
	{
		for (c = 0; c < sample.cols; c++)
		{
			//tmp = (short)(CV_MAT_ELEM(*mat, uchar, r, c));
			tmp = (short)(sample.at<unsigned char>(r,c));
			fwrite(&tmp, sizeof(tmp), 1, file);
		}
	}
}
```
使用C++语言替换掉使用C语言的版本。
同时对生成新的Vec增加int extNum = 7;倍。
