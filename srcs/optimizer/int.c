/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/int.h>
#include <alloc.h>

#define int_binary(f)                                            \
    do                                                           \
    {                                                            \
        if (left->ref)                                           \
        {                                                        \
            left->ref--;                                         \
                                                                 \
            if (right->ref)                                      \
            {                                                    \
                right->ref--;                                    \
                                                                 \
                int_value_t *value = int_init();                 \
                f(value->num, INT_CAST(left), INT_CAST(right));  \
                                                                 \
                value_t *res;                                    \
                value_set(res, INT_V, value);                    \
                return res;                                      \
            }                                                    \
                                                                 \
            f(INT_CAST(right), INT_CAST(left), INT_CAST(right)); \
            return right;                                        \
        }                                                        \
                                                                 \
        f(INT_CAST(left), INT_CAST(left), INT_CAST(right));      \
                                                                 \
        value_free_ts(right, int_free);                          \
        return left;                                             \
    } while (0)

#define int_binary2(f)                                                       \
    do                                                                       \
    {                                                                        \
        if (left->ref)                                                       \
        {                                                                    \
            left->ref--;                                                     \
                                                                             \
            if (right->ref)                                                  \
            {                                                                \
                right->ref--;                                                \
                                                                             \
                int_value_t *value = int_init();                             \
                f(value->num, INT_CAST(left), mpz_get_ui(INT_CAST(right)));  \
                                                                             \
                value_t *res;                                                \
                value_set(res, INT_V, value);                                \
                return res;                                                  \
            }                                                                \
                                                                             \
            f(INT_CAST(right), INT_CAST(left), mpz_get_ui(INT_CAST(right))); \
            return right;                                                    \
        }                                                                    \
                                                                             \
        f(INT_CAST(left), INT_CAST(left), mpz_get_ui(INT_CAST(right)));      \
                                                                             \
        value_free_ts(right, int_free);                                      \
        return left;                                                         \
    } while (0)

#define int_binary_ui(f)                          \
    do                                            \
    {                                             \
        if (left->ref)                            \
        {                                         \
            left->ref--;                          \
                                                  \
            int_value_t *value = int_init();      \
            f(value->num, INT_CAST(left), right); \
                                                  \
            value_t *res;                         \
            value_set(res, INT_V, value);         \
            return res;                           \
        }                                         \
                                                  \
        f(INT_CAST(left), INT_CAST(left), right); \
        return left;                              \
    } while (0)

#define int_binary_ui_rev(f)                       \
    do                                             \
    {                                              \
        if (right->ref)                            \
        {                                          \
            right->ref--;                          \
                                                   \
            int_value_t *value = int_init();       \
            f(value->num, left, INT_CAST(right));  \
                                                   \
            value_t *res;                          \
            value_set(res, INT_V, value);          \
            return res;                            \
        }                                          \
                                                   \
        f(INT_CAST(right), left, INT_CAST(right)); \
        return right;                              \
    } while (0)

#define int_binary_ui2(f)                              \
    do                                                 \
    {                                                  \
        if (left->ref)                                 \
        {                                              \
            left->ref--;                               \
                                                       \
            int_value_t *value = int_set_ui(right);    \
            f(value->num, INT_CAST(left), value->num); \
                                                       \
            value_t *res;                              \
            value_set(res, INT_V, value);              \
            return res;                                \
        }                                              \
                                                       \
        mpz_t rint;                                    \
        mpz_init_set_ui(rint, right);                  \
                                                       \
        f(INT_CAST(left), INT_CAST(left), rint);       \
                                                       \
        mpz_clear(rint);                               \
        return left;                                   \
    } while (0)

#define int_binary_ui2_rev(f)                           \
    do                                                  \
    {                                                   \
        if (right->ref)                                 \
        {                                               \
            right->ref--;                               \
                                                        \
            int_value_t *value = int_set_ui(left);      \
            f(value->num, value->num, INT_CAST(right)); \
                                                        \
            value_t *res;                               \
            value_set(res, INT_V, value);               \
            return res;                                 \
        }                                               \
                                                        \
        mpz_t lint;                                     \
        mpz_init_set_ui(lint, left);                    \
                                                        \
        f(INT_CAST(right), lint, INT_CAST(right));      \
                                                        \
        mpz_clear(lint);                                \
        return right;                                   \
    } while (0)

