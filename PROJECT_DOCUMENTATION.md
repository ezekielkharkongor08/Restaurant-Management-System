# Project Documentation - Restaurant Management System (C++)

This document describes **every code file** in the project, the **classes** each
file contains, and **what functionality** each class provides. The project is
written in pure C++14 and is split across many files instead of one big file;
headers are reused by other files through `#include`.

---

## 0. High-level architecture

All code lives inside one root namespace `restaurant`, split into smaller
namespaces (groups):

| Namespace      | Purpose                                                        |
|----------------|----------------------------------------------------------------|
| `restaurant::core`       | Helper classes: `Money`, `Date`, `Time`, exceptions, ID generator, serialization helpers, type tags |
| `restaurant::interfaces` | Abstract "ability" classes: `Serializable`, `Payable`, `Printable` |
| `restaurant::staff`      | `Person`, `Staff` and every role class (including the two diamond classes) |
| `restaurant::payroll`    | Pay policies, `Payroll`, `Payslip`, `TaxConfig` |
| `restaurant::menu`       | `Ingredient`, `Dish`, `Inventory` |
| `restaurant::orders`     | `Table`, `OrderItem`, `Order`, `Bill` |
| `restaurant::storage`    | `Repository<T>`, `FileStore<T>`, sorting templates, `StaffFactory` |
| `restaurant::ui`         | `Auth`, `RestaurantSystem`, role menus, console helpers |

### Folder layout

```
OOPs project/
  include/            <- all header (.h) files, grouped by namespace
    core/ interfaces/ staff/ payroll/ menu/ orders/ storage/ ui/
  src/                <- all implementation (.cpp) files, mirroring include/
    main.cpp          <- the single entry point
  data/               <- binary data files (created automatically on first run)
  build/              <- compiler output (restaurant.exe)
  build.ps1  run.ps1  <- build / run scripts
  README.md  PROJECT_DOCUMENTATION.md
```

### How classes use each other

* `Staff` **is-a** `Person`, **is-a** `Payable`, **is-a** `Serializable` and
  **is-a** `Printable` (multiple inheritance of one concrete + three abstract
  classes).
* `Staff` **has-a** pointer to a `PayPolicy` (composition) so a person's pay
  method can be swapped at run time.
* `HeadChef` inherits `Chef` and `Manager`; `ShiftSupervisor` inherits `Waiter`
  and `Cashier`. Both parents inherit `Staff` **virtually** to solve the diamond
  problem.
* `Inventory` **has-a** `Repository<Ingredient>`.
* `Repository<T>` **has-a** `vector<T*>`; `FileStore<T>` saves/loads them.
* `RestaurantSystem` owns everything and is the object every menu talks to.

---

## 1. `core` layer - small, reusable helpers

### `include/core/Money.h` / `src/core/Money.cpp` - class `Money`
Stores an amount in **paise** as a `long long`, so money arithmetic has no
floating point rounding errors.
**Functionality:** constructors, `fromRupees` / `fromPaise` factories, getters
(`paise`, `rupees`), overloaded operators `+ - * += -= < > <= >= == !=` so money
can be used like a normal number, `abs`, `toString()`, and `write/read` for
binary files. `operator<<` prints `Rs 1234.50`.

### `include/core/Date.h` / `src/core/Date.cpp` - class `Date`
Stores day / month / year and **validates itself** (an invalid date throws
`InvalidInputException`, e.g. 31 February is rejected).
**Functionality:** `isLeapYear`, `daysInMonth`, `valid`, a prefix/postfix `++`
that moves to the next day, `operator-` that returns the **number of days
between two dates**, comparison operators, `toString`, `julianDay` (internal),
binary `write/read`, and stream `operator<<` / `operator>>` (`dd/mm/yyyy`).

### `include/core/Time.h` / `src/core/Time.cpp` - class `Time`
Stores hours and minutes, used for shift start/end and to compute worked hours
and overtime.
**Functionality:** `valid`, `minutesSinceMidnight`, `operator-` giving the
minute difference between two times, comparison, `toString`, `write/read`,
stream `operator<<` / `operator>>` (`hh:mm`).

