# dlib-19.4 调研
 - 更新中...

# 人脸检测
 - 模块 : frontal_face_detector
 - 算法 : 基于HOG特征和图像金字塔
 - 论文 : Object Detection with Discriminatively Trained Part Based Models by P. Felzenszwalb, R. Girshick, D. McAllester, D. Ramanan, IEEE Transactions on Pattern Analysis and Machine Intelligence, Vol. 32, No. 9, Sep. 2010
 - 例子 : face_detection_ex.cpp, webcam_face_pose_ex.cpp
 
# 人脸对齐
 - 模块 : shape_predictor
 - 论文 : One Millisecond Face Alignment with an Ensemble of Regression Trees by Vahid Kazemi and Josephine Sullivan, CVPR 2014
 - 例子 : face_landmark_detection_ex.cpp, webcam_face_pose_ex.cpp

# 目标跟踪
 - 模块 : correlation_tracker
 - 论文 : Danelljan, Martin, et al. "Accurate scale estimation for robust visual tracking." Proceedings of the British Machine Vision Conference BMVC. 2014
 - 例子 : video_tracking_ex.cpp