/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/int.h>
#include <alloc.h>

#define int_binary(f)                               \
    do                                              \
    {                                               \
        if (left->ref)                              \
        {                                           \
            left->ref--;                            \
                                                    \
            if (right->ref)                         \
            {                                       \
                right->ref--;                       \
                                                    \
                int_value_t *res = int_init();      \
                f(res->num, left->num, right->num); \
                return res;                         \
            }                                       \
                                                    \
            f(right->num, left->num, right->num);   \
            return right;                           \
        }                                           \
                                                    \
        f(left->num, left->num, right->num);        \
                                                    \
        int_free(right);                            \
        return left;                                \
    } while (0)

#define int_binary2(f)                                          \
    do                                                          \
    {                                                           \
        if (left->ref)                                          \
        {                                                       \
            left->ref--;                                        \
                                                                \
            if (right->ref)                                     \
            {                                                   \
                right->ref--;                                   \
                                                                \
                int_value_t *res = int_init();                  \
                f(res->num, left->num, mpz_get_ui(right->num)); \
                return res;                                     \
            }                                                   \
                                                                \
            f(right->num, left->num, mpz_get_ui(right->num));   \
            return right;                                       \
        }                                                       \
                                                                \
        f(left->num, left->num, mpz_get_ui(right->num));        \
                                                                \
        int_free(right);                                        \
        return left;                                            \
    } while (0)

#define int_binary_ui(f)                   \
    do                                     \
    {                                      \
        if (left->ref)                     \
        {                                  \
            left->ref--;                   \
                                           \
            int_value_t *res = int_init(); \
            f(res->num, left->num, right); \
            return res;                    \
        }                                  \
                                           \
        f(left->num, left->num, right);    \
        return left;                       \
    } while (0)

#define int_binary_ui_rev(f)               \
    do                                     \
    {                                      \
        if (right->ref)                    \
        {                                  \
            right->ref--;                  \
                                           \
            int_value_t *res = int_init(); \
            f(res->num, left, right->num); \
            return res;                    \
        }                                  \
                                           \
        f(right->num, left, right->num);   \
        return right;                      \
    } while (0)

#define int_binary_ui2(f)                         \
    do                                            \
    {                                             \
        if (left->ref)                            \
        {                                         \
            left->ref--;                          \
                                                  \
            int_value_t *res = int_set_ui(right); \
            f(res->num, left->num, res->num);     \
            return res;                           \
        }                                         \
                                                  \
        mpz_t rint;                               \
        mpz_init(rint);                           \
                                                  \
        f(left->num, left->num, rint);            \
                                                  \
        mpz_clear(rint);                          \
        return left;                              \
    } while (0)

#define int_binary_ui2_rev(f)                    \
    do                                           \
    {                                            \
        if (right->ref)                          \
        {                                        \
            right->ref--;                        \
                                                 \
            int_value_t *res = int_set_ui(left); \
            f(res->num, res->num, right->num);   \
            return res;                          \
        }                                        \
                                                 \
        mpz_t lint;                              \
        mpz_init(lint);                          \
                                                 \
        f(right->num, lint, right->num);         \
                                                 \
        mpz_clear(lint);                         \
        return right;                            \
    } while (0)

#define int_binary_ui3(f)                                  \
    do                                                     \
    {                                                      \
        if (right->ref)                                    \
        {                                                  \
            right->ref--;                                  \
                                                           \
            int_value_t *res = int_set_ui(left);           \
            f(res->num, res->num, mpz_get_ui(right->num)); \
            return res;                                    \
        }                                                  \
                                                           \
        mpz_t lint;                                        \
        mpz_init(lint);                                    \
                                                           \
        f(right->num, lint, mpz_get_ui(right->num));       \
                                                           \
        mpz_clear(lint);                                   \
        return right;                                      \
    } while (0)

#define int_unary(f)                       \
    do                                     \
    {                                      \
        if (num->ref)                      \
        {                                  \
            num->ref--;                    \
                                           \
            int_value_t *res = int_init(); \
            mpz_neg(res->num, num->num);   \
            return res;                    \
        }                                  \
                                           \
        mpz_neg(num->num, num->num);       \
        return num;                        \
    } while (0)

int_value_t *int_init()
{
    int_value_t *num = mr_alloc(sizeof(int_value_t));
    mpz_init(num->num);
    num->ref = 0;
    return num;
}

int_value_t *int_set_str(const char *str)
{
    int_value_t *num = mr_alloc(sizeof(int_value_t));
    mpz_init_set_str(num->num, str, 10);
    num->ref = 0;
    return num;
}

int_value_t *int_set_ui(uint32_t ui)
{
    int_value_t *num = mr_alloc(sizeof(int_value_t));
    mpz_init_set_ui(num->num, ui);
    num->ref = 0;
    return num;
}

void int_free(int_value_t *num)
{
    if (num->ref)
    {
        num->ref--;
        return;
    }

    mpz_clear(num->num);
    mr_free(num);
}

