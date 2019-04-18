# OpenCV：使用OpenCV3随机森林进行统计特征多类分析 - wishchinYang的专栏 - CSDN博客
2017年11月29日 11:00:47[wishchin](https://me.csdn.net/wishchin)阅读数：1201
         原文链接：[在opencv3中的机器学习算法练习：对OCR进行分类](http://www.cnblogs.com/denny402/p/5032839.html)
         本文贴出的代码为自己的训练集所用，作为参考。可运行demo程序请拜访原作者。
         CNN作为图像识别和检测器，在分析物体结构分布的多类识别中具有绝对的优势。通多多层卷积核Pooling实现对物体表面分布的模板学习，以卷积核的形式存储在网络中。而对于统计特征，暂时没有明确的指导规则。
         opencv3中的ml类与opencv2中发生了变化，下面列举opencv3的机器学习类方法实例，以随机森林为例。
代码：
```cpp
//使用OpenCV随机森林训练模型//使用训练好的样本-TXT文件
	int RTreesTrain( int argc, char* argv[] )
	{
		if (argc < 9) {
			std::cout << "argc<9";
			return 0;
		}
		std::string fileFeatureTrain(argv[1]);
		std::string  fileFeatureTest(argv[2]);
		std::string        fileTrees(argv[3]);
		int lenF   = atoi(argv[4]);//特征长度 32
		int numF = atoi(argv[5]);//使用特征个数 1000
		int nsample = atoi(argv[6]);//总样本数 大于numF
		int nTrees = atoi(argv[7]);
		int nClass = atoi(argv[8]);
		//载入特征
		cv::Mat data;
		cv::Mat responses;
		const string data_filename = fileFeatureTrain;
		read_num_class_data( data_filename, numF, lenF, &data, &responses );
		cv::Ptr<cv::ml::RTrees>  StyleModelHSV;
		StyleModelHSV = cv::ml::RTrees::create();
		StyleModelHSV->setMaxDepth(10);
		StyleModelHSV->setMinSampleCount(10);
		StyleModelHSV->setRegressionAccuracy(0);
		StyleModelHSV->setUseSurrogates(false);
		StyleModelHSV->setMaxCategories(nClass);
		StyleModelHSV->setPriors(cv::Mat());
		StyleModelHSV->setCalculateVarImportance(true);
		StyleModelHSV->setActiveVarCount(4);
		StyleModelHSV->setTermCriteria(TC(10000, 0.01f));
		int nsamples_all = nsample;// data.rows;
		int ntrain_samples = numF;// (int)(nsamples_all*0.8);
		cv::Ptr<cv::ml::TrainData> tdata = prepare_train_data(data, responses, ntrain_samples);
		cout << "The Model is training....." << endl;
		StyleModelHSV->train(tdata);
		StyleModelHSV->save(fileTrees);
		return 1;
	}
```
```cpp
// 读取文件数据
	bool read_num_class_data( const string& fileFeatureTrain, int numF,int fLen, cv::Mat* _data, cv::Mat* _responses)
	{
		using namespace cv;
		Mat el_ptr(1, numF, CV_32F);
		vector<int>  responses(0);
		_data->release();
		_responses->release();
		freopen(fileFeatureTrain.c_str(), "r", stdin);
		cout << "The feature is loading....." << endl;
		int i = 0;
		int label = 0;
		for (int i = 0; i < numF; ++i) {
			StyleFeature  aFeat;aFeat.second.resize(fLen);
			std::string sline;getline(cin, sline);
			//以空格分开
			int idxBlank = sline.find_first_of(" ");
			std::string sLabel = sline;//获取标签；
			sLabel.erase(idxBlank, sLabel.length());
			responses.push_back(label);//aFeat.first = label = atoi(sLabel.c_str());
			std::string sFV = sline;
			sFV.erase(0, idxBlank + 1);//获取一行，特征
			int idxFv = 0;
			float fV = 0.0;
			while (sFV.length() > 0 && idxFv < fLen) {
				int idxColon = sFV.find_first_of(":");
				std::string sv = sFV;
				std::strstream ssv;
				sv = sv.substr(idxColon + 1, sv.find_first_of(" ") - 2);
				ssv << sv;ssv >> fV;
				el_ptr.at<float>(i) = fV;//aFeat.second[idxFv] = fV;
				++idxFv;
				sFV.erase(0, sFV.find_first_of(" ") + 1);
			}
			_data->push_back(el_ptr);//trainData.push_back(aFeat);
		}
		fclose(stdin); cout << "The feature load over....." << endl;
		Mat(responses).copyTo(*_responses);
		return true;
	}
```
```cpp
//准备训练数据
	cv::Ptr<cv::ml::TrainData> prepare_train_data( const cv::Mat& data, const cv::Mat& responses, int ntrain_samples )
	{
		using namespace cv;
		Mat sample_idx = Mat::zeros(1, data.rows, CV_8U);
		Mat train_samples = sample_idx.colRange(0, ntrain_samples);
		train_samples.setTo(Scalar::all(1));
		int nvars = data.cols;
		Mat var_type(nvars + 1, 1, CV_8U);
		var_type.setTo(Scalar::all(ml::VAR_ORDERED));
		var_type.at<uchar>(nvars) = ml::VAR_CATEGORICAL;
		return ml::TrainData::create(data, ml::ROW_SAMPLE, responses, noArray(), sample_idx, noArray(), var_type);
	}
```
样本结构：
```
0 1:211946 2:0 3:0 4:0 5:105 6:5693 7:34 8:0 9:0 10:0 11:25 12:12697 13:226916 14:1826 15:497 16:282 17:105 18:15 19:104 20:18 21:0 22:737 23:46979 24:17889 25:7121 26:6970 27:9441 28:12679 29:20890 30:37498 31:43568 32:27465 
0 1:23544 2:210 3:11663 4:158 5:310 6:166 7:591 8:6131 9:193297 10:1985 11:1136 12:809 13:149069 14:33036 15:20045 16:11525 17:6552 18:2928 19:2590 20:1844 21:1305 22:11106 23:81817 24:29063 25:6654 26:5015 27:4916 28:8862 29:34762 30:44044 31:17409 32:7458 
0 1:254596 2:0 3:65361 4:0 5:0 6:0 7:0 8:0 9:0 10:0 11:10 12:14033 13:333347 14:330 15:75 16:80 17:25 18:0 19:42 20:0 21:0 22:101 23:31990 24:66583 25:49191 26:59149 27:35800 28:25089 29:21463 30:18022 31:18409 32:8304 
0 1:11697 2:2431 3:228 4:9 5:0 6:1 7:150 8:28 9:8413 10:9673 11:6345 12:6025 13:7695 14:8080 15:5689 16:6175 17:5146 18:4358 19:3246 20:2170 21:1478 22:963 23:2192 24:6866 25:7082 26:4273 27:3100 28:2733 29:2833 30:3265 31:3835 32:8821
```

