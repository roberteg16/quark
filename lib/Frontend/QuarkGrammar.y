%require "3.5.1"
%language "C++"

%skeleton "lalr1.cc"

/// Ask bison to add location tracking for us
/// %locations
/// Ask bison to generate position/location code into separated file
/// %define api.location.file "SourceLoc.h"

/// Add prefix to all symbols
/// %define api.symbol.prefix {S_}
/// Add prefix to all tokens
%define api.token.prefix {TK_}

/// Automove all semantic values of actions
/// %define api.value.automove

/// Define objects inside namespace quark
%define api.namespace {quark}

/// Tell bison to change parser name to QuarkParser
%define api.parser.class {QuarkParser}

/// Ask bison to use variants insted of union (safer and easier to use)
%define api.value.type variant
/// When used along with '%define api.value.type variant' Bison modifies
/// what it expects for yylex and generates several handy constructors for each
/// token
%define api.token.constructor

/// If used along with '%define api.value.type variant', RTTI is needed, so
/// do not disable it
%define parse.assert
%define parse.error verbose

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                          REQUIRD CODE STARTS HERE                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

%code requires
{
#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Expr.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>
#include <quark/Frontend/LexContext.h>
#include <quark/Frontend/SourceModule.h>
#include <quark/Frontend/ParserUtils.h>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/Debug.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/Casting.h>

} // %code requires

%param { LexContext &ctx }
%param { SourceModule &sm }

%code{
  quark::QuarkParser::symbol_type yylex (quark::LexContext&,
                                         quark::SourceModule &sm);
  void error (const std::string &err);
}

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        TOKEN DEFINITION STARTS HERE                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

%token  END 0
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

// Token types
%type<long long> INTEGER;
%type<long double> REAL;
%type<llvm::SmallString<40>> STRING;
%type<llvm::SmallString<10>> ID Export_module Module_name Doted_name;
%type<char> CHAR;
%type<std::unique_ptr<Expr>> ConstNumber Literal Term Expr Func_call Method_call;
%type<std::unique_ptr<VarRefExpr>> Var_access;
%type<std::unique_ptr<Expr>> Member_access;
%type<llvm::SmallVector<std::unique_ptr<Expr>, 4>> List_of_expr List_of_expr1;
%type<llvm::SmallVector<llvm::SmallString<10>, 10>> List_of_imports;
%type<std::deque<std::unique_ptr<Decl>>> List_of_decls;
%type<std::unique_ptr<Decl>> Declaration;
%type<std::unique_ptr<FuncDecl>> Declaration_func;
%type<std::unique_ptr<TypeDecl>> Declaration_type;
%type<std::unique_ptr<VarDecl>> Reciver Var_decl;
%type<llvm::SmallVector<std::unique_ptr<TypeFieldDecl>, 4>> List_of_field_decl;
%type<std::unique_ptr<TypeFieldDecl>> Field_decl;
%type<llvm::SmallVector<std::unique_ptr<VarDecl>, 4>> List_of_params List_of_params1;
%type<std::unique_ptr<Stmt>> Stmt;
%type<std::unique_ptr<VarDeclStmt>> Local_var_decl;
%type<std::unique_ptr<BlockStmt>> Block;
%type<std::vector<std::unique_ptr<Stmt>>> List_of_stmt;
%type<llvm::SmallVector<IfStmt::CondAndStmt, 4>> List_of_elsif;
%type<llvm::SmallVector<TypeAccess, 4>> List_of_accesses;
%type<TypeAccess> Subtype_access;
%type<std::vector<unsigned>> List_of_static_arrays;
%type<std::vector<std::unique_ptr<Expr>>> List_of_dinamic_arrays;
%type<unsigned> List_of_pointers;
%type<std::unique_ptr<Type>> Type_declaration;
%type<std::unique_ptr<Expr>> Var_access_array_subscript;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           GRAMMAR STARTS HERE                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
%%
%start Start;

Start: Export_module List_of_imports List_of_decls END {
                                                          sm.ExportedModule = std::move($1);
                                                          sm.ImportedModules = std::move($2);
                                                          sm.Declarations = std::move($3);
                                                       };

List_of_imports:  List_of_imports IMPORT Module_name {
                                                        $1.push_back($3);
                                                        $$ = std::move($1);
                                                     }
               |  %empty                             {};

Export_module:  EXPORT MODULE Module_name { $$ = std::move($3); }
             |  %empty                    { } ;

Module_name: ID Doted_name {
                              $$ += $1;
                              $$ += $2;
                            };

Doted_name: Doted_name "." ID {
                                $$ += $1;
                                $$ += '.';
                                $$ += $3;
                              }
          | %empty            { };

