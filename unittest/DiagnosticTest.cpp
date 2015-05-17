#include "rhine/IR.h"
#include "rhine/Support.h"
#include "gtest/gtest.h"

TEST(Diagnostic, BareDefun)
{
  std::string SourcePrg = "defun foo []";
  std::string ExpectedErr = "string stream:1:13:.*error:.*syntax error";
  EXPECT_DEATH(rhine::parseCodeGenString(SourcePrg), ExpectedErr);
}

TEST(Diagnostic, UnboundVariable)
{
  std::string SourcePrg = "defun foo [] var ~Int;";
  std::string ExpectedErr = "string stream:1:14:.*error:.*unbound variable var";
  EXPECT_DEATH(rhine::parseCodeGenString(SourcePrg), ExpectedErr);
}

TEST(Diagnostic, UntypedVariable)
{
  std::string SourcePrg = "defun foo [var] var;";
  std::string ExpectedErr = "string stream:1:17:.*error:.*untyped variable var";
  EXPECT_DEATH(rhine::parseCodeGenString(SourcePrg), ExpectedErr);
}

TEST(Diagnostic, NotAFunction)
{
  std::string SourcePrg =
    "defun main [] {\n"
    "  foo = 2;\n"
    "  foo 4;\n"
    "}\n";
  std::string ExpectedErr =
    "string stream:3:3:.*error:.*foo was not declared as a function";
  EXPECT_DEATH(rhine::parseCodeGenString(SourcePrg), ExpectedErr);
}

TEST(Diagnostic, FunctionNotFound)
{
  std::string SourcePrg =
    "defun main [] {\n"
    "  bar 4;\n"
    "}\n";
  std::string ExpectedErr =
    "string stream:2:3:.*error:.*unable to look up function bar";
  EXPECT_DEATH(rhine::parseCodeGenString(SourcePrg), ExpectedErr);
}