# NGUI AnchorPoint与Camera CullingMask的结合之痛 - tkokof1的专栏 - CSDN博客

2017年09月04日 21:19:28[tkokof1](https://me.csdn.net/tkokof1)阅读数：314
个人分类：[游戏																[随性																[算法](https://blog.csdn.net/tkokof1/article/category/642709)](https://blog.csdn.net/tkokof1/article/category/642708)](https://blog.csdn.net/tkokof1/article/category/642710)


## 问题

NGUI内建的Anchor系统可以方便的定位UI(底层的一个支持结构是AnchorPoint),例如一些需要全屏显示的游戏界面一般都需要借助这项功能;另外的,对于稍复杂的UI设计,我们可能会将UI进行分层处理,一些游戏状态下我们可能也会主动显示或者隐藏其中的某些层级.而要实现这种UI的分层显示,一种自然的方式便是采用Camera的[CullingMask](https://docs.unity3d.com/ScriptReference/Camera-cullingMask.html). 

两个系统单独来看好像都没有什么问题,但是结合起来使用却容易出现岔子~

## 原因

原因在于NGUI的Anchor系统并不能很好的支持Camera CullingMask的切换,我们简单来看一段NGUI实现Anchor的代码片段:

```
public void ResetAnchors ()
{
    mAnchorsCached = true;

    leftAnchor.rect     = (leftAnchor.target)   ? leftAnchor.target.GetComponent<UIRect>()   : null;
    bottomAnchor.rect   = (bottomAnchor.target) ? bottomAnchor.target.GetComponent<UIRect>() : null;
    rightAnchor.rect    = (rightAnchor.target)  ? rightAnchor.target.GetComponent<UIRect>()  : null;
    topAnchor.rect      = (topAnchor.target)    ? topAnchor.target.GetComponent<UIRect>()    : null;

    mCam = NGUITools.FindCameraForLayer(cachedGameObject.layer);

    FindCameraFor(leftAnchor);
    FindCameraFor(bottomAnchor);
    FindCameraFor(rightAnchor);
    FindCameraFor(topAnchor);

    mUpdateAnchors = true;
}
```

代码的大意便是重置一些Anchor相关的数据,其中重要的有两处: 

第一个是 mAnchorsCached,他是缓存标记,用以标记Anchor数据是否已经重置; 

第二个则是 NGUITools.FindCameraForLayer ,该接口会返回CullingMask包含指定layer的Camera对象. 

一般而言,对于Camera CullingMask不会变动的情况,ResetAnchors的实现是没有问题的,但是正如上面所说,如果我们通过改变CullingMask来实现UI分层显示的话,某些被隐藏层级的UI可能会因为查找不到Camera而导致Anchor出错,而此时缓存标记又已设置,后面的逻辑便再不会主动重置Anchor数据,导致定位出错~

## 方法

解决方法可能有以下一些： 

一是避免使用NGUI的Anchor系统,转而自己实现UI的定位,一般来讲并不推荐. 

二是改造NGUI Anchor系统,以使其支持Camera的CullingMask变动,譬如实现某种CullingMask的变动通知机制,然后主动重置更新相应的Anchor~ 

而对于一些全屏性质的UI(需要通过整个屏幕来定位的UI),则有一个比较简单的改动方法,原理则是实现一种特定的[UIRect](https://docs.unity3d.com/ScriptReference/Camera-cullingMask.html),使其一直保持全屏大小,即便找不到对应layer的Camera也是如此,而UI则根据这个定制的UIRect来进行定位. 

一个简单的参考实现大概是这个样子:

```
// desc UIScreenRect is for handling problem NGUI anchor system does not fit Camera culling mask changes
// maintainer hugoyu

using UnityEngine;

public class UIScreenRect : UIRect
{

    Vector3[] m_invalidWorldCorners = new Vector3[] { Vector3.zero, Vector3.zero, Vector3.zero, Vector3.zero };

    public override float alpha 
    { 
        get
        {
            return 0;
        }

        set
        {
            Debug.LogWarning("[UIScreenRect]Do not support set UIScreenRect's alpha");
        }
    }

    public override float CalculateFinalAlpha(int frameID)
    {
        return 0;
    }

    public override Vector3[] localCorners 
    { 
        get
        {
            Vector3[] corners = worldCorners;
            Transform wt = cachedTransform;
            for (int i = 0; i < 4; ++i)
            {
                corners[i] = wt.InverseTransformPoint(corners[i]);
            }
            return corners;
        }
    }

    public override Vector3[] worldCorners 
    { 
        get
        {
            var anchorCam = GetAnchorCamera();
            if (anchorCam != null)
            {
                return anchorCam.GetWorldCorners(cameraRayDistance);
            }
            else
            {
                return m_invalidWorldCorners;
            }
        }
    }

    protected override void OnAnchor()
    {
    }

    protected override void OnStart()
    {
    }

    public override void SetRect(float x, float y, float width, float height)
    {
        Debug.LogWarning("[UIScreenRect]Do not support set UIScreenRect's rect");
    }

    public override bool canBeAnchored
    {
        get
        {
            return false;
        }
    }

    public override Vector3[] GetSides(Transform relativeTo)
    {
        GetAnchorCamera();
        return base.GetSides(relativeTo);
    }

    public Camera GetAnchorCamera()
    {
        if (!anchorCamera)
        {
            // when anchor camera is missing, we set to first UICamera
            if (UICamera.list.size > 0)
            {
                mCam = UICamera.list.buffer[0].cachedCamera;
            }
        }

        if (!mCam)
        {
            Debug.LogWarning("[UIScreenRect]Error to get anchor camera");
        }

        return mCam;
    }

}
```

OK,下次再见吧~

