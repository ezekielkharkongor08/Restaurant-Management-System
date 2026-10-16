#include "core/IdGenerator.h"

namespace restaurant {
namespace core {

int IdGenerator::staffCounter_ = 1;
int IdGenerator::orderCounter_ = 1;
int IdGenerator::dishCounter_ = 1;
int IdGenerator::ingredientCounter_ = 1;
int IdGenerator::billCounter_ = 1;
int IdGenerator::tableCounter_ = 1;
int IdGenerator::payslipCounter_ = 1;

int IdGenerator::nextStaffId() { return staffCounter_++; }
int IdGenerator::nextOrderId() { return orderCounter_++; }
int IdGenerator::nextDishId() { return dishCounter_++; }
int IdGenerator::nextIngredientId() { return ingredientCounter_++; }
int IdGenerator::nextBillId() { return billCounter_++; }
int IdGenerator::nextTableId() { return tableCounter_++; }
int IdGenerator::nextPayslipId() { return payslipCounter_++; }

void IdGenerator::seedStaff(int value) { if (value > staffCounter_) staffCounter_ = value; }
void IdGenerator::seedOrder(int value) { if (value > orderCounter_) orderCounter_ = value; }
void IdGenerator::seedDish(int value) { if (value > dishCounter_) dishCounter_ = value; }
void IdGenerator::seedIngredient(int value) { if (value > ingredientCounter_) ingredientCounter_ = value; }
void IdGenerator::seedBill(int value) { if (value > billCounter_) billCounter_ = value; }
void IdGenerator::seedTable(int value) { if (value > tableCounter_) tableCounter_ = value; }
void IdGenerator::seedPayslip(int value) { if (value > payslipCounter_) payslipCounter_ = value; }

} // namespace core
} // namespace restaurant
