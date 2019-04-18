# poi中getPhysicalNumberOfRows()和getLastRowNum()区别 - z69183787的专栏 - CSDN博客
2017年01月05日 11:32:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4374
getPhysicalNumberOfRows()获取的是物理行数，也就是不包括那些空行（隔行）的情况。
getLastRowNum()获取的是最后一行的编号（编号从0开始）。
hssfSheet.getLastRowNum();//最后一行**行标**，比行数小1
hssfSheet.getRow(k).getLastCellNum();//获取**列数**，比最后一列列标大1
　　通过getPhysicalNumberOfRows()获得的实际行数，不一定有数据的行数。
　　比如某个单元格只设置了非“常规”的格式，它也会被POI认为是实际行。
 
　　不过POI里似乎没有没有这样的方法，getNextPhysicalRow()。
　　所以只好从getFirstRowNum()到getLastRowNum()遍历，如果null==currentRow，验证下一行。
