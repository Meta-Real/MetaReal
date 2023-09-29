/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/float.h>
#include <alloc.h>
#include <string.h>

#define float_binary(f)                                                           \
    do                                                                            \
    {                                                                             \
        if (left->ref)                                                            \
        {                                                                         \
            left->ref--;                                                          \
                                                                                  \
            if (right->ref)                                                       \
            {                                                                     \
                right->ref--;                                                     \
                                                                                  \
                float_value_t *value = float_init();                              \
                f(value->num, FLOAT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);    \
                                                                                  \
                value_t *res;                                                     \
                value_set(res, FLOAT_V, value);                                   \
                return res;                                                       \
            }                                                                     \
                                                                                  \
            f(FLOAT_CAST(right), FLOAT_CAST(left), FLOAT_CAST(right), MPFR_RNDN); \
            return right;                                                         \
        }                                                                         \
                                                                                  \
        f(FLOAT_CAST(left), FLOAT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);      \
                                                                                  \
        value_free_ts(right, float_free);                                         \
        return left;                                                              \
    } while (0)

#define float_binary_int(f)                                                \
    do                                                                     \
    {                                                                      \
        if (left->ref)                                                     \
        {                                                                  \
            left->ref--;                                                   \
                                                                           \
            float_value_t *value = float_init();                           \
            f(value->num, FLOAT_CAST(left), INT_CAST(right), MPFR_RNDN);   \
                                                                           \
            if (right->ref)                                                \
            {                                                              \
                right->ref--;                                              \
                                                                           \
                value_t *res;                                              \
                value_set(res, FLOAT_V, value);                            \
                return res;                                                \
            }                                                              \
                                                                           \
            int_free(right->value);                                        \
            right->type = FLOAT_V;                                         \
            right->value = value;                                          \
            return right;                                                  \
        }                                                                  \
                                                                           \
        f(FLOAT_CAST(left), FLOAT_CAST(left), INT_CAST(right), MPFR_RNDN); \
                                                                           \
        value_free_ts(right, int_free);                                    \
        return left;                                                       \
    } while (0)

#define float_binary_int2(f)                                         \
    do                                                               \
    {                                                                \
        if (right->ref)                                              \
        {                                                            \
            right->ref--;                                            \
                                                                     \
            float_value_t *value = float_set_int(left->value);       \
            f(value->num, value->num, FLOAT_CAST(right), MPFR_RNDN); \
                                                                     \
            if (left->ref)                                           \
            {                                                        \
                left->ref--;                                         \
                                                                     \
                value_t *res;                                        \
                value_set(res, FLOAT_V, value);                      \
                return res;                                          \
            }                                                        \
                                                                     \
            int_free(left->value);                                   \
            left->type = FLOAT_V;                                    \
            left->value = value;                                     \
            return left;                                             \
        }                                                            \
                                                                     \
        mpfr_t lfloat;                                               \
        mpfr_init2(lfloat, 64);                                      \
        mpfr_set_z(lfloat, INT_CAST(left), MPFR_RNDN);               \
                                                                     \
        f(FLOAT_CAST(right), lfloat, FLOAT_CAST(right), MPFR_RNDN);  \
                                                                     \
        mpfr_clear(lfloat);                                          \
        value_free_ts(left, int_free);                               \
        return right;                                                \
    } while (0)

#define float_binary_int3(f)                                   \
    do                                                         \
    {                                                          \
        float_value_t *value = float_set_int(left->value);     \
        f(value->num, value->num, INT_CAST(right), MPFR_RNDN); \
                                                               \
        if (left->ref)                                         \
        {                                                      \
            left->ref--;                                       \
                                                               \
            if (right->ref)                                    \
            {                                                  \
                right->ref--;                                  \
                                                               \
                value_t *res;                                  \
                value_set(res, FLOAT_V, value);                \
                return res;                                    \
            }                                                  \
                                                               \
            int_free(right->value);                            \
            right->type = FLOAT_V;                             \
            right->value = value;                              \
            return right;                                      \
        }                                                      \
                                                               \
        value_free_ts(right, int_free);                        \
        int_free(left->value);                                 \
                                                               \
        left->type = FLOAT_V;                                  \
        left->value = value;                                   \
        return left;                                           \
    } while (0)

