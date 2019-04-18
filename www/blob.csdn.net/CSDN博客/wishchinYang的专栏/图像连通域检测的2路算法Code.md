# 图像连通域检测的2路算法Code - wishchinYang的专栏 - CSDN博客
2016年04月11日 16:47:31[wishchin](https://me.csdn.net/wishchin)阅读数：1876
         本文算法描述参考链接：[http://blog.csdn.net/icvpr/article/details/10259577](http://blog.csdn.net/icvpr/article/details/10259577)
**两遍扫描法：**
（1）第一次扫描：
> 
访问当前像素B(x,y)，如果B(x,y) == 1：
> 
a、如果B(x,y)的领域中像素值都为0，则赋予B(x,y)一个新的label：
> 
> 
label += 1， B(x,y) = label；
> 
b、如果B(x,y)的领域中有像素值 > 1的像素Neighbors：
> 
1）将Neighbors中的最小值赋予给B(x,y):
> 
> 
B(x,y) = min{Neighbors}
2）记录Neighbors中各个值（label）之间的相等关系，即这些值（label）同属同一个连通区域；
> 
 labelSet[i] = { label_m, .., label_n }，labelSet[i]中的所有label都属于同一个连通区域（注：这里可以有多种实现方式，只要能够记录这些具有相等关系的label之间的关系即可）
（2）第二次扫描：
> 
访问当前像素B(x,y)，如果B(x,y) > 1：
> 
a、找到与label = B(x,y)同属相等关系的一个最小label值，赋予给B(x,y)；
         完成扫描后，图像中具有相同label值的像素就组成了同一个连通区域。
![](https://img-blog.csdn.net/20130825153036000)
**算法代码：**
从直觉上看，二路法比堆栈法要快，其实速度只有堆栈法的1/5 - 1/10,代码如下：
```cpp
//使用两遍扫描法//查找所有的连通域
//简单方法，使用1-0矩阵//使用列标记的方法，收集所有的连通域
//需要使用倒排索引
bool CD2DetectInPic::searchConBy2Way(
	const cv::Mat& _binImg, 
	float valueForeB, float valueForeUp,
	std::vector<std::vector<cv::Point > > &foreAreas)
{
	int vFore = 255;
	int vBack =   0;
	foreAreas.resize(0);
	cv::Mat _lableImg;
	if (_binImg.channels()>1)
	{
		cv::cvtColor(_binImg,_lableImg,cv::COLOR_BGR2GRAY);
	}
	else
	{
		_binImg.copyTo(_lableImg);
	}
	//一遍扫描，得出前景和背景点，进行标记
	//背景点标记为0，前景点标记为1
#ifdef SHOW_TEMP
	cv::imshow("",_lableImg);//cv::waitKey(0);
#endif
	IplImage imageLabel = _lableImg;
	for (int i=0;i< imageLabel.height;++i)
	{
		char* pI = (char*)imageLabel.imageData + i * imageLabel.widthStep;
		for (int j=0;j<imageLabel.width;++j )
		{
			if ( *pI >=valueForeB &&*pI <=valueForeUp)//避开单一值失误！
			{
				*pI = vFore;
			} 
			else
			{
				*pI = vBack;
			}
			++pI;
		}
	}
	//对label图像进行遍历，寻找连通域//对Mark矩阵，进行修改，不修改标识矩阵
	cv::Mat imageMark(&imageLabel);
	cv::Mat imageMarkRe = imageMark.clone();
	//使用线扫描的方法进行分离合并
	//使用set集合表示相等关系
#ifdef SHOW_TEMP
	cv::imshow("imageMarkRe",imageMarkRe);//
	cv::waitKey(1);
#endif
	std::stack<std::pair<int,int> > neighborPixels;  
	//使用另外一个Label矩阵，用于记载当前点属于哪个连通域
	cv::Mat conAreaLocMat = cv::Mat::zeros(_binImg.rows,_binImg.cols,CV_8UC1);
	cv::bitwise_not(conAreaLocMat,conAreaLocMat);
	IplImage imageConAreaLoc = conAreaLocMat;
	std::vector<std::pair<cv::Point,uchar> >  conAreas(imageMark.cols);
	std::vector<std::vector<cv::Point> >  conPointsAreas(0);
	//第n个连通域
	int counter =0;//用于标记为第N个连通域
	int idx =0;//用于哈希索引
	//设置重复集合//用于最后合并连通域
	//std::vector<std::vector<int> >  overlapVec(0);
	//std::set<int,int>   overlapSet;
	std::multimap<int,int>  overlapMap;//使用多值哈希
	
	//对第一行进行连通域划分
	{
		char* pLabel = (char*)imageLabel.imageData + 0* imageLabel.widthStep;
		char* pLoc   = (char*)imageConAreaLoc.imageData + 0* imageConAreaLoc.widthStep;
		int lastL = *pLabel;
		if (vFore == lastL)
		{
			++counter;
			*pLoc = counter;//计数从1开始，索引从0开始
			//++idx;
			std::pair<int,int> p(counter,idx);
			overlapMap.insert(p);//插入索引
		} 
		else
		{ 
			*pLoc = -1;
		}
		int lastLoc = *pLoc;
		++pLabel;
		++pLoc;
		for (int n=1; n< imageMark.cols; ++n)
		{
			if (vFore == *pLabel )//若标记为连通域，则进行标记
			{
				if ( *pLabel==lastL )//若和上一个像素联通，则标记为上一个的标记
				{
					*pLoc = lastLoc;
				} 
				else
				{//若上一个为0，则增加标记符号，为新的标记
					++counter;
					*pLoc = counter;
					++idx;
					std::pair<int,int> p(counter,idx);
					overlapMap.insert(p);//插入索引
					//std::cout<< idx << "_" <<counter<<std::endl;
				}
			}
			else
			{
				*pLoc = -1;//若遇到第n个点为变化点，标记为第n个连通域的位置
			}
			lastL = *pLabel;
			lastLoc = *pLoc;
			++pLoc;
			++pLabel;
		}
	}
	//对图片每一行进行寻找连通域，必须遍历
	for (int m =1; m<imageMark.rows; ++m )
	{
		//对第一个像素进行指派
		//上一行的指针
		char* pLabelFore = (char*)imageLabel.imageData + (m-1)* imageLabel.widthStep;
		char* pLocFore   = (char*)imageConAreaLoc.imageData + (m-1)* imageConAreaLoc.widthStep;
		//这一行的指针
		char* pLabel = (char*)imageLabel.imageData + m* imageLabel.widthStep;
		char* pLoc   = (char*)imageConAreaLoc.imageData + m* imageConAreaLoc.widthStep;
		//标记第一个点，以上一行为准
		int foreL   =  *pLabelFore;
		int foreLoc =    *pLocFore;
		int lastL   =   *pLabel;
		if (*pLabel == foreL)//若和上一行的值相等，则连通域位置和上一行的第一个值相等
		{
			*pLoc = *pLocFore;
		} 
		else
		{
			if ( vFore == *pLabel)
			{
				//若为连通域，则增加标记符号，为新的标记
				++counter;
				*pLoc = counter;
				++idx;
				std::pair<int,int> p(counter,idx);
				overlapMap.insert(p);//插入索引
				//std::cout<< idx << "_" <<counter<<std::endl;
			} 
			else
			{
				*pLoc = -1;
			}
		}
		int lastLoc = *pLoc;
		for (int n=0; n< imageMark.cols-1; ++n)
		{
			//寻找每一行的连通域
			lastL   = *pLabel;//上一个像素的标记
			lastLoc = *pLoc;//上一个像素的连通域位置
			//到下一个像素
			++pLabel;
			++pLoc;
			++pLabelFore;
			++pLocFore;
			foreL   = *pLabelFore;//上一行此位置像素的值
			foreLoc = *pLocFore;//上一个像素的连通域位置
			if (vBack == *pLabel )//若当前label为0
			{
				*pLoc = -1;
			}
			else
			{
				if (*pLabel != foreL )//若当前label不等于上一行像素的label
				{
					if (*pLabel != lastL )
					{//若当前label不等于上一个像素的label，则表示为新的连通域起始，新建集合
						//若为连通域，则增加标记符号，为新的标记
						++counter;
						*pLoc = counter;
						++idx;
						std::pair<int,int> p(counter,idx);
						overlapMap.insert(p);//插入索引
						//std::cout<< idx << "_" <<counter<<std::endl;
					} 
					else
					{//若当前label等于上一个像素的label，则为连通，直接赋值
						*pLoc = lastLoc;
					}
				}
				else
				{//若等于前一行像素，就麻烦了！
					if (*pLabel != lastL)//若不等于上一个的像素label，则赋值为上一行的标记
					{
						*pLoc = foreLoc;
					} 
					else
					{
						if (foreLoc == lastLoc)//若前一个和前一行相等
						{
							*pLoc = foreLoc;
						} 
						else
						{//若前一个和前一行不相等
							//若等于前一个像素，需要合并集合//标记相同集合
							*pLoc = foreLoc;
							//合并哈希表
							//查找到值所在的哈希索引
							int idxF = ( *overlapMap.find(foreLoc) ).first;
							int idxFL =( *overlapMap.find(lastLoc) ).first;//map寻找的为key，而不是值
							int L1 = ( *overlapMap.find(foreLoc) ).second;
							int L2 = ( *overlapMap.find(lastLoc) ).second;
							
							if (L1 != L2)//若不在一个索引中，可以删除
							{
								//将当前值插入到索引之中，不增加idx索引
								//必须先删除再插入，以免误删除
								//去除掉旧的 键值 对
								//overlapMap.erase(idxFL);//一个参数为key，是错误的
								//int x = overlapMap.erase(lastLoc);
								overlapMap.erase(idxFL);
								//std::pair<int,int>  p(idxF,lastLoc);
								std::pair<int,int>  p(lastLoc,L1);
								overlapMap.insert(p);
							} 
							else
							{
							}
						}
						
					}
				}
					
			}
		}
	}
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	int MileTs = sys.wSecond;int MileT = sys.wMilliseconds;
	//直接查找到位置
	conPointsAreas.resize(overlapMap.size());
	IplImage imageConArea= imageConAreaLoc;
	for (int i=0;i< imageConArea.height;++i)
	{
		char* pI = (char*)imageConArea.imageData + i * imageConArea.widthStep;
		for (int j=0;j<imageConArea.width;++j )
		{
			if (*pI >0)
			{
				cv::Point p(j,i);
				int idx = (*overlapMap.find((int)*pI)).second-1;
				conPointsAreas[idx ].push_back(p);
			}
			++pI;
		}
	}
	int validVec =0;
	foreAreas.resize(validVec);
	for (int i=0;i< conPointsAreas.size();++i)
	{
		if (conPointsAreas[i].size() >0 )
		{
			++validVec;
			foreAreas.push_back(conPointsAreas[i]);
		}
	}
	GetLocalTime( &sys ); 
	int MileT2 = sys.wMilliseconds;
	int  DetaT = MileT2  - MileT;
	std::cout<< std::endl;
	std::cout<< "The CopyVec time is :"<< DetaT<<"mS..........."<< std::endl;
	std::cout<< std::endl;
	conPointsAreas.resize(0);
	return true;
}
```
