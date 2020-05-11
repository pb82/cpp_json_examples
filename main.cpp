#include <iostream>

#include "json/include/parser.h"
#include "json/include/printer.h"

#include "io/file.h"

int main() {
    using namespace JSON;
    using namespace std;

    // Examples for producing and parsing json
    // Producing first, Parsing afterwards

    // PrettyPrinter can print any JSON::Value nicely formatted and ready for
    // human consumption.
    // There is also Printer which does not use newlines and
    // indentations and instead creates somewhat smaller output
    PrettyPrinter printer;

    // Parser can parse strings with valid json into a JSON::Value
    // Parser objects can be reused: they reset themselves before every
    // parse
    Parser parser;

    // A Value can hold any valid json value, no matter how simple or complex
    // It has methods to access, convert, compare and manipulate those values
    // Internally, the library uses std::tuple
    Value v;

    // Creating simple values
    v = "a string";
    std::cout << printer.print(v) << std::endl;

    v = 25;
    std::cout << printer.print(v) << std::endl;

    v = false;
    std::cout << printer.print(v) << std::endl;

    // Arrays
    v = {1, 2, 3, "4", false, null};
    std::cout << printer.print(v) << std::endl;

    // Array index
    std::cout << v[3].as<string>() << std::endl;

    // Nested arrays
    v = {1, {2, {3, 4}}};
    std::cout << printer.print(v) << std::endl;

    // Access nested index and convert to string
    std::cout << v[1][1][0].as<int>() << std::endl;

    // Objects
    // The C++ compiler needs to be hinted on whether the expression denotes
    // a JSON object or an array. Objects are constructed from a list of key value pairs where the
    // key is always a string and the value can be anything
    v = Object{{"key", "value"}};
    std::cout << printer.print(v) << std::endl;

    // If you don't use the hint, the compiler will interpret the expression
    // as a nested array:
    v = {{"key", "value"}};
    std::cout << printer.print(v) << std::endl;

    // Nested objects
    // The library uses initializer list and conversion constructurs to allow a
    // somewhat json-like notation:
    v = Object{
            {"name",       "json"},
            {"properties", Object{
                    {"size",     42},
                    {"integers", {1, 2, "3"}}
            }}
    };
    std::cout << printer.print(v) << std::endl;

    // But this can get a bit ugly. You can also use index notation to create the same object:
    v = {};
    v["name"] = "json";
    v["properties"]["size"] = 42;
    v["properties"]["integers"] = {1, 2, "3"};
    std::cout << printer.print(v) << std::endl;

    // Conversions
    // The library has built in conversions
    v = 1;

    // As int
    std::cout << v.as<int>() << std::endl;

    // As string
    std::cout << v.as<string>() << std::endl;

    // As bool
    if (v.as<bool>()) {
        std::cout << "you should see this because 1 converts to boolean true" << std::endl;
    }

    // Let's convert strings to numbers. The library uses stringstream builtin conversion
    // to achieve this
    v = "2.5";
    std::cout << v.as<float>() * 2 << std::endl;

    // null
    // null is defined as a constant in the namespace
    v = null;
    std::cout << printer.print(v) << std::endl;

    // Let's try an invalid conversion
    // The library throws a std::runtime_error when a value cannot be converted into another
    v = null;
    try {
        // null cannot be converted to a number
        v.as<int>();
    } catch (const std::runtime_error &) {
        std::cout << "you should see this because we caught the error" << std::endl;
    }

    // Testing types
    // you can use the `is` function of a JSON::Value to test against any type
    v = 1;
    if (v.is(JSON_NUMBER)) {
        std::cout << "1 is indeed a number" << std::endl;
    }
    if (v.is(JSON_OBJECT) == false) {
        std::cout << "and it is clearly not an object" << std::endl;
    }

    // Matching over an unknown values type:
    switch (v.getType()) {
        case JSON_NUMBER:
            std::cout << "it's definitely a number" << std::endl;
            break;
        default:
            std::cout << "this should never happen" << std::endl;
    }

    // All of this works on nested values too:
    v = Object{{"a", {97, 98, 99}}};
    if (v["a"].is(JSON_ARRAY) && v["a"][0].as<int>() == 97) {
        std::cout << "you should see this line" << std::endl;
    }

    // Values can be mutated:
    // note: asMutable only has implementations for `Array` and `Object` (other values can
    // simply be overwritten).
    auto &ref = v["a"].asMutable<Array>();
    ref.push_back(100);
    std::cout << printer.print(v) << std::endl;

    // or just like this:
    v["a"].push_back(101);
    std::cout << printer.print(v) << std::endl;

    // Iterating over the previously created array:
    for (auto item : v["a"].as<Array>()) {
        std::cout << item.as<int>() << std::endl;
    }

    // Or with index:
    auto array = v["a"].as<Array>();
    for (std::size_t i = 0; i < array.size(); ++i) {
        std::cout << i << ": " << array[i].as<int>() << std::endl;
    }

    // ==========================================
    // Parsing
    // ==========================================

    // From string
    string json = "[1,2,3]";
    parser.parse(v, json);
    std::cout << v[1].as<int>() << std::endl;

    // Catching parse errors
    // The library throws a ParseError with some info about the line and index
    json = "[1,2,3";
    try {
        parser.parse(v, json);
    } catch (const ParseError &err) {
        std::cout << "parse error: " << err.what() << std::endl;
    }

    // Unicode
    // JSON uses the \u<code point> notation just as javascript
    json = "[22, 23, \"\\u03bb\"]";
    parser.parse(v, json);
    std::cout << printer.print(v) << std::endl;

    // Reading from a file
    File file("./example.json");
    if (file.open()) {
        json.resize(file.size);
        file.read(json, 0, file.size);
        parser.parse(v, json);

        std::cout << "name: " << v["name"].as<string>() << std::endl;

        Printer uglyPrinter;
        std::cout << uglyPrinter.print(v["important_values"]) << std::endl;
        std::cout << v["important_values"][3]["a"].as<string>() << std::endl;
    }
}
