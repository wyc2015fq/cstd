# css3 2d与3d变形 - weixin_33985507的博客 - CSDN博客
2016年11月22日 17:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
2d变形
transform: none | transform-function
1.rotate(角度)
Rotate（45deg）往右旋转
Rotate（-60deg）往左旋转
2.translate(50px)平移
x轴偏移了50px，y轴没有偏移
translate(50px,20%)
translateX(50px)
translateY(50px)
3.scale()缩放
scale(1.2)x轴和y轴同时缩放
scale(1,1.2)x轴没有缩放  y轴缩放了1.2倍
scaleX(1.2)
scaleY(1,2)
4.skew(30deg|30deg);倾斜
y轴往x轴方向倾斜的角度  x轴往y轴方向倾斜的角度
5.transform-origin: 0 0;
基于那个点进行变换
3d变形
perspective: 200px;离得越近透视程度越明显
perspective-origin: 50% 50%;透视的角度（从哪个视线看）
3d效果：
移动
translate3d(x轴位移 ,y轴位移, z轴上的位移);
z轴方向正方向移动了（也就是说离得近了），看起来就是变大的效果
translateX()  translateY()  translateZ()
缩放
transform: scale3d(X轴方向放大,y轴方向放大,z轴方向放大); z轴不影响盒子的大小
transform:scale3d(1.2,1.2,2) translateZ(50px);scale3d和translateZ配合使用scale3d的z就管用了
scaleX() scaleY() scaleZ()
旋转
transform: rotate3d(1,0,0,45deg);
第一个参数：x轴
第二个参数：y轴
第三个参数：z轴
第四个参数：角度
也可以拆分成
RotateX(角度)
RotateY(角度)
RotateZ(角度)
transform-style:preserve-3d | flat;
Flat是默认值，是扁平化的意思
preserve-3d让transform元素保留内部3d空间
backface-visibility: hidden | visible；
visible背面可见  是默认值
hidden 背面不可见
