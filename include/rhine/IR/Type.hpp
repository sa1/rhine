#ifndef RHINE_TYPE_H
#define RHINE_TYPE_H

#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/TypeBuilder.h"

#include <sstream>
#include <string>
#include <vector>

#include "rhine/Diagnostic/Diagnostic.hpp"
#include "rhine/Parse/Parser.hpp"

using namespace std;
using namespace llvm;
using Location = rhine::Parser::Location;

namespace rhine {
class Context;

enum RTType {
  RT_UnType,
  RT_VoidType,
  RT_IntegerType,
  RT_BoolType,
  RT_FloatType,
  RT_StringType,
  RT_FunctionType,
  RT_PointerType,
  RT_TensorType,
};

class Type : public FoldingSetNode {
protected:
  /// Context is an essential part of the Type; it is how every Value indirectly
  /// has access to the Context.
  Context *Kontext;
  Location SourceLoc;
  Type(RTType ID);

public:
  Type(Context *K, RTType ID);
  virtual ~Type();
  RTType getTyID() const;
  static Type *get() = delete;
  Context *getContext();
  void setSourceLocation(Location SrcLoc);
  Location getSourceLocation();
  friend ostream &operator<<(ostream &Stream, const Type &T) {
    auto DiagStream = DiagnosticPrinter(Stream);
    T.print(DiagStream);
    return Stream;
  }
  friend DiagnosticPrinter &operator<<(DiagnosticPrinter &Stream,
                                       const Type &T) {
    T.print(Stream);
    return Stream;
  }
  void dump();
  virtual llvm::Type *toLL(llvm::Module *M) = 0;

protected:
  virtual void print(DiagnosticPrinter &Stream) const = 0;

private:
  const RTType TyID;
  friend class Context;
};

class UnType : public Type {
  /// UnType is a special typed for expressions that are constructed by the
  /// parser without a literal type. The type is later filled in using
  /// inference. Private constructor because
  UnType(Context *K);

public:
  virtual ~UnType();
  static UnType *get(Context *K);
  static bool classof(const Type *T);
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
  friend class Context;
};

class VoidType : public Type {
  VoidType();

public:
  virtual ~VoidType();
  static VoidType *get(Context *K);
  static bool classof(const Type *T);
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
  friend class Context;
};

class IntegerType : public Type {
  IntegerType(Context *K, unsigned Width);

public:
  virtual ~IntegerType();
  static IntegerType *get(unsigned Bitwidth, Context *K);
  unsigned getBitwidth();
  static bool classof(const Type *T);
  static inline void Profile(FoldingSetNodeID &ID, const unsigned &W);
  void Profile(FoldingSetNodeID &ID) const;
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  unsigned Bitwidth;
  virtual void print(DiagnosticPrinter &Stream) const override;
};

class BoolType : public Type {
  /// Lowers to i1.
  BoolType();

public:
  virtual ~BoolType();
  static BoolType *get(Context *K);
  static bool classof(const Type *T);
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
  friend class Context;
};

class FloatType : public Type {
  /// Pivate constructor; only used by Context;
  FloatType();

public:
  virtual ~FloatType();
  static FloatType *get(Context *K);
  static bool classof(const Type *T);
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
  friend class Context;
};

class StringType : public Type {
  /// Pivate constructor; only used by Context;
  StringType();

public:
  virtual ~StringType();
  static StringType *get(Context *K);
  static bool classof(const Type *T);
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
  friend class Context;
};

class FunctionType : public Type {
  Type *ReturnType;
  bool VariadicFlag;
  std::vector<Type *> ArgumentTypes;

  /// Constructor is private; use ::get to construct a new one.
  FunctionType(Context *K, Type *RTy, std::vector<Type *> ATys, bool IsV);

public:
  virtual ~FunctionType();
  static FunctionType *get(Type *RTy, std::vector<Type *> ATys, bool IsV);
  static FunctionType *get(Type *RTy);
  static bool classof(const Type *T);
  static inline void Profile(FoldingSetNodeID &ID, const Type *RTy,
                             const std::vector<Type *> &ATys, const bool &IsV);
  void Profile(FoldingSetNodeID &ID) const;
  Type *getATy(unsigned i);
  std::vector<Type *> getATys();
  Type *getRTy();
  bool isVariadic() const;
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
};

class PointerType : public Type {
protected:
  Type *ContainedType;
  PointerType(RTType RTy, Type *CTy);

public:
  virtual ~PointerType();
  static PointerType *get(Type *CTy);
  static bool classof(const Type *T);
  static inline void Profile(FoldingSetNodeID &ID, const Type *CTy);
  void Profile(FoldingSetNodeID &ID) const;
  Type *getCTy();
  size_t getSize();
  virtual llvm::Type *toLL(llvm::Module *M) override;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
};

class TensorType : public PointerType {
  std::vector<size_t> Dimensions;

  /// Private constructor; use ::get to construct a new one.
  TensorType(Type *CTy, std::vector<size_t> &Dims);

public:
  virtual ~TensorType();
  static TensorType *get(Type *CTy, std::vector<size_t> &Dims);
  static bool classof(const Type *T);
  static inline void Profile(FoldingSetNodeID &ID, const Type *CTy,
                             const std::vector<size_t> &Dims);
  std::vector<size_t> getDims();
  void Profile(FoldingSetNodeID &ID) const;

protected:
  virtual void print(DiagnosticPrinter &Stream) const override;
};
}

#endif
