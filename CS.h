#pragma once
#include <string>
#include <iostream>

class CompressStation {
private:
    int id;
    std::string name;
    int number_of_workshops;
    int number_of_workshops_in_work;
    std::string class_cs;

public:
    CompressStation();
    CompressStation(int stationId, const std::string& n, int workshops, int workshops_in_work, const std::string& cls);

    int getId() const { return id; }
    std::string getName() const { return name; }
    int getNumberOfWorkshops() const { return number_of_workshops; }
    int getNumberOfWorkshopsInWork() const { return number_of_workshops_in_work; }
    std::string getClass() const { return class_cs; }

    double getUnusedPercentage() const;

    void setName(const std::string& n) { name = n; }
    void setNumberOfWorkshops(int n) { number_of_workshops = n; }
    void setNumberOfWorkshopsInWork(int n) { number_of_workshops_in_work = n; }
    void setClass(const std::string& cls) { class_cs = cls; }

    bool changeNumberOfWorkshopsInWork(int number);
    void displayInfo() const;

    friend std::ostream& operator<<(std::ostream& out, const CompressStation& cs);
    friend std::istream& operator>>(std::istream& in, CompressStation& cs);
};

