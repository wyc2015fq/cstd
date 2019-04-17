# 【剑指offer】题48：不能被继承的类 - xiaxzhou的博客 - CSDN博客





2017年07月06日 14:24:07[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：120








```
class sealedclass
{
public:
    static sealedclass* getinstance()
    {
        return new sealedclass();
    }
    static void Delet(sealedclass* instance)
    {
        delete instance;
    }
private:
    sealedclass(){};
    ~sealedclass(){};
};
```

```
template<typename T>
class sealed
{
    friend T;
private:
    sealed(){};
    ~sealed(){};
};

class sealedclass :public sealed<sealedclass>
{
public:
    sealedclass(){};
    ~sealedclass(){};

};
```



