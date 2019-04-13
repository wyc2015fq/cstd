
# QT创建文件夹 - 嵌入式Linux - CSDN博客

2017年02月27日 14:33:25[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：320


最近在做项目的时候，需要创建一个新的文件夹保存临时文件，但是很长时间都没有解决这个问题，今天终于解决了，拿出来和网友分享一下Qt中创建文件夹的方法。如下代码是在单击按钮后，先判断文件夹是否存在，若不存在便创建一个名字为temp的文件夹。
void MainWindow::on_pushButton_2_clicked()
{
QDir *temp = new QDir;
bool exist = temp->exists("D://temp");
if(exist)
QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
else
{
bool ok = temp->mkdir("D://temp");
if( ok )
QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹创建成功！"));
}
}

