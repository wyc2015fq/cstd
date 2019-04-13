
# PHPExcel读取excel文件示例 - anlcy - 博客园






# [PHPExcel读取excel文件示例](https://www.cnblogs.com/camilla/p/7650228.html)
PHPExcel读取excel文件示例
PHPExcel最新版官方下载网址:[http://phpexcel.codeplex.com/](http://phpexcel.codeplex.com/)
PHPExcel是一个非常方便生成Excel格式文件的类,官方下载包中带有大量如何生成各种样式excel文件的示例,但没有一个读取Excel文件的完整例子,下面是读取的例子：
读取Excel的内容主要有两个选择：PHPExcelReader、PHPExcel。
PHPExcelReader比较轻量级，仅支持Excel的读取，实际上就是一个Reader。但是可惜的是不能够支持Excel 2007的格式（.xlsx）。
PHPExcel比较强大，能够将内存中的数据输出成Excel文件，同时还能够对Excel做各种操作，下面主要介绍下如何使用PHPExcel进行Excel 2007格式（.xlsx）文件的读取。
下载PHPExcel后保存到自己的类文件目录中，然后使用以下代码可以打开Excel 2007（xlsx）格式的文件：
require_once '/libs/PHPExcel-1.8.0/Classes/PHPExcel.php';     //修改为自己的目录
echo '<p>TEST PHPExcel 1.8.0: read xlsx file</p>';
$objReader = PHPExcel_IOFactory::createReaderForFile($filename);
$objPHPExcel = $objReader->load($filename);
$objPHPExcel->setActiveSheetIndex(1);
$date = $objPHPExcel->getActiveSheet()->getCell('A16')->getValue();
输出$date变量就能够看到文件中的内容了。PHPExcel使用PHPExcel_IOFactory这个类来自动匹配所上传的文件类型，当然我们也可以自己制定要解析的文件类型。
之后通过load方法，将PHP文件加载到objPHPExcel对象中。如果Excel文件有多个Sheet，可以通过setActiveSheetIndex来设置当前活动的Sheet。
需要注意的是，对于Excel中的日期格式，PHPExcel读出来的是不是日期类型，需要我们使用以下方法来进行日期类型转换。
echo date("Y-m-d H:i:s",PHPExcel_Shared_Date::ExcelToPHP($date));
下面的代码显示了如何遍历显示Excel的内容：
<table>
<?php
$objWorksheet = $objPHPExcel->getActiveSheet();
$i = 0;
foreach($objWorksheet->getRowIterator() as $row){
?>
<tr>
<?php
$cellIterator = $row->getCellIterator();
$cellIterator->setIterateOnlyExistingCells(false);
if( $i == 0 ){
echo '<thead>';
}
foreach($cellIterator as $cell){
echo '<td>' . $cell->getValue() . '</td>';
}
if( $i == 0 ){
echo '</thead>';
}
$i++;
?>
</tr>
<?php
}
?>
</table>
=================================================
传统方法:
<?php
require_once '../Classes/PHPExcel/IOFactory.php';
// Check prerequisites
if (!file_exists("test.xls")) {
exit("not found test.xls.\n");
}
$reader = PHPExcel_IOFactory::createReader('Excel5'); //设置以Excel5格式(Excel97-2003工作簿)
$PHPExcel = $reader->load("test.xls"); // 载入excel文件
$sheet = $PHPExcel->getSheet(0); // 读取第一個工作表
$highestRow = $sheet->getHighestRow(); // 取得总行数
$highestColumm = $sheet->getHighestColumn(); // 取得总列数
$highestColumm= PHPExcel_Cell::columnIndexFromString($colsNum); //字母列转换为数字列 如:AA变为27
/** 循环读取每个单元格的数据 */
for ($row = 1; $row <= $highestRow; $row++){//行数是以第1行开始
for ($column = 0; $column < $highestColumm; $column++) {//列数是以第0列开始
$columnName = PHPExcel_Cell::stringFromColumnIndex($column);
echo $columnName.$row.":".$sheet->getCellByColumnAndRow($column, $row)->getValue()."<br />";
}
}
?>
精简方法:
<?php
require_once '../Classes/PHPExcel/IOFactory.php';
// Check prerequisites
if (!file_exists("test.xls")) {
exit("not found test.xls.\n");
}
$reader = PHPExcel_IOFactory::createReader('Excel5'); //设置以Excel5格式(Excel97-2003工作簿)
$PHPExcel = $reader->load("test.xls"); // 载入excel文件
$sheet = $PHPExcel->getSheet(0); // 读取第一個工作表
$highestRow = $sheet->getHighestRow(); // 取得总行数
$highestColumm = $sheet->getHighestColumn(); // 取得总列数
/** 循环读取每个单元格的数据 */
for ($row = 1; $row <= $highestRow; $row++){//行数是以第1行开始
for ($column = 'A'; $column <= $highestColumm; $column++) {//列数是以A列开始
$dataset[] = $sheet->getCell($column.$row)->getValue();
echo $column.$row.":".$sheet->getCell($column.$row)->getValue()."<br />";
}
}
?>





