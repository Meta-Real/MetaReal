/*
    MetaReal Compiler version 1.0.0
*/

#include <generator/generator.h>
#include <optimizer/complex.h>
#include <optimizer/list.h>
#include <string.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <consts.h>

#define generate_datatype_consts                                                                             \
    do                                                                                                       \
    {                                                                                                        \
        len = slen + clen + 18;                                                                              \
        if (res->csize + len >= res->calloc)                                                                 \
            res->consts = mr_realloc(res->consts, res->calloc += slen + GEN_CONSTS_LEN);                     \
                                                                                                             \
        sprintf(res->consts + res->csize, ".LC%llu:\n\t.ascii\t\"%s\\0\"\n", id, str);                       \
                                                                                                             \
        if (data->csize == data->calloc)                                                                     \
            data->consts = mr_realloc(data->consts, (data->calloc += GEN_CONSTS_LIST_LEN) * sizeof(vstr_t)); \
                                                                                                             \
        data->consts[data->csize].off = res->csize + clen + 14;                                              \
        data->consts[data->csize++].size = slen;                                                             \
                                                                                                             \
        res->csize += len;                                                                                   \
    } while (0)

#define generate_datatype_main                                                                \
    do                                                                                        \
    {                                                                                         \
        len = clen + 33;                                                                      \
        if (res->msize + len >= res->malloc)                                                  \
            res->main = mr_realloc(res->main, res->malloc += GEN_MAIN_LEN);                   \
                                                                                              \
        sprintf(res->main + res->msize, "\tleaq\t.LC%llu(%%rip), %%rcx\n\tcall\tputs\n", id); \
        res->msize += len;                                                                    \
    } while (0)

struct __GEN_ADV_RES_T
{
    char *consts;
    uint64_t csize;
    uint64_t calloc;

    char *main;
    uint64_t msize;
    uint64_t malloc;
};
typedef struct __GEN_ADV_RES_T gen_adv_res_t;

struct __VSTR_T
{
    uint64_t off;
    uint64_t size;
};
typedef struct __VSTR_T vstr_t;

struct __DATA_T
{
    vstr_t *consts;
    uint64_t csize;
    uint64_t calloc;
};
typedef struct __DATA_T data_t;

void generate_value(gen_adv_res_t *res, data_t *data, value_t *value);

void generate_none(gen_adv_res_t *res, data_t *data);
void generate_int(gen_adv_res_t *res, data_t *data, int_value_t *value);
void generate_float(gen_adv_res_t *res, data_t *data, float_value_t *value);
void generate_complex(gen_adv_res_t *res, data_t *data, complex_value_t *value);
void generate_bool(gen_adv_res_t *res, data_t *data, uint8_t value);
void generate_list(gen_adv_res_t *res, data_t *data, list_value_t *value);

char *value_sprint(char *consts, uint64_t *csize, uint64_t *calloc, value_t *value);
char *list_sprint(char *consts, uint64_t *csize, uint64_t *calloc, list_value_t *value);

uint64_t value_set_id(uint8_t *new, const char *consts, const data_t *data, const char *value, uint64_t size);
uint8_t uint64_len(uint64_t num);

gen_res_t generate(value_t *values, uint64_t size)
{
    gen_adv_res_t res_adv;

    res_adv.consts = mr_alloc(GEN_CONSTS_LEN);
    res_adv.csize = 0;
    res_adv.calloc = GEN_CONSTS_LEN;

    res_adv.main = mr_alloc(GEN_MAIN_LEN);
    res_adv.msize = 48;
    res_adv.malloc = GEN_MAIN_LEN;

    strcpy(res_adv.main, "\t.globl\tmain\nmain:\n\tsubq\t$32, %rsp\n\tcall\t__main\n");

    data_t data;
    data.consts = mr_alloc(GEN_CONSTS_LIST_LEN * sizeof(vstr_t));
    data.csize = 0;
    data.calloc = GEN_CONSTS_LIST_LEN;

    uint64_t i;
    for (i = 0; i < size; i++)
        generate_value(&res_adv, &data, values + i);

    mr_free(data.consts);
    mr_free(values);

    res_adv.consts = mr_realloc(res_adv.consts, res_adv.csize + 1);
    res_adv.consts[res_adv.csize] = '\0';

    res_adv.main = mr_realloc(res_adv.main, res_adv.msize + 39);
    strcpy(res_adv.main + res_adv.msize, "\txorl\t%eax, %eax\n\taddq\t$32, %rsp\n\tret\n");

    gen_res_t res;
    res.consts = res_adv.consts;
    res.main = res_adv.main;
    return res;
}

void generate_value(gen_adv_res_t *res, data_t *data, value_t *value)
{
    switch (value->type)
    {
    case NONE_V:
        generate_none(res, data);
        return;
    case INT_V:
        generate_int(res, data, value->value);
        return;
    case FLOAT_V:
        generate_float(res, data, value->value);
        return;
    case COMPLEX_V:
        generate_complex(res, data, value->value);
        return;
    case BOOL_V:
        generate_bool(res, data, (uintptr_t)value->value);
        return;
    case LIST_V:
        generate_list(res, data, value->value);
        return;
    }

    fprintf(stderr, "Internal Error: Invalid value type #%hu (generate_value function)\n", value->type);
    abort();
}

void generate_none(gen_adv_res_t *res, data_t *data)
{
    char *str = "none";
    uint8_t slen = 4;

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, res->consts, data, str, slen);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
        generate_datatype_consts;

    generate_datatype_main;
}

