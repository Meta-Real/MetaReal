/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_OPERATION__
#define __MR_OPERATION__

#include <optimizer/optimizer.h>

visit_res_t compute_add(value_t *left, value_t *right);
visit_res_t compute_sub(value_t *left, value_t *right);
visit_res_t compute_mul(value_t *left, value_t *right);

visit_res_t compute_pos(value_t *operand);
visit_res_t compute_neg(value_t *operand);

#endif /* __MR_OPERATION__ */
