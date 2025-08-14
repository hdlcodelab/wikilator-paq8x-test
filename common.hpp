cpp
/*********************************************************************
 *  common.hpp
 *
 *  Utility header used by the test program.
 *  ---------------------------------------------------------------
 *  What was wrong?
 *  ----------------
 *  The original file defined its own class called `string` and then
 *  performed `using namespace std;`.  That class hid the real
 *  `std::string` from the rest of the code, so every place that
 *  expected an STL string (operator<<, operator+, operator==, …)
 *  failed with messages such as “no member named ‘size’”.
 *
 *  The fix:
 *   • Include the real `<string>` header.
 *   • Remove the custom `class string`.
 *   • Do **not** pollute the global namespace with `using namespace std;`.
 *   • Bring only the I/O symbols we actually need into the global scope.
 *   • Provide a small overload for printing `std::string` (optional,
 *     but convenient for the test program).
 *********************************************************************/

#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <string>        // <-- the real std::string
#include <cstddef>
#include <cstring>

/* -----------------------------------------------------------------
 *  Bring only the symbols we really use into the global namespace.
 *  Keeping the header free of `using namespace std;` avoids the
 *  accidental hiding of standard library names.
 * ----------------------------------------------------------------- */
using std::cout;
using std::cerr;
using std::endl;

/* -----------------------------------------------------------------
 *  Helper for printing a std::string.  The standard library already
 *  provides an overload for `operator<<`, but keeping this tiny
 *  function makes the header self‑contained and mirrors the original
 *  intent of the author.
 * ----------------------------------------------------------------- */
inline std::ostream &operator<<(std::ostream &os, const std::string &s)
{
    return os << s.c_str();
}

#endif // COMMON_HPP
