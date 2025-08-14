/*********************************************************************
 *  wikilator-paq8x-test.cpp
 *
 *  Small test program that exercises the HTTP‑like interface of the
 *  PAQ‑8x test harness.  After fixing the `string` conflict the code
 *  simply uses `std::string` everywhere.
 *********************************************************************/

#include "common.hpp"          // pulls in iostream, string, etc.
#include <string>              // explicit include for clarity

int main()
{
    /* -----------------------------------------------------------------
     *  Use the proper std::string type.  The `using` declaration from
     *  common.hpp brings `cout`, `cerr`, and `endl` into the global
     *  namespace, but we keep the string type fully qualified to avoid
     *  any future name clashes.
     * ----------------------------------------------------------------- */
    std::string url     = "https://example.com/api";
    std::string payload = R"({"cmd":"test"})";
    std::string response = "OK";

    /* -----------------------------------------------------------------
     *  Concatenation: the left operand must be a std::string so the
     *  overload `operator+(const std::string&, const std::string&)`
     *  is selected.  Writing `std::string("Result: ")` forces that.
     * ----------------------------------------------------------------- */
    std::string logMsg = std::string("Result: ") + response;

    /* -----------------------------------------------------------------
     *  Equality test works unchanged because both sides are std::string
     *  or a string literal (the library provides the appropriate overload).
     * ----------------------------------------------------------------- */
    if (response == "OK") {
        // … do something useful …
    }

    /* -----------------------------------------------------------------
     *  Print the log message.
     * ----------------------------------------------------------------- */
    cout << "Log: " << logMsg << endl;

    return 0;
}
