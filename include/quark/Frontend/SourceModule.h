#ifndef __QUARK_FRONTEND_SOURCEMODULE_H__
#define __QUARK_FRONTEND_SOURCEMODULE_H__

#include <quark/Frontend/Decl.h>
#include <quark/Frontend/Type.h>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

#include <deque>

namespace quark {

class SourceModule {
public:
  SourceModule() = default;
  ~SourceModule() = default;
  SourceModule(SourceModule &&) = default;
  SourceModule &operator=(SourceModule &&) = default;
  SourceModule(const SourceModule &) = delete;
  SourceModule &operator=(const SourceModule &) = delete;

  void print(llvm::raw_ostream &) const;
  void dump() const;

public:
  /// Exported module
  llvm::SmallString<10> ExportedModule;

  /// Imported module
  llvm::SmallVector<llvm::SmallString<10>, 10> ImportedModules;

  /// Declarations
  std::deque<std::unique_ptr<Decl>> Declarations;

  /// Builtin types
  static const BuiltinType BuiltinTypes[18];
};

} // namespace quark

#endif // __QUARK_FRONTEND_SOURCEMODULE_H__
