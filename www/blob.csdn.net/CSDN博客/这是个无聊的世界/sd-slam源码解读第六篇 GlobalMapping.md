# sd-slam源码解读第六篇:GlobalMapping - 这是个无聊的世界 - CSDN博客





2016年07月02日 20:42:28[lancelot_vim](https://me.csdn.net/lancelot_vim)阅读数：4490
所属专栏：[slam](https://blog.csdn.net/column/details/slam.html)









﻿# lsd-slam源码解读第六篇:GlobalMapping

标签（空格分隔）： lsd-slam

地图优化可以说是整个技术里面最简单的一部分了，核心其实就是g2o，可以看到这个里面代码也不多，总共加起来不到1000行，实际上就是这些工作，都是在为g2o服务

## g2oTypeSim3Sophus

这个文件里面定义了图优化需要使用的基本定点class VertexSim3 : public g2o::BaseVertex<7, Sophus::Sim3d>和边class EdgeSim3 : public g2o::BaseBinaryEdge<7, Sophus::Sim3d, VertexSim3, VertexSim3> 

在这之前，我们需要知道优化的对象是什么，请看论文中的公式 
![2016-07-02-161458_1161x147_scrot.png-22kB](http://static.zybuluo.com/lancelot-vim/5p330d3isu9nvl3pluhjdwo4/2016-07-02-161458_1161x147_scrot.png)

实际上可以从代码和论文中明白，优化的是sim3(顶点)，代表的是相机坐标的位置，通过约束sim3((边)，代表的是两个坐标之间的变换关系，他们是g2o的基本元素，请自行研究g2o库的使用方法，即明白这里的函数都有什么作用
## TrackableKeyFrameSearch

这个类主要是用于查找可用可keyFrame的，让我们直接进入函数

### TrackableKeyFrameSearch::findCandidates

不得不说这是一个返回值及其扭曲的函数，下面这么长一串全部是它的返回值

```
std::unordered_set<Frame*, std::hash<Frame*>,
                std::equal_to<Frame*>,
                Eigen::aligned_allocator< Frame* > >
```

看起来比较难受，其实是返回了一个hash_set，调用的是std::hash的方法吧指针hash，应该会调用这个模板（我没仔细看，不过十有八九是这个）

```cpp
template<typename _Tp>
  static size_t
  hash(const _Tp& __val)
  { return hash(&__val, sizeof(__val)); }
```

比较方法是std::equal_to，实际上就是调用了下面这个模板

```cpp
template<typename _Tp>
struct equal_to : public binary_function<_Tp, _Tp, bool>
{
  bool
  operator()(const _Tp& __x, const _Tp& __y) const
  { return __x == __y; }
};
```

最后一个是空间配置器，选用了eigen的空间配置器 

传入值是简单的，即一个Frame的指针，一个Frame指针的引用，判断是否使用了FABMAP(本文默认没有)，最后是个阈值，之后向下调用了函数findEuclideanOverlapFrames，计算符合要求的Frame，实际上返回了个TrackableKFStruct结构体

```
struct TrackableKFStruct
{
    Frame* ref;
    SE3 refToFrame;
    float dist;
    float angle;
};
```

#### **findEuclideanOverlapFrames**

首先计算了阈值，然后本地化了照相机到世界坐标的参数，计算尺度给后面检测使用(传入参数为true)

```
float cosAngleTH = cosf(angleTH*0.5f*(fowX + fowY));

Eigen::Vector3d pos = frame->getScaledCamToWorld().translation();
Eigen::Vector3d viewingDir = frame->getScaledCamToWorld().rotationMatrix().rightCols<1>();

float distFacReciprocal = 1;
if(checkBothScales)
    distFacReciprocal = frame->meanIdepth / frame->getScaledCamToWorld().scale();
```

之后进入循环，首先计算两帧之间的距离和角度，并进行阈值检测

```
Eigen::Vector3d otherPos = graph->keyframesAll[i]->getScaledCamToWorld().translation();

// get distance between the frames, scaled to fit the potential reference frame.
float distFac = graph->keyframesAll[i]->meanIdepth / graph->keyframesAll[i]->getScaledCamToWorld().scale();
if(checkBothScales && distFacReciprocal < distFac) distFac = distFacReciprocal;
Eigen::Vector3d dist = (pos - otherPos) * distFac;
float dNorm2 = dist.dot(dist);
if(dNorm2 > distanceTH) continue;

Eigen::Vector3d otherViewingDir = graph->keyframesAll[i]->getScaledCamToWorld().rotationMatrix().rightCols<1>();    float dirDotProd = otherViewingDir.dot(viewingDir);
if(dirDotProd < cosAngleTH) continue;
```

如果都符合了要求(没有continue)，那么压入potentialReferenceFrames

```
potentialReferenceFrames.push_back(TrackableKFStruct());
potentialReferenceFrames.back().ref = graph->keyframesAll[i];
potentialReferenceFrames.back().refToFrame = se3FromSim3(graph->keyframesAll[i]->getScaledCamToWorld().inverse() * frame->getScaledCamToWorld()).inverse();
potentialReferenceFrames.back().dist = dNorm2;
potentialReferenceFrames.back().angle = dirDotProd;
```

那么之后，就可以在TrackableKeyFrameSearch中得到潜在的帧，之后循环把参考帧都压入result,最后返回result

```matlab
for(unsigned int i=0;i<potentialReferenceFrames.size();i++)
    results.insert(potentialReferenceFrames[i].ref);
```

### TrackableKeyFrameSearch::findRePositionCandidate

这个函数也是首先调用findEuclideanOverlapFrames查找到潜在的参考帧，然后循环，进入循环之后先判断，然后调用了checkPermaRefOverlap得到可用点的分数，并做了些记录

```
if(frame->getTrackingParent() == potentialReferenceFrames[i].ref)
    continue;

if(potentialReferenceFrames[i].ref->idxInKeyframes < INITIALIZATION_PHASE_COUNT)
    continue;

struct timeval tv_start, tv_end;
gettimeofday(&tv_start, NULL);
tracker->checkPermaRefOverlap(potentialReferenceFrames[i].ref, potentialReferenceFrames[i].refToFrame);
gettimeofday(&tv_end, NULL);
msTrackPermaRef = 0.9*msTrackPermaRef + 0.1*((tv_end.tv_sec-tv_start.tv_sec)*1000.0f + (tv_end.tv_usec-tv_start.tv_usec)/1000.0f);
nTrackPermaRef++;
```

之后计算参考帧分数，调用getRefFrameScore

```
inline float getRefFrameScore(float distanceSquared, float usage)
{
    return distanceSquared*KFDistWeight*KFDistWeight
            + (1-usage)*(1-usage) * KFUsageWeight * KFUsageWeight;
}
```

然后判断这个分数够不够高，如果不够就重新算分,方法是快速tracking，然后再次计算dist，再调用getRefFrameScore，这样就可以计算到分数，之后计算差异，然后更新goodVal

```
SE3 RefToFrame_tracked = tracker->trackFrameOnPermaref(potentialReferenceFrames[i].ref, frame, potentialReferenceFrames[i].refToFrame);
Sophus::Vector3d dist = RefToFrame_tracked.translation() * potentialReferenceFrames[i].ref->meanIdepth;

float newScore = getRefFrameScore(dist.dot(dist), tracker->pointUsage);
float poseDiscrepancy = (potentialReferenceFrames[i].refToFrame * RefToFrame_tracked.inverse()).log().norm();
float goodVal = tracker->pointUsage * tracker->lastGoodCount / (tracker->lastGoodCount+tracker->lastBadCount);
checkedSecondary++;
```

之后判断这个新的tracking符不符合要求，如果符合，就更新全局的最优数据

```
bestPoseDiscrepancy = poseDiscrepancy;
bestScore = score;
bestFrame = potentialReferenceFrames[i].ref;
bestRefToFrame = potentialReferenceFrames[i].refToFrame;
bestRefToFrame_tracked = RefToFrame_tracked;
bestDist = dist.dot(dist);
bestUsage = tracker->pointUsage;
```

循环完成之后，判断找没找到，找到了就返回帧的指针，没找到返回空指针

## KeyFrameGraph

这个类用于创建g2o需要使用的图，里面有的函数分别有插入帧，插入关键帧，加入约束等，这些函数都很简单，稍微长一点的就是void KeyFrameGraph::dumpMap(std::string folder),然而这个函数点进去看之后，发现竟然是保存数据用的，这个类中所有函数都巨简单无比，请读者自行研究

时至今日，总算吧lsd-slam的所有核心代码看了个精光，其他部分都有有一个让整个系统跑起来的SlamSystem，它大体上就把这些函数封装成了功能，没别的其他的难点，还有一些文件主要是用于显示的，它依托于ros，实际上这个并不需要这么麻烦的ros系统，可以根据这三个(Tracking DepthEstimation以及GlobalMapping)的现成实现，自己做一个没有ros的lsd-slam，这也是我接下来要进行的工作，整个lsd-slam篇就这么多，希望读者多多阅读代码和论文，其实在之前写的部分有一些小问题，我这两天重新阅读代码的时候发现的，我也没有进行修正，读者可以自行研究




