/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/int.h>
#include <alloc.h>

int_value_t *int_init()
{
    int_value_t *num = mr_alloc(sizeof(int_value_t));
    mpz_init(num->num);
    return num;
}

int_value_t *int_set_str(const char *str)
{
    int_value_t *num = mr_alloc(sizeof(int_value_t));
    mpz_init_set_str(num->num, str, 10);
    return num;
}

void int_free(int_value_t *num)
{
    mpz_clear(num->num);
    mr_free(num);
}

char *int_get_str(const int_value_t *num)
{
    return mpz_get_str(NULL, 10, num->num);
}

void int_print(const int_value_t *num)
{
    gmp_printf("%Zd", num->num);
}

void int_add(int_value_t *res, const int_value_t *left, const int_value_t *right)
{
    mpz_add(res->num, left->num, right->num);
}

void int_sub(int_value_t *res, const int_value_t *left, const int_value_t *right)
{
    mpz_sub(res->num, left->num, right->num);
}

void int_mul(int_value_t *res, const int_value_t *left, const int_value_t *right)
{
    mpz_mul(res->num, left->num, right->num);
}

void int_neg(int_value_t *res, const int_value_t *num)
{
    mpz_neg(res->num, num->num);
}

uint8_t int_iszero(const int_value_t *num)
{
    return !mpz_size(num->num);
}