#define float_binary_ui(f)                                                         \
    do                                                                             \
    {                                                                              \
        if (left->ref)                                                             \
        {                                                                          \
            left->ref--;                                                           \
                                                                                   \
            float_value_t *value = float_init();                                   \
            f(value->num, FLOAT_CAST(left), (uintptr_t)right->value, MPFR_RNDN);   \
                                                                                   \
            if (right->ref)                                                        \
            {                                                                      \
                right->ref--;                                                      \
                                                                                   \
                value_t *res;                                                      \
                value_set(res, FLOAT_V, value);                                    \
                return res;                                                        \
            }                                                                      \
                                                                                   \
            right->type = FLOAT_V;                                                 \
            right->value = value;                                                  \
            return right;                                                          \
        }                                                                          \
                                                                                   \
        f(FLOAT_CAST(left), FLOAT_CAST(left), (uintptr_t)right->value, MPFR_RNDN); \
                                                                                   \
        value_free_vo(right);                                                      \
        return left;                                                               \
    } while (0)

#define float_binary_ui_rev(f)                                                      \
    do                                                                              \
    {                                                                               \
        if (right->ref)                                                             \
        {                                                                           \
            right->ref--;                                                           \
                                                                                    \
            float_value_t *value = float_init();                                    \
            f(value->num, (uintptr_t)right->value, FLOAT_CAST(left), MPFR_RNDN);    \
                                                                                    \
            if (left->ref)                                                          \
            {                                                                       \
                left->ref--;                                                        \
                                                                                    \
                value_t *res;                                                       \
                value_set(res, FLOAT_V, value);                                     \
                return res;                                                         \
            }                                                                       \
                                                                                    \
            left->type = FLOAT_V;                                                   \
            left->value = value;                                                    \
            return left;                                                            \
        }                                                                           \
                                                                                    \
        f(FLOAT_CAST(right), (uintptr_t)left->value, FLOAT_CAST(right), MPFR_RNDN); \
                                                                                    \
        value_free_vo(left);                                                        \
        return right;                                                               \
    } while (0)

#define float_binary_ui2(f)                                          \
    do                                                               \
    {                                                                \
        float_value_t *value = float_set_ui((uintptr_t)left->value); \
        f(value->num, value->num, INT_CAST(right), MPFR_RNDN);       \
                                                                     \
        if (right->ref)                                              \
        {                                                            \
            right->ref--;                                            \
                                                                     \
            if (left->ref)                                           \
            {                                                        \
                left->ref--;                                         \
                                                                     \
                value_t *res;                                        \
                value_set(res, FLOAT_V, value);                      \
                return res;                                          \
            }                                                        \
                                                                     \
            left->type = FLOAT_V;                                    \
            left->value = value;                                     \
            return left;                                             \
        }                                                            \
                                                                     \
        int_free(right->value);                                      \
        right->type = FLOAT_V;                                       \
        right->value = value;                                        \
                                                                     \
        value_free_vo(left);                                         \
        return right;                                                \
    } while (0)

#define float_unary(f)                                     \
    do                                                     \
    {                                                      \
        if (num->ref)                                      \
        {                                                  \
            num->ref--;                                    \
                                                           \
            float_value_t *value = float_init();           \
            f(value->num, FLOAT_CAST(num), 1, MPFR_RNDN);  \
                                                           \
            value_t *res;                                  \
            value_set(res, FLOAT_V, value);                \
            return res;                                    \
        }                                                  \
                                                           \
        f(FLOAT_CAST(num), FLOAT_CAST(num), 1, MPFR_RNDN); \
        return num;                                        \
    } while (0)

float_value_t *float_init()
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    return num;
}

