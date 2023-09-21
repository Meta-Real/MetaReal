# AEOA: Arithmetic Expression Optimization Algorithm
This algorithm only considers addition, subtraction and multiplication on integers. But it's not too hard to extend it to cover other operators and data types.

### How to read
1. Variable is a data unit that can hold an expression, a value or another variable. Using variables does not add any additional calculation.
2. Expression is a binary operation between two variables.
3. Capital letters are used for expressions and small letters are used for variables.
4. Static variables are of the form `$#` where `$` is a letter and `#` is a natural number.
5. Value of a variable `a` is shown as `V(a)`.
6. IFF means "if and only if".
7. OCM means "one of these conditions has been met".

### Idioms
1. **Necessary** variable is a variable that is used in the program more than once or used in a non-removable expression.
2. **Dynamic** variable is a variable that its value can not be determined in the compile time (opposite of **Static**).
3. `a <op> b` is **Trivial** if you can replace it with a static variable.
4. `a <op> b` is **Eliminatory** if you can replace it with a variable.
5. An expression is **Optimal** if you can not optimize it further by factoring or distributing.
6. **Opening** a variable is replacing it with its value.

### Rule 1
`a <op> b` is trivial IFF OCM:
1. `a` and `b` are both static variables
optimal case: `a <op> b`.
3. `<op>` is addition, `a = c1 + e` and `b = c2 - e`
optimal case: `c1 + c2`.
4. `<op>` is subtraction, `a = c1 + e` and `b = c2 + e`
optimal case: `c1 - c2`.
5. `<op>` is multiplication and `a` or `b` is 0
optimal case: `0`.

### Rule 2
`a <op> b` is eliminatory IFF OCM:
1. `a <op> b` is trivial.
2. `<op>` is addition, `a = c1 * m + e` and `b = (1 - c1) * m + e`
optimal case: `m`.
4. `<op>` is subtraction, `a = c1 * m + e` and `b = (c1 - 1) * m + e`
optimal case: `m`.
5. `<op>` is multiplication and `a` or `b` is 1
optimal case: `a` or `b`.

### Rule 3
`a <op> b` is not optimal IFF OCM:
1. `a` and `b` are unnecessary variables
optimal case: optimal case of `V(a) <op> V(b)`.
3. `a <op> b` is eliminatory.
4. `<op>` is addition, `a = c1 * m + e` and `b = (-1 - c1) * m - e`
optimal case: `-m`.
5. `<op>` is subtraction, `a = c1 * m + e` and `b = (c1 + 1) * m + e`
optimal case: `-m`.
6. `<op>` is multiplication and `a` or `b` is -1
optimal case: `-a` or `-b`.

### Rule 4
Variable `a` should be opened IFF OCM:
1. `V(a) = c1`.
2. `V(a) = b`.

### Rule 5
If `p = m1 * a + n1 * b`, `q = m2 * a + n2 * b` and `C = m3 * a + n3 * b`, `C` is not optimal IFF:
`m3 / (m2 + m1) = n3 / (n2 + n1) = k` or `m3 / (m2 - m1) = n3 / (n2 - n1) = k` where `k` is an integer.
optimal case: `C = k * (p + q)` or `C = k * (p - q)`.

### Rule 6
