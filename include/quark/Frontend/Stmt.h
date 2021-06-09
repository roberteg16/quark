#ifndef __QUARK_FRONTEND_STMT_H__
#define __QUARK_FRONTEND_STMT_H__

#include <llvm/ADT/Optional.h>
#include <llvm/ADT/SmallVector.h>

#include <vector>

namespace quark {

/// Forward declarations
struct Decl;
struct VarDecl;
struct Expr;
struct VarRefExpr;

enum class StatementKind {
  BlockStmt = 0,
  ForStmt,
  IfStmt,
  WhileStmt,
  RetStmt,
  DeferStmt,
  DeallocStmt,
  VarDeclStmt,
  ExprStmt,
};

struct Stmt {
  virtual ~Stmt() = 0;

  virtual void print(llvm::raw_ostream &) = 0;
  virtual void dump() = 0;

  StatementKind Kind;
};

struct BlockStmt : public Stmt {
  virtual ~BlockStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::vector<std::unique_ptr<Stmt>> Stmts;
};

struct ForStmt : public Stmt {
  virtual ~ForStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<VarDecl> VarDecl;
  std::unique_ptr<Expr> Cond;
  std::unique_ptr<Expr> Inc;
  std::unique_ptr<BlockStmt> Body;
};

struct IfStmt : public Stmt {
  virtual ~IfStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  struct CondAndStmt {
    std::unique_ptr<Expr> Cond;
    std::unique_ptr<Stmt> Stmt;
  };

  CondAndStmt Cond;
  llvm::SmallVector<CondAndStmt, 4> Elsifs;
  CondAndStmt Else;
};

struct WhileStmt : public Stmt {
  virtual ~WhileStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<Expr> Cond;
  std::unique_ptr<Stmt> Stmt;
};

struct ReturnStmt : public Stmt {
  virtual ~ReturnStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<Expr> ReturnValue;
};

struct DeferStmt : public Stmt {
  virtual ~DeferStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<Expr> ExprToDefer;
};

struct DeallocStmt : public Stmt {
  virtual ~DeallocStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<VarRefExpr> ExprToDealloc;
};

struct VarDeclStmt : public Stmt {
  virtual ~VarDeclStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<VarDecl> VarDecl;
};

struct ExprStmt : public Stmt {
  virtual ~ExprStmt();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<Expr> Expr;
};

} // namespace quark

#endif // __QUARK_FRONTEND_STMT_H__
