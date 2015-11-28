//-*- C++ -*-

#ifndef RHINE_FUNCTION_H
#define RHINE_FUNCTION_H

#include "llvm/IR/Constants.h"
#include "llvm/ADT/iterator_range.h"

#include <string>
#include <vector>
#include <sstream>

#include "rhine/IR/Type.h"
#include "rhine/IR/User.h"
#include "rhine/IR/Constant.h"
#include "rhine/IR/UnresolvedValue.h"
#include "rhine/IR/BasicBlock.h"

using namespace llvm;

namespace rhine {
class Context;
class Module;

/// A function without a body, typically used to represent external functions.
class Prototype : public Constant {
  Module *ParentModule;
  std::vector<Argument *> ArgumentList;
  Argument *VariadicRestLoadInst;

public:
  Prototype(std::string Name, FunctionType *FTy, RTValue RTTy = RT_Prototype);
  virtual ~Prototype();
  void *operator new(size_t s);
  static Prototype *get(std::string Name, FunctionType *FTy);
  static bool classof(const Value *V);
  void setParent(Module *Parent);
  Module *getParent() const;
  virtual std::string getMangledName() const;
  llvm::Function *getOrInsert(llvm::Module *M);
  void setArguments(std::vector<Argument *> &L);
  void setVariadicRest(Argument *Rest);
  std::vector<Argument *> getArguments() const;
  typedef std::vector<Argument *>::iterator arg_iterator;
  arg_iterator arg_begin();
  arg_iterator arg_end();
  iterator_range<arg_iterator> args();
  virtual llvm::Constant *toLL(llvm::Module *M) override;

protected:
  virtual void print(std::ostream &Stream) const override;
  void emitArguments(std::ostream &Stream) const;
};

/// A prototype + a body
class Function : public Prototype {
  std::vector<BasicBlock *> Body;

public:
  Function(std::string Name, FunctionType *FTy);
  virtual ~Function();
  void *operator new(size_t s);
  static Function *get(std::string Name, FunctionType *FTy);
  static bool classof(const Value *V);
  virtual std::string getMangledName() const override;

  /// Add a new BasicBlock to the end of the function.
  void push_back(BasicBlock *NewBB);

  /// Forwards to the std equivalents front() and back() for the BasicBlocks.
  BasicBlock *getEntryBlock() const;
  BasicBlock *getExitBlock() const;

  /// The usual begin() and end() to go through the BasicBlocks.
  typedef std::vector<BasicBlock *>::iterator iterator;
  iterator begin();
  iterator end();

  /// Emits the decoration around BasicBlocks declaring the function along with
  /// the arguments, and then calls codegenAllBlocks(). Finally, extracts the
  /// (ReturnInst from the ExitBlock | nullptr), and returns the LLVM IR of the
  /// corresponding function, codegened in module M.
  virtual llvm::Constant *toLL(llvm::Module *M) override;

protected:
  virtual void print(std::ostream &Stream) const override;
};
}

#endif