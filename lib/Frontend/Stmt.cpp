#include <quark/Frontend/Stmt.h>

#include <quark/Frontend/Expr.h>
#include <quark/Frontend/Decl.h>

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace quark;

Stmt::~Stmt() {}

BlockStmt::~BlockStmt() {}

void BlockStmt::print(llvm::raw_ostream &out) { out << "BlockStmt\n"; }
void BlockStmt::dump() { print(llvm::dbgs()); }

ForStmt::~ForStmt() {}

void ForStmt::print(llvm::raw_ostream &out) { out << "ForStmt\n"; }
void ForStmt::dump() { print(llvm::dbgs()); }

IfStmt::~IfStmt() {}

void IfStmt::print(llvm::raw_ostream &out) { out << "IfStmt\n"; }
void IfStmt::dump() { print(llvm::dbgs()); }

WhileStmt::~WhileStmt() {}

void WhileStmt::print(llvm::raw_ostream &out) { out << "WhileStmt\n"; }
void WhileStmt::dump() { print(llvm::dbgs()); }

ReturnStmt::~ReturnStmt() {}

void ReturnStmt::print(llvm::raw_ostream &out) { out << "ReturnStmt\n"; }
void ReturnStmt::dump() { print(llvm::dbgs()); }

DeferStmt::~DeferStmt() {}

void DeferStmt::print(llvm::raw_ostream &out) { out << "DeferStmt\n"; }
void DeferStmt::dump() { print(llvm::dbgs()); }

DeallocStmt::~DeallocStmt() {}

void DeallocStmt::print(llvm::raw_ostream &out) { out << "DeallocStmt\n"; }
void DeallocStmt::dump() { print(llvm::dbgs()); }

VarDeclStmt::~VarDeclStmt() {}

void VarDeclStmt::print(llvm::raw_ostream &out) { out << "VarDeclStmt\n"; }
void VarDeclStmt::dump() { print(llvm::dbgs()); }

ExprStmt::~ExprStmt() {}

void ExprStmt::print(llvm::raw_ostream &out) { out << "ExprStmt\n"; }
void ExprStmt::dump() { print(llvm::dbgs()); }
