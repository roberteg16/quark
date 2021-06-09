#include <llvm-12/llvm/Support/Debug.h>
#include <quark/Frontend/SourceModule.h>

#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Stmt.h>
#include <quark/Frontend/Type.h>

using namespace quark;

SourceModule::SourceModule() {
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i8));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u8));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i16));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u16));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i32));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u32));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i64));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u64));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::f32));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::f64));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::f80));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i128));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u128));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u256));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i256));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::i512));
  TypesMap.push_back(std::make_unique<BuiltinType>(BuiltinTypeKind::u512));
}

SourceModule::~SourceModule() {}

void SourceModule::print(llvm::raw_ostream &out) {
  out << "=SourceModule=\n\n";

  llvm::dbgs() << "=TYPES=\n";
  for (const auto& type : TypesMap) {
    llvm::dbgs() << "  ";
    type->dump();
    llvm::dbgs() << "\n";
  }

  for (const auto& imported : ImportedModules) {
    llvm::dbgs() << imported << "\n";
  }

  for (const auto& decl : FunctionDeclarations) {
    decl->dump();
    llvm::dbgs() << "\n";
  }

  llvm::dbgs() << "\n===END===\n";;
}

void SourceModule::dump() {
  print(llvm::dbgs());
}