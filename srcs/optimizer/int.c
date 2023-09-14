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

void int_add(int_value_t *left, const int_value_t *right)
{
    mpz_add(left->num, left->num, right->num);
}

void int_sub(int_value_t *left, const int_value_t *right)
{
    mpz_sub(left->num, left->num, right->num);
}

void int_mul(int_value_t *left, const int_value_t *right)
{
    mpz_mul(left->num, left->num, right->num);
}

void int_mod(int_value_t *left, const int_value_t *right)
{
    mpz_mod(left->num, left->num, right->num);
}

void int_quot(int_value_t *left, const int_value_t *right)
{
    mpz_fdiv_q(left->num, left->num, right->num);
}

void int_pow(int_value_t *left, const int_value_t *right)
{
    mpz_pow_ui(left->num, left->num, mpz_get_ui(right->num));
}

void int_and(int_value_t *left, const int_value_t *right)
{
    mpz_and(left->num, left->num, right->num);
}

void int_or(int_value_t *left, const int_value_t *right)
{
    mpz_ior(left->num, left->num, right->num);
}

void int_xor(int_value_t *left, const int_value_t *right)
{
    mpz_xor(left->num, left->num, right->num);
}

void int_lshift(int_value_t *left, const int_value_t *right)
{
    mpz_mul_2exp(left->num, left->num, mpz_get_ui(right->num));
}

void int_rshift(int_value_t *left, const int_value_t *right)
{
    mpz_fdiv_q_2exp(left->num, left->num, mpz_get_ui(right->num));
}

void int_neg(int_value_t *num)
{
    mpz_neg(num->num, num->num);
}

void int_not(int_value_t *num)
{
    mpz_com(num->num, num->num);
}

uint8_t int_iszero(const int_value_t *num)
{
    return !mpz_sgn(num->num);
}

uint8_t int_isneg(const int_value_t *num)
{
    return mpz_sgn(num->num) < 0;
}
