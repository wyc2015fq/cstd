# jsoncpp 构造空数组 - DoubleLi - 博客园






因为要构造类似如下的

{"FurnitureItemObject":[],"FurniturePlaceItemObject":[],"RoomNum":0}

的json 数据，得用jsoncpp 构造一个空数组，在网上查了很久，自己也尝试了下，都没成功，没办法在群里向人请教，用resize 可以解决这个问题


- 



Json::Value root;


- 



root["FurniturePlaceItemObject"].resize(0);




就能构成空数组，记录下，供大家参考，也衷心希望与大家分享









