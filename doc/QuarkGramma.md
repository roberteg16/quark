# **Grammar of Quark 0.1**

----

**Start** &rightarrow; **Module_preamble** **List_of_decls**

**Module_preamble** &rightarrow; **Export_module** **List_of_imports**

**Export_module** &rightarrow; *export module* **Module_name**
**Export_module** &rightarrow; &epsilon;

**List_of_imports** &rightarrow; **List_of_imports** *import* **Module_name**
**List_of_imports** &rightarrow; &epsilon;

**Module_name**  &rightarrow; *identifier* **Doted_name**
**Doted_name** &rightarrow; **Doted_name** *. identifier*
**Doted_name** &rightarrow; &epsilon;

**List_of_decls** &rightarrow; **List_of_decls** **Declaration**
**List_of_decls** &rightarrow; &epsilon;

**Declaration** &rightarrow; **Declaration_type**
**Declaration** &rightarrow; **Declaration_func**

**Var_decl** &rightarrow; *identifier identifier*
**Var_decl** &rightarrow; *identifier identifier [* **Expr** *]*

**Declaration_type** &rightarrow; *type identifier*  *{ **List_of_vars** }*
**Declaration_type** &rightarrow; *type identifier* **Type**

**List_of_vars** &rightarrow; **List_of_vars** **Var_decl**
**List_of_vars** &rightarrow; &epsilon;

**Declaration_func** &rightarrow; *fn* **Reciver** *identifier :* **List_of_var_delcs** *-> identifier* **Block**
**Declaration_func** &rightarrow; *fn* **Reciver** *identifier :* **List_of_var_delcs** **Block**

**Reciver** &rightarrow; *( identifier & identifier )*
**Reciver** &rightarrow; &epsilon;

**List_of_var_decls** &rightarrow; **List_of_decls1**
**List_of_var_decls** &rightarrow; &epsilon;
**List_of_var_decls1** &rightarrow; **List_of_decls1** *,* **Var_decl**
**List_of_var_decls1** &rightarrow; **Var_decl**

**Block** &rightarrow;  *{* **List_of_stmt** *}*

**List_of_stmt** &rightarrow; **List_of_stmt** **Stmt**
**List_of_stmt** &rightarrow; &epsilon;

**Stmt** &rightarrow; *for (* **Local_var_decl** *;* **Expr** *;*  **Expr** *)* **Stmt**
**Stmt** &rightarrow; *if (* **Expr** *)* **Stmt** **List_of_elsif** **Final_else**
**Stmt** &rightarrow; *if (* **Expr** *)* **Stmt** **List_of_elsif** *else* **Stmt**
**Stmt** &rightarrow; *while (* **Expr** *)* **Stmt**
**Stmt** &rightarrow; *while (* **Expr** *)* **Stmt**
**Stmt** &rightarrow; *ret* **Expr** *endl*
**Stmt** &rightarrow; *defer* **Expr** *endl*
**Stmt** &rightarrow; *dealloc* **Expr** *endl*
**Stmt** &rightarrow; **Expr** *endl*
**Stmt** &rightarrow; **Local_var_decl** *endl*
**Stmt** &rightarrow; **Block**

**Local_var_decl** &rightarrow; *var identifier : type =* **Expr**
**Local_var_decl** &rightarrow; *var identifier : =* **Expr**

**List_of_elsif** &rightarrow; **List_of_elsif** *elsif (* **Expr** *)* **Stmt**
**List_of_elsif** &rightarrow; &epsilon;

**List_of_qualifiers** &rightarrow; **List_of_qualifiers** **Qualifier**
**List_of_qualifiers** &rightarrow; &epsilon;

**Qualifier** &rightarrow; *mut*

**Expr** &rightarrow; *(* **Expr** *)*
**Expr** &rightarrow; **Expr** *+* **Expr**
**Expr** &rightarrow; **Expr** *-* **Expr**
**Expr** &rightarrow; **Expr** *\** **Expr**
**Expr** &rightarrow; **Expr** */* **Expr**
**Expr** &rightarrow; **Expr** *%* **Expr**
**Expr** &rightarrow; **Expr** *=* **Expr**
**Expr** &rightarrow; **Expr** *!=* **Expr**
**Expr** &rightarrow; **Expr** *==* **Expr**
**Expr** &rightarrow; **Expr** *&&* **Expr**
**Expr** &rightarrow; **Expr** *||* **Expr**
**Expr** &rightarrow; **Expr** *<* **Expr**
**Expr** &rightarrow; **Expr** *<=* **Expr**
**Expr** &rightarrow; **Expr** *>* **Expr**
**Expr** &rightarrow; **Expr** *>=* **Expr**
**Expr** &rightarrow; *!* **Expr**
**Expr** &rightarrow; *-* **Expr**
**Expr** &rightarrow; *\** **Expr**
**Expr** &rightarrow; *&* **Expr**
**Expr** &rightarrow; **Func_call**
**Expr** &rightarrow; **Method_call**
**Expr** &rightarrow; *alloc identifier*
**Expr** &rightarrow; *alloc identifier [* **Expr** *]*
**Expr** &rightarrow; **Term**

**Term** &rightarrow; *identifier*
**Term** &rightarrow; *identifier [* **Expr** *]*
**Term** &rightarrow; **Literal**

**Func_call** &rightarrow; *identifier* *(* **List_of_expr** *)*
**Method_call** &rightarrow; *identifier* **List_of_subtype_accesses** *(* **List_of_expr** *)*

**List_of_expr** &rightarrow; **List_of_expr1**
**List_of_expr** &rightarrow; &epsilon;
**List_of_expr1** &rightarrow; **List_of_expr1** *,* **Expr**
**List_of_expr1** &rightarrow; **Expr**

**List_of_subtype_accesses** &rightarrow; **List_of_subtype_accesses** **Subtype_access**
**List_of_subtype_accesses** &rightarrow; **Subtype_access**

**Subtype_access** &rightarrow; *. identifier*
**Subtype_access** &rightarrow; *-> identifier*

**Literal** &rightarrow; **ConstNumber**
**Literal** &rightarrow; *ConstString*
**Literal** &rightarrow; *char*

**ConstNumber** &rightarrow; *integer*
**ConstNumber** &rightarrow; *real*
