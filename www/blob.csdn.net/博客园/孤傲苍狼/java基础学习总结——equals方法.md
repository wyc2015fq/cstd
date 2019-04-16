# java基础学习总结——equals方法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [java基础学习总结——equals方法](https://www.cnblogs.com/xdp-gacl/p/3637073.html)



## 一、equals方法介绍

![](https://images0.cnblogs.com/i/289233/201403/312233226727212.png)

### 1.1.通过下面的例子掌握equals的用法

```
1 package cn.galc.test;
 2 
 3 public class TestEquals {
 4     public static void main(String[] args) {
 5         /**
 6          * 这里使用构造方法Cat()在堆内存里面new出了两只猫，
 7          * 这两只猫的color，weight，height都是一样的，
 8          * 但c1和c2却永远不会相等，这是因为c1和c2分别为堆内存里面两只猫的引用对象，
 9          * 里面装着可以找到这两只猫的地址，但由于两只猫在堆内存里面存储在两个不同的空间里面，
10          * 所以c1和c2分别装着不同的地址，因此c1和c2永远不会相等。
11          */
12         Cat c1 = new Cat(1, 1, 1);
13         Cat c2 = new Cat(1, 1, 1);
14         System.out.println("c1==c2的结果是："+(c1==c2));//false
15         System.out.println("c1.equals(c2)的结果是："+c1.equals(c2));//false
16     }
17 }
18 
19 class Cat {
20     int color, weight, height;
21 
22     public Cat(int color, int weight, int height) {
23         this.color = color;
24         this.weight = weight;
25         this.height = height;
26     }
27 }
```

### **1.2.画出内存分析图分析c1和c2比较的结果**

程序：

Cat c1 = new Cat(1,1,1);

Cat c2 = new Cat(1,1,1);

执行完之后内存之中的布局如下图所示，

**![](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAasAAAELCAIAAADobz7UAAAgAElEQVR4nO3dfXQV5Z0H8OecfeubIIoQCkqFrqJW067o1stqFFslsjXiRhfLiadiOpJSOWFXRI3XLWiJuFFGREJCelhfo61SzpLOcQ3pInRFIaWxtVK8HHvC6cFGWuVIVcDA7B+T+9xnXu7M3DvPzPxm7vd3vn+QuXPnznNNPj4zz8wzTEehUKhKLRb3DqBQKFRsBQFRKFTlFgREoVCVWwUBc3/WGYpSxfhrYak/5+T8btTOm+e5zjwf66BQwcv43S78mf32PZ0x1tGvI7Fn26DOKAn43m91xlh/hx4023XGWL+uu2RQp9V2WfWe7t32/rckfc+Iawa36RCQdCBg3C2WXxCQTiAg9UDAuFssvyAgnUBA6oGAcbdYfkFAOoGA1AMB426x/IKAdAIBqQcCxt1i+QUB6QQCUg8EjLvF8gsC0gkEpB4IGHeL5RcEpBMISD0QMO4Wyy8ISCcQkHogoMSd1xTGGMuoufI3kVMz/E4CRStvGxCQTiAg9UBAWXtu8BdUQF3XOYMQMPmBgNQDASXuvIQ+oK7rEDBFgYDUAwEl7rwhFwREeCAg9UBAz13ih7fiyTnxfB1HzyKg4zrmrTFF40sK4EHA1AQCUg8EdN+fEZ4yai7vUkbNmcwSfhAFdFyntpZZCgKmOxCQeiCg294U0LMvLADFlezlAhZfp8vHuUIImJpAQOqBgG57M+KWmSKhW2gU566LC+hnneIFAVMTCEg9ENBtb0LoA/ZCwEoKBKQeCOi6O3wwo3A6z3rmTvhB7DK6ryMKaDsPaP7QMr5DCEgmEJB6IKDXHplHdPMo2cZ5TQsM4Pyso1sENL+nPAUhIJ1AQOqBgHG3WH5BQDqBgNQDAeNusfyCgHQCAakHAsbdYvkFAekEAlIPBIy7xfILAtIJBKQeCBh3i+UXBKQTCEg9EDDuFssvCEgnEJB6IGDcLZZfEJBOICD1QMC4Wyy/ICCdQEDqgYBxt1h+QUA6gYDUAwHjbrH8goB0AgGpp5IF/I2us/QWBKQQCEg9lSxgRQcCRhIISD0QsEIDASMJBKQeCFihgYCRBAJSDwSs0EDASAIBqQcCVmggYCSBgNQDASs0EDCSQEDqgYAVGggYSSAg9UDACg0EjCQQkHogYIUGAkYSCEg9lSzgr3X97TTmTQhIJhCQeipZwEHcFRc3EKkPBKQeCBh3i+UXZkagEwhIPRAw7hbLLwhIJxCQeiBg3C2WXxCQTiAg9UDAuFssvyAgnUBA6oGAcbdYfkFAOoGA1AMB426x/IKAdAIBqQcCxt1i+QUB6QQCUg8EjLvF8gsC0gkEpB4IGHeL5RcEpBMISD0QMKqW5dQMYyyj5kL/JAhIJyQEXLphR31zW2vPYKlvLOMtiQsElLfvmsIYY0zR7C8Z/PkVkK/tvDGvgoB0QkLA1p5B47epVAfrm9tq6puy3QOxOwUBS05S+oCapmmaE3OGpxAw2aEloFELV232L6DxlhQ7CAGjapmjgDk1k8lkHJmDgGkIOQEbV3T7fyMXkDFWXVNn9B/beocWt/e19Q7FjhcEdEtwAYVjUadSNHGFjJozGTdyRCzUXMWyuYzaJa6UaWnJMAE9CJiGkBOwpDeKfUBxeUNLp7GwcUV30imEgC47M0JcRs3lsVM0g6aM2lvgznjxzDMLFhp6Ff6laAVQTa+LzImfoUPAdCQlAtY3t1lealzRzbdZU9+0fOPe2C2DgKbIOQoeYUjlvT1Fyxll7yAqXRYUzRaKPUT+bxfmIGAakmwBjb4eK3LqUESQMba4vS92ziBgIZLOA+aPZjMZVVFG+nCqmu8Ums/qCctMB8FiFw8CVlaSLeDi9j533UQEq2vqYucMAhYiayQkb5mi5ftzGVXTxSNkXdf1nKZpWpdxIk/RcqqiKIpl1NcuIFO0PHO5XC7XazoPaN5+ad8hBCSTlAvYIfQTo2wRBPSOtLFgYbhWU0wmmUc78ucBmW2MgzkMg4zUucJYSmEkxHKIXWo/EALSSUoEtJ8HFFNdU8fQB5RU9AQMWtHdC5IvCEgnFSHg0g073PuJlAMBQ2iElj9jmAt0NFtuQUA6qQgBO5J8/xwEDKERtoHicsYzyi8ISCekBWxc0T27MevyxuUb9/oUkG/Q55p0AgHjbrH8goB0QldAPozrgqB4Q7Hnp2S7B4yV+d0jiQgEjLvF8gsC0kmkAja0dNY3t9kzuzHLBbQvcUHQj4Dq1kOtPYPGqUCxknJaEALG3WL5BQHpJFIB23qHqmvq6pvbFq7avLi9j0e8ak9cLsbxpg7RNb4pw9Ca+iZmK+OlhpZOY+VE9AQhYNwtll8QkE5IHAXz41OXD124ajPvM9ppY4zV1DcZr3Jel2/c29ozmAjmICAEhICxhISAYh+w2DqtPYPVNXU19U0W3fgbky4dBISAEDD6kBBQPOvnspq69ZB9Ie8SQsBoCgIGLwhIJ/ELqG49JB7Mlvp2LmByZ3+pTAF3bTvhqcDvdZ0x9qmupyx/hIBkEr+A4knAMj6U3/ablLHdChdwZ/vw6tu1b1x4Q8udnd4K6LrjOd90FASkkPgFtMxh1bii2/Fot1j8TI4gpqGlM1lzpqZGwI3L935n1l2nnfxF47/Xpuf2eStQyYGAkSR+AY1pC3gZ5wT9O8gFbGjp9FyZn3BMUIcx6QJue/TD7M1d1VNniP+Vzxh/lp/zgBUdCBhJYhaQ39bGS916iJvoxyk/N8YZF0VbrrKub24rqbMJAY0qVcCVt/3kr//qbyz/lb99ZTMEhIAUErOAlkNg40NFFv1MeWCsaUx+1dozuHzj3sXtfcb1gxb1WP6yQeOi6EQ8Xi7pAvZ36I8teukzf/s58b/C480vQ0AISCExC2g5BOYfKk5r6vj0uLbeIeNeN34UbCnj6mjx4sFE9PhSKWB/h/69ugf4f5rP/t3ndzx+FAJCQAqJU0De13O8IlrEsUO4J8Qi3ezGLF9z6YYdLqMcCb1cJgUCPnnPrlNOGnf3vHZjGOSy6mv7O3xdD1jRgYCRJE4BjY5edU2deNjLX+WdO+Pw1rhoxrgnZOmGHWKfjrO4dMOOYp9lXHWYuKmxUiDgC8v2jB09Qf1+T3+H/t8/fOf0cV++Z966/g4ICAFJJDYB+YXQxgwFdgE7+nXLUXCxw1hupeMT4yxKuk84SDCJFvBnD+6fcOrk5fOf4kte/s8/vtw21N8BASEgicQmoMEWH75wFHDhqs3VNXWe5+88h4MtF10nC8HkCrjl4YNfqpp2x78+6rwCBISABBKbgMbJO2M01mWWfJ/DF8XebmxBPKXYuKI7WScEEyrgtkc/PHfy9MbZ2aLr+BDwbdwTEjcQqU88AvKTesaPZT8nhIfPBmifH0G8IMblMJlskijgq48fuWjazBsu/57bapgZAQISSDwCGmBxrYILyEeTLRdRi6PMjlfV0E/iBNzZPnzF1+ZcddHcneuOu/3yQUAISCAxCGh0AMWb2IILyM/0ief4Fq7anHT+kihg3YxbM+fNem3tMY9fPggIAQkkBgFr6pssDy8PLqA4xZZx6lAc/UjW0EeiBbz5qiUXTLnkF4995P3LBwEhIIFELaAxBGwZiwguYId51oPU8JcsARddv/LLE8//+ar3ff3yQUAISCBRC1hdU2c/IJUioHgFdQoOfhMnYPbmroljp7z00AG/v3wQEAISSKQCFnsCuhQBLXNNe/KXlNuEEyHgQwteHHfyxE0P7Cvhlw8CQkACiU5A41FHju5IEbBDuCaG+ZhZq6GlMxEXBtIXsH1x3ymjxj+bHSjtlw8CQkACiU5Al2kLpAgoTifD8tdau3OZiIcrERfQmPWga8n2kn/55AmoKYwxllFzstoofYOWgoB0Ev8c0VIEtM8DWF1T5zJPjHHIDAHLKFHAF5btOXVUlTHrQcmRJKChVSlg5dSM2/qlb5BvkjHGmKJ5rg4B6STxArb1DnH+Zjdm23qH+EiIC4LGrcQQsIziAtpnPSgtqesDjjAIAROVZAu4fONe7h2/402ca6sYgsbF0rHrllwBPWY98BN5Ahr0SBSw6AY1xeVzIGASk2AB+S0fxgyD4ktLN+wQEbT39YzJsmLXLbkCesx64CfJE9DjcyBgEpNIAcUj32KPiLM8gcQyNGwsxFFwGfWHXx1hjHnMeuAnsgUUiyvFT+qJNokHuaYV8m/uddqgZc3aWvNGIWAykzwBedfPczDXgmB9c5txRNzWOwQBy6vh4eFrvjGHMeYx64GfhCCgonHSFM084mH8oGimgQ5hDRNfrhsc0TW/HQg4kp7Wwa3qodh3o9QkScBs94BxxV9NfZPPx7xZ5kZljDWu6OZ3j+B6wFJr/vz5M/9plpy/zLCOgg2aFM3eM8zzxPuA4huL/buwwZQeBTfOznaXei1nh27Hrn1xH2NsRWN3shxMhoCtPYPGmTv/9onvFa+UFisRz02nI+CSJUsuueSSwV9+lAwBC303l/U1JZPJFOkDVoiAxt9CTXVd++I+/3jV1zQ117dZ1m9p6OSbip22lAjI7Wto6Qxy0Or4UM1EPDiJiIArV648//zz33///fKelhmDgPk+oF2kwvrmc3vctgoU0KgVjd1+3rJVLdyBasGuvmaktxF0oKyiBHR8VlxHv97Q0jm7MevyBLiSom49ZH9AO/27gykIuH79+ilTphw4cEAv93nBoQpounyvcG7PdDZP13VN0zTNdBSsKYqiOIjlvkFF0zVNW2YdCbF+nEuRErCmuq4k/vo79O6s6eRSS0Mnf6mntdCbSQSCJAQ0Omj1zW2lHuGWl6UbdvChZPrz5scu4Isvvjhx4sR9+/YZPxIT0PFsn1EigozjJHb7Mi0t1ncrXS4bVOYWfjCNBdv2wl1BUgI21488RrGnddDnW4yj3WLMia+uWrg5duMSIGBb71AsfTGMBXtWX1/f+PHjBwYG+BJiAgapvFvmzp6Po9iglXQBRet7jWefChGPkZvr22I3LgECIi6JUcBdu3aNGzdu+/bt4sIUCaipqqoWzuqFPR9CoRItoHgIXGzEg2/T/5E1BEScE5eAe/bsqaqq6unpsSxPkYAOh9ARdAD1hAvI16+vaSq2DleS/pUxEJB6YhFw//79kydPfvrpp+0vpUnAuCq5AopHuBuX73VZc9XCzfYVetuGaqrryrj8EAJWbqIX8ODBg9OmTVu9erXjqxAweCVXQOOy5/JO8Bn8GW+ngyAEpJ6IBfzwww+nT5+ezWaLrQABg1dyBeSX+7U0dLYv7ispnD9SCEJA6olSwCNHjsycOXPhwoUu60DA4JVQAfnZvZrquub6Nsd4riDGPo4MARFrIhNweHh4zpw5N91004kTJ1xWg4DBK6EC8jU3LN1RbJ2yj5HjCgSknsgEnD9//qxZs44dO+a+GgQMXvQFXNHYbbmQRbzZw8VKCIhITjQCLlmyJJPJfPTRR55rQsDgRVzAFY0jN4+KCPLVICASaSIQkM964GdlaQL+VGeM9f9Kd8mevTpj7N0hPWXZ94532/v/R6aAvW1DxU7G8QGKxtnZ5vq2xtmmh44ZCIodQAiIRJqwBRRnPfBT0gQ031yFciyJf+rd2YGa6jr7GC4f3l3R2O04httv7gAyCIhEmVAFtMx64KckCohQiOdIiGUmGAiIRJrwBNyyZcv48ePfeOONkt4FAVMWTwGNw+SWhk4/o8YuAva2DVG4/MUSCEg9IQm4c+dO+6wHfgoCpizurhlTXRlTYAUR0LghpKa6jhqCEJB6whCw2KwHfgoCpiwurm1cvpcxxtkqW0DxfjhqCEJA6pEu4P79+8844wzHWQ/8FARMWYq5tlU9ZOm1WcZDyi5SCEJA6pEr4MGDB88+++xisx74KQiYshQTsHF21kKVeE9IT+ugY4wVGmdni61gBAIifiNRQM9ZD/wUBExZit0TYu+pBRwJoRkISD2yBPQz64GfgoApi90148pBezcNAiIxRIqAPmc98FMQMGWxu9a+uM/xKBUCIjFEioA+Zz3wUxAwZSljbhgIiESX4AL6n/XAT0HAlAUCQkDSCSjggw8+6H/WAz8FAVMWCAgBSSeIgKXOeuCnIGDKEpmA3dkBgg/PhIDUU7aAZcx64KcgYMoSjYB8egVqCEJA6ilPwPJmPfBTEDBlKUPAYvNo8SfJ1dc0iQtXLdzMhCKFIASknjIELHvWAz8FAVMWFwG7swMtDZ32u+KMuVSDPCmJyIPi+iEg/ZQq4J49eyZMmFDerAd+CgKmLI5XRBtTwtRU1xkzpBZb0x6MhCCSU5KAAWc98FMQMGXhrq1auNmAz+jl2btpEBCJIf4FDD7rgZ+CgGlKb9sQnyXfqJaGzmLAQUAkhvgU0Jj14L777gtLvnxBwBSkt21o1cLNFvtWNHZvVQ+5vAsCIjHEj4CyZj3wUxAwudm4fK8dPj/2GYGASAzxFFDirAd+CgImNMypWho6/dhnBAIiMcRTwPnz59fW1kqZ9cBPQcCEZsPSHaJ9xiSmJW0BAiIxxF1AubMe+CkImNxwwsRrXMp4e0ABcUU0UkJcBJQ+64GfgoCJTn1NU6ldPx4pAjbOzrL8w+coBAJSTzEBw5j1wE9BwIpNcAEN/vhheOwt6oeA9OMoYEizHvgpCFixkfWsOFIIQkDqsQsY3qwHfgoCVmz8PCuu1PgfiQ4pEJB6LAKGOuuBn4KAFRv/s8gkKBCQekQB9+zZU1VVFd6sB35KmoDP6P2aR379c/3tXSnMm9u8297/4/h1sAQCIjGECzg4OBj2rAd+SpqA252v0RVrwYIFks44JbViB0IMBERiiCFgNLMe+Cm5AvbruksGdWlPiydV7+nebe9/i9zZBkPA+pqm2E/eSQwEpB5DwAsvvDCCWQ/8FAQMXgkVcEVjN52ZTWUFAlJPX+4IYyyaWQ/8FAQMXgkVMJWBgKTTvnP4sqvnMMaimfXAT0HA4AUB6QQCks6Ma+d//fJaUgpAwOAFAekEAtLNVQ13TL0g8/LvPiKlAAQMXhCQTiAg0Vx/+4MTv3z+qp+/H+SJ6WEUBAxeEJBOICDFNNy7fuzEKQ+9dKAjwBPTQyoIGLwgIJ1AQHJZ8NCLJ4+b+MCmfcaPEFDizmsKY4xl1Fz5m8ipGX69sqKVtw0ISCfhCtj3jvd1/+muUr+xxWu3jDplfLb7Db4EAsrac4M/FlBAXdc5gxAw+QlXwO3E/nqjrLdK/w7vfnLnSaeMW9K1XVwIASXuvIQ+oK7rEDBFgYBhVakCLnthz+ixE25/9GeW5RBQ4s4bckFAhAcChlUlCdjaM3hK1Rnz73/a/hIE9NwlfnjLBJbE83UcPYuAjuuYt8YUjS8pgAcBUxMIGFb5F/DhLQfHTz577pLVjq9CQPf9GeEpo+byLmXUnMks4QdRQMd1amuZpSBgugMBwyqfAj667cPJ51z4z9+9r9gKENBtbwro2RcWgOJK9nIBi6/T5eNcIQRMTSBgWOVHwMdfPXL29Csuv3GhyzoQ0G1vRtwyUyR0C43i3HVxAf2sU7wgYGoCAcMqTwHbdw5/7Yo5F11907pdJyAgnT5gLwSspEDAsMpTwBnXzv9Kpnbta8fcv0MI6Lo7fDCjcDrPeuZO+EHsMrqvIwpoOw9o/tAyvkMISCYQMKxyF9CY9eCxX3zk+R1CQK89Mo/o5lGyjfOaFhjA+VlHtwhofk95CkJAOkm2gI5XMxApFwH5rAd+vkMIGHeL5RcEpJMECyhe2ZCnkBCCxQQUZz2AgBAQAsabBAuoKYqiWE/N0CHQUcDbHnpBnPUAAkJACBhvEi2gSF4CBLTPegABISAEjDeJEdDpzie+TNGEixv4eetl5tubRmq874XBNLUI6DjrAQSEgBAw3iRDQMc7n4qsYL8JygAzp6qqqmp6CQsDlSjgshf2jDq1yj7rAQSEgBAw3iRBQK8DXNdhELHvaLu8wXth+cUFdJn1AAJCQAgYb5IgoOOdT9ZXi5tlu/q/tIXlliFgW+97LrMeQEAICAHjTRIEdOkDijjm1IwTXpqayV80WNiC/4VllyGg+6wHEBACQsB4kwQBi9z5ZIExp2ZM97wzRcsLaTkj6H9hkBr4wxHGmPusBxAQAkLAeJMIAXWHO59sNydZLGNsrlLo0an2YWEfCwMh+N1FdzHG3Gc9gIDuCvxe1xljn+p6yvJHCEgmSREwedX/+8NSvsNKFrBf1x3+N5eWgoAUAgHDqjKelAQBEQgYcSBgWAUBPQIBISCBQMCwCgJ6BAJCQAKBgGEVBPQIBISABAIBwyoI6BEICAEJBAKGVRDQIxAQAhIIBAyrIKBHICAEJBAIGFZBQI9AQAhIIBAwrIKAHoGAEJBAwhWw+zdpvqbfT0HAovEh4Nu4JyRuIFKfcAVEgqeSBcTMCLEDkfpAQOqBgHG3WH5BQDqBgNQDAeNusfyCgHQCAakHAsbdYvkFAekEAlIPBIy7xfILAtIJBKQeCBh3i+UXBKQTCEg9EDDuFssvCEgnEJB6IGDcLZZfEJBOICD1JFrAl9uGlG/94IeNzz7d8sttqw+bXoWAEJBAICD1JFrAXetOjBszid8FMXb0hAvPuvz6S5XFNzz84jO/Iyag8fAtCQ9K9SwISCcQkHoSLWB/hz535iL7DWH/cFbN9t6/RC6g+HhpS/FnD/oSUHhSYTkPFISAdAIBqSfpAt717bUW/i6aNvMXj31E7yi4SB9Q0zRNc2KOP2K65IKAdAIBqSehAj5z7+7517R8qWraaSd/UeTvH8/55v+t+bi/g+B5QEcBc2omk8k4MgcB0xAISD0JEnDXuhP/dddrN1+1ZOLYKZNOm3rz1Xc+cffru9aduP6y2wz+Ljnv6lfXfDKyfnABhWNRp1I0cYWMmjMZN3JELNRcxbK5jNolrpRpackwAT0ImIZAQOqhL+DOdcc773hl7sxF48ZMOnPCObdec++z2QFxhcebX2aMzfjKNa8+fqSwXEYfcIS4jJrLY6doBk0ZtbfAnfHimWcWLDT0KvxL0Qqgml4XmRM/Q4eA6QgEpB6yAr7e/una5t5/uWzBqaOqzppU3VR3/09+8JbjL9nr7Z9+K/OdHY8fNS2XcxQ8wpDKe3uKljPK3kFUuiwomi0Ue4j83y7MQcA0BAJSDzUB/zBwlDF2beaWk78w9rwvXbzo+pWbHthXzi+fpPOA+aPZTEZVlJE+nKrmO4Xms3rCMtNBsNjFg4CVFQhIPUQE/Pjjjzdu3Dhv3ryTR41hjP37jerPHtwf6JdP1khI3jJFy/fnMqqmi0fIuq7rOU3TtC7jRJ6i5VRFURTLqK9dQKZoeeZyuVyu13Qe0Lz9kgoC0gkEpJ54BTx8+PBzzz1XX18/evToK6+8cu3atW++8q6cv0xpY8HCcK2mmEwyj3bkzwMy2xgHcxgGGalzhbGUwkiI5RC71H4gBKQTCEg9sQj4wQcfPPHEE9dee+2oUaNqa2u7urr+9Kc/jfz1pu6+4OjuBckXBKSTcAXse0d3/B9r5VSyBDx48OD69etnzZo1atSo66677qmnnjp06JD1rzcNAmr5M4a5QEez5RYEpBM8LTOsStDTMg8cOLBmzZorrrhi9OjRN9544/PPP3/48OFiK6dCQNtAcTnjGeUXBKQTCBhW0RdwcHDwkUcemTFjxpgxYxoaGjZt2vTJJ594visVAsZcEJBOIGBYRVbAXC7X2to6ffr0sWPH3nrrrZqmHT161P/bIWDwgoB0AgHDKmoCvvnmm8uWLbvggguqqqqampq2bNkyPDxcxnYgYPCCgHQCAcMqIgLu3r27paVl2rRpp59+enNz8/bt248fPx6kXRAweEFAOoGAYVWMAp44cWLHjh133HHHlClTpk6deuedd77++uuy2gUBgxcEpBMIGFZFL+Dx48dfeeWVRYsWTZo06ZxzzslmswMDA9LbBQGDFwSkEwgYVkUm4Kefftrb27tgwYKqqqqvfvWr999//1tvvRVeuyBg8IKAdAIBw6qwBTx69GhPT88tt9wyduzYiy++eOXKlfv27YugXRAweEFAOkmygOJlrdFe0eqnQhKQz1AwZsyYSy+9VFXV/fv3R9muKAXco+uMsXd1PWXZBwHJJMkC6rouTOIR6qeUUXIFPHz4cHd3tzhDwbvvvhtLu6IUsF9P812VEJBCIGBYJUXAVf/7wT2PPMEYM2Yo+NGPfsRnKIirIhawcgMBIwkEDKuCCPjwloMN964/75KrP/P5UZdefR1jzD5DQVwFASFgmpIYAc1TvRXAswvIZ8xcZnsWDmOMjfe9MNhsIWUI+NBLB266c83Z06/47BdGT//mjd9tfX719r8QmSGVFwSEgGlKMgQUJjOyzmXkIqBmerRNTlVVVdX0EhYGKv8CtvYM3vBvj0y9IPO5UWO+fk3D9x7etObVT/irELBCAwEjSRIEdJ3AzesoWOw78i34X1h+eQp4/09zc77fOvnc6V84eeyMultvX62tfe2YfTUIWKGBgJEkCQKO0OSMnPd5QMszDktdWG4VE/A/fvzmt25bNunvLxh1alVNfdPi9r72ncMu3yEErNBAwEiSBAED9QF1Tc3kLxsUHiDhe2HZZRHw3md2186/Z/zks0+pOuPKbzcv6dq+budxP98hBKzQQMBIkgQBC5c+F56GkxfP/JKu66bzgIUHGlofqO1vYZAyBFy6YcdVDXeM/eKZp02aevXNd9795M5Sv0MIWKGBgJEkEQLq1mnNTY8rtC7OC1h4+ldGVe3Dwj4WBkJwzZObGGMTzjxndmM2++xA2d9hJQu4W9cH0pjdEJBMkiJg8qpv96CU77CSBRzEPSFxA5H6QMCwisgMqdILMyMEL8yMQCcQMKyCgB6BgBCQQCBgWAUBPQIBISCBQMCwCgJ6BAJCQAKBgGEVBPQIBISABAIBwyoI6BEICAEJBAKGVRDQIxAQAhIIBAyrIKBHICAEJBAIGFZBQKRxG/0AAAIwSURBVI/IE9C4C0jCvdyhbdBSEJBOwhWw+zdpvqbfT0HAopEkIJ/UzDdYrjNtlLPBkh/aBQHpJFwBkeCBgJ67RKQP6P+BDRCQTiAg9UBAz10y6JEoYNENaorL50DAJAYCUg8E9NylqAT0+BwImMRAQOqBgJ67ZJsmzfnRB9wm8SBXc5oirddpg5Y1a2utp/0gYBIDAakHAnrukmma3MIzFUwjHsYPimYa6BDWMPHlusERXcWJeE27AQETFQhIPRDQc5fMR6f52b7tPUMmzjJeEFAk0v5v2/ThOApOVSAg9UBAz11yBqv4A7aE9TUlk8kU6QNCwEoIBKQeCOi5S+59QLtIhfXN5/YsD6GGgJUQCEg9ENBzl0yX7xXO7VmfoqVpmqaZjoI1RVEUB7HcN6houqZpy6wjIQ4P7Sr6HUJAMoGA1AMBXXfH8Wwf79JZXxVHQhhjmZYW67uVLpcNKnMLP5jGgos8tKvodwgByQQCUg8EDK0pebfMnb2gj0n1URCQTiAg9UDA0JqiqaqqFs7qhT0fQqEgIJ1AQOqBgOG1xX4IHUEHUIeAlAIBqQcCxt1i+QUB6QQCUg8EjLvF8gsC0gkEpB4IGHeL5RcEpBMISD0QMO4Wyy8ISCcQkHogYNwtll8QkE4gIPVAwLhbLL8gIJ1AQOqBgHG3WH5BQDpxFvDYsM5QlCqmP1WHGj4m53fjcyed5LnOST7WQaGCl/G7TejPDIVCoSIuCIhCoSq3/h+CPNzrgl2zuwAAAABJRU5ErkJggg==)**

c1指向一个对象，c2也指向一个对象，c1和c2里面装着的是这两只Cat对象在堆内存里面存储的地址，由于这两只Cat对象分别位于不同的存储空间，因此c1和c2里面装着的地址肯定不相等，因此c1和c2这两个引用对象也肯定不相等。因此执行：“System.out.println(c1==c2);”打印出来的结果肯定是false。因此你new出来了两个对象，你放心，这两个对象的引用永远不一样，一样的话就会把其中一个给覆盖掉了，这个可不成。c1是不是等于c2比较的是c1和c2这两个引用里面装着的内容，因为new出来的两个对象的它们的引用永远不一样，因此c1和c2这两个引用的内容也永远不一样，因此c1永远不可能等于c2。因此通过比较两个对象的引用是永远无法使得两个对象相等的，一模一样的。

**要想判断两个对象是否相等，不能通过比较两个对象的引用是否相等，这是永远都得不到相等的结果的，因为两个对象的引用永远不会相等，所以正确的比较方法是直接比较这两个对象，比较这两个对象的实质是不是一样的，即这两个对象里面的内容是不是相同的，通过比较这两个对象的属性值是否相同而决定这两个对象是否相等。**

　　Object类提供了一个equals()方法来比较两个对象的内容是否相同，因此我们可以采用这个方法去比较两个对象是否在逻辑上“相等”。如：c1.equals(c2);这里是调用从Object类继承下来的equals()方法，通过查阅API文档得到Object类里的equals方法的定义如下：

public boolean equals([Object](mk:@MSITStore:D:\java帮助文档\j2se7.chm::/j2se7/api/java/lang/Object.html) obj)

　　在Object这个类里面提供的Equals()方法默认的实现是比较当前对象的引用和你要比较的那个引用它们指向的是否是同一个对象，即和“c1==c2”这种写法是一样的，“c1.equals(c2)”与“c1==c2”是完全等价的。因此直接使用继承下来的equals()方法也是无法直接比较两个对象的内容是否相同的，为此，我们必须得重写equals()方法，改变这个方法默认的实现。

下面在Cat类里面**重写这个继承下来的****equals()****方法**：

```
1 class Cat {
 2     int color, weight, height;
 3 
 4     public Cat(int color, int weight, int height) {
 5         this.color = color;
 6         this.weight = weight;
 7         this.height = height;
 8     }
 9     
10     /**
11      * 这里是重写相等从Object类继承下来的equals()方法，改变这个方法默认的实现，
12      * 通过我们自己定义的实现来判断决定两个对象在逻辑上是否相等。
13      * 这里我们定义如果两只猫的color，weight，height都相同，
14      * 那么我们就认为这两只猫在逻辑上是一模一样的，即这两只猫是“相等”的。
15      */
16     public boolean equals(Object obj){
17         if (obj==null){
18             return false;
19         }
20         else{
21             /**
22              * instanceof是对象运算符。
23              * 对象运算符用来测定一个对象是否属于某个指定类或指定的子类的实例。
24              * 对象运算符是一个组合单词instanceof。
25              * 该运算符是一个双目运算符，其左边的表达式是一个对象，右边的表达式是一个类，
26              * 如果左边的对象是右边的类创建的对象，则运算结果为true，否则为false。
27              */
28             if (obj instanceof Cat){
29                 Cat c = (Cat)obj;
30                 if (c.color==this.color && c.weight==this.weight && c.height==this.height){
31                     return true;
32                 }
33             }
34         }
35         return false;
36     }
37 }
```

此时在再main方法里面执行打印的命令：

```
1 public static void main(String[] args) {
 2         /**
 3          * 这里使用构造方法Cat()在堆内存里面new出了两只猫，
 4          * 这两只猫的color，weight，height都是一样的，
 5          * 但c1和c2却永远不会相等，这是因为c1和c2分别为堆内存里面两只猫的引用对象，
 6          * 里面装着可以找到这两只猫的地址，但由于两只猫在堆内存里面存储在两个不同的空间里面，
 7          * 所以c1和c2分别装着不同的地址，因此c1和c2永远不会相等。
 8          */
 9         Cat c1 = new Cat(1, 1, 1);
10         Cat c2 = new Cat(1, 1, 1);
11         System.out.println("c1==c2的结果是："+(c1==c2));//false
12         System.out.println("c1.equals(c2)的结果是："+c1.equals(c2));//true
13     }
```

　　这一次得到的结果就与上次没有重写equals()方法时得到的结果就不一样了：

**“System.out.println(c1 == c2);”打印出来的结果依然是false**，因为这里是比较两个对象的引用里面的内容，这两个引用里面的内容当然不相等，而且永远不会相等，所以打印出来的结果肯定是false。

**“System.out.println(c1.equals(c2));”打印出来的结果为true**，因为我们在Cat类里面重写了equals()方法，改变了这个方法默认的实现，我们把方法的实现改为只要这个两个对象是真的存在，并且都是猫，并且它们的颜色(color)，身高(height)和体重(weight)都相同，那么这两只猫在逻辑上就是一模一样的，是完全相同的两只猫，即这两只猫是“相等”的。所以这里打印出来的结果是true。

### 1.3.如何比较两个字符串对象是否相等？

看下面的例子：

```
1 public class TestEquals {
2     
3     public static void main(String args[]){
4         String s1 = new String("hello");
5         String s2 = new String("hello");
6         System.out.println("s1 == s2的结果是："+(s1 == s2));//false
7         System.out.println("s1.equals(s2)的结果是："+s1.equals(s2));//true
8     }
9 }
```

这一次是比较两个字符串对象是否相等：

　　System.out.println(s1 == s2);

　　打印出来的结果依然是fase，因为这里比较的是s1和s2两个字符串对象的引用，两个对象的引用永远不会相等，所以打印出来的结果为false。

　　System.out.println(s1.equals(s2));

　　打印出来的结果为true，因为在String类里面重写了从Object类继承(所有的类都是从Object类继承下来，String类当然也不例外，从父类继承下来就拥有了父类的一切属性与方法，所以Sting类里面也有equals()方法，并且还把这个继承下来的equals()方法重写了)下来的equals()方法，改变了这个方法默认的实现，

**　　在String类里面是这样重写equals()方法的实现的：用当前的这个字符串对象和指定的字符串对象比较，指定的字符串对象不能为空并且这个对象的字符序列和当前这个字符串对象的字符串序列一样，如果这些条件都满足，那么这两个字符串对象就是相等的。**

因此这里的s2已经满足了条件，所以打印出来的结果是true。

　　以后在某一个类里面比较两个对象是否相等时，首先去API文档里面查找这个类是否重写了从Object类继承下来的equals()方法。如果重写了equals()方法，那么在比较两个对象是否相等时调用的就是重写以后的equals()方法，如果没有重写，那么调用时就是直接调用从Object类里面的继承下来的那个equals()方法，并且采用equals()方法默认的实现去比较两个对象是否相等。因此每一个类都可以根据需要对从Object类继承下来的equals()方法进行重写。

　　对于在API文档里面找某个类，如果一个类不用引入包就可以直接使用，那么这个类肯定是在java.lang这个包里面，如这里的String类，直接就可以使用了，所以String类一定是在java.lang这个包里面。使用某个类时看这个类引入的是哪个包，然后就去这个包里面找这个类，不用引入包的类一定是位于java.lang里面，直接去java.lang里面找就可以了。

　　总结：比较两个对象是否相等，我们采用equals()方法，判断两个对象是否相等的条件是由我们重写equals()方法的实现后定义的，这样就可以比较灵活地使用equals()方法在不同的类里面比较位于同一类下的两个对象是否相等了。