List_of_decls: List_of_decls Declaration {
                                            $1.push_back(std::move($2));
                                            $$ = std::move($1);
                                          }
             | %empty                    {};

Declaration: Declaration_type {
                                ctx.addTypeDecl(*$1);
                                $$ = std::move($1);
                              }
           | Declaration_func {
                                ctx.addFunctionDecl(*$1);
                                $$ = std::move($1);
                              };

Var_decl: ID Type_declaration   {
                                  ctx.checkNonExistenceOfVarCurrentLevel($1);
                                  $$ = std::make_unique<VarDecl>($1, std::move($2));
                                };

Declaration_type: TYPE ID "{" List_of_field_decl "}"  {
                                                        ctx.checkNonExistenceOfType($2);
                                                        $$ = std::make_unique<TypeDecl>($2, std::move($4));
                                                      };

Field_decl: ID Type_declaration   { $$ = std::make_unique<TypeFieldDecl>($1, std::move($2)); };

List_of_field_decl: List_of_field_decl Field_decl {
                                                    $1.push_back(std::move($2));
                                                    $$ = std::move($1);
                                                  }
                  | %empty                        {};

Declaration_func: FN Reciver ID ":" { ctx.enterScope(); } List_of_params "->" ID {
                                                                                    auto &listOfParams = $6;
                                                                                    for (auto &param : listOfParams) {
                                                                                      ctx.addVar(*param);
                                                                                    }
                                                                                  } "{" List_of_stmt "}" {
                                                                                                          auto reciver = std::move($2);
                                                                                                          if (reciver)
                                                                                                            reciver->setKind(VarDeclKind::RecieverVar);

                                                                                                          llvm::SmallVector<const Type*, 4> paramsTypes;
                                                                                                          for (auto &param : $6) {
                                                                                                            paramsTypes.push_back(param->Type.get());
                                                                                                          }

                                                                                                          $$ = std::make_unique<FuncDecl>(std::move($3), std::move($6),
                                                                                                                                          ctx.getType($8)->clone(),
                                                                                                                                          std::move($11), std::move(reciver));
                                                                                                          ctx.exitScope();
                                                                                                         };

Reciver: "(" ID ID ")"  {
                          auto varDecl = std::make_unique<VarDecl>(std::move($2), ctx.getType($3)->clone());
                          varDecl->setKind(VarDeclKind::RecieverVar);
                          $$ = std::move(varDecl);
                        }
       |  %empty        {};

List_of_params: List_of_params1 { $$ = std::move($1); }
              | %empty          { };
List_of_params1:  List_of_params1 "," Var_decl {
                                                $1.push_back(std::move($3));
                                                $1.back()->setKind(VarDeclKind::ParamVar);
                                                $$ = std::move($1);
                                               }
               | Var_decl                      {
                                                $$.push_back(std::move($1));
                                                $$.back()->setKind(VarDeclKind::ParamVar);
                                               };

Block: "{" { ctx.enterScope(); } List_of_stmt { ctx.exitScope(); } "}" { $$ = std::make_unique<BlockStmt>(std::move($3)); };

List_of_stmt: List_of_stmt Stmt {
                                  $1.push_back(std::move($2));
                                  $$ = std::move($1);
                                }
            | %empty            { };

Stmt: FOR "(" { ctx.enterScope(); } Local_var_decl ";" Expr ";" Expr ")" Stmt { $$ = std::make_unique<ForStmt>(std::move($4), std::move($6), std::move($8), std::move($10)); }
    | IF "(" Expr ")" Stmt List_of_elsif { $$ = std::make_unique<IfStmt>(std::move($3), std::move($5), std::move($6)); }
    | IF "(" Expr ")" Stmt List_of_elsif ELSE Stmt { $$ = std::make_unique<IfStmt>(std::move($3), std::move($5), std::move($6), std::move($8)); }
    | WHILE "(" Expr ")" Stmt { $$ = std::make_unique<WhileStmt>(std::move($3), std::move($5)); }
    | RET Expr ";" { $$ = std::make_unique<ReturnStmt>(std::move($2)); }
    | DEFER Expr ";" { $$ = std::make_unique<DeferStmt>(std::move($2)); }
    | DEALLOC Expr ";"  {
                          auto expr = std::move($2);
                          if (!llvm::isa<PtrType>(&expr->getType())) {
                            llvm::llvm_unreachable_internal("Unreachable Dealloc");
                          }
                          $$ = std::make_unique<DeallocStmt>(std::move(expr));
                        }
    | Expr ";"           { $$ = std::make_unique<ExprStmt>(std::move($1)); }
    | Local_var_decl ";" { $$ = std::move($1); }
    | Block              { $$ = std::move($1); };

