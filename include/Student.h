#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <json.hpp>
#include <string>

struct Student {
    std::string Name = "DEFAULT";
    double Grade = -1.0;
    int Year = -1;

    bool operator<(const Student& other) const {
        return Name <= other.Name;
    }

    bool isDefault() const {
        return Name == "DEFAULT" && Grade == -1.0 && Year == -1;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << "Name: " << student.Name << ", Grade: " << student.Grade << ", Year: " << student.Year;
    return os;
}

inline void to_json(nlohmann::json& j, const Student& student) {
    j = nlohmann::json{{"name", student.Name}, {"grade", student.Grade}, {"year", student.Year}};
}

inline void from_json(const nlohmann::json& j, Student& student) {
    j.at("name").get_to(student.Name);
    j.at("grade").get_to(student.Grade);
    j.at("year").get_to(student.Year);
}

#endif