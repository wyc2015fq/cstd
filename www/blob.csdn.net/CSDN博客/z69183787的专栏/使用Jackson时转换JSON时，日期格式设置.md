# 使用Jackson时转换JSON时，日期格式设置 - z69183787的专栏 - CSDN博客
2015年01月06日 21:56:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11849
近日，使用Jackson转化JSON对象的时候，现： 显示的时候，日期始终显示不正确，输出的日期是一串数字代表的时间戳，不符合要求，所以想到Jackson应当有方法设置输出的日期格式。后来一查果然有两种方式来实
1.普通的方式：
默认是转成timestamps形式的，通过下面方式可以取消timestamps。
objectMapper.configure(SerializationConfig.Feature.WRITE_DATES_AS_TIMESTAMPS, false); 这样将使时间生成使用所谓的use a [ISO-8601 ]-compliant notation, 输出类似如下格式的时间: "1970-01-01T00:00:00.000+0000".
当然也可以自定义输出格式：
objectMapper.getSerializationConfig().setDateFormat(myDateFormat);myDateFormat对象为java.text.DateFormat，具体使用清查java API
2.annotaion的注释方式：
先定义自己需要的格式，例如：
Java代码
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import org.codehaus.jackson.JsonGenerator;
import org.codehaus.jackson.JsonProcessingException;
import org.codehaus.jackson.map.JsonSerializer;
import org.codehaus.jackson.map.SerializerProvider;
/**
 *  java日期对象经过Jackson库转换成JSON日期格式化自定义类
 * @author yuruli
 * @date 2010-7-25
 */publicclass CustomDateSerializer extends JsonSerializer {
        @Override
        publicvoid serialize(Date value, JsonGenerator jgen, SerializerProvider provider) throws IOException, JsonProcessingException {
                SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd");
                String formattedDate = formatter.format(value);
                jgen.writeString(formattedDate);
        }
}
然后在你的POJO上找到日期的get方法
@JsonSerialize(using = CustomDateSerializer.class)
public Date getCreateAt() {
     return createAt;
}
好了，记录完毕