Local_var_decl: VAR ID ":" Type_declaration "=" Expr  {
                                          ctx.checkNonExistenceOfVarCurrentLevel($2);
                                          auto varDecl = std::make_unique<VarDecl>($2, std::move($4));
                                          varDecl->setKind(VarDeclKind::LocalVar);
                                          ctx.addVar(*varDecl);
                                          $$ = std::make_unique<VarDeclStmt>(std::move(varDecl), std::move($6));
                                        }
              | VAR ID ":=" Expr        {
                                          ctx.checkNonExistenceOfVarCurrentLevel($2);
                                          auto expr = std::move($4);
                                          auto varDecl = std::make_unique<VarDecl>($2, expr->getType().clone());
                                          varDecl->setKind(VarDeclKind::LocalVar);
                                          ctx.addVar(*varDecl);
                                          $$ = std::make_unique<VarDeclStmt>(std::move(varDecl), std::move(expr));
                                        }
              | VAR ID ":" Type_declaration           {
                                          ctx.checkNonExistenceOfVarCurrentLevel($2);
                                          auto varDecl = std::make_unique<VarDecl>($2, std::move($4));
                                          varDecl->setKind(VarDeclKind::LocalVar);
                                          ctx.addVar(*varDecl);
                                          $$ = std::make_unique<VarDeclStmt>(std::move(varDecl), nullptr);
                                        };

List_of_elsif: List_of_elsif ELSIF "(" Expr ")" Stmt  {
                                                        $1.emplace_back(std::move($4), std::move($6));
                                                        $$ = std::move($1);
                                                      }
             | %empty                                 {};


/*List_of_qualifiers: List_of_qualifiers Qualifier
                  | %empty;

Qualifier: MUT;*/

Expr: "(" Expr ")"       { $$ = std::move($2); }
    | Expr "+" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::Add, std::move($1), std::move($3)); }
    | Expr "-" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::Minus, std::move($1), std::move($3)); }
    | Expr "*" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::Mul, std::move($1), std::move($3)); }
    | Expr "/" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::Div, std::move($1), std::move($3)); }
    | Expr "%" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::Mod, std::move($1), std::move($3)); }
    | Expr "=" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::Assign, std::move($1), std::move($3)); }
    | Expr "!=" Expr     { $$ = BinaryExpr::Create(BinaryOperatorKind::NotEquals, std::move($1), std::move($3)); }
    | Expr "==" Expr     { $$ = BinaryExpr::Create(BinaryOperatorKind::Equals, std::move($1), std::move($3)); }
    | Expr "&&" Expr     { $$ = BinaryExpr::Create(BinaryOperatorKind::LogicalAnd, std::move($1), std::move($3)); }
    | Expr "||" Expr     { $$ = BinaryExpr::Create(BinaryOperatorKind::LogicalOr, std::move($1), std::move($3)); }
    | Expr "<" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::LogicalLess, std::move($1), std::move($3)); }
    | Expr "<=" Expr     { $$ = BinaryExpr::Create(BinaryOperatorKind::LogicalLessEqual, std::move($1), std::move($3)); }
    | Expr ">" Expr      { $$ = BinaryExpr::Create(BinaryOperatorKind::LogicalGreater, std::move($1), std::move($3)); }
    | Expr ">=" Expr     { $$ = BinaryExpr::Create(BinaryOperatorKind::LogicalGreaterEqual, std::move($1), std::move($3)); }
    | "!" Expr           { $$ = UnaryExpr::Create(UnaryOperatorKind::LogicalNegation, std::move($2)); }
    | "-" Expr %prec "&" { $$ = UnaryExpr::Create(UnaryOperatorKind::ArithmeticNegation, std::move($2)); }
    | "*" Expr %prec "&" {
                          auto ptrType = llvm::dyn_cast<PtrType>(&$2->getType());
                          if (!ptrType) {
                            throw quark::QuarkParser::syntax_error("Deferencing non pointer");
                          }
                          $$ = std::make_unique<DereferenceExpr>(UnaryExpr::Create(UnaryOperatorKind::Dereference, std::move($2)));
                         }
    | "&" Expr           {
                          $$ = std::make_unique<AddressofExpr>(UnaryExpr::Create(UnaryOperatorKind::AddressOf, std::move($2)));
                         }
    | Func_call          { $$ = std::move($1); }
    | Method_call        { $$ = std::move($1); }
    | ALLOC ID {
                                $$ = std::make_unique<AllocExpr>(std::make_unique<PtrType>(ctx.getType($2)->clone()),
                                                                                           std::make_unique<IntegerExpr>(1));
                              }
    | ALLOC ID "[" Expr "]" {
                              auto ptrType = std::make_unique<PtrType>(ctx.getType($2)->clone());
                              $$ = std::make_unique<AllocExpr>(std::move(ptrType), std::move($4));
                            }
    | Term { $$ = std::move($1); };