float_value_t *float_set_str(const char *str)
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    mpfr_set_str(num->num, str, 10, MPFR_RNDN);
    return num;
}

float_value_t *float_set_int(const int_value_t *num)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_z(res->num, num->num, MPFR_RNDN);
    return res;
}

float_value_t *float_set_ui(uint32_t ui)
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    mpfr_set_ui(num->num, ui, MPFR_RNDN);
    return num;
}

void float_free(float_value_t *num)
{
    mpfr_clear(num->num);
    mr_free(num);
}

char *float_get_str(const float_value_t *num)
{
    char* res = mr_alloc(100);
    mpfr_sprintf(res, "%.16Rg", num->num);

    res = mr_realloc(res, strlen(res) + 1);
    return res;
}

int_value_t *float_get_int(const float_value_t *num)
{
    int_value_t *res = mr_alloc(sizeof(int_value_t));
    mpz_init(res->num);
    mpfr_get_z(res->num, num->num, MPFR_RNDZ);
    return res;
}

void float_print(const float_value_t *num)
{
    mpfr_printf("%.16Rg", num->num);
}

value_t *float_add(value_t *left, value_t *right)
{
    float_binary(mpfr_add);
}

value_t *float_sub(value_t *left, value_t *right)
{
    float_binary(mpfr_sub);
}

value_t *float_mul(value_t *left, value_t *right)
{
    float_binary(mpfr_mul);
}

value_t *float_div(value_t *left, value_t *right)
{
    float_binary(mpfr_div);
}

value_t *float_mod(value_t *left, value_t *right)
{
    float_binary(mpfr_remainder);
}

value_t *float_quot(value_t *left, value_t *right)
{
    int_value_t *value = int_init();

    if (left->ref)
    {
        left->ref--;

        if (right->ref)
        {
            right->ref--;

            mpfr_t tmp;
            mpfr_init2(tmp, 64);
            mpfr_div(tmp, FLOAT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);
            mpfr_get_z(value->num, tmp, MPFR_RNDZ);

            value_t *res;
            value_set(res, INT_V, value);

            mpfr_clear(tmp);
            return res;
        }

        mpfr_div(FLOAT_CAST(right), FLOAT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);
        mpfr_get_z(value->num, FLOAT_CAST(right), MPFR_RNDZ);

        float_free(right->value);
        right->type = INT_V;
        right->value = value;
        return right;
    }

    mpfr_div(FLOAT_CAST(left), FLOAT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);
    mpfr_get_z(value->num, FLOAT_CAST(left), MPFR_RNDZ);

    value_free_ts(right, float_free);
    float_free(left->value);
    left->type = INT_V;
    left->value = value;
    return left;
}

value_t *float_pow(value_t *left, value_t *right)
{
    float_binary(mpfr_pow);
}

value_t *float_neg(value_t *num)
{
    if (num->ref)
    {
        num->ref--;

        float_value_t *value = float_init();
        mpfr_neg(value->num, FLOAT_CAST(num), MPFR_RNDN);

        value_t *res;
        value_set(res, FLOAT_V, value);
        return res;
    }

    mpfr_neg(FLOAT_CAST(num), FLOAT_CAST(num), MPFR_RNDN);
    return num;
}

value_t *float_inc(value_t *num)
{
    float_unary(mpfr_add_ui);
}

value_t *float_dec(value_t *num)
{
    float_unary(mpfr_sub_ui);
}

uint8_t float_eq(const float_value_t *left, const float_value_t *right)
{
    return mpfr_equal_p(left->num, right->num) != 0;
}

uint8_t float_neq(const float_value_t *left, const float_value_t *right)
{
    return !mpfr_equal_p(left->num, right->num);
}

uint8_t float_lt(const float_value_t *left, const float_value_t *right)
{
    return mpfr_less_p(left->num, right->num) != 0;
}

uint8_t float_gt(const float_value_t *left, const float_value_t *right)
{
    return mpfr_greater_p(left->num, right->num) != 0;
}

