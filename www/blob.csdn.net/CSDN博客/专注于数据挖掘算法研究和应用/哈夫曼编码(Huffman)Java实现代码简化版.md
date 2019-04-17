# 哈夫曼编码(Huffman)Java实现代码简化版 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月14日 17:12:24[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5035
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









这个网上发现的Huffuman编码Java实现在组织上相对简化，便于理解文件压缩过程：提取文件统计字符频度-根据字符频度创建huffman树-根据huffman树生成huffman可变字长无前缀编码-根据huffman编码对文件中的字符转化成二进制串-将huffman编码的二进制串（非固定8位，可变字长）转化成8位固定字节的字符并输出文件。

代码中对于Java数据类型的使用也值得参考。



```java
package cn.hm;

import java.io.File;  
import java.io.FileInputStream;  
import java.io.FileOutputStream;  
import java.io.InputStream;  
import java.io.OutputStream;  
import java.util.ArrayList;  
import java.util.Comparator;  
import java.util.HashMap;  
import java.util.Iterator;  
import java.util.PriorityQueue;  
import java.util.Set;  
  
public class HFMCondense {  
	
	public class HFMNode {  
	    byte data;  //存储字节的数据域  
	    int value;  //字节出现的频率  
	    String code;//叶子结点的哈弗曼编码  
	    HFMNode lchild,rchild;//左右孩子的引用  
	    //只指定数据的构造体  
	    public HFMNode(byte data,int rate){  
	        this(data,rate,null,null);  
	    }  
	    //同时指定左右孩子的构造体  
	    public HFMNode(byte data,int value,HFMNode lchild,HFMNode rchild){  
	        this.data=data;  
	        this.value=value;  
	        this.lchild=lchild;  
	        this.rchild=rchild;  
	    }  
	}  
	
    public static void main(String args[]){  
        String file="D://tmp//ori.txt";  
        HFMCondense condense=new HFMCondense(); 
        HFMNode hfmTree=condense.HashMapToHFMTree(condense.readFiletoMap(file));  
        condense.HuffmanCoding(hfmTree, "");  
        System.out.println("开始压缩...");  
        long start=System.currentTimeMillis();  
        condense.CompressFile(condense.createByteArray(condense.FileToString(file)),"D://tmp//des");  
        System.out.println("压缩结束...用时:"+(System.currentTimeMillis()-start));  
    }  
    /** 
     * 读取将要被压缩的文件，统计每一个字符出现的频率，并将得到的内容存入HashMap中 
     * @param fileName 将要被压缩的文件 
     * @return 每一个字节数出现的频率所对应的HashMap 
     */  
    public HashMap<Byte,Integer> readFiletoMap(String fileName){  
        HashMap<Byte,Integer> hashMap=new HashMap<Byte,Integer>();  
        File file=new File(fileName);  
        if(file.exists()){  
            try{  
                InputStream in=new FileInputStream(file);  
                //创建与文件大小相同的字节数组  
                byte[] content=new byte[in.available()];  
                //读取文件  
                in.read(content);  
                //存入HashMap中  
                for(int i=0;i<content.length;i++){  
                    //如果表中存在某一个键  
                    if(hashMap.containsKey(content[i])){  
                        //获取该字节当前的键值  
                        int rate=hashMap.get(content[i]);  
                        //键值增大  
                        rate++;  
                        hashMap.put(content[i], rate);  
                    }  
                    //如果不存在某一个字节对象,则将它存入HashMap当中  
                    else{  
                        hashMap.put(content[i],1);  
                    }  
                }  
                in.close();  
            }catch(Exception e){  
                e.printStackTrace();  
            }     
        }  
        else{  
            System.out.println("文件不存在");  
        }  
        return hashMap;  
    }  
    /** 
     * 将HashMap中的元素取出来封装到哈弗曼树中，树中叶子结点保存的是HashMap中的每一个键值与频率 
     * @param map 读取的Map 
     * @return  哈夫曼树的根结点 
     */  
    public HFMNode HashMapToHFMTree(HashMap<Byte,Integer> map){  
        //得到存储键值的系  
        Set<Byte> keys=map.keySet();  
        //得到迭代器对象  
        Iterator<Byte> iter=keys.iterator();  
        //如果还有值  
        while(iter.hasNext()){  
            byte key=iter.next();//获取系中的键  
            int value=map.get(key);//得到该键出现的频率  
            //创建结点并将结点对象加入到队列当中  
            HFMNode node=new HFMNode(key,value);  
            nodeQueue.add(node);  
            nodeList.add(node);  
        }  
        //当所剩的结点数还大于两个  
        while(nodeQueue.size()>=2){  
            //得到键值频率最小的两个结点  
            HFMNode left=nodeQueue.poll();  
            HFMNode right=nodeQueue.poll();  
            //将这两个结点组合起来生成新的结点  
            HFMNode node=new HFMNode(left.data,left.value+right.value,left,right);  
            nodeQueue.add(node);  
        }  
        //获取队列中的最后一个结点作为根结点  
        HFMNode hfm=nodeQueue.poll();  
        return hfm;  
    }  
    /** 
     * 为生成的哈弗曼树进行编码，产生对应的哈弗曼编码表 
     * @param hfm  对应的哈弗曼树 
     * @param code 对应生成的哈弗曼编码 
     * @return 哈弗曼编码表 
     */  
    //创建一个新的哈弗曼编码表  
    HashMap<Byte,String> codeMap=new HashMap<Byte,String>();  
    public HashMap<Byte,String> HuffmanCoding(HFMNode hfm,String code){  
        //如果左子树不为空，则左子树编码加1  
        if(hfm.lchild!=null){  
            HuffmanCoding(hfm.lchild,code+"1");  
        }  
        //如果右子树不为空，则右子树编码加0  
        if(hfm.rchild!=null){  
            HuffmanCoding(hfm.rchild,code+"0");  
        }  
        //如果到达叶子结点，则将元素放入HashMap中生成哈弗曼编码表  
        if(hfm.lchild==null&&hfm.rchild==null){  
            codeMap.put(hfm.data,code);  
            hfm.code=code;  
        }  
        return codeMap;  
    }  
    /** 
     * 将哈弗曼编码转换成字符串 
     * @param fileName 读取的文件名 
     * @return 编码之后的哈弗曼字符串 
     */  
    public String CodeToString(String fileName){  
        File file=new File(fileName);  
        String codeString="";  
        //如果文件存在  
        if(file.exists()){  
            try{  
                InputStream in=new FileInputStream(file);  
                byte content[]=new byte[in.available()];  
                in.read(content);  
                int i=0;  
                int len=content.length;//得到文件的字节长度  
                int size=nodeList.size();//得到队列的长度  
                while(i<len){  
                    for(int j=0;j<size;j++){  
                        if(content[i]==nodeList.get(j).data){  
                            codeString+=nodeList.get(j).code;  
                            break;  
                        }  
                    }  
                    i++;  
                }  
                in.close();  
            }catch(Exception e){  
                e.printStackTrace();  
            }  
        }else {  
            System.out.println("文件不存在");  
        }  
        return codeString;  
    }  
    /** 
     * 将文件按照对应的哈弗曼编码表转成01字符串 
     * @param fileName 读入的文件名 
     * @return 转译后的字符串 
     */  
    public String FileToString(String fileName){  
        File file=new File(fileName);  
        String StringContent="";  
        //如果文件存在  
        if(file.exists()){  
            try{  
                InputStream in=new FileInputStream(file);  
                byte content[]=new byte[in.available()];  
                in.read(content);  
                //循环转译  
                int len=content.length;  
                for(int i=0;i<len;i++){  
                    StringContent+=codeMap.get(content[i]);  
                }  
                in.close();  
            }catch(Exception e){  
                e.printStackTrace();  
            }  
        }else{  
            System.out.println("文件不存在");  
        }  
        return StringContent;  
    }  
    /** 
     * 将转译后的01字符串重新转换后放入新的字节数组当中 
     * @param code 转译后的01字符串 
     * @return 新的字节数组，里面包含了压缩后的字节内容 
     */  
    public byte[] createByteArray(String code) {  
        //将每8位字符分隔开来得到字节数组的长度  
        int size=code.length()/8;  
        //截取得到字符串8整数后的最后几个字符串  
        String destString=code.substring(size*8);     
        byte dest[]=destString.getBytes();    
        //s用来记录字节数组的单字节内容  
        int s = 0;  
        int i=0;        
        int temp = 0;     
        // 将字符数组转换成字节数组，得到字符的字节内容,方便将二进制转为十进制  
        byte content[] = code.getBytes();     
        for (int k = 0; k < content.length; k++) {     
            content[k] = (byte) (content[k] - 48);     
        }  
        //转译后的字节内容数组  
        byte byteContent[];     
        if (content.length % 8 == 0) {// 如果该字符串正好是8的倍数     
            byteContent = new byte[content.length / 8 + 1];     
            byteContent[byteContent.length - 1] = 0;// 那么返回的字节内容数组的最后一个数就补0     
        } else {     
            //否则该数组的最后一个数就是补0的个数  
            byteContent = new byte[content.length / 8 + 2];     
            byteContent[byteContent.length - 1] = (byte) (8 - content.length % 8);     
        }     
        int bytelen=byteContent.length;  
        int contentlen=content.length;  
        // byteContent数组中最后一个是补0的个数，实际操作到次后个元素  
        //Math.pow返回第一个参数的第二个参数次幂的值  
        while (i < bytelen - 2) {     
            for (int j = 0; j < contentlen; j++) {     
                if (content[j] == 1) {// 如果数组content的值为1     
                    s =(int)(s + Math.pow(2, (7 - (j - 8 * i))));// 累积求和     
                }// if     
                if ((j+1)%8==0) {// 当有8个元素时     
                    byteContent[i] = (byte) s;// 就将求出的和放入到byteContent数组中去     
                    i++;     
                    s = 0;// 并重新使s的值赋为0     
                }// if     
            }// for     
        }// while       
        int destlen=dest.length;  
        for(int n=0;n<destlen;n++){     
            temp+=(dest[n]-48)*Math.pow(2, 7-n);//求倒数第2个字节的大小     
        }    
        byteContent[byteContent.length - 2] = (byte) temp;   
        return byteContent;     
    }    
    /** 
     * 压缩并输出新文件 
     * @param content 压缩后产生的新的字节数组 
     * @param fileName 输出文件名 
     */  
    public void CompressFile(byte[] content,String fileName){  
        File file=null;  
        //统一后缀名  
        if(!fileName.endsWith("hfm")){  
            file=new File(fileName+".hfm");  
        }else if(fileName.endsWith("hfm")){  
            file=new File(fileName);      
        }  
        int len=content.length;  
        if(len>0){  
            try{  
                OutputStream out=new FileOutputStream(file);  
                //将字节内容写入文件  
                out.write(content);  
                out.close();  
            }catch(Exception e){  
                e.printStackTrace();  
            }  
        }else{  
            System.out.println("压缩出错");  
        }  
    }  
    /** 
     * 测试一下哈弗曼树建立是否正确 
     * @param hfm 
     */  
    public void PreOrderTraverse(HFMNode hfm){  
        if(hfm!=null){  
            System.out.print(hfm.value+" ");  
            PreOrderTraverse(hfm.lchild);  
            PreOrderTraverse(hfm.rchild);  
        }  
    }  
    /** 
     * 存储哈弗曼树结点的优先队列 
     */  
    ArrayList<HFMNode> nodeList=new ArrayList<HFMNode>();  
    PriorityQueue<HFMNode> nodeQueue=new PriorityQueue<HFMNode>(11,new MyComparator());  
    /** 
     * 实例化的一个比较器类 
     */  
    class MyComparator implements Comparator<HFMNode>{  
        public int compare(HFMNode o1, HFMNode o2) {  
            return o1.value-o2.value;  
        }     
    }  
}
```





