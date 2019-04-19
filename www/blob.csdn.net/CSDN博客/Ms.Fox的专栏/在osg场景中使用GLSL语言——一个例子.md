# 在osg场景中使用GLSL语言——一个例子 - Ms.Fox的专栏 - CSDN博客
2014年06月16日 11:22:19[lulu-lu](https://me.csdn.net/smbluesky)阅读数：519
个人分类：[glsl](https://blog.csdn.net/smbluesky/article/category/2336335)
1、由于环境变量神马的都已经设置好了，基本上要使用osg，只需要在添加连接器中的附加依赖项中添加：
kernel32.lib
user32.lib
gdi32.lib
winspool.lib
shell32.lib
ole32.lib
oleaut32.lib
uuid.lib
comdlg32.lib
advapi32.lib
OpenThreadsd.lib
osgd.lib
osgDBd.lib
osgUtild.lib
osgGAd.lib
osgViewerd.lib
osgTextd.lib
glu32.lib
opengl32.lib
2、直接添加代码如下：
// 1.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
static const char* vertSource=
{
"varying vec3 normal;\n"
"void main()\n"
"{\n"
"     normal=normalize(gl_NormalMatrix*gl_Normal);\n"
"     gl_Position=ftransform();\n"
"}\n"
};
static const char* fragSource={
"uniform vec4 mainColor;\n"
"varying vec3 normal;\n"
"void main()\n"
"{\n"
"    float intensity=dot(vec3(gl_LightSource[0].position),normal);\n"
"    if(intensity>0.95) gl_FragColor=mainColor;\n"
"    else if(intensity>0.5)  gl_FragColor=vec4(0.6,0.3,0.3,1.0);\n" 
"    else if(intensity>0.25)  gl_FragColor=vec4(0.4,0.2,0.2,1.0);\n" 
"    else gl_FragColor=vec4(0.2,0.1,0.1,1.0);\n" 
"}\n"
};
class ColorCallback:public osg::Uniform::Callback
{
public:
ColorCallback():_incRed(false){}
virtual void operator()(osg::Uniform * uniform,osg::NodeVisitor* nv)
{
if (!uniform) return;
osg::Vec4 color;
uniform->get(color);
if (_incRed==true)
{
if (color.x()<1.0) color.x()+=0.01;
else _incRed=false;
}
else
{
if (color.x()>0.0) color.x()-=0.01;
else _incRed=true;
}
uniform->set(color);
}
protected:
bool _incRed;
};
void createShaders(osg::StateSet & ss)
{
osg::ref_ptr<osg::Shader> vertShader=new osg::Shader(osg::Shader::VERTEX,vertSource);
osg::ref_ptr<osg::Shader> fragShader=new osg::Shader(osg::Shader::FRAGMENT,fragSource);
osg::ref_ptr<osg::Program> program=new osg::Program;
program->addShader(vertShader.get());
program->addShader(fragShader.get());
osg::ref_ptr<osg::Uniform> mainColor=new osg::Uniform("mainColor",osg::Vec4(1.0,0.5,0.5,1.0));
mainColor->setUpdateCallback(new ColorCallback);
ss.addUniform(mainColor.get());
ss.setAttributeAndModes(program.get());
}
int _tmain(int argc, _TCHAR* argv[])
{
osg::ArgumentParser arguments(&argc,argv);
osg::Node *model=osgDB::readNodeFiles(arguments);
if (!model) model=osgDB::readNodeFile("cow.osg");
createShaders(*(model->getOrCreateStateSet()));
osgViewer::Viewer a;
a.setSceneData(model);
return a.run();
}
效果截图：
![](https://img-blog.csdn.net/20140616112129515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc21ibHVlc2t5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
具体的参考文档是
http://download.csdn.net/detail/smbluesky/7504667
可参考该文档的6.4章节，里面有具体的介绍。
