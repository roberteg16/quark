#ifndef __QUARK_FRONTEND_ASTDUMPER_H__
#define __QUARK_FRONTEND_ASTDUMPER_H__

#include "quark/Frontend/Expr.h"
#include <quark/Frontend/ASTVisitor.h>
#include <quark/Support/IdentationFormat.h>

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

#include <vector>

namespace quark {

struct ASTDumper : public ASTVisitor<ASTDumper> {
  ASTDumper(llvm::raw_ostream &out)
      : Out(out), Holder(), RSO(Holder), Depth(0) {}
  virtual ~ASTDumper() {}

  void dump(const SourceModule &);
  void dump(const Decl &);
  void dump(const Stmt &);
  void dump(const Expr &);
  void dump(const Type &);

  virtual void VisitSourceModule();
  virtual void VisitExitSourceModule();
  virtual void VisitExportModule(llvm::StringRef);
  virtual void VisitImportModule(llvm::StringRef);

  virtual void VisitAllocExpr(const AllocExpr &);
  virtual void VisitFunctionCallExpr(const FunctionCallExpr &);
  virtual void VisitMemberCallExpr(const MemberCallExpr &);
  virtual void VisitMemberExpr(const MemberExpr &);
  virtual void VisitStringExpr(const StringExpr &);
  virtual void VisitIntegerExpr(const IntegerExpr &);
  virtual void VisitFloatingExpr(const FloatingExpr &);
  virtual void VisitCharExpr(const CharExpr &);
  virtual void VisitVarRefExpr(const VarRefExpr &);
  virtual void VisitBinaryExpr(const BinaryExpr &);
  virtual void VisitUnaryExpr(const UnaryExpr &);
  virtual void VisitDereferenceExpr(const DereferenceExpr &);
  virtual void VisitAddressofExpr(const AddressofExpr &);
  virtual void VisitArrayAccessExpr(const ArrayAccessExpr &);

  virtual void VisitBlockStmt(const BlockStmt &);
  virtual void VisitDeallocStmt(const DeallocStmt &);
  virtual void VisitDeferStmt(const DeferStmt &);
  virtual void VisitExprStmt(const ExprStmt &);
  virtual void VisitForStmt(const ForStmt &);
  virtual void VisitIfStmt(const IfStmt &);
  virtual void VisitReturnStmt(const ReturnStmt &);
  virtual void VisitVarDeclStmt(const VarDeclStmt &);
  virtual void VisitWhileStmt(const WhileStmt &);

  virtual void VisitVarDecl(const VarDecl &);
  virtual void VisitTypeDecl(const TypeDecl &);
  virtual void VisitTypeFieldDecl(const TypeFieldDecl &);
  virtual void VisitFuncDecl(const FuncDecl &);
  virtual void VisitAliasTypeDecl(const AliasTypeDecl &);

  virtual void VisitBuiltinType(const BuiltinType &);
  virtual void VisitAliasType(const AliasType &);
  virtual void VisitCompoundType(const CompoundType &);
  virtual void VisitPtrType(const PtrType &);
  virtual void VisitArrayType(const ArrayType &);

  llvm::raw_ostream &Out;
  std::string Holder;
  llvm::raw_string_ostream RSO;
  std::vector<IdentationElement> Elements;
  std::size_t Depth;

  bool PrintVerboseStmtHeaders = false;
};

} // namespace quark

#endif // __QUARK_FRONTEND_ASTDUMPER_H__