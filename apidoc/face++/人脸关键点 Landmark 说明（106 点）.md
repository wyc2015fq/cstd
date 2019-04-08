# 人脸关键点 Landmark 说明（106 点）

### Landmark 对象结构

分别包括脸部，眼睛，眉毛，嘴唇，鼻子轮廓。下图为示例，蓝点为 Landmark 关键点。

![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/1all.jpg)

 

### Landmark字段说明：

注意：

- 下文提及的坐标单位均为像素。
- 下文提及的“左”或“右”，均指图像中的左右，对于人脸照来说，是反向的。比如：下文提到的“左眼”实际为他的右眼、“左眉”实际为他的右眉。以此类推
- 下文字段均代表一个人脸关键点，包含字段 x: 横坐标位置; y: 纵坐标位置  

| 字段                              | 类型   | 说明   | 图示                                                         |
| :-------------------------------- | :----- | :----- | :----------------------------------------------------------- |
| contour_left1                     | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/3c.jpg) |
| contour_left2                     | Object | 关键点 |                                                              |
| contour_left3                     | Object | 关键点 |                                                              |
| contour_left4                     | Object | 关键点 |                                                              |
| contour_left5                     | Object | 关键点 |                                                              |
| contour_left6                     | Object | 关键点 |                                                              |
| contour_left7                     | Object | 关键点 |                                                              |
| contour_left8                     | Object | 关键点 |                                                              |
| contour_left9                     | Object | 关键点 |                                                              |
| contour_left10                    | Object | 关键点 |                                                              |
| contour_left11                    | Object | 关键点 |                                                              |
| contour_left12                    | Object | 关键点 |                                                              |
| contour_left13                    | Object | 关键点 |                                                              |
| contour_left14                    | Object | 关键点 |                                                              |
| contour_left15                    | Object | 关键点 |                                                              |
| contour_left16                    | Object | 关键点 |                                                              |
| contour_chin                      | Object | 关键点 |                                                              |
| contour_right1                    | Object | 关键点 |                                                              |
| contour_right2                    | Object | 关键点 |                                                              |
| contour_right3                    | Object | 关键点 |                                                              |
| contour_right4                    | Object | 关键点 |                                                              |
| contour_right5                    | Object | 关键点 |                                                              |
| contour_right6                    | Object | 关键点 |                                                              |
| contour_right7                    | Object | 关键点 |                                                              |
| contour_right8                    | Object | 关键点 |                                                              |
| contour_right9                    | Object | 关键点 |                                                              |
| contour_right10                   | Object | 关键点 |                                                              |
| contour_right11                   | Object | 关键点 |                                                              |
| contour_right12                   | Object | 关键点 |                                                              |
| contour_right13                   | Object | 关键点 |                                                              |
| contour_right14                   | Object | 关键点 |                                                              |
| contour_right15                   | Object | 关键点 |                                                              |
| contour_right16                   | Object | 关键点 |                                                              |
| left_eyebrow_left_corner          | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/5leb.jpg) |
| left_eyebrow_upper_left_quarter   | Object | 关键点 |                                                              |
| left_eyebrow_upper_middle         | Object | 关键点 |                                                              |
| left_eyebrow_upper_right_quarter  | Object | 关键点 |                                                              |
| left_eyebrow_upper_right_corner   | Object | 关键点 |                                                              |
| left_eyebrow_lower_left_quarter   | Object | 关键点 |                                                              |
| left_eyebrow_lower_middle         | Object | 关键点 |                                                              |
| left_eyebrow_lower_right_quarter  | Object | 关键点 |                                                              |
| left_eyebrow_lower_right_corner   | Object | 关键点 |                                                              |
| right_eyebrow_upper_left_corner   | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/7reb.jpg) |
| right_eyebrow_upper_left_quarter  | Object | 关键点 |                                                              |
| right_eyebrow_upper_middle        | Object | 关键点 |                                                              |
| right_eyebrow_upper_right_quarter | Object | 关键点 |                                                              |
| right_eyebrow_right_corner        | Object | 关键点 |                                                              |
| right_eyebrow_lower_left_corner   | Object | 关键点 |                                                              |
| right_eyebrow_lower_left_quarter  | Object | 关键点 |                                                              |
| right_eyebrow_lower_middle        | Object | 关键点 |                                                              |
| right_eyebrow_lower_right_quarter | Object | 关键点 |                                                              |
| nose_bridge1                      | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/2n%20.jpg) |
| nose_bridge2                      | Object | 关键点 |                                                              |
| nose_bridge3                      | Object | 关键点 |                                                              |
| nose_tip                          | Object | 关键点 |                                                              |
| nose_left_contour1                | Object | 关键点 |                                                              |
| nose_left_contour2                | Object | 关键点 |                                                              |
| nose_left_contour3                | Object | 关键点 |                                                              |
| nose_left_contour4                | Object | 关键点 |                                                              |
| nose_left_contour5                | Object | 关键点 |                                                              |
| nose_middle_contour               | Object | 关键点 |                                                              |
| nose_right_contour1               | Object | 关键点 |                                                              |
| nose_right_contour2               | Object | 关键点 |                                                              |
| nose_right_contour3               | Object | 关键点 |                                                              |
| nose_right_contour4               | Object | 关键点 |                                                              |
| nose_right_contour5               | Object | 关键点 |                                                              |
| left_eye_left_corner              | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/6le.jpg) |
| left_eye_upper_left_quarter       | Object | 关键点 |                                                              |
| left_eye_top                      | Object | 关键点 |                                                              |
| left_eye_upper_right_quarter      | Object | 关键点 |                                                              |
| left_eye_right_corner             | Object | 关键点 |                                                              |
| left_eye_lower_right_quarter      | Object | 关键点 |                                                              |
| left_eye_bottom                   | Object | 关键点 |                                                              |
| left_eye_lower_left_quarter       | Object | 关键点 |                                                              |
| left_eye_pupil                    | Object | 关键点 |                                                              |
| left_eye_center                   | Object | 关键点 |                                                              |
| right_eye_left_corner             | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/8re.jpg) |
| right_eye_upper_left_quarter      | Object | 关键点 |                                                              |
| right_eye_top                     | Object | 关键点 |                                                              |
| right_eye_upper_right_quarter     | Object | 关键点 |                                                              |
| right_eye_right_corner            | Object | 关键点 |                                                              |
| right_eye_lower_right_quarter     | Object | 关键点 |                                                              |
| right_eye_bottom                  | Object | 关键点 |                                                              |
| right_eye_lower_left_quarter      | Object | 关键点 |                                                              |
| right_eye_pupil                   | Object | 关键点 |                                                              |
| right_eye_center                  | Object | 关键点 |                                                              |
| mouth_left_corner                 | Object | 关键点 | ![img](http://bj-mc-prod-asset.oss-cn-beijing.aliyuncs.com/wiki-pic/4m.jpg) |
| mouth_upper_lip_left_contour1     | Object | 关键点 |                                                              |
| mouth_upper_lip_left_contour2     | Object | 关键点 |                                                              |
| mouth_upper_lip_left_contour3     | Object | 关键点 |                                                              |
| mouth_upper_lip_left_contour4     | Object | 关键点 |                                                              |
| mouth_right_corner                | Object | 关键点 |                                                              |
| mouth_upper_lip_right_contour1    | Object | 关键点 |                                                              |
| mouth_upper_lip_right_contour2    | Object | 关键点 |                                                              |
| mouth_upper_lip_right_contour3    | Object | 关键点 |                                                              |
| mouth_upper_lip_right_contour4    | Object | 关键点 |                                                              |
| mouth_upper_lip_top               | Object | 关键点 |                                                              |
| mouth_upper_lip_bottom            | Object | 关键点 |                                                              |
| mouth_lower_lip_right_contour1    | Object | 关键点 |                                                              |
| mouth_lower_lip_right_contour2    | Object | 关键点 |                                                              |
| mouth_lower_lip_right_contour3    | Object | 关键点 |                                                              |
| mouth_lower_lip_left_contour1     | Object | 关键点 |                                                              |
| mouth_lower_lip_left_contour2     | Object | 关键点 |                                                              |
| mouth_lower_lip_left_contour3     | Object | 关键点 |                                                              |
| mouth_lower_lip_top               | Object | 关键点 |                                                              |
| mouth_lower_lip_bottom            | Object | 关键点 |                                                              |