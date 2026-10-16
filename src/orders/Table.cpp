#include "orders/Table.h"
#include "core/Serialization.h"

namespace restaurant {
namespace orders {

std::string tableStatusName(TableStatus status) {
    switch (status) {
        case TABLE_FREE:     return "Free";
        case TABLE_OCCUPIED: return "Occupied";
        case TABLE_RESERVED: return "Reserved";
        default:             return "Unknown";
    }
}

Table::Table() : number_(0), seats_(0), status_(TABLE_FREE), waiterId_(0) {}

Table::Table(int number, int seats)
    : number_(number), seats_(seats), status_(TABLE_FREE), waiterId_(0) {}

int Table::id() const { return number_; }
int Table::number() const { return number_; }
int Table::seats() const { return seats_; }
TableStatus Table::status() const { return status_; }
int Table::waiterId() const { return waiterId_; }

void Table::setSeats(int seats) { seats_ = seats; }
void Table::setStatus(TableStatus status) { status_ = status; }
void Table::setWaiterId(int waiterId) { waiterId_ = waiterId; }

bool Table::isFree() const { return status_ == TABLE_FREE; }

Table* Table::clone() const { return new Table(*this); }

void Table::print(std::ostream& os) const {
    os << "Table " << number_ << "  seats=" << seats_
       << "  status=" << tableStatusName(status_)
       << "  waiter=" << (waiterId_ ? "set" : "none");
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
    table.print(os);
    return os;
}

void Table::write(std::ostream& os) const {
    core::writeValue(os, number_);
    core::writeValue(os, seats_);
    int st = static_cast<int>(status_);
    core::writeValue(os, st);
    core::writeValue(os, waiterId_);
}

void Table::read(std::istream& is) {
    core::readValue(is, number_);
    core::readValue(is, seats_);
    int st = 0;
    core::readValue(is, st);
    status_ = static_cast<TableStatus>(st);
    core::readValue(is, waiterId_);
}

} // namespace orders
} // namespace restaurant
