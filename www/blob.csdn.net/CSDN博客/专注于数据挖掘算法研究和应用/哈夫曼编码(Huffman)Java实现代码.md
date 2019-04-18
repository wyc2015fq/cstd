# 哈夫曼编码(Huffman)Java实现代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月14日 17:00:50[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：8130
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









网上找到的一个组Huffman编码Java实现代码，比较经典。

1、主类，压缩和解压



```java
package cn.hm;

import java.io.BufferedInputStream;  
import java.io.BufferedOutputStream;  
import java.io.DataInputStream;  
import java.io.File;  
import java.io.FileInputStream;  
import java.io.FileOutputStream;  
import java.io.IOException;  
import java.io.InputStream;  
import java.io.OutputStream;  
  
public class HZIP {  
    /** 
     * 压缩文件的方法，此方法需要传入正确的绝对路径名 
     * @param inFile 需要被压缩的文件 
     * @param outFile 压缩之后的输出文件 
     * @throws IOException IO异常 
     */  
    public static void compress(String inFile,String outFile) throws IOException{  
        String compressFile=null;//创建压缩文件  
        String extension=inFile.substring(inFile.length()-4);//获取源文件的后缀名  
        File file=new File(outFile);  
        //如果文件已经存在  
        if(file.exists()){  
            System.out.println("文件已经存在");  
        }else{  
            //自动补齐后缀名  
            if(!outFile.endsWith(".hfm")){  
                compressFile=outFile+extension+".hfm";    
            }  
            else{  
                compressFile=outFile+extension;  
            }  
            //创建文件输入的缓冲流  
            InputStream in=new BufferedInputStream(new FileInputStream(inFile));  
            //创建文件输出的缓冲流  
            OutputStream out=new BufferedOutputStream(new FileOutputStream(compressFile));  
            int ch;  
            //创建哈弗曼压缩的输入流  
            HZIPOutputStream hzout=new HZIPOutputStream(out);  
            while((ch=in.read())!=-1){  
                hzout.write(ch);  
            }  
            //关闭流  
            in.close();  
            hzout.close();  
        }  
    }  
    /** 
     * 解压文件的方法，此方法需要填入正确的绝对路径名 
     * @param compressedFile  需要被解压的文件 
     * @param outFile 解压之后的输出文件 
     * @throws IOException IO异常 
     */  
    public static void uncompress(String compressedFile,String outFile) throws IOException{  
        String extension;//文件的后缀名  
        extension =compressedFile.substring(compressedFile.length()-4);  
        //得到压缩前的文件的后缀名  
        String suffix=compressedFile.substring(compressedFile.length()-8,compressedFile.length()-4);  
        //如果文件不合法则不执行解压操作  
        if(!extension.equals(".hfm")){  
            System.out.println("文件格式错误或者不是有效的压缩文件");  
            return;  
        }  
        File file=new File(outFile);  
        //如果已经存在同名文件  
        if(file.exists()){  
            System.out.println("该文件已经存在，请重新命名解压后的文件");  
        }  
        else{  
            outFile+=(suffix+".uc");//输出文件的格式统一为uc格式  
            //创建文件输入的缓冲流  
            InputStream fin=new BufferedInputStream(new FileInputStream(compressedFile));  
            //创建数据读入流  
            DataInputStream in=new DataInputStream(fin);  
            //创建哈弗曼压缩输入流  
            HZIPInputStream hzin=new HZIPInputStream(in);  
            //创建文件输出的缓冲流  
            OutputStream fout=new BufferedOutputStream(new FileOutputStream(outFile));  
            int ch;  
            //解压并输出文件  
            while((ch=hzin.read())!=-1){  
                fout.write(ch);  
            }  
            //关闭流  
            hzin.close();  
            fout.close();     
        }  
      
    }  
    public static void main(String args[]) throws IOException{  
        System.out.println("开始压缩");   
        long start=System.currentTimeMillis();  
        compress("d://tmp/ori.txt","d://tmp/des"); 
        System.out.println("压缩结束,用时:"+(System.currentTimeMillis()-start)); 
        System.out.println("开始解压");  
        start=System.currentTimeMillis();  
        uncompress("d://tmp/des.txt.hfm","d://tmp/ori1");  
        System.out.println("解压结束,用时:"+(System.currentTimeMillis()-start));
    }  
}
```


