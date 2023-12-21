/**
 * @file token.h
 * Definitions of the tokens which are used by the lexer to organize the code. \n
 * All the things defined in \a token.c and this file have the prefix <em>mr_token</em>.
*/

#ifndef __MR_TOKEN__
#define __MR_TOKEN__

#include <defs.h>

/**
 * @struct __MR_TOKEN_T
 * The word equivalent of the compilation process. \n
 * The token contains all the information needed about a code element. \n
 * There are two types of tokens: \n
 * 1. Symbols: tokens that don't have a value and only their type is important. \n
 *    Examples: plus sign, equal sign, semicolon, keywords, types, etc. \n
 * 2. Words: tokens that have a value and their value can change according to the code. \n
 *    Examples: numbers (int, float, complex), strings, identifiers, etc. \n
 * Tokens only hold the information about one symbol or word. Not the grammar.
 * @var mr_byte_t __MR_TOKEN_T::type
 * The type of the token.
 * @var mr_str_t __MR_TOKEN_T::value
 * The value of the token (if the token is a word, NULL otherwise).
 * @var mr_size_t __MR_TOKEN_T::size
 * The size of the \a value (if the token is a word, undefined otherwise).
 * @var mr_pos_t __MR_TOKEN_T::poss
 * The starting position of the token.
 * @var mr_pos_t __MR_TOKEN_T::pose
 * The ending position of the token.
*/
struct __MR_TOKEN_T
{
    mr_byte_t type;
    mr_str_t value;
    mr_size_t size;

    mr_pos_t poss;
    mr_pos_t pose;
};
typedef struct __MR_TOKEN_T mr_token_t;

