#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <set>
#include <unordered_map>

#include "inidoc.hpp"
#include "utils.hpp"

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
    inidoc doc;
    doc.AddSection("Section 1");
    doc.AddSection("Section 2");
    doc.AddSection("Section 3");
    doc.AddSection("Section 3");
    doc.AddSection("Section 2");
    std::set<std::string> expected_sections = {"Section 1", "Section 2", "Section 3"};
    assert(doc.GetSectionNames() == expected_sections);
}

void test_add_params()
{
    inidoc doc;
    
    doc.AddSection("Section 1");
    doc.AddParam("Section 1", "One", "Two");
    doc.AddParam("Section 1", "Two", "Three");

    std::unordered_map<std::string, iniparam> expected;
    expected.insert({"One", iniparam("Two")});
    expected.insert({"Two", iniparam("Three")});

    auto actual = doc.GetSection("Section 1");
    assert(actual == expected);
}

int main() {
    try {
        test_add_section();
        test_add_params();
        std::cout << "All tests passed\n";
    } catch (std::exception) {
        std::cout << "Failed to test parsini\n";
    }
}