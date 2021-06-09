#ifndef __QUARK_FRONTEND_SOURCEMODULE_H__
#define __QUARK_FRONTEND_SOURCEMODULE_H__

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/raw_ostream.h>

namespace quark {

struct Type;
struct Decl;

class SourceModule {
public:
  SourceModule();
  ~SourceModule();

  void print(llvm::raw_ostream &);
  void dump();

private:
  llvm::SmallVector<std::string, 10> ImportedModules;
  llvm::SmallVector<std::unique_ptr<Type>, 20> TypesMap;
  llvm::SmallVector<std::unique_ptr<Decl>, 20> FunctionDeclarations;
};

} // namespace quark

#endif // __QUARK_FRONTEND_SOURCEMODULE_H__
