/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_OPERATION__
#define __MR_OPERATION__

#include <optimizer/optimizer.h>

visit_res_t compute_add(value_t *left, value_t *right);
visit_res_t compute_sub(value_t *left, value_t *right);
visit_res_t compute_mul(value_t *left, value_t *right);
visit_res_t compute_div(value_t *left, value_t *right);
visit_res_t compute_mod(value_t *left, value_t *right);
visit_res_t compute_quot(value_t *left, value_t *right);
visit_res_t compute_pow(value_t *left, value_t *right);
visit_res_t compute_band(value_t *left, value_t *right);
visit_res_t compute_bor(value_t *left, value_t *right);
visit_res_t compute_bxor(value_t *left, value_t *right);
visit_res_t compute_lshift(value_t *left, value_t *right);
visit_res_t compute_rshift(value_t *left, value_t *right);

visit_res_t compute_pos(value_t *operand, pos_t *poss);
visit_res_t compute_neg(value_t *operand, pos_t *poss);
visit_res_t compute_bnot(value_t *operand, pos_t *poss);

#endif /* __MR_OPERATION__ */
