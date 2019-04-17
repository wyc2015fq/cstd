# Save vtkMatrix4x4 to a file 保存到文件 - Grandyang - 博客园







# [Save vtkMatrix4x4 to a file 保存到文件](https://www.cnblogs.com/grandyang/p/4790505.html)







vtkMatrix4x4是VTK中的一个表示4x4矩阵的一种数据结构，有时候我们想把其保存到一个文件中，那么可以使用如下的代码：



```
void writeVtkMatrix4x4ToFile(const vtkMatrix4x4 *m, char *filename) {
    ofstream fout(filename);
    if(!fout) {
        cout<<"File Not Opened"<<endl;  return;
    }
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            fout << m->GetElement(i, j) << " ";
        }
        fcout << endl;
    }
    fcout << endl;
    fout.close();
}
```














