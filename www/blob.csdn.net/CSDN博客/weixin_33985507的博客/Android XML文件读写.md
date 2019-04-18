# Android XML文件读写 - weixin_33985507的博客 - CSDN博客
2012年07月26日 11:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
```
<?xml version="1.0" encoding="UTF-8"?>
<persons>
    <person id="23">
        <name>eboy</name>
        <age>22</age>
    </person>
    <person id="24">
        <name>Frr</name>
        <age>20</age>
    </person>
</persons>
```
package com.eboy.service;
```
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlSerializer;
import com.eboy.domain.Person;
import android.util.Xml;
public class PersonService {
    public static List<Person> getPersons(InputStream xml) throws Exception {        
        List<Person> persons = null;
        Person person = null;
        XmlPullParser pullParser = Xml.newPullParser();
        pullParser.setInput(xml, "UTF-8"); //为Pull解释器设置要解析的XML数据        
        int event = pullParser.getEventType();
        
        while (event != XmlPullParser.END_DOCUMENT){
            
            switch (event) {
            
            case XmlPullParser.START_DOCUMENT:
                persons = new ArrayList<Person>();                
                break;    
            case XmlPullParser.START_TAG:    
                if ("person".equals(pullParser.getName())){
                    int id = Integer.valueOf(pullParser.getAttributeValue(0));
                    person = new Person();
                    person.setId(id);
                }
                if ("name".equals(pullParser.getName())){
                    String name = pullParser.nextText();
                    person.setName(name);
                }                                
                if ("age".equals(pullParser.getName())){
                    int age = Integer.valueOf(pullParser.nextText());
                    person.setAge(age);
                }
                break;
                
            case XmlPullParser.END_TAG:
                if ("person".equals(pullParser.getName())){
                    persons.add(person);
                    person = null;
                }
                break;
                
            }
            
            event = pullParser.next();
        }
        
        
        return persons;
    }
    
    /**
     * 保存数据到xml文件中
     * @param persons
     * @param out
     * @throws Exception
     */
    public static void save(List<Person> persons, OutputStream out) throws Exception {
        XmlSerializer serializer = Xml.newSerializer();
        serializer.setOutput(out, "UTF-8");
        serializer.startDocument("UTF-8", true);
        serializer.startTag(null, "persons");        
        for (Person person : persons) {
            serializer.startTag(null, "person");            
            serializer.attribute(null, "id", person.getId().toString());            
            serializer.startTag(null, "name");            
            serializer.text(person.getName().toString());
            serializer.endTag(null, "name");            
            serializer.startTag(null, "age");            
            serializer.text(person.getAge().toString());
            serializer.endTag(null, "age");            
            serializer.endTag(null, "person");
        }        
        serializer.endTag(null, "persons");
        serializer.endDocument();
        out.flush();
        out.close();
    }
}
```
[/Files/jxgxy/com.eboy.readwritexml.MainActivity.rar](http://files.cnblogs.com/jxgxy/com.eboy.readwritexml.MainActivity.rar)
