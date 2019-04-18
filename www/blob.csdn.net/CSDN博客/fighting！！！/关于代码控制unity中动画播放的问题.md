# 关于代码控制unity中动画播放的问题 - fighting！！！ - CSDN博客
2017年06月21日 11:33:18[dujiahei](https://me.csdn.net/dujiahei)阅读数：8369标签：[unity																[动画																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=动画&t=blog)](https://so.csdn.net/so/search/s.do?q=unity&t=blog)
个人分类：[unity学习](https://blog.csdn.net/dujiahei/article/category/6975775)
unity 圣典：http://www.ceeger.com/Manual/
错误尝试：
using UnityEngine;
using System.Collections;
public class hands_2sPlay : MonoBehaviour {
    // Use this for initialization
      public  GameObject hands ;
     public  float m_timer = 0;
    void Start () {
         GameObject hands = GameObject.Find ("4_2sLeft");
        //hands.transform.GetComponent<Animation>().wrapMode = WrapMode.Once;
        //hands.transform.GetComponent<Animation>().Play ("hands");        
    }
    // Update is called once per frame
    void Update()
    {
        m_timer += Time.time;
        if (m_timer >= 10)
        {
            hands.transform.GetComponent<Animation>().wrapMode = WrapMode.Once;
            hands.transform.GetComponent<Animation>().Play("2s_15");
            m_timer = 0;
        }
    }
    // IEnumerator ShowA()
    //{
    //    yield return new WaitForSeconds(10);
    //    hand();
    //}
    void hand()
    {
        hands.transform.GetComponent<Animation>().wrapMode = WrapMode.Once;
        hands.transform.GetComponent<Animation>().Play("2s_15");
        Debug.Log(111111111);
    }
}
成功：
using UnityEngine;
using System.Collections;
public class hands_2sPlay : MonoBehaviour {
    // Use this for initialization
    public Animation hands;
    public  float m_timer = 0;
    void Start () {
        Invoke("hand", 5f);
    }
    // Update is called once per frame
    void Update()
    {
    }
    void hand()
    {
        hands.CrossFade("2s_15");
        Debug.Log("11111111");
     }
}
方法1：选中Hierarchy 列表中的模型，在其Inspector列表中的Animation组件中，取消“Play Automatically”的勾选。
 Invoke("hand", 5f);
 void hand()
    {
        hands.CrossFade("2s_15");
        Debug.Log("11111111");
     }
配合使用。
方法2：选中Hierarchy 列表中的模型，在其Inspector列表中的Animation组件中，Animation size为2，添加“动作”、“静止”两个Element。
 void Start () {
        hands.wrapMode = WrapMode.Once;
        hands.CrossFade("static");
        Invoke("hand", 5f);
    }
 void hand()
    {
        hands.CrossFade("2s_15");        
    }
配合使用。
