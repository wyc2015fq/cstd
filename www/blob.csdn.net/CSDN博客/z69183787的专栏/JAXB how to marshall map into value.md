# JAXB: how to marshall map into value - z69183787的专栏 - CSDN博客
2013年10月23日 14:20:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3315
MapElements :
```
package com.cellfish.mediadb.rest.lucene;
import javax.xml.bind.annotation.XmlElement;
class MapElements
{
  @XmlElement public String  key;
  @XmlElement public Integer value;
  private MapElements() {} //Required by JAXB
  public MapElements(String key, Integer value)
  {
    this.key   = key;
    this.value = value;
  }
}
```
MapAdapter :
```
import java.util.HashMap;
import java.util.Map;
import javax.xml.bind.annotation.adapters.XmlAdapter;
class MapAdapter extends XmlAdapter<MapElements[], Map<String, Integer>> {
    public MapElements[] marshal(Map<String, Integer> arg0) throws Exception {
        MapElements[] mapElements = new MapElements[arg0.size()];
        int i = 0;
        for (Map.Entry<String, Integer> entry : arg0.entrySet())
            mapElements[i++] = new MapElements(entry.getKey(), entry.getValue());
        return mapElements;
    }
    public Map<String, Integer> unmarshal(MapElements[] arg0) throws Exception {
        Map<String, Integer> r = new HashMap<String, Integer>();
        for (MapElements mapelement : arg0)
            r.put(mapelement.key, mapelement.value);
        return r;
    }
}
```
The rootElement :
```
import java.util.HashMap;
import java.util.Map;
import javax.xml.bind.annotation.XmlRootElement;
import javax.xml.bind.annotation.adapters.XmlJavaTypeAdapter;
@XmlRootElement
public class Root {
    private Map<String, Integer> mapProperty;
    public Root() {
        mapProperty = new HashMap<String, Integer>();
    }
    @XmlJavaTypeAdapter(MapAdapter.class)
    public Map<String, Integer> getMapProperty() {
        return mapProperty;
    }
    public void setMapProperty(Map<String, Integer> map) {
        this.mapProperty = map;
    }
}
```
I found the code in this website : [http://www.developpez.net/forums/d972324/java/general-java/xml/hashmap-jaxb/](http://www.developpez.net/forums/d972324/java/general-java/xml/hashmap-jaxb/)
