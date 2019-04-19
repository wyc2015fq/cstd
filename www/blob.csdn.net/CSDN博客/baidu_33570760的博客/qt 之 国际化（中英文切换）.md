# qt 之 国际化（中英文切换） - baidu_33570760的博客 - CSDN博客
2017年05月04日 11:37:27[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2701
转载自：http://blog.csdn.net/liang19890820/article/details/50276673
# 简介
Qt国际化属于Qt高级中的一部分，本想着放到后面来说，上节刚好介绍了Qt Linguist，趁热打铁就一起了解下。
对于绝大多数的应用程序，在刚启动时，需要加载默认的语言（或最后一次设置的语言）。在使用的过程中，我们也不需要重启应用程序而实现语言的动态切换。这样的话，易用性就非常好了。
- [简介](http://blog.csdn.net/liang19890820/article/details/50276673#%E7%AE%80%E4%BB%8B)
- [多语言切换](http://blog.csdn.net/liang19890820/article/details/50276673#%E5%A4%9A%E8%AF%AD%E8%A8%80%E5%88%87%E6%8D%A2)
- [源码分析](http://blog.csdn.net/liang19890820/article/details/50276673#%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90)
- [添加翻译源](http://blog.csdn.net/liang19890820/article/details/50276673#%E6%B7%BB%E5%8A%A0%E7%BF%BB%E8%AF%91%E6%BA%90)
- [翻译](http://blog.csdn.net/liang19890820/article/details/50276673#%E7%BF%BB%E8%AF%91)
- [发布翻译](http://blog.csdn.net/liang19890820/article/details/50276673#%E5%8F%91%E5%B8%83%E7%BF%BB%E8%AF%91)
- [加载翻译文件](http://blog.csdn.net/liang19890820/article/details/50276673#%E5%8A%A0%E8%BD%BD%E7%BF%BB%E8%AF%91%E6%96%87%E4%BB%B6)
- [源码下载](http://blog.csdn.net/liang19890820/article/details/50276673#%E6%BA%90%E7%A0%81%E4%B8%8B%E8%BD%BD)
# 多语言切换
![这里写图片描述](https://img-blog.csdn.net/20151212171954680)
要进行多语言的切换，需要执行以下步骤：
- 
对用户可见的文本信息全部使用tr()进行封装
- 
提供用户可以用来切换语言的一种方法。
- 
对于每一个窗口部件或者对话框，重写changeEvent事件，当事件类型为QEvent::LanguageChange时，翻译文本进行重新调用（为了简单我把它放在一个单独的函数translateUI()中）。
# 源码分析
我们来看一个简单的示例：主界面、设置界面，设置界面可根据选择不同语言下拉选项实现语言的动态切换！
为了简单起见，这里只给出关键代码（**源码下载请参考最下面下载地址**）：
设置界面：setting_dialog.h
```
typedef enum{
    UI_ZH,
    UI_EN
}LANGUAGE;
Q_DECLARE_METATYPE(LANGUAGE)
```
``
设置界面：setting_dialog.cpp
```cpp
language_combo_box->addItem("chinese", QVariant::fromValue(UI_ZH));
language_combo_box->addItem("english", QVariant::fromValue(UI_EN));
qRegisterMetaType<LANGUAGE>("LANGUAGE");
connect(language_combo_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingDialog::onIndexChanged);
void SettingDialog::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        translateUI();
        break;
    default:
        QDialog::changeEvent(event);
    }
}
void SettingDialog::translateUI()
{
    this->setWindowTitle(tr("setting dialog"));
    info_label->setText(tr("no brothers no programming"));
    language_label->setText(tr("language"));
    language_combo_box->setItemText(UI_ZH, tr("chinese"));
    language_combo_box->setItemText(UI_EN, tr("english"));
}
void SettingDialog::onIndexChanged()
{
    LANGUAGE language = language_combo_box->currentData().value<LANGUAGE>();
    emit switchLanguage(language);
}
```
```cpp
```
主界面：MainWidget.cpp
```
connect(setting_dialog, &SettingDialog::switchLanguage, this, &MainWidget::switchLanguage);
void MainWidget::translateUI()
{
    this->setWindowTitle(tr("main widget"));
    welcome_label->setText(tr("welcome to Qt") + QString("26197884/26188347"));
    setting_button->setText(tr("setting"));
    ok_button->setText(tr("ok"));
    cancel_button->setText(tr("cancel"));
}
void MainWidget::setLanguage(LANGUAGE current_language)
{
    this->current_language = current_language;
}
void MainWidget::setTranslator(QTranslator* translator)
{
    this->translator = translator;
}
void MainWidget::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::LanguageChange:
        translateUI();
        break;
    default:
        QWidget::changeEvent(event);
    }
}
void MainWidget::switchLanguage(LANGUAGE language)
{
    QString language_qm;
    switch(language)
    {
    case UI_ZH:
        language = UI_ZH;
        language_qm = QString(":/qm/main_widget_zh");
        break;
    case UI_EN:
        language = UI_EN;
        language_qm = QString(":/qm/main_widget_en");
        break;
    default:
        language = UI_ZH;
        language_qm = QString(":/qm/main_widget_zh");
    }
    if(current_language != language)
    {
        current_language = language;
        translator->load(language_qm);
        Util::writeInit(QString("./user.ini"), QString("language"), QString::number(language, 10));
    }
}
```
main.cpp
```
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString language_value;
    QString language_suffix = QString("zh");
    LANGUAGE language = UI_ZH;
    bool is_read = Util::readInit(QString("./user.ini"), QString("language"), language_value);
    if(is_read)
    {
        language = (LANGUAGE)language_value.toInt();
        if(language == UI_EN)
        {
            language_suffix = QString("en");
        }
    }
    QTranslator translator;  
    translator.load(QString(":/qm/main_widget_") + language_suffix);  
    app.installTranslator(&translator); 
    MainWidget main_widget;
    main_widget.setTranslator(&translator);
    main_widget.setLanguage(language);
    main_widget.show();
    return app.exec();
}
```
``
流程：
- 
进入main()，首先加载默认的语言（或最后一次设置的语言），然后显示主界面。
- 
当打开设置界面后，首先加载当前选择的语言，然后通过下拉设置语言选项来进行语言切换（设置界面的switchLanguage信号关联到主界面的switchLanguage槽中），然后将当前的语言进行保存。
语言切换后，首先会进入changeEvent函数，判断当事件类型为QEvent::LanguageChange时，就会执行translateUI函数进行文本重新显示。
**注：**
- 
Q_DECLARE_METATYPE：如果要使自定义类型或其他非QMetaType内置类型在QVariant中使用，必须使用该宏 。
- 
qRegisterMetaType：如果非QMetaType内置类型要在信号与槽中使用。
# 添加翻译源
为了方便，这里只介绍中、英文之间的切换。
在pro中添加：
```
TRANSLATIONS += Resource/main_widget_zh.ts \
               Resource/main_widget_en.ts
```
- 1
- 2
选择：工具->外部->Qt语言家->更新翻译，则会生成对应的ts文件。
![这里写图片描述](https://img-blog.csdn.net/20151212165743601)
# 翻译
使用Qt Linguist打开要翻译的ts文件，对翻译源进行相应语言的翻译。
可参考：[Qt Linguist介绍](http://blog.csdn.net/liang19890820/article/details/50274409)
# 发布翻译
选择：文件->发布，就会生成对应的qm文件。
![这里写图片描述](https://img-blog.csdn.net/20151212170131501)
# 加载翻译文件
使用QTranslator来加载生成的qm文件，就可以让程序显示指定的语言。
```
QTranslator translator;  
translator.load(":/qm/main_widget_zh");  
app.installTranslator(&translator);
```
