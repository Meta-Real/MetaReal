/*
    MetaReal Compiler version 1.0.0
*/

#ifndef __MR_POS__
#define __MR_POS__

#include <stdint.h>

struct __POS_T
{
    uint64_t idx;           /**< Index of the position inside the code */
    uint64_t ln;            /**< Line of the position (incremented by newlines) */
};
typedef struct __POS_T pos_t; /**< Position in the MetaReal code (used by the debugger) */

/**
 * \def pos_set(i, l)
 * Creates a new position with the specified index and line.
 * \param i Index of the position (uint64_t)
 * \param l Line of the position (uint64_t)
 * \return A new position (pos_t)
*/
#define pos_set(i, l) ((pos_t){i, l})

#endif /* __MR_POS__ */
