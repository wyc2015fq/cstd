# k-means算法及opencv实现 - 建建的博客 - CSDN博客
2018年09月20日 16:54:22[纪建](https://me.csdn.net/u013898698)阅读数：53
K-means算法是最为经典的基于划分的聚类方法，是十大经典数据挖掘算法之一。K-means算法的基本思想是：以空间中k个点为中心进行聚类，对最靠近他们的对象归类。通过迭代的方法，逐次更新各聚类中心的值，直至得到最好的聚类结果。
假设要把样本集分为c个类别，算法描述如下：
(1)适当选择c个类的初始中心;
(2) 在第k次迭代中，对任意一样本，求其到c个中心的距离，将其归类到距离最短的中心所在的类；
(3) 利用均值等方法更新该类的中心值;
(4) 重复步骤(2)(3)，直到聚类中心保持不变。
该算法的最大优势在于简洁和快速，算法的关键在于初始中心的选择和距离公式。[百度百科]
K-means在opencv中的实现：
- 
`void kmeans_mat(const Mat& src_img,Mat& dst_img)`
- 
`{`
- 
`int width_src=src_img.cols;`
- 
`int height_src=src_img.rows;`
- 
- 
`Mat samples=Mat::zeros(width_src*height_src,1,CV_32FC3);//创建样本矩阵，CV_32FC3代表32位浮点3通道（彩色图像）`
- 
`Mat clusters;//类别标记矩阵`
- 
`int k=0;`
- 
`for (int i=0;i<height_src;i++)`
- 
`{`
- 
`for (int j=0;j<width_src;j++,k++)`
- 
`{`
- 
`//将像素点三通道的值按顺序排入样本矩阵`
- 
`samples.at<Vec3f>(k,0)[0]=(float)src_img.at<Vec3b>(i,j)[0];`
- 
`samples.at<Vec3f>(k,0)[1]=(float)src_img.at<Vec3b>(i,j)[1];`
- 
`samples.at<Vec3f>(k,0)[2]=(float)src_img.at<Vec3b>(i,j)[2];`
- 
`}`
- 
`}`
- 
`int nCuster=2;//聚类类别数，自己修改。`
- 
`//聚类，KMEANS PP CENTERS Use kmeans++ center initialization by Arthur and Vassilvitskii`
- 
`kmeans(samples,nCuster,clusters,TermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER,10,1.0),2,KMEANS_PP_CENTERS);`
- 
- 
`//显示聚类结果`
- 
`if (dst_img.empty())`
- 
`{`
- 
`dst_img=Mat::zeros(height_src,width_src,CV_8UC1);`
- 
`}`
- 
- 
`k=0;`
- 
`int val=0;`
- 
`float step=255/(nCuster-1);`
- 
`for (int i=0;i<height_src;i++)`
- 
`{`
- 
`for (int j=0;j<width_src;j++,k++)`
- 
`{`
- 
`val=255-clusters.at<int>(k,0)*step;//int`
- 
`dst_img.at<uchar>(i,j)=val;`
- 
`}`
- 
`}`
- 
`}`
 k-means聚类算法，初始中心的选择是关键，有时间还需要研究下KMEANS_PP_CENTERS Use kmeans++ center initialization by Arthur and Vassilvitskii方法。
