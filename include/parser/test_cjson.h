#include <stdio.h>
#include "cJSON.hpp"

int main()
{
    cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
    cJSON * next =  cJSON_CreateObject();

    cJSON_AddItemToObject(root, "rc", cJSON_CreateNumber(0));//根节点下添加
    cJSON_AddItemToObject(root, "operation", cJSON_CreateString("CALL"));
    cJSON_AddItemToObject(root, "service", cJSON_CreateString("telephone"));
    cJSON_AddItemToObject(root, "text", cJSON_CreateString("打电话给张三"));
    cJSON_AddItemToObject(root, "semantic", item);//root节点下添加semantic节点
    cJSON_AddItemToObject(item, "slots", next);//semantic节点下添加item节点
    cJSON_AddItemToObject(next, "name", cJSON_CreateString("张三"));//添加name节点

    printf("%s\n", cJSON_Print(root));

    cJSON_Delete(root);
    return 0;
}