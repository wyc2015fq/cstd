# osg的灯光使用 - Ms.Fox的专栏 - CSDN博客
2014年06月12日 10:56:31[lulu-lu](https://me.csdn.net/smbluesky)阅读数：3447
在一个场景中添加光源主要包括以下步骤：
      1、指定场景模型的法线 ——只有设有单位法线才会正确显示光照。如果没有指定法线，可以用osgUtil::SmoothingVisitor自动生成法线。可能缩放变换会造成光照结果过于明亮或暗淡，要在StateSet中允许法线的重缩放模式。
**state->setMode(GL_RESCALE_NORMAL,osg::StateAttribute::ON);**
        上面所述是均匀缩放，面对非均匀缩放变换，则需要允许法线归一化模式，但会耗费大量的时间，编程时要尽量避免。归一化模式代码：
** state->setMode(GL_NORMALIXE,osg::StateAttribute::ON);**
      2、允许光照并设置光照状态——在OSG中获得光照效果，需要允许光照并至少允许一个光源。在OSG中，最多允许8个光源。下面代码表示，允许光照，并且允许了两个光源。
 state->setMode(GL_LIGHTING,osg::StateAttribute::ON);
state->setMode(GL_LIGHT0,osg::StateAttribute::ON);
state->setMode(GL_LIGHT1,osg::StateAttribute::ON);
      3、指定光源属性并关联到场景图形——在场景中添加一个光源，可以创建osg::Light对象然后定义光源参数，然后将osg::Light添加到一个osg::LightSource节点中，并将LightSource节点添加到场景图形。
**    osg::ref_ptr<osg::Light> light=new osg::Light();**
**         light->setLightNum(0);      //对于上面的第一个光源：GL_LIGHT0**
**         light->setDirection(osg::Vec3(0.0f,0.0f,-1.0f));**
         ……设置灯光其他参数
         //创建光源
         osg::ref_ptr<osg::LightSource> lightSource=new osg::LightSource();
         lightSource->setLight(light.get());
**      //关联到场景**
**         osg::ref_ptr<osg::Group> lightRoot=new osg::Group();**
**         lightRoot->addChild(node);//添加模型节点**
**         lightRoot->addChild(lightSource.get());**
**具体代码如下：**
**![](https://img-blog.csdn.net/20140612105502703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc21ibHVlc2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140612105510703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc21ibHVlc2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140612105516890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc21ibHVlc2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
