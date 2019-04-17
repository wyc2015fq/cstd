# Github Upload Large File 上传超大文件 - Grandyang - 博客园







# [Github Upload Large File 上传超大文件](https://www.cnblogs.com/grandyang/p/8606887.html)







Github中单个文件的大小限制是100MB，为了能突破这个限制，我们需要使用Git Large File Storage这个工具，参见这个[官方帖子](https://git-lfs.github.com/)，但是按照其给的步骤，博主未能成功上传超大文件，那么这里就给出自己成功的步骤吧：



```
git lfs install

git lfs track "*.exe"

git add .gitattributes

git commit -m "Updated the attributes"

git push origin master

git add my_large_file.exe

git lfs ls-files

git commit -m "Add the large file"

git push origin master
```














