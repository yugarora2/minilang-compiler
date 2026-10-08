/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "symtab.h"
#include "report.h"
#include "ast.h"
#include "tac.h"

extern int yylineno;
extern int verbose_tokens;
extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);

int error_count = 0;
ASTNode *ast_root = NULL;

/* ---- Semantic analysis helpers ---- */
static void sem_error(int line, const char *category, const char *fmt, ...) {
    char msg[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);
    fprintf(stderr, "%s error at line %d: %s\n", category, line, msg);
    report_add_error(line, category, msg);
    error_count++;
}

static int type_compatible(DataType target, DataType source) {
    if (target == source) return 1;
    if (target == TYPE_FLOAT && source == TYPE_INT) return 1; 
    return 0;
}

static DataType check_arith(DataType left, DataType right, int line, const char *op) {
    if (left == TYPE_STRING || right == TYPE_STRING) {
        sem_error(line, "Semantic", "operator '%s' not valid on type 'string'.", op);
        return TYPE_UNKNOWN;
    }
    if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) return TYPE_UNKNOWN;
    if (left == TYPE_FLOAT || right == TYPE_FLOAT) return TYPE_FLOAT;
    return TYPE_INT;
}

static DataType check_relational(DataType left, DataType right, int line, const char *op) {
    if (left == TYPE_UNKNOWN || right == TYPE_UNKNOWN) return TYPE_INT;
    int left_numeric = (left == TYPE_INT || left == TYPE_FLOAT);
    int right_numeric = (right == TYPE_INT || right == TYPE_FLOAT);
    if (left_numeric && right_numeric) return TYPE_INT;
    if (left == TYPE_STRING && right == TYPE_STRING) return TYPE_INT;
    sem_error(line, "Semantic", "mismatched types on either side of '%s'.", op);
    return TYPE_INT;
}

