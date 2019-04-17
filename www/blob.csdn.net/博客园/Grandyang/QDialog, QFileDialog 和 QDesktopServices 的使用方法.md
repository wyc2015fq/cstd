# QDialog, QFileDialog 和 QDesktopServices 的使用方法 - Grandyang - 博客园







# [QDialog, QFileDialog 和 QDesktopServices 的使用方法](https://www.cnblogs.com/grandyang/p/4304644.html)







Qt中的QDialog类是用来生成对话框的类，QFileDialog 类是QDialog的衍生类，主要用来生成打开文件，或是打开文件目录的对话框，或者是保存文件的对话框，下面我们一一来看代码：



1. Load File Dialog

```
/**
 * Button event for Load File button
 */
void QtConfigFile::on_pbLoadFile_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Configuration File"), "",
        tr("Configuration File (*.xml);;All Files (*)"));
}
```



2. Save File Dialog

```
/**
 * Button event for Save File button
 */
void QtConfigFile::on_pbSaveFile_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Configuration File"), "",
        tr("Configuration File (*.xml);;All Files (*)"));
}
```



3. Browse Directory Dialog，其中leVideoDir是QLineEdit类的对象名

```
/**
 * Button event for Video Dir Browse button
 */
void QtConfigFile::on_pbVideoDirBrowse_clicked() {
    QString directory = QFileDialog::getExistingDirectory(this,
        tr("Video Directory"), QDir::currentPath());

    if (!directory.isEmpty()) {
        ui.leVideoDir->setText(directory);
    }
}
```



QDesktopServices类是针对操作系统的桌面服务应用的类，我们主要用它来打开文件夹：

```
/**
 * Button event for Show Folder button
 */
void QtConfigFile::on_pbShowFolder_clicked() {
    QDesktopServices::openUrl( QUrl::fromLocalFile( QDir::currentPath() ) );
}
```














