# java 定时执行某个函数，并获取返回值 - sxf_123456的博客 - CSDN博客
2018年10月22日 17:53:24[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：336
private static Long timpgep = 600000L;
private Long curr;
public boolean TimeRun() {
    if (curr == null || System.currentTimeMillis() - curr > timpgep) {
        return true;
    } else {
        curr = System.currentTimeMillis();
        return false;
    }
}
FluxDataMapFunction fluxDataMapFunction = new FluxDataMapFunction();
Boolean bool = fluxDataMapFunction.TimeRun();
if (bool) {
    GetHisMap getHisMap = new GetHisMap();
    Map<Float, Float> his_map = getHisMap.getmap_();
    Iterator entries = his_map.entrySet().iterator();
    while (entries.hasNext()) {
        Map.Entry entry = (Map.Entry) entries.next();
        Float key = Float.valueOf(entry.getKey().toString());
        Float val = Float.valueOf(entry.getValue().toString());
        this.a = key;
        this.b = val;
        break;
    }
}
