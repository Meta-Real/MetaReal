/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_OPERATION__
#define __MR_OPERATION__

#include <optimizer/optimizer.h>

/* binary operations */

void compute_add(visit_res_t *res, value_t *left, value_t *right);
void compute_sub(visit_res_t *res, value_t *left, value_t *right);
void compute_mul(visit_res_t *res, value_t *left, value_t *right);
void compute_div(visit_res_t *res, value_t *left, value_t *right);
void compute_mod(visit_res_t *res, value_t *left, value_t *right);
void compute_quot(visit_res_t *res, value_t *left, value_t *right);
void compute_pow(visit_res_t *res, value_t *left, value_t *right);

void compute_b_and(visit_res_t *res, value_t *left, value_t *right);
void compute_b_or(visit_res_t *res, value_t *left, value_t *right);
void compute_b_xor(visit_res_t *res, value_t *left, value_t *right);
void compute_lshift(visit_res_t *res, value_t *left, value_t *right);
void compute_rshift(visit_res_t *res, value_t *left, value_t *right);

void compute_eq(visit_res_t *res, value_t *left, value_t *right);
void compute_neq(visit_res_t *res, value_t *left, value_t *right);
void compute_ex_eq(visit_res_t *res, value_t *left, value_t *right);
void compute_ex_neq(visit_res_t *res, value_t *left, value_t *right);

void compute_lt(visit_res_t *res, value_t *left, value_t *right);
void compute_gt(visit_res_t *res, value_t *left, value_t *right);
void compute_lte(visit_res_t *res, value_t *left, value_t *right);
void compute_gte(visit_res_t *res, value_t *left, value_t *right);

/* unary operations */

void compute_pos(visit_res_t *res, value_t *operand, pos_t *poss);
void compute_neg(visit_res_t *res, value_t *operand, pos_t *poss);

void compute_b_not(visit_res_t *res, value_t *operand, pos_t *poss);

void compute_inc(visit_res_t *res, value_t *operand, pos_t *poss, pos_t *pose);
void compute_dec(visit_res_t *res, value_t *operand, pos_t *poss, pos_t *pose);

/* helper operations */

uint8_t compute_vneq(const value_t *left, const value_t *right);

#endif /* __MR_OPERATION__ */
