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
using std::get;


const char* const simple_ini_str =
R"(`; comment
# unix-style comment
\n
[Section1]
; setcion comment
var1=value_1 ; parameter commment
var2=value_2
\n
[Section2]
var1=value_1
var2=value_2
\n
; multiple coma separated params
[Section3]
var1=value_1_1, value_1_2, value_1_3
var2=value_2
)";

const char* const ini_str =
R"(`; comment
# unix-style comment
\n
[Section1]
; setcion comment
var1=value_1 ; parameter commment
var2=value_2
\n 
[Section2]
var1=value_1
var2=value_2
\n
; multiple coma separated params
[Section3]
var1=value_1_1, value_1_2, value_1_3
var2=value_2
\n
; Zend framework list of params
[Section3.1]
var1[]=value_1_1
var1[]=value_1_2
var1[]=value_1_3
var2=value_2
\n
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

void test_add_vector_param()
{
    cout << " * test_add_vector_param - started\n";

    inidoc doc;
    
    vector<string> vec_param = vector<string>{"Three", "Four", "Five"};

    doc.AddSection("Section 1");
    doc.AddParam("Section 1", "Two", vec_param);

    std::unordered_map<std::string, iniparam> expected;
    expected.insert({"Two", iniparam(vec_param)});

    assert(doc.GetSection("Section 1") == expected);

    auto parameter = doc.GetParameter("Section 1", "Two");

    assert(get<vector<string>>(parameter) == vec_param);

    cout << " * test_add_vector_param - done\n";
}

void test_loading_from_input()
{
    cout << " * test_loading_from_input - started\n";

    std::istringstream is(simple_ini_str);
    inidoc doc = load(is);

    assert (doc.GetSection("Section1").size() == 2);

    inisection& section3 = doc.GetSection("Section3");
    assert(section3.size() == 2);

    iniparam section_three_param = section3.at("var1");
    assert (get<vector<string>>(section_three_param).size() == 3);

    cout << " * test_loading_from_input - done\n";
}

void test_saving_loaded_from_input()
{
    cout << " * test_saving_loaded_from_input - started\n";

    // loading and saving to other file
    std::istringstream is(simple_ini_str);
    inidoc doc = load(is);
    string file_name = "temp.ini";
    save(doc, file_name);

    // reading duplicate and checking if params are equal
    std::ifstream file;
    file.open(file_name, std::ios::in);
    inidoc duplicate = load(file);
    file.close();

    assert (doc.GetSection("Section1") == duplicate.GetSection("Section1"));
    assert (doc.GetSection("Section2") == duplicate.GetSection("Section2"));
    assert (doc.GetSection("Section3") == duplicate.GetSection("Section3"));


    cout << " * test_saving_loaded_from_input - done\n";
}

int main() {
    try {
        test_add_section();
        test_add_params();
        test_add_vector_param();
        test_loading_from_input();
        test_saving_loaded_from_input();
        std::cout << "\nAll tests passed\n";
    } catch (std::exception) {
        std::cout << "\nFailed to test parsini\n";
    }
}