2、字符频度统计类



```java
package cn.hm;

/** 
 * 字符统计类,获取输入流（通常是文件）中所含的字符数 
 * 8位字节认为是ASCII字符 
 */  
import java.io.IOException;  
import java.io.InputStream;  
  
public class CharCounter {  
    //字节的下标表示字节的种类，对应的值表示出现的次数  
    private int theCounts[]=new int[BitUtils.DIFF_BYTES];//字节的种类总共有256种  
    /** 
     * 默认的无参的构造方法 
     */  
    public CharCounter(){  
          
    }  
    /** 
     * 封装了基本的InputStream,读取数据并进行字符的频次统计 
     * @param input InputStream对象 
     * @throws IOException 
     */  
    public CharCounter(InputStream input) throws IOException{  
        int ch;//读到的字节  
        //一直读到文件的末尾，每一种byte出现了多少次  
        while((ch=input.read())!=-1){  
            theCounts[ch]++;  
        }  
    }  
    /** 
     * 获取该字符统计数组的某一个字符出现的次数 
     * @param ch 数组下标 
     * @return 该下标位置字符出现的次数 
     */  
    public int getCount(int ch){  
        return theCounts[ch&0xff];  
    }  
    /** 
     * 设置某一个字符出现的次数 
     * @param ch 数组下标 
     * @param count 字符出现次数 
     */  
    public void setCount(int ch,int count){  
        theCounts[ch&0xff]=count;  
    }  
}
```


3、Huffman树编码类



