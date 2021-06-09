#ifndef __QUARK_FRONTEND_TYPE_H__
#define __QUARK_FRONTEND_TYPE_H__

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

namespace quark {

struct Type {
  virtual ~Type() = 0;

  virtual void print(llvm::raw_ostream &) const = 0;
  virtual void dump() const = 0;

  std::string Name;

protected:
  Type(std::string name) : Name(std::move(name)) {}
};

enum class BuiltinTypeKind {
  // clang-format off
  u8,  i8,
  u16, i16,
  u32, i32,
  u64, i64,
  f32, f64,
  f80,
  u128, i128,
  u256, i256,
  u512, i512
  // clang-format on
};

auto GetTypeName(BuiltinTypeKind type) -> llvm::StringRef;
auto GetBytes(BuiltinTypeKind type) -> std::size_t;
auto IsSigned(BuiltinTypeKind type) -> bool;
auto IsUnsigned(BuiltinTypeKind type) -> bool;
auto IsFloatingPoint(BuiltinTypeKind type) -> bool;
auto IsInteger(BuiltinTypeKind type) -> bool;
auto IsVector(BuiltinTypeKind type) -> bool;

struct BuiltinType : public Type {
  BuiltinType(BuiltinTypeKind kind)
      : Type(GetTypeName(kind).data()), Kind(kind) {}
  virtual ~BuiltinType();

  auto bytes() const -> std::size_t;
  auto isSigned() const -> bool;
  auto isUnsigned() const -> bool;
  auto isFloatingPoint() const -> bool;
  auto isInteger() const -> bool;
  auto isVector() const -> bool;

  virtual void print(llvm::raw_ostream &) const;
  virtual void dump() const;

  BuiltinTypeKind Kind;
};

struct AliasType : public Type {
  AliasType(std::string name, const Type &type)
      : Type(std::move(name)), RealType(type) {}
  virtual ~AliasType();

  virtual void print(llvm::raw_ostream &) const;
  virtual void dump() const;

  const Type &RealType;
};

struct CompoundType : public Type {
  CompoundType(std::string name) : Type(std::move(name)) {}
  virtual ~CompoundType();

  virtual void print(llvm::raw_ostream &) const;
  virtual void dump() const;

  llvm::SmallVector<std::unique_ptr<Type>, 8> Types;
};

} // namespace quark

#endif // __QUARK_FRONTEND_TYPE_H__
