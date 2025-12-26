#include "CS.h"

CompressStation::CompressStation()
    : id(0), name(""), number_of_workshops(0), number_of_workshops_in_work(0), class_cs("") {
}

CompressStation::CompressStation(int stationId, const std::string& n, int workshops, int workshops_in_work, const std::string& cls)
    : id(stationId), name(n), number_of_workshops(workshops), number_of_workshops_in_work(workshops_in_work), class_cs(cls) {
}

double CompressStation::getUnusedPercentage() const {
    if (number_of_workshops == 0) return 0.0;
    return ((number_of_workshops - number_of_workshops_in_work) * 100.0) / number_of_workshops;
}

bool CompressStation::changeNumberOfWorkshopsInWork(int number) {
    if (number > number_of_workshops) {
        return false;
    }
    else if (number < 0) {
        return false;
    }
    else {
        number_of_workshops_in_work = number;
        return true;
    }
}

void CompressStation::displayInfo() const {
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Number of workshops: " << number_of_workshops << "\n";
    std::cout << "Number of workshops in work: " << number_of_workshops_in_work << "\n";
    std::cout << "Class: " << class_cs << "\n";
    std::cout << "Unused percentage: " << getUnusedPercentage() << "%\n\n";
}

std::ostream& operator<<(std::ostream& out, const CompressStation& cs) {
    out << cs.id << "\n";
    out << cs.name << "\n";
    out << cs.number_of_workshops << "\n";
    out << cs.number_of_workshops_in_work << "\n";
    out << cs.class_cs << "\n";
    return out;
}

std::istream& operator>>(std::istream& in, CompressStation& cs) {
    in >> cs.id;
    in.ignore();
    std::getline(in, cs.name);
    in >> cs.number_of_workshops;
    in >> cs.number_of_workshops_in_work;
    in.ignore();
    std::getline(in, cs.class_cs);
    return in;
}