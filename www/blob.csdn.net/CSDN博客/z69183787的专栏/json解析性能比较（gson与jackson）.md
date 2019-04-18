# json解析性能比较（gson与jackson） - z69183787的专栏 - CSDN博客
2015年06月18日 07:31:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2044
现在json的第三方解析工作很多，如json-lib，gson，jackson，fastjson等等。在我们完成一般的json-object转换工作时，几乎都没有任何问题。
但是当数据的量上来时，他们的性能几何呢？我着手测试了一下gson和jackson，其他的如json-lib性能很差，fastjson虽然性能很好，但是在使用时有些问题，所以这里就没有测试。
简略代码：
        //生成较大的json
        List<JsonObject> list = new ArrayList<JsonObject>();
        for (int i = 0; i < 500000; i++) {
            JsonObject obj = new JsonObject();
            obj.setId(i);
            obj.setName("name" + String.valueOf(i));
            list.add(obj);
        }
        Gson gson = new GsonBuilder().create();
        String str = gson.toJson(list);
        //1,gson解析
        long start1 = System.currentTimeMillis();
        List l = gson.fromJson(str, new TypeToken<List<JsonObject>>() {
        }.getType());
        System.out.println("gson time elapse:" + (System.currentTimeMillis() - start1));
        System.out.println(l.size());
        //2,jackson解析
        ObjectMapper mapper = new ObjectMapper();
        long start2 = System.currentTimeMillis();
        List l2 = mapper.readValue(str, new TypeReference<List<JsonObject>>() {
        });
        System.out.println("jackson time elapse:" + (System.currentTimeMillis() - start2));
        System.out.println(l2.size());
测试结果：
数据集     gson耗时         Jackson耗时
10w           1366                   138
20w           2720                   165
30w           4706                   332
40w           9526                   317
50w           本机OOM             363
从测试结果可以看出，jackson的性能几乎是gson的10倍，而且随着数据的增长，jackson的耗时也都很平稳，而gson则耗时增长明显，最后直接OOM了，
至于为什么jackson性能这么好，我也没有细究，可能是由于jackson采用流的处理方式吧。
