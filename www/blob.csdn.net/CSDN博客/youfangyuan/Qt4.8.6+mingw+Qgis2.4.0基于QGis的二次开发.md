# Qt4.8.6+mingw+Qgis2.4.0基于QGis的二次开发 - youfangyuan - CSDN博客
2014年11月15日 23:28:44[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：8210
关于QGis的二次开发，大致看了一下，基本都是在VC+QT的环境下做环境部署，而且QGis的版本非常老，在mingw下直接开发搭建环境的例子少之又少，基于最新的Qgis2.4.0版本做了相应的尝试，并成功搭建相应的环境，大致的配置如下：
由于在搭建的过程之中遇到的问题实在太多，没有一一记录下来，所以在本文中没有做过多的描述，如有疑问可以联系husteryou@163.com，或者留言
环境搭建
![](https://img-blog.csdn.net/20141115231636625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91ZmFuZ3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141115231641031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91ZmFuZ3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141115231713542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91ZmFuZ3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
pro文件配置
```
QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = qgisdemo
TEMPLATE = app
SOURCES += main.cpp# \
    #mainwindow.cpp
#INCLUDEPATH += qgis-2.4.0
#INCLUDEPATH += qgis-2.4.0\core
#INCLUDEPATH += qgis-2.4.0\core\symbology-ng
#INCLUDEPATH += qgis-2.4.0\analysis
#INCLUDEPATH += qgis-2.4.0\gui
INCLUDEPATH += qgis-2.4.0
INCLUDEPATH += qgis-2.4.0/core
INCLUDEPATH += qgis-2.4.0/core/symbology-ng
INCLUDEPATH += qgis-2.4.0/analysis
INCLUDEPATH += qgis-2.4.0/gui
FORMS    += mainwindow.ui
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibqgis_analysis
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibqgis_analysis
else:unix:!macx: LIBS += -L$$PWD/lib/ -llibqgis_analysis
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibqgis_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibqgis_core
else:unix:!macx: LIBS += -L$$PWD/lib/ -llibqgis_core
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibqgis_gui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibqgis_gui
else:unix:!macx: LIBS += -L$$PWD/lib/ -llibqgis_gui
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
```
实例
#include<QApplication>#include<QString>#include<QWidget>#include<qgis.h>#include<qgsapplication.h>#include<qgsproviderregistry.h>#include<qgssinglesymbolrendererv2.h>#include<qgsmaplayerregistry.h>#include<qgsvectorlayer.h>#include<qgsmapcanvas.h>#include<qgsgeometry.h>#include"mouseprocess.h"#include"paintprocess.h"intmain(intargc,char*argv[]){QgsApplicationa(argc,argv,true);////////////////////////////////////////////////////////////////////////////注意这三行代码需要更改路径;QStringmyPluginsDir="D:/Qt/workspace/qgis-2.4.0/qgis-2.4.0/build/output/plugins";//插件路径(编译好的qgis文件夹下的plugins文件夹);QStringmyLayerPath1="F:/gis/data_1_3/10m_admin_0_countries.shp";//图层路径,必须设置为你电脑里面shp文件的路径,不然打不开数据;QStringmyLayerPath2="F:/gis/qgis_sample_data/shapefiles/airports.shp";//图层路径,必须设置为你电脑里面shp文件的路径,不然打不开数据;//////////////////////////////////////////////////////////////////////////QgsProviderRegistry::instance(myPluginsDir);//初始化插件的目录;QgsVectorLayer*mypLayer1=newQgsVectorLayer(myLayerPath1,"myLayer1","ogr");//初始化矢量图层;QgsVectorLayer*mypLayer2=newQgsVectorLayer(myLayerPath2,"myLayer2","ogr");//初始化矢量图层;QgsVectorLayer*mypLayer3=newQgsVectorLayer();//初始化矢量图层;mypLayer3->setRendererV2(QgsFeatureRendererV2::defaultRenderer(QGis::Point));QgsFeatureIteratoriter=mypLayer2->getFeatures();QgsFeaturefeature;while(iter.nextFeature(feature)){QgsGeometry*geo=feature.geometry();QgsPointpoint=geo->asPoint();//qDebug()<<point.x()<<point.y()<<endl;}for(intindex=0;index<100;index++){QgsPointpoint;doublexmin=-4.4802e+06;doublexmax=4.61512e+06;doubleymin=1.43353e+06;doubleymax=6.50259e+06;point.setX(xmin+(xmax-xmin)*((double)qrand())/RAND_MAX);point.setY(ymin+(ymax-ymin)*((double)qrand())/RAND_MAX);QgsFeaturefeature;QgsGeometry*geo=QgsGeometry::fromPoint(point);feature.setGeometry(geo);mypLayer3->addFeature(feature);mypLayer3->updateFeature(feature);qDebug()<<point.x()<<point.y()<<endl;}QList<QgsPoint>ring;//-4.4802e+064.61512e+061.43353e+066.50259e+06ring.append(QgsPoint(-4.4802e+06,1.43353e+06));ring.append(QgsPoint(4.61512e+06,6.50259e+06));ring.append(QgsPoint(2.61512e+06,4.50259e+06));//mypLayer2->addRing(ring);qDebug()<<mypLayer2->featureCount()<<endl;//QgsSingleSymbolRendererV2*mypRenderer=newQgsSingleSymbolRendererV2(mypLayer->geometryType());QList<QgsMapCanvasLayer>myLayerSet;//mypLayer->setRenderer(mypRenderer);//mypLayer->setRendererV2(mypRenderer);QgsMapLayerRegistry::instance()->addMapLayer(mypLayer1,true);QgsMapLayerRegistry::instance()->addMapLayer(mypLayer2,true);QgsMapLayerRegistry::instance()->addMapLayer(mypLayer3,true);//myLayerSet.append(QgsMapCanvasLayer(mypLayer3,true));myLayerSet.append(QgsMapCanvasLayer(mypLayer2,true));//myLayerSet.append(QgsMapCanvasLayer(mypLayer1,true));QgsRectangleextent=mypLayer2->extent();qDebug()<<extent.xMinimum()<<extent.xMaximum()<<extent.yMinimum()<<extent.yMaximum()<<endl;QgsMapCanvas*mypMapCanvas=newQgsMapCanvas(0,0);mypMapCanvas->setExtent(mypLayer2->extent());mypMapCanvas->enableAntiAliasing(true);mypMapCanvas->setCanvasColor(QColor(255,255,255));mypMapCanvas->freeze(false);mypMapCanvas->setLayerSet(myLayerSet);mypMapCanvas->setVisible(true);mypMapCanvas->refresh();mypMapCanvas->show();MouseProcess*mouseProcess=newMouseProcess();PaintProcess*paintProcess=newPaintProcess();QObject::connect(mypMapCanvas,SIGNAL(xyCoordinates(QgsPoint)),mouseProcess,SLOT(xyCoordinates(QgsPoint)));QObject::connect(mypMapCanvas,SIGNAL(renderComplete(QPainter*)),paintProcess,SLOT(renderComplete(QPainter*)));returna.exec();}
运行结果
![](https://img-blog.csdn.net/20141115232654312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveW91ZmFuZ3l1YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