### `include/core/IdGenerator.h` / `src/core/IdGenerator.cpp` - class `IdGenerator`
A class whose counters are **static**, so they are shared by every object.
**Functionality:** `nextStaffId`, `nextOrderId`, `nextDishId`,
`nextIngredientId`, `nextBillId`, `nextTableId`, `nextPayslipId` return the next
unique ID; `seed*` methods push the counters forward after data is loaded from
disk so new IDs never clash with saved ones.

### `include/core/Exceptions.h` - exception hierarchy
Header-only. Base `RestaurantException` derives from `std::exception`; children
`InvalidInputException`, `StaffNotFoundException`, `InsufficientStockException`,
`UnauthorizedAccessException`, `FileCorruptException`.
**Functionality:** every menu action is wrapped in `try/catch`, so bad input
never crashes the program - it prints a clear message instead.

### `include/core/Serialization.h` - binary helpers (header-only)
Function templates `writeValue` / `readValue` (raw bytes of a primitive) and
`writeString` / `readString` (a length prefix followed by the characters).
**Functionality:** reused by every class that is saved to a file.

### `include/core/Tags.h` - type tags (header-only)
Enums `StaffTag` (MANAGER, CHEF, WAITER, CASHIER, CLEANER, HEAD_CHEF,
SHIFT_SUPERVISOR) and `PolicyTag` (MONTHLY_SALARY, HOURLY_WAGE, COMMISSION_PAY).
**Functionality:** allow one data file to hold different staff roles / pay
policies, because the tag says which class to rebuild when loading.

---

## 2. `interfaces` layer - abstract "ability" classes

### `include/interfaces/Interfaces.h` (header-only)
Three pure-virtual classes, each expressing one ability:
* `Serializable` - `write(ostream&)` and `read(istream&)` (can be saved to a file).
* `Payable` - `calculatePay()` (can be paid).
* `Printable` - `print(ostream&)` (can show itself on screen).

Because a class can inherit several of these, this is how **multiple
inheritance of abstract classes** is used cleanly.

---

## 3. `staff` layer - the main part

### `include/staff/Person.h` / `src/staff/Person.cpp` - class `Person`
Base class of every human. Holds `name`, `phone`, `address` (protected, i.e.
encapsulation). **Functionality:** default and parameterised constructors,
virtual destructor, getters/setters, a `print`, and protected `writeOwn/readOwn`
used by `Staff` for serialization.

### `include/staff/Attendance.h` / `src/staff/Attendance.cpp` - class `Attendance`
The monthly record of one staff member. Stores present days, absent days, leave
days, overtime hours, tips earned and attributed sales.
**Functionality:** `markPresent`, `markAbsent`, `markLeave`, `addOvertime`,
`addTips`, `addSales`, getters, `reset`, and `write/read`. `Payroll` is a
**friend** of this class. `operator<<` prints a short summary.

### `include/staff/Staff.h` / `src/staff/Staff.cpp` - class `Staff` (abstract)
The parent of every worker. Inherits `Person`, `Payable`, `Serializable`,
`Printable`. Holds staff `id`, `joinDate`, `baseSalary`, `password`, an
`Attendance` object and a `PayPolicy*`.
**Functionality:**
* Pure virtual `calculatePay()`, `showDuties()`, `clone()`, `roleName()`,
  `typeTag()` - so `Staff` cannot be instantiated directly (abstraction).
* A **copy constructor** and **assignment operator** that make a **deep copy**
  of the `PayPolicy` pointer; a **virtual destructor** so the correct derived
  destructor runs.
* A static counter `totalStaff()`.
* `setPayPolicy` / `changePayPolicy` to swap the pay method at run time.
* `write/read` (Person part first, then the Staff part) and `print`.
* `Payroll` and `StaffFactory` are declared friends.

### `include/staff/Manager.h` / `src/staff/Manager.cpp` - class `Manager`
Runs the restaurant. Inherits `Staff` **virtually**.
**Functionality:** stores a fixed allowance and a `canApproveLeave` flag.
`calculatePay()` = policy pay + allowance; `showDuties()` describes approving
leave, payroll and reports; `clone()`, `typeTag()`, `write/read`.

### `include/staff/Chef.h` / `src/staff/Chef.cpp` - class `Chef`
Cooks orders. Inherits `Staff` virtually.
**Functionality:** a per-dish rate, a count of dishes cooked, and a **queue** of
order IDs (`enqueueOrder`, `nextOrder`, `dequeueOrder`). `calculatePay()` =
policy pay + rate x dishes cooked; `showDuties()`, `clone()`, `write/read`.