Type_declaration: List_of_pointers ID List_of_static_arrays  {
                                                        std::unique_ptr<Type> result = ctx.getType($2)->clone();
                                                        for(unsigned i = 0; i < $1; i++) {
                                                          result = std::make_unique<PtrType>(std::move(result));
                                                        }
                                                        for(unsigned size : $3) {
                                                          result = std::make_unique<ArrayType>(std::move(result), size);
                                                        }
                                                        $$ = std::move(result);
                                                      }

List_of_dinamic_arrays: List_of_dinamic_arrays "[" Expr "]" {
                                                              $1.push_back(std::move($3));
                                                              $$ = std::move($1);
                                                            }
                      | %empty {};

List_of_static_arrays: List_of_static_arrays "[" INTEGER "]"  {
                                                                $1.push_back($3);
                                                                $$ = std::move($1);
                                                              }
              | %empty {};

List_of_pointers: List_of_pointers "*" { $$ += $1 + 1; }
                | %empty {};

Term: Var_access_array_subscript  { $$ = std::move($1); }
    | Member_access               { $$ = std::move($1); }
    | Literal                     { $$ = std::move($1); };

Func_call: ID "(" List_of_expr ")" {
                                      llvm::SmallVector<const Type*, 4> paramsTypes;
                                      for (auto &expr : $3) {
                                        paramsTypes.push_back(&expr->getType());
                                      }
                                      const FuncDecl *func = ctx.getFunctionDecl($1, paramsTypes, nullptr);
                                      $$ = std::make_unique<FunctionCallExpr>(*func, std::move($3));
                                   };

Method_call: Var_access_array_subscript List_of_accesses "(" List_of_expr ")" {
                                                                                std::unique_ptr<Expr> memberExpr = std::move($1);
                                                                                llvm::MutableArrayRef<TypeAccess> accesses = $2;
                                                                                while (accesses.size() > 1) {
                                                                                  memberExpr = getMemberAccess(std::move(memberExpr), accesses[0]);
                                                                                  accesses = accesses.take_back(accesses.size() - 1);
                                                                                }

                                                                                memberExpr = derefererenceIfNeeded(std::move(memberExpr), accesses[0]);

                                                                                llvm::SmallVector<const Type *, 8> types;
                                                                                for (const std::unique_ptr<Expr> &e : $4) {
                                                                                  types.push_back(&e->getType());
                                                                                }

                                                                                $$ = std::make_unique<MemberCallExpr>(
                                                                                    *ctx.getFunctionDecl(accesses[0].Name, types, &memberExpr->getType()),
                                                                                    std::move(memberExpr), $4);
                                                                              };

List_of_expr: List_of_expr1 { $$ = std::move($1); }
            | %empty        {};
List_of_expr1: List_of_expr1 "," Expr {
                                        $1.push_back(std::move($3));
                                        $$ = std::move($1);
                                      }
             | Expr                   { $$.push_back(std::move($1)); };

Var_access_array_subscript: Var_access List_of_dinamic_arrays {
                                                                std::unique_ptr<Expr> result = std::move($1);
                                                                for (auto &arrayAccess : $2) {
                                                                  std::unique_ptr<Type> innerType = getArrayAccessType(*result);
                                                                  result = std::make_unique<ArrayAccessExpr>(std::move(result), std::move(innerType), std::move(arrayAccess));
                                                                }
                                                                $$ = std::move(result);
                                                              };

Var_access: ID {
                $$ = std::make_unique<VarRefExpr>(*ctx.getVar($1));
               };

Member_access: Var_access_array_subscript List_of_accesses {
                                              std::unique_ptr<Expr> memberAccess = std::move($1);
                                              for (auto &access : $2) {
                                                memberAccess = getMemberAccess(std::move(memberAccess), access);
                                              }
                                              $$ = std::move(memberAccess);
                                           }

List_of_accesses: List_of_accesses Subtype_access {
                                                    $1.push_back(std::move($2));
                                                    $$ = std::move($1);
                                                  }
                | Subtype_access                  {
                                                    $$.push_back(std::move($1));
                                                  };

Subtype_access: "." ID List_of_dinamic_arrays  { $$ = {TypeAccessKind::Value, $2, std::move($3)}; }
              | "->" ID List_of_dinamic_arrays { $$ = {TypeAccessKind::Pointer, $2, std::move($3)}; };

Literal: ConstNumber  { $$ = std::move($1); }
       | STRING       { $$ = std::make_unique<StringExpr>($1); }
       | CHAR         { $$ = std::make_unique<CharExpr>($1); } ;

ConstNumber: INTEGER  { $$ = std::make_unique<IntegerExpr>($1); }
           | REAL     { $$ = std::make_unique<FloatingExpr>($1); };
%%
