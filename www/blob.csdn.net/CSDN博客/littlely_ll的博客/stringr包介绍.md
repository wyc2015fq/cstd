# stringr包介绍 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:27:15[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1011









1. case用法：

str_to_upper(string, locale = "")

str_to_lower(string, locale = "")

str_to_title(string, locale = "")

string为要处理的字符串；locale为要翻译的...。
[1] "THE QUICK BROWN DOG"
[1] "the quick brown dog"
[1] "The Quick Brown Dog"
[1] "I"
[1] "İ"


2. 的用法：
str_c(..., sep = "", collapse = NULL)


... 为一组字符串向量；sep为插入字符串向量的字符串；collapse为把输入的字符串合并为单个字符串（默认没有）
 [1] "Letter: a" "Letter: b" "Letter: c" "Letter: d" "Letter: e"
 [6] "Letter: f" "Letter: g" "Letter: h" "Letter: i" "Letter: j"
[11] "Letter: k" "Letter: l" "Letter: m" "Letter: n" "Letter: o"
[16] "Letter: p" "Letter: q" "Letter: r" "Letter: s" "Letter: t"
[21] "Letter: u" "Letter: v" "Letter: w" "Letter: x" "Letter: y"
[26] "Letter: z"
[1] "abcdefghijklmnopqrstuvwxyz"
[1] "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z"
[1] "a-d" NA    "B-d"


[1] "a-d"  "NA-d" "b-d" 


3. 的用法：


str_count(string, pattern = "")


string为字符串；pattern为寻找模式。 [1] 1 3 1 1
[1] 2 0 1 3
[1] 1 1 1 3
[1] 2 3 4
[1] 1 3 2


4. 的用法：


str_detect(string, pattern)


string与pattern如3.
[1] TRUE TRUE TRUE TRUE
[1]  TRUE FALSE FALSE FALSE
[1] FALSE  TRUE FALSE FALSE

5. 的用法：


str_extract(string, pattern)

str_extract_all(string, pattern, simplify = FALSE)


string,pattern如上；simplify：FALSE为返回字符串向量，TRUE为返回字符串矩阵。
[1] "4" NA  NA  "2"
[1] "apples" "bag"    "bag"    "milk"  
[1] "appl" "bag"  "bag"  "milk"
[1] NA     "bag"  "bag"  "milk"
[[1]]
[1] "apples" "x"     

[[2]]
[1] "bag"   "of"    "flour"

[[3]]
[1] "bag"   "of"    "sugar"

[[4]]
[1] "milk" "x"   

[[1]]
[1] "apples"

[[2]]
[1] "bag"   "of"    "flour"

[[3]]
[1] "bag"   "of"    "sugar"

[[4]]
     [,1]     [,2] [,3]   
[1,] "apples" ""   ""     
[2,] "bag"    "of" "flour"
[3,] "bag"    "of" "sugar"
[4,] "milk"   ""   ""     
[[1]]
[1] "This"        "is"          "suprisingly" "a"          
[5] "sentence"   


6. 的用法：


str_match(string, pattern)


string与pattern用法如上。
 [1] " 219 733 8965"                 "329-293-8753 "                
 [3] "banana"                        "595 794 7569"                 
 [5] "387 287 6718"                  "apple"                        
 [7] "233.398.9187 "                 "482 952 3315"                 
 [9] "239 923 8115 and 842 566 4692" "Work: 579-499-7527"           
[11] "$1000"                         "Home: 543.355.3679"           
 [1] "219 733 8965" "329-293-8753" NA             "595 794 7569" "387 287 6718"
 [6] NA             "233.398.9187" "482 952 3315" "239 923 8115" "579-499-7527"
[11] NA             "543.355.3679"
      [,1]           [,2]  [,3]  [,4]  
 [1,] "219 733 8965" "219" "733" "8965"
 [2,] "329-293-8753" "329" "293" "8753"
 [3,] NA             NA    NA    NA    
 [4,] "595 794 7569" "595" "794" "7569"
 [5,] "387 287 6718" "387" "287" "6718"
 [6,] NA             NA    NA    NA    
 [7,] "233.398.9187" "233" "398" "9187"
 [8,] "482 952 3315" "482" "952" "3315"
 [9,] "239 923 8115" "239" "923" "8115"
