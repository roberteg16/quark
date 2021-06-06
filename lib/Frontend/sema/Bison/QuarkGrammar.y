%require "3.5.1"

%token  END 0 ENDL "\n"
%token  MODULE EXPORT IMPORT
%token  FOR WHILE
%token  IF ELSIF ELSE
%token  RET DEFER TYPE FN
%token  ALLOC DEALLOC
%token  VAR
%token  MUT

%token  PLUS "+" MINUS "-" SLASH "/" ASTERISK "*" AMPERSAND "&" MOD "%" EQUAL "="
%token PIPEPIPE "||" LESS "<" GREAT ">" AMPERSANDAMPERSAND "&&" EXCLAMATION "!"
%token EQUALEQUAL "==" EXCLAMATIONEQUAL "!=" GREATEQUAL ">=" LESSEQUAL "<="

%token ARROW "->" DOUBLEDOTEQUAL ":=" DOUBLEDOT ":" DOT "." SEMICOLON ";" COMMA ","
%token LEFTKEY "{" RIGHTKEY "}"
%token LEFTBRACE "[" RIGHTBRACE "]"
%token LEFTPARENTHESIS "(" RIGHTPARENTHESIS ")"

%token INTEGER REAL STRING CHAR ID

// Associativity and precendece (lower to greater)
%right "="
%left "||"
%left "&&"
%left "==" "!="
%left ">" "<" ">=" "<="
%left "+" "-"
%left "*" "/" "%"
%right "&" "!"
%left "(" ")" "[" "]" "." "->"

// Dangling ELSE
%right ELSIF ELSE

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           GRAMMAR STARTS HERE                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
%%
%start Start;

Start:  Module_preamble List_of_decls END;

Module_preamble:  Export_module List_of_imports;

List_of_imports:  List_of_imports IMPORT Module_name
               |  %empty;

Export_module:  EXPORT MODULE Module_name
             |  %empty;

Module_name:  ID Doted_name;

Doted_name: Doted_name "." ID
          | %empty;

List_of_decls: List_of_decls Declaration
             | %empty;

Declaration: Declaration_type
           | Declaration_func;

Var_decl: ID ID;
        | ID ID "[" INTEGER "]";

Declaration_type: TYPE ID "{" List_of_vars "}"
                | TYPE ID TYPE;

List_of_vars: List_of_vars Var_decl
            | %empty;

Declaration_func: FN Reciver ID ":" List_of_var_decls "->" ID Block
                | FN Reciver ID ":" List_of_var_decls Block;

Reciver: "(" ID "&" ID ")"
       |  %empty;

List_of_var_decls: List_of_var_decls1
                 | %empty;
List_of_var_decls1: List_of_var_decls1 "," Var_decl
                  | Var_decl;

Block: "{" List_of_stmt "}";

List_of_stmt: List_of_stmt Stmt
            | %empty;

Stmt: FOR "(" Local_var_decl ";" Expr ";" Expr ")" Stmt
    | IF "(" Expr ")" Stmt List_of_elsif
    | IF "(" Expr ")" Stmt List_of_elsif ELSE Stmt
    | WHILE "(" Expr ")" Stmt
    | RET Expr ENDL
    | DEFER Expr ENDL
    | DEALLOC Expr ENDL
    | Expr ENDL
    | Local_var_decl ENDL
    | Block;

Local_var_decl: VAR List_of_qualifiers ID ":" ID "=" Expr
              | VAR List_of_qualifiers ID ":=" Expr;


List_of_elsif: List_of_elsif ELSIF "(" Expr ")" Stmt
             | %empty;


List_of_qualifiers: List_of_qualifiers Qualifier
                  | %empty;

Qualifier: MUT;

Expr: "(" Expr ")"
    | Expr "+" Expr
    | Expr "-" Expr
    | Expr "*" Expr
    | Expr "/" Expr
    | Expr "%" Expr
    | Expr "=" Expr
    | Expr "!=" Expr
    | Expr "==" Expr
    | Expr "&&" Expr
    | Expr "||" Expr
    | Expr "<" Expr
    | Expr "<=" Expr
    | Expr ">" Expr
    | Expr ">=" Expr
    | "!" Expr
    | "-" Expr        %prec "&"
    | "*" Expr        %prec "&"
    | "&" Expr
    | Func_call
    | Method_call
    | ALLOC ID
    | ALLOC ID "[" Expr "]"
    | Term;

Term: ID
    | ID "[" Expr "]"
    | Literal;

Func_call: ID "(" List_of_expr ")";
Method_call: ID List_of_subtype_accesses "(" List_of_expr ")";

List_of_expr: List_of_expr1
            | %empty;
List_of_expr1: List_of_expr1 "," Expr
             | Expr;

List_of_subtype_accesses: List_of_subtype_accesses Subtype_access
                        | Subtype_access;

Subtype_access: "." ID
              | "->" ID;

Literal: ConstNumber
       | STRING
       | CHAR;

ConstNumber: INTEGER
           | REAL;
%%
