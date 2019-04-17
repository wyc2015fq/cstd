# move_base代码学习 - 站在巨人的肩膀上coding - CSDN博客





2018年07月13日 23:24:23[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：294








# 参考链接：[https://www.cnblogs.com/shhu1993/p/6323699.html#system-overview](https://www.cnblogs.com/shhu1993/p/6323699.html#system-overview)

# [move_base代码学习一](https://www.cnblogs.com/shhu1993/p/6323699.html)




- [System overview](https://www.cnblogs.com/shhu1993/p/6323699.html#system-overview)
- [move_base](https://www.cnblogs.com/shhu1993/p/6323699.html#move_base)- [源码](https://www.cnblogs.com/shhu1993/p/6323699.html#%E6%BA%90%E7%A0%81)
- [API](https://www.cnblogs.com/shhu1993/p/6323699.html#api)

- [nav_core](https://www.cnblogs.com/shhu1993/p/6323699.html#nav_core)- [BaseGlobalPlanner](https://www.cnblogs.com/shhu1993/p/6323699.html#baseglobalplanner)
- [BaseLocalPlanner](https://www.cnblogs.com/shhu1993/p/6323699.html#baselocalplanner)
- [RecoveryBehavior](https://www.cnblogs.com/shhu1993/p/6323699.html#recoverybehavior)

- [Recovery behavior](https://www.cnblogs.com/shhu1993/p/6323699.html#recovery-behavior)- [clear_costmap_recovery](https://www.cnblogs.com/shhu1993/p/6323699.html#clear_costmap_recovery)
- [rotate_recovery](https://www.cnblogs.com/shhu1993/p/6323699.html#rotate_recovery)

- [costmap_2d](https://www.cnblogs.com/shhu1993/p/6323699.html#costmap_2d)- [costmap layer](https://www.cnblogs.com/shhu1993/p/6323699.html#costmap-layer)
- [Map type](https://www.cnblogs.com/shhu1993/p/6323699.html#map-type)
- [code](https://www.cnblogs.com/shhu1993/p/6323699.html#code)

- [Tips](https://www.cnblogs.com/shhu1993/p/6323699.html#tips)

# System overview
- 
move_base 主要有以下两部分
- [Planner](http://wiki.ros.org/nav_core)- nav_core::BaseGlobalPlanner
- nav_core::BaseLocalPlanner
- nav_core::RecoveryBehavior

- [costmap](http://www.ros.org/wiki/costmap_2d)- global planner costmap
- local planner costmap



![](https://images2015.cnblogs.com/blog/542140/201701/542140-20170120192232015-956251832.png)

**注意上面灰色的模块是可选的，地图也是可有可无的，蓝色是根据不同的机器模块进行配置的，白色模块是必须的**

# move_base

## 源码


```cpp
```cpp
namespace move_base{
    
enum MoveBaseState {
    PLANNING,
    CONTROLLING,
    CLEARING
};

enum RecoveryTrigger
{
    PLANNING_R,
    CONTROLLING_R,
    OSCILLATION_R
};
  
MoveBase::MoveBase(tf::TransformListener& tf){
    #action server
    as_ = new MoveBaseActionServer(ros::NodeHandle(), "move_base", boost::bind(&MoveBase::executeCb, this, _1), false);
    
    #参数的配置读取
    private_nh.param("base_global_planner", global_planner, std::string("navfn/NavfnROS"));
    .........

    //set up plan triple buffer
    planner_plan_ = new std::vector<geometry_msgs::PoseStamped>();
    latest_plan_ = new std::vector<geometry_msgs::PoseStamped>();
    controller_plan_ = new std::vector<geometry_msgs::PoseStamped>();

    //set up the planner's thread
    planner_thread_ = new boost::thread(boost::bind(&MoveBase::planThread, this));

    #global costmap，
    planner_costmap_ros_ = new costmap_2d::Costmap2DROS("global_costmap", tf_);
    #global planner
    planner_ = bgp_loader_.createInstance(global_planner);

    #local costmap
    controller_costmap_ros_ = new costmap_2d::Costmap2DROS("local_costmap", tf_);
    #local planner
    tc_ = blp_loader_.createInstance(local_planner);

    // Start actively updating costmaps based on sensor data
    planner_costmap_ros_->start();
    controller_costmap_ros_->start();
}

void MoveBase::executeCb(const move_base_msgs::MoveBaseGoalConstPtr& move_base_goal){

//we have a goal so start the planner(通知planner线程进行路径规划)
    boost::unique_lock<boost::mutex> lock(planner_mutex_);
    planner_goal_ = goal;
    runPlanner_ = true;
    #通知规划路径线程
    planner_cond_.notify_one();
    lock.unlock();
   
    ros::NodeHandle n;
    while(n.ok())
    {
        #被抢占了(可能是发出新的goal，也可能是取消了)
        if(as_->isPreemptRequested()){
            if(as_->isNewGoalAvailable()){
                #发布新的goal，通知planner线程工作。
                planner_cond_.notify_one();
            }
            else{
                //if we've been preempted explicitly we need to shut things down
                //强制退出
                return;
            }
        }

        //the real work on pursuing a goal is done here
        //真正工作的代码
        bool done = executeCycle(goal, global_plan);

        //这个是一般的警告信息，规划的时间超时
        if(r.cycleTime() > ros::Duration(1 / controller_frequency_) && state_ == CONTROLLING)
        ROS_WARN("Control loop missed its desired rate of %.4fHz... the loop actually took %.4f seconds", controller_frequency_, r.cycleTime().toSec());
    }
}

void MoveBase::planThread(){
    ros::NodeHandle n;

    boost::unique_lock<boost::mutex> lock(planner_mutex_);
    while(n.ok()){
        //等待上面的executeCb函数使得runPlanner_ = true
        while(wait_for_wake || !runPlanner_){
            //if we should not be running the planner then suspend this thread
            ROS_DEBUG_NAMED("move_base_plan_thread","Planner thread is suspending");
        }

        //time to plan! get a copy of the goal and unlock the mutex
        geometry_msgs::PoseStamped temp_goal = planner_goal_;

        //路径规划global
        bool gotPlan = n.ok() && makePlan(temp_goal, *planner_plan_);
        if(gotPlan){
            //成功规划路径
            ROS_DEBUG_NAMED("move_base_plan_thread","Generated a plan from the base_global_planner");
        }
        //if we didn't get a plan and we are in the planning state (the robot isn't moving)
        else if(state_==PLANNING){
            //check if we've tried to make a plan for over our time limit
        
            if(ros::Time::now() > attempt_end && runPlanner_){
                //we'll move into our obstacle clearing mode
                state_ = CLEARING;
                publishZeroVelocity();
                recovery_trigger_ = PLANNING_R;
            }
        }
        //定时器，多久没有规划路径，就通知一次规划路径
        timer = n.createTimer(sleep_time, &MoveBase::wakePlanner, this);
    }
}

//这个是在global costmap下做的global planner
bool MoveBase::makePlan(const geometry_msgs::PoseStamped& goal, std::vector<geometry_msgs::PoseStamped>& plan){
    // global costmap
    boost::unique_lock<costmap_2d::Costmap2D::mutex_t> lock(*(planner_costmap_ros_->getCostmap()->getMutex()));
    //得到机器人现在的位置信息
    if(!planner_costmap_ros_->getRobotPose(global_pose)) {
        ROS_WARN("Unable to get starting pose of robot, unable to create global plan");
        return false;
    }
    //global planner
    if(!planner_->makePlan(start, goal, plan) || plan.empty()){
        ROS_DEBUG_NAMED("move_base","Failed to find a  plan to point (%.2f, %.2f)", goal.pose.position.x, goal.pose.position.y);
        return false;
    }
}

bool MoveBase::executeCycle(geometry_msgs::PoseStamped& goal, std::vector<geometry_msgs::PoseStamped>& global_plan){
    //发布速度topic
    geometry_msgs::Twist cmd_vel;

    //push the feedback out
    //发布一些反馈信息
    move_base_msgs::MoveBaseFeedback feedback;
    feedback.base_position = current_position;
    as_->publishFeedback(feedback);

     //check to see if we've moved far enough to reset our oscillation timeout
    if(distance(current_position, oscillation_pose_) >= oscillation_distance_)
    {
      last_oscillation_reset_ = ros::Time::now();
      oscillation_pose_ = current_position;

      //if our last recovery was caused by oscillation, we want to reset the recovery index 
      if(recovery_trigger_ == OSCILLATION_R)
        recovery_index_ = 0;
    }

    //if we have a new plan then grab it and give it to the controller
    if(new_global_plan_){
        std::vector<geometry_msgs::PoseStamped>* temp_plan = controller_plan_;
        latest_plan_ = temp_plan;
        //这是local planner的规划
        if(!tc_->setPlan(*controller_plan_)){
            //ABORT and SHUTDOWN COSTMAPS
            ROS_ERROR("Failed to pass global plan to the controller, aborting.");
            resetState();

            //disable the planner thread
            lock.lock();
            runPlanner_ = false;
            lock.unlock();

            as_->setAborted(move_base_msgs::MoveBaseResult(), "Failed to pass global plan to the controller.");
            return true;
        }

        //make sure to reset recovery_index_ since we were able to find a valid plan
        if(recovery_trigger_ == PLANNING_R)
            recovery_index_ = 0;
    }

    //the move_base state machine, handles the control logic for navigation
    switch(state_){
        //if we are in a planning state, then we'll attempt to make a plan
        case PLANNING:
            runPlanner_ = true;
            planner_cond_.notify_one();
            
            ROS_DEBUG_NAMED("move_base","Waiting for plan, in the planning state.");
            break;
        //if we're controlling, we'll attempt to find valid velocity commands
        case CONTROLLING:
            //check to see if we've reached our goal
            //这是local planner的
            if(tc_->isGoalReached()){
                ROS_DEBUG_NAMED("move_base","Goal reached!");
                as_->setSucceeded(move_base_msgs::MoveBaseResult(), "Goal reached.");
                return true;
            }
            //这个似乎是小车不知道为什么来回走动，在一定的时间没有移动足够的距离
            if(oscillation_timeout_ > 0.0 && last_oscillation_reset_ + ros::Duration(oscillation_timeout_) < ros::Time::now())
            {
                publishZeroVelocity();
                state_ = CLEARING;
                recovery_trigger_ = OSCILLATION_R;
            }
            //发布小车行走的cmd_vel topic
            if(tc_->computeVelocityCommands(cmd_vel)){
                ROS_DEBUG_NAMED( "move_base", "Got a valid command from the local planner: %.3lf, %.3lf, %.3lf",cmd_vel.linear.x, cmd_vel.linear.y, cmd_vel.angular.z );
                vel_pub_.publish(cmd_vel);
                if(recovery_trigger_ == CONTROLLING_R)
                    recovery_index_ = 0;
            }
            else{
                ROS_DEBUG_NAMED("move_base", "The local planner could not find a valid plan.");
                if(ros::Time::now() > attempt_end){
                    //we'll move into our obstacle clearing mode
                    publishZeroVelocity();
                    state_ = CLEARING;
                    recovery_trigger_ = CONTROLLING_R;
                }
                else{
                    //otherwise, if we can't find a valid control, we'll go back to planning
                    planner_cond_.notify_one();
                }
            }

            break;
        //we'll try to clear out space with any user-provided recovery behaviors
        case CLEARING:
            ROS_DEBUG_NAMED("move_base","In clearing/recovery state");
            if(recovery_behavior_enabled_ && recovery_index_ < recovery_behaviors_.size()){
                ROS_DEBUG_NAMED("move_base_recovery","Executing behavior %u of %zu", recovery_index_, recovery_behaviors_.size());
                recovery_behaviors_[recovery_index_]->runBehavior();
                
                ROS_DEBUG_NAMED("move_base_recovery","Going back to planning state");
                state_ = PLANNING;
                //update the index of the next recovery behavior that we'll try
                recovery_index_++;
             }
             else{
                ROS_DEBUG_NAMED("move_base_recovery","All recovery behaviors have failed, locking the planner and disabling it.");
                runPlanner_ = false;
                if(recovery_trigger_ == CONTROLLING_R){
                    ROS_ERROR("Aborting because a valid control could not be found. Even after executing all recovery behaviors");
                    as_->setAborted(move_base_msgs::MoveBaseResult(), "Failed to find a valid control. Even after executing recovery behaviors.");
                }
                else if(recovery_trigger_ == PLANNING_R){
                    ROS_ERROR("Aborting because a valid plan could not be found. Even after executing all recovery behaviors");
                    as_->setAborted(move_base_msgs::MoveBaseResult(), "Failed to find a valid plan. Even after executing recovery behaviors.");
                }
                else if(recovery_trigger_ == OSCILLATION_R){
                    ROS_ERROR("Aborting because the robot appears to be oscillating over and over. Even after executing all recovery behaviors");
                    as_->setAborted(move_base_msgs::MoveBaseResult(), "Robot is oscillating. Even after executing recovery behaviors.");
                }
                resetState();
             }
             break;
        default:
            ROS_ERROR("This case should never be reached, something is wrong, aborting");
            resetState();
    }
}


}
```
```


从构造函数的actionServer 和线程函数planThread(等待`executeCb`通知他工作，规划一下路径，这个是global plan)


```cpp
```cpp
as_ = new MoveBaseActionServer(ros::NodeHandle(), "move_base", boost::bind(&MoveBase::executeCb, this, _1), false);
planner_thread_ = new boost::thread(boost::bind(&MoveBase::planThread, this));
```
```


每次有goal发布的时候，都会去调用`executeCb`,`executeCb`会去调用`executeCycle`
进行local plan,发布cmd_vel的topic，根据小车处于的状态，进行相应的实现(会进行plan,control,clear obstacle)

## API
- 提供两种方式`send goal`目标位置的信息- SimpleActionServer(可以追踪移动到目标过程的状态信息，到了没有，是不是没有plan，取消了etc)
- Subscribe topic(move_base_simple/goal),直接发布这个topic的信息，不会有目标执行过程中的反馈信息

- 提供的`service`- ~make_plan(只会提供plan该怎么走的位置信息，不会使机器人移动)
- ~clear_unknown_space(告诉move_base清楚机器人周围的unknown space)
- ~clear_costmaps(告诉move_base清楚costmap中的障碍物信息，可能导致撞到障碍物)


# nav_core

[ROS wiki](http://wiki.ros.org/nav_core)

[代码文档](http://docs.ros.org/api/nav_core/html/dir_e565826054f60a25ba14575947230cc2.html)

提供三类接口(这些接口都是以plugin的形式存在，很容易更换)：
- BaseGlobalPlanner
- BaseLocalPlanner
- RecoveryBehavior

![](https://images2015.cnblogs.com/blog/542140/201701/542140-20170120192300406-379647971.png)

## BaseGlobalPlanner

所有的`global planner`都必须实现`nav_core::BaseGlobalPlanner`定义的接口(纯续函数)
- [global_planner](http://wiki.ros.org/global_planner)
- [navfn](http://wiki.ros.org/navfn)
- [carrot_planner](http://wiki.ros.org/carrot_planner)

主要实现的函数定义如下：


```cpp
```cpp
virtual void initialize (std::string name, costmap_2d::Costmap2DROS *costmap_ros)=0

virtual bool makePlan (const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal, std::vector< geometry_msgs::PoseStamped > &plan)=0
```
```


## BaseLocalPlanner

所有的`local planner`都必须实现`nav_core::BaseLocalPlanner`定义的接口(纯续函数)
- [base_local_planner](http://wiki.ros.org/base_local_planner)
- [eband_local_planner](http://wiki.ros.org/eband_local_planner)
- [teb_local_planner](http://wiki.ros.org/teb_local_planner)

主要实现的函数定义如下：


```cpp
```cpp
#Given the current position, orientation, and velocity of the robot, compute velocity commands to send to the base
virtual bool computeVelocityCommands (geometry_msgs::Twist &cmd_vel)=0
 
virtual void initialize (std::string name, tf::TransformListener *tf, costmap_2d::Costmap2DROS *costmap_ros)=0
 
virtual bool isGoalReached ()=0
 
virtual bool    setPlan (const std::vector< geometry_msgs::PoseStamped > &plan)=0
```
```


## RecoveryBehavior

所有的`recovery behaviors`都必须实现`nav_core::RecoveryBehavior`定义的接口(纯续函数)
- [clear_costmap_recovery](http://wiki.ros.org/clear_costmap_recovery)
- [rotate_recovery](http://wiki.ros.org/rotate_recovery)

主要实现的函数定义如下：


```cpp
```cpp
virtual void initialize (std::string name, tf::TransformListener *tf, costmap_2d::Costmap2DROS *global_costmap, costmap_2d::Costmap2DROS *local_costmap)=0
 
virtual void runBehavior ()=0
```
```


# Recovery behavior

![](https://images2015.cnblogs.com/blog/542140/201701/542140-20170120192438250-2133098092.png)

首先, obstacles outside of a user-specified region will be cleared from the robot's map.(`conservative_reset_dist`半径之外的非`NO_INFORMATION`
都会被设置成`NO_INFORMATION`)。然后, if possible, the robot will perform an in-place rotation to clear out space. If this too fails, 再然后，
the robot will more aggressively clear its map, removing all obstacles outside of the rectangular region in
which it can rotate in place.(默认是4倍的最小外接圆半径) This will be followed by another in-place rotation. If all this fails,
the robot will consider its goal infeasible and notify the user that it has aborted

插件的配置


```
~recovery_behaviors 
     (list, default: [{name: conservative_reset, type: clear_costmap_recovery/ClearCostmapRecovery},
                      {name: rotate_recovery, type: rotate_recovery/RotateRecovery}, 
                      {name: aggressive_reset, type: clear_costmap_recovery/ClearCostmapRecovery}]
```


参数


```
#The distance away from the robot in meters at which obstacles will be cleared from the costmap when attempting to clear space in the map.
~conservative_reset_dist (double, default: 3.0) 
#Whether or not to enable the move_base recovery behaviors to attempt to clear out space. 
~recovery_behavior_enabled (bool, default: true) 
#Determines whether or not the robot will attempt an in-place rotation when attempting to clear out space.
~clearing_rotation_allowed (bool, default: true) 
#来回震荡的距离，超过一定的时间没有移动足够的距离就会进行recovery behavior
#How far in meters the robot must move to be considered not to be oscillating. Moving this far resets the timer counting up to the ~oscillation_timeout
~oscillation_distance (double, default: 0.5) 
#How long in seconds to allow for oscillation before executing recovery behaviors. A value of 0.0 corresponds to an infinite timeout. 
~oscillation_timeout (double, default: 0.0)
```


**For the default parameters, the `aggressive_reset` behavior will clear out to a distance of `4 * ~/local_costmap/circumscribed_radius`.**

## clear_costmap_recovery

[ROS wiki](http://wiki.ros.org/clear_costmap_recovery)

[代码文档](http://docs.ros.org/api/clear_costmap_recovery/html/classclear__costmap__recovery_1_1ClearCostmapRecovery.html)

是把local,global costmap在reset_distance半径之外的free，occupied都清除，变成`NO_INFORMATION`。不对static layer操作

#### 重要的参数


```
#距离以机器人为中心，reset_distance米为半径的圆外面的costmap(local,global)都会被清除掉，
#只留下static map原来的信息
~<name>/reset_distance (double, default: 3.0)
```


#### 怎么使用


```cpp
```cpp
#include <tf/transform_listener.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <clear_costmap_recovery/clear_costmap_recovery.h>

tf::TransformListener tf(ros::Duration(10));
costmap_2d::Costmap2DROS global_costmap("global_costmap", tf);
costmap_2d::Costmap2DROS local_costmap("local_costmap", tf);

clear_costmap_recovery::ClearCostmapRecovery ccr;
ccr.initialize("my_clear_costmap_recovery", &tf, &global_costmap, &local_costmap);

//recovery 行为
ccr.runBehavior();
```
```


#### 源码


```cpp
```cpp
namespace clear_costmap_recovery{
class ClearCostmapRecovery : public nav_core::RecoveryBehavior {

//在多大的半径之外的obstacle全部清除
double reset_distance_;
//那些layer的obstacle要清除
std::set<std::string> clearable_layers_; ///< Layer names which will be cleared.

void ClearCostmapRecovery::initialize(std::string name, tf::TransformListener* tf,
                costmap_2d::Costmap2DROS* global_costmap, costmap_2d::Costmap2DROS* local_costmap){
    if(!initialized_){
        global_costmap_ = global_costmap;
        local_costmap_ = local_costmap;

        private_nh.param("reset_distance", reset_distance_, 3.0);

        std::vector<std::string> clearable_layers_default, clearable_layers;
        clearable_layers_default.push_back( std::string("obstacles") );
        private_nh.param("layer_names", clearable_layers, clearable_layers_default);

        for(unsigned i=0; i < clearable_layers.size(); i++) {
            ROS_INFO("Recovery behavior will clear layer %s", clearable_layers[i].c_str());
            clearable_layers_.insert(clearable_layers[i]);
        }
        initialized_ = true;
    }
}

void ClearCostmapRecovery::runBehavior(){
    ROS_WARN("Clearing costmap to unstuck robot (%fm).", reset_distance_);
    //看好是把global，local的costmap的obstacle都清除掉
    clear(global_costmap_);
    clear(local_costmap_);
}

void ClearCostmapRecovery::clear(costmap_2d::Costmap2DROS* costmap){
{
    //得到costmap中定义的所有layer
    std::vector<boost::shared_ptr<costmap_2d::Layer> >* plugins = costmap->getLayeredCostmap()->getPlugins();
    for (std::vector<boost::shared_ptr<costmap_2d::Layer> >::iterator pluginp = plugins->begin(); pluginp != plugins->end(); ++pluginp) {
    {
        if(clearable_layers_.count(name)!=0){
            costmap = boost::static_pointer_cast<costmap_2d::CostmapLayer>(plugin);
            clearMap(costmap, x, y);
        }
    }
}

void ClearCostmapRecovery::clearMap(boost::shared_ptr<costmap_2d::CostmapLayer> costmap, 
                                        double pose_x, double pose_y){
{
    double start_point_x = pose_x - reset_distance_ / 2;
    double start_point_y = pose_y - reset_distance_ / 2;
    double end_point_x = start_point_x + reset_distance_;
    double end_point_y = start_point_y + reset_distance_;

    int start_x, start_y, end_x, end_y;
    costmap->worldToMapNoBounds(start_point_x, start_point_y, start_x, start_y);
    costmap->worldToMapNoBounds(end_point_x, end_point_y, end_x, end_y);
    
    for(int x=0; x<(int)costmap->getSizeInCellsX(); x++){
        bool xrange = x>start_x && x<end_x;
                   
        for(int y=0; y<(int)costmap->getSizeInCellsY(); y++){
            if(xrange && y>start_y && y<end_y)
                continue;
            int index = costmap->getIndex(x,y);
            //注意重点
            if(grid[index]!=NO_INFORMATION){
                grid[index] = NO_INFORMATION;
            }
        }
    }
    
    double ox = costmap->getOriginX(), oy = costmap->getOriginY();
    double width = costmap->getSizeInMetersX(), height = costmap->getSizeInMetersY();
    costmap->addExtraBounds(ox, oy, ox + width, oy + height);

}

}
```
```


## rotate_recovery

[ROS wiki](http://wiki.ros.org/rotate_recovery)

[代码文档](http://docs.ros.org/api/rotate_recovery/html/classrotate__recovery_1_1RotateRecovery.html)

`rotate_recovery::RotateRecovery`默认的`local planner`是`base_local_planner::TrajectoryPlannerROS`,如果使用别的local planner
你要具体的定义一些参数，[具体参考这里](http://wiki.ros.org/rotate_recovery#TrajectoryPlannerROS_Parameters)

他的作用只是在原地旋转，这样costmap就会自己更新，mark或者clear一点的值


```
#The distance in radians between checks for obstacles when checking if an in-place rotation is safe. Defaults to 1 degree. 
~<name>/sim_granularity (double, default: 0.017)
```


#### 源码


```cpp
```cpp
namespace rotate_recovery{
class RotateRecovery : public nav_core::RecoveryBehavior {

double sim_granularity_, min_rotational_vel_, max_rotational_vel_, acc_lim_th_, tolerance_, frequency_;
base_local_planner::CostmapModel* world_model_;

void RotateRecovery::initialize(std::string name, tf::TransformListener* tf,
    costmap_2d::Costmap2DROS* global_costmap, costmap_2d::Costmap2DROS* local_costmap){
{
    if(!initialized_){
        global_costmap_ = global_costmap;
        local_costmap_ = local_costmap;

        //get some parameters from the parameter server
        ros::NodeHandle private_nh("~/" + name_);
        #默认的local planner，他的参数空间，要不是的话，要自己显性设置下面那些参数
        ros::NodeHandle blp_nh("~/TrajectoryPlannerROS");

        //we'll simulate every degree by default
        private_nh.param("sim_granularity", sim_granularity_, 0.017);
        private_nh.param("frequency", frequency_, 20.0);

        blp_nh.param("acc_lim_th", acc_lim_th_, 3.2);
        blp_nh.param("max_rotational_vel", max_rotational_vel_, 1.0);
        blp_nh.param("min_in_place_rotational_vel", min_rotational_vel_, 0.4);
        blp_nh.param("yaw_goal_tolerance", tolerance_, 0.10);

        world_model_ = new base_local_planner::CostmapModel(*local_costmap_->getCostmap());

        initialized_ = true;
    }
}

void RotateRecovery::runBehavior(){
    ros::Rate r(frequency_);
    ros::NodeHandle n;
    ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    //下面就是看能不能在原地旋转，不会碰到障碍物
    vel_pub.publish(cmd_vel);
}
}
```
```


# costmap_2d

使用分层的机制，每层的信息都加到`costmap_2d::LayeredCostmap`进行管理。

## costmap layer
- [static_layer](http://wiki.ros.org/costmap_2d/hydro/staticmap)
- [obstacle_layer](http://wiki.ros.org/costmap_2d/hydro/obstacles)
- [inflation_layer](http://wiki.ros.org/costmap_2d/hydro/inflation)
- otherlayers- [Social Costmap Layer](http://wiki.ros.org/social_navigation_layers)跟人行走，移动有关的
- [Range Sensor Layer](http://wiki.ros.org/range_sensor_layer)跟超声波传感器有关


#### Marking and Clearing

Each sensor is used to either mark (insert obstacle information into the costmap),
clear (remove obstacle information from the costmap), or both.
A marking operation is just an index into an array to change the cost of a cell.
A clearing operation, however, consists of raytracing through a grid from the origin of the sensor outwards for each observation reported.

#### Occupied, Free, and Unknown Space

**注意这里说的是列，那是不是意味着它使用的voxel，3维存储信息**

Columns that have a certain number of occupied cells (see `mark_threshold` parameter)
are assigned a `costmap_2d::LETHAL_OBSTACLE` cost, columns that have a certain number of unknown cells (see `unknown_threshold` parameter)
are assigned a `costmap_2d::NO_INFORMATION` cost, and other columns are assigned a
`costmap_2d::FREE_SPACE` cost.

costmap更新的频率由参数`update_frequency`确定。**注意costmap严重依赖于`tf`的信息，要是`tf`太久没有更新的话，navigation就会挂掉**
这个时间tolerance是由参数`transform_tolerance`确定的

## Map type

有两种方法初始化`costmap_2d::Costmap2DROS`对象：
- 
static map(map_server)

这种情况下，costmap初始化的长度宽度和static map的一样,obstacle的信息也来自static map。
一般用在`amcl`定位导航中，随着机器人的移动，使用传感器的信息更新costmap

- 
rolling window

自己给一个长，宽值，设置`rolling_window`参数为`true`,这个参数设置机器人的位置在costmap的中心，丢弃离机器人比较远的obstacle信息。
一般用在在里程计坐标下的移动，机器人只关心在他周边的障碍物信息。


**注意提供plugin申明，还有在move_base node下面运行，不然有一些兼容性问题(可能是Hydro版本以前的初始化方式)**

## code

[代码文档](http://docs.ros.org/api/costmap_2d/html/classcostmap__2d_1_1Costmap2DROS.html)

A ROS wrapper for a 2D Costmap. Handles subscribing to topics that provide observations about obstacles in either the form of PointCloud or LaserScan messages


```cpp
```cpp
namespace costmap_2d
{
class Costmap2DROS
{
Costmap2DROS::Costmap2DROS(std::string name, tf::TransformListener& tf){
    layered_costmap_ = new LayeredCostmap(global_frame_, rolling_window, track_unknown_space);
    //发布costmap
    publisher_ = new Costmap2DPublisher(&private_nh, layered_costmap_->getCostmap(), global_frame_, "costmap",
                                      always_send_full_costmap);
    timer_ = private_nh.createTimer(ros::Duration(.1), &Costmap2DROS::movementCB, this);
    
    map_update_thread_ = new boost::thread(boost::bind(&Costmap2DROS::mapUpdateLoop, this, map_update_frequency));

}

void Costmap2DROS::mapUpdateLoop(double frequency)
   
    updateMap();
    publisher_->publishCostmap();
}

void Costmap2DROS::updateMap()
{
    if (!stop_updates_)
    {
        // get global pose
        tf::Stamped < tf::Pose > pose;
        if (getRobotPose (pose))
        {
            double x = pose.getOrigin().x(),
                    y = pose.getOrigin().y(),
                    yaw = tf::getYaw(pose.getRotation());

            //对所有layer的costmap进行更新
            layered_costmap_->updateMap(x, y, yaw);
        }
    }
}
}
}
```
```



```cpp
```cpp
namespace costmap_2d
{
class LayeredCostmap{

void LayeredCostmap::updateMap(double robot_x, double robot_y, double robot_yaw)
{
    minx_ = miny_ = 1e30;
    maxx_ = maxy_ = -1e30;

    // Lock for the remainder of this function, some plugins (e.g. VoxelLayer)
    // implement thread unsafe updateBounds() functions.
    boost::unique_lock<Costmap2D::mutex_t> lock(*(costmap_.getMutex()));

    for (vector<boost::shared_ptr<Layer> >::iterator plugin = plugins_.begin(); plugin != plugins_.end();++plugin)
    {
        double prev_minx = minx_;
        double prev_miny = miny_;
        double prev_maxx = maxx_;
        double prev_maxy = maxy_;
        (*plugin)->updateBounds(robot_x, robot_y, robot_yaw, &minx_, &miny_, &maxx_, &maxy_);
    }

    int x0, xn, y0, yn;
    costmap_.worldToMapEnforceBounds(minx_, miny_, x0, y0);
    costmap_.worldToMapEnforceBounds(maxx_, maxy_, xn, yn);

    x0 = std::max(0, x0);
    xn = std::min(int(costmap_.getSizeInCellsX()), xn + 1);
    y0 = std::max(0, y0);
    yn = std::min(int(costmap_.getSizeInCellsY()), yn + 1);

    ROS_DEBUG("Updating area x: [%d, %d] y: [%d, %d]", x0, xn, y0, yn);

    if (xn < x0 || yn < y0)
        return;

    costmap_.resetMap(x0, y0, xn, yn);
    for (vector<boost::shared_ptr<Layer> >::iterator plugin = plugins_.begin(); plugin != plugins_.end();
        ++plugin)
    {
        //cost 值的更新，每个plugin都实现这个函数,是一份costmap在不同的plugin下的更新，costmap是一样的，cost值是叠加上去的
        (*plugin)->updateCosts(costmap_, x0, y0, xn, yn);
    }
}
}
}
```
```



```cpp
```cpp
namespace costmap_2d
{
class StaticLayer : public CostmapLayer
{
    virtual void updateBounds(double robot_x, double robot_y, double robot_yaw, double* min_x, double* min_y,
                            double* max_x, double* max_y);
    virtual void updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);

    void incomingMap(const nav_msgs::OccupancyGridConstPtr& new_map);
    void incomingUpdate(const map_msgs::OccupancyGridUpdateConstPtr& update);
}
}
```
```



```cpp
```cpp
namespace costmap_2d
{
class ObstacleLayer : public CostmapLayer
{
    virtual void updateBounds(double robot_x, double robot_y, double robot_yaw, double* min_x, double* min_y,
                            double* max_x, double* max_y);
    virtual void updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i, int max_j);

    virtual void activate();
  
    void laserScanCallback(const sensor_msgs::LaserScanConstPtr& message,
                         const boost::shared_ptr<costmap_2d::ObservationBuffer>& buffer);
 
    void pointCloudCallback(const sensor_msgs::PointCloudConstPtr& message,
         https://zhuanlan.zhihu.com/p/21738718?refer=robotics-learning                 const boost::shared_ptr<costmap_2d::ObservationBuffer>& buffer);

    virtual void raytraceFreespace(const costmap_2d::Observation& clearing_observation, double* min_x, double* min_y,
                                 double* max_x, double* max_y);

    void updateRaytraceBounds(double ox, double oy, double wx, double wy, double range, double* min_x, double* min_y,
                            double* max_x, double* max_y);
}
}
```
```


怎么计算每个costmap中的单元格的cost代码还没有研究

# Tips

[开启ros debug level的模式](http://wiki.ros.org/rosconsole#Configuration)

[栅格地图理论编程](https://zhuanlan.zhihu.com/p/21738718?refer=robotics-learning)





