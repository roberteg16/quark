#ifndef __QUARK_FRONTEND_LEXCONTEXT_H__
#define __QUARK_FRONTEND_LEXCONTEXT_H__

#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Expr.h>
#include <quark/Frontend/SourceModule.h>
#include <quark/Frontend/Type.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace quark {

struct NamedType;
struct Type;

using Scope = llvm::SmallVector<const VarDecl *, 5>;

struct LexContext {
  LexContext() : BuiltinTypes(SourceModule::BuiltinTypes) {}

  const FuncDecl *findFunctionDecl(const FuncDecl &funcDecl) const;
  const FuncDecl *findFunctionDecl(llvm::StringRef name,
                                   llvm::ArrayRef<const Type *> params,
                                   const Type *reciver) const;
  const Type *findType(llvm::StringRef id) const;
  const TypeDecl *findTypeDecl(llvm::StringRef id) const;
  const VarDecl *findVarCurrentLevel(llvm::StringRef id) const;
  const VarDecl *findVar(llvm::StringRef id) const;

  const FuncDecl *getFunctionDecl(const FuncDecl &funcDecl) const;
  const FuncDecl *getFunctionDecl(llvm::StringRef name,
                                  llvm::ArrayRef<const Type *> params,
                                  const Type *reciver) const;
  const Type *getType(llvm::StringRef id) const;
  const TypeDecl *getTypeDecl(llvm::StringRef id) const;
  const VarDecl *getVarCurrentLevel(llvm::StringRef id) const;
  const VarDecl *getVar(llvm::StringRef id) const;

  void checkNonExistenceOfFunction(const FuncDecl &funcDecl);
  void checkNonExistenceOfVar(llvm::StringRef id);
  void checkNonExistenceOfVarCurrentLevel(llvm::StringRef id);
  void checkNonExistenceOfType(llvm::StringRef id);

  const FuncDecl *addFunctionDecl(const FuncDecl &);
  const TypeDecl *addTypeDecl(const TypeDecl &);
  const VarDecl *addVar(const VarDecl &var);

  void enterFunction();
  void exitFunction();

  void enterScope();
  void exitScope();

private:
  /// References to real storage
  std::vector<std::reference_wrapper<const TypeDecl>> TypeDecls;
  std::vector<std::reference_wrapper<const FuncDecl>> FunctionDecls;

  /// Builtin types
  llvm::ArrayRef<BuiltinType> BuiltinTypes;

  /// Scopes
  llvm::SmallVector<Scope, 10> Scopes;
};

} // namespace quark

#endif // __QUARK_FRONTEND_EXPR_H__