```java
package cn.hm;

import java.io.DataInputStream;  
import java.io.DataOutputStream;  
import java.io.IOException;  
import java.util.PriorityQueue;  
  
public class HuffmanTree {  
    private CharCounter theCounts;//字符统计类对象  
    private HuffNode root;//根结点  
    private HuffNode[] theNodes=new HuffNode[BitUtils.DIFF_BYTES+1];//存储HuffNode的数组  
    public static final int ERROR=-3;//错误  
    public static final int INCOMPLETE_CODE=-2;//不完全的结点编码  
    public static final int END=BitUtils.DIFF_BYTES;//字节的溢出位  
    /** 
     * 实例化一个字符统计类对象 
     */  
    public HuffmanTree(){  
        theCounts=new CharCounter();  
        root=null;  
    }  
    /** 
     * 可以通过CharCounter对象来创建一个huffmanTree对象 
     * @param cc CharCounter对象 
     */  
    public HuffmanTree(CharCounter cc){  
        theCounts=cc;  
        root=null;  
        createTree();//创建HuffmanTree  
    }  
    /** 
     * 得到要寻找的字符编码所在的树结点,如果该字符不在树上，则返回null表示出错,否则,通过父亲链逆向查找,直到到达根结点 
     * @param ch 当前结点的下标 
     * @return 结点相对的字符编码数组 
     */  
    public int[] getCode(int ch){  
        HuffNode current=theNodes[ch];  
          
        if(current==null)  
            return null;  
        String v="";//结点的编码  
        HuffNode parent=current.parent;  
          
        while(parent!=null){  
            if(parent.left==current)  
                v="0"+v;//左结点编码  
            else   
                v="1"+v;//右结点编码  
            //向下遍历  
            current=current.parent;  
            parent=current.parent;  
        }  
        int len=v.length();  
        int [] result=new int[len];//创建一个与编码相同大小数组  
        for(int i=0;i<len;i++)  
            result[i]=v.charAt(i)=='0'?0:1;  
        return result;  
    }  
    /** 
     * 获取编码对应的字符 
     * @param code 哈弗曼编码 
     * @return 存储在结点中的值(如果结点不是叶子结点,则返回符号INCOMPLETE) 
     */  
    public int getChar(String code){  
        HuffNode leaf=root;//获取根结点  
        int len=code.length();  
        //按照编码向左或向右遍历到叶子结点  
        for(int i=0;leaf!=null&&i<len;i++)  
            if(code.charAt(i)=='0')  
                leaf=leaf.left;  
            else  
                leaf=leaf.right;  
        //根结点为空  
        if(leaf==null)  
            return ERROR;  
        return leaf.value;  
    }  
    /** 
     * 写入编码表的方法 
     * @param out 写入的数据流 
     * @throws IOException 
     */  
    public void writeEncodingTable(DataOutputStream out) throws IOException{  
        for(int i=0;i<BitUtils.DIFF_BYTES;i++){  
            if(theCounts.getCount(i)>0){  
                out.writeByte(i);//将字节写入（通常是文件）  
                out.writeInt(theCounts.getCount(i));//将字节出现的次数写入（通常是文件）  
            }  
        }  
        //最后写入0表示编码的结束  
        out.writeByte(0);  
        out.writeInt(0);  
    }  
    /** 
     * 读取编码表的方法 
     * @param in 数据输入流对象 
     * @throws IOException 
     */  
    public void readEncodingTable(DataInputStream in) throws IOException{  
        for(int i=0;i<BitUtils.DIFF_BYTES;i++)  
            theCounts.setCount(i, 0);  
        byte ch;  
        int num;  
        for(;;){  
            ch=in.readByte();//读到的字节  
            num=in.readInt();//字符出现的次数  
            if(num==0)//如果读到0表示编码表的结束  
                break;  
            theCounts.setCount(ch, num);  
        }  
        createTree();//创建HuffmanTree  
    }  
    /** 
     * 构造哈弗曼编码树的方法 
     */  
    public void createTree(){  
        //创建一个优先队列来保存HuffNode  
        PriorityQueue<HuffNode> pq=new PriorityQueue<HuffNode>();  
          
        for(int i=0;i<BitUtils.DIFF_BYTES;i++){  
            if(theCounts.getCount(i)>0){//如果某一个字节出现过  
                HuffNode newNode=new HuffNode(i,theCounts.getCount(i),null,null,null);  
                theNodes[i]=newNode;  
                pq.add(newNode);//将新结点添加到队列中  
            }  
        }  
          
        theNodes[END] =new HuffNode(END,1,null,null,null);  
        pq.add(theNodes[END]);  
        //当剩余的结点多于一个时  
        while(pq.size()>1){  
            //每次取出当前最小的两个结点  
            HuffNode n1=pq.remove();//remove方法与poll方法的唯一不同之处在于:此队列为空时将抛出一个异常  
            HuffNode n2=pq.remove();  
            //将最小的两个结点链接形成新结点  
            HuffNode result=new HuffNode(INCOMPLETE_CODE,n1.weight+n2.weight,n1,n2,null);  
            n1.parent=n2.parent=result;  
            //将新结点添加到队列当中  
            pq.add(result);  
        }  
        root=pq.element();//根结点就是队列中的最后一个结点  
    }  
}
```

```java
package cn.hm;

/** 
 * 哈弗曼结点类 
 */  
public class HuffNode implements Comparable<HuffNode>{  
    public int value;//结点数据  
    public int weight;//权重  
    HuffNode left,right;//左右孩子结点  
    HuffNode parent;//父亲结点  
    /** 
     * 初始化结点的数据，权重，左右孩子结点与父亲结点 
     * @param v 数据 
     * @param w 权重 
     * @param lchild 左孩子结点 
     * @param rchild 右孩子结点 
     * @param pt    父亲结点 
     */  
    HuffNode(int v,int w,HuffNode lchild,HuffNode rchild,HuffNode pt){  
        value=v;  
        weight=w;  
        left=lchild;  
        right=rchild;  
        parent=pt;  
    }  
    /** 
     * 比较两个结点的权重 
     */  
    public int compareTo(HuffNode rhs) {  
        return weight-rhs.weight;  
    }  
}
```



4、输入输出辅助类



