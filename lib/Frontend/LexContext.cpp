#include "quark/Frontend/Expr.h"
#include <cstdlib>
#include <llvm-12/llvm/Support/Casting.h>
#include <llvm-12/llvm/Support/Debug.h>
#include <llvm-12/llvm/Support/raw_ostream.h>
#include <quark/Frontend/LexContext.h>

#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>

#include <../build/lib/Frontend/QuarkParser.hpp>
#include <llvm/ADT/ArrayRef.h>

using namespace quark;

static bool AreReciversEqual(const VarDecl *lhs, const Type *rhs) {
  if ((lhs && !rhs) || (!lhs && rhs)) {
    return false;
  }
  if (!lhs && !rhs) {
    return true;
  }
  return *lhs->Type == *rhs;
}

static void throwSyntaxError(llvm::Twine twine) {
  throw quark::QuarkParser::syntax_error(twine.str());
}

const FuncDecl *LexContext::findFunctionDecl(const FuncDecl &funcDecl) const {
  for (const FuncDecl &rhsFunc : FunctionDecls) {
    if (rhsFunc == funcDecl) {
      return &rhsFunc;
    }
  }
  return nullptr;
}

const FuncDecl *
LexContext::findFunctionDecl(llvm::StringRef name,
                             llvm::ArrayRef<const Type *> params,
                             const Type *reciver) const {
  for (const FuncDecl &rhsFunc : FunctionDecls) {
    // Discard by number of params or name
    if (rhsFunc.Params.size() != params.size() || rhsFunc.Name != name) {
      continue;
    }
    // Discard by recivers
    if (!AreReciversEqual(rhsFunc.Reciver.get(), reciver)) {
      continue;
    }
    // Discard by types of params
    // TODO: Alias type are not considered the same to what they alias
    for (const auto pair : llvm::zip(rhsFunc.Params, params)) {
      if (!(*std::get<0>(pair)->Type == *std::get<1>(pair))) {
        continue;
      }
    }
    return &rhsFunc;
  }

  return nullptr;
}

void LexContext::checkNonExistenceOfFunction(const FuncDecl &funcDecl) {
  if (findFunctionDecl(funcDecl))
    throwSyntaxError(llvm::Twine("type '") + funcDecl.Name +
                     "' already defined!");
}

void LexContext::checkNonExistenceOfVar(llvm::StringRef id) {
  if (findVar(id))
    throwSyntaxError(llvm::Twine("type '") + id + "' already defined!");
}

void LexContext::checkNonExistenceOfVarCurrentLevel(llvm::StringRef id) {
  if (findVarCurrentLevel(id))
    throwSyntaxError(llvm::Twine("type '") + id + "' already defined!");
}

void LexContext::checkNonExistenceOfType(llvm::StringRef id) {
  if (findType(id))
    throwSyntaxError(llvm::Twine("type '") + id + "' already defined!");
}

const Type *LexContext::findType(llvm::StringRef id) const {
  for (const BuiltinType &type : BuiltinTypes) {
    if (type.Name == id) {
      return &type;
    }
  }

  for (const TypeDecl &type : TypeDecls) {
    if (type.Name == id) {
      return &type.Type;
    }
  }

  return nullptr;
}

const TypeDecl *LexContext::findTypeDecl(llvm::StringRef id) const {
  for (const TypeDecl &typeDecl : TypeDecls) {
    if (typeDecl.Name == id) {
      return &typeDecl;
    }
  }

  return nullptr;
}

const VarDecl *LexContext::findVarCurrentLevel(llvm::StringRef id) const {
  for (const VarDecl *varDecl : Scopes.back()) {
    if (varDecl->Name == id) {
      return varDecl;
    }
  }

  return nullptr;
}

const VarDecl *LexContext::findVar(llvm::StringRef id) const {
  for (const Scope &scope : llvm::reverse(Scopes)) {
    for (const VarDecl *varDecl : scope) {
      if (varDecl->Name == id) {
        return varDecl;
      }
    }
  }

  return nullptr;
}

const FuncDecl *LexContext::getFunctionDecl(const FuncDecl &funcDecl) const {
  const FuncDecl *funcDeclPtr = findFunctionDecl(funcDecl);
  if (!funcDeclPtr) {
    throwSyntaxError(llvm::Twine("function '") + funcDecl.Name +
                     "' not defined!");
  }

  return funcDeclPtr;
}

const FuncDecl *LexContext::getFunctionDecl(llvm::StringRef name,
                                            llvm::ArrayRef<const Type *> params,
                                            const Type *reciver) const {
  const FuncDecl *funcDecl = findFunctionDecl(name, params, reciver);
  if (!funcDecl) {
    if (reciver) {
      throwSyntaxError(llvm::Twine("method (") + ") '" + name +
                       "' not defined!");
    } else {
      throwSyntaxError(llvm::Twine("function '") + name + "' not defined!");
    }
  }

  return funcDecl;
}

const Type *LexContext::getType(llvm::StringRef id) const {
  const Type *type = findType(id);
  if (!type) {
    throwSyntaxError(llvm::Twine("type '") + id + "' not found!");
  }

  return type;
}

const TypeDecl *LexContext::getTypeDecl(llvm::StringRef id) const {
  const TypeDecl *typeDecl = findTypeDecl(id);
  if (!typeDecl) {
    throwSyntaxError(llvm::Twine("type declaration '") + id + "' not found!");
  }

  return typeDecl;
}

const VarDecl *LexContext::getVarCurrentLevel(llvm::StringRef id) const {
  const VarDecl *varDecl = findVarCurrentLevel(id);
  if (!varDecl) {
    throwSyntaxError(llvm::Twine("variable '") + id + "' not defined!");
  }

  return varDecl;
}

const VarDecl *LexContext::getVar(llvm::StringRef id) const {
  const VarDecl *varDecl = findVar(id);
  if (!varDecl) {
    throwSyntaxError(llvm::Twine("variable '") + id + "' not defined!");
  }

  return varDecl;
}

const FuncDecl *
LexContext::LexContext::addFunctionDecl(const FuncDecl &funcDecl) {
  checkNonExistenceOfFunction(funcDecl);

  FunctionDecls.push_back(funcDecl);
  return &FunctionDecls.back().get();
}

const TypeDecl *LexContext::addTypeDecl(const TypeDecl &typeDecl) {
  checkNonExistenceOfType(typeDecl.Name);

  TypeDecls.push_back(typeDecl);
  return &TypeDecls.back().get();
}

const VarDecl *LexContext::addVar(const VarDecl &var) {
  checkNonExistenceOfVarCurrentLevel(var.Name);

  Scopes.back().push_back(&var);
  return &var;
}

void LexContext::enterScope() { Scopes.emplace_back(); }

void LexContext::exitScope() { Scopes.pop_back(); }
