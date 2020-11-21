#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <set>
#include <unordered_map>
#include <type_traits>
#include <variant>

#include "inidoc.hpp"
#include "utils.hpp"

using std::vector;
using std::string;
using std::cout;

const char* const ini_str =
R"(`; comment
# unix-style comment

[Section1]
; setcion comment
var1=value_1 ; parameter commment
var2=значение_2
  
[Section2]
var1=value_1
var2=значение_2

; multiple coma separated params
[Section3]
var1=value_1_1, value_1_2, value_1_3
var2=значение_2
; Zend framework list of params
[Section3.1]
var1[]=value_1_1
var1[]=value_1_2
var1[]=value_1_3
var2=значение_2

; Empty params
[Section4.0]
[ViewState]
Mode=
Vid=
FolderType=Generic
)";

void test_add_section()
{
    cout << " * test_add_section - started\n";

    inidoc doc;
    doc.AddSection("Section 1");
    doc.AddSection("Section 2");
    doc.AddSection("Section 3");
    doc.AddSection("Section 3");
    doc.AddSection("Section 2");
    std::set<std::string> expected_sections = {"Section 1", "Section 2", "Section 3"};
    assert(doc.GetSectionNames() == expected_sections);

    cout << " * test_add_section - done\n";
}

void test_add_params()
{
    cout << " * test_add_params - started\n";

    inidoc doc;
    
    doc.AddSection("Section 1");
    doc.AddParam("Section 1", "One", "Two");
    doc.AddParam("Section 1", "Two", "Three");

    std::unordered_map<std::string, iniparam> expected;
    expected.insert({"One", iniparam("Two")});
    expected.insert({"Two", iniparam("Three")});

    auto actual = doc.GetSection("Section 1");
    assert(actual == expected);

    cout << " * test_add_params - done\n";
}

template<class> inline constexpr bool always_false_v = false;

void test_add_vector_param()
{
    cout << " * test_add_vector_param - started\n";

    inidoc doc;
    
    vector<string> vec_param = vector<string>{"Three", "Four", "Five"};

    doc.AddSection("Section 1");
    doc.AddParam("Section 1", "Two", vec_param);

    std::unordered_map<std::string, iniparam> expected;
    expected.insert({"Two", iniparam(vec_param)});

    auto actual = doc.GetSection("Section 1");
    assert(actual == expected);

    auto parameter = doc.GetParameter("Section 1", "Two");

    assert(std::get<vector<string>>(parameter) == vec_param);

    cout << " * test_add_vector_param - done\n ";
}

int main() {
    try {
        test_add_section();
        test_add_params();
        test_add_vector_param();
        std::cout << "\nAll tests passed\n";
    } catch (std::exception) {
        std::cout << "\nFailed to test parsini\n";
    }
}