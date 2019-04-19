# python 提取光猫下挂设备路由器信息 - sxf_123456的博客 - CSDN博客
2018年05月18日 19:51:07[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：345
#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2018/5/11 0011 20:33
# @Author  : Sxf
# @QQ      :
# @File    : get_dev.py
# @Software: PyCharm Community Edition
import pymysql
import  re
read_path = "F:\\data_mac\\data\\mysql\\test.txt"
write_path = "F:\\data_mac\\data\\mysql\\data_2.txt"
# read_path = "/home/data/mysql/test/data/new_customer.txt"
# write_path = "/home/data/mysql/test/data/data.txt"
read_file = open(read_path, "r", encoding='utf-8')
write_file = open(write_path, "w")
connect = pymysql.connect(host="", port=3306, user="root", passwd="", db="test", charset="utf8")
type_dict = {
    "-1":"未知",
"0":"光猫",
"1":"机顶盒",
"2":"路由器",
"3":"手机",
"4":"平板",
"5":"电脑",
"6":"中继器",
"7":"交换机",
"8":"其他"
}
# loid,factory_name,model_name,wifi_count,port_count,hard_version
line_title = 'loid'+','+'光猫厂商'+','+'光猫型号'+','+'是否使用光猫WiFi'+','+'光猫类型（四口或两口）'+','+'光猫硬件版本'+','+'LAN口下挂设备MAC'+','+'下挂设备类型'+','+'下挂设备厂家'+','+'下挂设备型号'+','+'是否使用LAN口'+','+'千兆网口位置'+','+'带宽支持200M'+','+'路由器性能'+','+'是否暑促路由器'+','+'下挂设备是否有路由器''\n'
write_file.write(line_title)
for line in read_file:
    line_ = line.strip()
    line_cus = line_.split(',')
    loid = line_cus[0]
    hard_ver = line_cus[5]
    model_hgw = line_cus[2]
    sql = """select mac,type,factory_name,model_name,port from ana_device_20180109 where loid='%s'"""%(loid)
    type_lyq = []
    with connect.cursor() as cursor:
        cursor.execute(sql)
        result_ = cursor.fetchall()
        for res in result_:
            type_lyq.append(res[1])
        for result in result_:
            try:
                mac = result[0]
                type_ = result[1]
                # lyq_model = result[3]
if result[1] is None:
                    type = -1
else:
                    type = type_dict.get(str(type_))
                if result[2] is None:
                    factory = ""
else:
                    factory = result[2]
                # model = result[3]
if result[3] is None:
                    lyq_model = ""
else:
                    _model = str(result[3]).split("||")
                    lyq_model = _model[0]
                port = result[4]
                if port == 1 or port == 4:
                    _port = "是"
else:
                    _port = "否"
if model_hgw == "None":
                    line_.replace("None","")
                    mport = ""
band = ""
else:
                    sql_hgw_class = """SELECT mbport,band_width FROM ana_hgw_class WHERE model_name='%s';"""%(model_hgw)
                    cursor.execute(sql_hgw_class)
                    hgw_result = cursor.fetchone()
                    if hgw_result is None:
                        mport = ""
band = ""
else:
                        if hgw_result[0] is None:
                            # print(hgw_result[0])
mport = ""
else:
                            mport = hgw_result[0]
                        # band_width =
if hgw_result[1] is None:
                            band = ""
elif hgw_result[1] >= 200:
                            band = "是"
else:
                            band = "否"
if str(type_) == "2":
                    sql_router = """SELECT grade FROM ana_router_proformance WHERE model_name='%s';"""%(lyq_model)
                    cursor.execute(sql_router)
                    route_res = cursor.fetchone()
                    if route_res is None:
                        grade_ = ""
cu_ = ""
else:
                        if route_res[0] is None:
                            grade_ = ""
elif str(route_res[0]) == "2":
                            grade_ = "高"
elif str(route_res[0]) == "1":
                            grade_ = "中"
elif str(route_res[0]) == "0":
                            grade_ = "低"
else:
                            grade_ = ""
# lower_model = str(lyq_model).lower()
list_type = ["WS831","WS851","R3G","R3L","E8820","WR886N","WDR6300","AC6","AC9","HM200W","HS200W"]
                        for model_type in list_type:
                            if re.search(model_type.strip(),lyq_model):
                                cu_ = "是"
break
                            else:
                                cu_ = "否"
line_str = line_ + "," + mac + "," + str(type) + "," + factory + "," + lyq_model + "," + _port+","+str(mport)+","+ band+","+grade_+","+cu_+","+"是"+"\n"
write_file.write(line_str)
                else:
                    if "2" in type_lyq:
                        continue
                    else:
                        line_str = line_ + "," + mac + "," + str(type) + "," + factory + "," + "用户没有路由器" + "," + "" + "," + "" + "," + "" + "," +""+ "," +""  +","+"否"+"\n"
write_file.write(line_str)
                        break
            except:
                raise  Exception
read_file.close()
write_file.close()