### `include/staff/Waiter.h` / `src/staff/Waiter.cpp` - class `Waiter`
Takes orders and receives tips. Inherits `Staff` virtually.
**Functionality:** counts orders taken, `totalTips()` / `addTip()` forwarding to
attendance. Default pay policy is `CommissionPay(1.0)` so pay = base + tips.

### `include/staff/Cashier.h` / `src/staff/Cashier.cpp` - class `Cashier`
Creates bills and takes payment. Inherits `Staff` virtually.
**Functionality:** overtime rate and count of bills issued. `calculatePay()` =
policy pay + overtime rate x overtime hours.

### `include/staff/Cleaner.h` / `src/staff/Cleaner.cpp` - class `Cleaner`
Cleans tables and the kitchen. Inherits `Staff` virtually.
**Functionality:** hourly wage and a **cleaning schedule** (`addSchedule`,
`schedule`). Default pay policy is `HourlyWage`.

### `include/staff/HeadChef.h` / `src/staff/HeadChef.cpp` - class `HeadChef` (diamond #1)
`class HeadChef : public Chef, public Manager`. Both parents inherit `Staff`
virtually, so a `HeadChef` has only **one** `Staff` part - this is the diamond
problem solution.
**Functionality:** overrides `calculatePay()` (policy pay + allowance + per-dish
rate x dishes), `showDuties()` (cooks **and** manages), `roleName()`, `clone()`,
`typeTag()`, and `write/read` (writes the virtual base once, then the Chef part,
then the Manager part). It must override these because both parents provide them
(otherwise the call would be ambiguous).

### `include/staff/ShiftSupervisor.h` / `src/staff/ShiftSupervisor.cpp` - class `ShiftSupervisor` (diamond #2)
`class ShiftSupervisor : public Waiter, public Cashier`, again with virtual
inheritance of `Staff`. Overrides the shared functions and combines taking
orders with billing.

---

## 4. `payroll` layer

### `include/payroll/PayPolicy.h` / `src/payroll/PayPolicy.cpp` - class `PayPolicy` (abstract)
Describes **how** a person is paid. Pure virtual `compute(attendance, base)`,
`clone()`, `typeTag()`, `name()`, `write/read`. The static factory
`PayPolicy::create(tag)` rebuilds the correct policy from a tag in a file.
Because `Staff` holds a `PayPolicy*`, the pay method is chosen at run time
(run-time polymorphism).

### `include/payroll/MonthlySalary.h` / `src/payroll/MonthlySalary.cpp` - class `MonthlySalary`
Base salary + fixed allowance, reduced for unpaid (absent) days.
`compute()` = base + allowance - (base/30 x absent days), never below zero.

### `include/payroll/HourlyWage.h` / `src/payroll/HourlyWage.cpp` - class `HourlyWage`
Rate per hour x hours worked, with overtime paid at **1.5x**.
`compute()` = rate x (presentDays x hoursPerDay) + rate x 1.5 x overtime.

### `include/payroll/CommissionPay.h` / `src/payroll/CommissionPay.cpp` - class `CommissionPay`
Base salary + a percentage of the sales or tips earned.
`compute()` = base + (tips + sales) x commissionRate (rate 1.0 = 100% of tips,
used by waiters).

### `include/payroll/TaxConfig.h` / `src/payroll/TaxConfig.cpp` - class `TaxConfig`
A class with **only static members**: PF percentage, GST percentage and income
tax slabs. **Functionality:** getters/setters (changing one value changes it for
the whole program) and `providentFund`, `incomeTax` (progressive slabs) and
`gst` calculations.

### `include/payroll/Payslip.h` / `src/payroll/Payslip.cpp` - class `Payslip`
One person's salary slip for one month: staff details, gross, bonus, PF, tax and
net pay. Inherits `Printable` and `Serializable`.
**Functionality:** `print()` prints a neat slip; `operator<<` is a friend; it is
saved to file so a payment history is kept; `clone()` supports storage.

### `include/payroll/Payroll.h` / `src/payroll/Payroll.cpp` - class `Payroll`
Performs the payroll run and is a **friend class** of `Staff` and `Attendance`,
so it can read private salary/attendance data.
**Functionality:** `runMonthly(staff, month, bonus)` / `payFor(...)` compute
gross (`calculatePay`), PF and tax (`TaxConfig`), add the bonus and create a
`Payslip`; `totalExpense()` sums the slips in the current run; `payslips()` and
`clear()`.

