/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/float.h>
#include <alloc.h>
#include <string.h>

#define float_binary(f)                                        \
    do                                                         \
    {                                                          \
        if (left->ref)                                         \
        {                                                      \
            left->ref--;                                       \
                                                               \
            if (right->ref)                                    \
            {                                                  \
                right->ref--;                                  \
                                                               \
                float_value_t *res = float_init();             \
                f(res->num, left->num, right->num, MPFR_RNDN); \
                return res;                                    \
            }                                                  \
                                                               \
            f(right->num, left->num, right->num, MPFR_RNDN);   \
            return right;                                      \
        }                                                      \
                                                               \
        f(left->num, left->num, right->num, MPFR_RNDN);        \
                                                               \
        float_free(right);                                     \
        return left;                                           \
    } while (0)

#define float_binary_int(f)                                \
    do                                                     \
    {                                                      \
        if (left->ref)                                     \
        {                                                  \
            left->ref--;                                   \
                                                           \
            float_value_t *res = float_init();             \
            f(res->num, left->num, right->num, MPFR_RNDN); \
                                                           \
            int_free(right);                               \
            return res;                                    \
        }                                                  \
                                                           \
        f(left->num, left->num, right->num, MPFR_RNDN);    \
                                                           \
        int_free(right);                                   \
        return left;                                       \
    } while (0)

#define float_binary_int2(f)                                          \
    do                                                                \
    {                                                                 \
        if (left->ref)                                                \
        {                                                             \
            left->ref--;                                              \
                                                                      \
            float_value_t *res = float_set_int(right);                \
            mpfr_remainder(res->num, left->num, res->num, MPFR_RNDN); \
                                                                      \
            int_free(right);                                          \
            return res;                                               \
        }                                                             \
                                                                      \
        mpfr_t rfloat;                                                \
        mpfr_init2(rfloat, 64);                                       \
        mpfr_set_z(rfloat, right->num, MPFR_RNDN);                    \
                                                                      \
        mpfr_remainder(left->num, left->num, rfloat, MPFR_RNDN);      \
                                                                      \
        mpfr_clear(rfloat);                                           \
        int_free(right);                                              \
        return left;                                                  \
    } while (0)

#define float_binary_int2_rev(f)                                       \
    do                                                                 \
    {                                                                  \
        if (right->ref)                                                \
        {                                                              \
            right->ref--;                                              \
                                                                       \
            float_value_t *res = float_set_int(left);                  \
            mpfr_remainder(res->num, res->num, right->num, MPFR_RNDN); \
                                                                       \
            int_free(left);                                            \
            return res;                                                \
        }                                                              \
                                                                       \
        mpfr_t lfloat;                                                 \
        mpfr_init2(lfloat, 64);                                        \
        mpfr_set_z(lfloat, left->num, MPFR_RNDN);                      \
                                                                       \
        mpfr_remainder(right->num, lfloat, right->num, MPFR_RNDN);     \
                                                                       \
        mpfr_clear(lfloat);                                            \
        int_free(left);                                                \
        return right;                                                  \
    } while (0)

#define float_binary_ui(f)                            \
    do                                                \
    {                                                 \
        if (left->ref)                                \
        {                                             \
            left->ref--;                              \
                                                      \
            float_value_t *res = float_init();        \
            f(res->num, left->num, right, MPFR_RNDN); \
            return res;                               \
        }                                             \
                                                      \
        f(left->num, left->num, right, MPFR_RNDN);    \
        return left;                                  \
    } while (0)

#define float_binary_ui_rev(f)                        \
    do                                                \
    {                                                 \
        if (right->ref)                               \
        {                                             \
            right->ref--;                             \
                                                      \
            float_value_t *res = float_init();        \
            f(res->num, left, right->num, MPFR_RNDN); \
            return res;                               \
        }                                             \
                                                      \
        f(right->num, left, right->num, MPFR_RNDN);   \
        return right;                                 \
    } while (0)

float_value_t *float_init()
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    num->ref = 0;
    return num;
}

float_value_t *float_set_str(const char *str)
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    mpfr_set_str(num->num, str, 10, MPFR_RNDN);
    num->ref = 0;
    return num;
}

float_value_t *float_set_int(const int_value_t *num)
{
    float_value_t *res = mr_alloc(sizeof(float_value_t));
    mpfr_init2(res->num, 64);
    mpfr_set_z(res->num, num->num, MPFR_RNDN);
    res->ref = 0;
    return res;
}

float_value_t *float_set_ui(uint32_t ui)
{
    float_value_t *num = mr_alloc(sizeof(float_value_t));
    mpfr_init2(num->num, 64);
    mpfr_set_ui(num->num, ui, MPFR_RNDN);
    num->ref = 0;
    return num;
}