#line 129 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_STRING_LIT = 4,                 /* STRING_LIT  */
  YYSYMBOL_INT_LIT = 5,                    /* INT_LIT  */
  YYSYMBOL_FLOAT_LIT = 6,                  /* FLOAT_LIT  */
  YYSYMBOL_INT_KW = 7,                     /* INT_KW  */
  YYSYMBOL_FLOAT_KW = 8,                   /* FLOAT_KW  */
  YYSYMBOL_STRING_KW = 9,                  /* STRING_KW  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_WHILE = 12,                     /* WHILE  */
  YYSYMBOL_PRINT = 13,                     /* PRINT  */
  YYSYMBOL_EQ = 14,                        /* EQ  */
  YYSYMBOL_NEQ = 15,                       /* NEQ  */
  YYSYMBOL_LE = 16,                        /* LE  */
  YYSYMBOL_GE = 17,                        /* GE  */
  YYSYMBOL_LT = 18,                        /* LT  */
  YYSYMBOL_GT = 19,                        /* GT  */
  YYSYMBOL_20_ = 20,                       /* '+'  */
  YYSYMBOL_21_ = 21,                       /* '-'  */
  YYSYMBOL_22_ = 22,                       /* '*'  */
  YYSYMBOL_23_ = 23,                       /* '/'  */
  YYSYMBOL_UMINUS = 24,                    /* UMINUS  */
  YYSYMBOL_25_ = 25,                       /* ';'  */
  YYSYMBOL_26_ = 26,                       /* '='  */
  YYSYMBOL_27_ = 27,                       /* '('  */
  YYSYMBOL_28_ = 28,                       /* ')'  */
  YYSYMBOL_29_ = 29,                       /* '{'  */
  YYSYMBOL_30_ = 30,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_program = 32,                   /* program  */
  YYSYMBOL_stmt_list = 33,                 /* stmt_list  */
  YYSYMBOL_stmt = 34,                      /* stmt  */
  YYSYMBOL_type = 35,                      /* type  */
  YYSYMBOL_decl_stmt = 36,                 /* decl_stmt  */
  YYSYMBOL_assign_stmt = 37,               /* assign_stmt  */
  YYSYMBOL_if_stmt = 38,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 39,                /* while_stmt  */
  YYSYMBOL_print_stmt = 40,                /* print_stmt  */
  YYSYMBOL_block = 41,                     /* block  */
  YYSYMBOL_expr = 42                       /* expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   167

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  12
/* YYNRULES -- Number of rules.  */
#define YYNRULES  37
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  73

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   275


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      27,    28,    22,    20,     2,    21,     2,    23,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    25,
       2,    26,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    29,     2,    30,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    84,    84,    95,    96,   100,   101,   102,   103,   104,
     105,   109,   110,   111,   115,   122,   138,   155,   156,   160,
     164,   168,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   191,   192,   193,   194
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "STRING_LIT",
  "INT_LIT", "FLOAT_LIT", "INT_KW", "FLOAT_KW", "STRING_KW", "IF", "ELSE",
  "WHILE", "PRINT", "EQ", "NEQ", "LE", "GE", "LT", "GT", "'+'", "'-'",
  "'*'", "'/'", "UMINUS", "';'", "'='", "'('", "')'", "'{'", "'}'",
  "$accept", "program", "stmt_list", "stmt", "type", "decl_stmt",
  "assign_stmt", "if_stmt", "while_stmt", "print_stmt", "block", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-41)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -41,    11,    35,   -41,   -12,   -41,   -41,   -41,    -9,    -8,
       5,   -41,   -41,    30,   -41,   -41,   -41,   -41,   -41,   -41,
      46,    46,    46,    46,    27,   -10,   -41,   -41,   -41,   -41,
      46,    46,   120,    60,    75,    90,   -41,   -41,    46,   -41,
     105,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,   -41,    12,    12,    21,   132,   -41,   -41,   -41,   -41,
     -41,   -41,   -41,   144,   144,   -11,   -11,    42,   -41,   -41,
     -41,    12,   -41
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,    11,    12,    13,     0,     0,
       0,     3,     4,     0,     5,     6,     7,     8,     9,    10,
       0,     0,     0,     0,     0,     0,    37,    36,    34,    35,
       0,     0,     0,     0,     0,     0,    21,    14,     0,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,     0,    32,    30,    31,    28,
      29,    26,    27,    22,    23,    24,    25,    17,    19,    20,
      15,     0,    18
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -41,   -41,    43,   -41,   -41,   -41,   -41,   -41,   -41,   -41,
     -40,   -21
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    12,    13,    14,    15,    16,    17,    18,
      19,    32
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      33,    34,    35,    41,    42,    43,    44,    45,    46,    39,
      40,     3,    67,    68,    20,    37,    38,    55,    21,    22,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
       4,    72,    23,    25,     5,     6,     7,     8,     4,     9,
      10,    11,     5,     6,     7,     8,    69,     9,    10,    26,
      27,    28,    29,    71,    24,     0,    11,    36,     0,     0,
       0,     0,     0,     0,    11,     0,     0,    30,     0,     0,
       0,     0,     0,    31,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,     0,     0,     0,     0,    52,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,     0,     0,    53,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,     0,     0,     0,     0,    54,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,     0,     0,    56,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,     0,    51,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,     0,    70,    41,    42,
      43,    44,    45,    46,     0,     0,    49,    50
};