char *int_get_str(const int_value_t *num)
{
    return mpz_get_str(NULL, 10, num->num);
}

uint64_t int_get_ull(const int_value_t *num)
{
    uint64_t dst = 0;
    mpz_export(&dst, 0, -1, sizeof(uint64_t), 0, 0, num->num);
    return dst;
}

void int_print(const int_value_t *num)
{
    gmp_printf("%Zd", num->num);
}

int_value_t *int_add(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_add);
}

int_value_t *int_sub(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_sub);
}

int_value_t *int_mul(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_mul);
}

int_value_t *int_mod(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_mod);
}

int_value_t *int_quot(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_tdiv_q);
}

int_value_t *int_pow(int_value_t *left, int_value_t *right)
{
    int_binary2(mpz_pow_ui);
}

int_value_t *int_and(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_and);
}

int_value_t *int_or(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_ior);
}

int_value_t *int_xor(int_value_t *left, int_value_t *right)
{
    int_binary(mpz_xor);
}

int_value_t *int_lshift(int_value_t *left, int_value_t *right)
{
    int_binary2(mpz_mul_2exp);
}

int_value_t *int_rshift(int_value_t *left, int_value_t *right)
{
    int_binary2(mpz_tdiv_q_2exp);
}

int_value_t *int_neg(int_value_t *num)
{
    int_unary(mpz_neg);
}

int_value_t *int_not(int_value_t *num)
{
    int_unary(mpz_com);
}

uint8_t int_eq(const int_value_t *left, const int_value_t *right)
{
    return !mpz_cmp(left->num, right->num);
}

uint8_t int_neq(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) != 0;
}

uint8_t int_lt(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) < 0;
}

uint8_t int_gt(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) > 0;
}

uint8_t int_lte(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) <= 0;
}

uint8_t int_gte(const int_value_t *left, const int_value_t *right)
{
    return mpz_cmp(left->num, right->num) >= 0;
}

int_value_t *int_add_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_add_ui);
}

int_value_t *int_sub_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_sub_ui);
}

int_value_t *int_ui_sub(uint32_t left, int_value_t *right)
{
    int_binary_ui_rev(mpz_ui_sub);
}

int_value_t *int_mul_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_mul_ui);
}

int_value_t *int_mod_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_mod_ui);
}

int_value_t *int_ui_mod(uint32_t left, int_value_t *right)
{
    int_binary_ui2_rev(mpz_mod);
}

int_value_t *int_quot_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_tdiv_q_ui);
}

int_value_t *int_ui_quot(uint32_t left, int_value_t *right)
{
    int_binary_ui2_rev(mpz_tdiv_q);
}

int_value_t *int_pow_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_pow_ui);
}

int_value_t *int_ui_pow(uint32_t left, int_value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        int_value_t *res = int_init();
        mpz_ui_pow_ui(res->num, left, mpz_get_ui(right->num));
        return res;
    }

    mpz_ui_pow_ui(right->num, left, mpz_get_ui(right->num));
    return right;
}

int_value_t *int_and_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui2(mpz_and);
}

int_value_t *int_or_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui2(mpz_ior);
}

int_value_t *int_xor_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui2(mpz_xor);
}

int_value_t *int_lshift_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_mul_2exp);
}

int_value_t *int_ui_lshift(uint32_t left, int_value_t *right)
{
    int_binary_ui3(mpz_mul_2exp);
}

int_value_t *int_rshift_ui(int_value_t *left, uint32_t right)
{
    int_binary_ui(mpz_tdiv_q_2exp);
}

int_value_t *int_ui_rshift(uint32_t left, int_value_t *right)
{
    int_binary_ui3(mpz_tdiv_q_2exp);
}

uint8_t int_eq_ui(const int_value_t *left, uint32_t right)
{
    return !mpz_cmp_ui(left->num, right);
}

uint8_t int_neq_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) != 0;
}

uint8_t int_lt_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) < 0;
}

uint8_t int_gt_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) > 0;
}

uint8_t int_lte_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) <= 0;
}

uint8_t int_gte_ui(const int_value_t *left, uint32_t right)
{
    return mpz_cmp_ui(left->num, right) >= 0;
}

uint8_t int_iszero(const int_value_t *num)
{
    return !mpz_sgn(num->num);
}

uint8_t int_isnzero(const int_value_t *num)
{
    return mpz_sgn(num->num) != 0;
}

uint8_t int_isneg(const int_value_t *num)
{
    return mpz_sgn(num->num) < 0;
}

uint8_t int_gt_ull(const int_value_t *left, uint64_t right)
{
    if (mpz_size(left->num) > 1)
        return 1;

    return mpz_getlimbn(left->num, 0) > right;
}

uint8_t int_nfit_ull(const int_value_t *num)
{
    return mpz_size(num->num) > 1;
}
