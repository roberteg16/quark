#ifndef __QUARK_FRONTEND_ASTVISITOR_H__
#define __QUARK_FRONTEND_ASTVISITOR_H__

#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Expr.h>
#include <quark/Frontend/SourceModule.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>

#include <llvm/Support/Casting.h>

namespace quark {

template <typename Visitor> struct ASTVisitor {
  ASTVisitor() {}
  virtual ~ASTVisitor() {}

protected:
  void visit(const SourceModule &sm) {
    static_cast<Visitor *>(this)->VisitSourceModule();

    if (!sm.ExportedModule.empty())
      static_cast<Visitor *>(this)->VisitExportModule(sm.ExportedModule);

    for (llvm::StringRef imported : sm.ImportedModules) {
      static_cast<Visitor *>(this)->VisitImportModule(imported);
    }

    for (const std::unique_ptr<Decl> &decl : sm.Declarations) {
      visit(*decl);
    }

    static_cast<Visitor *>(this)->VisitExitSourceModule();
  }

  void visit(const Decl &decl) {
    switch (decl.Kind) {
    case DeclKind::VarDecl:
      return static_cast<Visitor *>(this)->VisitVarDecl(
          *llvm::cast<VarDecl>(&decl));
    case DeclKind::TypeDecl:
      return static_cast<Visitor *>(this)->VisitTypeDecl(
          *llvm::cast<TypeDecl>(&decl));
    case DeclKind::TypeFieldDecl:
      return static_cast<Visitor *>(this)->VisitTypeFieldDecl(
          *llvm::cast<TypeFieldDecl>(&decl));
    case DeclKind::FuncDecl:
      return static_cast<Visitor *>(this)->VisitFuncDecl(
          *llvm::cast<FuncDecl>(&decl));
    case DeclKind::AliasTypeDecl:
      return static_cast<Visitor *>(this)->VisitAliasTypeDecl(
          *llvm::cast<AliasTypeDecl>(&decl));
    }
  }

  void visit(const Stmt &stmt) {
    switch (stmt.Kind) {
    case StmtKind::BlockStmt:
      return static_cast<Visitor *>(this)->VisitBlockStmt(
          *llvm::cast<BlockStmt>(&stmt));
    case StmtKind::DeallocStmt:
      return static_cast<Visitor *>(this)->VisitDeallocStmt(
          *llvm::cast<DeallocStmt>(&stmt));
    case StmtKind::DeferStmt:
      return static_cast<Visitor *>(this)->VisitDeferStmt(
          *llvm::cast<DeferStmt>(&stmt));
    case StmtKind::ExprStmt:
      return static_cast<Visitor *>(this)->VisitExprStmt(
          *llvm::cast<ExprStmt>(&stmt));
    case StmtKind::ForStmt:
      return static_cast<Visitor *>(this)->VisitForStmt(
          *llvm::cast<ForStmt>(&stmt));
    case StmtKind::IfStmt:
      return static_cast<Visitor *>(this)->VisitIfStmt(
          *llvm::cast<IfStmt>(&stmt));
    case StmtKind::ReturnStmt:
      return static_cast<Visitor *>(this)->VisitReturnStmt(
          *llvm::cast<ReturnStmt>(&stmt));
    case StmtKind::VarDeclStmt:
      return static_cast<Visitor *>(this)->VisitVarDeclStmt(
          *llvm::cast<VarDeclStmt>(&stmt));
    case StmtKind::WhileStmt:
      return static_cast<Visitor *>(this)->VisitWhileStmt(
          *llvm::cast<WhileStmt>(&stmt));
    }
  }

  void visit(const Expr &expr) {
    switch (expr.Kind) {
    case ExprKind::AllocExpr:
      return static_cast<Visitor *>(this)->VisitAllocExpr(
          *llvm::cast<AllocExpr>(&expr));
    case ExprKind::BinaryExpr:
      return static_cast<Visitor *>(this)->VisitBinaryExpr(
          *llvm::cast<BinaryExpr>(&expr));
    case ExprKind::CharExpr:
      return static_cast<Visitor *>(this)->VisitCharExpr(
          *llvm::cast<CharExpr>(&expr));
    case ExprKind::FloatingExpr:
      return static_cast<Visitor *>(this)->VisitFloatingExpr(
          *llvm::cast<FloatingExpr>(&expr));
    case ExprKind::FunctionCallExpr:
      return static_cast<Visitor *>(this)->VisitFunctionCallExpr(
          *llvm::cast<FunctionCallExpr>(&expr));
    case ExprKind::IntegerExpr:
      return static_cast<Visitor *>(this)->VisitIntegerExpr(
          *llvm::cast<IntegerExpr>(&expr));
    case ExprKind::MemberCallExpr:
      return static_cast<Visitor *>(this)->VisitMemberCallExpr(
          *llvm::cast<MemberCallExpr>(&expr));
    case ExprKind::MemberExpr:
      return static_cast<Visitor *>(this)->VisitMemberExpr(
          *llvm::cast<MemberExpr>(&expr));
    case ExprKind::StringExpr:
      return static_cast<Visitor *>(this)->VisitStringExpr(
          *llvm::cast<StringExpr>(&expr));
    case ExprKind::UnaryExpr:
      return static_cast<Visitor *>(this)->VisitUnaryExpr(
          *llvm::cast<UnaryExpr>(&expr));
    case ExprKind::VarRefExpr:
      return static_cast<Visitor *>(this)->VisitVarRefExpr(
          *llvm::cast<VarRefExpr>(&expr));
    case ExprKind::DereferenceExpr:
      return static_cast<Visitor *>(this)->VisitDereferenceExpr(
          *llvm::cast<DereferenceExpr>(&expr));
    case ExprKind::AddressofExpr:
      return static_cast<Visitor *>(this)->VisitAddressofExpr(
          *llvm::cast<AddressofExpr>(&expr));
    case ExprKind::ArrayAccessExpr:
      return static_cast<Visitor *>(this)->VisitArrayAccessExpr(
          *llvm::cast<ArrayAccessExpr>(&expr));
    }
  }

  void visit(const Type &type) {
    switch (type.TKind) {
    case TypeKind::AliasType:
      return static_cast<Visitor *>(this)->VisitAliasType(
          *llvm::cast<AliasType>(&type));
    case TypeKind::ArrayType:
      return static_cast<Visitor *>(this)->VisitArrayType(
          *llvm::cast<ArrayType>(&type));
    case TypeKind::BuiltinType:
      return static_cast<Visitor *>(this)->VisitBuiltinType(
          *llvm::cast<BuiltinType>(&type));
    case TypeKind::CompoundType:
      return static_cast<Visitor *>(this)->VisitCompoundType(
          *llvm::cast<CompoundType>(&type));
    case TypeKind::PtrType:
      return static_cast<Visitor *>(this)->VisitPtrType(
          *llvm::cast<PtrType>(&type));
    }
  };

  virtual void VisitSourceModule() {}
  virtual void VisitExitSourceModule() {}
  virtual void VisitExportModule(llvm::StringRef) {}
  virtual void VisitImportModule(llvm::StringRef) {}

  virtual void VisitAllocExpr(const AllocExpr &) {}
  virtual void VisitFunctionCallExpr(const FunctionCallExpr &) {}
  virtual void VisitMemberCallExpr(const MemberCallExpr &) {}
  virtual void VisitMemberExpr(const MemberExpr &) {}
  virtual void VisitStringExpr(const StringExpr &) {}
  virtual void VisitIntegerExpr(const IntegerExpr &) {}
  virtual void VisitFloatingExpr(const FloatingExpr &) {}
  virtual void VisitCharExpr(const CharExpr &) {}
  virtual void VisitVarRefExpr(const VarRefExpr &) {}
  virtual void VisitBinaryExpr(const BinaryExpr &) {}
  virtual void VisitUnaryExpr(const UnaryExpr &) {}
  virtual void VisitDereferenceExpr(const DereferenceExpr &) {}
  virtual void VisitAddressofExpr(const AddressofExpr &) {}
  virtual void VisitArrayAccessExpr(const ArrayAccessExpr &) {}

  virtual void VisitBlockStmt(const BlockStmt &) {}
  virtual void VisitDeallocStmt(const DeallocStmt &) {}
  virtual void VisitDeferStmt(const DeferStmt &) {}
  virtual void VisitExprStmt(const ExprStmt &) {}
  virtual void VisitForStmt(const ForStmt &) {}
  virtual void VisitIfStmt(const IfStmt &) {}
  virtual void VisitReturnStmt(const ReturnStmt &) {}
  virtual void VisitVarDeclStmt(const VarDeclStmt &) {}
  virtual void VisitWhileStmt(const WhileStmt &) {}

  virtual void VisitVarDecl(const VarDecl &) {}
  virtual void VisitTypeDecl(const TypeDecl &) {}
  virtual void VisitTypeFieldDecl(const TypeFieldDecl &) {}
  virtual void VisitFuncDecl(const FuncDecl &) {}
  virtual void VisitAliasTypeDecl(const AliasTypeDecl &) {}

  virtual void VisitBuiltinType(const BuiltinType &) {}
  virtual void VisitAliasType(const AliasType &) {}
  virtual void VisitCompoundType(const CompoundType &) {}
  virtual void VisitPtrType(const PtrType &) {}
  virtual void VisitArrayType(const ArrayType &) {}
};

}; // namespace quark

#endif // __QUARK_FRONTEND_ASTVISITOR_H__
