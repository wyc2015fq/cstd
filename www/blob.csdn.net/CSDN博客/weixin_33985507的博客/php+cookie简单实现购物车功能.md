# php+cookie简单实现购物车功能 - weixin_33985507的博客 - CSDN博客
2016年05月05日 16:33:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
部分代码未完成  只是提供一个思路
```
<?php
header("Content-Type: text/html; charset=UTF-8");
class Cart extends MY_Controller {
function __construct() {
    parent::__construct();
    $this->load->model('mobile/goods_class_model', 'class');
    $this->load->model('mobile/goods_model', 'goods');
    $this->load->model('mobile/cart_model', 'cart');
}
/*
 * 首页
 *
 */
function index() {
    $biao = 0;
    $uid = !empty($_SESSION['uid']) ? $_SESSION['uid'] : 1;
    if ($uid > 0) {
        $cartgoods = $this->cart->getcart("user_id=" . $uid);
        $sku_ids = array_column($cartgoods, 'goods_id');
        $where = "g.id in (" . implode(",", $sku_ids) . ")";
        $goods = $this->goods->getgoodslist($where);
        foreach ($goods as $k => $v) {
            foreach ($cartgoods as $key => $val) {
                if ($v['id'] == $val['goods_id']) {
                    $goods[$k]['num'] = $val['goods_number'];
                }
            }
        }
    } else {
        $date = json_decode($_COOKIE['cart'], TRUE);
        $sku_ids = array_keys($date);
        $where = "g.id in (" . implode(",", $sku_ids) . ")";
        $goods = $this->goods->getgoodslist($where);
        foreach ($goods as $k => $v) {
            foreach ($date as $key => $val) {
                if ($v['id'] == $key) {
                    $num = explode(",", $val);
                    $goods[$k]['num'] = $num['1'];
                }
            }
        }
    }
    $date['goods'] = $goods;
    $this->mobile_show_view('mobile/cart/index', $date);
}
/*
 * 加入购物车
 * $id  商品id
 * $pp  是否一步购物 1.加入购物车  2.立即购买
 * $num 商品数量    目前暂设定为1
 * $uid 用户id
 */
function addcart() {
    $id = $_REQUEST['id'];
    $pp = $_REQUEST['pp'];
    $num = 1;
    $uid = !empty($_SESSION['uid']) ? $_SESSION['uid'] : 1;
    //如果$pp==1  立即购买则执行
    if ($pp == 1) {
        
    } else {
        //如果$pp==2  加入购物车则执行
        //如果用户登录则写入到数据库内 否则写入到cookie
        if ($uid > 0) {
            $date['user_id'] = $uid;
            $date['session_id'] = $_COOKIE['PHPSESSID'];
            $date['goods_id'] = $id;
            $date['goods_number'] = $num;
            $date['goods_type'] = $num;
            $date['rec_type'] = $num;
            $biao = $this->cart->getcartgoods("goods_id=" . $id . " and user_id=" . $uid);
            if (empty($biao)) {
                $add = $this->cart->addcart($date);
            } else {
                $date['goods_number'] = $biao['goods_number'] + 1;
                $add = $this->cart->updatecart($date, $biao['id']);
            }
        } else {
            //赋值数组
            $date = json_decode($_COOKIE['cart'], TRUE);
            $sku_ids = array_keys($date);
            if (count($date) > 0 && in_array($id, $sku_ids)) {
                $aa = explode(',', $date[$id]);
                $aa['1'] += 1;
                $date[$id] = implode(',', $aa);
                setcookie('cart', json_encode($date), time() + 3600 * 24, '/');
                echo "1";
            } else {
                $date[$id] = $id . ',' . $num;
                //设置24小时后过期
                setcookie('cart_nums', count($date), time() + 3600 * 24, '/');
                setcookie('cart', json_encode($date), time() + 3600 * 24, '/');
                echo "1";
            }
        }
    }
}
/*
 * 删除购物车商品
 * $id  购物车id
 *
 */
function delcart() {
    $id = $_GET['id'];
    $uid = !empty($_SESSION['uid']) ? $_SESSION['uid'] : 1;
    if ($uid > 0) {
        $this->cart->delcart($id, $uid);
    } else {
        $date = json_decode($_COOKIE['cart'], TRUE);
        unset($date[$id]);
        setcookie('cart_nums', count($date), time() + 3600 * 24, '/');
        setcookie('cart', json_encode($date), time() + 3600 * 24, '/');
        echo "1";
    }
}
/*
 * 购物车商品数量加减
 * $id  购物车id
 *
 */
function cartnum() {
    $id = $_GET['id'];
    $uid = !empty($_SESSION['uid']) ? $_SESSION['uid'] : 1;
    if ($uid > 0) {
        $this->cart->delcart($id, $uid);
    } else {
        $date = json_decode($_COOKIE['cart'], TRUE);
        unset($date[$id]);
        setcookie('cart_nums', count($date), time() + 3600 * 24, '/');
        setcookie('cart', json_encode($date), time() + 3600 * 24, '/');
        echo "1";
    }
}
}
```
