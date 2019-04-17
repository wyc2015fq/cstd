# [CareerCup] 11.4 Sort the File 文件排序 - Grandyang - 博客园







# [[CareerCup] 11.4 Sort the File 文件排序](https://www.cnblogs.com/grandyang/p/4884625.html)







11.4 Imagine you have a 20 GB file with one string per line. Explain how you would sort the file.



这道题说给了我们一个20GB大小的文件，每行有一个字符串，让我们给文件内容排序。那么既然强调了这么大的一个文件，肯定不想让我们直接进入内存中，那么我们可以把大文件分块，每块xMB，其中x的大小为我们可用的内存大小，我们对每块分别排序，然后把所有的有序块进行合并，这样我们就能得到一个有序的文件了。












