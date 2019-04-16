# CCF URL映射 - 彩虹糖的博客 - CSDN博客





2018年09月14日 20:44:00[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：79








这题是魔鬼吗？这么麻烦，只给了我40分(╥╯^╰╥)

问题描述
|试题编号：|201803-3|
|----|----|
|试题名称：|URL映射|
|时间限制：|1.0s|
|内存限制：|256.0MB|
|问题描述：|问题描述　　URL 映射是诸如 Django、Ruby on Rails 等网页框架 (web frameworks) 的一个重要组件。对于从浏览器发来的 HTTP 请求，URL 映射模块会解析请求中的 URL 地址，并将其分派给相应的处理代码。现在，请你来实现一个简单的 URL 映射功能。			　　本题中 URL 映射功能的配置由若干条 URL 映射规则组成。当一个请求到达时，URL 映射功能会将请求中的 URL 地址按照配置的先后顺序逐一与这些规则进行匹配。当遇到第一条完全匹配的规则时，匹配成功，得到匹配的规则以及匹配的参数。若不能匹配任何一条规则，则匹配失败。			　　本题输入的 URL 地址是以斜杠 / 作为分隔符的路径，保证以斜杠开头。其他合法字符还包括大小写英文字母、阿拉伯数字、减号 -、下划线 _ 和小数点 .。例如，/person/123/ 是一个合法的 URL 地址，而 /person/123? 则不合法（存在不合法的字符问号 ?）。另外，英文字母区分大小写，因此 /case/ 和 /CAse/ 是不同的 URL 地址。			　　对于 URL 映射规则，同样是以斜杠开始。除了可以是正常的 URL 地址外，还可以包含参数，有以下 3 种：			　　字符串 <str>：用于匹配一段字符串，注意字符串里不能包含斜杠。例如，abcde0123。			　　整数 <int>：用于匹配一个不带符号的整数，全部由阿拉伯数字组成。例如，01234。			　　路径 <path>：用于匹配一段字符串，字符串可以包含斜杠。例如，abcd/0123/。			　　以上 3 种参数都必须匹配非空的字符串。简便起见，题目规定规则中 <str> 和 <int> 前面一定是斜杠，后面要么是斜杠，要么是规则的结束（也就是该参数是规则的最后一部分）。而 <path> 的前面一定是斜杠，后面一定是规则的结束。无论是 URL 地址还是规则，都不会出现连续的斜杠。输入格式　　输入第一行是两个正整数 *n* 和 *m*，分别表示 URL 映射的规则条数和待处理的 URL 地址个数，中间用一个空格字符分隔。			　　第 2 行至第 *n*+1 行按匹配的先后顺序描述 URL 映射规则的配置信息。第 *i*+1 行包含两个字符串 *pi* 和 *ri*，其中 *pi* 表示 URL 匹配的规则，*ri* 表示这条 URL 匹配的名字。两个字符串都非空，且不包含空格字符，两者中间用一个空格字符分隔。			　　第 *n*+2 行至第 *n*+*m*+1 行描述待处理的 URL 地址。第 *n*+1+*i* 行包含一个字符串 *qi*，表示待处理的 URL 地址，字符串中不包含空格字符。输出格式　　输入共 *m* 行，第 *i* 行表示 *qi* 的匹配结果。如果匹配成功，设匹配了规则 *pj* ，则输出对应的 *rj*。同时，如果规则中有参数，则在同一行内依次输出匹配后的参数。注意整数参数输出时要把前导零去掉。相邻两项之间用一个空格字符分隔。如果匹配失败，则输出 404。样例输入5 4			/articles/2003/ special_case_2003			/articles/<int>/ year_archive			/articles/<int>/<int>/ month_archive			/articles/<int>/<int>/<str>/ article_detail			/static/<path> static_serve			/articles/2004/			/articles/1985/09/aloha/			/articles/hello/			/static/js/jquery.js样例输出year_archive 2004			article_detail 1985 9 aloha			404			static_serve js/jquery.js样例说明　　对于第 1 个地址 /articles/2004/，无法匹配第 1 条规则，可以匹配第 2 条规则，参数为 2004。			　　对于第 2 个地址 /articles/1985/09/aloha/，只能匹配第 4 条规则，参数依次为 1985、9（已经去掉前导零）和 aloha。			　　对于第 3 个地址 /articles/hello/，无法匹配任何一条规则。			　　对于第 4 个地址 /static/js/jquery.js，可以匹配最后一条规则，参数为 js/jquery.js。数据规模和约定　　1 ≤ *n* ≤ 100，1 ≤ *m* ≤ 100。			　　所有输入行的长度不超过 100 个字符（不包含换行符）。			　　保证输入的规则都是合法的。|



40分的答案：

```java
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

public class Main {

    class Rule implements Comparable<Rule>{
        String url;
        String result;

        public Rule(String url, String result) {
            //去掉首尾的斜杠以方便处理
            url = removeSlash(url);
            this.url = url;
            this.result = result;
        }
        public int compareTo(Rule o) {
            if(o.url.split("/").length>this.url.split("/").length)
                return 1;
            return -1;
        }
    }

    class OutPut {
        String out;
        ArrayList<String> parameters;

        public OutPut(String out, ArrayList<String> parameters) {
            this.out = out;
            this.parameters = parameters;
        }

        @Override
        public String toString(){
            for(String parameter:parameters){
                if(parameter!=null&&!Main.isValidString(parameter))
                    return "404";
            }
            String result = out;
            for(String parameter:parameters){
                if(parameter!=null){
                    result=result+" "+parameter;
                }

            }
            return result;
        }
    }

    public static boolean isValidString(String s){
        char[] chars = s.toCharArray();
        boolean flag = true;
        for(char c:chars){
            boolean tmp = (c>='0'&&c<='9')||(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='-'||c=='_'||c=='.'||c=='/';
            if(!tmp){
                flag = false;
                return flag;
            }

        }
        return true;
    }

    class BooleanAndValue{
        boolean flag;
        String value;

        public BooleanAndValue(boolean flag, String value) {
            this.flag = flag;
            this.value = value;
        }
    }
    public OutPut outputStrings(String sample, ArrayList<Rule> rules) {
        OutPut outPut = null;
        String[] splits = sample.split("/");
        for (Rule rule : rules) {
            boolean flag2 = true;
            ArrayList<String> parameters = new ArrayList<>();
            String[] rule_splits = rule.url.split("/");
            for(int i=0;i<rule_splits.length;i++){
                if(rule_splits[i].equals("<path>")){
                    parameters.add(formString(splits, i));
                    outPut = new OutPut(rule.result,parameters);
                    return outPut;
                }
                else{
                    if(i>=splits.length)
                        break;
                    BooleanAndValue booleanAndValue = coString(rule_splits[i],splits[i]);
                    flag2=booleanAndValue.flag;
                    if(!flag2)
                        break;
                    else
                        parameters.add(booleanAndValue.value);
                }
            }
            if(flag2&&splits.length>=rule_splits.length){
                outPut = new OutPut(rule.result,parameters);
                return outPut;
            }
        }
        outPut = new OutPut("404",new ArrayList<>());
        return outPut;
    }

    public String formString(String[] strings, int i){
        String result = "";
        for(int j=i;j<strings.length;j++){
            result=result+strings[j]+"/";
        }
        result.substring(0,result.length()-2);
        return result;
    }

    public String removeZero(String s){
        while(s.startsWith("0")){
            s=s.substring(1);
        }
        return s;
    }
    public BooleanAndValue coString(String rule_split, String sample_split){
        BooleanAndValue booleanAndValue = null;
        if(rule_split.equals(sample_split))
            return new BooleanAndValue(true,null);
        else if(rule_split.equals("<int>")){
            boolean flag = true;
            for(int i=0;i<sample_split.length();i++){
                if (!Character.isDigit(sample_split.charAt(i))){
                    flag=false;
                }
            }
            if(flag){
                String s= removeZero(sample_split);
                return new BooleanAndValue(true,s);
            }

            else
                return new BooleanAndValue(false,null);
        }
        else if(rule_split.equals("<str>")){
            if(sample_split.contains("/")){
                return new BooleanAndValue(false,null);
            }
            return new BooleanAndValue(true,sample_split);
        }
        else
            return new BooleanAndValue(false,null);
    }

    public String removeSlash(String s) {
        s = s.substring(1);
        if (s.endsWith("/")) {
            s = s.substring(0, s.length() - 1);
        }
        return s;
    }

    public void run() {
        BufferedReader bin = new BufferedReader(new InputStreamReader(System.in));
        Scanner in = new Scanner(System.in);
        ArrayList<Rule> rules = new ArrayList<Rule>();
        ArrayList<String> samples = new ArrayList<String>();
        int ruleLen = 0;
        int sampleLen = 0;
        try{
            String firstLine = bin.readLine();
            String[] ss = firstLine.split(" ");
            ruleLen = Integer.valueOf(ss[0]);
            sampleLen =Integer.valueOf(ss[1]);
        }catch (Exception e){
            e.printStackTrace();
        }

        // 读取数据操作
        try {
            for (int i = 0; i < ruleLen; i++) {
                String state = bin.readLine();
                String[] states = state.split(" ");
                rules.add(new Rule(states[0], states[1]));
            }
            for (int i = 0; i < sampleLen; i++) {
                String sample = bin.readLine();
                samples.add(removeSlash(sample));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        Collections.sort(rules);
        ArrayList<OutPut> outPuts = new ArrayList<>();
        for(String sample:samples){
            OutPut outPut = outputStrings(sample,rules);
            outPuts.add(outPut);
        }
        for(int i=0;i<outPuts.size();i++){
            System.out.println(outPuts.get(i));
        }

    }

    public static void main(String[] args) {
        Main main = new Main();
        main.run();
    }
}
```





