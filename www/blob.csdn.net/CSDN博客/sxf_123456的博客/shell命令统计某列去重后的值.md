# shell命令统计某列去重后的值 - sxf_123456的博客 - CSDN博客
2018年03月13日 15:00:39[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1573
需求：根据MAC获取，MAC对应的硬件版本和软件版本。在根据硬件版本统计MAC的数量
$ head test_1.txt
00:07:67,EC2108,HWV010537P0000
00:07:63,Z83,ZNV861010P1225
00:07:67,EC6108V9U_ca_sccdx,HWV218012P0000
00:07:63,SBox8900hisi3716V3,ZGV865012P0001
00:07:67,EC6108V9U_ca_sccdx,HWV218012P0000
00:21:26,EC6108V9U_ca_sccdx,HWV218012P0000
00:07:67,EC6108V9U_ca_sccdx,HWV218012P0000
00:07:63,STBHD-ST7105-000,ZTV70433089P0002
00:21:26,EC6108V9U_ca_sccdx,HWV215012P0000
00:21:26,EC6108V9U_ca_sccdx,HWV218012P0000
第二列去重
$ cat test_2.txt | awk -F ',' '{print $2}' | sort | uniq
EC2108
EC6108V9U_ca_sccdx
SBox8900hisi3716V3
STBHD-ST7105-000
Z83
将字符串转化为列表
**import **os
path = **"C:****\\****Users****\\****Administrator****\\****Desktop****\\****stb****\\****dd.txt"**read = open(path,**"r"**)
list_ = []
**for **line **in **read:
    line_ = line.strip()
    list_.append(line_)
print(list_)
根据厂商和硬件版本获取对应的软件版本
方法一：
GET gather-012-20180109/_search
{
  "size": 0, 
  "aggs": {
    "1": {
      "terms": {
        "field": "dev_hard_ver",
        "size": 200
      },
      "aggs": {
        "2": {
          "terms": {
            "field": "soft_ver",
            "size": 1
          }
        }
      }
    }
  }
}
方法二：
**import **os
**from **elasticsearch **import **Elasticsearch
path = **"C:****\\****Users****\\****Administrator****\\****Desktop****\\****stb****\\****fact_stb.txt"**write_file = open(path,**"a+"**)
es = Elasticsearch()
**def **get_Data(dev_hard_ver,dev_vendor_name):
    index_ = **"gather-012-20180109"****try**:
        rs = es.search(index=index_,body={
            **"size"**: 1,
              **"query"**: {
                **"bool"**: {
                  **"must"**: [
                    {**"term"**: {
                      **"dev_hard_ver"**: {
                        **"value"**: dev_hard_ver
                      }
                    }
                  },
                  {
                    **"term"**: {
                      **"dev_vendor_name"**: {
                        **"value"**: dev_vendor_name
                      }
                    }
                  }
                  ]
                }
              },
              **"_source"**: [**"soft_ver"**,**"dev_vendor_name"**,**"dev_hard_ver"**]
        })
        **return  **rs
    **except**:
        **return **-1
dev_hard_ver=[**"02L0B61701MTAS0001"**, **"0x209"**, **"0x441"**, **"0x485"**, **"0xE0A"**, **"0xE17"**, **"16060102L0B61701MTAS0001"**, **"1828"**, **"1903"**, **"2035"**, **"5202"**, **"99007002L0B6170635100791"**, **"9900B002L0B61701IHO3000T"**, **"990104010004102300000001"**, **"99010402L0B6083100000001"**, **"990501010003170908A5C800"**, **"A7.32.64.A0"**, **"B860AV2.1"**, **"BJDS2Z-52"**, **"CHEP3000SC"**, **"CHI619112"**, **"CHIHO3000"**, **"CHIHO3000A"**, **"CHIHO3000JX2"**, **"CHIHO33"**, **"CHIHO3300A"**, **"CHIHO3300AD"**, **"CUB-ANDROIDSTB"**, **"EC2106V1"**, **"EC2108"**, **"EC6108V9_pub_gdydx"**, **"EC6108V9U_ca_sccdx"**, **"EC6108V9U_pub_hbjdx"**, **"EC6108V9U_pub_sccdx"**, **"G2-40_2544"**, **"HG510I"**, **"HG650-TDSWH"**, **"HG650-TDSWH-11"**, **"HT670-V"**, **"HY_V1.0"**, **"HY107"**, **"HY112"**, **"HY127"**, **"HY2021"**, **"HY34E71C02L0B6V003HG510P04"**, **"HY34E71C02L0B6V3.0HG510P10"**, **"HY44170102L0B6V003HG510P04"**, **"HY9900E0013798KK0120170001"**, **"HY990104010004102300000001"**, **"HYC37.E"**, **"HYCHEP920SC"**, **"HYCHEP921SC"**, **"HYEE"**, **"HYHG510IV1.0"**, **"HYHG510PV3.0"**, **"HYTY_H1.00.00"**, **"HYV10-ANDROIDIGW-068"**, **"HYYiX-G210V1.1"**, **"IBHN-ANDROIDSTB-068"**, **"IBHR-ANDROIDSTBGA-069"**, **"IBHU-ANDROIDSTB-068"**, **"IBHX-ANDROIDSTBGA-068"**, **"IBHX-ANDROIDSTBGA-069"**, **"IPTV-S63-V1.1"**, **"IPTV-S65-V1.1"**, **"IPTV-S65-V1.2"**, **"ITV628 HD"**, **"KB1.100.01H"**, **"M8043V02"**, **"MR222-DSGH-19"**, **"mx_ca_sccdx"**, **"S80-E-R1.01.01"**, **"SBox8900hisi3716V3"**, **"SBox8900hisi3716V386501007"**, **"STBAB-ANDROIDSTB-068"**, **"STBAB-ANDROIDSTBGA-069"**, **"STBHD-AML8626X-000"**, **"STBHD-ST7105-000"**, **"STBHD-ST7105-128-256-000"**, **"STBHO-AML8626X-000"**, **"tgh0001"**, **"TY_A1.00.00"**, **"TY_H1.00.00"**, **"V1"**, **"V10-ANDROIDIGW-068"**, **"V2.1"**, **"Z82"**, **"Z83"**, **"Z85E"**, **"ZXB700V5C(IBHL-000-000)"**, **"ZXB860A(IBHR-ANDROIDSTB-068)"**, **"ZXB860A(IBHR-ANDROIDSTBGA-068)"**]
dev_vendor_name=[**"SCTY"**, **"CHANGHONG"**, **"Jiuzhou"**, **"Jiuzhou"**, **"Hisense"**, **"Hisense"**, **"SCTY"**, **"SKYWORTH"**, **"SKYWORTH"**, **"SKYWORTH"**, **"Huawei"**, **"ZTE"**, **"CHANGHONG"**, **"SKYWORTH"**, **"SKYWORTH"**, **"Zhaoneng"**, **"UTSTARCOM"**, **"ZTE"**, **"Fiberhome"**, **"CHANGHONG"**, **"CHANGHONG"**, **"CHANGHONG"**, **"CHANGHONG"**, **"CHANGHONG"**, **"CHANGHONG"**, **"CHANGHONG"**, **"CHANGHONG"**, **"YOUHUA"**, **"Huawei"**, **"Huawei"**, **"Huawei"**, **"Huawei"**, **"Huawei"**, **"Huawei"**, **"YINHE"**, **"UTStarcom"**, **"Fiberhome"**, **"Fiberhome"**, **"Zhaoneng"**, **"SCTY"**, **"Hisense"**, **"Hisense"**, **"Hisense"**, **"SKYWORTH"**, **"YHTC"**, **"YHTC"**, **"YHTC"**, **"FiberHome"**, **"SKYWORTH"**, **"Huawei"**, **"CHANGHONG"**, **"CHANGHONG"**, **"Hisense"**, **"UTStarcom"**, **"YHTC"**, **"SCTY"**, **"ZTE"**, **"CCS"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"ChinaGCI"**, **"ChinaGCI"**, **"ChinaGCI"**, **"ChangHong"**, **"30kt"**, **"Huawei"**, **"FiberHome"**, **"Huawei"**, **"ChinaGCI"**, **"Sunniwell"**, **"Sunniwell"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"ZTE"**, **"Yihekeji"**, **"SCTY"**, **"SCTY"**, **"ZTE"**, **"ZTE"**, **"FiberHome"**, **"Zhaoneng"**, **"Zhaoneng"**, **"Zhaoneng"**, **"ZTE"**, **"ZTE"**, **"ZTE"**]
**for **i **in **range(len(dev_vendor_name)):
    rs = get_Data(dev_hard_ver[i],dev_vendor_name[i])
    **if **rs!=-1:
        **for **hit **in **rs[**'hits'**][**'hits'**]:
            soft_ver=hit[**'_source'**][**'soft_ver'**]
            **if **soft_ver **is None**:
                soft_ver=**'None'**write_file.write(soft_ver+**"****\n****"**)
        **else**:
            write_file.write(**"-1" **+ **"****\n****"**)
write_file.close()
