#include <quark/Frontend/Expr.h>

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace quark;


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

BinaryExpr::~BinaryExpr() {}

void BinaryExpr::print(llvm::raw_ostream &outs) { outs << "BinaryExpr\n"; }
void BinaryExpr::dump() { print(llvm::dbgs()); }

enum class UnaryOperatorKind {
  LogicalNegation,
  ArithmeticNegation,
  Dereference,
  AddressOf
};

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

UnaryExpr::~UnaryExpr() {}

void UnaryExpr::print(llvm::raw_ostream &outs) { outs << "UnaryExpr\n"; }
void UnaryExpr::dump() { print(llvm::dbgs()); }

FunctionCallExpr::~FunctionCallExpr() {}

void FunctionCallExpr::print(llvm::raw_ostream &outs) {
  outs << "FunctionCallExpr\n";
}
void FunctionCallExpr::dump() { print(llvm::dbgs()); }

VarRefExpr::~VarRefExpr() {}

void VarRefExpr::print(llvm::raw_ostream &outs) { outs << "VarRefExpr\n"; }
void VarRefExpr::dump() { print(llvm::dbgs()); }

AllocExpr::~AllocExpr() {}

void AllocExpr::print(llvm::raw_ostream &outs) { outs << "AllocExpr\n"; }
void AllocExpr::dump() { print(llvm::dbgs()); }