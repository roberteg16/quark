#ifndef __QUARK_FRONTEND_PARSERUTILS_H__
#define __QUARK_FRONTEND_PARSERUTILS_H__

#include <quark/Frontend/LexContext.h>

namespace quark {

std::unique_ptr<quark::Expr>
derefererenceIfNeeded(std::unique_ptr<quark::Expr> expr,
                      const quark::TypeAccess &access);

std::unique_ptr<quark::Expr> getMemberAccess(std::unique_ptr<quark::Expr> expr,
                                             quark::TypeAccess &access);

std::unique_ptr<quark::Type> getArrayAccessType(const quark::Expr &expr);

} // namespace quark

#endif // __QUARK_FRONTEND_PARSERUTILS_H__
