import hashlib
import os
import json
import filecmp

def test():

    path = 'E:/book'
    data = {}
    if os.path.exists(path + '/list.json'):
        with open(path + '/list.json', 'r') as f:
            data = json.load(f)

    flist = []

    for dirpath, dirnames, filenames in os.walk(path):
        for filename in filenames:
            file = dirpath+'/'+filename
            flist.append(file)

    flist = flist[:10]
    print(len(flist))

    #求文件md5
    #文件位置中的路径，请用双反斜杠，

    for file in flist:
        if file not in data:
            md5file=open(file, 'rb')
            md5=hashlib.md5(open(file,'rb').read()).hexdigest()
            md5file.close()
            data[file] = md5
            print('calc md5: ' + file)

    with open(path + '/list.json', 'w') as f:
        json.dump(data, f, indent=' ')

    hash = {}

    for file in data:
        print(file)
        md5 = data[file]
        if md5 not in hash:
            hash[md5] = [file]
        else:
            hash[md5].append(file)