uint8_t float_lte(const float_value_t *left, const float_value_t *right)
{
    return mpfr_lessequal_p(left->num, right->num) != 0;
}

uint8_t float_gte(const float_value_t *left, const float_value_t *right)
{
    return mpfr_greaterequal_p(left->num, right->num) != 0;
}

value_t *float_add_int(value_t *left, value_t *right)
{
    float_binary_int(mpfr_add_z);
}

value_t *float_sub_int(value_t *left, value_t *right)
{
    float_binary_int(mpfr_sub_z);
}

value_t *float_int_sub(value_t *left, value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        float_value_t *value = float_init();
        mpfr_z_sub(value->num, INT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);

        if (left->ref)
        {
            left->ref--;

            value_t *res;
            value_set(res, FLOAT_V, value);
            return res;
        }

        int_free(left->value);
        left->type = FLOAT_V;
        left->value = value;
        return left;
    }

    mpfr_z_sub(FLOAT_CAST(right), INT_CAST(left), FLOAT_CAST(right), MPFR_RNDN);

    value_free_ts(left, int_free);
    return right;
}

value_t *float_mul_int(value_t *left, value_t *right)
{
    float_binary_int(mpfr_mul_z);
}

value_t *float_div_int(value_t *left, value_t *right)
{
    float_binary_int(mpfr_div_z);
}

value_t *float_int_div(value_t *left, value_t *right)
{
    float_binary_int2(mpfr_div);
}

value_t *float_int_div_int(value_t *left, value_t *right)
{
    float_binary_int3(mpfr_div_z);
}

value_t *float_mod_int(value_t *left, value_t *right)
{
    if (left->ref)
    {
        left->ref--;

        float_value_t *value = float_set_int(right->value);
        mpfr_remainder(value->num, FLOAT_CAST(left), value->num, MPFR_RNDN);

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, FLOAT_V, value);
            return res;
        }

        int_free(right->value);
        right->type = FLOAT_V;
        right->value = value;
        return right;
    }

    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_z(rfloat, INT_CAST(right), MPFR_RNDN);

    mpfr_remainder(FLOAT_CAST(left), FLOAT_CAST(left), rfloat, MPFR_RNDN);

    mpfr_clear(rfloat);
    value_free_ts(right, int_free);
    return left;
}

value_t *float_int_mod(value_t *left, value_t *right)
{
    float_binary_int2(mpfr_remainder);
}

value_t *float_quot_int(value_t *left, value_t *right)
{
    if (left->ref)
    {
        left->ref--;

        mpfr_t tmp;
        mpfr_init2(tmp, 64);
        mpfr_div_z(tmp, FLOAT_CAST(left), INT_CAST(right), MPFR_RNDN);

        if (right->ref)
        {
            right->ref--;

            int_value_t *value = int_init();
            mpfr_get_z(value->num, tmp, MPFR_RNDZ);

            value_t *res;
            value_set(res, INT_V, value);

            mpfr_clear(tmp);
            return res;
        }

        mpfr_get_z(INT_CAST(right), tmp, MPFR_RNDZ);

        mpfr_clear(tmp);
        return right;
    }

    mpfr_div_z(FLOAT_CAST(left), FLOAT_CAST(left), INT_CAST(right), MPFR_RNDN);

    if (right->ref)
    {
        right->ref--;

        int_value_t *value = int_init();
        mpfr_get_z(value->num, FLOAT_CAST(left), MPFR_RNDZ);

        float_free(left->value);
        left->type = INT_V;
        left->value = value;
        return left;
    }

    mpfr_get_z(INT_CAST(right), FLOAT_CAST(left), MPFR_RNDZ);

    float_free(left->value);
    mr_free(left);
    return right;
}

