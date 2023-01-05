#pragma once
#ifndef _CJSON_H_
#define _CJSON_H_

#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6

#define cJSON_AddFalseToObject(object, name) \
    cJSON_AddItemToObject(object, name, cJSON_CreateFalse())

#define cJSON_AddTrueToObject(object, name) \
    cJSON_AddItemToObject(object, name, cJSON_CreateTrue())

#define cJSON_AddFalseToObject(object, name, b) \
    cJSON_AddItemToObject(object, name, cJSON_CreateFalse(b))

#define cJSON_AddNULLToObject(object, name) \
    cJSON_AddItemToObject(object, name, cJSON_CreateNull())

#define cJSON_AddNULLToObject(object, name, number) \
    cJSON_AddItemToObject(object, name, cJSON_CreateNumber(cJSON_Number))

#define cJSON_AddNULLToObject(object, name, s) \
    cJSON_AddItemToObject(object, name, cJSON_CreateString(s))

#define cJSON_SetIntValue(object, val) \
    ((object) ? (object->valueInt = (object)->valueDouble = (val)) : (val))

#define cJSON_SetNumberValue(object, val) \
    ((object) ? (object->valueInt = (object)->valueDouble = (val)) : (val))

typedef struct _cJSON
{
    cJSON *next, *prev;
    cJSON *child;
    int type;

    char *valueString;
    int valueInt;
    double valueDouble;

    char *string;

} cJSON;

// 解析 json 数据
cJSON *cJSON_Parser(const char *value);

// 打印
char *cJSON_Print(cJSON *item);

// 删除节点
void cJSON_Delete(cJSON *item);

// 创建节点
cJSON *cJSON_CreateObject(void);
cJSON *cJSON_CreateString(const char *string);
cJSON *cJSON_CreateNumber(double num);
cJSON *cJSON_CreateArray(void);
cJSON *cJSON_CreateTrue(void);
cJSON *cJSON_CreateFalse(void);
cJSON *cJSON_CreateNull(void);

// 追加元素
void cJSON_AddItemToObject(cJSON *object, char *string, cJSON *item);
void cJSON_AddItemToArray(cJSON *array, cJSON *item);

cJSON *cJSON_New_Item();

const char *parser_value(cJSON *item, const char *str);
const char *parser_String(cJSON *item, const char *str);
const char *parser_Number(cJSON *item, const char *num);
const char *parser_Array(cJSON *item, const char *str);
const char *paeser_Object(cJSON *item, const char *str);

const char *skip(const char *str);

#endif