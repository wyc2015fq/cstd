# 详解 Qt 线程间共享数据（使用signal/slot传递数据，线程间传递信号会立刻返回，但也可通过connect改变） - xqhrs232的专栏 - CSDN博客
2018年05月28日 12:27:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：337
原文地址::[https://www.cnblogs.com/findumars/p/6846734.html](https://www.cnblogs.com/findumars/p/6846734.html)
相关文章
1、Qt学习：线程间共享数据----[https://blog.csdn.net/wojiuguowei/article/details/17146839](https://blog.csdn.net/wojiuguowei/article/details/17146839)
使用共享内存。即使用一个两个线程都能够共享的变量（如全局变量），这样两个线程都能够访问和修改该变量，从而达到共享数据的目的。
Qt 线程间共享数据是本文介绍的内容，多的不说，先来啃内容。Qt线程间共享数据主要有两种方式：
使用共享内存。即使用一个两个线程都能够共享的变量（如全局变量），这样两个线程都能够访问和修改该变量，从而达到共享数据的目的；
使用singal/slot机制，把数据从一个线程传递到另外一个线程。
第一种办法在各个编程语言都使用普遍，而第二种方式倒是QT的特有方式，下面主要学习一下这种方式：
在线程之间传递signal与在一个线程内传递signal是不一样的。在一个线程内传递signal时，emit语句会直接调用所有连接的slot并等待到所有slot被处理完；在线程之间传递signal时，slot会被放到队列中(queue)，而emit这个signal后会马上返回；默认情况，线程之间使用queue机制，而线程内使用direct机制，但在connect中可以改变这些默认的机制。
```
1 view plaincopy to clipboardprint?  
  2 //TextDevice.h    
  3 #ifndef TEXTDEVICE_H    
  4 #define TEXTDEVICE_H    
  5 #include <QThread>    
  6 #include <QString>    
  7 #include <QMutex>     
  8 class TextDevice : public QThread {     
  9     Q_OBJECT     
 10 public:     
 11     TextDevice();     
 12     void run();     
 13     void stop();     
 14 public slots:     
 15     void write(const QString& text);     
 16 private:     
 17     int m_count;     
 18     QMutex m_mutex;     
 19 };    
 20 #endif // TEXTDEVICE_H     
 21     
 22     
 23 //TextDevice.cpp    
 24 #include <QMutexLocker>    
 25 #include <QDebug>    
 26 #include <QString>    
 27 #include "TextDevice.h"     
 28 TextDevice::TextDevice() {     
 29     m_count = 0;     
 30 }     
 31 void TextDevice::run() {     
 32     exec();     
 33 }     
 34 void TextDevice::stop() {     
 35     quit();     
 36 }     
 37 void TextDevice::write(const QString& text) {     
 38     QMutexLocker locker(&m_mutex);     
 39     qDebug() << QString("Call %1: %2").arg(m_count++).arg(text);     
 40 }     
 41     
 42 //TextThread.h    
 43 #ifndef TEXTTHREAD_H    
 44 #define TEXTTHREAD_H    
 45 #include <QThread>    
 46 #include <QString>     
 47 class TextThread : public QThread {     
 48     Q_OBJECT     
 49 public:     
 50     TextThread(const QString& text);     
 51     void run();     
 52     void stop();     
 53 signals:     
 54     void writeText(const QString&);     
 55 private:     
 56     QString m_text;     
 57     bool m_stop;     
 58 };    
 59 #endif // TEXTTHREAD_H     
 60     
 61 //TextThread.cpp    
 62 #include "TextThread.h"     
 63 TextThread::TextThread(const QString& text) : QThread() {     
 64     m_text = text;     
 65     m_stop = false;     
 66 }     
 67 void TextThread::stop() {     
 68     m_stop = true;     
 69 }     
 70 void TextThread::run() {     
 71     while(!m_stop) {     
 72         emit writeText(m_text);     
 73         sleep(1);     
 74     }     
 75 }     
 76     
 77 //main.cpp    
 78 #include <QApplication>    
 79 #include <QMessageBox>    
 80 #include "TextDevice.h"    
 81 #include "TextThread.h"     
 82     
 83 int main(int argc, char** argv) {     
 84     QApplication app(argc, argv);     
 85     //启动线程     
 86     TextDevice device;     
 87     TextThread foo("foo"), bar("bar");     
 88     //把两个线程使用signal/slot连接起来     
 89     QObject::connect(&foo, SIGNAL(writeText(const QString&)), &device, SLOT(write(const QString&)));     
 90     QObject::connect(&bar, SIGNAL(writeText(const QString&)), &device, SLOT(write(const QString&)));     
 91     //启动线程     
 92     foo.start();     
 93     bar.start();     
 94     device.start();     
 95     QMessageBox::information(0, "Threading", "Close me to stop.");     
 96     //停止线程     
 97     foo.stop();     
 98     bar.stop();     
 99     device.stop();     
100     //等待线程结束     
101     device.wait();     
102     foo.wait();     
103     bar.wait();     
104     return 0;     
105 }    
106 //TextDevice.h  
107 #ifndef TEXTDEVICE_H  
108 #define TEXTDEVICE_H  
109 #include <QThread> 
110 #include <QString> 
111 #include <QMutex> 
112 class TextDevice : public QThread {  
113     Q_OBJECT  
114 public:  
115     TextDevice();  
116     void run();  
117     void stop();  
118 public slots:  
119     void write(const QString& text);  
120 private:  
121     int m_count;  
122     QMutex m_mutex;  
123 };  
124 #endif // TEXTDEVICE_H  
125  
126  
127 //TextDevice.cpp  
128 #include <QMutexLocker> 
129 #include <QDebug> 
130 #include <QString> 
131 #include "TextDevice.h"  
132 TextDevice::TextDevice() {  
133     m_count = 0;  
134 }  
135 void TextDevice::run() {  
136     exec();  
137 }  
138 void TextDevice::stop() {  
139     quit();  
140 }  
141 void TextDevice::write(const QString& text) {  
142     QMutexLocker locker(&m_mutex);  
143     qDebug() << QString("Call %1: %2").arg(m_count++).arg(text);  
144 }  
145  
146 //TextThread.h  
147 #ifndef TEXTTHREAD_H  
148 #define TEXTTHREAD_H  
149 #include <QThread> 
150 #include <QString> 
151 class TextThread : public QThread {  
152     Q_OBJECT  
153 public:  
154     TextThread(const QString& text);  
155     void run();  
156     void stop();  
157 signals:  
158     void writeText(const QString&);  
159 private:  
160     QString m_text;  
161     bool m_stop;  
162 };  
163 #endif // TEXTTHREAD_H  
164  
165 //TextThread.cpp  
166 #include "TextThread.h"  
167 TextThread::TextThread(const QString& text) : QThread() {  
168     m_text = text;  
169     m_stop = false;  
170 }  
171 void TextThread::stop() {  
172     m_stop = true;  
173 }  
174 void TextThread::run() {  
175     while(!m_stop) {  
176         emit writeText(m_text);  
177         sleep(1);  
178     }  
179 }  
180  
181 //main.cpp  
182 #include <QApplication> 
183 #include <QMessageBox> 
184 #include "TextDevice.h"  
185 #include "TextThread.h"  
186 int main(int argc, char** argv) {  
187     QApplication app(argc, argv);  
188     //启动线程  
189     TextDevice device;  
190     TextThread foo("foo"), bar("bar");  
191     //把两个线程使用signal/slot连接起来  
192     QObject::connect(&foo, SIGNAL(writeText(const QString&)), &device, SLOT(write(const QString&)));  
193     QObject::connect(&bar, SIGNAL(writeText(const QString&)), &device, SLOT(write(const QString&)));  
194     //启动线程  
195     foo.start();  
196     bar.start();  
197     device.start();  
198     QMessageBox::information(0, "Threading", "Close me to stop.");  
199     //停止线程  
200     foo.stop();  
201     bar.stop();  
202     device.stop();  
203     //等待线程结束  
204     device.wait();  
205     foo.wait();  
206     bar.wait();  
207     return 0;  
208 }
```
　　上面例子代码可以看出两个线程之间传送了类型为QString的信息。像QString等这些QT本身定义的类型，直接传送即可。但如果是自己定义的类型如果想使用signal/slot来传递的话，则没有这么简单。直接使用的话，会产生下面这种错误：
`1 QObject::connect: Cannot queue arguments of type 'TextAndNumber' (Make sure 'TextAndNumber' is registed using qRegisterMetaType().) `
原因：当一个signal被放到队列中（queued）时，它的参数(arguments)也会被一起一起放到队列中（queued起来），这就意味着参数在被传送到slot之前需要被拷贝、存储在队列中（queue）中；为了能够在队列中存储这些参数(argument)，Qt需要去construct、destruct、copy这些对象，而为了让Qt知道怎样去作这些事情，参数的类型需要使用qRegisterMetaType来注册（如错误提示中的说明）
步骤：(以自定义TextAndNumber类型为例）
自定一种类型，在这个类型的顶部包含：#include <QMetaType>
在类型定义完成后，加入声明：Q_DECLARE_METATYPE(TextAndNumber);
在main()函数中注册这种类型：qRegisterMetaType<TextAndNumber>("TextAndNumber");
如果还希望使用这种类型的引用，可同样要注册：qRegisterMetaType<TextAndNumber>("TextAndNumber&");
```
1 view plaincopy to clipboardprint?  
  2 //TextAndNumber.h     
  3 #ifndef TEXTANDNUMBER_H     
  4 #define TEXTANDNUMBER_H     
  5 #include <QMetaType>     
  6 //必须包含QMetaType，否则会出现下面错误：     
  7 //error: expected constructor, destructor, or type conversion before ‘;’ token     
  8 #include <QString>     
  9 class TextAndNumber {     
 10 public:     
 11     TextAndNumber();     
 12     TextAndNumber(int, QString);     
 13     int count();     
 14     QString text();     
 15 private:     
 16     int m_count;     
 17     QString m_text;     
 18 };     
 19 Q_DECLARE_METATYPE(TextAndNumber);     
 20 #endif // TEXTANDNUMBER_H     
 21     
 22 //TextAndNumber.cpp     
 23 #include "TextAndNumber.h"     
 24 TextAndNumber::TextAndNumber() {     
 25 }     
 26 TextAndNumber::TextAndNumber(int count, QString text) {     
 27     m_count = count;     
 28     m_text = text;     
 29 }     
 30 int TextAndNumber::count() {     
 31     return m_count;     
 32 }     
 33 QString TextAndNumber::text() {     
 34     return m_text;     
 35 }     
 36     
 37 //TextDevice.h     
 38 #ifndef TEXTDEVICE_H     
 39 #define TEXTDEVICE_H     
 40 #include <QThread>     
 41 #include <QDebug>     
 42 #include <QString>     
 43 #include "TextAndNumber.h"     
 44 class TextDevice : public QThread {     
 45     Q_OBJECT     
 46 public:     
 47     TextDevice();     
 48     void run();     
 49     void stop();     
 50 public slots:     
 51     void write(TextAndNumber& tran);     
 52 private:     
 53     int m_count;     
 54 };     
 55 #endif // TEXTDEVICE_H     
 56     
 57 //TextDevice.cpp     
 58 #include "TextDevice.h"     
 59 TextDevice::TextDevice() : QThread() {     
 60     m_count = 0;     
 61 }     
 62 void TextDevice::run() {     
 63     exec();     
 64 }     
 65 void TextDevice::stop() {     
 66     quit();     
 67 }     
 68 void TextDevice::write(TextAndNumber& tran) {     
 69     qDebug() << QString("Call %1 (%3): %2").arg(m_count++).arg(tran.text()).arg(tran.count());     
 70 }     
 71     
 72 //TextThread.h     
 73 #ifndef TEXTTHREAD_H     
 74 #define TEXTTHREAD_H     
 75 #include <QThread>     
 76 #include <QString>     
 77 #include "TextAndNumber.h"     
 78 class TextThread : public QThread {     
 79     Q_OBJECT     
 80 public:     
 81     TextThread(const QString& text);     
 82     void run();     
 83     void stop();     
 84 signals:     
 85     void writeText(TextAndNumber& tran);     
 86 private:     
 87     QString m_text;     
 88     int m_count;     
 89     bool m_stop;     
 90 };     
 91     
 92 #endif // TEXTTHREAD_H     
 93     
 94 //TextThread.cpp     
 95 #include "TextThread.h"     
 96 TextThread::TextThread(const QString& text) : QThread() {     
 97     m_text = text;     
 98     m_stop = false;     
 99     m_count = 0;     
100 }     
101 void TextThread::run() {     
102     while(!m_stop) {     
103         TextAndNumber tn(m_count++, m_text);     
104         emit writeText(tn);     
105         sleep(1);     
106     }     
107 }     
108 void TextThread::stop() {     
109     m_stop = true;     
110 }     
111     
112 //main.cpp     
113 #include <QApplication>     
114 #include <QMessageBox>     
115 #include "TextThread.h"     
116 #include "TextDevice.h"     
117 #include "TextAndNumber.h"     
118 int main(int argc, char *argv[])     
119 {     
120     QApplication app(argc, argv);     
121     qRegisterMetaType<TextAndNumber>("TextAndNumber");     
122     qRegisterMetaType<TextAndNumber>("TextAndNumber&");     
123     TextDevice device;     
124     TextThread foo("foo"), bar("bar");     
125     QObject::connect(&foo, SIGNAL(writeText(TextAndNumber&)), &device, SLOT(write(TextAndNumber&)));     
126     QObject::connect(&bar, SIGNAL(writeText(TextAndNumber&)), &device, SLOT(write(TextAndNumber&)));     
127     device.start();     
128     foo.start();     
129     bar.start();     
130     QMessageBox::information(0, "Threading", "Click me to close");     
131     foo.stop();     
132     bar.stop();     
133     device.stop();     
134     foo.wait();     
135     bar.wait();     
136     device.wait();     
137     qDebug() << "Application end.";     
138     return 0;     
139 }
```
http://www.cnblogs.com/bingcaihuang/archive/2011/07/14/2106885.html
