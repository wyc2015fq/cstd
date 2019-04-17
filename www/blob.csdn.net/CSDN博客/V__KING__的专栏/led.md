# led - V__KING__的专栏 - CSDN博客





2014年04月11日 16:07:14[v__king__](https://me.csdn.net/V__KING__)阅读数：741








                
//例2.点亮一个led，闪烁

int led_pin= 8;

void setup(){

    pinMode(led_pin,OUTPUT);

}


void loop(){

    digitalWrite(led_pin,LOW);

    delay(500); //延时0.5秒

    digitalWrite(led_pin,HIGH);

    delay(500); //延时0.5秒

}
            


