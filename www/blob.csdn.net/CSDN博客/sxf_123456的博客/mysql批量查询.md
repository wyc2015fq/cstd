# mysql批量查询 - sxf_123456的博客 - CSDN博客
2017年09月05日 21:41:24[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：2150
                
ana_device表中通过mac找到对应的loid,再通过loid找到对应ana_customer表中的band_count值
sql 语句：
SELECT 
a.mac,a.loid,b.band_account 
FROM 
ana_device a LEFT JOIN ana_customer b ON a.`loid`=b.`loid`
WHERE   a.`mac` IN ('E865D400F7A8','28FF3EA00242');
a.mac 的值可以通过给出的excel中的mac，通过下边代码
将
E865D400F7A8
28FF3EA00242
其转化为
'E865D400F7A8',
'28FF3EA00242'
格式的数据
然后，拷贝到sql语句IN后边的括号内
注意：sql查询数据的条数最好不要超过10000，即mac的值最好不要超过10000条
批量处理
E865D400F7A8
28FF3EA00242
转化为
'E865D400F7A8',
'28FF3EA00242'
import os
def change_charcter(inputfile_path,file_num):
    readfile = open(inputfile_path,"r")
    i=1
    while(i<=file_num):
        outputfile_path = "C:\\Users\\Administrator\\Desktop\\mac_"+str(i)+".txt"
        if os.path.exists(outputfile_path):
            os.remove(outputfile_path)
        count = 0
        writefile = open(outputfile_path, 'a+')
        for line in readfile:
            count += 1
            if (count<=9000):
                line = line.strip()
                str_line = "'"+line+"'"+",\n"
                writefile.write(str_line)
            else:
                line = line.strip()
                str_line = "'" + line + "'" + ",\n"
                writefile.write(str_line)
                writefile.close()
                break
        i += 1
    readfile.close()
if __name__=='__main__':
    inputfile_path = "C:\\Users\\Administrator\\Desktop\\mac.txt"
    file_num = 5
    change_charcter(inputfile_path,file_num)
