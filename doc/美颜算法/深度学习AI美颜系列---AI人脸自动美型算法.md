# 深度学习AI美颜系列---AI人脸自动美型算法

置顶

 

2018年09月13日 13:17:05

 

Trent1985

 

阅读数：50072

更多

所属专栏： [SF图像滤镜/美颜/美妆算法详解与实战](https://blog.csdn.net/column/details/25028.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/82686434

​    人脸智能美型技术主要用于智能美颜，对用户的照片进行自动智能调整，而不需要用户手工调整，该技术在美颜相机、天天P图等app中都已应用。

本文在这里对人脸智能美型做个详解。

人脸智能美型包含如下两个部分：

**①人脸轮廓自动调整**

**②五官自动修正**

人脸轮廓自动修正：对人脸大小，胖瘦进行自动调整，目前app中常用的瘦脸只是其中一个特例而已；

五官自动修正：包含眼睛大小自动调整，鼻子形状位置修正，眉毛位置修正以及嘴巴形状、大小和位置自动修正等等。App中常用的大眼和立体修鼻功能，也属于其中一个特例；

人脸智能美型和磨皮美白结合使用，就是所谓的智能美颜。

**人脸智能美型算法逻辑**如下：

1，构建平均脸

针对男女分别构建正脸平均脸，如下图所示：

![img](https://img-blog.csdn.net/2018091313124225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2，性别识别

这一步需要将用户的人像照片进行性别识别，根据识别结果分别选择男/女平均脸数据。

性别识别可以参考本人博客：[性别识别](https://blog.csdn.net/trent1985/article/details/80253642)

核心代码如下：

```python
def weight_variable(shape,name):
    return tf.Variable(tf.truncated_normal(shape, stddev = 0.1),name=name)
 
def bias_variable(shape,name):
    return tf.Variable(tf.constant(0.1, shape = shape),name=name)
 
def conv2d(x,w,padding="SAME"):
    if padding=="SAME" :
        return tf.nn.conv2d(x, w, strides = [1,1,1,1], padding = "SAME")
    else:
        return tf.nn.conv2d(x, w, strides = [1,1,1,1], padding = "VALID")
    
def max_pool(x, kSize, Strides):
    return tf.nn.max_pool(x, ksize = [1,kSize,kSize,1],strides = [1,Strides,Strides,1], padding = "SAME")    
 
def compute_cost(Z3, Y):
    """
    Computes the cost
    
    Arguments:
    Z3 -- output of forward propagation (output of the last LINEAR unit), of shape (6, number of examples)
    Y -- "true" labels vector placeholder, same shape as Z3
    
    Returns:
    cost - Tensor of the cost function
    """
    cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=Z3, labels=Y))    
    return cost
 
def initialize_parameters():
    tf.set_random_seed(1)
    W1 = tf.cast(weight_variable([5,5,1,32],"W1"), dtype = tf.float32)
    b1 = tf.cast(bias_variable([32],"b1"), dtype = tf.float32)
    W2 = tf.cast(weight_variable([5,5,32,64],"W2"), dtype = tf.float32)
    b2 = tf.cast(bias_variable([64],"b2"), dtype = tf.float32)
    W3 = tf.cast(weight_variable([5,5,64,128],"W3"), dtype = tf.float32)
    b3 = tf.cast(bias_variable([128],"b3"), dtype = tf.float32)
    
    W4 = tf.cast(weight_variable([14*12*128,500],"W4"), dtype = tf.float32)
    b4 = tf.cast(bias_variable([500],"b4"), dtype = tf.float32)
    W5 = tf.cast(weight_variable([500,500],"W5"), dtype = tf.float32)
    b5 = tf.cast(bias_variable([500],"b5"), dtype = tf.float32)
    W6 = tf.cast(weight_variable([500,2],"W6"), dtype = tf.float32)
    b6 = tf.cast(bias_variable([2],"b6"), dtype = tf.float32)
    parameters = {"W1":W1,
                 "b1":b1,
                 "W2":W2,
                 "b2":b2,
                 "W3":W3,
                 "b3":b3,
                 "W4":W4,
                 "b4":b4,
                 "W5":W5,
                 "b5":b5,
                 "W6":W6,
                 "b6":b6}
    return parameters
 
def cnn_net(x, parameters, keep_prob = 1.0):
    #frist convolution layer
    w_conv1 = parameters["W1"]
    b_conv1 = parameters["b1"]
    h_conv1 = tf.nn.relu(conv2d(x,w_conv1) + b_conv1)  #output size 112x92x32
    h_pool1 = max_pool(h_conv1,2,2)    #output size 56x46x32
    
    #second convolution layer
    w_conv2 = parameters["W2"]
    b_conv2 = parameters["b2"]
    h_conv2 = tf.nn.relu(conv2d(h_pool1, w_conv2) + b_conv2) #output size 56x46x64
    h_pool2 = max_pool(h_conv2,2,2) #output size 28x23x64
    
    #third convolution layer
    w_conv3 = parameters["W3"]
    b_conv3 = parameters["b3"]
    h_conv3 = tf.nn.relu(conv2d(h_pool2,w_conv3) + b_conv3) #output size 28x23x128
    h_pool3 = max_pool(h_conv3,2,2) #output size 14x12x128
    
    #full convolution layer 
    w_fc1 = parameters["W4"]
    b_fc1 = parameters["b4"]
    h_fc11 = tf.reshape(h_pool3,[-1,14*12*128])
    h_fc1 = tf.nn.relu(tf.matmul(h_fc11,w_fc1) + b_fc1)
    
    w_fc2 = parameters["W5"]
    b_fc2 = parameters["b5"]
    h_fc2 = tf.nn.relu(tf.matmul(h_fc1,w_fc2)+b_fc2)
    h_fc2_drop = tf.nn.dropout(h_fc2,keep_prob)
    
    w_fc3 = parameters["W6"]
    b_fc3 = parameters["b6"]
    y_conv = tf.matmul(h_fc2_drop, w_fc3) + b_fc3
    #y_conv = tf.nn.softmax(tf.matmul(h_fc2_drop, w_fc3) + b_fc3)
    #rmse = tf.sqrt(tf.reduce_mean(tf.square(y_ - y_conv)))
    #cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels = y, logits = y_conv))
    #train_step = tf.train.GradientDescentOptimizer(0.001).minimize(cross_entropy)
    #correct_prediction  = tf.equal(tf.argmax(y_conv, 1), tf.argmax(y,1))
    #accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
```

3，将用户照片人脸映射到平均脸

这一步主要根据用户照片人脸关键点和平均脸的人脸关键点，加上一定的映射算法将用户照片对齐到平均脸中，可以参考仿射变换等。

关键点可以使用商汤科技或者FACE++等人脸SDK。

效果如下图所示：

![img](https://img-blog.csdn.net/20180913131427456?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4，计算用户人脸和平均脸的距离D

此处计算规则可以使用欧氏距离等，距离D表示用户人脸到完美人脸的差。

距离的计算还需要参考人脸的旋转角度信息，根据人脸旋转角度对距离进行加权处理，以此来适应各种角度的用户人脸照片。

5，根据D对用户人脸进行不同程度的变形，得到智能美型结果

此处变形可以使用MLS、三角网格变形等等。

MLS详细算法与代码，可参考博客：[MLS变形](https://blog.csdn.net/Trent1985/article/details/80667611)

MLS核心代码如下：

```cpp
static void setSrcPoints(const vector<PointD> &qsrc, vector<PointD> &newDotL, int* nPoint) {
    *nPoint = qsrc.size();
    newDotL.clear();
    newDotL.reserve(*nPoint);
    for (size_t i = 0; i < qsrc.size(); i++) 
        newDotL.push_back(qsrc[i]);
}
 
static void setDstPoints(const vector<PointD> &qdst,vector<PointD> &oldDotL, int* nPoint) {
    *nPoint = qdst.size();
    oldDotL.clear();
    oldDotL.reserve(*nPoint);
 
    for (size_t i = 0; i < qdst.size(); i++) oldDotL.push_back(qdst[i]);
}
static double bilinear_interp(double x, double y, double v11, double v12,
                              double v21, double v22) {
    return (v11 * (1 - y) + v12 * y) * (1 - x) + (v21 * (1 - y) + v22 * y) * x;
}
 
static double calcArea(const vector<PointD> &V) {
    PointD lt, rb;
    lt.x = lt.y = 1e10;
    rb.x = rb.y = -1e10;
    for (vector<PointD >::const_iterator i = V.begin(); i != V.end();
         i++) {
        if (i->x < lt.x) lt.x = i->x;
        if (i->x > rb.x) rb.x = i->x;
        if (i->y < lt.y) lt.y = i->y;
        if (i->y > rb.y) rb.y = i->y;
    }
    return (rb.x - lt.x) * (rb.y - lt.y);
}
static void calcDelta_rigid(int srcW, int srcH, int tarW, int tarH, double alpha, int gridSize, int nPoint, int preScale, double *rDx, double *rDy, vector<PointD> &oldDotL, vector<PointD> &newDotL)
{
    int i, j, k;
    PointD swq, qstar, newP, tmpP;
    double sw;
 
    double ratio;
 
    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) {
			newDotL[i].x *= 1 / ratio;
			newDotL[i].y *= 1 / ratio;
		}
    }
    double *w = new double[nPoint];
 
    if (nPoint < 2) {
        //rDx.setTo(0);
        //rDy.setTo(0);
        return;
    }
    PointD swp, pstar, curV, curVJ, Pi, PiJ, Qi;
    double miu_r;
 
    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                if (alpha == 1)
                    w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                (j - oldDotL[k].y) * (j - oldDotL[k].y));
                else
                    w[k] = pow((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                   (j - oldDotL[k].y) * (j - oldDotL[k].y),
                               -alpha);
                sw = sw + w[k];
                swp.x = swp.x + w[k] * oldDotL[k].x;
				swp.y = swp.y + w[k] * oldDotL[k].y;
                swq.x = swq.x + w[k] * newDotL[k].x;
                swq.y = swq.y + w[k] * newDotL[k].y;
            }
            if (k == nPoint) {
				pstar.x = (1 / sw) * swp.x;
				pstar.y = (1 / sw) * swp.y;
                qstar.x = 1 / sw * swq.x;
				qstar.y = 1 / sw * swq.y;
                // Calc miu_r
                double s1 = 0, s2 = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
					Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
					Qi.x = newDotL[k].x - qstar.x;
					Qi.y = newDotL[k].y - qstar.y;
					s1 += w[k] * (Qi.x*Pi.x+Qi.y*Pi.y);
                    s2 += w[k] * (Qi.x*PiJ.x+Qi.y*PiJ.y);
                }
                miu_r = sqrt(s1 * s1 + s2 * s2);
				curV.x -= pstar.x;
				curV.y -= pstar.y;
 
                curVJ.x = -curV.y, curVJ.y = curV.x;
 
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
					 Pi.x = oldDotL[k].x - pstar.x;
					  Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
					 tmpP.x = (Pi.x*curV.x+Pi.y*curV.y)* newDotL[k].x -
                             (PiJ.x*curV.x+PiJ.y*curV.y)* newDotL[k].y;
                    tmpP.y = -(Pi.x*curVJ.x+Pi.y*curVJ.y) * newDotL[k].x +
                             (PiJ.x*curVJ.x+PiJ.y*curVJ.y) * newDotL[k].y;
                    tmpP.x *= w[k] / miu_r;
					tmpP.y *= w[k] / miu_r;
                    newP.x += tmpP.x;
					newP.y += tmpP.y;
                }
				newP.x += qstar.x;
				newP.y += qstar.y;
            } else {
                newP = newDotL[k];
            }
 
            if (preScale) {
                rDx[j * tarW + i] = newP.x * ratio - i;
                rDy[j * tarW + i] = newP.y * ratio - j;
            } else {
				rDx[j * tarW + i] = newP.x - i;
				rDy[j * tarW + i] = newP.y - j;
            }
        }
    }
    delete[] w;
 
    if (preScale!=0) {
        for (i = 0; i < nPoint; i++){
			newDotL[i].x *= ratio;
			newDotL[i].y *= ratio;
		}
    }
}
static void calcDelta_Similarity(int srcW, int srcH, int tarW, int tarH, double alpha, int gridSize, int nPoint, int preScale, double *rDx, double *rDy, vector<PointD> &oldDotL, vector<PointD> &newDotL)
{
    int i, j, k;
 
    PointD swq, qstar, newP, tmpP;
    double sw;
	
    double ratio;
 
    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) {
			newDotL[i].x *= 1 / ratio;
			newDotL[i].y *= 1 / ratio;
		}
    }
    double *w = new double[nPoint];
 
    if (nPoint < 2) {
        return;
    }
 
    PointD swp, pstar, curV, curVJ, Pi, PiJ;
    double miu_s;
 
    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                            (j - oldDotL[k].y) * (j - oldDotL[k].y));
                sw = sw + w[k];
                swp.x = swp.x + w[k] * oldDotL[k].x;
				swp.y = swp.y + w[k] * oldDotL[k].y;
                swq.x = swq.x + w[k] * newDotL[k].x;
				swq.y = swq.y + w[k] * newDotL[k].y;
            }
            if (k == nPoint) {
                pstar.x = (1 / sw) * swp.x;
				pstar.y = (1 / sw) * swp.y;
                qstar.x = 1 / sw * swq.x;
				qstar.y = 1 / sw * swq.y;
                // Calc miu_s
                miu_s = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
 
                    Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    miu_s += w[k] * (Pi.x*Pi.x+Pi.y*Pi.y);
                }
 
                curV.x -= pstar.x;
				curV.y -= pstar.y;
                curVJ.x = -curV.y, curVJ.y = curV.x;
 
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;
 
                    Pi.x = oldDotL[k].x - pstar.x;
					Pi.y = oldDotL[k].y - pstar.y;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
 
					tmpP.x = (Pi.x*curV.x+Pi.y*curV.y) * newDotL[k].x -
                             (PiJ.x*curV.x+PiJ.y*curV.y) * newDotL[k].y;
					tmpP.y = -(Pi.x*curVJ.x+Pi.y*curVJ.y) * newDotL[k].x +
                             (PiJ.x*curVJ.x+PiJ.y*curVJ.y) * newDotL[k].y;
                    tmpP.x *= w[k] / miu_s;
					tmpP.y *= w[k] / miu_s;
                    newP.x += tmpP.x;
					newP.y += tmpP.y;
                }
                newP.x += qstar.x;
				newP.y += qstar.y;
            } else {
                newP = newDotL[k];
            }
 
            rDx[j * tarW + i] = newP.x - i;
            rDy[j * tarW + i] = newP.y - j;
        }
    }
 
    delete[] w;
	    if (preScale!=0) {
        for (i = 0; i < nPoint; i++){
			newDotL[i].x *= ratio;
			newDotL[i].y *= ratio;
		}
    }
}
static int GetNewImg(unsigned char* oriImg, int width, int height, int stride, unsigned char* tarImg, int tarW, int tarH, int tarStride, int gridSize, double* rDx, double* rDy, double transRatio)
{
    int i, j;
    double di, dj;
    double nx, ny;
    int nxi, nyi, nxi1, nyi1;
    double deltaX, deltaY;
    double w, h;
    int ni, nj;
	int pos, posa, posb, posc, posd;
    for (i = 0; i < tarH; i += gridSize)
        for (j = 0; j < tarW; j += gridSize) {
            ni = i + gridSize, nj = j + gridSize;
            w = h = gridSize;
            if (ni >= tarH) ni = tarH - 1, h = ni - i + 1;
            if (nj >= tarW) nj = tarW - 1, w = nj - j + 1;
            for (di = 0; di < h; di++)
                for (dj = 0; dj < w; dj++) {
                    deltaX =
                        bilinear_interp(di / h, dj / w, rDx[i * tarW + j], rDx[i * tarW + nj],
                                        rDx[ni * tarW + j], rDx[ni * tarW + nj]);
                    deltaY =
                        bilinear_interp(di / h, dj / w, rDy[i * tarW + j], rDy[i * tarW + nj],
                                        rDy[ni * tarW + j], rDy[ni * tarW + nj]);
                    nx = j + dj + deltaX * transRatio;
                    ny = i + di + deltaY * transRatio;
                    if (nx > width - 1) nx = width - 1;
                    if (ny > height - 1) ny = height - 1;
                    if (nx < 0) nx = 0;
                    if (ny < 0) ny = 0;
                    nxi = int(nx);
                    nyi = int(ny);
                    nxi1 = ceil(nx);
                    nyi1 = ceil(ny);
					pos = (int)(i + di) * tarStride + ((int)(j + dj) << 2);
					posa = nyi * stride + (nxi << 2);
					posb = nyi * stride + (nxi1 << 2);
					posc = nyi1 * stride + (nxi << 2);
					posd = nyi1 * stride + (nxi1 << 2);
					tarImg[pos]     = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa],    oriImg[posb],   oriImg[posc],   oriImg[posd]);
					tarImg[pos + 1] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 1],oriImg[posb + 1], oriImg[posc + 1], oriImg[posd + 1]);
					tarImg[pos + 2] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 2],oriImg[posb + 2], oriImg[posc + 2], oriImg[posd + 2]);
					tarImg[pos + 3] = (unsigned char)bilinear_interp(ny - nyi, nx - nxi, oriImg[posa + 3],oriImg[posb + 3], oriImg[posc + 3], oriImg[posd + 3]);
                }
        }
		return 0;
};
 
static void MLSImageWrapping(unsigned char* oriImg,int width, int height, int stride,const vector<PointD > &qsrc, const vector<PointD > &qdst, unsigned char* tarImg, int outW, int outH, int outStride, double transRatio, int preScale, int gridSize, int method)
{
	int srcW = width;
	int srcH = height;
	int tarW = outW;
    int tarH = outH;
    double alpha = 1;
	int nPoint;
	int len = tarH * tarW;
	vector<PointD> oldDotL, newDotL;
	double *rDx = NULL,*rDy = NULL;
    setSrcPoints(qsrc,newDotL,&nPoint);
    setDstPoints(qdst,oldDotL,&nPoint);
    rDx = (double*)malloc(sizeof(double) * len);
    rDy = (double*)malloc(sizeof(double) * len);
    memset(rDx, 0, sizeof(double) * len);
    memset(rDy, 0, sizeof(double) * len);
	if(method!=0)
		calcDelta_Similarity(srcW, srcH, tarW, tarH, alpha, gridSize, nPoint, preScale, rDx, rDy, oldDotL, newDotL);
	else
	    calcDelta_rigid(srcW, srcH, tarW, tarH, alpha, gridSize, nPoint, preScale, rDx, rDy, oldDotL, newDotL);
	GetNewImg(oriImg, srcW, srcH, stride, tarImg, tarW, tarH, outStride, gridSize, rDx, rDy, transRatio);
	if(rDx != NULL)
		free(rDx);
	if(rDy != NULL)
		free(rDy);
};
int f_TMLSImagewarpping(unsigned char* srcData, int width ,int height, int stride, unsigned char* dstData, int outW, int outH, int outStride, int srcPoint[], int dragPoint[], int pointNum, double intensity, int preScale, int gridSize, int method)
{
	int res = 0;
    vector<PointD> qDst;
    vector<PointD> qSrc;
	PointD point = {0};
	int len = 0;
	for(int i = 0; i < pointNum; i++)
	{
		len = (i << 1);
		point.x = srcPoint[len];
		point.y = srcPoint[len + 1];
		qSrc.push_back(point);
		point.x = dragPoint[len];
		point.y = dragPoint[len + 1];
		qDst.push_back(point);
	}
	MLSImageWrapping(srcData, width, height, stride, qSrc, qDst, dstData, outW, outH, outStride, intensity, preScale,gridSize, method);
	return res;
};

```

上述过程就是人脸自动美型的算法逻辑，对于不同的人像照片，会自动判断大眼矫正或者小眼矫正，瘦脸或者胖脸等等，综合达到一个相对完美的结果。

**自动美型效果**如下图所示：

![img](https://img-blog.csdn.net/20180913131613258?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RyZW50MTk4NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后，本人QQ1358009172，微信公众号：SF图像算法，欢迎学习交流！