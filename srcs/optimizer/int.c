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

int_value_t *int_set_ui(uint32_t ui)
{
    int_value_t *num = mr_alloc(sizeof(int_value_t));
    mpz_init_set_ui(num->num, ui);
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
    mpz_tdiv_q(left->num, left->num, right->num);
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
    mpz_tdiv_q_2exp(left->num, left->num, mpz_get_ui(right->num));
}

void int_neg(int_value_t *num)
{
    mpz_neg(num->num, num->num);
}

void int_not(int_value_t *num)
{
    mpz_com(num->num, num->num);
}

uint8_t int_eq(const int_value_t *left, const int_value_t *right)
{
    return !mpz_cmp(left->num, right->num);
}

uint8_t int_lt(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) < 0;
}

uint8_t int_gt(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) > 0;
}

void int_add_ui(int_value_t *left, uint32_t right)
{
    mpz_add_ui(left->num, left->num, right);
}

void int_sub_ui(int_value_t *left, uint32_t right)
{
    mpz_sub_ui(left->num, left->num, right);
}

void int_ui_sub(uint32_t left, int_value_t *right)
{
    mpz_ui_sub(right->num, left, right->num);
}

void int_mul_ui(int_value_t *left, uint32_t right)
{
    mpz_mul_ui(left->num, left->num, right);
}

void int_mod_ui(int_value_t *left, uint32_t right)
{
    mpz_mod_ui(left->num, left->num, right);
}

void int_ui_mod(uint32_t left, int_value_t *right)
{
    mpz_t lint;
    mpz_init_set_ui(lint, left);

    mpz_mod(right->num, lint, right->num);

    mpz_clear(lint);
}

void int_quot_ui(int_value_t *left, uint32_t right)
{
    mpz_tdiv_q_ui(left->num, left->num, right);
}

void int_ui_quot(uint32_t left, int_value_t *right)
{
    mpz_t lint;
    mpz_init_set_ui(lint, left);

    mpz_tdiv_q(right->num, lint, right->num);

    mpz_clear(lint);
}

void int_pow_ui(int_value_t *left, uint32_t right)
{
    mpz_pow_ui(left->num, left->num, right);
}

void int_ui_pow(uint32_t left, int_value_t *right)
{
    mpz_ui_pow_ui(right->num, left, mpz_get_ui(right->num));
}

void int_and_ui(int_value_t *left, uint32_t right)
{
    mpz_t rint;
    mpz_init_set_ui(rint, right);

    mpz_and(left->num, left->num, rint);

    mpz_clear(rint);
}

void int_or_ui(int_value_t *left, uint32_t right)
{
    mpz_t rint;
    mpz_init_set_ui(rint, right);

    mpz_ior(left->num, left->num, rint);

    mpz_clear(rint);
}

void int_xor_ui(int_value_t *left, uint32_t right)
{
    mpz_t rint;
    mpz_init_set_ui(rint, right);

    mpz_xor(left->num, left->num, rint);

    mpz_clear(rint);
}

void int_lshift_ui(int_value_t *left, uint32_t right)
{
    mpz_mul_2exp(left->num, left->num, right);
}

void int_ui_lshift(uint32_t left, int_value_t *right)
{
    mpz_t lint;
    mpz_init_set_ui(lint, left);

    mpz_mul_2exp(right->num, lint, mpz_get_ui(right->num));

    mpz_clear(lint);
}

void int_rshift_ui(int_value_t *left, uint32_t right)
{
    mpz_tdiv_q_2exp(left->num, left->num, right);
}

void int_ui_rshift(uint32_t left, int_value_t *right)
{
    mpz_t lint;
    mpz_init_set_ui(lint, left);

    mpz_tdiv_q_2exp(right->num, lint, mpz_get_ui(right->num));

    mpz_clear(lint);
}

uint8_t int_eq_ui(const int_value_t *left, uint32_t right)
{
    return !mpz_cmp_ui(left->num, right);
}

uint8_t int_lt_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) < 0;
}

uint8_t int_gt_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) > 0;
}

uint8_t int_iszero(const int_value_t *num)
{
    return !mpz_sgn(num->num);
}

uint8_t int_isneg(const int_value_t *num)
{
    return mpz_sgn(num->num) < 0;
}
