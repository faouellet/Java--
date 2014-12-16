#ifndef JAVAMM_LEXER_H
#define JAVAMM_LEXER_H

// Making sure everything is included
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Taking over
#undef YY_DECL
#define YY_DECL int Lexer::yylex()

#include "parser.hpp"

namespace javamm {

class Lexer : public yyFlexLexer {
public:
  Lexer(std::istream *In) : yyFlexLexer(In), yyval(nullptr){};

  int yylex(javamm::Parser::semantic_type *Val) {
    yyval = Val;
    return yylex();
  }

private:
  int yylex();
  Parser::semantic_type *yyval;
};

} // End namespace javamm

#endif // JAVAMM_LEXER_H

