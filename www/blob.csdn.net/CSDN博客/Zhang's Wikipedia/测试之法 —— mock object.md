
# 测试之法 —— mock object - Zhang's Wikipedia - CSDN博客


2018年06月06日 23:06:12[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：718


mock object 与真实对象相比，用来构造测试场景。
## 1. 一个实例
一个闹钟根据时间来进行提醒服务，如果过了下午5点钟就播放音频文件提醒大家下班了，如果我们要利用真实的对象来测试的话就只能苦苦等到下午五点，然后把耳朵放在音箱旁，我们应该利用mock对象  来进行测试，这样我们就可以模拟控制时间了，而不用苦苦等待时钟转到下午5点钟了。
Environmental：抽象类
publicabstractclassEnvironmental{privatebooleanplayedWave =false;publicabstractlonggetTime();publicvoidplayWavFile(String filename){
        playedWave =true;
    }publicbooleanwasPlayedWave() {returnplayedWave;
    }publicvoidresetWave(){
        playedWave =false;
    }
}
银行实现类：
publicclassBankEnvironmentextendsEnvironmental{publiclonggetTime() {returnSystem.currentTimeMillis();
    }
}
mock 银行对象：
publicclassMockBankEnvironmentextendsEnvironmental{privatelongcurrentTime;publiclonggetTime() {returncurrentTime;
    }publicvoidsetTime(longtime) {
        currentTime = time;
    }
}