[10,] "579-499-7527" "579" "499" "7527"
[11,] NA             NA    NA    NA    
[12,] "543.355.3679" "543" "355" "3679"


7.的用法：


str_pad(string, width, side = c("left", "right", "both"), pad = " ")


string为字符串；width为空格的最小宽度；side为空格字符填充的方向；pad为填充的单个字符，默认的为空格。
     [,1]                            
[1,] "                        hadley"
[2,] "hadley                        "
[3,] "            hadley            "
     [,1]                            
[1,] "........................hadley"
[2,] "hadley........................"
[3,] "............hadley............"


8. 的用法：


str_replace(string, pattern, replacement)


string为字符串；pattern为要替换的内容，常为正则表达式；replacement为替换者。
[1] "-ne apple"     "tw- pears"     "thr-e bananas"
[1] "-n- -ppl-"     "tw- p--rs"     "thr-- b-n-n-s"
[1] "ne apple"     "tw pears"     "thre bananas"
[1] "1 apple---2 pears---3 bananas"


9. 的用法：


str_split(string, pattern, n = Inf, simplify = FALSE)


str_split_fixed(string, pattern, n)


string为字符串；pattern为分离模式；n为分割为多少块；simplify：FALSE的时候返回字符串向量列表，为TRUE的时候返回字符串矩阵。


10. 的用法：


str_sub(string, start = 1L, end = -1L)


string为字符串；start和end分别为开始和结束字符。


11.的用法：


str_subset(string, pattern)


string与pattern用法如上。
[1] "apple"    "banana"   "pear"     "pinapple"
[1] "apple"
[1]  TRUE FALSE FALSE FALSE
[1] "banana"
[1] "banana"
[1] "apple"    "banana"   "pear"     "pinapple"
[1] "Hadley"
[1] "Hadley"
[1] "Wickham"
[1] "Hadley"  "Wickham"
[1] "m"
[1] "Wickham"
[1] "B"
[1] "ABCDEF"
[1] "ABCDEK"
[1] "ABCDGHIJK"
[1] "AK"
[1] "apples and oranges and pears and bananas" "pineapples and mangos and guavas"        
[[1]]
[1] "apples "   " oranges " " pears "   " bananas" 

[[2]]
[1] "pineapples " " mangos "    " guavas"    

     [,1]          [,2]        [,3]      [,4]      
[1,] "apples "     " oranges " " pears " " bananas"
[2,] "pineapples " " mangos "  " guavas" ""  
[[1]]
[1] "apples "            " oranges "          " pears and bananas"

[[2]]
[1] "pineapples " " mangos "    " guavas"    
[[1]]
[1] "apples "   " oranges " " pears "   " bananas" 

[[2]]
[1] "pineapples " " mangos "    " guavas"    

     [,1]          [,2]        [,3]                
[1,] "apples "     " oranges " " pears and bananas"
[2,] "pineapples " " mangos "  " guavas"           
     [,1]          [,2]        [,3]      [,4]      
[1,] "apples "     " oranges " " pears " " bananas"
[2,] "pineapples " " mangos "  " guavas" ""     
[1] "a" "b"



12. word的用法：

 word(string, start = 1L, end = start, sep =
fixed(" "))

sep为单词之间的分隔符。
[1] "Jane saw a cat" "Jane sat down" 
[1] "Jane" "Jane"
[1] "saw" "sat"
[1] "cat"  "down"
[1] "saw a cat" "sat down" 
[1] "Jane saw a cat" "saw a cat"      "a cat"         
[1] "Jane"           "Jane saw"       "Jane saw a"     "Jane saw a cat"
[1] "abc.def"
[1] "123.4568.999"
[1] "abc.def..123.4568.999"




