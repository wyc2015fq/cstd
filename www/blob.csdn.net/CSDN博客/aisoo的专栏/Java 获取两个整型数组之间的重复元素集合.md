# Java 获取两个整型数组之间的重复元素集合 - aisoo的专栏 - CSDN博客
2009年11月10日 14:08:00[aisoo](https://me.csdn.net/aisoo)阅读数：2843
    public List<Integer> findSame(int array1[], int array2[]) {
        // 重复元素结果集合
        List<Integer> result = new ArrayList<Integer>();
        // 利用hashmap来寻找重复元素
        HashMap<String, String> hashMap = new HashMap<String, String>();
        // 将第一个数组加入hashmap
        for (int i = 0; i < array1.length; i++) {
            String temp = array1[i] + "";
            hashMap.put(temp, temp);
        }
        // 遍历第二个数组
        for (int i = 0; i < array2.length; i++) {
            String temp = array2[i] + "";
            // 在已经存在第一个数组所有元素的hashmap里寻找第二数组里的元素
            if (hashMap.get(temp) != null) {
                // 将重复出现的元素加入结果集合
                result.add(array2[i]);
            }
        }
        return result;
    }
