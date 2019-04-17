# StereoBM::disp12MaxDiff Crash the Release - Grandyang - 博客园







# [StereoBM::disp12MaxDiff Crash the Release](https://www.cnblogs.com/grandyang/p/4364601.html)







Initializing "cv::StereoBM bm.state->disp12MaxDiff" should be careful, inappropriate value might crash your program. Suggested that you set it to -1 to disable it.



OpenCV中的算两幅图像差的函数CVAPI(void) cvFindStereoCorrespondenceBM( const CvArr* left, const CvArr* right, CvArr* disparity, CvStereoBMState* state ); 中的最后一个参数state是用来调节参数得到不同效果的Disparity图，里面有个参数`disp12MaxDiff是用来设定左右两视图的最大不同值，在初始化值的时候要小心，错误的初始化值可能会让整个程序崩溃，保险的做法是设为-1，禁用该参数。`

**disp12MaxDiff** – Maximum allowed difference (in integer pixel units) in the left-right disparity check. Set it to a non-positive value to disable the check.














