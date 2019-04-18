# tp5 foreach分页类 - weixin_33985507的博客 - CSDN博客
2018年04月05日 12:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
```
<?php
    class Page {
        public $page;   //当前页
        public $total; //总记录数
        public $listRows; //每页显示记录数
        private $uri;//动态url
        public $pageNum; //总页数
        private $listNum=6;//显示页码按钮数量
        public $render;//分页后的html模板
        public $data;//分页后渲染到模板的数据
        /*
         * 初始化分页数据
         *$sdata 待分页的数据
         * $listRows 每页记录数
         */
        public function __construct($sdata, $listRows=15){
            $this->total=count($sdata);
            $this->listRows=$listRows;
            $this->uri=$this->getUri();
            $this->page=!empty($_GET["page"]) ? $_GET["page"] : 1;
            $this->pageNum=ceil($this->total/$this->listRows);
            $this->render=$this->pageHtml();
            $this->data=array_slice($sdata,($this->page-1)*$this->listRows,$listRows);
            return $this->data;
        }
        //动态获取url
        private function getUri(){
            $url=$_SERVER["REQUEST_URI"].(strpos($_SERVER["REQUEST_URI"], '?')?'':"?");
            $parse=parse_url($url);
            if(isset($parse["query"])){
                parse_str($parse['query'],$params);
                unset($params["page"]);
                $url=$parse['path'].'?'.http_build_query($params);
            }
            return $url;
        }
        //首页
        private function first(){
            $html = "";
            if($this->page==1)
                $html.=" <a style='magin=10px;' class='current btn disabled'>首 页</a>";
            else
                $html.=" <a class='btn btn-primary-outline' href='{$this->uri}&page=1'>首 页</a>";
            return $html;
        }
        //上一页
        private function prev(){
            $html = "";
            if($this->page==1)
                $html.=" <a class='current btn disabled'>上一页</a>";
            else
                $html.=" <a class='btn btn-primary-outline' href='{$this->uri}&page=".($this->page-1)."'>上一页</a>";
            return $html;
        }
        //页码按钮
        private function pageList(){
            $linkPage="";
            $inum=floor($this->listNum/2);
            for($i=$this->page-$inum;$i<=$this->page+$inum;$i++){
                if($i<=0){
                    continue;
                }
                if($i>$this->pageNum){
                    continue;
                }
                if($i == $this->page){
                    $linkPage.=" <a class='current btn btn-secondary'>{$i}</a>";
                }else{
                    $linkPage.=" <a class='btn btn-primary-outline' href='{$this->uri}&page={$i}'>{$i}</a>";
                }
            }
            return $linkPage;
        }
        //下一页
        private function next(){
            $html = "";
            if($this->page==$this->pageNum)
                $html.=" <a class='current btn disabled'>下一页</a>";
            else
                $html.=" <a class='btn btn-primary-outline' href='{$this->uri}&page=".($this->page+1)."'>下一页</a>";
            return $html;
        }
        //尾页
        private function last(){
            $html = "";
            if($this->page==$this->pageNum)
                $html.=" <a class='current btn disabled'>尾 页</a>";
            else
                $html.=" <a class='btn btn-primary-outline' href='{$this->uri}&page=".($this->pageNum)."'>尾 页</a>";
            return $html;
        }
        //输入指定页码
        private function goPage(){
            return '  <input class="input-text" type="text" onkeydown="javascript:if(event.keyCode==13){var page=(this.value>'.$this->pageNum.')?'.$this->pageNum.':this.value;location=\''.$this->uri.'&page=\'+page+\'\'}" value="'.$this->page.'" style="width:52px"><input class="btn btn-secondary" type="button" value="GO" onclick="javascript:var page=(this.previousSibling.value>'.$this->pageNum.')?'.$this->pageNum.':this.previousSibling.value;location=\''.$this->uri.'&page=\'+page+\'\'">  ';
        }
        //选择指定页码
        function selectPage(){
            $inum=10;
            $location = $this->uri.'&page=';
            $selectPage ="<span class='va-m'>到第 </span> <span class='select-box' style='width:initial'><select class='select' name='topage' size='1' onchange='window.location=\"$location\"+this.value'>";
            for($i=$this->page-$inum;$i<=$this->page+$inum;$i++){
                if($i<=0){
                    continue;
                }
                if($i>$this->pageNum){
                    continue;
                }
                if($i == $this->page){
                    $selectPage .="<option value='$i' selected>$i</option>";
                }else{
                    $selectPage .="<option value='$i'>$i</option>";
                }
            }
            $selectPage .="</select></span> <span class='va-m'>页</span>";
            return $selectPage;
        }
        //组装分页的html模板
        function pageHtml(){
            $html  = "<div class='cl mt-20 text-c'>";
            // $html .= "<span class='pr-20 va-m'>共有<b>{$this->total}</b>条记录</span>";
            // $html .= "<span class='pr-20 va-m'>每页显示<b>{$this->listRows}</b>条</span>";
            // $html .= "<span class='pr-20 va-m'><b>当前{$this->page}/{$this->pageNum}</b>页</span>";
            $html .= $this->first();
            $html .= $this->prev();
            $html .= $this->pageList();
            $html .= $this->next();
            $html .= $this->last();
            $html .= $this->goPage();
            $html .= $this->selectPage();
            $html .= '</div>';
            return $html;
        }
    }
```
使用方法：在控制器中调用这个扩展类，new一个分页对象$p，并渲染到模板
```
//$data：array,通过select()查询未分页的数据，不能是已经分页的对象
        $data = db('table_name')->select();
        //$data：未分页的数据  2：每页显示的记录数
        $p = new \Page($data,2);
        //把分页后的对象$p渲染到模板
        $this->assign([
            'p' => $p,
        ]);
        return $this->fetch();
使用方法：最后在模板中使用这个分页后的对象$p，通过$p->属性，获取对应的数据
//$p->data; //当前页数据
//$p->render; 分页html模板
//$p->page; //当前第几页
//$p->total; //总记录数
//$p->listRows; //每页显示记录数
//$p->pageNum; //总页数
//根据需要组装数据输出显示，例如：
//遍历数据
{volist name='$p->data' id='vo'}
//code...
{/volist}
//分页html模板输出
//默认已经载入h-ui框架css样式，否则需要重写css样式
{$p->render}
```
注意：支持URL模式，模块/控制器/操作?参数名=参数值&...
