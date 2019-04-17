# 将QMessageBox的Yes和No按钮翻译成中文 - DumpDoctorWang的博客 - CSDN博客





2017年07月14日 12:31:25[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：597








废话不多说，直接上代码

```cpp
/**
 * @brief Base::question 根据被点击的按钮，返回true或者false
 * @param buttonYesText 按钮Yes的文本
 * @param buttonNoText 按钮No的文本
 * @param title 窗口标题
 * @param content 提问内容
 * @return 当buttonYesText对应按钮被点击时，返回true，否则返回false
 */

bool Base::question(QString buttonYesText, QString buttonNoText, QString title, QString content){
    QMessageBox msg(QMessageBox::Question,title,content,QMessageBox::Yes | QMessageBox::No);
    msg.setButtonText(QMessageBox::Yes,buttonYesText);
    msg.setButtonText(QMessageBox::No,buttonNoText);
    msg.exec();
    if (msg.clickedButton() == msg.button(QMessageBox::Yes)) {
        return true;
    }
    else
        return false;
}
```











