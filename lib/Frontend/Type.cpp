#include <llvm-12/llvm/Support/Debug.h>
#include <quark/Frontend/Type.h>

#include <llvm/Support/raw_ostream.h>

using namespace quark;

llvm::StringRef quark::GetTypeName(BuiltinTypeKind type) {
  switch (type) {
  case BuiltinTypeKind::i8:
    return "i8";
  case BuiltinTypeKind::u8:
    return "u8";
  case BuiltinTypeKind::i16:
    return "i16";
  case BuiltinTypeKind::u16:
    return "u16";
  case BuiltinTypeKind::i32:
    return "i32";
  case BuiltinTypeKind::u32:
    return "u32";
  case BuiltinTypeKind::i64:
    return "i64";
  case BuiltinTypeKind::u64:
    return "u64";
  case BuiltinTypeKind::f32:
    return "f32";
  case BuiltinTypeKind::f64:
    return "f64";
  case BuiltinTypeKind::f80:
    return "f80";
  case BuiltinTypeKind::i128:
    return "i128";
  case BuiltinTypeKind::u128:
    return "u128";
  case BuiltinTypeKind::i256:
    return "i256";
  case BuiltinTypeKind::u256:
    return "u256";
  case BuiltinTypeKind::i512:
    return "i512";
  case BuiltinTypeKind::u512:
    return "u512";
  }
}

std::size_t quark::GetBytes(BuiltinTypeKind type) {
  switch (type) {
  case BuiltinTypeKind::i8:
  case BuiltinTypeKind::u8:
    return 1;
  case BuiltinTypeKind::i16:
  case BuiltinTypeKind::u16:
    return 2;
  case BuiltinTypeKind::i32:
  case BuiltinTypeKind::u32:
    return 4;
  case BuiltinTypeKind::i64:
  case BuiltinTypeKind::u64:
    return 8;
  case BuiltinTypeKind::f32:
    return 4;
  case BuiltinTypeKind::f64:
    return 8;
  case BuiltinTypeKind::f80:
    return 16;
  case BuiltinTypeKind::i128:
  case BuiltinTypeKind::u128:
    return 16;
  case BuiltinTypeKind::i256:
  case BuiltinTypeKind::u256:
    return 32;
  case BuiltinTypeKind::i512:
  case BuiltinTypeKind::u512:
    return 64;
  }
}

bool quark::IsSigned(BuiltinTypeKind type) {
  switch (type) {
  case BuiltinTypeKind::i8:
  case BuiltinTypeKind::i16:
  case BuiltinTypeKind::i32:
  case BuiltinTypeKind::i64:
  case BuiltinTypeKind::i128:
  case BuiltinTypeKind::i256:
  case BuiltinTypeKind::i512:
  case BuiltinTypeKind::f32:
  case BuiltinTypeKind::f64:
  case BuiltinTypeKind::f80:
    return true;
  case BuiltinTypeKind::u8:
  case BuiltinTypeKind::u16:
  case BuiltinTypeKind::u32:
  case BuiltinTypeKind::u64:
  case BuiltinTypeKind::u128:
  case BuiltinTypeKind::u256:
  case BuiltinTypeKind::u512:
    return false;
  }
}

bool quark::IsUnsigned(BuiltinTypeKind type) {
  switch (type) {
  case BuiltinTypeKind::i8:
  case BuiltinTypeKind::i16:
  case BuiltinTypeKind::i32:
  case BuiltinTypeKind::i64:
  case BuiltinTypeKind::i128:
  case BuiltinTypeKind::i256:
  case BuiltinTypeKind::i512:
  case BuiltinTypeKind::f32:
  case BuiltinTypeKind::f64:
  case BuiltinTypeKind::f80:
    return false;
  case BuiltinTypeKind::u8:
  case BuiltinTypeKind::u16:
  case BuiltinTypeKind::u32:
  case BuiltinTypeKind::u64:
  case BuiltinTypeKind::u128:
  case BuiltinTypeKind::u256:
  case BuiltinTypeKind::u512:
    return true;
  }
}

bool quark::IsFloatingPoint(BuiltinTypeKind type) {
  switch (type) {
  case BuiltinTypeKind::i8:
  case BuiltinTypeKind::u8:
  case BuiltinTypeKind::i16:
  case BuiltinTypeKind::u16:
  case BuiltinTypeKind::i32:
  case BuiltinTypeKind::u32:
  case BuiltinTypeKind::i64:
  case BuiltinTypeKind::u64:
  case BuiltinTypeKind::i128:
  case BuiltinTypeKind::u128:
  case BuiltinTypeKind::i256:
  case BuiltinTypeKind::u256:
  case BuiltinTypeKind::i512:
  case BuiltinTypeKind::u512:
    return false;
  case BuiltinTypeKind::f32:
  case BuiltinTypeKind::f64:
  case BuiltinTypeKind::f80:
    return true;
  }
}

bool quark::IsInteger(BuiltinTypeKind type) {
  switch (type) {
  case BuiltinTypeKind::i8:
  case BuiltinTypeKind::u8:
  case BuiltinTypeKind::i16:
  case BuiltinTypeKind::u16:
  case BuiltinTypeKind::i32:
  case BuiltinTypeKind::u32:
  case BuiltinTypeKind::i64:
  case BuiltinTypeKind::u64:
    return true;
  case BuiltinTypeKind::i128:
  case BuiltinTypeKind::u128:
  case BuiltinTypeKind::i256:
  case BuiltinTypeKind::u256:
  case BuiltinTypeKind::i512:
  case BuiltinTypeKind::u512:
  case BuiltinTypeKind::f32:
  case BuiltinTypeKind::f64:
  case BuiltinTypeKind::f80:
    return false;
  }
}

bool quark::IsVector(BuiltinTypeKind type) {
  return !IsInteger(type) && !IsFloatingPoint(type);
}

Type::~Type() {}

BuiltinType::~BuiltinType() {}

std::size_t BuiltinType::bytes() const { return GetBytes(Kind); }
bool BuiltinType::isSigned() const { return IsSigned(Kind); }
bool BuiltinType::isUnsigned() const { return IsUnsigned(Kind); }
bool BuiltinType::isFloatingPoint() const { return IsFloatingPoint(Kind); }
bool BuiltinType::isInteger() const { return IsInteger(Kind); }
bool BuiltinType::isVector() const { return IsVector(Kind); }

void BuiltinType::print(llvm::raw_ostream &out) const {
  out << "BuiltinType:" << Name;
}
void BuiltinType::dump() const { print(llvm::dbgs()); }

AliasType::~AliasType() {}

void AliasType::print(llvm::raw_ostream &out) const {
  out << "AliasType:";
  RealType.dump();
}
void AliasType::dump() const { print(llvm::dbgs()); }

CompoundType::~CompoundType() {}

void CompoundType::print(llvm::raw_ostream &out) const {
  out << "CompoundType";
}
void CompoundType::dump() const { print(llvm::dbgs()); }
