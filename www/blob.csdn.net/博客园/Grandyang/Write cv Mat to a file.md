# Write cv::Mat to a file - Grandyang - 博客园







# [Write cv::Mat to a file](https://www.cnblogs.com/grandyang/p/4354952.html)







如果我们想把OpenCV中的矩阵数据类型cv::Mat保存在一个文件中，可以使用如下的代码：



```
void writeMatToFile(cv::Mat& m, const char* filename) {
    ofstream fout(filename);
    if(!fout) {
        cout<<"File Not Opened"<<endl;  return;
    }
    fout << m;
    fout.close();
}
```



上述代码生成的数据由中括号括起来了，我们如果想导入Matlab做进一步处理的话，最好去掉首尾的中括号，当然，我们可以打开txt，手动删除中括号。如果我们想偷懒，让程序来删除的话，可以使用如下的代码：



```
void writeMatToFile(cv::Mat& m, const char* filename)
{
    ofstream fout(filename);
    if(!fout) {
        cout<<"File Not Opened"<<endl;  return;
    }
    //fout << m.rows << " " << m.cols << " " << m.dims << endl;
    fout << m;
    fout.close();
    // Delete the begining '[' and ending ']'
    fstream file(filename);
    string line;
    int cnt = 0;
    fout.open("tmp.txt");
    while (!file.eof()) {
        getline(file, line);
        if (cnt == 0) line = line.substr(1); // Delete '['
        if (cnt == m.rows - 1) line.pop_back(); // Delete ']'
        fout << line << endl;
        ++cnt;
    }
    file.close();
    fout.close();
    file.open("tmp.txt");
    fout.open(filename);
    while (!file.eof()) {
        getline(file, line);
        fout << line << endl;
    }
    file.close();
    fout.close();
    system("del tmp.txt");
}
```



还有一种方法用for循环将每个位置的数据写入文本中，但是这种方法保存下来的值跟上面那种方法稍有些出入，上述方法精度应该更高一些，我也把这种方法列出来作为参考吧：



```
void writeMatToFile(cv::Mat& m, const char* filename)
{
    ofstream fout(filename);
    if(!fout) {
        cout<<"File Not Opened"<<endl;  return;
    }
    for (int i = 0; i < m.rows; ++i) {
        for (int j = 0; j < m.cols; ++j) {
            for (int k = 0; k < m.channels(); ++k) {
                fout << m.at<float>(i, j * m.channels() + k);
                if (j * m.channels() + k < m.cols * m.channels() - 1) {
                    fout << ", ";
                }
            }
        }
        if (i < m.rows - 1) fout << "; " << endl;
    }
    fout.close();
}
```





我们也可以用OpenCV自带的API来完成数据的读和写，我们来看CvMat数据类型的读和写，关于CvMat和cv::Mat之间的转换请猛戳[这里](http://www.cnblogs.com/grandyang/p/4273714.html)：



```
// Save CvMat to .xml file
CvMat *m = cvLoadImageM("image.jpg", CV_LOAD_IMAGE_COLOR);
cvSave("m.yml", m);

// Save cv::Mat to .xml file
cv::Mat m;
CvMat cm = m;
cvSave("cm.yml", &cm);

// Load .xml file to CvMat
CvFileStorage *fs = cvOpenFileStorage("m.yml", 0, CV_STORAGE_READ); 
CvMat *newM = (CvMat*) cvLoad("m.yml");
```














