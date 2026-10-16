#pragma once
#include <string>
#include <ostream>
#include <istream>
#include "interfaces/Interfaces.h"

namespace restaurant {
namespace orders {

enum TableStatus { TABLE_FREE = 0, TABLE_OCCUPIED = 1, TABLE_RESERVED = 2 };

std::string tableStatusName(TableStatus status);

// Table: table number, number of seats and status.  It is linked to the waiter
// who serves it.
class Table : public interfaces::Printable, public interfaces::Serializable {
public:
    Table();
    Table(int number, int seats);

    int id() const;            // for Repository compatibility (== table number)
    int number() const;
    int seats() const;
    TableStatus status() const;
    int waiterId() const;

    void setSeats(int seats);
    void setStatus(TableStatus status);
    void setWaiterId(int waiterId);

    bool isFree() const;

    Table* clone() const;

    void print(std::ostream& os) const;
    void write(std::ostream& os) const;
    void read(std::istream& is);

private:
    int number_;
    int seats_;
    TableStatus status_;
    int waiterId_;
};

std::ostream& operator<<(std::ostream& os, const Table& table);

} // namespace orders
} // namespace restaurant
