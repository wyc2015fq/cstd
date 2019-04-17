# QDir, QFileInfo 和 QDirIterator 区别 - Grandyang - 博客园







# [QDir, QFileInfo 和 QDirIterator 区别](https://www.cnblogs.com/grandyang/p/4289564.html)







这三个类相互有关联，但是有不尽相同，首先从名字上看，QDir 和 QDirIterator 是针对于文件目录的，也就是文件夹，我们知道，对于一个文件夹，可以包含很多文件，也可以包含其他文件夹，通常是一个树的结构。文件夹里还可以包含符号链接，符号链接是指向其他文件或文件夹的一种链接，和Windows系统中的快捷方式有些类似，具体可参考[这里](https://blog.alphatr.com/windows-mklink.html)。而QFileInfo主要是针对某一个具体的文件，来保存和这个文件相关的所有信息，比如文件名，路径啥的。下面来看一个例子，是通过递归方法来查询所有.mp3文件：



```
void recurseAddDir(QDir d, bool recursive=true, bool symlinks=false ) {
    d.setSorting( QDir::Name );
    QDir::Filters df = QDir::Files | QDir::NoDotAndDotDot;
    if (recursive) df |= QDir::Dirs;
    if (not symlinks) df |= QDir::NoSymLinks;
    QStringList qsl = d.entryList(df, QDir::Name | QDir::DirsFirst);
    foreach (const QString &entry, qsl) {
        QFileInfo finfo(d, entry);
        if ( finfo.isDir() ) {
            QDir sd(finfo.absoluteFilePath());
            recurseAddDir(sd);
        } else {
            if (finfo.completeSuffix()=="mp3")
                addMp3File(finfo.absoluteFilePath()); 
        }
    }
}
```



上面那个例子在递归函数recurseAddDir()中调用了功能函数addMp3File()，严重的限制了该函数的可重用性，下面来看用Iterator Pattern来实现的版本：



```
int main (int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    QDir dir = QDir::current();
    if (app.arguments().size() > 1) {
        dir = app.arguments()[1];
    }
    if (!dir.exists()) {
        cerr << dir.path() << " does not exist!" << endl;
        usage(); 
        return -1;
    }
    QDirIterator qdi(dir.absolutePath(),
            QStringList() << "*.mp3",
            QDir::NoSymLinks | QDir::Files,
            QDirIterator::Subdirectories );
    while (qdi.hasNext()) {
        addMp3File(qdi.next());
    }
}
```



这样设计的好处是提高了代码可重用性，而且更简单了，这就是Design Pattern的魅力所在。