#define int_binary_ui3(f)                                           \
    do                                                              \
    {                                                               \
        if (right->ref)                                             \
        {                                                           \
            right->ref--;                                           \
                                                                    \
            int_value_t *value = int_set_ui(left);                  \
            f(value->num, value->num, mpz_get_ui(INT_CAST(right))); \
                                                                    \
            value_t *res;                                           \
            value_set(res, INT_V, value);                           \
            return res;                                             \
        }                                                           \
                                                                    \
        mpz_t lint;                                                 \
        mpz_init_set_ui(lint, left);                                \
                                                                    \
        f(INT_CAST(right), lint, mpz_get_ui(INT_CAST(right)));      \
                                                                    \
        mpz_clear(lint);                                            \
        return right;                                               \
    } while (0)

#define int_unary(f)                         \
    do                                       \
    {                                        \
        if (num->ref)                        \
        {                                    \
            num->ref--;                      \
                                             \
            int_value_t *value = int_init(); \
            f(value->num, INT_CAST(num));    \
                                             \
            value_t *res;                    \
            value_set(res, INT_V, value);    \
            return res;                      \
        }                                    \
                                             \
        f(INT_CAST(num), INT_CAST(num));     \
        return num;                          \
    } while (0)

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

value_t *int_add(value_t *left, value_t *right)
{
    int_binary(mpz_add);
}

value_t *int_sub(value_t *left, value_t *right)
{
    int_binary(mpz_sub);
}

value_t *int_mul(value_t *left, value_t *right)
{
    int_binary(mpz_mul);
}

value_t *int_mod(value_t *left, value_t *right)
{
    int_binary(mpz_mod);
}

value_t *int_quot(value_t *left, value_t *right)
{
    int_binary(mpz_tdiv_q);
}

value_t *int_pow(value_t *left, value_t *right)
{
    int_binary2(mpz_pow_ui);
}

value_t *int_and(value_t *left, value_t *right)
{
    int_binary(mpz_and);
}

value_t *int_or(value_t *left, value_t *right)
{
    int_binary(mpz_ior);
}

value_t *int_xor(value_t *left, value_t *right)
{
    int_binary(mpz_xor);
}

value_t *int_lshift(value_t *left, value_t *right)
{
    int_binary2(mpz_mul_2exp);
}

value_t *int_rshift(value_t *left, value_t *right)
{
    int_binary2(mpz_tdiv_q_2exp);
}

value_t *int_neg(value_t *num)
{
    int_unary(mpz_neg);
}

value_t *int_not(value_t *num)
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

value_t *int_add_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_add_ui);
}

value_t *int_sub_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_sub_ui);
}

value_t *int_ui_sub(uint32_t left, value_t *right)
{
    int_binary_ui_rev(mpz_ui_sub);
}

value_t *int_mul_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_mul_ui);
}

value_t *int_mod_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_mod_ui);
}

value_t *int_ui_mod(uint32_t left, value_t *right)
{
    int_binary_ui2_rev(mpz_mod);
}

value_t *int_quot_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_tdiv_q_ui);
}

value_t *int_ui_quot(uint32_t left, value_t *right)
{
    int_binary_ui2_rev(mpz_tdiv_q);
}

value_t *int_pow_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_pow_ui);
}

value_t *int_ui_pow(uint32_t left, value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        int_value_t *value = int_init();
        mpz_ui_pow_ui(value->num, left, mpz_get_ui(INT_CAST(right)));

        value_t *res;
        value_set(res, INT_V, value);
        return res;
    }

    mpz_ui_pow_ui(INT_CAST(right), left, mpz_get_ui(INT_CAST(right)));
    return right;
}

value_t *int_and_ui(value_t *left, uint32_t right)
{
    int_binary_ui2(mpz_and);
}

value_t *int_or_ui(value_t *left, uint32_t right)
{
    int_binary_ui2(mpz_ior);
}

value_t *int_xor_ui(value_t *left, uint32_t right)
{
    int_binary_ui2(mpz_xor);
}

value_t *int_lshift_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_mul_2exp);
}

value_t *int_ui_lshift(uint32_t left, value_t *right)
{
    int_binary_ui3(mpz_mul_2exp);
}

value_t *int_rshift_ui(value_t *left, uint32_t right)
{
    int_binary_ui(mpz_tdiv_q_2exp);
}

value_t *int_ui_rshift(uint32_t left, value_t *right)
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
