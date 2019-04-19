# csv中字段类型转化与mutate中的convert比较 - sxf_123456的博客 - CSDN博客
2017年12月20日 17:41:27[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：99
                
csv中字段类型转化与mutate中的convert比较
convert 哈希类型
可以转化为integer、float、date、date_time、boolean类型
filter {
    csv {
        convert => { 
            "column1" => "integer",
            "column2" => "float",
            "column3" => "boolean",
            "column4" => "string"
            }
    }
}
mutate 与csv 含有的convert比较
上边使用csv中的convert，这种方法使用时出现异常，
采用mutate中的convert
filter{
    csv {
columns => ["column1","column2","column3","column4"]
    }
    mutate {
convert => {"column1" => "integer"}
convert => {"column2" => "float"}
convert => {"column3" => "boolean"}
        convert => {"column4" => "string"}
    }
}
            
