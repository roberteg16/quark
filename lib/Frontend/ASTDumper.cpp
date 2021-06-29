#include "quark/Frontend/Expr.h"
#include <quark/Frontend/ASTDumper.h>

using namespace quark;

static void printAll(llvm::ArrayRef<IdentationElement> elems,
                     llvm::raw_ostream &out) {
  ElementTreeIndentationFormat formater(elems);

  for (unsigned i = 0; i < elems.size(); i++) {
    formater.printFormat(out, i);
    out << elems[i].Str << "\n";
  }
}

void ASTDumper::dump(const SourceModule &sm) {
  visit(sm);
  printAll(Elements, Out);
}

void ASTDumper::dump(const Decl &decl) {
  visit(decl);
  printAll(Elements, Out);
}

void ASTDumper::dump(const Stmt &stmt) {
  visit(stmt);
  printAll(Elements, Out);
}

void ASTDumper::dump(const Expr &expr) {
  visit(expr);
  printAll(Elements, Out);
}

void ASTDumper::dump(const Type &type) {
  visit(type);
  printAll(Elements, Out);
}

void ASTDumper::VisitSourceModule() {
  RSO << "SourceModule: ";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
}

void ASTDumper::VisitExitSourceModule() { Depth--; }

void ASTDumper::VisitExportModule(llvm::StringRef exported) {
  RSO << "export: " << exported;
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitImportModule(llvm::StringRef import) {
  RSO << "import: " << import;
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitVarDecl(const VarDecl &decl) {
  RSO << "VarDecl: (" << ToString(decl.getVarDeclKind()) << ") " << decl.Name
      << ' ';
  visit(*decl.Type);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitTypeDecl(const TypeDecl &decl) {
  RSO << "TypeDecl: " << decl.Name << ' ';
  visit(decl.Type);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
  for (const std::unique_ptr<TypeFieldDecl> &field : decl.FieldDecls) {
    VisitTypeFieldDecl(*field);
  }
  Depth--;
}

void ASTDumper::VisitTypeFieldDecl(const TypeFieldDecl &decl) {
  RSO << "TypeFieldDecl: " << decl.Name << ' ';
  visit(*decl.Type);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitFuncDecl(const FuncDecl &decl) {
  RSO << "FuncDecl: " << decl.Name << ' ';
  visit(*decl.ReturnType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;

  if (decl.Reciver)
    visit(*decl.Reciver);
  for (const std::unique_ptr<VarDecl> &varDecl : decl.Params) {
    visit(*varDecl);
  }
  for (const std::unique_ptr<Stmt> &stmt : decl.Body) {
    visit(*stmt);
  }

  Depth--;
}

void ASTDumper::VisitAliasTypeDecl(const AliasTypeDecl &decl) {}

void ASTDumper::VisitBlockStmt(const BlockStmt &blockStmt) {
  RSO << "BlockStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  for (const std::unique_ptr<Stmt> &stmt : blockStmt.Stmts) {
    visit(*stmt);
  }
  Depth--;
}

void ASTDumper::VisitDeallocStmt(const DeallocStmt &stmt) {
  RSO << "DeallocStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*stmt.ExprToDealloc);
  Depth--;
}

void ASTDumper::VisitDeferStmt(const DeferStmt &stmt) {
  RSO << "DeferStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*stmt.ExprToDefer);
  Depth--;
}

void ASTDumper::VisitExprStmt(const ExprStmt &stmt) {
  if (PrintVerboseStmtHeaders) {
    RSO << "ExprStmt:";
    Elements.emplace_back(RSO.str(), Depth);
    RSO.str().clear();
    Depth++;
  }

  visit(*stmt.Expr);

  if (PrintVerboseStmtHeaders) {
    Depth--;
  }
}

void ASTDumper::VisitForStmt(const ForStmt &stmt) {
  RSO << "ForStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
  RSO << "Var-decl: ";
  visit(*stmt.VarDecl);
  RSO << "Cond: ";
  visit(*stmt.Cond);
  RSO << "Inc: ";
  visit(*stmt.Inc);
  RSO << "Body: ";
  visit(*stmt.Body);
  Depth--;
}

void ASTDumper::VisitIfStmt(const IfStmt &stmt) {
  RSO << "IfStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
  RSO << "If-Cond: ";
  visit(*stmt.Cond);
  RSO << "If-Code: ";
  visit(*stmt.Code);
  for (const IfStmt::CondAndStmt &elsif : stmt.Elsifs) {
    RSO << "Elsif:";
    Elements.emplace_back(RSO.str(), Depth);
    RSO.str().clear();
    Depth++;
    RSO << "Elsif-Cond: ";
    visit(*elsif.Cond);
    RSO << "Elsif-Code: ";
    visit(*elsif.Stmt);
    Depth--;
  }
  if (stmt.ElseCode) {
    RSO << "Else-Code:";
    Elements.emplace_back(RSO.str(), Depth);
    RSO.str().clear();
    Depth++;
    visit(*stmt.ElseCode);
    Depth--;
  }
  Depth--;
}

void ASTDumper::VisitReturnStmt(const ReturnStmt &stmt) {
  RSO << "RetStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*stmt.ReturnValue);
  Depth--;
}

void ASTDumper::VisitVarDeclStmt(const VarDeclStmt &stmt) {
  if (PrintVerboseStmtHeaders) {
    RSO << "VarDeclStmt:";
    Elements.emplace_back(RSO.str(), Depth);
    RSO.str().clear();
    Depth++;
  }

  visit(*stmt.VarDecl);
  if (stmt.InitExpr) {
    Depth++;
    visit(*stmt.InitExpr);
    Depth--;
  }

  if (PrintVerboseStmtHeaders) {
    Depth--;
  }
}

void ASTDumper::VisitWhileStmt(const WhileStmt &stmt) {
  RSO << "WhileStmt:";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  RSO << "Cond: ";
  visit(*stmt.Cond);
  Depth--;
  Depth++;
  RSO << "Code: ";
  visit(*stmt.Code);
  Depth--;
}

void ASTDumper::VisitAllocExpr(const AllocExpr &expr) {
  RSO << "AllocExpr: ";
  visit(*expr.AllocType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
  visit(*expr.SizeToAlloc);
  Depth--;
}

void ASTDumper::VisitFunctionCallExpr(const FunctionCallExpr &expr) {
  RSO << "FunctionCallExpr: " << expr.FunctionDecl.Name << " (";
  for (std::size_t i = 0; i < expr.FunctionDecl.Params.size(); i++) {
    if (i == expr.FunctionDecl.Params.size() - 1) {
      visit(*expr.FunctionDecl.Params[i]->Type);
    } else {
      visit(*expr.FunctionDecl.Params[i]->Type);
      RSO << ", ";
    }
  }
  RSO << ") -> ";
  visit(*expr.ExprType);

  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  for (const std::unique_ptr<Expr> &param : expr.Params) {
    visit(*param);
  }
  Depth--;
}

void ASTDumper::VisitMemberCallExpr(const MemberCallExpr &expr) {
  RSO << "MemberCallExpr: (rec: ";
  visit(expr.Accessor->getType());
  RSO << ") " << expr.FunctionDecl.Name << " (";
  for (std::size_t i = 0; i < expr.FunctionDecl.Params.size(); i++) {
    if (i == expr.FunctionDecl.Params.size() - 1) {
      visit(*expr.FunctionDecl.Params[i]->Type);
    } else {
      visit(*expr.FunctionDecl.Params[i]->Type);
      RSO << ", ";
    }
  }
  RSO << ") -> ";
  visit(*expr.ExprType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*expr.Accessor);
  Depth--;
  Depth++;
  for (const std::unique_ptr<Expr> &param : expr.Params) {
    visit(*param);
  }
  Depth--;
}

void ASTDumper::VisitMemberExpr(const MemberExpr &expr) {
  RSO << "MemberExpr: ";
  visit(*expr.ExprType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
  visit(expr.Field);
  visit(*expr.Accessed);
  Depth--;
}

void ASTDumper::VisitStringExpr(const StringExpr &expr) {
  RSO << "StringExpr: ";
  visit(*expr.ExprType);
  RSO << " " << expr.Value;
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitIntegerExpr(const IntegerExpr &expr) {
  RSO << "IntegerExpr: ";
  visit(*expr.ExprType);
  RSO << " " << expr.Value;
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitFloatingExpr(const FloatingExpr &expr) {
  RSO << "FloatingExpr: ";
  visit(*expr.ExprType);
  RSO << " " << (double)expr.Value;
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitCharExpr(const CharExpr &expr) {
  RSO << "CharExpr: ";
  visit(*expr.ExprType);
  RSO << " " << expr.Value;
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
}

void ASTDumper::VisitVarRefExpr(const VarRefExpr &expr) {
  RSO << "VarRefExpr: ";
  visit(*expr.ExprType);
  RSO << " ";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();
  Depth++;
  visit(expr.RefVar);
  Depth--;
}

void ASTDumper::VisitBinaryExpr(const BinaryExpr &expr) {
  RSO << "BinaryExpr: ";
  visit(*expr.ExprType);
  RSO << " (" << ToString(expr.Op) << ")";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*expr.Lhs);
  visit(*expr.Rhs);
  Depth--;
}

void ASTDumper::VisitUnaryExpr(const UnaryExpr &expr) {
  RSO << "UnaryExpr: ";
  visit(*expr.ExprType);
  RSO << " (" << ToString(expr.Op) << ")";
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*expr.Lhs);
  Depth--;
}

void ASTDumper::VisitAddressofExpr(const AddressofExpr &expr) {
  RSO << "AddressofExpr: ";
  visit(*expr.ExprType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*expr.AdressOfExpr);
  Depth--;
}

void ASTDumper::VisitDereferenceExpr(const DereferenceExpr &expr) {
  RSO << "DereferenceExpr: ";
  visit(*expr.ExprType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*expr.DereferencingExpr);
  Depth--;
}

void ASTDumper::VisitArrayAccessExpr(const ArrayAccessExpr &expr) {
  RSO << "ArrayAccessExpr: ";
  visit(*expr.ExprType);
  Elements.emplace_back(RSO.str(), Depth);
  RSO.str().clear();

  Depth++;
  visit(*expr.RefVar);
  visit(*expr.Idx);
  Depth--;
}

void ASTDumper::VisitBuiltinType(const BuiltinType &builtinType) {
  RSO << builtinType.Name;
}

void ASTDumper::VisitAliasType(const AliasType &aliasType) {
  RSO << aliasType.Name;
}

void ASTDumper::VisitCompoundType(const CompoundType &compoundType) {
  RSO << compoundType.Name;
}

void ASTDumper::VisitPtrType(const PtrType &ptrType) {
  RSO << "*";
  visit(*ptrType.PointeeType);
}

void ASTDumper::VisitArrayType(const ArrayType &arrayType) {
  visit(*arrayType.RealType);
  RSO << "[" << arrayType.Elements << "]";
}