void generate_int(gen_adv_res_t *res, data_t *data, int_value_t *value)
{
    char *str = int_get_str(value);
    uint64_t slen = strlen(str);

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, res->consts, data, str, slen);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
        generate_datatype_consts;

    generate_datatype_main;
    mr_free(str);
    int_free(value);
}

void generate_float(gen_adv_res_t *res, data_t *data, float_value_t *value)
{
    char *str = float_get_str(value);
    uint64_t slen = strlen(str);

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, res->consts, data, str, slen);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
        generate_datatype_consts;

    generate_datatype_main;
    mr_free(str);
    float_free(value);
}

void generate_complex(gen_adv_res_t *res, data_t *data, complex_value_t *value)
{
    char *str = complex_get_str(value);
    uint64_t slen = strlen(str);

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, res->consts, data, str, slen);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
        generate_datatype_consts;

    generate_datatype_main;
    mr_free(str);
    complex_free(value);
}

void generate_bool(gen_adv_res_t *res, data_t *data, uint8_t value)
{
    char *str;
    uint8_t slen;
    if (value)
    {
        str = "true";
        slen = 4;
    }
    else
    {
        str = "false";
        slen = 5;
    }

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, res->consts, data, str, slen);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
        generate_datatype_consts;

    generate_datatype_main;
}

void generate_list(gen_adv_res_t *res, data_t *data, list_value_t *value)
{
    uint64_t off = res->csize;

    uint64_t clen = uint64_len(data->csize);
    uint64_t len = clen + 14;
    if (off + len >= res->calloc)
        res->consts = mr_realloc(res->consts, res->calloc += GEN_CONSTS_LEN);

    sprintf(res->consts + off, ".LC%llu:\n\t.ascii\t\"", data->csize);
    res->csize += len;

    res->consts = list_sprint(res->consts, &res->csize, &res->calloc, value);
    uint64_t id = off + len;

    uint8_t new = 0;
    uint64_t size = res->csize - id;
    id = value_set_id(&new, res->consts, data, res->consts + id, size);

    if (new)
    {
        if (res->csize + 4 > res->calloc)
            res->consts = mr_realloc(res->consts, res->calloc += GEN_CONSTS_LEN);

        memcpy(res->consts + res->csize, "\\0\"\n", 4);

        if (data->csize == data->calloc)
            data->consts = mr_realloc(data->consts, (data->calloc += GEN_CONSTS_LIST_LEN) * sizeof(vstr_t));

        data->consts[data->csize].off = off + len;
        data->consts[data->csize++].size = size;

        res->csize += 4;
    }
    else
    {
        res->csize = off;
        clen = uint64_len(id);
    }

    generate_datatype_main;
}

char *value_sprint(char *consts, uint64_t *csize, uint64_t *calloc, value_t *value)
{
    char *str = NULL;
    uint64_t len = 0;
    switch (value->type)
    {
    case INT_V:
        str = int_get_str(value->value);

        int_free(value->value);
        goto sfree;
    case FLOAT_V:
        str = float_get_str(value->value);

        float_free(value->value);
        goto sfree;
    case COMPLEX_V:
        str = complex_get_str(value->value);

        complex_free(value->value);
        goto sfree;
    case BOOL_V:
        if (value->value)
        {
            str = "true";
            len = 4;
        }
        else
        {
            str = "false";
            len = 5;
        }

        break;
    case LIST_V:
        return list_sprint(consts, csize, calloc, value->value);
    }

    if (*csize + len > *calloc)
        consts = mr_realloc(consts, *calloc += len + GEN_CONSTS_LEN);

    memcpy(consts + *csize, str, len);
    *csize += len;
    return consts;

sfree:
    len = strlen(str);

    if (*csize + len > *calloc)
        consts = mr_realloc(consts, *calloc += len + GEN_CONSTS_LEN);

    memcpy(consts + *csize, str, len);
    *csize += len;
    return consts;
}

char *list_sprint(char *consts, uint64_t *csize, uint64_t *calloc, list_value_t *value)
{
    if (!value)
    {
        if (*csize + 2 > *calloc)
            consts = mr_realloc(consts, *calloc += GEN_CONSTS_LEN);

        memcpy(consts + *csize, "[]", 2);
        *csize += 2;

        return consts;
    }

    if (*csize == *calloc)
        consts = mr_realloc(consts, *calloc += GEN_CONSTS_LEN);

    consts[(*csize)++] = '[';
    consts = value_sprint(consts, csize, calloc, value->elements);

    for (uint64_t i = 1; i < value->size; i++)
    {
        if (*csize + 2 > *calloc)
            consts = mr_realloc(consts, *calloc += GEN_CONSTS_LEN);

        memcpy(consts + *csize, ", ", 2);
        *csize += 2;

        consts = value_sprint(consts, csize, calloc, value->elements + i);
    }

    if (*csize == *calloc)
        consts = mr_realloc(consts, *calloc += GEN_CONSTS_LEN);

    consts[(*csize)++] = ']';

    mr_free(value->elements);
    mr_free(value);
    return consts;
}

uint64_t value_set_id(uint8_t *new, const char *consts, const data_t *data, const char *value, uint64_t size)
{
    for (uint64_t i = 0; i < data->csize; i++)
        if (data->consts[i].size == size && !strncmp(consts + data->consts[i].off, value, size))
            return i;

    *new = 1;
    return data->csize;
}

uint8_t uint64_len(uint64_t num)
{
    uint8_t len;
    for (len = 1; num /= 10; len++);
    return len;
}
