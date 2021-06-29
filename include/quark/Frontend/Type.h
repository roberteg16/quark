#ifndef __QUARK_FRONTEND_TYPE_H__
#define __QUARK_FRONTEND_TYPE_H__

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

namespace quark {

struct TypeDecl;

enum class TypeKind {
  BuiltinType = 0,
  AliasType,
  CompoundType,
  PtrType,
  ArrayType
};

struct Type {
  Type(TypeKind kind) : TKind(kind) {}
  virtual ~Type() = 0;

  virtual void print(llvm::raw_ostream &) const;
  virtual void dump() const;

  virtual std::unique_ptr<Type> clone() const = 0;
  virtual const Type *getUnmaskedType() const = 0;

  virtual TypeKind getKind() const { return TKind; }

  virtual bool operator==(const Type &) const = 0;

  TypeKind TKind;
};

struct NamedType : Type {
  virtual ~NamedType() = 0;
  llvm::SmallString<20> Name;

protected:
  NamedType(TypeKind kind, llvm::StringRef name) : Type(kind), Name(name) {}
};

enum class BuiltinTypeKind {
  // clang-format off
  Void,
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

auto getTypeName(BuiltinTypeKind type) -> llvm::StringRef;
auto getBytes(BuiltinTypeKind type) -> std::size_t;
auto IsSigned(BuiltinTypeKind type) -> bool;
auto IsUnsigned(BuiltinTypeKind type) -> bool;
auto IsFloatingPoint(BuiltinTypeKind type) -> bool;
auto IsInteger(BuiltinTypeKind type) -> bool;
auto IsVector(BuiltinTypeKind type) -> bool;

struct BuiltinType : public NamedType {
  BuiltinType(BuiltinTypeKind kind)
      : NamedType(TypeKind::BuiltinType, getTypeName(kind)), Kind(kind) {}
  virtual ~BuiltinType() {}

  auto bytes() const -> std::size_t;
  auto isSigned() const -> bool;
  auto isUnsigned() const -> bool;
  auto isFloatingPoint() const -> bool;
  auto isInteger() const -> bool;
  auto isVector() const -> bool;

  virtual std::unique_ptr<Type> clone() const;
  virtual const Type *getUnmaskedType() const;

  virtual bool operator==(const Type &) const;

  static bool classof(const Type *t) {
    return t->getKind() == TypeKind::BuiltinType;
  }

  BuiltinTypeKind Kind;
};

struct AliasType : public NamedType {
  AliasType(llvm::StringRef name, const Type &type)
      : NamedType(TypeKind::AliasType, name), RealType(type) {}
  virtual ~AliasType() {}

  std::unique_ptr<Type> getRealType() const;

  virtual std::unique_ptr<Type> clone() const;
  virtual const Type *getUnmaskedType() const;

  virtual bool operator==(const Type &) const;

  static bool classof(const Type *t) {
    return t->getKind() == TypeKind::AliasType;
  }

  const Type &RealType;
};

struct CompoundType : public NamedType {
  CompoundType(const TypeDecl &typeDecl);
  virtual ~CompoundType() {}

  virtual std::unique_ptr<Type> clone() const;
  virtual const Type *getUnmaskedType() const;

  virtual bool operator==(const Type &) const;

  static bool classof(const Type *t) {
    return t->getKind() == TypeKind::CompoundType;
  }

  llvm::SmallVector<std::unique_ptr<Type>, 4> Types;
  const TypeDecl &Decl;
};

struct PtrType : Type {
  PtrType(std::unique_ptr<Type> pointeeType)
      : Type(TypeKind::PtrType), PointeeType(std::move(pointeeType)) {}
  virtual ~PtrType() {}

  virtual std::unique_ptr<Type> clone() const;
  virtual const Type *getUnmaskedType() const;

  virtual bool operator==(const Type &) const;

  static bool classof(const Type *t) {
    return t->getKind() == TypeKind::PtrType;
  }

  const std::unique_ptr<Type> PointeeType;
};

struct ArrayType : Type {
  ArrayType(std::unique_ptr<Type> type, std::size_t n)
      : Type(TypeKind::ArrayType), RealType(std::move(type)), Elements(n) {}
  virtual ~ArrayType() {}

  virtual std::unique_ptr<Type> clone() const;
  virtual const Type *getUnmaskedType() const;

  virtual bool operator==(const Type &) const;

  static bool classof(const Type *t) {
    return t->getKind() == TypeKind::ArrayType;
  }

  std::unique_ptr<Type> RealType;
  const std::size_t Elements;
};

} // namespace quark

#endif // __QUARK_FRONTEND_TYPE_H__