value_t *float_int_quot(value_t *left, value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, INT_CAST(left), MPFR_RNDN);

    mpfr_div(lfloat, lfloat, FLOAT_CAST(right), MPFR_RNDN);

    if (left->ref)
    {
        left->ref--;

        int_value_t *value = int_init();
        mpfr_get_z(value->num, lfloat, MPFR_RNDZ);

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, INT_V, value);

            mpfr_clear(lfloat);
            return res;
        }

        mpfr_clear(lfloat);
        float_free(right->value);

        right->type = INT_V;
        right->value = value;
        return right;
    }

    mpfr_get_z(INT_CAST(left), lfloat, MPFR_RNDZ);

    mpfr_clear(lfloat);
    value_free_ts(right, float_free);
    return left;
}

value_t *float_pow_int(value_t *left, value_t *right)
{
    float_binary_int(mpfr_pow_z);
}

value_t *float_int_pow(value_t *left, value_t *right)
{
    float_binary_int2(mpfr_pow);
}

value_t *float_int_pow_int(value_t *left, value_t *right)
{
    float_binary_int3(mpfr_pow_z);
}

uint8_t float_eq_int(const float_value_t *left, const int_value_t *right)
{
    return !mpfr_cmp_z(left->num, right->num);
}

uint8_t float_neq_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) != 0;
}

uint8_t float_lt_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) < 0;
}

uint8_t float_gt_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) > 0;
}

uint8_t float_lte_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) <= 0;
}

uint8_t float_gte_int(const float_value_t *left, const int_value_t *right)
{
    return mpfr_cmp_z(left->num, right->num) >= 0;
}

value_t *float_add_ui(value_t *left, value_t *right)
{
    float_binary_ui(mpfr_add_ui);
}

value_t *float_sub_ui(value_t *left, value_t *right)
{
    float_binary_ui(mpfr_sub_ui);
}

value_t *float_ui_sub(value_t *left, value_t *right)
{
    float_binary_ui_rev(mpfr_ui_sub);
}

value_t *float_mul_ui(value_t *left, value_t *right)
{
    float_binary_ui(mpfr_mul_ui);
}

value_t *float_div_ui(value_t *left, value_t *right)
{
    float_binary_ui(mpfr_div_ui);
}

value_t *float_ui_div(value_t *left, value_t *right)
{
    float_binary_ui_rev(mpfr_ui_div);
}

value_t *float_int_div_ui(value_t *left, value_t *right)
{
    float_value_t *value = float_set_int(left->value);
    mpfr_div_ui(value->num, value->num, (uintptr_t)right->value, MPFR_RNDN);

    if (left->ref)
    {
        left->ref--;

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, FLOAT_V, value);
            return res;
        }

        right->type = FLOAT_V;
        right->value = value;
        return right;
    }

    int_free(left->value);
    left->type = FLOAT_V;
    left->value = value;

    value_free_vo(right);
    return left;
}

value_t *float_ui_div_int(value_t *left, value_t *right)
{
    float_binary_ui2(mpfr_div_z);
}

value_t *float_ui_div_ui(value_t *left, value_t *right)
{
    float_value_t *value = float_set_ui((uintptr_t)left->value);
    mpfr_div_ui(value->num, value->num, (uintptr_t)right->value, MPFR_RNDN);

    if (left->ref)
    {
        left->ref--;

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, FLOAT_V, value);
            return res;
        }

        right->type = FLOAT_V;
        right->value = value;
        return right;
    }

    left->type = FLOAT_V;
    left->value = value;

    value_free_vo(right);
    return left;
}

value_t *float_mod_ui(value_t *left, value_t *right)
{
    if (left->ref)
    {
        left->ref--;

        float_value_t *value = float_set_ui((uintptr_t)right->value);
        mpfr_remainder(value->num, FLOAT_CAST(left), value->num, MPFR_RNDN);

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, FLOAT_V, value);
            return res;
        }

        right->type = FLOAT_V;
        right->value = value;
        return right;
    }

    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_ui(rfloat, (uintptr_t)right->value, MPFR_RNDN);

    mpfr_remainder(FLOAT_CAST(left), FLOAT_CAST(left), rfloat, MPFR_RNDN);

    mpfr_clear(rfloat);
    value_free_vo(right);
    return left;
}