---

## 5. `menu` layer

### `include/menu/Ingredient.h` / `src/menu/Ingredient.cpp` - class `Ingredient`
One raw material in stock (name, unit, quantity, minimum level).
**Functionality:** `restock(qty)`, `consume(qty)` which **throws
`InsufficientStockException`** if stock is too low, `isLow()`, `print`,
`clone`, `write/read`. `operator<<` for printing.

### `include/menu/Dish.h` / `src/menu/Dish.cpp` - class `Dish`
One menu item: name, price (`Money`), category and a `vector<RecipeLine>` of
required ingredients with quantities (`struct RecipeLine`).
**Functionality:** `addIngredient`, `recipe()`, `isAvailable(inventory)` which
checks whether the kitchen has enough of every ingredient, `print`, `clone`,
`write/read`.

### `include/menu/Inventory.h` / `src/menu/Inventory.cpp` - class `Inventory`
Owns/references a `Repository<Ingredient>`.
**Functionality:** `findIngredient`, `findByName`, `addIngredient`,
`checkDish(dish, times)` (throws if short), `consumeDish(dish, times)` (checks
then reduces stock), `lowStock()` (returns items at or below their minimum) and
`listAll()`. This is called whenever an order is placed.

---

## 6. `orders` layer

### `include/orders/Table.h` / `src/orders/Table.cpp` - class `Table`
Table number, seats and `TableStatus` (`Free`, `Occupied`, `Reserved`), linked to
the waiter who serves it. **Functionality:** getters/setters, `isFree`,
`tableStatusName`, `clone`, `print`, `write/read`.

### `include/orders/OrderItem.h` / `src/orders/OrderItem.cpp` - class `OrderItem`
One line of an order: dish id, dish name, quantity, unit price. Has an explicit
**copy constructor** and assignment operator. **Functionality:**
`lineTotal()` = price x quantity, `write/read`, `operator<<`.

### `include/orders/Order.h` / `src/orders/Order.cpp` - class `Order`
A full order for one table: id, table number, waiter, chef, date, a
`vector<OrderItem>` and an `OrderStatus` (`Placed -> Cooking -> Ready -> Served
-> Paid`).
**Functionality:** overloaded `operator+=` (add an item), `operator[]` (get an
item by position), `operator<<` (print the whole order), `total()`, status and
waiter/chef setters, `clone`, `write/read`.

### `include/orders/Bill.h` / `src/orders/Bill.cpp` - class `Bill`
Made from an order. **Functionality:** `subtotal()` from the items, then adds
GST (from `TaxConfig`), applies a discount percentage, adds a tip and produces
`total()`; `setDiscountPercent`, `setTip`, `markPaid`, `print` (a formatted
receipt), `clone`, `write/read`. The tip is also passed to the waiter's pay data
by `RestaurantSystem`.

---

## 7. `storage` layer - templates and the factory

### `include/storage/Repository.h` - template class `Repository<T>` (header-only)
A general container that owns `T*` in a `vector`. **Functionality:** `add`,
`find(id)` (throws if not found), `findOrNull`, `remove(id)`, `update(id, obj)`,
`filter(predicate)`, `findWhere`, `size`, `at(i)`, `all`, `clear`; a **deep-copy
constructor** and a **destructor that deletes every object**. The same template
is used for `Staff`, `Dish`, `Ingredient`, `Order`, `Table`, `Bill` and
`Payslip`.

### `include/storage/FileStore.h` - template class `FileStore<T>` (header-only)
Saves and loads objects of type `T` in a binary file. Each record is written as
`[id][type tag][size][data]` and an **ID -> file position index** is kept.
**Functionality:** `save` (insert or update), `load(id)` (random access read via
`seekg`), `loadAll`, `remove(id)`, `updateInPlace` (`seekp`), `rebuildIndex`,
`reset` (truncate for a clean rewrite), `exists`, `count`. For polymorphic
`Staff` you pass a tag function and a loader (the `StaffFactory`); for concrete
types you pass a simple `new T()` loader.

### `include/storage/Algorithms.h` - function templates (header-only)
`sortRange(begin, end, cmp)`, `topN(items, n, key)` and `sumBy(items, key)`.
**Functionality:** generic sorting / ranking used for the reports (top dish,
best waiter, totals).

