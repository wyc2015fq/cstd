# HALCON算子函数——Chapter 17 : Tools

2015年07月29日 20:18:05 [龙舞天涯](https://me.csdn.net/u011135902) 阅读数：3335



## **Chapter_17:Tools**

### **17.1  2D-Transformations**

#### **1. affine_trans_pixel**

  功能：对像素坐标轴进行任意的仿射二维变换。

#### **2. affine_trans_point_2d**

  功能：对点进行任意的最简二维变换

#### **3. bundle_adjust_mosaic**

  功能：对一幅图像的嵌合体采取一系列调整。

#### **4. hom_mat2d_compose**

  功能：将两种相同类型二维变换矩阵相乘。

#### **5. hom_mat2d_determinant**

  功能：计算一个同质的二维变换矩阵的行列式。

#### **6. hom_mat2d_identity**

  功能：构建二维变换同样的同质变换矩阵。

#### **7. hom_mat2d_invert**

  功能：插入一个同质二维变换矩阵。

#### **8. hom_mat2d_rotate**

  功能：为一个同质二维变换矩阵添加一个循环。

#### **9. hom_mat2d_rotate_local**

  功能：为一个同质二维变换矩阵添加一个循环。

#### **10. hom_mat2d_scale**

   功能：为一个同质二维变换矩阵添加一个缩放。

#### **11. hom_mat2d_scale_local**

   功能：为一个同质二维变换矩阵添加一个缩放。

#### **12. hom_mat2d_slant**

   功能：为一个同质二维变换矩阵添加一个斜面。

#### **13. hom_mat2d_slant_local**

   功能：为一个同质二维变换矩阵添加一个斜面。

#### **14. hom_mat2d_to_affine_par**

   功能：计算一个来自一个同质二维变换矩阵的仿射变换参数。

#### **15. hom_mat2d_translate**

   功能：为一个同质二维变换矩阵添加一个旋转。

#### **16. hom_mat2d_translate_local**

   功能：为一个同质二维变换矩阵添加一个旋转。

#### **17. hom_mat2d_transpose**

   功能：将一个同质二维变换矩阵转置。

#### **18. hom_mat3d_project**

   功能：给一个二维投影变换矩阵投影一个仿射三维变换矩阵。

#### **19. hom_vector_to_proj_hom_mat2d**

   功能：根据给定点的映射计算一个同质变换矩阵。

#### **20. proj_match_points_ransack**

   功能：通过找到两副图像中点与点之间的映射计算一个投影变换矩阵。

#### **21. projective_trans_pixel**

   功能：利用一个同质投影变换矩阵表示像素坐标轴。

#### **22. projective_trans_point_2d**

   功能：利用一个投影变换矩阵表示一个同质二维点。

#### **23. vector_angle_to_rigid**

   功能：从点和角度方面计算一个严格的仿射变换。

#### **24. vector_field_to_hom_mat2d**

   功能：根据位移矢量字段获取一个最接近的近似图。

#### **25. vector_to_hom_mat2d**

   功能：根据点与点间的映射获取一个最接近的近似图

#### **26. vector_to_proj_hom_mat2d**

   功能：利用给定点的映射计算一个映射变换矩阵。

#### **27. vector_to_rigid**

   功能：根据点的映射获取一个近似严格的仿射变换。

#### **28. vector_to_similarity**

   功能：根据点的映射获取一个近似的相似变换。

 

### **17.2  3D-Transformations**

#### **1. affine_trans_point_3d**

  功能：对点运用一个随即仿射三维变换。

#### **2. convert_pose_type**

  功能：改变一个三维模式的表示类型。

#### **3. create_pose**

  功能：创建一个三维模式。

#### **4. get_pose_type**

  功能：获取一个三维模式的表示类型。

#### **5. hom_mat3d_compose**

  功能：将两个同质三维变换矩阵相乘。

#### **6. hom_mat3d_identity**

  功能：构建三维变换同样的同质变换矩阵。

#### **7. hom_mat3d_invert**

  功能：插入一个同质三维变换矩阵。

#### **8. hom_mat3d_rotate**

  功能：为一个同质三维变换矩阵添加一个循环。

#### **9. hom_mat3d_rotate_local**

  功能：为一个同质三维变换矩阵添加一个循环。

**10. hom_mat3d_scale**

  功能：为一个同质三维变换矩阵添加一个缩放。

**11. hom_mat3d_scale_local**

  功能：为一个同质三维变换矩阵添加一个缩放。

**12. hom_mat3d_to_pose**

   功能：将一个同质变换矩阵转换为一个三维模式。

**13. hom_mat3d_translate**

   功能：为一个同质三维变换矩阵添加一个旋转。

**14. hom_mat3d_translate_local**

   功能：为一个同质三维变换矩阵添加一个旋转。

**15_.pose_to_hom_mat3d**

   功能：将一个三位模式转换为一个同质变换矩阵。

**16. read_pose**

   功能：从一个文本文件中读取一个三维模式。

**17. set_origin_pose**

   功能：转换一个三位模式的原点。

**18. write_pose**

   功能：将一个三维模式写入一个文本文件。

 

**17.3  Background-Estimator**

**1. close_all_bg_esti**

  功能：清除所有的背景评估数据集。

**2. close_bg_esti**

  功能：清除背景估测数据集。

**3. create_bg_esti**

  功能：为背景评估创建和初始化一个数据集。

**4. get_bg_esti_params**

  功能：返回数据集的参数。

**5. give_bg_esti**

  功能：返回估测背景图像。

**6. run_bg_esti**

  功能：评估背景并返回前景区域。

**7. set_bg_esti_params**

  功能：改变数据集的参数。

**8. update_bg_esti**

  功能：改变估测背景图像。

 

**17.4  Barcode**

**1. clear_all_bar_code_models**

  功能：清除所有条形码模型，释放其分配的存储空间。

**2. clear_bar_code_model**

  功能：清除一个条形码模型，释放相应的存储空间。

**3. create_bar_code_model**

  功能：创建一个条形码阅读器模型。

**4. find_bar_code**

  功能：检测和读取一幅图像中条形码符号。

**5. get_bar_code_object**

  功能：访问创建在搜寻或条形码符号解码过程中的对象图标。

**6. get_bar_code_param**

  功能：获取一个或多个描述条形码模式的参数。

**7. get_bar_code_result**

  功能：获取字母数字混合编码的结果，其是在条形码符号解码过程中累计的。

**8. set_bar_code_param**

  功能：设置条形码模型的选定参数。

 

**17.5  Calibration**

**1. caltab_points**

  功能：从校准板说明文件中读取标志中心点。

**2. cam_mat_to_cam_par**

  功能：计算从一个相机矩阵获取的内部相机参数。

**3. cam_par_to_cam_mat**

  功能：从相机内部参数计算一个相机矩阵。

**4. camera_calibration**

  功能：决定同时发生的最小化程序的所有相机参数。

**5. change_radial_distortion_cam_par**

  功能：根据与特殊放射失真相一致决定新的相机参数。

**6. change_radial_distortion_contours_xld**

  功能：改变了轮廓(contour)的放射失真。

**7. change_radial_distortion_image**

  功能：改变一幅图像的放射失真。

**8. contour_to_world_plane_xld**

  功能：将一个XLD轮廓(contour)转换为一个坐标系统中平面Z为零。

**9. create_caltab**

  功能：创建一个描述文件和附文件的校准板。

**10. disp_caltab**

   功能：投射和视觉化图像中校准板的三维模型。

**11. find_caltab**

   功能：分割和标准化图像中的校准板区域。

**12. find_marks_and_pose**

   功能：从图像中提取二维校准标志和为外部计算机参数计算内部数值。

**13. gen_caltab**

   功能：创建一个校准板说明文件和相应的附文件。

**14. gen_image_to_world_plane_map**

   功能：创建一个投射图，其描述图像平面与坐标轴系统中平面Z为零之间的映射。

**15. gen_radial_distortion_map**

  功能：创建一个投射图，其描述图像与其相应正在改变的放射失真间的映射。

**16. get_circle_pose**

   功能：从一个圆周相应的二维投射中决定它的三维模式。

**17. get_line_of_sight**

   功能：计算相应于图像中一个点的视线。

**18. get_rectangle_pose**

   功能：从一个矩形相应的二维投射中决定它的三维模式。

**19. hand_eye_calibration**

   功能：执行一个手---眼校准。

**20. image_points_to_world_plane**

   功能：将图像中的点转换到坐标轴平面Z为零上。

**21. image_to_world_plane**

   功能：通过将一副图像转换为坐标轴系统中平面Z为零而矫正图像。

**22. project_3d_point**

   功能：将三维点投射到子像素图像坐标。

**23. radiometric_self_calibration**

   功能：执行一个相机的辐射测量的自校准。

**24. read_cam_par**

   功能：从文本文件中读取内部相机参数。

**25. sim_caltab**

   功能：根据校准板模拟一幅图像。

**26. stationary_camera_self_calibration**

   功能：投射一个静止投射相机的自校准。

**27. write_cam_par**

   功能：将内部相机参数写入文本文件中。

 

**17.6  Datacode**

**1. clear_all_data_code_2d_models**

  功能：清除所有的二维数据模型并释放它们分配的存储空间。

**2. clear_data_code_2d_model**

  功能：清除一个二维数据模型并释放它分配的存储空间。

**3. create_data_code_2d_model**

  功能：创建一个二维数据编码类的模式。

**4. find_data_code_2d**

  功能：检测和读取一副图像或测试的二维数据编码模式中的二维数据编码符号。

**5. get_data_code_2d_objects**

  功能：查询搜索二维数据编码符号过程中创建的对象的图标。

**6. get_data_code_2d_param**

  功能：获取一个或多个描述二维数据编码模型的参数。

**7. get_data_code_2d_results**

  功能：获取字母数字混合编码的结果，其是在搜索二维数据编码符号过程中累计的。

**8. query_data_code_2d_params**

  功能：为一个给定二维数据编码模型获取通用参数或对象的名字，其也可用于其他的二维数据编码模型中。

**9. read_data_code_2d_model**

  功能：从一个文件中读取一个二维数据编码模型并新建一个模型。

**10. set_data_code_2d_param**

   功能：设置二维数据编码模型的选定参数。

**11. write_data_code_2d_model**

   功能：将一个二维数据编码模型写入一个文件。

 

**17.7  Fourier-Descriptor**

**1. abs_invar_fourier_coeff**

  功能：根据起始点的位移标准化傅里叶系数。

**2. fourier_1dim**

  功能：计算一个参数化的元组的傅里叶系数。

**3. fourier_1dim_inv**

  功能：空间傅里叶变换（傅里叶逆变换）。

**4. invar_fourier_coeff**

  功能：傅里叶系数标准化。

**5. match_fourier_coeff**

  功能：两个元组的相似性。

**6. move_contour_orig**

  功能：将原点变换到引力的中心。

**7. prep_contour_fourier**

  功能：参数化传输的元组。

 

**17.8  Function**

**1. abs_funct_1d**

  功能：Y值的绝对值。

**2. compose_funct_1d**

  功能：组合两个函数。

**3. create_funct_1d_array**

  功能：从Y值的序列中创建一个函数。

**4. create_funct_1d_pairs**

  功能：从（X，Y）集合中创建一个函数。

**5. derivate_funct_1d**

  功能：计算一个函数的派生物。

**6. distance_funct_1d**

  功能：计算两个函数的间隔。

**7. funct_1d_to_pairs**

  功能：查询一个函数的（X，Y）值。

**8. get_pair_funct_1d**

  功能：根据控制点的索引查询一个函数值。

**9. get_y_value_funct_1d**

  功能：返回任意位置函数的值。

**10. integrate_funct_1d**

   功能：计算一个函数的正区域和负区域。

**11. invert_funct_1d**

   功能：计算一个函数的反转。

**12. local_min_max_funct_1d**

   功能：计算一个函数的局域最小和最大值点。

**13. match_funct_1d_trans**

   功能：计算两个函数传递参数。

**14. negate_funct_1d**

   功能：对Y值取非（反）。

**15. num_points_funct_1d**

   功能：函数控制点的数目。

**16. read_funct_1d**

   功能：从文件中读取一个函数。

**17. sample_funct_1d**

   功能：再间隔区等距取样。

**18. scale_y_funct_1d**

   功能：将Y值相乘和相加。

**19. smooth_funct_1d_gauss**

   功能：采用高斯函数平滑一个等距一维函数。

**20. smooth_funct_1d_mean**

   功能：采用平均值将一个等距一维函数平滑化。

**21. transform_funct_1d**

   功能：根据给定传递参数变换你一个函数。

**22. write_funct_1d**

   功能：将一个函数写入一个文件。

**23. x_range_funct_1d**

   功能：函数的最小和最大X值。

**24. y_range_funct_1d**

   功能：函数的最小和最大Y值。

**25. zero_crossings_funct_1d**

   功能：计算一个函数的零点。

 

**17.9  Geometry**

**1. angle_ll**

  功能：计算两条线的夹角。

**2. angle_lx**

  功能：计算一条线与垂直轴之间的角度。

**3. distance_cc**

  功能：计算两个轮廓(contour)间的距离。

**4. distance_cc_min**

  功能：计算两个轮廓(contour)间的最小距离。

**5. distance_lc**

  功能：计算一条线和一个轮廓(contour)间的距离。

**6. distance_lr**

  功能：计算一条线和一个区域间的距离。

**7. distance_pc**

  功能：计算一个点和一个轮廓(contour)间的距离。

**8. distance_pl**

  功能：计算一个点和一条线间的距离。

**9. distance_pp**

  功能：计算两个点之间的距离。

**10. distance_pr**

   功能：计算一个点和一个区域间的距离。

**11. distance_ps**

   功能：计算一个点和一条分割线间的距离。

**12. distance_rr_min**

   功能：两个相邻区域的相同像素间的最小距离。

**13. distance_rr_min_dil**

   功能：膨胀时两个区域间的最小距离。

**14. distance_sc**

   功能：计算一条分割线和一个轮廓(contour)间的距离。

**15. distance_sl**

   功能：计算一条分割线和一条线间的距离。

**16. distance_sr**

   功能：计算一条分割线和一个区域间的距离。

**17. distance_ss**

   功能：计算两条分割线间的距离。

**18. get_points_ellipse**

   功能：计算椭圆上特定角度的一个点。

**19. intersection_ll**

   功能：计算两条线的交集点（相交点）。

**20. projection_pl**

   功能：计算一条线上一个点的投影。

 

**17.10  Grid-Rectiﬁcation**

**1. connect_grid_points**

  功能：建立矫正网格的矫正点间的连接。

**2. create_rectification_grid**

  功能：建立一个附文件，描述矫正网格。

**3. find_rectification_grid**

  功能：分割图像中矫正网格区域。

**4. gen_arbitrary_distortion_map**

  功能：产生一个投射图，其描述随意扭曲图像与正确图像间的映射。

**5. gen_grid_rectification_map**

  功能：计算扭曲图像与基于规律的网格的正确的图像的映射。

 

**17.11  Hough**

**1. hough_circle_trans**

  功能：返回指定半径的圆周的Hough变换。

**2. hough_circles**

  功能：特定半径的圆周的中心。

**3. hough_line_trans**

  功能：对区域中的线进行Hough变换。

**4. hough_line_trans_dir**

  功能：利用局部方向梯度对线进行Hough变换。

**5. hough_lines**

  功能：借助Hough变化查询图像中的线，并将其返回到HNF中。

**6. hough_lines_dir**

  功能：借助采用局部方向梯度的Hough变换查询图像中的线，并将它们以正常形式返回。

**7. select_matching_lines**

  功能：选取HNF中线的集合中匹配区域最好的线。

 

**17.12  Image-Comparison**

**1. clear_all_variation_models**

  功能：释放所有变化模型（variation_model）的存储空间。

**2. clear_train_data_variation_model**

  功能：释放变化模型（variation_model）的测试数据的存储空间。

**3. clear_variation_model**

  功能：释放一个变化模型（variation_model）的存储空间。

**4. compare_ext_variation_model**

  功能：将一副图像与一个变化模型（variation_model）相比较。

**5. compare_variation_model**

  功能：将一副图像与一个变化模型（variation_model）相比较。

**6. create_variation_model**

  功能：为图像对比创建一个变化模型。

**7. get_thresh_images_variation_model**

  功能：返回阀值图像用于图像对比。

**8. get_variation_model**

  功能：返回图像用于图像对比。

**9. prepare_direct_variation_model**

  功能：为图像对比准备一个变化模型。

**10. prepare_variation_model**

   功能：为图像对比准备一个变化模型。
**11. read_variation_model**

   功能：从一个文件中读取一个变化模型。

**12. train_variation_model**

   功能：测试一个变化模型。

**13. write_variation_model**

   功能：将一个变化模型写入文件。

 

**17.13  Kalman-Filter**

**1. filter_kalman**

  功能：借助Kalman（卡尔曼）滤波器估测系统的当前状态。

**2. read_kalman**

  功能：读取一个卡尔曼滤波器的说明文件。

**3. sensor_kalman**

  功能：卡尔曼滤波器测量值的交互式输入。

**4. update_kalman**

  功能：读取一个卡尔曼滤波器的更新文件。

 

**17.14  Measure**

**1. close_all_measures**

  功能：清除所有测试对象。

**2. close_measure**

  功能：清除一个测试对象。

**3. fuzzy_measure_pairing**

  功能：提取与矩形或环状弧垂直的直线边缘。

**4. fuzzy_measure_pairs**

  功能：提取与矩形或环状弧垂直的直线边缘。

**5. fuzzy_measure_pos**

  功能：提取与矩形或环状弧垂直的直线边缘。

**6. gen_measure_arc**

  功能：垂直与环状弧的直线边缘的提取。

**7. gen_measure_rectangle2**

  功能：垂直与矩形的直线边缘的提取。

**8. measure_pairs**

  功能：提取与矩形或环状弧垂直的直线边缘。

**9. measure_pos**

  功能：提取与矩形或环状弧垂直的直线边缘。

**10. measure_projection**

   功能：提取垂直于一个矩形或环状弧的灰度值轮廓(contour)。

**11. measure_thresh**

   功能：提取沿着一个矩形或环状弧，特殊灰度值的点。

**12. reset_fuzzy_measure**

   功能：重置一个模糊元函数。

**13. set_fuzzy_measure**

   功能：指定一个模糊元函数。

**14. set_fuzzy_measure_norm_pair**

   功能：为边缘匹配指定一个规范化模糊元函数。

**15. translate_measure**

   功能：转化（解释）一个测试对象。

 

**17.15  OCV（**Open_Circuit_Voltage_|_光学字符校验**）**

**1. close_all_ocvs**

  功能：关闭所有OCV工具。

**2. close_ocv**

  功能：关闭一个OCV工具。

**3. create_ocv_proj**

  功能：创建一个基于灰度值突出的新的OCV工具。

**4. do_ocv_simple**

  功能：利用一个OCV工具查证一个模式。

**5. read_ocv**

  功能：从文件中读取一个OCV工具。

**6. traind_ocv_proj**

  功能：测试一个OCV工具。

**7. write_ocv**

  功能：将一个OCV工具保存到文件。

 

**17.16  Shape-from**

**1. depth_from_focus**

  功能：利用多倍聚焦灰度级提取高度（厚度）。

**2. estimate_al_am**

  功能：估测一个平面的反射率和反射光的数目。

**3. estimate_sl_al_lr**

  功能：估测一个光源的倾斜度和一个平面的反射率。

**4. estimate_sl_al_zc**

  功能：估测一个光源的倾斜度和一个平面的反射率。

**5. estimate_tilt_lr**

  功能：估测一个光源的倾斜。

**6. estimate_tilt_zc**

  功能：估测一个光源的倾斜。

**7. phot_stereo**

  功能：根据至少三个灰度值的图像来重建一个平面。

**8. select_grayvalues_from_channels**

  功能：利用索引图像选择一个多通道图像的灰度值。

**9. sfs_mod_lr**

  功能：从一个灰度值图像重建一个平面。

**10. sfs_orig_lr**

   功能：从一个灰度值图像重建一个平面。

**11. sfs_pentland**

   功能：从一个灰度值图像重建一个平面。

**12. shade_height_field**

   功能：遮蔽一个突起的字段。

 

**17.17  Stereo**

**1. binocular_calibration**

  功能：决定一个双目视觉立体系统的所有相机参数。

**2. binocular_disparity**

  功能：计算一个矫正图像对的不均衡。

**3. binocular_distance**

  功能：计算一个矫正立体图像对的间隔值。

**4. disparity_to_distance**

  功能：将不均衡值转换为矫正双目视觉立体系统中的间隔值。

**5. disparity_to_point_3d**

  功能：将一个图像点和它的不均衡值转换为一个矫正立体系统中的三维点。

**6.distance_to_disparity**

  功能：将一个间隔值转换为一个矫正立体系统中的一个不均衡值。

**7. essential_to_fundamental_matrix**

  功能：计算一个从原始矩阵衍生而来的基本矩阵。

**8. gen_binocular_proj_rectification**

  功能：计算弱双目视觉立体系统图像的投射矫正值。

**9. gen_binocular_rectification_map_**

  功能：创建传输图，其描述从一个双目相机到一个普通的矫正图像面的图像的映射。

**10. gen_binocular_rectification_map**

   功能：从一个双目相机系统视觉中两条线的交点中获取一个三维点。

**11. match_essential_matrix_ransack**

   功能：通过自动发掘图像点间对应关系来计算立体图像对的原始（本质）矩阵。

**12. match_fundamental_matrix_ransack**

   功能：通过自动发掘图像点间对应关系来计算立体图像对的基本矩阵。

**13. match_rel_pose_ransack**

   功能：通过自动发掘图像点间对应关系来计算两个相机间的相对方位。

**14. reconst3d_from_fundamental_matrix**

   功能：计算基于基本矩阵的点的投影的三维重建。

**15. rel_pose_to_fundamental_matrix**

   功能：计算两个相机相关方向中获取的基本矩阵。

**16. vector_to_essential_matrix**

   功能：计算给定图像点间映射和已知相机矩阵的原始矩阵，重建三维点。

**17. vector_to_fundamental_matrix**

  功能：计算给定图像点间映射的集合的基本矩阵，重建三维点。

**18. vector_to_fundamental_matrix**

   功能：计算给定图像点间对应关系和已知相机参数的两个相机的相对方位，重建三维点。

 

**17.18  Tools-Legacy**

**1. decode_1d_bar_code**

  功能：一个条形码的顺序解码。

**2. decode_2d_bar_code**

  功能：解码二维条形码数据。

**3. discrete_1d_bar_code**

  功能：从元素宽度创建一个离散条形码。

**4. find_1d_bar_code**

  功能：搜索一幅图像中的一个条形码。

**5. find_1d_bar_code_region**

  功能：搜索一幅图像中的多种条形码。

**6. find_1d_bar_code_scanline**

  功能：搜索一幅图像中的一个条形码。

**7. find_2d_bar_code**

  功能：搜索可能包括一个二维条形码的区域。

**8. gen_1d_bar_code_descry**

  功能：创建一个一维条形码的说明。

**9. gen_1d_bar_code_descr_gen**

  功能：创建一个一维条形码的类属描述。

**10. gen_2d_bar_code_descry**

   功能：创建一个二维条形码的类属描述。

**11. get_1d_bar_code**

   功能：提取一个条形码中元素的宽度。

**12. get_1d_bar_code_scanline**

   功能：提取一个条形码区域中元素的宽度。

**13. get_2d_bar_code_**

   功能：提取一个条形码区域（“数据矩阵符号”）中数据元素（在ECC200：“模块”中）的值。_

**14. get_2d_bar_code_pos**

   功能：提取一个条形码区域（“数据矩阵符号”）中数据元素（在ECC200：“模块”中）的数值和它们在图像中的位置。