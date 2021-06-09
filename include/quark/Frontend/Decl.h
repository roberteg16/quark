#ifndef __QUARK_FRONTEND_DECL_H__
#define __QUARK_FRONTEND_DECL_H__

#include <llvm/ADT/Optional.h>
#include <llvm/ADT/SmallVector.h>

namespace quark {

/// Forward declarations
struct Stmt;
struct BlockStmt;
struct Type;

struct Decl {
  Decl(std::string name) : Name(std::move(name)) {}
  virtual ~Decl() = 0;

  virtual void print(llvm::raw_ostream &) = 0;
  virtual void dump() = 0;

  const std::string Name;
};

enum class VarDeclKind { LocalVar, ParamVar, RecieverVar };

struct VarDecl : public Decl {
  VarDecl(VarDeclKind kind, std::string name, const Type &type)
      : Decl(std::move(name)), Type(type), Kind(kind) {}
  virtual ~VarDecl();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  const Type &Type;
  VarDeclKind Kind;
};

struct FuncDecl : public Decl {
  FuncDecl(std::string name, const Type &type)
      : Decl(std::move(name)), ReturnType(&type) {}
  FuncDecl(std::string name) : Decl(std::move(name)) {}
  virtual ~FuncDecl();

  bool isVoid() { return ReturnType; }
  bool isMethod() { return Reciver.get(); }

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  /// Params of the function
  llvm::SmallVector<std::unique_ptr<VarDecl>, 4> Params;

  /// Reciver, if any, of the function
  std::unique_ptr<VarDecl> Reciver;

  /// Return type of the function. Nullptr means no value is returned
  const Type *ReturnType;

  /// Body of the function
  std::unique_ptr<BlockStmt> Body;
};

struct TypeFieldDecl : public Decl {
  TypeFieldDecl(std::string name, const Type &type)
      : Decl(std::move(name)), Type(type) {}
  virtual ~TypeFieldDecl();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  const Type &Type;
};

struct TypeDecl : public Decl {
  TypeDecl(std::string name, const Type &type) : Decl(std::move(name)) {}
  virtual ~TypeDecl();

  virtual void print(llvm::raw_ostream &);
  virtual void dump();

  llvm::SmallVector<TypeFieldDecl, 4> FieldDecls;
};

} // namespace quark

#endif // __QUARK_FRONTEND_DECL_H__