```java
package cn.hm;

/** 
 * 包含压缩的包装类 
 */  
import java.io.ByteArrayInputStream;  
import java.io.ByteArrayOutputStream;  
import java.io.DataOutputStream;  
import java.io.IOException;  
import java.io.OutputStream;  
  
public class HZIPOutputStream extends OutputStream{  
    private ByteArrayOutputStream byteOut=new ByteArrayOutputStream();//实例化的一个字节数组输出流对象  
    private DataOutputStream dout;//数据输出流对象  
    /** 
     * 实例化一个DataOutputStream对象的构造方法 
     * @param out 输出流对象 
     * @throws IOException 
     */  
    public HZIPOutputStream(OutputStream out) throws IOException{  
        dout=new DataOutputStream(out);  
    }  
    /** 
     * 写入编码频率的方法 
     */  
    public void write(int ch) throws IOException{  
        byteOut.write(ch);  
    }  
    /** 
     * 关闭流的方法 
     */  
    public void close() throws IOException{  
        byte[] theInput=byteOut.toByteArray();//将字节数组输出流转换数据转换成字节数组进行输入  
        ByteArrayInputStream byteIn=new ByteArrayInputStream(theInput);//将字节数组封装到字节输入流中  
          
        CharCounter countObj=new CharCounter(byteIn);//实例化字符统计对象并统计字节数组的字符出现的次数  
        byteIn.close();//关闭字节输入流  
          
        HuffmanTree codeTree=new HuffmanTree(countObj);//通过CharCounter对象实例化一个HuffmanTree对象  
        codeTree.writeEncodingTable(dout);//将编码写入数据输出流中  
          
        BitOutputStream bout=new BitOutputStream(dout);//创建位输出流  
          
        //将按编码转换后的位写入  
        int len=theInput.length;  
        for(int i=0;i<len;i++)  
            bout.writeBits(codeTree.getCode(theInput[i]&0xff));  
        bout.writeBits(codeTree.getCode(BitUtils.EOF));//文件结束的标示符  
          
        //关闭流  
        bout.close();  
        byteOut.close();  
    }  
}
```


```java
package cn.hm;
/** 
 * 包含解压缩的包装类 
 */  
import java.io.DataInputStream;  
import java.io.IOException;  
import java.io.InputStream;  
  
public class HZIPInputStream extends InputStream{  
    private BitInputStream bin;//位输入流  
    private HuffmanTree codeTree;//编码的HuffmanTree对象  
    /** 
     * 封装InputStream对象,实例化HuffmanTree对象与BitInputStream对象,并读入哈弗曼编码 
     * @param in 
     * @throws IOException 
     */  
    public HZIPInputStream(InputStream in) throws IOException{  
        //数据输入流  
        DataInputStream din=new DataInputStream(in);  
          
        codeTree=new HuffmanTree();  
        codeTree.readEncodingTable(din);  
          
        bin=new BitInputStream(in);  
    }  
    /** 
     * 读取文件的方法 
     */  
    public int read() throws IOException{  
        String bits="";//哈弗曼编码的字符串  
        int bit;//位  
        int decode;//解码后的字符  
        while(true){  
            bit=bin.readBit();  
            if(bit == -1)  
                throw new IOException("Unexpected EOF");//意外的资源结束  
              
            bits+=bit;  
            decode=codeTree.getChar(bits);//获取编码对应的字符  
            if(decode==HuffmanTree.INCOMPLETE_CODE)//向下搜索到叶子结点  
                continue;  
            else if(decode==HuffmanTree.ERROR)//编码出错  
                throw new IOException("Unexpected error");  
            else if(decode==HuffmanTree.END)//编码溢出  
                return -1;  
            else   
                return decode;  
        }  
    }  
    /** 
     * 关闭输入流 
     */  
    public void close() throws IOException{  
        bin.close();  
    }  
}
```


