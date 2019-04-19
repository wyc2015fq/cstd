# 使用Kinect2作为Oculus游戏应用的输入设备 - 逍遥剑客 - CSDN博客
2016年01月04日 22:54:09[xoyojank](https://me.csdn.net/xoyojank)阅读数：5237
注: 文章写于2015年8月, 目前VR游戏Demo已经完结, 所以把上一次预研的一些经验分享出来, 希望对大家有所帮助
# 背景
初接触Oculus时, 从网上下载了一大堆的Demo来体验, 但是, 操作体验大都比较差, 特别是FPS类. 这也让我们意识到, 对于VR游戏, 最大的挑战还不是显示方式的变化, 而是交互方式. 在一个沉浸式的环境中, 最自然的交互就是最完美的方式. 其中基本的需求, 就是可以使用双手跟VR中的虚拟环境进行交互. 这么一来, 首先键鼠或手柄就被排除掉了, 我们只好针对市面上的一些输入设备, 挨个进行评估实验: 
- Wiimote: 只能检测运动和方向, 无法准确定位双手的位置 
- Leap Motion: 粘在Oculus上可以模拟出双手, 但是识别范围太小, 骨骼的稳定性比较差, 严重影响体验 
- Razer Hydra: 可以获取双手的空间位置和旋转, 加上两个手柄上的按键也能触发一些状态切换, 算是看起来还不错的方案. 缺点是位置偏差比较大, 可能是磁场受干扰的问题 
- RealSense: 类似LeapMotion, 但是精度比较低, 导致识别出的骨骼位置抖动严重, 无法用于双手的骨骼映射
试来试去, 好像目前市面上除了高成本的动作捕捉设备, 还没有比较完美的VR输入设备可以用. 最后, 把目光转向了旁边XboxOne开发机配的Kinect2. 
因为我们组去年进行了XboxOne体感游戏的研发, 积累了一些Kinect2体感操作的经验, 就把Kinect2连接到了PC上, 看看能不能把体感操作与Oculus的VR显示结合到一起.
# 需求分析
前面也提到了, 尽量达到接近自然的交互方式, 那就需要实现这几个关键点:
- 可以在虚拟世界中显示出双手, 最好能有肢体躯干
- 虚拟空间中的双手位置与现实空间中跟头部的相对位置(包括旋转)保持一致
- 可以使用双手对虚拟世界中的物体产生影响
- 能够识别一些简单的手势, 如抓, 推, 拉, 按, 摸等等
那Kinect2提供的数据或者功能有哪些呢? 
- 30帧/s的Color/Depth/BodyIndex/IR数据
- 身体骨骼位置和朝向(比较不稳定, 会抖动)
- 双手的三种状态识别, 正好对应 石头 剪刀 布(误识别率较高)
- 其它诸如语音之类的功能暂时用不上
像LeapMotion那样每根手指单独识别是做不到了, 退而求其次, 只能在交互设计上就去规避过小元素, 使用整个手掌做为交互 
双手的骨骼位置可以为我们提供双手的空间定位, 而三种状态可以参考我们在XboxOne体感游戏中的UI交互经验, 把抓住拖动之类的手势利用起来
# 实现细节
## 双手肢体的绘制
因为Kinect API已经提供了人体骨骼的变换信息, 那自然而然的我们就想在游戏中绑定到一个蒙皮模型上 
![这里写图片描述](https://img-blog.csdn.net/20160104223127602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
最终我们也在UE4中实现了, 但是体验下来非常不满意, 为什么呢?
- Kinect中获取的骨骼变换信息会频繁抖动, 如果不进行处理, 会像抽风一样
- 如果对骨骼变换数据进行稳定性的过滤处理, 会增加响应延迟, 导致虚拟肢体的动作比实际总是慢半拍
- 不同体形的的人的映射到同一模型的效果会有问题, 比如想象一下一个身材高大的人在Oculus中看到自己胳膊变短了是什么感觉. 这会影响基于直觉和经验的空间位置判断
那还有什么别的方法去实现双手肢体的绘制吗? 在使用KinectStudio调试时, 发现3D视图下的深度呈现比较有意思: 
![这里写图片描述](https://img-blog.csdn.net/20160104223154471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其实这就是深度数据(DepthBuffer)映射到3D空间里的离散的点, 这里我把这种表示方式称之为 “点云(PointCloud)” 
![这里写图片描述](https://img-blog.csdn.net/20160104223214569?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
于是突发奇想, 在虚拟空间使用点云表现自己的躯体, 双手手指的动作也可以精确地映射过去. 那么, 这可行吗?
- 延迟: 由于DepthBuffer是硬件采集的原始数据, 是没有经过处理的, 不存在中间的数据处理时间(延迟), 所以在响应速度上肯定是非常理想的, 可以控制在70ms左右(Kinect2硬件固定60ms)
- 数据量: DepthBuffer的分辨率是512x424, 也就是需要映射到21万多个顶点, 虽说有点多, 但也在可接受范围内, 实在不行可以隔行取点, 以最终效果需求为准
- UE4点云渲染: 每帧根据DepthBuffer计算出对应的VertexBuffer, 构建DynamicMesh进行绘制, PrimitiveType使用PointList 
![这里写图片描述](https://img-blog.csdn.net/20160104223303029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后再根据BodyIndex数据剔除掉周围环境和其它人的点, 就完美的把自己映射到UE4的3D场景中了. 下图加了个简单的材质, 顶点法线使用地形中常用的SlopeBased方法计算 
![这里写图片描述](https://img-blog.csdn.net/20160104223333442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 点云坐标系对齐
有了点云的躯体了, 怎么把它 “装”在虚拟世界中的头下面呢? 
由于Kinect, Oculus, UE4相当于是三个不同的坐标系, 如果要把点云映射到Oculus视角下身体的位置, 需要一些坐标映射和转换.
- UE4已经默认集成了Oculus的支持, 所以这两个坐标系的处理不用我们操心了, 默认Oculus头戴显示器的坐标就是UE4摄像机的位置加上PostionTracking的Offset
- 而Oculus头戴显示器的位置来源于Oculus DK2中带的CameraSensor, 这才是Oculus虚拟坐标的基准点, 只不过UE4做了变换, 把Oculus初始位置映射到了摄像机的位置上
- Kinect中的DepthBuffer映射成顶点后, 全都是CameraSpacePoint, 即Kinect设备本身就是原点. 需要注意的是, Kinect坐标与UE4坐标需要做一下转换, 对应关系为 UE4Vector = FVector(-V.Z * 100, V.X * 100, V.Y * 100)
那找到Oculus和Kinect坐标系中分别固定不动的基准点后, 把它们进行对齐, 两边的坐标系不就可以重合了? 方法很简单, 把Kinect和Oculus的CameraSensor”合体”: 
![这里写图片描述](https://img-blog.csdn.net/20160104223357405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
游戏中可以通过CameraComponent位置和Oculus的CameraOrigin计算出Sensor的世界坐标, 然后把点云对齐到这个位置即可, 通过一个可以保存配置的Offset进行偏移校正 
![这里写图片描述](https://img-blog.csdn.net/20160104223438693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 交互设计
整个交互灵感其实来源于钢铁侠电影里的全息投影的交互片段, 我们的目标就是把这种科幻片中常常出现的镜头变成现实 
![这里写图片描述](https://img-blog.csdn.net/20160104223457891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
以全息投影的感觉做为美术风格的指导方向, 结合我们日常接触最多的功能, 我们实现了5种交互控件:
- 
图片查看器: 只有一个翻页操作 
![这里写图片描述](https://img-blog.csdn.net/20160104223530354?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
视频播放器, 可以操作播放/暂停, 放大后有电影院看电影的感觉, 这也是目前VR视频应用比较常用的方式 
![这里写图片描述](https://img-blog.csdn.net/20160104223554893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
网页浏览器: 我们集成了CEF, 相当于内嵌了一个chrome, 支持HTML5的游戏. 下面的视频中我们选择了一个H5的猜单词小游戏, 支持网页上的点击操作 
![这里写图片描述](https://img-blog.csdn.net/20160104223613048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
打飞机小游戏: 这个是使用体感操作的, 虽然是一个2D平面的游戏, 但是爆炸后的碎片会落到地板上, 视觉效果还不错 
![这里写图片描述](https://img-blog.csdn.net/20160104223631577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
模型查看器: 主要是用于演示在3D空间怎么用双手比较直观地观察一个三维物体, 可以说这才是VR交互的亮点所在, 你可以从各个角度和任意大小去观察一个物体的每一个细节 
![这里写图片描述](https://img-blog.csdn.net/20160104223649081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
每个控件我们还做了统一的Tooltips的弹出动画提示, 这种3D空间的信息显示也是AR应用场景中比较常见的 
![这里写图片描述](https://img-blog.csdn.net/20160104223712529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为了更好地展示每个控件的功能, 我们把整个全息交互场景分成了前后两”层”
- 远景: 只能同时存在一个控件, 可以抓住进行拖动和缩放操作, 并进行每个控件特定的功能操作, 如网页的点击, 小游戏的手势移动等等. 
- 近景: 摆放各个功能控件, 相当于任务栏图标, 可以通过手势把它”扔”到远景的那一层上去, 相当于窗口最大化/Active状态
对远景的控件进行操作时, 双手各加了一根闪电特效, 如同释放魔法一样, 在远处控件上模拟出类似iPad的操作体验
PS: 为了不对第一人称的VR显示产生干扰, 已经把点云的头”砍”掉了 
![这里写图片描述](https://img-blog.csdn.net/20160104223732160?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
近景的交互是基于双手的”Touch”操作, 通过Kinect获取双手骨骼位置, 挂了两个碰撞体用于检测与控件之间的Overlap状态 
![这里写图片描述](https://img-blog.csdn.net/20160104223747378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 效果展示
点击播放视频(略)
# 优化
基于VertexBuffer的点云由于要进行顶点坐标计算, 十分消耗CPU, 为了节省时间, 可以把顶点计算转移到GPU, 使用静态VertexBuffer+动态VertexTexture进行Mesh的构建, 而同时带来的好处是点云不再限于Point渲染, 可以做成Particle的样子 
![这里写图片描述](https://img-blog.csdn.net/20160104223811384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20160104231029000)
# 总结
在做VR技术预研的过程中, 我们也发现三大VR设备(Oculus, Steam VR, PS VR)的发布的操作设备已经趋于一致: 双持控制器, 每个控制器都可以获取位置和旋转, 并且带有传统的按钮和摇杆. 这虽然不是最自然的交互方式, 但是也是目前在成本和功能之间的一种平衡, 后续的VR游戏开发, 操作上就可以基于这些设备做统一的设计. 
有了这个VR交互Demo的成功经验, 我们把这种交互方式也带入了正在开发的一个VR游戏Demo, 在Oculus Touch没上市之前, 这是目前我们能在VR中实现的比较好的操作体验. 个人以为, 只是显示方式的变化并不能带来游戏性上的太大变化, 双手控制器才可以让VR游戏玩法产生更多创意, 从根本上推动产生新的游戏类型和全新体验.
