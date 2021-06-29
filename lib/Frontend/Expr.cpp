#include <memory>
#include <quark/Frontend/Expr.h>

#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>

#include <quark/Frontend/ASTDumper.h>
#include <quark/Frontend/LexContext.h>
#include <quark/Frontend/ParserUtils.h>

#include <llvm/Support/Casting.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace quark;

void Expr::print(llvm::raw_ostream &out) { ASTDumper{out}.dump(*this); }

void Expr::dump() { print(llvm::dbgs()); }

Expr::~Expr() {}

llvm::StringRef quark::ToString(BinaryOperatorKind binOp) {
  switch (binOp) {
  case BinaryOperatorKind::Add:
    return "+";
  case BinaryOperatorKind::Minus:
    return "-";
  case BinaryOperatorKind::Mul:
    return "*";
  case BinaryOperatorKind::Div:
    return "/";
  case BinaryOperatorKind::Mod:
    return "%";
  case BinaryOperatorKind::Assign:
    return "=";
  case BinaryOperatorKind::NotEquals:
    return "!=";
  case BinaryOperatorKind::Equals:
    return "==";
  case BinaryOperatorKind::LogicalAnd:
    return "&&";
  case BinaryOperatorKind::LogicalOr:
    return "||";
  case BinaryOperatorKind::LogicalLess:
    return "<";
  case BinaryOperatorKind::LogicalLessEqual:
    return "<=";
  case BinaryOperatorKind::LogicalGreater:
    return ">";
  case BinaryOperatorKind::LogicalGreaterEqual:
    return ">=";
  }
}

llvm::StringRef quark::ToString(UnaryOperatorKind unaryOp) {
  switch (unaryOp) {
  case UnaryOperatorKind::LogicalNegation:
    return "!";
  case UnaryOperatorKind::ArithmeticNegation:
    return "-";
  case UnaryOperatorKind::Dereference:
    return "*";
  case UnaryOperatorKind::AddressOf:
    return "&";
  }
}

// ==== Constructors ==== //
BinaryExpr::BinaryExpr(BinaryOperatorKind op, std::unique_ptr<Expr> lhs,
                       std::unique_ptr<Expr> rhs)
    : Expr(ExprKind::BinaryExpr, lhs->ExprType->clone()), Lhs(std::move(lhs)),
      Rhs(std::move(rhs)), Op(op) {}

UnaryExpr::UnaryExpr(UnaryOperatorKind kind, std::unique_ptr<Expr> lhs)
    : Expr(ExprKind::UnaryExpr, lhs->ExprType->clone()), Lhs(std::move(lhs)),
      Op(kind) {}

FunctionCallExpr::FunctionCallExpr(
    const FuncDecl &funcDecl,
    llvm::SmallVector<std::unique_ptr<Expr>, 4> params)
    : Expr(ExprKind::FunctionCallExpr, funcDecl.ReturnType->clone()),
      FunctionDecl(funcDecl), Params(std::move(params)) {}

MemberExpr::MemberExpr(std::unique_ptr<Expr> expr, const TypeFieldDecl &decl)
    : Expr(ExprKind::MemberExpr, decl.Type->clone()), Field(decl),
      Accessed(std::move(expr)) {}

MemberCallExpr::MemberCallExpr(
    const FuncDecl &funcDecl, std::unique_ptr<Expr> expr,
    llvm::SmallVectorImpl<std::unique_ptr<Expr>> &params)
    : Expr(ExprKind::MemberCallExpr, funcDecl.ReturnType->clone()),
      FunctionDecl(funcDecl), Accessor(std::move(expr)),
      Params(std::move(params)) {}

VarRefExpr::VarRefExpr(const VarDecl &var)
    : Expr(ExprKind::VarRefExpr, var.Type->clone()), RefVar(var) {}

ArrayAccessExpr::ArrayAccessExpr(std::unique_ptr<Expr> refVar,
                                 std::unique_ptr<Type> type,
                                 std::unique_ptr<Expr> idx)
    : Expr(ExprKind::ArrayAccessExpr, std::move(type)),
      RefVar(std::move(refVar)), Idx(std::move(idx)) {}

AllocExpr::AllocExpr(std::unique_ptr<Type> type, std::unique_ptr<Expr> size)
    : Expr(ExprKind::AllocExpr, type->clone()), AllocType(std::move(type)),
      SizeToAlloc(std::move(size)) {}

StringExpr::StringExpr(llvm::SmallString<40> v)
    : Expr(ExprKind::StringExpr,
           std::make_unique<PtrType>(
               std::make_unique<BuiltinType>(BuiltinTypeKind::u8))),
      Value(std::move(v)) {}

IntegerExpr::IntegerExpr(long long v)
    : Expr(ExprKind::IntegerExpr,
           std::make_unique<BuiltinType>(BuiltinTypeKind::i32)),
      Value(v) {}

CharExpr::CharExpr(char v)
    : Expr(ExprKind::CharExpr,
           std::make_unique<BuiltinType>(BuiltinTypeKind::u8)),
      Value(v) {}

FloatingExpr::FloatingExpr(long double v)
    : Expr(ExprKind::FloatingExpr,
           std::make_unique<BuiltinType>(BuiltinTypeKind::f32)),
      Value(v) {}

DereferenceExpr::DereferenceExpr(std::unique_ptr<Expr> expr)
    : Expr(ExprKind::DereferenceExpr,
           llvm::cast<PtrType>(&expr->getType())->PointeeType->clone()),
      DereferencingExpr(std::move(expr)) {}

AddressofExpr::AddressofExpr(std::unique_ptr<Expr> expr)
    : Expr(ExprKind::AddressofExpr,
           std::make_unique<PtrType>(expr->getType().clone())),
      AdressOfExpr(std::move(expr)) {}

// AddressofExpr::AddressofExpr(std::unique_ptr<Expr> expr) :

// ==== End of constructors ==== //

void TypeAccess::dump() {
  if (Kind == TypeAccessKind::Pointer) {
    llvm::dbgs() << "->";
  } else if (Kind == TypeAccessKind::Value) {
    llvm::dbgs() << ".";
  } else {
    llvm::dbgs() << "<unk>";
  }
  llvm::dbgs() << Name;
}

std::unique_ptr<BinaryExpr> BinaryExpr::Create(BinaryOperatorKind op,
                                               std::unique_ptr<Expr> lhs,
                                               std::unique_ptr<Expr> rhs) {
  return std::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
}

std::unique_ptr<UnaryExpr> UnaryExpr::Create(UnaryOperatorKind op,
                                             std::unique_ptr<Expr> lhs) {
  return std::make_unique<UnaryExpr>(op, std::move(lhs));
}