static const yytype_int8 yycheck[] =
{
      21,    22,    23,    14,    15,    16,    17,    18,    19,    30,
      31,     0,    52,    53,    26,    25,    26,    38,    27,    27,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
       3,    71,    27,     3,     7,     8,     9,    10,     3,    12,
      13,    29,     7,     8,     9,    10,    25,    12,    13,     3,
       4,     5,     6,    11,    11,    -1,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    29,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    27,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    28,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,    28,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    28,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,    28,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    -1,    25,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    -1,    25,    14,    15,
      16,    17,    18,    19,    -1,    -1,    22,    23
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    32,    33,     0,     3,     7,     8,     9,    10,    12,
      13,    29,    34,    35,    36,    37,    38,    39,    40,    41,
      26,    27,    27,    27,    33,     3,     3,     4,     5,     6,
      21,    27,    42,    42,    42,    42,    30,    25,    26,    42,
      42,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    25,    28,    28,    28,    42,    28,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    41,    41,    25,
      25,    11,    41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    34,    34,    34,    34,    34,
      34,    35,    35,    35,    36,    36,    37,    38,    38,    39,
      40,    41,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     4,     5,     7,     5,
       5,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 84 "parser.y"
              { 
        ast_root = (yyvsp[0].node); 
        if (error_count == 0) {
            tac_generate(ast_root); 
        }
        (yyval.node) = ast_root;
        printf("\n[Parser] Program parsed & AST constructed successfully.\n"); 
    }
#line 1221 "parser.tab.c"
    break;

  case 3: /* stmt_list: %empty  */
#line 95 "parser.y"
                  { (yyval.node) = ast_new_block(); }
#line 1227 "parser.tab.c"
    break;

  case 4: /* stmt_list: stmt_list stmt  */
#line 96 "parser.y"
                     { ast_block_add((yyvsp[-1].node), (yyvsp[0].node)); (yyval.node) = (yyvsp[-1].node); }
#line 1233 "parser.tab.c"
    break;

  case 5: /* stmt: decl_stmt  */
#line 100 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1239 "parser.tab.c"
    break;

  case 6: /* stmt: assign_stmt  */
#line 101 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1245 "parser.tab.c"
    break;

  case 7: /* stmt: if_stmt  */
#line 102 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1251 "parser.tab.c"
    break;

  case 8: /* stmt: while_stmt  */
#line 103 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1257 "parser.tab.c"
    break;

  case 9: /* stmt: print_stmt  */
#line 104 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1263 "parser.tab.c"
    break;

  case 10: /* stmt: block  */
#line 105 "parser.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1269 "parser.tab.c"
    break;

  case 11: /* type: INT_KW  */
#line 109 "parser.y"
                { (yyval.ival) = TYPE_INT; }
#line 1275 "parser.tab.c"
    break;

  case 12: /* type: FLOAT_KW  */
#line 110 "parser.y"
                { (yyval.ival) = TYPE_FLOAT; }
#line 1281 "parser.tab.c"
    break;

  case 13: /* type: STRING_KW  */
#line 111 "parser.y"
                { (yyval.ival) = TYPE_STRING; }
#line 1287 "parser.tab.c"
    break;

  case 14: /* decl_stmt: type ID ';'  */
#line 115 "parser.y"
                  {
          if (!symtab_insert((yyvsp[-1].sval), (yyvsp[-2].ival), yylineno)) {
              sem_error(yylineno, "Semantic", "'%s' already declared.", (yyvsp[-1].sval));
          }
          (yyval.node) = ast_new_decl(yylineno, (yyvsp[-2].ival), (yyvsp[-1].sval), NULL);
          free((yyvsp[-1].sval));
      }
#line 1299 "parser.tab.c"
    break;

  case 15: /* decl_stmt: type ID '=' expr ';'  */
#line 122 "parser.y"
                           {
          if (!symtab_insert((yyvsp[-3].sval), (yyvsp[-4].ival), yylineno)) {
              sem_error(yylineno, "Semantic", "'%s' already declared.", (yyvsp[-3].sval));
          } else {
              if ((yyvsp[-1].node)->type != TYPE_UNKNOWN && !type_compatible((DataType)(yyvsp[-4].ival), (yyvsp[-1].node)->type)) {
                  sem_error(yylineno, "Semantic", "cannot initialize '%s' of type '%s' with value of type '%s'.",
                            (yyvsp[-3].sval), type_to_string((DataType)(yyvsp[-4].ival)), type_to_string((yyvsp[-1].node)->type));
              }
              symtab_mark_initialized((yyvsp[-3].sval));
          }
          (yyval.node) = ast_new_decl(yylineno, (yyvsp[-4].ival), (yyvsp[-3].sval), (yyvsp[-1].node));
          free((yyvsp[-3].sval));
      }
#line 1317 "parser.tab.c"
    break;

  case 16: /* assign_stmt: ID '=' expr ';'  */
#line 138 "parser.y"
                      {
          Symbol *sym = symtab_lookup((yyvsp[-3].sval));
          if (sym == NULL) {
              sem_error(yylineno, "Semantic", "'%s' used without declaration.", (yyvsp[-3].sval));
          } else {
              if ((yyvsp[-1].node)->type != TYPE_UNKNOWN && !type_compatible(sym->type, (yyvsp[-1].node)->type)) {
                  sem_error(yylineno, "Semantic", "cannot assign type '%s' to '%s' of type '%s'.",
                            type_to_string((yyvsp[-1].node)->type), (yyvsp[-3].sval), type_to_string(sym->type));
              }
              symtab_mark_initialized((yyvsp[-3].sval));
          }
          (yyval.node) = ast_new_assign(yylineno, (yyvsp[-3].sval), (yyvsp[-1].node));
          free((yyvsp[-3].sval));
      }
#line 1336 "parser.tab.c"
    break;

  case 17: /* if_stmt: IF '(' expr ')' block  */
#line 155 "parser.y"
                            { (yyval.node) = ast_new_if(yylineno, (yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1342 "parser.tab.c"
    break;

  case 18: /* if_stmt: IF '(' expr ')' block ELSE block  */
#line 156 "parser.y"
                                       { (yyval.node) = ast_new_if(yylineno, (yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1348 "parser.tab.c"
    break;

  case 19: /* while_stmt: WHILE '(' expr ')' block  */
#line 160 "parser.y"
                               { (yyval.node) = ast_new_while(yylineno, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1354 "parser.tab.c"
    break;

  case 20: /* print_stmt: PRINT '(' expr ')' ';'  */
#line 164 "parser.y"
                             { (yyval.node) = ast_new_print(yylineno, (yyvsp[-2].node)); }
#line 1360 "parser.tab.c"
    break;

  case 21: /* block: '{' stmt_list '}'  */
#line 168 "parser.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1366 "parser.tab.c"
    break;

  case 22: /* expr: expr '+' expr  */
#line 172 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "+", check_arith((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "+"), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1372 "parser.tab.c"
    break;

  case 23: /* expr: expr '-' expr  */
#line 173 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "-", check_arith((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "-"), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1378 "parser.tab.c"
    break;

  case 24: /* expr: expr '*' expr  */
#line 174 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "*", check_arith((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "*"), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1384 "parser.tab.c"
    break;

  case 25: /* expr: expr '/' expr  */
#line 175 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "/", check_arith((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "/"), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1390 "parser.tab.c"
    break;

  case 26: /* expr: expr LT expr  */
#line 176 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "<", check_relational((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "<"), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1396 "parser.tab.c"
    break;

  case 27: /* expr: expr GT expr  */
#line 177 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, ">", check_relational((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, ">"), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1402 "parser.tab.c"
    break;

  case 28: /* expr: expr LE expr  */
#line 178 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "<=", check_relational((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "<="), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1408 "parser.tab.c"
    break;

  case 29: /* expr: expr GE expr  */
#line 179 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, ">=", check_relational((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, ">="), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1414 "parser.tab.c"
    break;

  case 30: /* expr: expr EQ expr  */
#line 180 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "==", check_relational((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "=="), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1420 "parser.tab.c"
    break;

  case 31: /* expr: expr NEQ expr  */
#line 181 "parser.y"
                    { (yyval.node) = ast_new_binop(yylineno, "!=", check_relational((yyvsp[-2].node)->type, (yyvsp[0].node)->type, yylineno, "!="), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1426 "parser.tab.c"
    break;

  case 32: /* expr: '(' expr ')'  */
#line 182 "parser.y"
                    { (yyval.node) = (yyvsp[-1].node); }
#line 1432 "parser.tab.c"
    break;

  case 33: /* expr: '-' expr  */
#line 183 "parser.y"
                            {
          if ((yyvsp[0].node)->type == TYPE_STRING) {
              sem_error(yylineno, "Semantic", "unary '-' not valid on type 'string'.");
              (yyval.node) = ast_new_unary(yylineno, "uminus", TYPE_UNKNOWN, (yyvsp[0].node));
          } else {
              (yyval.node) = ast_new_unary(yylineno, "uminus", (yyvsp[0].node)->type, (yyvsp[0].node));
          }
      }
#line 1445 "parser.tab.c"
    break;

  case 34: /* expr: INT_LIT  */
#line 191 "parser.y"
                 { (yyval.node) = ast_new_int_lit(yylineno, (yyvsp[0].ival)); }
#line 1451 "parser.tab.c"
    break;

  case 35: /* expr: FLOAT_LIT  */
#line 192 "parser.y"
                 { (yyval.node) = ast_new_float_lit(yylineno, (yyvsp[0].fval)); }
#line 1457 "parser.tab.c"
    break;

  case 36: /* expr: STRING_LIT  */
#line 193 "parser.y"
                 { (yyval.node) = ast_new_string_lit(yylineno, (yyvsp[0].sval)); }
#line 1463 "parser.tab.c"
    break;

  case 37: /* expr: ID  */
#line 194 "parser.y"
         {
          Symbol *sym = symtab_lookup((yyvsp[0].sval));
          DataType t = TYPE_UNKNOWN;
          if (sym == NULL) {
              sem_error(yylineno, "Semantic", "'%s' used without declaration.", (yyvsp[0].sval));
          } else {
              t = sym->type;
          }
          (yyval.node) = ast_new_id(yylineno, t, (yyvsp[0].sval));
          free((yyvsp[0].sval));
      }
#line 1479 "parser.tab.c"
    break;


#line 1483 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 207 "parser.y"


void yyerror(const char *s) {
    sem_error(yylineno, "Syntax", "%s", s);
}

int main(int argc, char **argv) {
    const char *filename = NULL;
    const char *html_out = "report.html";
    int want_html = 1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose_tokens = 1;
        } else if (strcmp(argv[i], "--no-html") == 0) {
            want_html = 0;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            html_out = argv[++i];
        } else {
            filename = argv[i];
        }
    }

    if (filename == NULL) {
        fprintf(stderr, "Usage: %s [-v] [--no-html] [-o report.html] <source_file.ml>\n", argv[0]);
        return 1;
    }

    yyin = fopen(filename, "r");
    if (!yyin) return 1;

    yyparse();
    fclose(yyin);

    if (want_html) {
        report_write_html(html_out, filename, error_count, ast_root);
        printf("\nHTML report written to: %s\n", html_out);
    }
    if (want_html) {
        report_write_html(html_out, filename, error_count, ast_root);
        printf("\nHTML report written to: %s\n", html_out);
        
        // Export traditional bubble-tree graph in DOT format
        FILE *dot_fp = fopen("ast.dot", "w");
        if (dot_fp) {
            ast_write_dot(dot_fp, ast_root);
            fclose(dot_fp);
            printf("DOT graph written to: ast.dot\n");
            
            // Try to automatically compile to PNG if Graphviz 'dot' tool is available
            int ret = system("dot -Tpng ast.dot -o ast.png 2>/dev/null");
            if (ret == 0) {
                printf("Bubble tree image generated: ast.png\n");
            }
        }
    }

    return error_count == 0 ? 0 : 1;
}