/**
 * @enum __MR_TOKEN_ENUM
 * The list of valid token types. \n
 * The <em>MR_TOKEN_PLUS</em>, <em>MR_TOKEN_MINUS<em>,
 * <em>MR_TOKEN_B_NOT</em>, and <em>MR_TOKEN_NOT_K</em>
 * are placed together for performance reasons (see the \a mr_parser_factor function).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EOF
 * The <em>end of file</em> token type. \n
 * This token must be present at the end of the tokens list (null terminator).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NEWLINE
 * The \a newline and the \a semicolon token type ('\\n' and ';' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IDENTIFIER
 * The \a identifier token type. The \a identifier token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INT
 * The \a integer token type. The \a integer token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FLOAT
 * The <em>float number</em> token type. The <em>float number</em> token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IMAGINARY
 * The <em>imaginary number</em> token type. The <em>imaginary number</em> token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CHR
 * The \a character token type. The \a character token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STR
 * The \a string token type. The \a string token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FSTR
 * The <em>formatted string</em> token type. The <em>formatted string</em> token is a word.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FSTR_START
 * The <em>start of the formatted string</em> token type. \n
 * This token contains the information about the start and end position of the string.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FSTR_END
 * The <em>end of the formatted string</em> token type. \n
 * This token only indicates the end of the string. Not its ending position.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PLUS
 * The \a plus token type ('+' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MINUS
 * The \a minus token type ('-' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MULTIPLY
 * The \a multiply token type ('*' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DIVIDE
 * The \a divide token type ('/' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MODULO
 * The \a modulo token type ('%' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_QUOTIENT
 * The \a quotient token type ('//' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_POWER
 * The \a power token type ('**' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_AND
 * The <em>binary and</em> token type ('&' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_OR
 * The <em>binary or</em> token type ('|' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_XOR
 * The <em>binary xor</em> token type ('^' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_SHIFT
 * The <em>left shift</em> token type ('<<' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_SHIFT
 * The <em>right shift</em> token type ('>>' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_NOT
 * The <em>binary not</em> token type ('~' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EQUAL
 * The \a equal token type ('==' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NEQUAL
 * The <em>not equal</em> token type ('!=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EX_EQUAL
 * The <em>exactly equal</em> token type ('===' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EX_NEQUAL
 * The <em>exactly not equal</em> token type ('!==' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LESS
 * The <em>less than</em> token type ('<' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_GREATER
 * The <em>greater than</em> token type ('>' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LESS_EQUAL
 * The <em>less than or equal to</em> token type ('<=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_GREATER_EQUAL
 * The <em>greater than or equal to</em> token type ('>=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INCREMENT
 * The \a increment token type ('++' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DECREMENT
 * The \a decrement token type ('--' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ASSIGN
 * The \a assign token type ('=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LINK
 * The \a link token type ('->' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PLUS_ASSIGN
 * The <em>plus and assign</em> token type ('+=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MINUS_ASSIGN
 * The <em>minus and assign</em> token type ('-=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MULTIPLY_ASSIGN
 * The <em>multiply and assign</em> token type ('*=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DIVIDE_ASSIGN
 * The <em>divide and assign</em> token type ('/=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_MODULO_ASSIGN
 * The <em>modulo and assign</em> token type ('%=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_QUOTIENT_ASSIGN
 * The <em>quotient and assign</em> token type ('//=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_POWER_ASSIGN
 * The <em>power and assign</em> token type ('**=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_AND_ASSIGN
 * The <em>binary and and assign</em> token type ('&=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_OR_ASSIGN
 * The <em>binary or and assign</em> token type ('|=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_B_XOR_ASSIGN
 * The <em>binary xor and assign</em> token type ('^=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_SHIFT_ASSIGN
 * The <em>left shift and assign</em> token type ('<<=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_SHIFT_ASSIGN
 * The <em>right shift and assign</em> token type ('>>=' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_PAREN
 * The <em>left parentheses</em> token type ('(' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_PAREN
 * The <em>right parentheses</em> token type (')' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_SQUARE
 * The <em>left square bracket</em> token type ('[' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_SQUARE
 * The <em>right square bracket</em> token type (']' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_L_CURLY
 * The <em>left curly bracket</em> token type ('{' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_R_CURLY
 * The <em>right curly bracket</em> token type ('}' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_COMMA
 * The \a comma token type (',' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DOT
 * The \a dot token type ('.' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_COLON
 * The \a colon token type (':' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_QUESTION
 * The <em>question mark</em> token type ('?' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DOLLAR
 * The <em>dollar sign</em> token type ('$' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ELLIPSIS
 * The \a ellipsis token type ('...' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TRUE_K
 * The \b true keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FALSE_K
 * The \b false keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NONE_K
 * The \b none keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FUNC_K
 * The \b func keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STRUCT_K
 * The \b struct keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CLASS_K
 * The \b class keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ENUM_K
 * The \b enum keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PRIVATE_K
 * The \b private keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_PUBLIC_K
 * The \b public keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LOCAL_K
 * The \b local keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_GLOBAL_K
 * The \b global keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CONST_K
 * The \b const keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STATIC_K
 * The \b static keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IS_K
 * The \b is keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ARE_K
 * The \b are keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IN_K
 * The \b in keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_AND_K
 * The \b and keyword and the \a and token type ('&&' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_OR_K
 * The \b or keyword and the \a or token type ('||' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_NOT_K
 * The \b not keyword and the \a not token type ('!' equivalent).
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IF_K
 * The \b if keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ELIF_K
 * The \b elif keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_ELSE_K
 * The \b else keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_SWITCH_K
 * The \b switch keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CASE_K
 * The \b case keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DEFAULT_K
 * The \b default keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FOR_K
 * The \b for keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TO_K
 * The \b to keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STEP_K
 * The \b step keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_WHILE_K
 * The \b while keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DO_K
 * The \b do keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TRY_K
 * The \b try keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_EXCEPT_K
 * The \b except keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FINALLY_K
 * The \b finally keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_RAISE_K
 * The \b raise keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_IMPORT_K
 * The \b import keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INCLUDE_K
 * The \b include keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_RETURN_K
 * The \b return keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_BREAK_K
 * The \b break keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CONTINUE_K
 * The \b continue keyword token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_OBJECT_T
 * The \b object type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_INT_T
 * The \b int type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_FLOAT_T
 * The \b float type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_COMPLEX_T
 * The \b complex type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_BOOL_T
 * The \b bool type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_CHAR_T
 * The \b char type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_STR_T
 * The \b str type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_LIST_T
 * The \b list type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TUPLE_T
 * The \b tuple type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_DICT_T
 * The \b dict type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_SET_T
 * The \b set type token type.
 * @var __MR_TOKEN_ENUM::MR_TOKEN_TYPE_T
 * The \b type type token type.
*/
enum __MR_TOKEN_ENUM
{
    MR_TOKEN_EOF,
    MR_TOKEN_NEWLINE,

