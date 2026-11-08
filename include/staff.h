#ifndef STAFF_H
#define STAFF_H

#include <string>

enum Role {
    MANAGER = 1,
    CHEF = 2,
    WAITER = 3,
    HEAD_CHEF = 4
};

struct StaffData {
    int id;
    char name[32];
    int role;
    double baseSalary;
    int active;

    int getId() const {
        return id;
    }
};

class Staff {
protected:
    int id;
    std::string name;
    double baseSalary;

    static int nextId;

public:
    Staff();
    Staff(const std::string &staffName, double salary);
    virtual ~Staff();

    static int nextIdValue();
    static void setNextIdValue(int value);

    int getId() const;
    std::string getName() const;
    void setName(const std::string &staffName);
    void setBaseSalary(double salary);

    virtual double calculateSalary() const = 0;
    virtual void display() const = 0;
    virtual Role getRole() const = 0;

    StaffData toData() const;
    void loadFromData(const StaffData &data);
};

class Chef : virtual public Staff {
public:
    Chef();
    Chef(const std::string &staffName, double salary);

    double calculateSalary() const override;
    void display() const override;
    Role getRole() const override;
};

class Waiter : virtual public Staff {
public:
    Waiter();
    Waiter(const std::string &staffName, double salary);

    double calculateSalary() const override;
    void display() const override;
    Role getRole() const override;
};

class Manager : virtual public Staff {
protected:
    double allowance;

public:
    Manager();
    Manager(const std::string &staffName, double salary);

    double calculateSalary() const override;
    void display() const override;
    Role getRole() const override;
};

class HeadChef : public Chef, public Manager {
public:
    HeadChef();
    HeadChef(const std::string &staffName, double salary);

    double calculateSalary() const override;
    void display() const override;
    Role getRole() const override;
};

#endif
