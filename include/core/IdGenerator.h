#pragma once

namespace restaurant {
namespace core {

// IdGenerator owns all the running ID counters.  Because the members are
// static they are shared by every object in the program.
class IdGenerator {
public:
    static int nextStaffId();
    static int nextOrderId();
    static int nextDishId();
    static int nextIngredientId();
    static int nextBillId();
    static int nextTableId();
    static int nextPayslipId();

    // Used after loading data from disk so new IDs do not clash with old ones.
    static void seedStaff(int value);
    static void seedOrder(int value);
    static void seedDish(int value);
    static void seedIngredient(int value);
    static void seedBill(int value);
    static void seedTable(int value);
    static void seedPayslip(int value);

private:
    static int staffCounter_;
    static int orderCounter_;
    static int dishCounter_;
    static int ingredientCounter_;
    static int billCounter_;
    static int tableCounter_;
    static int payslipCounter_;
};

} // namespace core
} // namespace restaurant
