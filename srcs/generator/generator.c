/*
    MetaReal Compiler version 1.0.0
*/

#include <generator/generator.h>
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
    uint64_t ccount;
};
typedef struct __DATA_T data_t;

void generate_value(gen_adv_res_t *res, data_t *data, value_t *value);

void generate_int(gen_adv_res_t *res, data_t *data, int_value_t *value);

uint8_t uint64_len(uint64_t num);

gen_res_t generate(value_t *values, uint64_t size)
{
    gen_adv_res_t res_adv;

    res_adv.consts = mr_alloc(GEN_CONSTS_LEN);
    res_adv.csize = 0;
    res_adv.calloc = GEN_CONSTS_LEN;

    res_adv.main = mr_alloc(GEN_MAIN_LEN);
    res_adv.msize = 49;
    res_adv.malloc = GEN_MAIN_LEN;

    strcpy(res_adv.main, "\t.globl\tmain\nmain:\n\tsubq\t$40, %rsp\n\tcall\t__main\n\n");

    data_t data;
    data.ccount = 0;

    for (uint64_t i = 0; i < size; i++)
        generate_value(&res_adv, &data, values + i);
    mr_free(values);

    res_adv.consts = mr_realloc(res_adv.consts, res_adv.csize + 1);
    res_adv.consts[res_adv.csize] = '\0';

    res_adv.main = mr_realloc(res_adv.main, res_adv.msize + 39);
    strcpy(res_adv.main + res_adv.msize, "\txorl\t%eax, %eax\n\taddq\t$40, %rsp\n\tret\n");

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
    }

    fprintf(stderr, "Internal Error: Invalid value type #%hu (generate_value function)\n", value->type);
    abort();
}

void generate_int(gen_adv_res_t *res, data_t *data, int_value_t *value)
{
    char *num = mpz_get_str(NULL, 10, value->num);
    uint64_t nlen = strlen(num);
    uint8_t clen = uint64_len(data->ccount);

    uint64_t len = nlen + clen + 18;
    if (res->csize + len > res->calloc)
        res->consts = mr_realloc(res->consts, res->calloc += nlen + GEN_CONSTS_LEN);

    sprintf(res->consts + res->csize, ".LC%llu:\n\t.ascii\t\"%s\\0\"\n", data->ccount, num);
    res->csize += len;

    len = clen + 34;
    if (res->msize + len > res->malloc)
        res->main = mr_realloc(res->main, res->malloc += GEN_MAIN_LEN);

    sprintf(res->main + res->msize, "\tleaq\t.LC%llu(%%rip), %%rcx\n\tcall\tputs\n\n", data->ccount);
    res->msize += len;

    data->ccount++;

    mr_free(num);
    free_int_value(value);
}

uint8_t uint64_len(uint64_t num)
{
    uint8_t len;
    for (len = 1; num /= 10; len++);
    return len;
}
