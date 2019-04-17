# python按标签移动文件到对应文件夹 - 别说话写代码的博客 - CSDN博客





2018年06月13日 15:27:02[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：373








直接调用label2picture()然后改了路径就可以。例如在pathnew下按标签1...100建立文件夹并将对应文件复制过去

```python
def label2picture():
    f = open('train.txt','r')
    pathold='/home/dataset/train_max0/'
    pathnew = '/home/dataset/train_max1/'
    lines = f.readlines()
    for line in lines:
        classlabel = line.strip('\n').split(' ')[1]
        picname = line.strip('\n').split(' ')[0]
        print(picname,classlabel)
        if(os.path.exists(pathnew+classlabel)):

            shutil.copyfile(pathold + picname, pathnew+classlabel+'/'+picname)
            print('folder exists,copy finish')
        else:
            os.makedirs(pathnew+classlabel)
            shutil.copyfile(pathold + picname, pathnew + classlabel +'/'+ picname)
            print('new folder,copy finish')




    # path_img = 'C:/Users/49691/Desktop/数据集/test'
    # ls = os.listdir(path_img)
    # lenl = len(ls)
    # print(len(ls))
    #
    # train_labels = pd.read_csv('C:/Users/49691/Desktop/数据集/b.csv')
    # train_labels.head()
    # labels = train_labels.invasive.values
    # name = train_labels.name.values
    # print(train_labels.shape, train_labels.shape[0], labels, name)
    #
    # for i in range(lenl):
    #     # if i.find('testnan')!=-1:
    #     shutil.move(path_img + '/' + str(i) + ".jpg", "C:/Users/49691/Desktop/数据集/test1/" + str(name[i]) + ".jpg")
    #     print(i, name[i])
```




