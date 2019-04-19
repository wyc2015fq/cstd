# QML类与对象列表或分组 - LC900730的博客 - CSDN博客
2017年08月29日 18:29:01[lc900730](https://me.csdn.net/LC900730)阅读数：701
### 对象类型的属性
如果对象类型成功注册到QML类型系统，就可以在QML中访问对象类型的属性。如Message有一个MessageBody *类型的body属性：
```
class Message:public QObject{
    Q_OBJECT
    Q_PROPERTY(MessageBody * body READ body WRITE setBody NOTIFY bodyChanged)
    public:
        MessageBody * body() const;
        void setBody(MessageBody * body);
};
class MessageBody:public  QObject{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE text NOTIFY textChanged)
}
```
### 对象列表类型的属性
如果属性包含QObject子类列表，也可以在QML中访问。但是为了在QML中访问QObject子类的列表属性，必须使用QQmlListProperty类型，而不是QList类型，因为QList不是QObject的子类，不能通过Qt元对象系统就能获得QML所必须的一些特性，如NOTIFY信号。 
QQmlListProperty是一个模板类，可以直接通过QList构造。如MessageBoard类有一个messages属性，用于存储Message对象列表。这个属性应该是QQmlListProperty类型的。
```
class MessageBoard:public QObject{
        Q_OBJECT
        Q_PROPERTY(QQmlListProperty<Message>messages READ messages)
        public :
            QQmlListProperty<Message> messages() const;
        private:
            static void append_message(QQmlListProperty<Message> *list),Message *msg);
            QList<Message *> m_message;
    };
```
