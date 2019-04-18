# QRunnable如何返回执行信息 - alw2009的博客 - CSDN博客
2016年09月18日 20:26:10[traveler_zero](https://me.csdn.net/alw2009)阅读数：234
```python
from PyQt5.QtCore import QThreadPool, QRunnable, QMetaObject, pyqtSlot, QThread, Qt, Q_ARG
from PyQt5.QtWidgets import QApplication, QTextEdit
import random
class SomeTask(QRunnable):
    def __init__(self, manager):
        super().__init__()
        self.manager = manager
    def run(self):
        workTime = random.randint(1, 20)
        QThread.sleep(workTime)
        QMetaObject.invokeMethod(self.manager, 'slot_task_finished',
                                 Qt.QueuedConnection, Q_ARG('int', workTime))
class MyGui(QTextEdit):
    def __init__(self):
        super().__init__()
        self.task = SomeTask(self)
        self.threadPool = QThreadPool()
        self.threadPool.setMaxThreadCount(10)
    def run_time_consuming_thead(self):
        for i in range(20):
            self.threadPool.start(self.task)
    @pyqtSlot(int)
    def slot_task_finished(self, result):
        self.append('some task return result:%d' % result)
if __name__ == '__main__':
    import sys
    app = QApplication(sys.argv)
    te = MyGui()
    te.show()
    te.run_time_consuming_thead()
    sys.exit(app.exec_())
```
