# RANSAC算法及其消除错配应用 - 深之JohnChen的专栏 - CSDN博客

2010年08月11日 10:58:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：11963


**RANSAC算法及其消除错配应用**

**一、RANSAC算法介绍**
      模型参数估计方法，如经典的最小二乘法，可以根据某种给定的目标方程估计并优化模型参数以使其最大程度适应于所有给定的数据集。这些方法都没有包含检测并排除异常数据的方法，他们都基于平滑假设：忽略给定的数据集的大小，总有足够多的准确数据值来消除异常数据的影响。但是在很多实际情况下，平滑假设无法成立，数据中可能包含无法得到补偿的严重错误数据，这时候此类模型参数估计方法将无法使用。
      例如如下情况：给定7个点（如图1所示），如何拟出一条最合适的直线段，使得所有的正确点到直线的距离不超过0.8。此时显然无法使用最小二乘法等方法进行拟合。

![ransac0](http://www.jellon.cn/wp-content/uploads/2010/03/ransac0.JPG)
*图1*

      RANSAC为RANdom SAmple Consensus的缩写，它是根据一组包含异常数据的样本数据集，计算出数据的数学模型参数，得到有效样本数据的算法。它于1981年由Fischler和Bolles最先提出[1]。
      RANSAC算法的基本假设是样本中包含正确数据(inliers，可以被模型描述的数据)，也包含异常数据(Outliers，偏离正常范围很远、无法适应数学模型的数据)，即数据集中含有噪声。这些异常数据可能是由于错误的测量、错误的假设、错误的计算等产生的。同时RANSAC也假设，给定一组正确的数据，存在可以计算出符合这些数据的模型参数的方法。
      RANSAC基本思想描述如下：
      ①考虑一个最小抽样集的势为n的模型(n为初始化模型参数所需的最小样本数)和一个样本集P，集合P的样本数#(P)>n，从P中随机抽取包含n个样本的P的子集S初始化模型M；
      ②余集SC=P/S中与模型M的误差小于某一设定阈值t的样本集以及S构成S*。S*认为是内点集，它们构成S的一致集(Consensus Set)；
      ③若#(S*)≥N，认为得到正确的模型参数，并利用集S*(内点inliers)采用最小二乘等方法重新计算新的模型M*；重新随机抽取新的S，重复以上过程。
      ④在完成一定的抽样次数后，若为找到一致集则算法失败，否则选取抽样后得到的最大一致集判断内外点，算法结束。
      由上可知存在两个可能的算法优化策略。①如果在选取子集S时可以根据某些已知的样本特性等采用特定的选取方案或有约束的随机选取来代替原来的完全随机选取；②当通过一致集S*计算出模型M*后，可以将P中所有与模型M*的误差小于t的样本加入S*，然后重新计算M*。
      RANSAC算法包括了3个输入的参数：①判断样本是否满足模型的误差容忍度t。t可以看作为对内点噪声均方差的假设，对于不同的输入数据需要采用人工干预的方式预设合适的门限，且该参数对RANSAC性能有很大的影响；②随机抽取样本集S的次数。该参数直接影响SC中样本参与模型参数的检验次数，从而影响算法的效率，因为大部分随机抽样都受到外点的影响；③表征得到正确模型时，一致集S*的大小N。为了确保得到表征数据集P的正确模型，一般要求一致集足够大；另外，足够多的一致样本使得重新估计的模型参数更精确。
      RANSAC算法经常用于计算机视觉中。例如，在立体视觉领域中同时解决一对相机的匹配点问题及基本矩阵的计算。

**二、RANSAC在消除错配中的应用**
      在特征点配对中，模型即为从一个平面上的特征点到另外一平面上的特征点的射影关系，反应为射影矩阵H。H是一个包含8个自由度的3×3矩阵，它最少可以由两平面中的4对匹配点计算出，但同一平面上的3个点必须不共面。
      图2、图3为RANSAC消除错配实验结果，两幅图像中的匹配点是由人工选取加Harris角点定位选取而来，匹配点对选取完毕后人工修改点集中的数据以产生外点。两图中绿色点为RANSAC认为正确匹配的点对，红色的点为错误匹配点对。

![ransac1](http://www.jellon.cn/wp-content/uploads/2010/03/ransac1.JPG)
*图2*
![ransac2](http://www.jellon.cn/wp-content/uploads/2010/03/ransac2.JPG)
*图3*

参考文献：
[1] Fischler, M.A. and Bolles, R.C. Random Sample Consensus: A Paradigm for Model Fitting with Applications to Image Analysis and Automated Cartography. Communications of the ACM , 24(6): 381–395, 1981.
[2] Richard Hartley and Andrew Zisserman. Multiple View Geometry in Computer Vision (2nd ed). Cambridge University Press .
[3] 程焱，周焰，林洪涛，潘恒辉. 基于SIFT特征遥感影像自动配准与拼接，遥感技术与应用，23(6):721–728，2008年12月

—————————————————————————————————————-

RANSAC消除错配matlab代码（match_ransac.m）：

function correctPoints = match_ransac(P1,P2, it, N, t)% MATCH_RANSAC:RANSAC outliner detector for matched point-pairs%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Input:%       P1        - coordinates of points in the 1st image, [pointnum x 2]%       P2        - coordinates of points in the 2nd image, [pointnum x 2]%       it        - iterations (the number of subset to try)%       N         - number of points in a consensus set to determine a correct model has been found%       t         - error tolerance to determine whether a point is compatible for the model% % Output:%       correctPoints - indexes of point-pairs corrected matched determined by RANSAC% % Author: % Jiaolong Yang% Beijing Laboratory of Intelligent Information Technology,% School of Computer Science,% Beijing Institute of Technology% jiaolong@bit.edu.cn% % Reference:% Fishler M A, Bolles R C. Random sample concensus:A paradigm for model fitting% with applications to image analysis and automated cartography[J]. Communications% of ACM, 1981, 24(6):381—395.% % $Revision: 1.0 $  $Date: 2010.03.17 21:44:20 $ Original version% $Revision: 1.1 $  $Date: 2010.03.19 16:04:50% $Revision: 2.0 $  $Date: 2010.03.23 13:17:39%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%switchnargincase2
            it = size(P1,1);
            N = fix(0.4*size(P1,1));
            t = 3;
        case3
            N = fix(0.4*size(P1,1));
            t = 3;
        case4
            t = 3;
    end
 
    correctPoints = [];
    pointNum = size(P1, 1);
    history = [];
 
    fori = 1:it
        %initialize consensus set with 4 points randomly selected 
        S = randperm(pointNum);
        S = S(1:4);
        %check whether every 3 points are noncollinearif check_collineation(P1(S, :))
            it = it+1;
            continue;
        end%check whether the selected group haven't been used before
        historyNum = size(history,1);
        exist = 0;
        forj = 1:historyNum
            equal = 1;
            for k = 1:4if history(j,k) ~= S(k)
                    equal = 0;
                    break;
                endendif equal
                exist = 1;
                break;
            endendifexist%reject used group
            it = it+1;
            continue;
        else
            history(end+1, :) = S;
        end%compute projective transformation matrix
        H = projectivematrix(P2(S,:)', P1(S,:)');
        %get full consensus set
        S =  get_consensus_set(P1, P2, S, H, t); 
        %check if the model(namely the matrix) is correct or better than that we've foundifsize(S,2) > N 
            N = size(S,2);
            correctPoints = S;
        endend%update projective transformation matrix
    H = projectivematrix(P2(correctPoints,:)',P1(correctPoints,:)');
    %get all points compatible with the best model
    correctPoints =  get_consensus_set(P1, P2, correctPoints, H, t);
endfunction S = get_consensus_set(P1, P2, S, H, t)
 
    dataNum = size(P1,1);
    %traverse all the pointsfori = 1:dataNum
        ifsize(find(S == i), 2) ~= 0continue;
        end%project p1 to p1_h using the given transformation matrix
        p1 = P1(i,:);
        tmp = H*[p1,1]';
        p1_h = tmp(1:2)' / tmp(3);
 
        p2 = P2(i,:);
 
        %compute the error(euclidian distance betwween p1_h and p2)
        dis = sqrt((p1_h(1)-p2(1))*(p1_h(1)-p2(1)) + (p1_h(2)-p2(2))*(p1_h(2)-p2(2)));
 
        %check error toleranceif dis < t
            S(end+1) = i;
        endendendfunction re = check_collineation(P)
 
    num = size(P,1);
    if num > 2fori = 1:num-2forj = i+1:num-1for k =j+1:num
                    a = [P(i,:)-P(j,:), 0];
                    b = [P(i,:)-P(k,:), 0];
                    %determine collineation of 3 points by "corss(p1p2,p1p3)==0"ifcross(a,b) == 0
                        re = 1;
                        return;
                    endendendendend
 
    re = 0;
end

文章来源于:http://www.jellon.cn/index.php/archives/291

