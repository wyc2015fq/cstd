# 关于Jackson2.x中com.fasterxml.jackson包的用法 - z69183787的专栏 - CSDN博客
2014年03月27日 10:01:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：47431
Jackson应该是目前最好的json解析工具了，之前一直用的是org.codehaus.jackson包中的工具，使用的包是jackson-all-1.9.11.jar。
最近发现Jackson升级到2.2.3了，出于好奇，准备下载来测试一下，于是在官网下了一个jackson-core-2.2.3的jar包，发现使用的包名都变成了com.fasterxml.jackson，而且没有找到ObjectMapper对象，以为是整个设计全变了，看了一下jackson-core的文档，发现有个com.fasterxml.jackson.core.JsonFactory对象，于是用其来构造com.fasterxml.jackson.core.JsonGenerator，调用writeObject(pojo)方法时报pojo不是ObjectCodec对象的错误……
最后经过一番检查才发现是包没有导全的原因，Jackson2.2.3是有jackson-core-2.2.3、jackson-annotations-2.2.3、jackson-databind-2.2.3三个包的，新的ObjectMapper对象在jackson-databind-2.2.3包中，具体使用实例如下。        =====文件名：JacksonCoreTest.java=====import com.fasterxml.jackson.databind.ObjectMapper;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
/** * * @author yetao */publicclass JacksonCoreTest {
    publicstaticvoid main(String[] args) throwsIOException{
        ArrayList<String> lists = newArrayList();
        lists.add("testlist01");
        lists.add("testlist02");
        HashMap<String,String> maps = newHashMap();
        maps.put("mapkey01","mapvalue01");
        maps.put("mapkey02","mapvalue02");
        User user1 = new User();
        user1.setListinfo(lists);
        user1.setMapinfo(maps);
        user1.setNumber(100);
        user1.setUid(10);
        user1.setUname("yetao");
        user1.setUpwd("666");
        ObjectMapper mapper = new ObjectMapper();
        // 仅输出一行json字符串
        mapper.writeValue(System.out, user1);
        // 将字符串美化成多行System.out.println(mapper.writerWithDefaultPrettyPrinter().writeValueAsString(user1));
    }
}        =====文件名：User.java=====import java.util.ArrayList;
import java.util.HashMap;
publicclass User {
    privateintuid;
    privateString uname;
    privateString upwd;
    privatedoublenumber;
    privateArrayList<String> listinfo;
    privateHashMap<String,String> mapinfo;
    @Override
    publicString toString(){
        StringBuilder sb = new StringBuilder();
        sb.append("uid:").append(uid).append("\n");
        sb.append("uname:").append(uname).append("\n");
        sb.append("upwd:").append(upwd).append("\n");
        sb.append("number:").append(number).append("\n");
        sb.append("listinfo:").append("[");
        for(String info:listinfo){
            sb.append(info).append(";");
        }
        sb.append("]\n");
        sb.append("mapinfo:").append("[");
        for(Stringkey:mapinfo.keySet()){
            sb.append(key).append(":").append(mapinfo.get(key)).append(";");
        }
        sb.append("]\n");
        return sb.toString();
    }
    publicdouble getNumber() {
        returnnumber;
    }
    publicvoid setNumber(doublenumber) {
        this.number = number;
    }
    publicint getUid() {
        returnuid;
    }
    publicvoid setUid(intuid) {
        this.uid = uid;
    }
    publicString getUname() {
        return uname;
    }
    publicvoid setUname(String uname) {
        this.uname = uname;
    }
    publicString getUpwd() {
        return upwd;
    }
    publicvoid setUpwd(String upwd) {
        this.upwd = upwd;
    }
    /**     * @return the listinfo     */publicArrayList<String> getListinfo() {
        return listinfo;
    }
    /**     * @param listinfo the listinfo to set     */publicvoid setListinfo(ArrayList<String> listinfo) {
        this.listinfo = listinfo;
    }
    /**     * @return the mapinfo     */publicHashMap<String,String> getMapinfo() {
        return mapinfo;
    }
    /**     * @param mapinfo the mapinfo to set     */publicvoid setMapinfo(HashMap<String,String> mapinfo) {
        this.mapinfo = mapinfo;
    }
}美化成多行后的输出如下：{
  "uid" : 10,
  "uname" : "yetao",
  "upwd" : "666",
  "number" : 100.0,
  "listinfo" : [ "testlist01", "testlist02" ],
  "mapinfo" : {
    "mapkey02" : "mapvalue02",
    "mapkey01" : "mapvalue01"
  }
}