void float_free(float_value_t *num)
{
    if (num->ref)
    {
        num->ref--;
        return;
    }

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

float_value_t *float_add(float_value_t *left, float_value_t *right)
{
    float_binary(mpfr_add);
}

float_value_t *float_sub(float_value_t *left, float_value_t *right)
{
    float_binary(mpfr_sub);
}

float_value_t *float_mul(float_value_t *left, float_value_t *right)
{
    float_binary(mpfr_mul);
}

float_value_t *float_div(float_value_t *left, float_value_t *right)
{
    float_binary(mpfr_div);
}

float_value_t *float_mod(float_value_t *left, float_value_t *right)
{
    float_binary(mpfr_remainder);
}

int_value_t *float_quot(float_value_t *left, float_value_t *right)
{
    int_value_t *res = int_init();

    if (left->ref)
    {
        left->ref--;

        if (right->ref)
        {
            right->ref--;

            mpfr_t tmp;
            mpfr_init2(tmp, 64);
            mpfr_div(tmp, left->num, right->num, MPFR_RNDN);
            mpfr_get_z(res->num, tmp, MPFR_RNDZ);

            mpfr_clear(tmp);
            return res;
        }

        mpfr_div(right->num, left->num, right->num, MPFR_RNDN);
        mpfr_get_z(res->num, right->num, MPFR_RNDZ);

        mpfr_clear(right->num);
        mr_free(right);
        return res;
    }

    mpfr_div(left->num, left->num, right->num, MPFR_RNDN);
    mpfr_get_z(res->num, left->num, MPFR_RNDZ);

    float_free(right);
    mpfr_clear(left->num);
    mr_free(left);
    return res;
}

float_value_t *float_pow(float_value_t *left, float_value_t *right)
{
    float_binary(mpfr_pow);
}

float_value_t *float_neg(float_value_t *num)
{
    if (num->ref)
    {
        num->ref--;

        float_value_t *res = float_init();
        mpfr_neg(res->num, num->num, MPFR_RNDN);
        return res;
    }

    mpfr_neg(num->num, num->num, MPFR_RNDN);
    return num;
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

float_value_t *float_add_int(float_value_t *left, int_value_t *right)
{
    float_binary_int(mpfr_add_z);
}

float_value_t *float_sub_int(float_value_t *left, int_value_t *right)
{
    float_binary_int(mpfr_sub_z);
}

float_value_t *float_int_sub(int_value_t *left, float_value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        float_value_t *res = float_init();
        mpfr_z_sub(res->num, left->num, right->num, MPFR_RNDN);

        int_free(left);
        return res;
    }

    mpfr_z_sub(right->num, left->num, right->num, MPFR_RNDN);

    int_free(left);
    return right;
}

float_value_t *float_mul_int(float_value_t *left, int_value_t *right)
{
    float_binary_int(mpfr_mul_z);
}

float_value_t *float_div_int(float_value_t *left, int_value_t *right)
{
    float_binary_int(mpfr_div_z);
}

float_value_t *float_int_div(int_value_t *left, float_value_t *right)
{
    float_binary_int2_rev(mpfr_div);
}

float_value_t *float_int_div_int(int_value_t *left, int_value_t *right)
{
    float_value_t *res = float_set_int(left);
    mpfr_div_z(res->num, res->num, right->num, MPFR_RNDN);

    int_free(right);
    int_free(left);
    return res;
}

float_value_t *float_mod_int(float_value_t *left, int_value_t *right)
{
    float_binary_int2(mpfr_remainder);
}

float_value_t *float_int_mod(int_value_t *left, float_value_t *right)
{
    float_binary_int2_rev(mpfr_remainder);
}

int_value_t *float_quot_int(float_value_t *left, int_value_t *right)
{
    if (left->ref)
    {
        left->ref--;

        mpfr_t tmp;
        mpfr_init2(tmp, 64);
        mpfr_div_z(tmp, left->num, right->num, MPFR_RNDN);

        if (right->ref)
        {
            right->ref--;

            int_value_t *res = int_init();
            mpfr_get_z(res->num, tmp, MPFR_RNDZ);
            return res;
        }

        mpfr_get_z(right->num, tmp, MPFR_RNDZ);
        return right;
    }

    mpfr_div_z(left->num, left->num, right->num, MPFR_RNDN);

    if (right->ref)
    {
        right->ref--;

        int_value_t *res = int_init();
        mpfr_get_z(res->num, left->num, MPFR_RNDZ);

        mpfr_clear(left->num);
        mr_free(left);
        return res;
    }

    mpfr_get_z(right->num, left->num, MPFR_RNDZ);

    mpfr_clear(left->num);
    mr_free(left);
    return right;
}

int_value_t *float_int_quot(int_value_t *left, float_value_t *right)
{
    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_z(lfloat, left->num, MPFR_RNDN);

    mpfr_div(lfloat, lfloat, right->num, MPFR_RNDN);

    if (left->ref)
    {
        left->ref--;

        int_value_t *res = int_init();
        mpfr_get_z(res->num, lfloat, MPFR_RNDZ);

        mpfr_clear(lfloat);
        float_free(right);
        return res;
    }

    mpfr_get_z(left->num, lfloat, MPFR_RNDZ);

    mpfr_clear(lfloat);
    float_free(right);
    return left;
}

float_value_t *float_pow_int(float_value_t *left, int_value_t *right)
{
    float_binary_int(mpfr_pow_z);
}

float_value_t *float_int_pow(int_value_t *left, float_value_t *right)
{
    float_binary_int2_rev(mpfr_pow);
}

float_value_t *float_int_pow_int(int_value_t *left, int_value_t *right)
{
    float_value_t *res = float_set_int(left);
    mpfr_pow_z(res->num, res->num, right->num, MPFR_RNDN);

    int_free(right);
    int_free(left);
    return res;
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

float_value_t *float_add_ui(float_value_t *left, uint32_t right)
{
    float_binary_ui(mpfr_add_ui);
}

float_value_t *float_sub_ui(float_value_t *left, uint32_t right)
{
    float_binary_ui(mpfr_sub_ui);
}

float_value_t *float_ui_sub(uint32_t left, float_value_t *right)
{
    float_binary_ui_rev(mpfr_ui_sub);
}

float_value_t *float_mul_ui(float_value_t *left, uint32_t right)
{
    float_binary_ui(mpfr_mul_ui);
}

float_value_t *float_div_ui(float_value_t *left, uint32_t right)
{
    float_binary_ui(mpfr_div_ui);
}

float_value_t *float_ui_div(uint32_t left, float_value_t *right)
{
    float_binary_ui_rev(mpfr_ui_div);
}

float_value_t *float_int_div_ui(int_value_t *left, uint32_t right)
{
    float_value_t *res = float_set_int(left);
    mpfr_div_ui(res->num, res->num, right, MPFR_RNDN);

    int_free(left);
    return res;
}

float_value_t *float_ui_div_int(uint32_t left, int_value_t *right)
{
    float_value_t *res = float_set_ui(left);
    mpfr_div_z(res->num, res->num, right->num, MPFR_RNDN);

    int_free(right);
    return res;
}

float_value_t *float_ui_div_ui(uint32_t left, uint32_t right)
{
    float_value_t *res = float_set_ui(left);
    mpfr_div_ui(res->num, res->num, right, MPFR_RNDN);
    return res;
}

float_value_t *float_mod_ui(float_value_t *left, uint32_t right)
{
    if (left->ref)
    {
        left->ref--;

        float_value_t *res = float_set_ui(right);
        mpfr_remainder(res->num, left->num, res->num, MPFR_RNDN);
        return res;
    }

    mpfr_t rfloat;
    mpfr_init2(rfloat, 64);
    mpfr_set_ui(rfloat, right, MPFR_RNDN);

    mpfr_remainder(left->num, left->num, rfloat, MPFR_RNDN);

    mpfr_clear(rfloat);
    return left;
}

float_value_t *float_ui_mod(uint32_t left, float_value_t *right)
{
    if (right->ref)
    {
        right->ref--;

        float_value_t *res = float_set_ui(left);
        mpfr_remainder(res->num, res->num, right->num, MPFR_RNDN);
        return res;
    }

    mpfr_t lfloat;
    mpfr_init2(lfloat, 64);
    mpfr_set_ui(lfloat, left, MPFR_RNDN);

    mpfr_remainder(right->num, lfloat, right->num, MPFR_RNDN);

    mpfr_clear(lfloat);
    return right;
}

int_value_t *float_quot_ui(float_value_t *left, uint32_t right)
{
    int_value_t *res = int_init();

    if (left->ref)
    {
        left->ref--;

        mpfr_t tmp;
        mpfr_init2(tmp, 64);
        mpfr_div_ui(tmp, left->num, right, MPFR_RNDN);
        mpfr_get_z(res->num, tmp, MPFR_RNDZ);

        mpfr_clear(tmp);
        return res;
    }

    mpfr_div_ui(left->num, left->num, right, MPFR_RNDN);
    mpfr_get_z(res->num, left->num, MPFR_RNDZ);

    mpfr_clear(left->num);
    mr_free(left);
    return res;
}

int_value_t *float_ui_quot(uint32_t left, float_value_t *right)
{
    int_value_t *res = int_init();

    if (right->ref)
    {
        right->ref--;

        mpfr_t tmp;
        mpfr_init2(tmp, 64);
        mpfr_ui_div(tmp, left, right->num, MPFR_RNDN);
        mpfr_get_z(res->num, tmp, MPFR_RNDZ);

        mpfr_clear(tmp);
        return res;
    }

    mpfr_ui_div(right->num, left, right->num, MPFR_RNDN);
    mpfr_get_z(res->num, right->num, MPFR_RNDZ);

    mpfr_clear(right->num);
    mr_free(right);
    return res;
}

float_value_t *float_pow_ui(float_value_t *left, uint32_t right)
{
    float_binary_ui(mpfr_pow_ui);
}

float_value_t *float_ui_pow(uint32_t left, float_value_t *right)
{
    float_binary_ui_rev(mpfr_ui_pow);
}

float_value_t *float_ui_pow_int(uint32_t left, int_value_t *right)
{
    float_value_t *res = float_set_ui(left);
    mpfr_pow_z(res->num, res->num, right->num, MPFR_RNDN);

    int_free(right);
    return res;
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
