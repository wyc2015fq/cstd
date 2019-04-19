# Onboard SDK - KinboSong的博客 - CSDN博客
2017年06月25日 17:25:46[songjinbo3](https://me.csdn.net/KinboSong)阅读数：1020
模拟器验证：无人机的x、y方向能达到的最大速度是14m/s，z方向能达到的最大速度是4m/s，最大高度为120m
1、moveWithVelocity（CoreAPI*api, Flight* flight,float32_t xVelocity,float32_t yVelocity,float32_t zVleocity,float32_t yawRate,int timeoutInMs=2000,float yawRateThresholdInDegs = 0.5,float velThresholdInMs = 0.5）
功能介绍：无人机达到xVelocity,yVelocity,zVelocity或者阻塞的时间达到timeoutInMs时退出
参数介绍：
参数3 ，参数4，参数5分别代表x,y,z方向上的速度，单位是m/s，（模拟器验证：无人机的x、y方向能达到的最大速度是14m/s，z方向能达到的最大速度是4m/s）
参数6代表yaw的角速度，单位是角度（degree）
参数7代表留给这个函数阻塞的时间，单位是ms
参数8，9代表无人机实际速度/角速度和期望值之间的阈值，当实际值和期望值得差别小于这个值得时候，认为速度已经达到要求。
2、moveByPositionOffset（Core* API,Flight* flight,float32_t xOffset,float32_t yOffset,float32_t zOffset,float32_t yawDesired,int timeoutInMs = 10000,float yawThresholdIndeg = 1,float posThresholdInCm = 5）
参数介绍：
参数3 ，参数4，参数5分别代表x,y,z方向上的位移，单位是m
参数6代表yaw偏移的角度，单位是角度（degree）；从上往下看，顺时针旋转是正值；
参数7代表运动时间，单位是ms
参数8，9代表无人机实际位移和
3、moveWithVelocity，moveByOffset和attitudeAltitudeControl这几个控制函在使用之前，无人机必须要起飞才可以
4、若无人机起飞后没有遥控降落或者指令控制降落，那么无人机是不会降落的。
5、Onboard-SDK-raspberry-support 读取无人机传感器信息的API在文件osdk-core/inc/DJI_Flight.h中