    MR_TOKEN_IDENTIFIER,

    MR_TOKEN_INT,
    MR_TOKEN_FLOAT,
    MR_TOKEN_IMAGINARY,
    MR_TOKEN_CHR,
    MR_TOKEN_STR,
    MR_TOKEN_FSTR,
    MR_TOKEN_FSTR_START,
    MR_TOKEN_FSTR_END,

    MR_TOKEN_MULTIPLY,
    MR_TOKEN_DIVIDE,
    MR_TOKEN_MODULO,
    MR_TOKEN_QUOTIENT,
    MR_TOKEN_POWER,

    MR_TOKEN_B_AND,
    MR_TOKEN_B_OR,
    MR_TOKEN_B_XOR,
    MR_TOKEN_L_SHIFT,
    MR_TOKEN_R_SHIFT,

    MR_TOKEN_EQUAL,
    MR_TOKEN_NEQUAL,
    MR_TOKEN_EX_EQUAL,
    MR_TOKEN_EX_NEQUAL,
    MR_TOKEN_LESS,
    MR_TOKEN_GREATER,
    MR_TOKEN_LESS_EQUAL,
    MR_TOKEN_GREATER_EQUAL,

    MR_TOKEN_INCREMENT,
    MR_TOKEN_DECREMENT,

    MR_TOKEN_ASSIGN,
    MR_TOKEN_LINK,

    MR_TOKEN_PLUS_ASSIGN,
    MR_TOKEN_MINUS_ASSIGN,
    MR_TOKEN_MULTIPLY_ASSIGN,
    MR_TOKEN_DIVIDE_ASSIGN,
    MR_TOKEN_MODULO_ASSIGN,
    MR_TOKEN_QUOTIENT_ASSIGN,
    MR_TOKEN_POWER_ASSIGN,
    MR_TOKEN_B_AND_ASSIGN,
    MR_TOKEN_B_OR_ASSIGN,
    MR_TOKEN_B_XOR_ASSIGN,
    MR_TOKEN_L_SHIFT_ASSIGN,
    MR_TOKEN_R_SHIFT_ASSIGN,

    MR_TOKEN_L_PAREN,
    MR_TOKEN_R_PAREN,
    MR_TOKEN_L_SQUARE,
    MR_TOKEN_R_SQUARE,
    MR_TOKEN_L_CURLY,
    MR_TOKEN_R_CURLY,

    MR_TOKEN_COMMA,
    MR_TOKEN_DOT,
    MR_TOKEN_COLON,
    MR_TOKEN_QUESTION,
    MR_TOKEN_DOLLAR,
    MR_TOKEN_ELLIPSIS,

    MR_TOKEN_PLUS,
    MR_TOKEN_MINUS,
    MR_TOKEN_B_NOT,
    MR_TOKEN_NOT_K,

    MR_TOKEN_TRUE_K,
    MR_TOKEN_FALSE_K,
    MR_TOKEN_NONE_K,