value_t *float_ui_mod(value_t *left, value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        float_value_t *value = float_set_ui((uintptr_t)left->value);
        mpfr_remainder(value->num, value->num, FLOAT_CAST(right), MPFR_RNDN);

        if (left->ref)
        {
            left->ref--;

            value_t *res;
            value_set(res, FLOAT_V, value);
            return res;
        }

        left->type = FLOAT_V;
        left->value = value;
        return left;
    }

    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_ui(lfloat, (uintptr_t)left->value, MPFR_RNDN);

    mpfr_remainder(FLOAT_CAST(right), lfloat, FLOAT_CAST(right), MPFR_RNDN);

    mpfr_clear(lfloat);
    value_free_vo(left);
    return right;
}

value_t *float_quot_ui(value_t *left, value_t *right)
{
    int_value_t *value = int_init();

    if (left->ref)
    {
        left->ref--;

        mpfr_t tmp;
        mpfr_init2(tmp, 64);
        mpfr_div_ui(tmp, FLOAT_CAST(left), (uintptr_t)right->value, MPFR_RNDN);
        mpfr_get_z(value->num, tmp, MPFR_RNDZ);

        if (right->ref)
        {
            right->ref--;

            value_t *res;
            value_set(res, INT_V, value);

            mpfr_clear(tmp);
            return res;
        }

        right->type = INT_V;
        right->value = value;

        mpfr_clear(tmp);
        return right;
    }

    mpfr_div_ui(FLOAT_CAST(left), FLOAT_CAST(left), (uintptr_t)right->value, MPFR_RNDN);
    mpfr_get_z(value->num, FLOAT_CAST(left), MPFR_RNDZ);

    float_free(left->value);
    left->type = INT_V;
    left->value = value;

    value_free_vo(right);
    return left;
}

value_t *float_ui_quot(value_t *left, value_t *right)
{
    int_value_t *value = int_init();

    if (right->ref)
    {
        right->ref--;

        mpfr_t tmp;
        mpfr_init2(tmp, 64);
        mpfr_ui_div(tmp, (uintptr_t)left->value, FLOAT_CAST(right), MPFR_RNDN);
        mpfr_get_z(value->num, tmp, MPFR_RNDZ);

        if (left->ref)
        {
            left->ref--;

            value_t *res;
            value_set(res, INT_V, value);

            mpfr_clear(tmp);
            return res;
        }

        left->type = INT_V;
        left->value = value;

        mpfr_clear(tmp);
        return left;
    }

    mpfr_ui_div(FLOAT_CAST(right), (uintptr_t)left->value, FLOAT_CAST(right), MPFR_RNDN);
    mpfr_get_z(value->num, FLOAT_CAST(right), MPFR_RNDZ);

    float_free(right->value);
    right->type = INT_V;
    right->value = value;

    value_free_vo(left);
    return right;
}

value_t *float_pow_ui(value_t *left, value_t *right)
{
    float_binary_ui(mpfr_pow_ui);
}

value_t *float_ui_pow(value_t *left, value_t *right)
{
    float_binary_ui_rev(mpfr_ui_pow);
}

value_t *float_ui_pow_int(value_t *left, value_t *right)
{
    float_binary_ui2(mpfr_pow_z);
}

uint8_t float_eq_ui(const float_value_t *left, uint32_t right)
{
    return !mpfr_cmp_ui(left->num, right);
}

uint8_t float_neq_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) != 0;
}

uint8_t float_lt_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) < 0;
}

uint8_t float_gt_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) > 0;
}

uint8_t float_lte_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) <= 0;
}

uint8_t float_gte_ui(const float_value_t *left, uint32_t right)
{
    return mpfr_cmp_ui(left->num, right) >= 0;
}

uint8_t float_iszero(const float_value_t *num)
{
    return mpfr_zero_p(num->num);
}

uint8_t float_isnzero(const float_value_t *num)
{
    return !mpfr_zero_p(num->num);
}

uint8_t float_isneg(const float_value_t *num)
{
    return mpfr_sgn(num->num) < 0;
}
