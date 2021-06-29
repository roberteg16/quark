#ifndef __QUARK_FRONTEND_EXPR_H__
#define __QUARK_FRONTEND_EXPR_H__

#include <quark/Frontend/Type.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/raw_ostream.h>

namespace quark {

struct VarDecl;
struct TypeFieldDecl;
struct FuncDecl;
struct Decl;
struct LexContext;

enum class ExprKind {
  BinaryExpr,
  UnaryExpr,
  FunctionCallExpr,
  MemberCallExpr,
  MemberExpr,
  VarRefExpr,
  AllocExpr,
  IntegerExpr,
  CharExpr,
  FloatingExpr,
  StringExpr,
  DereferenceExpr,
  AddressofExpr,
  ArrayAccessExpr
};

struct Expr {
  Expr(ExprKind kind, std::unique_ptr<Type> exprType)
      : Kind(kind), ExprType(std::move(exprType)) {}
  virtual ~Expr() = 0;

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  const Type &getType() const { return *ExprType; }
  virtual ExprKind getKind() const { return Kind; }

  ExprKind Kind;
  std::unique_ptr<Type> ExprType;
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
  BinaryExpr(BinaryOperatorKind op, std::unique_ptr<Expr> lhs,
             std::unique_ptr<Expr> rhs);
  virtual ~BinaryExpr() {}

  static std::unique_ptr<BinaryExpr> Create(BinaryOperatorKind op,
                                            std::unique_ptr<Expr> lhs,
                                            std::unique_ptr<Expr> rhs);

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::BinaryExpr;
  }

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
  UnaryExpr(UnaryOperatorKind kind, std::unique_ptr<Expr> lhs);
  virtual ~UnaryExpr() {}

  static std::unique_ptr<UnaryExpr> Create(UnaryOperatorKind kind,
                                           std::unique_ptr<Expr> lhs);

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::UnaryExpr;
  }

  std::unique_ptr<Expr> Lhs;
  UnaryOperatorKind Op;
};

struct FunctionCallExpr : public Expr {
  FunctionCallExpr(const FuncDecl &funcDecl,
                   llvm::SmallVector<std::unique_ptr<Expr>, 4> params);
  virtual ~FunctionCallExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::FunctionCallExpr;
  }

  const FuncDecl &FunctionDecl;
  llvm::SmallVector<std::unique_ptr<Expr>, 6> Params;
};

struct MemberExpr : public Expr {

  MemberExpr(std::unique_ptr<Expr> expr, const TypeFieldDecl &decl);
  virtual ~MemberExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::MemberExpr;
  }

  const TypeFieldDecl &Field;
  const std::unique_ptr<Expr> Accessed;
};

enum class TypeAccessKind { Unknown, Value, Pointer };
struct TypeAccess {
  TypeAccess() : Kind(TypeAccessKind::Unknown), Name("") {}

  TypeAccess(TypeAccessKind k, llvm::SmallString<10> name,
             std::vector<std::unique_ptr<Expr>> arrayAccesses)
      : Kind(k), Name(std::move(name)),
        ArrayAccesses(std::move(arrayAccesses)) {}

  void dump();

  TypeAccessKind Kind;
  llvm::SmallString<10> Name;
  std::vector<std::unique_ptr<Expr>> ArrayAccesses;
};

struct VarRefExpr : public Expr {
  VarRefExpr(const VarDecl &var);
  virtual ~VarRefExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::VarRefExpr;
  }

  const VarDecl &RefVar;
};

struct ArrayAccessExpr : public Expr {
  ArrayAccessExpr(std::unique_ptr<Expr> refVar, std::unique_ptr<Type> type,
                  std::unique_ptr<Expr> idx);
  virtual ~ArrayAccessExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::ArrayAccessExpr;
  }

  std::unique_ptr<Expr> RefVar;
  std::unique_ptr<Expr> Idx;
};

struct MemberCallExpr : public Expr {
  MemberCallExpr(const FuncDecl &funcDecl, std::unique_ptr<Expr> memberExpr,
                 llvm::SmallVectorImpl<std::unique_ptr<Expr>> &params);
  virtual ~MemberCallExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::MemberCallExpr;
  }

  const FuncDecl &FunctionDecl;
  std::unique_ptr<Expr> Accessor;
  llvm::SmallVector<std::unique_ptr<Expr>, 6> Params;
};

struct AllocExpr : public Expr {
  AllocExpr(std::unique_ptr<Type> type, std::unique_ptr<Expr> size);
  virtual ~AllocExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::AllocExpr;
  }

  const std::unique_ptr<Type> AllocType;
  const std::unique_ptr<Expr> SizeToAlloc;
};

struct StringExpr : public Expr {
  StringExpr(llvm::SmallString<40> v);
  virtual ~StringExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::StringExpr;
  }

  llvm::SmallString<40> Value;
};

struct IntegerExpr : public Expr {
  IntegerExpr(long long v);
  virtual ~IntegerExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::IntegerExpr;
  }

  long long Value;
};

struct CharExpr : public Expr {
  CharExpr(char v);
  virtual ~CharExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::CharExpr;
  }

  char Value;
};

struct FloatingExpr : public Expr {
  FloatingExpr(long double v);
  virtual ~FloatingExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::FloatingExpr;
  }

  long double Value;
};

struct DereferenceExpr : public Expr {
  DereferenceExpr(std::unique_ptr<Expr> expr);
  virtual ~DereferenceExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::DereferenceExpr;
  }

  std::unique_ptr<Expr> DereferencingExpr;
};

struct AddressofExpr : public Expr {
  AddressofExpr(std::unique_ptr<Expr> expr);
  virtual ~AddressofExpr() {}

  static bool classof(const Expr *expr) {
    return expr->getKind() == ExprKind::AddressofExpr;
  }

  std::unique_ptr<Expr> AdressOfExpr;
};

} // namespace quark

#endif // __QUARK_FRONTEND_EXPR_H__
