//===----------------- lexer.h - Lexer class definition -------------------===//
/// \file
///
/// This file contains the lexer declaration
///
//===----------------------------------------------------------------------===//

#ifndef JAVAMM_LEXER_H
#define JAVAMM_LEXER_H

// Making sure everything is included
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Taking over
#undef YY_DECL
#define YY_DECL int javamm::Lexer::yylex()

#include "parser.hpp"

namespace javamm {

/// \brief Lexer
/// This class replaces the default lexer built by Flex.
class Lexer : public yyFlexLexer {
public:
  Lexer(std::istream *In) : yyFlexLexer(In), yyval(nullptr){};

  int yylex(javamm::Parser::semantic_type *Val) {
    yyval = Val;
    return yylex();
  }

private:
  int yylex();

private:
  Parser::semantic_type *yyval;
};

} // End namespace javamm

#endif // JAVAMM_LEXER_H

