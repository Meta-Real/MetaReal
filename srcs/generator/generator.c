/*
    MetaReal Compiler version 1.0.0
*/

#include <generator/generator.h>
#include <optimizer/complex.h>
#include <string.h>
#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <consts.h>

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

struct __DATA_T
{
    char **consts;
    uint64_t csize;
    uint64_t calloc;
};
typedef struct __DATA_T data_t;

void generate_value(gen_adv_res_t *res, data_t *data, value_t *value);

void generate_int(gen_adv_res_t *res, data_t *data, int_value_t *value);
void generate_float(gen_adv_res_t *res, data_t *data, float_value_t *value);
void generate_complex(gen_adv_res_t *res, data_t *data, complex_value_t *value);
void generate_bool(gen_adv_res_t *res, data_t *data, uint8_t value);

uint64_t value_set_id(uint8_t *new, const data_t *data, const char *value);
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
    data.consts = mr_alloc(GEN_CONSTS_LIST_LEN * sizeof(char*));
    data.csize = 0;
    data.calloc = GEN_CONSTS_LIST_LEN;

    uint64_t i;
    for (i = 0; i < size; i++)
        generate_value(&res_adv, &data, values + i);

    for (i = 0; i < data.csize; i++)
        mr_free(data.consts[i]);
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
    }

    fprintf(stderr, "Internal Error: Invalid value type #%hu (generate_value function)\n", value->type);
    abort();
}

void generate_int(gen_adv_res_t *res, data_t *data, int_value_t *value)
{
    char *num = int_get_str(value);
    uint64_t nlen = strlen(num);

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, data, num);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
    {
        len = nlen + clen + 18;
        if (res->csize + len > res->calloc)
            res->consts = mr_realloc(res->consts, res->calloc += nlen + GEN_CONSTS_LEN);

        sprintf(res->consts + res->csize, ".LC%llu:\n\t.ascii\t\"%s\\0\"\n", id, num);
        res->csize += len;

        if (data->csize == data->calloc)
            data->consts = mr_realloc(data->consts, (data->calloc += GEN_CONSTS_LIST_LEN) * sizeof(char*));

        data->consts[data->csize++] = num;
    }
    else
        mr_free(num);

    len = clen + 33;
    if (res->msize + len > res->malloc)
        res->main = mr_realloc(res->main, res->malloc += GEN_MAIN_LEN);

    sprintf(res->main + res->msize, "\tleaq\t.LC%llu(%%rip), %%rcx\n\tcall\tputs\n", id);
    res->msize += len;

    int_free(value);
}

void generate_float(gen_adv_res_t *res, data_t *data, float_value_t *value)
{
    char *num = float_get_str(value);
    uint64_t nlen = strlen(num);

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, data, num);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
    {
        len = nlen + clen + 18;
        if (res->csize + len > res->calloc)
            res->consts = mr_realloc(res->consts, res->calloc += nlen + GEN_CONSTS_LEN);

        sprintf(res->consts + res->csize, ".LC%llu:\n\t.ascii\t\"%s\\0\"\n", id, num);
        res->csize += len;

        if (data->csize == data->calloc)
            data->consts = mr_realloc(data->consts, (data->calloc += GEN_CONSTS_LIST_LEN) * sizeof(char*));

        data->consts[data->csize++] = num;
    }
    else
        mr_free(num);

    len = clen + 33;
    if (res->msize + len > res->malloc)
        res->main = mr_realloc(res->main, res->malloc += GEN_MAIN_LEN);

    sprintf(res->main + res->msize, "\tleaq\t.LC%llu(%%rip), %%rcx\n\tcall\tputs\n", id);
    res->msize += len;

    float_free(value);
}

void generate_complex(gen_adv_res_t *res, data_t *data, complex_value_t *value)
{
    char *num = complex_get_str(value);
    uint64_t nlen = strlen(num);

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, data, num);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
    {
        len = nlen + clen + 18;
        if (res->csize + len > res->calloc)
            res->consts = mr_realloc(res->consts, res->calloc += nlen + GEN_CONSTS_LEN);

        sprintf(res->consts + res->csize, ".LC%llu:\n\t.ascii\t\"%s\\0\"\n", id, num);
        res->csize += len;

        if (data->csize == data->calloc)
            data->consts = mr_realloc(data->consts, (data->calloc += GEN_CONSTS_LIST_LEN) * sizeof(char*));

        data->consts[data->csize++] = num;
    }
    else
        mr_free(num);

    len = clen + 33;
    if (res->msize + len > res->malloc)
        res->main = mr_realloc(res->main, res->malloc += GEN_MAIN_LEN);

    sprintf(res->main + res->msize, "\tleaq\t.LC%llu(%%rip), %%rcx\n\tcall\tputs\n", id);
    res->msize += len;

    complex_free(value);
}

void generate_bool(gen_adv_res_t *res, data_t *data, uint8_t value)
{
    char *state;
    uint8_t slen;
    if (value)
    {
        state = mr_alloc(5);
        strcpy(state, "true");
        slen = 4;
    }
    else
    {
        state = mr_alloc(6);
        strcpy(state, "false");
        slen = 5;
    }

    uint8_t new = 0;
    uint64_t id = value_set_id(&new, data, state);
    uint8_t clen = uint64_len(id);

    uint64_t len;

    if (new)
    {
        len = slen + clen + 18;
        if (res->csize + len > res->calloc)
            res->consts = mr_realloc(res->consts, res->calloc += slen + GEN_CONSTS_LEN);

        sprintf(res->consts + res->csize, ".LC%llu:\n\t.ascii\t\"%s\\0\"\n", id, state);
        res->csize += len;

        if (data->csize == data->calloc)
            data->consts = mr_realloc(data->consts, (data->calloc += GEN_CONSTS_LIST_LEN) * sizeof(char*));

        data->consts[data->csize++] = state;
    }
    else
        mr_free(state);

    len = clen + 33;
    if (res->msize + len > res->malloc)
        res->main = mr_realloc(res->main, res->malloc += GEN_MAIN_LEN);

    sprintf(res->main + res->msize, "\tleaq\t.LC%llu(%%rip), %%rcx\n\tcall\tputs\n", id);
    res->msize += len;
}

uint64_t value_set_id(uint8_t *new, const data_t *data, const char *value)
{
    for (uint64_t i = 0; i < data->csize; i++)
        if (!strcmp(data->consts[i], value))
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
