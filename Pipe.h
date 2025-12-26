#ifndef PIPE_H
#define PIPE_H 

#include <string>
#include <iostream>
#include <unordered_map>
#include <set>

extern const std::unordered_map<int, double> PIPE_CAPACITY;
extern const std::set<int> ALLOWED_DIAMETERS;
extern const double INFINITE_WEIGHT;

class Pipe {
private:
    int id;
    std::string name;
    float length;
    int diameter;
    bool status;

public:
    Pipe();
    Pipe(int pipeId, const std::string& n, float l, int d);

    int getId() const { return id; }
    std::string getName() const { return name; }
    float getLength() const { return length; }
    int getDiameter() const { return diameter; }
    bool getStatus() const { return status; }

    void setName(const std::string& n) { name = n; }
    void setLength(float l) { length = l; }
    void setDiameter(int d) { diameter = d; }
    void setStatus(bool s) { status = s; }

    bool switchStatus(int o);
    void displayInfo() const;

    friend std::ostream& operator<<(std::ostream& out, const Pipe& p);
    friend std::istream& operator>>(std::istream& in, Pipe& p);
};

#endif