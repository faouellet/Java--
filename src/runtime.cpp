//===------------- runtime.h - Runtime fonctions definitions --------------===//
/// \file
///
/// This file contains the functions built into the java-- language.
/// They are:
///   - println
///   - read
///
//===----------------------------------------------------------------------===//

#ifndef JAVAMM_RUNTIME_H
#define JAVAMM_RUNTIME_H

#include <iostream>

//namespace javamm {
void println(const char *Message) { std::cout << Message << std::endl; }

void read() { /*TODO*/
}

int main() { println("Hello World"); }

//} // End namespace javamm

#endif // JAVAMM_RUNTIME_H

