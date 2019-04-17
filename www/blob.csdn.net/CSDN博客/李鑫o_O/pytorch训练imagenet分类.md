# pytorch训练imagenet分类 - 李鑫o_O - CSDN博客





2017年07月10日 18:11:23[hustlx](https://me.csdn.net/HUSTLX)阅读数：2609








1、imagenet数据准备：

a、下载数据集

b、提取training data:




```
mkdir train && mv ILSVRC2012_img_train.tar train/ && cd train
tar -xvf ILSVRC2012_img_train.tar && rm -f ILSVRC2012_img_train.tar
find . -name "*.tar" | while read NAME ; do mkdir -p "${NAME%.tar}"; tar -xvf "${NAME}" -C "${NAME%.tar}"; rm -f "${NAME}"; done
cd ..
```
c、提取验证集

```
mkdir val
cp pre_val.sh val/
cp ILSVRC2012_img_val.tar val/ && cd val && tar -xvf ILSVRC2012_img_val.tar && rm -f ILSVRC2012_img_val.tar
echo "pre_val.sh running"
sh pre_val.sh
rm pre_val.sh
```
注意：如果最后一步无法成功，打开网页复制内容生成一个pre_val.sh文件后处理

windows编辑的sh文件用下面去问题cat -v old.sh |tr -d "^M" > new.sh 




