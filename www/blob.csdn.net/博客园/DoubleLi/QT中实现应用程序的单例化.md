# QT中实现应用程序的单例化 - DoubleLi - 博客园






## 一介绍

通过编写一个QSingleApplication类，来实现Qt程序的单例化，原文的作者是在Windows Vista + Qt4.4 下实现的，不过应用在其他平台上是没问题的。(本文是我在http://www.qtcentre.org/wiki/index.php?title=SingleApplication上看到的)



## 二代码

方案一：使用Qt中的QSharedMemory,QLocalServer和QLocalSocket实现(不过需要在你的.pro里加上QT += network)

别的没翻译，就是大概说了一下，直接来代码吧：
// "single_application.h"

#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>

class SingleApplication : public QApplication

{
	Q_OBJECT
public:

	SingleApplication(int &argc, char *argv[], const QString uniqueKey);
	bool isRunning();
	bool sendMessage(const QString &message);
public slots:
	void receiveMessage();
signals:
	void messageAvailable(QString message);
private:
	bool _isRunning;
	QString _uniqueKey;
	QSharedMemory sharedMemory;
	QLocalServer *localServer;
	static const int timeout = 1000;
};

#endif // SINGLE_APPLICATION_H

// "single_application.cpp"
#include <QLocalSocket>
#include "single_application.h"

SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv), _uniqueKey(uniqueKey)

{
	sharedMemory.setKey(_uniqueKey);
	if (sharedMemory.attach())
		_isRunning = true;
	else
	{
		_isRunning = false;
		// create shared memory.
		if (!sharedMemory.create(1))
		{
			qDebug("Unable to create single instance.");
			return;
		}

		// create local server and listen to incomming messages from other instances.
		localServer = new QLocalServer(this);
		connect(localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
		localServer->listen(_uniqueKey);
	}
}

// public slots.
void SingleApplication::receiveMessage()
{
	QLocalSocket *localSocket = localServer->nextPendingConnection();
	if (!localSocket->waitForReadyRead(timeout))
	{
		qDebug(localSocket->errorString().toLatin1());
		return;
	}

	QByteArray byteArray = localSocket->readAll();
	QString message = QString::fromUtf8(byteArray.constData());
	emit messageAvailable(message);
	localSocket->disconnectFromServer();
}

// public functions.
bool SingleApplication::isRunning()
{
	return _isRunning;
}

bool SingleApplication::sendMessage(const QString &message)
{
	if (!_isRunning)
		return false;
	QLocalSocket localSocket(this);
	localSocket.connectToServer(_uniqueKey, QIODevice::WriteOnly);
	if (!localSocket.waitForConnected(timeout))
	{
		qDebug(localSocket.errorString().toLatin1());
		return false;
	}

	localSocket.write(message.toUtf8());
	if (!localSocket.waitForBytesWritten(timeout))
	{
		qDebug(localSocket.errorString().toLatin1());
		return false;
	}

	localSocket.disconnectFromServer();
	return true;


方案二：使用Qt中的QSharedMemory,和QTimert实现,别的也没翻译，还是直接来代码吧：
// "single_application.h"
#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QApplication>
#include <QSharedMemory>

class SingleApplication : public QApplication
{
	Q_OBJECT
public:
	SingleApplication(int &argc, char *argv[], const QString uniqueKey);
	bool isRunning();
	bool sendMessage(const QString &message);
public slots:
	void checkForMessage();
signals:
	void messageAvailable(QString message);
private:
	bool _isRunning;
	QSharedMemory sharedMemory;
};

#endif // SINGLE_APPLICATION_H

// "single_application.cpp"
#include <QTimer>
#include <QByteArray>
#include "single_application.h"

SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv)
{
	sharedMemory.setKey(uniqueKey);
	if (sharedMemory.attach())
		_isRunning = true;
	else
	{
		_isRunning = false;
		// attach data to shared memory.
		QByteArray byteArray("0"); // default value to note that no message is available.
		if (!sharedMemory.create(byteArray.size()))
		{
			qDebug("Unable to create single instance.");
			return;
		}
		sharedMemory.lock();
		char *to = (char*)sharedMemory.data();
		const char *from = byteArray.data();
		memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
		sharedMemory.unlock();

                // start checking for messages of other instances.
		QTimer *timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(checkForMessage()));
		timer->start(1000);
	}
}

// public slots.
void SingleApplication::checkForMessage()
{
	sharedMemory.lock();
	QByteArray byteArray = QByteArray((char*)sharedMemory.constData(), sharedMemory.size());
	sharedMemory.unlock();
	if (byteArray.left(1) == "0")
		return;
	byteArray.remove(0, 1);
	QString message = QString::fromUtf8(byteArray.constData());
	emit messageAvailable(message);

        // remove message from shared memory.
	byteArray = "0";
	sharedMemory.lock();
	char *to = (char*)sharedMemory.data();
	const char *from = byteArray.data();
	memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
	sharedMemory.unlock();
}

// public functions.
bool SingleApplication::isRunning()
{
	return _isRunning;
}

bool SingleApplication::sendMessage(const QString &message)
{
	if (!_isRunning)
		return false;

	QByteArray byteArray("1");
	byteArray.append(message.toUtf8());
	byteArray.append('/0'); // < should be as char here, not a string!
	sharedMemory.lock();
	char *to = (char*)sharedMemory.data();
	const char *from = byteArray.data();
	memcpy(to, from, qMin(sharedMemory.size(), byteArray.size()));
	sharedMemory.unlock();
	return true;
}


## 三使用
// "main.cpp"
#include "single_application.h"
int main(int argc, char *argv[])
{
	SingleApplication app(argc, argv, "some unique key string");
	if (app.isRunning())
	{
		app.sendMessage("message from other instance.");
		return 0;
	}

	MainWindow *mainWindow = new MainWindow();

        // connect message queue to the main window.
	QObject::connect(&app, SIGNAL(messageAvailable(QString)), mainWindow, SLOT(receiveMessage(QString)));

        // show mainwindow.
	mainWindow->show();
	return app.exec();

}
我想代码都应该能看得懂吧，这个挺不错的~








