#include "catch_amalgamated.hpp"
#include "PandoraEX/string.hpp"

TEST_CASE("PandoraEX String Class Tests", "[PandoraEX][String]") {
    using namespace PandoraEX;

    SECTION("String Constructors") {
        String str1;
        String str2("Hello");
        String str3(std::string("World"));
        String str4(str2);
        String str5(42);

        REQUIRE(str1.empty());
        REQUIRE(str2 == "Hello");
        REQUIRE(str3 == "World");
        REQUIRE(str4 == "Hello");
        REQUIRE(str5 == "42");
        
    }

    SECTION("String Pad Function") {
        String str = "7";
        String padded = str.pad(7, 2);
        REQUIRE(padded == "07");

        padded = str.pad("7", 2);
        REQUIRE(padded == "07");
    }

    SECTION("String Format Function") {
        String formatted = String().format("Value: %d", 42);
        REQUIRE(formatted == "Value: 42");
    }
}