### `include/storage/StaffFactory.h` / `src/storage/StaffFactory.cpp` - class `StaffFactory`
When loading, the program only sees a numeric type tag. `create(tag)` returns
`new Manager`, `new Chef`, `new HeadChef`, ... according to the tag, and `tagOf`
returns a staff member's tag. This is what lets one file hold many staff types.

---

## 8. `ui` layer - the user interface

### `include/ui/Console.h` (header-only)
`readLine`, `readInt`, `readDouble`, `readMoney`, `pressEnter`. These never let
bad keyboard input crash the program - they ask again if the input is not valid.

### `include/ui/Auth.h` / `src/ui/Auth.cpp` - class `Auth`
`login(repository, id, password)` finds the staff member; a missing ID throws
`StaffNotFoundException` and a wrong password throws
`UnauthorizedAccessException`.

### `include/ui/RestaurantSystem.h` / `src/ui/RestaurantSystem.cpp` - class `RestaurantSystem`
The glue of the whole program. Owns all repositories, all `FileStore`s, the
`Inventory` and the `Payroll`.
**Functionality:**
* `run()` - shows the login screen, seeds data if needed, then dispatches to the
  correct role menu based on the logged-in user's `typeTag()`.
* `loadAll()` / `saveAll()` - load/save every data file (and reseed the ID
  counters after loading).
* `seedIfEmpty()` - on first run creates demo staff, ingredients, dishes and
  tables plus default passwords.
* `today()`, and accessors for all repositories, `inventory()` and `payroll()`.

### `include/ui/Menus.h` / `src/ui/Menus.cpp` - classes `ManagerMenu`, `CashierMenu`, `WaiterMenu`, `ChefMenu`, `CleanerMenu`
Each menu shows **only what that role is allowed to do** and calls into
`RestaurantSystem`. The file also contains the private helper functions that
implement the actions (hire/fire staff, add/update/delete dish, add/restock
ingredient, place order, create bill, run payroll, reports, attendance).

---

## 9. Entry point and scripts

### `src/main.cpp`
The **single entry point**. Creates `ui::RestaurantSystem("data")` and calls
`run()` inside a `try/catch`.

### `build.ps1`
Compiles every `.cpp` under `src\` with `g++ -std=c++14 -I include` into
`build\restaurant.exe`.

### `run.ps1`
Runs `build.ps1` and then the produced executable.

---

## 10. Which syllabus topic appears where

| Topic | Where |
|-------|-------|
| Encapsulation / abstraction | private/protected data + getters/setters in all classes; pure virtual methods in `Staff`, `PayPolicy`, interfaces |
| Constructors & overloading | default + parameterised constructors in every class |
| `this` pointer | setters and operators that return `*this` (`Money::operator+=`, `Order::operator+=`) |
| Dynamic allocation + destructor | `Repository<T>` creates and deletes objects; virtual destructor in `Staff` |
| Copy constructor + `=` | `Staff` (deep copy of policy), `Repository<T>`, `OrderItem`, `Order` |
| Static members | `IdGenerator`, `Staff::totalStaff()`, `TaxConfig` |
| Friend function / class | `Payroll` (friend of `Staff` and `Attendance`), `operator<<` for `Payslip` |
| Inheritance / virtual base (diamond) | `Person -> Staff -> roles`; `HeadChef` (Chef+Manager), `ShiftSupervisor` (Waiter+Cashier) |
| Virtual & pure virtual functions | `Staff`, `PayPolicy`, the `interfaces` classes |
| Run-time polymorphism | `Staff*` calling `calculatePay()`; swappable `PayPolicy*` |
| Operator overloading | `Money`, `Date`, `Time`, `Order` (`+=`, `[]`, `<<`) |
| File handling + random access | `FileStore<T>` with the ID index and `seekg`/`seekp` |
| Templates | `Repository<T>`, `FileStore<T>`, `sortRange`/`topN`/`sumBy` |
| Exception handling | the exception hierarchy used in every menu action |
| Namespaces | `restaurant::core`, `restaurant::staff`, `restaurant::payroll`, ... |

---

## 11. Build & run quick reference

```powershell
.\run.ps1        # build + run
.\build.ps1      # build only
```

No virtual environment or external package is required: the project uses only
the C++ standard library and the MinGW g++ compiler.
