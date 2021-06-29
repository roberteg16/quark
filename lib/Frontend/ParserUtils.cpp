#include <quark/Frontend/ParserUtils.h>

#include <../build/lib/Frontend/QuarkParser.hpp>
#include <llvm/Support/Casting.h>

using namespace quark;

std::unique_ptr<quark::Expr>
quark::derefererenceIfNeeded(std::unique_ptr<quark::Expr> expr,
                             const quark::TypeAccess &access) {

  const PtrType *ptrType = llvm::dyn_cast<PtrType>(&expr->getType());
  if ((ptrType && access.Kind != TypeAccessKind::Pointer) ||
      (!ptrType && access.Kind != TypeAccessKind::Value)) {
    if (ptrType) {
      throw quark::QuarkParser::syntax_error("Did you mean '->'?");
    } else {
      throw quark::QuarkParser::syntax_error("Did you mean '.'?");
    }
  }

  if (ptrType) {
    expr = std::make_unique<DereferenceExpr>(std::move(expr));
  }

  return expr;
}

std::unique_ptr<quark::Expr>
quark::getMemberAccess(std::unique_ptr<quark::Expr> expr,
                       quark::TypeAccess &access) {
  expr = derefererenceIfNeeded(std::move(expr), access);
  auto type = &expr->getType();

  auto compType = llvm::dyn_cast<quark::CompoundType>(type);
  if (!compType) {
    throw quark::QuarkParser::syntax_error("Not a compound type");
  }
  auto field = compType->Decl.findField(access.Name);
  if (!field) {
    throw quark::QuarkParser::syntax_error("Field does not exists");
  }

  expr = std::make_unique<quark::MemberExpr>(std::move(expr), *field);

  for (std::unique_ptr<Expr> &arrayAccess : access.ArrayAccesses) {
    std::unique_ptr<Type> innerType = getArrayAccessType(*expr);
    expr = std::make_unique<ArrayAccessExpr>(
        std::move(expr), std::move(innerType), std::move(arrayAccess));
  }

  return expr;
}

std::unique_ptr<quark::Type>
quark::getArrayAccessType(const quark::Expr &expr) {
  const Type &type = expr.getType();
  const auto *ptrType = llvm::dyn_cast<PtrType>(&type);
  const auto *arrayType = llvm::dyn_cast<ArrayType>(&type);
  if (ptrType) {
    return ptrType->PointeeType->clone();
  } else if (arrayType) {
    return arrayType->RealType->clone();
  }

  throw quark::QuarkParser::syntax_error(
      "Accessing with [] non pointer nor array");
}
