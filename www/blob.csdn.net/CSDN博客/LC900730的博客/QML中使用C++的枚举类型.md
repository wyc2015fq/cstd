# QML中使用C++的枚举类型 - LC900730的博客 - CSDN博客
2017年08月30日 23:48:38[lc900730](https://me.csdn.net/LC900730)阅读数：1354
### C++头文件定义枚举类型
```
class Gemini:public QObject{
    Q_OBJECT
    Q_ENUMS(BALL_COLOR)
    public:
        Gemini():m_ballColor(BALL_COLOR_YELLOW){
            qDebug()<<"Gemini::Gemini() called";
        }
        enum BALL_COLOR{
            BALL_COLOR_YELLOW,
            BALL_COLOR_RED,
            BALL_COLOR_ALL,
        }
    signals:
        void begin();
    public slots:
        void doSomething(BALL_COLOR ballColor){
            qDebug()<<"Gemini::doSomething() called with"<<ballColor;
            if(ballcolor!=m_ballColor){
                m_ballColor=ballColor;
                qDebug()<<"ball color changed";
            }
    }
    private:
        BALL_COLOR m_ballColor;
}
```
//在main.qml中
```
import Union.Lotto.Gemini 1.0
Window{
    visible:true;
    width:300;height:400;
    title:"Union Lotto Game"
    color:"white"
    MouseArea{
        anchors.fill:parent
        onclicked:{
            gemini.begin()
        }
    }
    Gemini{
        id:gemini;
        //参数类型是BALL_COLOR枚举
        //QML中使用C++的枚举属性
        onBegin:doSomething(Gemini.BALL_COLOR_RED)
    }
}
```
