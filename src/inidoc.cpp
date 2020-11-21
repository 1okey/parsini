
#include "inidoc.hpp"


void inidoc::_copy(const inidoc& other) noexcept {
    if (*this != other) {
        this->sections = other.sections;
    }
}


inidoc::inidoc(){}


inidoc::~inidoc(){}


inidoc::inidoc(inidoc&& tmp_doc) {
    sections = tmp_doc.sections;
}


inidoc::inidoc(const inidoc& other) {
    _copy(other);
}


inidoc& inidoc::operator=(const inidoc& other) {
    _copy(other);
    return *this;
}


inidoc& inidoc::operator=(inidoc&& other) {
    _copy(other);
    return *this;
}


std::set<std::string> inidoc::GetSectionNames() {
    std::set<std::string> section_names;
    for(auto& [key, _]: sections) {
        section_names.insert(key);
    }
    return section_names;
}


inisection& inidoc::GetSection(const std::string& section_name) {
    auto section = sections.find(section_name);
    if (section == sections.end()) {
        throw new std::range_error("No section named " + section_name);
    }
    return section->second;
}


const inisection& inidoc::GetSection(const std::string& section_name) const {
    return const_cast<const inisection&>(GetSection(section_name));
}


const std::unordered_map<std::string, inisection>& inidoc::GetSections() const noexcept {
    return sections;
}


iniparam inidoc::GetParameter(
    const std::string& section_name, const std::string& param_name
) {
    auto section = GetSection(section_name);
    auto param = section.find(param_name);
    if (param == section.end()) {
        throw new std::range_error("No parameter named " + param_name + " in section " + section_name);
    }

    return param->second;
}


inidoc& inidoc::AddParam(
    const std::string& section_name, const std::string& key, const std::string& value
) {
    inisection& section = GetSection(section_name);
    iniparam var = value;

    section.insert({key, var});
    return *this;
}

inidoc& inidoc::AddParam(
    const std::string& section_name, const std::string& key, const std::vector<std::string>& value
) {
    inisection& section = GetSection(section_name);
    iniparam var = value;

    section.insert({key, var});
    return *this;
}


inidoc& inidoc::AddSection(const std::string& section_name) noexcept {
    auto section = sections.find(section_name);
    if (section == sections.end()) {
        sections.insert({section_name, inisection()});
    }
    return *this;
}


bool inidoc::operator==(const inidoc& other) const noexcept {
    return sections == other.sections;
}


bool inidoc::operator!=(const inidoc& other) const noexcept {
    return !(*this == other);
}


inidoc load(std::istream& file_stream)
{
    std::string line;
    std::string cur_section;
    std::string key, value;
    size_t closing_idx;
    inidoc doc;
    while(!file_stream.eof()) {
        getline(file_stream, line);
        if (line.empty()) {
            continue;
        }
        line = rtrim(line);
        
        if (isalpha(*line.begin()) && !cur_section.empty()) {
            auto pair = get_key_value(line);
            iniparam param = pair.second;
            if (std::holds_alternative<std::vector<std::string>>(param)) {
                doc.AddParam(cur_section, pair.first, std::get<std::vector<std::string>>(param));
            } else {
                doc.AddParam(cur_section, pair.first, std::get<std::string>(param));
            }
        } else {
            switch(*line.begin()) {
                case '[':
                    closing_idx = line.find_last_of(']');
                    if (closing_idx == std::string::npos) {
                        throw new std::invalid_argument("Provided document is invalid: missing closing bracker ']'");
                    }
                    cur_section = line.substr(1, closing_idx - 1);
                    doc.AddSection(cur_section);
                    break;
                case ';': case '#':
                    continue;
                case '\n':
                    cur_section.clear();
                    break;
            }
        }
    }
    return std::move(doc);
}

void save(const inidoc& document, std::string file_name)
{
    if (file_name.size() == 0) {
        file_name = "document.obj.ini";
    }

    std::ofstream file(file_name, std::ios::binary);

    if (!file.is_open()) {
        throw new std::bad_exception();
    }

    for (auto& [name, section]: document.GetSections()) {
        file << ('[' + name + ']') << '\n';
        for (auto& [key, value]: section) {
            file << key << '=';
            if (std::holds_alternative<std::string>(value)) {
                file << std::get<std::string>(value);
            } else {
                const std::vector<std::string>& param = std::get<std::vector<std::string>>(value);
                for (const std::string& item: param) {
                    file << item << ", ";
                }
            }
            file << '\n';
        }
        file << '\n';
    }

    file.close();
}