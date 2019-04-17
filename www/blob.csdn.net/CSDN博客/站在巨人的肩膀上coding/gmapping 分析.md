# gmapping 分析 - 站在巨人的肩膀上coding - CSDN博客





2018年07月23日 21:14:38[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：508








【转载】这一篇先讲我对gmapping源码的理解(难免有错，欢迎指正，相互学习)。

原博客：[https://blog.csdn.net/roadseek_zw/article/details/53316177](https://blog.csdn.net/roadseek_zw/article/details/53316177)，博客主页：[https://blog.csdn.net/roadseek_zw](https://blog.csdn.net/roadseek_zw)

参考论文: 

Improved Techniques for Grid Mapping with Rao-Blackwellized Particle Filters 

参考博客： 
[http://blog.csdn.net/heyijia0327/article/details/40899819](http://blog.csdn.net/heyijia0327/article/details/40899819) pf原理讲解 
[http://blog.csdn.net/u010545732/article/details/17462941](http://blog.csdn.net/u010545732/article/details/17462941) pf代码实现 
[http://www.cnblogs.com/yhlx125/p/5634128.html](http://www.cnblogs.com/yhlx125/p/5634128.html) gmapping分析 
[http://wenku.baidu.com/view/3a67461550e2524de4187e4d.html?from=search](http://wenku.baidu.com/view/3a67461550e2524de4187e4d.html?from=search) gmapping 分析 

其他参考 ： 
[http://ishare.iask.sina.com.cn/f/24615049.html](http://ishare.iask.sina.com.cn/f/24615049.html)

从ros官网上下载 slam_gmapping 包以及在openslam （ [http://openslam.org/](http://openslam.org/) ）上下载openslam_gmapping包。 

为了方便的阅读源码，这里强力推荐一款源码阅读软件 understand （聪明的你一定找的到资源），可以方便实现各种跳转与生成图、表、树，流程等。 

废话不多说了，开始看源码，对于我这种c++都没有过关的菜鸟，看着大几千行的c++的代码，简直是身体和精神上的蹂躏。 

先说说 slam_gmapping 包与openslam_gmapping包 

进入slam_gmapping 的main.cpp文件的关系，slam_gmapping 是openslam_gampping在ros下的二次封装，你可以直接用这个包，而真正的核心代码实现都在openslam_gampping里面。

进入代码 

先用understand 看看代码的调用关系。 （调用太复杂了，图太大，我就截取了一小部分） 
![这里写图片描述](https://img-blog.csdn.net/20161124085040117)

转到

```
gn.startLiveSlam();
{
  entropy_publisher_ = private_nh_.advertise<std_msgs::Float64>("entropy", 1, true);
  sst_ = node_.advertise<nav_msgs::OccupancyGrid>("map", 1, true);
  sstm_ = node_.advertise<nav_msgs::MapMetaData>("map_metadata", 1, true);
  ss_ = node_.advertiseService("dynamic_map", &SlamGMapping::mapCallback, this);
  scan_filter_sub_ = new message_filters::Subscriber<sensor_msgs::LaserScan>(node_, "scan", 5);
  scan_filter_ = new tf::MessageFilter<sensor_msgs::LaserScan>(*scan_filter_sub_, tf_, odom_frame_, 5);
  scan_filter_->registerCallback(boost::bind(&SlamGMapping::laserCallback, this, _1));

  transform_thread_ = new boost::thread(boost::bind(&SlamGMapping::publishLoop, this, transform_publish_period_));
}
```

也没写啥，主要就是一些消息的回调以及发布一些服务，重点在

```
void SlamGMapping::laserCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
  laser_count_++;
   /*每隔throttle_scans_ （默认值 1）帧数据计算一次，限流作用*/
  if ((laser_count_ % throttle_scans_) != 0)  
    return;

  static ros::Time last_map_update(0,0);

  /* We can't initialize the mapper until we've got the first scan */
  if(!got_first_scan_)
  {
 /*一些重要参数的初始化，将slam里的参数传递到 openslam 里 ，设定坐标系，坐标原点，以及采样函数随机种子的初始化，等等
 里面还调用了 GridSlamProcessor::init ,这个初始化了粒子数，子地图大小 */
    if(!initMapper(*scan))
      return;
    got_first_scan_ = true;
  }

  GMapping::OrientedPoint odom_pose;
/**
pay attention： addScan这个函数*要转到pf的核心代码了 ，将调用processScan
*/
  if(addScan(*scan, odom_pose))   
  {
    ROS_DEBUG("scan processed");

    GMapping::OrientedPoint mpose = gsp_->getParticles()[gsp_->getBestParticleIndex()].pose;
    ROS_DEBUG("new best pose: %.3f %.3f %.3f", mpose.x, mpose.y, mpose.theta);
    ROS_DEBUG("odom pose: %.3f %.3f %.3f", odom_pose.x, odom_pose.y, odom_pose.theta);
    ROS_DEBUG("correction: %.3f %.3f %.3f", mpose.x - odom_pose.x, mpose.y - odom_pose.y, mpose.theta - odom_pose.theta);

    tf::Transform laser_to_map = tf::Transform(tf::createQuaternionFromRPY(0, 0, mpose.theta), tf::Vector3(mpose.x, mpose.y, 0.0)).inverse();
    tf::Transform odom_to_laser = tf::Transform(tf::createQuaternionFromRPY(0, 0, odom_pose.theta), tf::Vector3(odom_pose.x, odom_pose.y, 0.0));

    map_to_odom_mutex_.lock();
    map_to_odom_ = (odom_to_laser * laser_to_map).inverse();
    map_to_odom_mutex_.unlock();

    if(!got_map_ || (scan->header.stamp - last_map_update) > map_update_interval_)
    {
      updateMap(*scan);  //更新地图
      last_map_update = scan->header.stamp;
      ROS_DEBUG("Updated the map");
    }
  } else
    ROS_DEBUG("cannot process scan");
}
```

在 processScan 函数里依次执行了

## 运动模型

更新t时刻的粒子群，（模型中加入了高斯噪声） 

你要问我是为啥是这样的公式，请自行参考《Probabilistic Robot 》一书的P107页 ，里程计运动模型 

relPose 当前时刻的位姿（x,y，theta） ,m_odoPose 上一时刻的位姿

```
OrientedPoint 
MotionModel::drawFromMotion(const OrientedPoint& p, const OrientedPoint& pnew, const OrientedPoint& pold) const{
    double sxy=0.3*srr;  //目测是两轮轴间耦合方差，有点诡异？？？
    OrientedPoint delta=absoluteDifference(pnew, pold);
    OrientedPoint noisypoint(delta);  //噪声估计
    noisypoint.x+=sampleGaussian(srr*fabs(delta.x)+str*fabs(delta.theta)+sxy*fabs(delta.y));
    noisypoint.y+=sampleGaussian(srr*fabs(delta.y)+str*fabs(delta.theta)+sxy*fabs(delta.x));
    noisypoint.theta+=sampleGaussian(stt*fabs(delta.theta)+srt*sqrt(delta.x*delta.x+delta.y*delta.y));
    noisypoint.theta=fmod(noisypoint.theta, 2*M_PI);
    if (noisypoint.theta>M_PI)  
        noisypoint.theta-=2*M_PI;
    return absoluteSum(p,noisypoint);   //叠加噪声
}
```

计算t-1 —> t 时刻的 位移增量，以及角位移增量

```
OrientedPoint move=relPose-m_odoPose;
    move.theta=atan2(sin(move.theta), cos(move.theta));
    m_linearDistance+=sqrt(move*move);
    m_angularDistance+=fabs(move.theta);
```

```
if (! m_count ||m_linearDistance>=m_linearThresholdDistance 
    || m_angularDistance>=m_angularThresholdDistance
    || (period_ >= 0.0 && (reading.getTime() - last_update_time_) > period_))
    {
        //...
    }
```

if 里面有几个重要的函数，如下

## 扫描匹配

通过匹配选取最优的粒子，如果匹配失败，则返回一个默认的似然估计 

原理就参考 《Probabilistic Robot》 一书的P143 页 ， likelihood_field_range_finder_mode

```
inline void GridSlamProcessor::scanMatch(const double* plainReading){
  /* sample a new pose from each scan in the reference */

  double sumScore=0;
  for (ParticleVector::iterator it=m_particles.begin(); it!=m_particles.end(); it++){
    OrientedPoint corrected;
    double score, l, s;
/* 计算最优的粒子
optimize 调用了 score 这个函数 （计算粒子得分）
在score 函数里，首先计算障碍物的坐标phit，然后将phit转换成网格坐标iPhit
计算光束上与障碍物相邻的非障碍物网格坐标pfree,pfrree由phit沿激光束方向移动一个网格的距离得到，将pfree转换成网格坐标ipfree（增量，并不是实际值）
在iphit 及其附近8个（m_kernelSize:default=1）栅格（pr,对应自由栅格为pf）搜索最优可能是障碍物的栅格。
最优准则： pr 大于某一阈值，pf小于该阈值，且pr栅格的phit的平均坐标与phit的距离bestMu最小。
得分计算： s +=exp(-1.0/m_gaussianSigma*bestMu*besMu)  参考NDT算法,距离越大，分数越小，分数的较大值集中在距离最小值处，符合正态分布模型
至此 score 函数结束并返回粒子（currentPose）得分，然后回到optimize函数
optimize 干的事就是 currentPose 的位姿进行微调，前、后、左、右、左转、右转 共6次，然后选取得分最高的位姿，返回最终的得分
*/
    score=m_matcher.optimize(corrected, it->map, it->pose, plainReading);
    if (score>m_minimumScore){  //判断得分是否符合要求
      it->pose=corrected;
    } else {
    if (m_infoStream){
      m_infoStream << "Scan Matching Failed, using odometry. Likelihood=" << l <<std::endl;
      m_infoStream << "lp:" << m_lastPartPose.x << " "  << m_lastPartPose.y << " "<< m_lastPartPose.theta <<std::endl;
      m_infoStream << "op:" << m_odoPose.x << " " << m_odoPose.y << " "<< m_odoPose.theta <<std::endl;
    }
    }
/*   likelihoodAndSocre 作用是计算粒子的权重和（l），如果出现匹配失败，则 l=noHit     */
    m_matcher.likelihoodAndScore(s, l, it->map, it->pose, plainReading);
    sumScore+=score;
    it->weight+=l;
    it->weightSum+=l;

/* 计算可活动区域
    //set up the selective copy of the active area
    //by detaching the areas that will be updated
computeActiveArea 用于计算每个粒子相应的位姿所扫描到的区域  
计算过程首先考虑了每个粒子的扫描范围会不会超过子地图的大小，如果会，则resize地图的大小
然后定义了一个activeArea 用于设置可活动区域，调用了gridLine() 函数,这个函数如何实现的，
请参考百度文库那篇介绍。
*/
    m_matcher.invalidateActiveArea();
    m_matcher.computeActiveArea(it->map, it->pose, plainReading);
  }
  if (m_infoStream)
    m_infoStream << "Average Scan Matching Score=" << sumScore/m_particles.size() << std::endl; 
}
```

## 权重更新

重采样前更新过一次，重采样后又更新过一次，更新原理，参见论文（表示不是太懂）

```
void  GridSlamProcessor::updateTreeWeights(bool weightsAlreadyNormalized){

  if (!weightsAlreadyNormalized) {  
    normalize();   //Neff 计算 （论文中公式20）
  }
  resetTree();  //初始化粒子的树节点 权重，访问次数 ，父节点
  propagateWeights();  //为了迭代计算，计算上一次的该粒子的权重   （论文中公式19）
}
```

## 重采样

粒子集对目标分布的近似越差，则权重的方差越大，可用Neff来度量，具体原理参见论文，以及白巧克力亦唯心的那篇博客 

代码太长了就不粘贴了 

重采样里还调用了registerScan ，这个函数和computeActive 函数有点像，不同的是，registerScan用于注册每个单元格 

的状态，自由、障碍，调用update()以及entroy()函数更新，最后是障碍物的概率 p=n/visits , 

障碍物的坐标用平均值来算完了后，又有一次权重计算。

至此，processScan 结束，回到laserCallback，还有最优一步updateMap
`resample(plainReading, adaptParticles, reading_copy);`
## 地图更新

先得到最优的粒子（用权重和 weightSum判断 ），得到机器人最优轨迹 

地图膨胀更新

```
void
SlamGMapping::updateMap(const sensor_msgs::LaserScan& scan)
{
  ROS_DEBUG("Update map");
  boost::mutex::scoped_lock map_lock (map_mutex_);
  GMapping::ScanMatcher matcher;
  matcher.setLaserParameters(scan.ranges.size(), &(laser_angles_[0]),
                             gsp_laser_->getPose());
  matcher.setlaserMaxRange(maxRange_);
  matcher.setusableRange(maxUrange_);
  matcher.setgenerateMap(true);
/* 取最优粒子，根据权重和weightSum 判断（最大） */
  GMapping::GridSlamProcessor::Particle best =
          gsp_->getParticles()[gsp_->getBestParticleIndex()];
  std_msgs::Float64 entropy;
  entropy.data = computePoseEntropy();
  if(entropy.data > 0.0)
    entropy_publisher_.publish(entropy);
  if(!got_map_) {
    map_.map.info.resolution = delta_;
    map_.map.info.origin.position.x = 0.0;
    map_.map.info.origin.position.y = 0.0;
    map_.map.info.origin.position.z = 0.0;
    map_.map.info.origin.orientation.x = 0.0;
    map_.map.info.origin.orientation.y = 0.0;
    map_.map.info.origin.orientation.z = 0.0;
    map_.map.info.origin.orientation.w = 1.0;
  } 
  GMapping::Point center;
  center.x=(xmin_ + xmax_) / 2.0;
  center.y=(ymin_ + ymax_) / 2.0;
  GMapping::ScanMatcherMap smap(center, xmin_, ymin_, xmax_, ymax_, 
                                delta_);
  ROS_DEBUG("Trajectory tree:");
  /*得到机器人最优轨迹 */
  for(GMapping::GridSlamProcessor::TNode* n = best.node;
      n;
      n = n->parent)
  {
    ROS_DEBUG("  %.3f %.3f %.3f",
              n->pose.x,
              n->pose.y,
              n->pose.theta);
    if(!n->reading)
    {
      ROS_DEBUG("Reading is NULL");
      continue;
    }
    /*重新计算栅格单元的概率*/
    matcher.invalidateActiveArea();
    matcher.computeActiveArea(smap, n->pose, &((*n->reading)[0]));  
    matcher.registerScan(smap, n->pose, &((*n->reading)[0]));
  }
  /* the map may have expanded, so resize ros message as well */
  if(map_.map.info.width != (unsigned int) smap.getMapSizeX() || map_.map.info.height != (unsigned int) smap.getMapSizeY()) {
    /* NOTE: The results of ScanMatcherMap::getSize() are different from the parameters given to the constructor
       so we must obtain the bounding box in a different way */
    GMapping::Point wmin = smap.map2world(GMapping::IntPoint(0, 0));
    GMapping::Point wmax = smap.map2world(GMapping::IntPoint(smap.getMapSizeX(), smap.getMapSizeY()));
    xmin_ = wmin.x; ymin_ = wmin.y;
    xmax_ = wmax.x; ymax_ = wmax.y;

    ROS_DEBUG("map size is now %dx%d pixels (%f,%f)-(%f, %f)", smap.getMapSizeX(), smap.getMapSizeY(),
              xmin_, ymin_, xmax_, ymax_);
    map_.map.info.width = smap.getMapSizeX();
    map_.map.info.height = smap.getMapSizeY();
    map_.map.info.origin.position.x = xmin_;
    map_.map.info.origin.position.y = ymin_;
    map_.map.data.resize(map_.map.info.width * map_.map.info.height);   //地图需要膨胀
    ROS_DEBUG("map origin: (%f, %f)", map_.map.info.origin.position.x, map_.map.info.origin.position.y);
  }
  /*确定地图的未知区域、自由区域、障碍 */
  for(int x=0; x < smap.getMapSizeX(); x++)   
  {
    for(int y=0; y < smap.getMapSizeY(); y++)
    {
      /// @todo Sort out the unknown vs. free vs. obstacle thresholding
      GMapping::IntPoint p(x, y);
      double occ=smap.cell(p);
      assert(occ <= 1.0);
      if(occ < 0)
        map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = -1;
      else if(occ > occ_thresh_)
      {
        //map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = (int)round(occ*100.0);
        map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = 100;
      }
      else
        map_.map.data[MAP_IDX(map_.map.info.width, x, y)] = 0;
    }
  }
  got_map_ = true;
  //make sure to set the header information on the map
  map_.map.header.stamp = ros::Time::now();
  map_.map.header.frame_id = tf_.resolve( map_frame_ );
  sst_.publish(map_.map);
  sstm_.publish(map_.map.info);
}
```

至此，整个gmapping 的主线已经挑出来了，当然里面还有很多初始化我没讲，而且有很多细节我也还没有弄清楚， 

还有待进一步的研究（比如地图是如何膨胀的，扫描匹配得分计算的具体实施过程）。



