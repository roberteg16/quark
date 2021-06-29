#include "quark/Frontend/ASTDumper.h"
#include <quark/Frontend/SourceModule.h>

#include <quark/Frontend/ASTDumper.h>
#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>

#include <llvm/Support/Debug.h>

using namespace quark;

const BuiltinType SourceModule::BuiltinTypes[18] = {
    {BuiltinTypeKind::Void}, {BuiltinTypeKind::i8},   {BuiltinTypeKind::u8},
    {BuiltinTypeKind::i16},  {BuiltinTypeKind::u16},  {BuiltinTypeKind::i32},
    {BuiltinTypeKind::u32},  {BuiltinTypeKind::i64},  {BuiltinTypeKind::u64},
    {BuiltinTypeKind::f32},  {BuiltinTypeKind::f64},  {BuiltinTypeKind::f80},
    {BuiltinTypeKind::i128}, {BuiltinTypeKind::u128}, {BuiltinTypeKind::u256},
    {BuiltinTypeKind::i256}, {BuiltinTypeKind::i512}, {BuiltinTypeKind::u512}};

void SourceModule::print(llvm::raw_ostream &out) const {
  ASTDumper{out}.dump(*this);
}

void SourceModule::dump() const { print(llvm::dbgs()); }
