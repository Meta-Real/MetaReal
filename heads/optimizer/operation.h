/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_OPERATION__
#define __MR_OPERATION__

#include <optimizer/optimizer.h>

/* binary operations */

visit_res_t compute_add(value_t *left, value_t *right);
visit_res_t compute_sub(value_t *left, value_t *right);
visit_res_t compute_mul(value_t *left, value_t *right);
visit_res_t compute_div(value_t *left, value_t *right);
visit_res_t compute_mod(value_t *left, value_t *right);
visit_res_t compute_quot(value_t *left, value_t *right);
visit_res_t compute_pow(value_t *left, value_t *right);

visit_res_t compute_b_and(value_t *left, value_t *right);
visit_res_t compute_b_or(value_t *left, value_t *right);
visit_res_t compute_b_xor(value_t *left, value_t *right);
visit_res_t compute_lshift(value_t *left, value_t *right);
visit_res_t compute_rshift(value_t *left, value_t *right);

visit_res_t compute_eq(value_t *left, value_t *right);
visit_res_t compute_neq(value_t *left, value_t *right);
visit_res_t compute_ex_eq(value_t *left, value_t *right);
visit_res_t compute_ex_neq(value_t *left, value_t *right);

visit_res_t compute_lt(value_t *left, value_t *right);
visit_res_t compute_gt(value_t *left, value_t *right);
visit_res_t compute_lte(value_t *left, value_t *right);
visit_res_t compute_gte(value_t *left, value_t *right);

/* unary operations */

visit_res_t compute_pos(value_t *operand, pos_t *poss);
visit_res_t compute_neg(value_t *operand, pos_t *poss);

visit_res_t compute_b_not(value_t *operand, pos_t *poss);

visit_res_t compute_not(value_t *operand, pos_t *poss);

visit_res_t compute_inc(value_t *operand, pos_t *poss, pos_t *pose);
visit_res_t compute_dec(value_t *operand, pos_t *poss, pos_t *pose);

/* helper operations */

uint8_t compute_vneq(const value_t *left, const value_t *right);

#endif /* __MR_OPERATION__ */
