# Qt开发：Qt Widgets模块——QCommonStyle类 - 一世豁然的专栏 - CSDN博客





2018年05月23日 20:16:55[一世豁然](https://me.csdn.net/Explorer_day)阅读数：299








QCommonStyle类封装了GUI的常见外观和感觉。



头文件：#include <QCommonStyle> 

qmake：QT += widgets

继承：QStyle

被继承：QProxyStyle

这个抽象类实现了一些窗口小部件的外观和风格，这对于作为Qt一部分提供和发布的所有GUI风格都很常见。

由于QCommonStyle继承了QStyle，所以它的所有功能都在QStyle文档中完整记录。








一、成员函数

1、QCommonStyle::QCommonStyle()

构造一个QCommonStyle。





2、QCommonStyle::~QCommonStyle()

销毁风格。





3、[virtual] void QCommonStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget = Q_NULLPTR) const

从QStyle :: drawComplexControl（）重新实现。





4、[virtual] void QCommonStyle::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *widget = Q_NULLPTR) const

从QStyle :: drawControl（）重新实现。





5、[virtual] void QCommonStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *widget = Q_NULLPTR) const

从QStyle :: drawPrimitive（）重新实现。





6、[virtual] QPixmap QCommonStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const

从QStyle :: generatedIconPixmap（）重新实现。





7、[virtual] SubControl QCommonStyle::hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, const QPoint &pt, const QWidget *widget = Q_NULLPTR) const

从QStyle :: hitTestComplexControl（）重新实现。





8、[virtual] int QCommonStyle::layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option = Q_NULLPTR, const QWidget *widget = Q_NULLPTR) const

从QStyle :: layoutSpacing（）重新实现。





9、[virtual] int QCommonStyle::pixelMetric(PixelMetric m, const QStyleOption *opt = Q_NULLPTR, const QWidget *widget = Q_NULLPTR) const

从QStyle :: pixelMetric（）重新实现。





10、[virtual] void QCommonStyle::polish(QPalette &pal)

从QStyle :: polish（）重新实现。





11、[virtual] void QCommonStyle::polish(QApplication *app)

从QStyle :: polish（）重新实现。





12、[virtual] void QCommonStyle::polish(QWidget *widget)

从QStyle :: polish（）重新实现。





13、、[virtual] QSize QCommonStyle::sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &csz, const QWidget *widget = Q_NULLPTR) const

从QStyle :: sizeFromContents（）重新实现。





14、[virtual] QPixmap QCommonStyle::standardPixmap(StandardPixmap sp, const QStyleOption *option = Q_NULLPTR, const QWidget *widget = Q_NULLPTR) const

从QStyle :: standardPixmap（）重新实现。





15、[virtual] int QCommonStyle::styleHint(StyleHint sh, const QStyleOption *opt = Q_NULLPTR, const QWidget *widget = Q_NULLPTR, QStyleHintReturn *hret = Q_NULLPTR) const

从QStyle :: styleHint（）重新实现。





16、[virtual] QRect QCommonStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget = Q_NULLPTR) const

从QStyle :: subControlRect（）重新实现。





17、[virtual] QRect QCommonStyle::subElementRect(SubElement sr, const QStyleOption *opt, const QWidget *widget = Q_NULLPTR) const

从QStyle :: subElementRect（）重新实现。





18、[virtual] void QCommonStyle::unpolish(QWidget *widget)

从QStyle :: unpolish（）重新实现。





19、[virtual] void QCommonStyle::unpolish(QApplication *application)

从QStyle :: unpolish（）重新实现。




