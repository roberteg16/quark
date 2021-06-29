#include <quark/Frontend/Decl.h>

#include <quark/Frontend/ASTDumper.h>
#include <quark/Frontend/Expr.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace quark;

void Decl::print(llvm::raw_ostream &out) const { ASTDumper{out}.dump(*this); }

void Decl::dump() const { print(llvm::dbgs()); }

Decl::~Decl() {}

llvm::StringRef quark::ToString(VarDeclKind kind) {
  switch (kind) {
  case VarDeclKind::LocalVar:
    return "local";
  case VarDeclKind::ParamVar:
    return "param";
  case VarDeclKind::RecieverVar:
    return "reciv";
  case VarDeclKind::None:
    llvm::llvm_unreachable_internal("VarDeclKind not setted correctly");
  }
}

VarDecl::~VarDecl() {}

bool VarDecl::operator==(const VarDecl &rhs) const {
  return this->Kind == rhs.Kind && this->Name == rhs.Name &&
         &this->Type == &rhs.Type;
}

FuncDecl::FuncDecl(llvm::StringRef name,
                   llvm::SmallVector<std::unique_ptr<VarDecl>, 4> params,
                   std::unique_ptr<Type> returnType,
                   std::vector<std::unique_ptr<Stmt>> stmts,
                   std::unique_ptr<VarDecl> reciver)
    : Decl(DeclKind::FuncDecl, name), Params(std::move(params)),
      Reciver(std::move(reciver)), ReturnType(std::move(returnType)),
      Body(std::move(stmts)) {}

FuncDecl::~FuncDecl() {}

static bool AreReciversEqual(const VarDecl *lhs, const VarDecl *rhs) {
  if ((lhs && !rhs) || (!lhs && rhs)) {
    return false;
  }
  if (!lhs && !rhs) {
    return true;
  }
  return *lhs->Type == *rhs->Type;
}

bool FuncDecl::operator==(const FuncDecl &rhs) const {
  if (Name != rhs.Name || Params.size() != rhs.Params.size()) {
    return false;
  }
  if (!AreReciversEqual(Reciver.get(), rhs.Reciver.get())) {
    return false;
  }
  for (const auto pair : llvm::zip(Params, rhs.Params)) {
    if (!(*std::get<0>(pair)->Type == *std::get<1>(pair)->Type)) {
      return false;
    }
  }
  return true;
}

TypeFieldDecl::~TypeFieldDecl() {}

TypeDecl::~TypeDecl() {}

const TypeFieldDecl *TypeDecl::findField(llvm::StringRef name) const {
  for (const std::unique_ptr<TypeFieldDecl> &field : FieldDecls) {
    if (field->Name == name) {
      return field.get();
    }
  }
  return nullptr;
}
