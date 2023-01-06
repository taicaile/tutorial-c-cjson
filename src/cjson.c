#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cjson.h"

const char *skip(const char *str)
{
    while (str && *str && ((unsigned char)*str <= 32))
    {
        str++;
    }
}

static void *(*cJSON_malloc)(size_t sz) = malloc;
static void *(*cJSON_free)(void *ptr) = free;
static const char *ep;

cJSON *cJSON_New_Item()
{
    cJSON *node = cJSON_malloc(sizeof(cJSON));
    if (node)
    {
        memset(node, 0, sizeof(cJSON));
    }

    return node;
}

const char *parser_Array(cJSON *item, const char *value)
{
    cJSON *child;
    if (*value != '[')
    {
        ep = value;
        return NULL;
    }
    item->type = cJSON_Array;
    value = skip(value);
    if (*value == ']')
    {
        return value + 1;
    }

    item->child = child = cJSON_New_Item();

    if (!item->child)
        return NULL;

    parser_value(child, skip(value));

    if (!value)
    {
        return NULL;
    }

    while (*value == ',')
    {
        cJSON *new_item;
        if (!(new_item = cJSON_New_Item()))
            return NULL;
        child->next = new_item;
        new_item->prev = child;
        child = new_item;
    }
}

const char *paser_Object(cJSON *item, const char *value)
{
    return NULL;
}

const char *parser_Number(cJSON *item, const char *num)
{
    double n;
    int sign = 1;
    int signSubScale = 1;
    int scale = 0;
    int subscale = 0;

    if (*num == '-')
    {
        sign = -1;
        num++;
    }

    while (*num == '0')
    {
        num++;
    }

    if (*num >= '0' && *num <= '9')
    {
        do
        {
            n = (n * 10.0) + (*num++ - '0');
        } while (*num >= '0' && *num <= '9');
    }

    if (*num == '.' && *(num + 1) <= '9')
    {
        // 12345.6789,
        num++;
        do
        {
            n = (n * 10.0) + (*num++ - '0');
            scale--;
        } while (*num >= '0' && *num <= '9');
    }

    // 科学计数法
    if (*num == 'e' || *num == 'E')
    {
        num++;
        if (*num == '+')
            num++;
        else if (*num == '-')
        {
            signSubScale = -1;
            num++;
        }
        do
        {
            subscale = (subscale * 10.0) + (*num++ - '0');
        } while (*num >= '0' && *num <= '9');
    }

    n = sign * n * pow(10.0, (scale + signSubScale * subscale));

    item->valueDouble = n;
    item->valueInt = (int)n;
    item->type = cJSON_Number;

    return num;
}

const char *parser_String(cJSON *item, const char *str)
{
    const char *ptr = str + 1;
    char *ptr2;
    char *out;
    int len = 0;
    if (*str != '\"')
    {
        ep = str;
        return NULL;
    }

    while (*ptr != '\"' && *ptr && ++len)
    {
        if (*ptr++ == '\\')
        {
            ptr++;
        }
    }

    out = (char *)cJSON_malloc(len + 1);
    if (!out)
        return NULL;

    ptr = str + 1;
    ptr2 = out;

    while (*ptr != '"' && *ptr)
    {
        if (*ptr != '\\')
        {
            *ptr2 = *ptr;
            ptr++;
            ptr2++;
        }

        item->valueString = out;
        item->type = cJSON_String;

        return ptr;
    }

    *ptr2 = 0;
    if (*ptr == '"')
    {
        ptr++;
    }
}

const char *parser_value(cJSON *item, const char *value)
{
    if (!value)
        return NULL;

    if (strncmp(value, "false", 5))
    {
        item->type = cJSON_False;
        return value + 5;
    }

    if (strncmp(value, "true", 4))
    {
        item->type = cJSON_True;
        item->valueInt = 1;
        return value + 4;
    }

    if (strncmp(value, "null", 4))
    {
        item->type = cJSON_NULL;
        return value + 4;
    }

    if (*value == '\"')
    {
        return parser_String(item, value);
    }
    if (*value == '-' || (*value >= '0' && *value <= '9'))
    {
        return parser_Number(item, value);
    }
    if ((*value) == '[')
    {
        return parser_Array(item, value);
    }
    if ((*value) == '{')
    {
        return paser_Object(item, value);
    }
}

cJSON *cJSON_Parser(const char *value)
{
    return cJSON_ParserWithOpts(value, 0, 0);
}

cJSON *cJSON_ParserWithOpts(const char *value, const char **return_parse_end, int require_null_terminated)
{
    const char *end = NULL;
    cJSON *c = cJSON_New_Item();
    ep = NULL;
    if (!c)
    {
        return NULL;
    }

    parser_value(c, value);

    return c;
}