    MR_TOKEN_FUNC_K,
    MR_TOKEN_STRUCT_K,
    MR_TOKEN_CLASS_K,
    MR_TOKEN_ENUM_K,

    MR_TOKEN_PRIVATE_K,
    MR_TOKEN_PUBLIC_K,
    MR_TOKEN_LOCAL_K,
    MR_TOKEN_GLOBAL_K,
    MR_TOKEN_CONST_K,
    MR_TOKEN_STATIC_K,

    MR_TOKEN_IS_K,
    MR_TOKEN_ARE_K,
    MR_TOKEN_IN_K,
    MR_TOKEN_AND_K,
    MR_TOKEN_OR_K,

    MR_TOKEN_IF_K,
    MR_TOKEN_ELIF_K,
    MR_TOKEN_ELSE_K,

    MR_TOKEN_SWITCH_K,
    MR_TOKEN_CASE_K,
    MR_TOKEN_DEFAULT_K,

    MR_TOKEN_FOR_K,
    MR_TOKEN_TO_K,
    MR_TOKEN_STEP_K,

    MR_TOKEN_WHILE_K,
    MR_TOKEN_DO_K,

    MR_TOKEN_TRY_K,
    MR_TOKEN_EXCEPT_K,
    MR_TOKEN_FINALLY_K,
    MR_TOKEN_RAISE_K,

    MR_TOKEN_IMPORT_K,
    MR_TOKEN_INCLUDE_K,

    MR_TOKEN_RETURN_K,
    MR_TOKEN_BREAK_K,
    MR_TOKEN_CONTINUE_K,

    MR_TOKEN_OBJECT_T,
    MR_TOKEN_INT_T,
    MR_TOKEN_FLOAT_T,
    MR_TOKEN_COMPLEX_T,
    MR_TOKEN_BOOL_T,
    MR_TOKEN_CHAR_T,
    MR_TOKEN_STR_T,
    MR_TOKEN_LIST_T,
    MR_TOKEN_TUPLE_T,
    MR_TOKEN_DICT_T,
    MR_TOKEN_SET_T,
    MR_TOKEN_TYPE_T
};

#define MR_TOKEN_COUNT (MR_TOKEN_TYPE_T + 1)

#define MR_TOKEN_KEYWORD_PAD MR_TOKEN_NOT_K
#define MR_TOKEN_KEYWORD_COUNT (MR_TOKEN_CONTINUE_K - MR_TOKEN_KEYWORD_PAD + 1)
#define MR_TOKEN_KEYWORD_MAXSIZE 8

#define MR_TOKEN_TYPE_PAD MR_TOKEN_OBJECT_T
#define MR_TOKEN_TYPE_COUNT (MR_TOKEN_TYPE_T - MR_TOKEN_TYPE_PAD + 1)
#define MR_TOKEN_TYPE_MAXSIZE 7

extern mr_str_ct mr_token_label[MR_TOKEN_COUNT];

extern mr_str_ct mr_token_keyword[MR_TOKEN_KEYWORD_COUNT];
extern mr_byte_t mr_token_keyword_size[MR_TOKEN_KEYWORD_COUNT];

extern mr_str_ct mr_token_type[MR_TOKEN_TYPE_COUNT];
extern mr_byte_t mr_token_type_size[MR_TOKEN_TYPE_COUNT];

/**
 * It deallocates the tokens list and its elements from memory. \n
 * The \a tokens must end with an EOF token (null terminator). \n
 * Only words value will be deallocated from memory.
 * @param tokens
 * The list of tokens.
*/
void mr_token_free(mr_token_t *tokens);

/**
 * It prints out the tokens list into the <em>outstream</em>.
 * The \a tokens must end with an EOF token (null terminator). \n
 * The \a outstream is \a stdout by default and
 * can be changed with the dollar method <em>$set_outstream</em>.
 * @param tokens
 * The list of tokens.
*/
void mr_token_print(mr_token_t *tokens);

#endif /* __MR_TOKEN__ */
