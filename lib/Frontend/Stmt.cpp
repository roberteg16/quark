#include <quark/Frontend/Stmt.h>

#include <quark/Frontend/ASTDumper.h>
#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Expr.h>

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace quark;

Stmt::~Stmt() {}
BlockStmt::~BlockStmt() {}
ForStmt::~ForStmt() {}
IfStmt::~IfStmt() {}
WhileStmt::~WhileStmt() {}
ReturnStmt::~ReturnStmt() {}
DeferStmt::~DeferStmt() {}
DeallocStmt::~DeallocStmt() {}
VarDeclStmt::~VarDeclStmt() {}
ExprStmt::~ExprStmt() {}

void Stmt::print(llvm::raw_ostream &out) { ASTDumper{out}.dump(*this); }

void Stmt::dump() { print(llvm::dbgs()); }

IfStmt::CondAndStmt::CondAndStmt(std::unique_ptr<Expr> cond,
                                 std::unique_ptr<::Stmt> stmt)
    : Cond(std::move(cond)), Stmt(std::move(stmt)) {}
