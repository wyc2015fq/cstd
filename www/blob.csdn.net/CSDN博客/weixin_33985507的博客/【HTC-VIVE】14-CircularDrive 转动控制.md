# 【HTC-VIVE】14-CircularDrive:转动控制 - weixin_33985507的博客 - CSDN博客
2018年11月04日 07:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：22
![13380604-0815a3a416b84bf3.jpg](https://upload-images.jianshu.io/upload_images/13380604-0815a3a416b84bf3.jpg)
# 使用手转动圆盘：
> 
![13380604-72c21e9d88df296a.png](https://upload-images.jianshu.io/upload_images/13380604-72c21e9d88df296a.png)
CircularDrive.png
# 实现该功能的主要脚本组件：Interactable 与 CircularDrive
> 
![13380604-e57f727dc3e7e167.png](https://upload-images.jianshu.io/upload_images/13380604-e57f727dc3e7e167.png)
Compenent.png
> 
##### Interactable 组件不做介绍，主要介绍 CircularDrive 组件
> 
# CircularDrive 组件:
![13380604-9d9a8ead995edff7.png](https://upload-images.jianshu.io/upload_images/13380604-9d9a8ead995edff7.png)
Circular Drive.png
##### 可设置内容：
- Axis Of Rotation : **圆盘转动的轴向，围绕本地坐标的某个轴转动（默认X Axis)**
- Child Collider : **转动盘的子物体所拥有的Collider组件，自动获取（可以不手动设置）**
- Linear Mapping : **记录转动了多少比例，0-1之间取值，1：表示转到最大角度，0:表示转到最小角度**
- hoverLock : **勾选后，只要一直按下控制按钮，将一直可以控制圆盘转动，不勾选，当控制器手柄离开Collider,就不再控制其转动**
#### Limited Rotation:
- Limited : **勾选后，转动角度将被限制在[Min Angle,Max Angle],不勾选，则没有任何限制**
- Frozen Distance Min Max Threshold : **在冻结冻结旋转的最小最大阈值**
##### 事件:
- **On Frozen Distance Threshold()** : **在冻结j距离阈值之间的事件**
#### Limited Rotation Min:
- Min Angle : **最小转动角度值(默认 :-45)**
- Freeze On Min : **是否在转到最小转动角度值时，冻结转动，让其无法再转动**
##### 事件:
- **On Min Angle()** : **在转动到最小转动角度时，触发该事件**
#### Limited Rotation Max:
- Max Angle : **最大转动角度值(默认: 45)**
- Freeze On Max :  **是否在转到最大转动角度值时，冻结转动，让其无法再转动**
- **On Max Angle()** : **在转动到最大转动角度时，触发该事件**
- Force Start : **是否强制圆盘初始化到转动范围的某个角度值**
- Start Angle : **一开始圆盘初始化的角度值**
- Rotate Game Object : **是否让转动盘物体也一起旋转**
- Debug Path : **是否绘制手的挥动路径**
- Dbg Path Limit : **路径绘制的最大物体个数（使用不断生成颜色的小球来表示路径）**
- Debug Text : **显示转动比例与转动的3D Mesh Text (不设置，将不输出)**
- Out Angle : **转动的角度值（可用于访问，不进行设置）**
