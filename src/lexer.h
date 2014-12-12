#ifndef JAVAMM_LEXER_H
#define JAVAMM_LEXER_H

// Making sure everything is included
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Taking over
#undef YY_DECL
#define YY_DECL int javamm::Lexer::yylex()

namespace javamm {

class Lexer : public yyFlexLexer {
public:
private:
  int yylex();
};

} // End namespace javamm

#endif // JAVAMM_LEXER_H

