#ifndef __QUARK_FRONTEND_EXPR_H__
#define __QUARK_FRONTEND_EXPR_H__

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/raw_ostream.h>

namespace quark {

struct Type;
struct DeclVar;

enum class ExprKind {
  BinaryExpr,
  UnaryExpr,
  FunctionCallExpr,
  VarRefExpr,
  AllocExpr
};

struct Expr {
  Expr(ExprKind kind) : Kind(kind){};
  virtual ~Expr() = 0;

  virtual void print(llvm::raw_ostream &) = 0;
  virtual void dump() = 0;

  ExprKind Kind;
};

enum class BinaryOperatorKind {
  Add,
  Minus,
  Mul,
  Div,
  Mod,
  Assign,
  NotEquals,
  Equals,
  LogicalAnd,
  LogicalOr,
  LogicalLess,
  LogicalLessEqual,
  LogicalGreater,
  LogicalGreaterEqual,
};

llvm::StringRef ToString(BinaryOperatorKind);

struct BinaryExpr : public Expr {
  BinaryExpr() : Expr(ExprKind::BinaryExpr) {}
  virtual ~BinaryExpr();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<Expr> Lhs;
  std::unique_ptr<Expr> Rhs;
  BinaryOperatorKind Op;
};

enum class UnaryOperatorKind {
  LogicalNegation,
  ArithmeticNegation,
  Dereference,
  AddressOf
};

llvm::StringRef ToString(UnaryOperatorKind);

struct UnaryExpr : public Expr {
  UnaryExpr() : Expr(ExprKind::UnaryExpr) {}
  virtual ~UnaryExpr();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  std::unique_ptr<Expr> Lhs;
  UnaryOperatorKind Kind;
};

struct FunctionCallExpr : public Expr {
  FunctionCallExpr(const Type *returnType = nullptr)
      : Expr(ExprKind::FunctionCallExpr), ReturnType(returnType) {}
  virtual ~FunctionCallExpr();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  /// Void means void
  const Type *ReturnType;
};

struct VarRefExpr : public Expr {
  VarRefExpr(const DeclVar &var) : Expr(ExprKind::VarRefExpr), RefVar(var) {}
  virtual ~VarRefExpr();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  const DeclVar &RefVar;
};

struct AllocExpr : public Expr {
  AllocExpr(const Type &type) : Expr(ExprKind::AllocExpr), Type(type) {}
  virtual ~AllocExpr();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  const Type &Type;
};

} // namespace quark

#endif // __QUARK_FRONTEND_EXPR_H__
