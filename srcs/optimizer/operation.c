/*
    MetaReal Compiler version 1.0.0
*/

#include <optimizer/operation.h>
#include <stdlib.h>

#define INT_C(v) ((int_value_t*)v->value)

visit_res_t compute_add(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (left->type)
        {
        case INT_V:
            mpz_add(INT_C(left)->num, INT_C(left)->num, INT_C(right)->num);
            res.value = *left;

            free_int_value(right->value);
            return res;
        }
    }

    abort();
}

visit_res_t compute_sub(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (left->type)
        {
        case INT_V:
            mpz_sub(INT_C(left)->num, INT_C(left)->num, INT_C(right)->num);
            res.value = *left;

            free_int_value(right->value);
            return res;
        }
    }

    abort();
}

visit_res_t compute_mul(value_t *left, value_t *right)
{
    visit_res_t res;

    switch (left->type)
    {
    case INT_V:
        switch (left->type)
        {
        case INT_V:
            mpz_mul(INT_C(left)->num, INT_C(left)->num, INT_C(right)->num);
            res.value = *left;

            free_int_value(right->value);
            return res;
        }
    }

    abort();
}


visit_res_t compute_pos(value_t *operand)
{
    visit_res_t res;

    switch (operand->type)
    {
    case INT_V:
        res.value = *operand;
        return res;
    }

    abort();
}

visit_res_t compute_neg(value_t *operand)
{
    visit_res_t res;

    switch (operand->type)
    {
    case INT_V:
        mpz_neg(INT_C(operand)->num, INT_C(operand)->num);
        res.value = *operand;
        return res;
    }

    abort();
}
