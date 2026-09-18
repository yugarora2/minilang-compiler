# MiniLang Compiler — Phase 1

A working compiler front-end for **MiniLang**, a small C-like language, built with
**Flex** (lexical analysis) and **Bison** (syntax + semantic analysis).

Phase 1 covers the full front end: **Lexical Analysis → Syntax Analysis → Symbol
Table Construction → Semantic Analysis (type checking)**. This is a genuinely
working compiler stage — it will correctly accept valid programs and reject
invalid ones with specific, line-numbered error messages.

## Language features (MiniLang)
- Types: `int`, `float`, `string`
- Declarations: `int x;` or `int x = 5;`
- Assignment: `x = expr;`
- Arithmetic: `+ - * /`, parentheses, unary minus
- Relational: `< > <= >= == !=`
- Control flow: `if (cond) { ... } else { ... }`, `while (cond) { ... }`
- Output: `print(expr);`
- Comments: `// line comment`

## What each phase does
1. **Lexical analysis (Flex, `lexer.l`)** — scans the source into tokens
   (keywords, identifiers, literals, operators), tracks line numbers, and
   reports invalid characters as lexical errors.
2. **Syntax analysis (Bison, `parser.y`)** — validates the token stream
   against the grammar; reports syntax errors with line numbers.
3. **Symbol table (`symtab.c` / `symtab.h`)** — records every declared
   variable with its type, declaration line, and whether it's been
   initialized. Catches **redeclaration** and **undeclared-variable use**.
4. **Semantic analysis (type checking, in `parser.y`)** — every expression
   carries a resolved type. Catches real type errors:
   - assigning/initializing a variable with an incompatible type
     (e.g. `int a = "hello";`)
   - using arithmetic operators on strings (`a + s` where `s` is a string)
   - narrowing assignment (`int c = someFloat;`)
   - relational comparisons across incompatible types

## Visual output

Every run generates a styled, self-contained **HTML report** (`report.html` by
default) — no server needed, just open it in a browser. It shows:
- the source code with line numbers
- the symbol table
- the full token stream, color-coded by category
- a pass/fail banner and an error table with line numbers and messages

This is what you should actually show in your demo/review — much clearer
than a terminal dump.

```bash
./minilang test_valid.ml              # writes report.html, open it in a browser
./minilang -o my_report.html test.ml  # custom output path
./minilang --no-html test.ml          # skip HTML generation, terminal only
```

## Build & run (macOS)

Install the toolchain once (via Homebrew):
```bash
brew install flex bison
```

macOS ships an old Bison by default, so tell your shell to use Homebrew's:
```bash
export PATH="/opt/homebrew/opt/bison/bin:$PATH"    # Apple Silicon
# or: export PATH="/usr/local/opt/bison/bin:$PATH" # Intel Mac
```
(Add that line to your `~/.zshrc` so you don't have to repeat it every terminal session.)

Then, from the project folder:
```bash
make            # builds ./minilang
./minilang test_valid.ml              # compiles a correct program
./minilang -v test_valid.ml           # same, plus a full token trace
./minilang test_errors.ml             # redeclaration / undeclared-variable errors
./minilang test_type_errors.ml        # type-checking errors
```

If VS Code's terminal doesn't pick up the PATH change, just open a fresh
terminal tab, or run `make` from a plain Terminal.app window instead.

## Files
| File | Purpose |
|---|---|
| `lexer.l` | Flex lexical analyzer spec |
| `parser.y` | Bison grammar, symbol table hookup, semantic checks, `main()` |
| `symtab.c` / `symtab.h` | Symbol table implementation |
| `report.c` / `report.h` | Collects tokens/errors and writes the styled HTML report |
| `Makefile` | Builds everything into `./minilang` |
| `test_valid.ml` | Sample program that compiles cleanly |
| `test_errors.ml` | Demonstrates redeclaration / undeclared-variable detection |
| `test_type_errors.ml` | Demonstrates type-checking errors |

## What's next (Phase 2 / Phase 3 — not built yet)
- AST construction + visual rendering
- Intermediate code generation (three-address code)
- Optimization passes: common subexpression elimination, dead code elimination
- Target assembly generation
