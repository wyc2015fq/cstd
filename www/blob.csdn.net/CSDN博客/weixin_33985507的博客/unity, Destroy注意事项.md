# unity, Destroy注意事项 - weixin_33985507的博客 - CSDN博客
2015年07月05日 12:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
Destroy不是立即发生作用，而是推迟到帧末，所以下面代码是错误的：
void OnTriggerEnter(Collider other){
   if (other.gameObject.tag == "coin") {
　　　　m_score++;
　　　　Destroy(other.gameObject);
　　}
}
会导致吃一个金币score加好几次的问题。因为OnTriggerEnter一帧之内可能会触发好几次。
正确的写法是：
void OnTriggerEnter(Collider other){
   if (other.gameObject.tag == "coin") {
　　　　m_score++;
other.gameObject.SetActive(false);
　　　　Destroy(other.gameObject);
　　}
}
这样，虽然Destroy销毁不及时，但是SetActive确保这个coin在此后不会再触发OnTriggerEnter了。
另外注意把Destroy改成DestroyImmediate是不行的，原因不知.
