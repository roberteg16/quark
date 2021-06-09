#include <quark/Frontend/Decl.h>

#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Expr.h>

#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

using namespace quark;

Decl::~Decl() {}

VarDecl::~VarDecl() {}

void VarDecl::print(llvm::raw_ostream &out) { out << "VarDecl\n"; }

void VarDecl::dump() { print(llvm::dbgs()); }

FuncDecl::~FuncDecl() {}

void FuncDecl::print(llvm::raw_ostream &out) { out << "FuncDecl\n"; }

void FuncDecl::dump() { print(llvm::dbgs()); }

TypeFieldDecl::~TypeFieldDecl() {}

void TypeFieldDecl::print(llvm::raw_ostream &out) { out << "TypeFieldDecl\n"; }

void TypeFieldDecl::dump() { print(llvm::dbgs()); }

TypeDecl::~TypeDecl() {}

void TypeDecl::print(llvm::raw_ostream &out) { out << "TypeDecl\n"; }

void TypeDecl::dump() { print(llvm::dbgs()); }
