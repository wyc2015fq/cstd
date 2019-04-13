
# kafka+sparkstreaming实时读取计算nginx日志，存储结果到mongodb-mysql - leofionn的博客 - CSDN博客


2018年05月04日 17:21:28[leofionn](https://me.csdn.net/qq_36142114)阅读数：430



```python
#!/usr/bin/env python
#coding:utf-8
import sys
import urlparse
import re
import datetime
import MySQLdb
import logging
from pymongo import MongoClient
from pyspark import SparkContext, SparkConf
from pyspark.streaming import StreamingContext
from pyspark.streaming.kafka import KafkaUtils
from urllib import unquote
import hashlib
import base64
import ConfigParser
import mcrypt 

reload(sys)
sys.setdefaultencoding('utf-8')

#mysql 连接信息
mysqlHost = '***' 
mysqlUser = '***'
mysqlPasswd = '***'
mysqlDatabase = '***'
mysqlPort = 3306
# mongodb 连接信息
mongodbClient = MongoClient('***', 27017)
mongodbDb = '***'
collection_useraction = mongodbClient[mongodbDb]
class Decrypt:
    def __init__(self, configFile = None):
        self.configFile = configFile
        if self.configFile != None:
            self.config = ConfigParser.ConfigParser()
            self.config.read(self.configFile)
        self.SUCCESS, self.FAILED = True, False
        self.KEY_SIZE = 32
        self.BLOCK_SIZE = 32

    def getKey(self, key):
        md5Key = hashlib.md5()
        md5Key.update(key)
        return md5Key.hexdigest()
    def getBase64Decode(self, encString):
        missing_padding = 4 - len(encString) % 4
        if missing_padding:
            encString += b'='* missing_padding
        return base64.decodestring(encString)


    def xorDecode(self, string, key):
        mString = self.xorMerge(string, key)
        if mString == self.FAILED:
            return self.FAILED
        dec = ''
        i = 0
        while i < len(mString):
            try:
                dec += chr(ord(mString[i+1:i+2]) ^ ord(mString[i:i+1]))
                i += 2
            except Exception, e:
                return self.FAILED
        return dec
    def xorMerge(self, string, key):
        hashString = self.hashMethod(key)
        if hashString == self.FAILED:
            return self.FAILED
        xored = ''
        i = 0
        while i < len(string):
            j = i % len(hashString)
            xored += chr(ord(string[i:i+1]) ^ ord(hashString[j:j+1]))
            i += 1
        return xored
    def hashMethod(self, key):
        hashStr = ''
        try:
            hashStr = hashlib.sha1(key).hexdigest()
        except Exception, e:
            return self.FAILED
        return hashStr
    def removeCipherNoise(self, string, key):
        keyHash = self.hashMethod(key)
        keylen = len(keyHash)
        stri = ''
        i, j = 0, 0
        length = len(string)
        while i < length:
            if j >= keylen:
                j = 0
            temp = ord(string[i]) - ord(keyHash[j])
            if temp < 0:
                temp = temp + 256
            stri += chr(temp)
            i += 1
            j += 1
        return stri
    def mcryptDecode(self, string, key):
        string = self.removeCipherNoise(string, key)
        initSize = 32
        initVect = string[0:initSize]
        string = string[initSize:]
        decoded = self.mcryptDecryption(key, initVect, string)
        if decoded == self.FAILED:
            return self.FAILED    
        return decoded
    def decode(self, string):
        key = "encrypt@xin.com"      
        if not key:
            return     self.FAILED
        key = self.getKey(key)
        dec = self.getBase64Decode(string)
        if True:
            decoded = self.mcryptDecode(dec, key)
            if decoded == self.FAILED:
                return self.FAILED
        else:
            xorDec = self.xorDecode(dec, key)
            if xorDec == self.FAILED:
                return self.FAILED
            decoded = xorDec
        # Just make sure you remove the trailing \x00
        decoded = decoded.rstrip('\x00')
        return decoded
    def mcryptDecryption(self, key, iv, data):
        try:
            m = mcrypt.MCRYPT('rijndael-256', 'cbc')    
            m.init(key, iv)
            decrypted = m.decrypt(data)
        except Exception, e:
            return self.FAILED
        return decrypted
dec = Decrypt()
def date_format(line):
    datetime_ser=line[0][1:21]
    day=datetime_ser[:2]
    month=datetime_ser[3:6]
    year=datetime_ser[7:11]
    time=datetime_ser[12:]
    month_map={'Jan':'01' ,'Feb':'02' ,'Mar':'03' ,'Apr':'04' ,'May':'05' ,'Jun':'06' ,'Jul':'07' ,'Aug':'08' ,'Sep':'09' ,'Oct':'10' ,'Nov':'11' ,'Dec':'12'}
    line[0]='%s-%s-%s' %(year, month_map[month], day)
    return line
def get_cid(line):
    urlstring=line[6]
    url = urlparse.urlparse(urlstring.replace('"', '').replace(' HTTP/1.1', ''))
    params=url.query
    params_map = urlparse.parse_qs(params)
    cid=params_map.get("cid", "")
    line.append("".join(cid))
    return line

def get_platform(line):
    urlstring=line[6]
    url = urlparse.urlparse(urlstring.replace('"', '').replace(' HTTP/1.1', ''))
    params=url.query
    params_map = urlparse.parse_qs(params)
    cid=params_map.get("from", "")
    line.append("".join(cid))
    return line
def log_filter(line):
    request=unquote(line[6].strip('"'))
    pattern = re.compile(r'ev=Page/(half)?vehicle_details/[0-9]+|pageid=4')
    match = pattern.search(request)
    if match:
        param= match.group()
    else:
        param = ''
    line.append(param)
    return line
def get_uid(line):
    urlstring=line[6]
    url = urlparse.urlparse(urlstring.replace('"', '').replace(' HTTP/1.1', ''))
    params=url.query
    params_map = urlparse.parse_qs(params)
    uid_ser="".join(params_map.get("uid", "0"))
    uid_str = uid_ser.replace(' ', '')
    if (uid_str.find("==")>=0 and len(uid_str)==88):
        uid=dec.decode(uid_str)
    else:
        if uid_str.find("==")>=0:
            uid="0"
        else:
            uid=uid_str
    line.append(uid)
    return line

def save(x):
    row=x.collect()    # Return a list that contains all of the elements in this RDD
    conn=MySQLdb.connect(host=mysqlHost,port=mysqlPort,user=mysqlUser,passwd=mysqlPasswd,charset='utf8')
    cur=conn.cursor()
    try:
        cur.execute("delete from dbname.device_behavior_info")          
        conn.commit()
    except MySQLdb.Error,e:
        logging.exception("Mysql Error %d: %s" % (e.args[0],e.args[1]))
        conn.rollback()
    for cid_uid, pv in row:
        tmp_list=cid_uid.encode('utf-8').split(',')
        date=tmp_list[0]
        device_id=tmp_list[1]
        user_id=int(tmp_list[2])
        platform=tmp_list[3]
        try:
            sql="insert into dbname.device_behavior_info (date, device_id, user_id, car_page_cnt,platform) \
                    values ('%s', '%s', %d, %d, '%s')  on duplicate key update car_page_cnt=car_page_cnt+%d"  %(date, device_id, user_id, pv, platform,pv)
            cur.execute(sql)          
            conn.commit()
        except MySQLdb.Error,e:
            logging.exception("Mysql Error %d: %s" % (e.args[0],e.args[1]))
            conn.rollback()
        collection_useraction.device_behavior_info.update_one({"_id":cid_uid.encode('utf-8')}, {"$inc":{"pv":pv}}, upsert=True)
    try:
        sql="insert into dbname.da_user_behavior_info(date, device_id, car_page_cnt, user_id, mobile,platform) \
                  select a.date, a.device_id, a.car_page_cnt, a.user_id, b.mobile, a.platform \
                  from dbname.device_behavior_info a \
                  left join xin.user b on a.user_id = b.userid \
                  on duplicate key update da_user_behavior_info.car_page_cnt=da_user_behavior_info.car_page_cnt+a.car_page_cnt"
        cur.execute(sql)          
        conn.commit()    
    except MySQLdb.Error,e:
        logging.exception("Mysql Error %d: %s" % (e.args[0],e.args[1]))
        conn.rollback()

    cur.close()
    conn.close()

if __name__ == '__main__':
    # 初始化SparkContext对象
    conf = SparkConf() 
    sc = SparkContext(conf=conf)
    # 处理时间间隔为10s
    ssc=StreamingContext(sc,10)
    #paths='/apps/checkpoints/spark/detailpagepv' #程序自动建目录
    #ssc.checkpoint(paths)   #缓存机制
    topic='***'
    broker='***'
    stream = KafkaUtils.createDirectStream(ssc=ssc, topics=[topic], kafkaParams={"metadata.broker.list":broker})  # 返回元祖，kafka流为第二个元素
    # 详情页log过滤 
    log=stream.map(lambda x:x[1]).\
                        map(lambda line:line.split("\t")).\
                        map(lambda line:date_format(line)).\
                        map(lambda line:log_filter(line)).\
                        filter(lambda line:line[15] != '').\
                        map(lambda line:get_cid(line)).\
                        filter(lambda line:line[16] not in  ["", "unknow"]).\
                        map(lambda line:get_uid(line)).\
                        map(lambda line:get_platform(line)).\
                        map(lambda line:('%s,%s,%s,%s' % (line[0], line[16], line[17],line[18]), 1))
    cidcount=log.reduceByKey(lambda x,y:x+y)
    cidcount.foreachRDD(save)
    # 启动spark streaming应用
    ssc.start()
    # 等待计算终止
    ssc.awaitTermination()
```
本地运行：
```python
{SPARK_HOME}/bin/spark-submit \
--master local[2] \
--name spark_streaming1 \
--jars spark-streaming-kafka-0-8-assembly_2.11-2.0.2.jar \
spark_streaming_test.py
```
提交hadoop yarn 集群：
```python
nohup \
{SPARK_HOME}/bin/spark-submit \
--master yarn \
--name spark_streaming1 \
--jars spark-streaming-kafka-0-8-assembly_2.11-2.0.2.jar \
spark_streaming_test.py  &
```