```java
package cn.hm;

/** 
 * InputStream的包装类,提供按位输入 
 */  
import java.io.IOException;  
import java.io.InputStream;  
  
public class BitInputStream {  
    private InputStream in;//基本输入流  
    private int buffer;//byte缓冲区  
    private int bufferPos;//表示缓冲区中有多少未被使用的空间  
    /** 
     * 封装InputStream的构造方法，初始化byte缓冲区的大小 
     * @param is InputStream对象 
     */  
    public BitInputStream(InputStream is){  
        in=is;  
        bufferPos=BitUtils.BITS_PER_BYTES;//初始化缓冲区的剩余空间  
    }  
    /** 
     * 读取一位的方法，每8次对其进行调用就会从基本输入流中读出一个byte 
     * @return 1位数据，1或者0 
     * @throws IOException  
     */  
    public int readBit() throws IOException{  
        //如果缓冲区还未被使用  
        if(bufferPos==BitUtils.BITS_PER_BYTES){  
            //输入流读取一位  
            buffer=in.read();  
            //读到文件的末尾了  
            if(buffer==-1)  
                return -1;  
            //清空缓冲区  
            bufferPos=0;  
        }  
        //扩张缓冲区  
        return getBit(buffer,bufferPos++);  
    }  
    /** 
     * 关闭输入流 
     * @throws IOException  
     */  
    public void close() throws IOException{  
        in.close();  
    }  
    /** 
     * 获取一个byte中每一位的方法 
     * @param pack  
     * @param pos  
     * @return  
     */  
    private static int getBit(int pack,int pos){  
        //将一个字节进行8次按位与运算，得到这个字节的8位  
        return (pack&(1<<pos))!=0?1:0;  
    }  
}
```


```java
package cn.hm;

/** 
 * OutputStream的包装类，提供按位输出的方法 
 */  
import java.io.IOException;  
import java.io.OutputStream;  
  
public class BitOutputStream {  
    private OutputStream out; //基本输出流  
    private int buffer;//输出的缓冲区  
    private int bufferPos;//缓冲区中剩余的位数  
    /** 
     * 封装OutputStream的构造方法，初始化缓冲区大小 
     * @param os 
     */  
    public BitOutputStream(OutputStream os){  
        bufferPos=0;  
        buffer=0;  
        out=os;  
    }  
    /** 
     * 写入一串的位 
     * @param val 包含有位数据的数组 
     * @throws IOException 
     */  
    public void writeBits(int []val) throws IOException{  
        int len=val.length;  
        for(int i=0;i<len;i++){  
            writeBit(val[i]);  
        }  
    }  
    /** 
     * 写入位的方法(0或1)，每8次对其进行调用就从基本流中写入一个byte 
     * @param val 当前写入的位数据 
     * @throws IOException 
     */  
    public void writeBit(int val) throws IOException{  
        buffer=setBit(buffer,bufferPos++,val);//将缓冲数据转换成位数据  
        //每读到一个byte就刷新一次  
        if(bufferPos==BitUtils.BITS_PER_BYTES)//缓冲区已满则刷新缓冲区  
            flush();  
    }  
    /** 
     * 刷新此缓冲的输出流 
     * @throws IOException 
     */  
    public void flush() throws IOException{  
        if(bufferPos==0)//如果缓冲中没有数据则不执行  
            return;  
        //将缓冲区中的数据写入  
        out.write(buffer);  
        //重置缓冲区  
        bufferPos=0;  
        buffer=0;  
    }  
    /** 
     * 关闭流的方法 
     * @throws IOException 
     */  
    public void close() throws IOException{  
        flush();  
        out.close();  
    }  
    /** 
     * 进行位数据转换的方法 
     * @param pack 
     * @param pos 
     * @param val 当前位 
     * @return 
     */  
    private int setBit(int pack,int pos,int val){  
        if(val==1)  
            //按位或运算  
            pack|=(val<<pos);  
        return pack;  
    }  
}
```


```java
package cn.hm;

public interface BitUtils {
	public static final int BITS_PER_BYTES=8;//位与byte之间的转换单位  
    public static final int DIFF_BYTES=256;//0x100  
    public static final int EOF=256;//EndOfFile 资料源无更多的资料可读取
}
```








