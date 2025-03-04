#pragma once

#include <memory>

#include "unicode/unistr.h"

#include "faust-lib/common/vector.h"

namespace faust {

class AbstractSyntaxNode {
public:
  virtual ~AbstractSyntaxNode() = default;
};

class NumberAST : public AbstractSyntaxNode {
public:
  inline NumberAST(double value) : value(value) {}

private:
  double value;

protected:
};

class VariableExprAST : public AbstractSyntaxNode {
public:
  inline VariableExprAST(const icu::UnicodeString &name) : name(name) {}

private:
  icu::UnicodeString name;

protected:
};

class CallExprAST : public AbstractSyntaxNode {
public:
  inline CallExprAST(const icu::UnicodeString &callee, Vector<std::unique_ptr<AbstractSyntaxNode>> &&arguments)
      : callee(callee), arguments(std::move(arguments)) {}

private:
  icu::UnicodeString callee;
  Vector<std::unique_ptr<AbstractSyntaxNode>> arguments;
};

class VariableDeclAST : public AbstractSyntaxNode {
public:
  inline VariableDeclAST(const icu::UnicodeString &name, AbstractSyntaxNode *value) : name(name), value(value) {}

private:
  icu::UnicodeString name;
  AbstractSyntaxNode *value;
};

} // namespace faust
