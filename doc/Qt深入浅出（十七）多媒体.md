# Qt深入浅出（十七）多媒体 - qq769651718的专栏 - CSDN博客












2018年02月24日 00:41:06[吓人的猿](https://me.csdn.net/qq769651718)阅读数：455








# 多媒体编程

## 1 音频组件

​	Qt提供的多媒体可以从高层到底层进行访问, 以及处理音频的输出和输入。Qt为了开发基于多媒体的应用程序，可以使用QMediaPlayer类.

 	不仅支持从属的压缩音频格式,也支持用户安装的多媒体插件.



        播放多媒体使用到QMediaPlayer类,需要在.pro文件中添加`QT += multimedia`

### 1.1 最简单的音乐播放器的实现
- 
最简单例子


```cpp
QMediaPlayer * player = new QMediaPlayer;
player->setMedia(QUrl::fromLocalFile("c:\\123.MP3")); 
player->setVolume(50);
player->play();
```



### 1.2 常用函数

```cpp
[public slot] void QMediaPlayer::play()    //播放
[public slot] void QMediaPlayer::pause()    //暂停
[public slot] void QMediaPlayer::stop() //停止
[public slot] void QMediaPlayer::setVolume(int volume); //设置音量
[public slot] void QMediaPlayer::setMedia(const QMediaContent &media, QIODevice *stream = Q_NULLPTR); //设置多媒体资源.
[public slot] void QMediaPlayer::setPlaylist(QMediaPlaylist *playlist); //设置播放资源
[public slot] void QMediaPlayer::setPosition(qint64 position);  //设置资源位置
```


- 
一个简单播放器的例子

widget.h


```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
#include <QMediaPlayer>
#include <QLabel>
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void playAudio();
private:
    QMediaPlayer *_player;
    QLabel * _label;
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QPushButton>
​
#include <QVBoxLayout>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSlider>
​
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    _player = new QMediaPlayer(this);
​
    /*布局音量滑块*/
    QHBoxLayout* hBox0 = new QHBoxLayout;
    QLabel *label = new QLabel("volume:");
    QSlider* slider = new QSlider;
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(_player->volume());
    hBox0->addWidget(label);
    hBox0->addWidget(slider);
​
    /*布局控制按钮*/
    QHBoxLayout* hBox1 = new QHBoxLayout;
    QPushButton* pb0 = new QPushButton("open");
    QPushButton* pb1 = new QPushButton("play");
    QPushButton* pb2 = new QPushButton("pause");
    QPushButton* pb3 = new QPushButton("stop");
    hBox1->addWidget(pb0);
    hBox1->addWidget(pb1);
    hBox1->addWidget(pb2);
    hBox1->addWidget(pb3);
​
    /*窗口布局*/
    QVBoxLayout* vBox = new QVBoxLayout;
    _label = new QLabel("please open a mp3 file", this);
    vBox->addWidget(_label, 2);
    vBox->addLayout(hBox0, 1);
    vBox->addLayout(hBox1, 1);
    this->setLayout(vBox);
​
    /*信号链接*/
    connect(pb0, SIGNAL(clicked()), this, SLOT(playAudio()));
    connect(pb1, SIGNAL(clicked()), _player, SLOT(play()));
    connect(pb2, SIGNAL(clicked()), _player, SLOT(pause()));
    connect(pb3, SIGNAL(clicked()), _player, SLOT(stop()));
    connect(slider, SIGNAL(valueChanged(int)), _player, SLOT(setVolume(int)));
    connect(_player, SIGNAL(volumeChanged(int)), slider, SLOT(setValue(int)));
​
​
}
​
void Widget::playAudio()
{
    QString filename = QFileDialog::getOpenFileName(this, "MP3", "", "(*.mp3)");
​
    if(QFileInfo(filename).isReadable())
    {
        _label->setText(QFileInfo(filename).fileName());
        _player->setMedia(QUrl::fromLocalFile(filename));
    }
}
​
Widget::~Widget()
{
​
}
```



### 1.3 播放列表的使用

​	上面简单的播放器，每次只能设置一首歌曲，如果想实现多首歌曲播放，可以使用多媒体播放列表类QMediaPlayList。


- 
带播放列表的播放


```cpp
QMediaPlayer * player = new QMediaPlayer;
QMediaPlayList * playlist = new QMediaPlayList
player-> setPlaylist(playlist);
playlist ->addMedia(QUrl::fromLocalFile("c:\\123.mp3"));
playlist->addMedia(QUrl("http://example.com/123.mp3"));
player->play();
```


- 
QMediaPlayList一些常用函数


```cpp
​
void QmediaPlayList::setCurrentIndex(int playlistPosition)//设置当前播放的资源索引.
void QmediaPlayList::setPlaybackMode(PlaybackMode mode)   //设置播放模式,循环播放，随机播放等
void QmediaPlayList::next()         //播放下一首
void QmediaPlayList::previous()         //播放前一首
bool QmediaPlayList::addMedia(const QMediaContent &content; //添加资源
bool QmediaPlayList::removeMedia(int pos);  //移除某一个资源
bool QmediaPlayList::clear()                //移除所有资源
```



## 2 视频播放组件

​	Qt提供的多媒体功能能够打开并控制从低层到高层的视频数据，而且可以重叠(Overlap)使用音频以及视频数据。

 	为了能使用c++处理视频数据的多媒体程序，使用QMediaPlayer类实现视频编码。使用QVideoWidget和QGraphicsVideoItem类将视频数据显示到界面中。

         需要在.pro文件中添加`QT+= multimedia  multimediawidgets`



### 2.1 使用QVideoWidget播放视频





​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>
​
class Widget : public QWidget
{
    Q_OBJECT
​
public:
    Widget(QWidget *parent = 0);
    ~Widget();
public slots:
    void playVideo();
private:
    QMediaPlayer *_player;
    QVideoWidget *_videoWidget;
    QMediaPlaylist *_playList;
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
​
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(600, 480);
    _player = new QMediaPlayer(this);
    _videoWidget = new QVideoWidget(this);
    _playList = new QMediaPlaylist(this);
    _player->setPlaylist(_playList);
    _player->setVideoOutput(_videoWidget);
​
    QPushButton * pb = new QPushButton("play", this);
​
    QVBoxLayout * vBox = new QVBoxLayout(this);
    vBox->addWidget(_videoWidget, 5);
    vBox->addWidget(pb);
    this->setLayout(vBox);
​
    connect(pb, SIGNAL(clicked()), this, SLOT(playVideo()));
}
​
void Widget::playVideo()
{
    QString filename = QFileDialog::getOpenFileName(this, "MP3", "", "(*.mp4 *.wmv)");
​
    if(QFileInfo(filename).isReadable())
    {
        _playList->addMedia(QUrl::fromLocalFile(filename));
        _player->play();
    }
}
​
Widget::~Widget()
{
​
}
```



### 2.2 使用QGraphicsVideoItem播放视频

​	widget.h

```cpp
#ifndef WIDGET_H
#define WIDGET_H
​
#include <QGraphicsView>
​
class GraphicsView : public QGraphicsView
{
    Q_OBJECT
​
public:
    GraphicsView(QWidget *parent = 0);
    ~GraphicsView();
};
​
#endif // WIDGET_H
```

​	widget.cpp

```cpp
#include "widget.h"
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
​
​
GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    this->resize(600, 480);
    QMediaPlayer * player = new QMediaPlayer(this);
    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsVideoItem * item = new QGraphicsVideoItem;
    item->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(item);
    this->setScene(scene);
    player->setVideoOutput(item);
    player->setMedia(QUrl::fromLocalFile("E:\\123.wmv"));
    player->play();
​
​
}
​
GraphicsView::~GraphicsView()
{
​
}
```






