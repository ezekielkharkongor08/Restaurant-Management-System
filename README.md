# Restaurant Management System (C++ OOP Course Project)

A console (terminal) Restaurant Management System built in pure C++11. The main focus is **restaurant staff and payroll** — who works in the restaurant, what each person does, and how each person is paid. Around that is a simple menu, ordering, and billing system.

## Project Purpose
Entry-level intermediate 3rd-semester B.Tech OOP project. Every line is written so a beginner can explain it in a viva. Built with only syllabus-allowed topics — no smart pointers, lambdas, `std::map`, threads, or external libraries.

## How to Build and Run
See `instruction.md` for the single compile-and-run command and step-by-step testing instructions.

## File Layout

| File | Contains |
|------|----------|
| `main.cpp` | Entry point. Creates `Restaurant`, runs the app, wraps everything in `try/catch` so bad input never crashes the program. |
| `staff.h` / `staff.cpp` | Abstract `Staff` base class + concrete roles: `Chef`, `Waiter`, `Cashier`, `Cleaner`. Pure virtual pay/duties/print/role. Copy constructor, `this` pointer, `operator=`, virtual destructor, `friend class Payroll`, conversion to/from binary record. |
| `manager.h` / `manager.cpp` | `Manager` and `HeadChef`. `Manager` = base + fixed allowance. `HeadChef` = **diamond problem** (multiple + virtual inheritance: both Chef and Manager, sharing one `Staff`). |
| `menu.h` / `menu.cpp` | `Dish` class (name, price, stock). `operator<<` (friend), `operator<` (for sorting), `reduceStock()`, static auto-increasing dish ID, copy constructor/assignment, binary record conversion. |
| `order.h` / `order.cpp` | `OrderItem` and `Order`. Order states Placed → Cooking → Ready → Served → Paid. `operator+=` to add an item. Static auto-increasing order ID. |
| `billing.h` / `billing.cpp` | `Bill` — subtotal, discount %, GST (5%), tip, total. `operator<<` prints the receipt. Tip is added to the waiter's pay data. |
| `payroll.h` / `payroll.cpp` | `Payslip` and `Payroll`. Payroll is a **friend of Staff**. PF (12%), tax slabs, gross → net calculation, payslip history. Run-time polymorphism via `Staff*` pointers. |
| `restaurant.h` / `restaurant.cpp` | `Restaurant` — the system. Holds staff roster (`vector<Staff*>`), menu (`Repo<Dish>`), orders, payslip history (`Repo<Payslip>`), and two `FileStore`s for binary files. Login, staff CRUD, menu CRUD, order flow, billing, payroll, reports, load/save, first-run default manager seed. |
| `operations.cpp` | Terminal menus and input/output flow for each role (Manager, Chef, Waiter, Cashier, Cleaner). Calls public `Restaurant` methods. |
| `templates.h` | Class templates `Repo<T>` (generic container with deep copy) and `FileStore<T>` (generic binary file storage with random access via `seekg`/`seekp`). Function templates `sortItems<T>` and `arraySum<T>`. |
| `records.h` | Fixed-size binary records `StaffRecord` and `DishRecord` (no `std::string` inside — uses `char[32]`, so `sizeof` is constant, enabling random access). |
| `exceptions.h` | `RestaurantException` — the one custom exception class. |
| `utils.h` / `utils.cpp` | Input helpers `readInt`, `readDouble`, `trim`, `copyToCharArray`. |
| `instruction.md` | Build and run instructions. |

## Class Responsibilities

- **Staff** (abstract): base for all staff. Fields `id`, `name`, `baseSalary`, `daysPresent`, `overtimeHours`. Pure virtuals `calculatePay()`, `duties()`, `print()`, `role()`. Static auto-increasing `nextId`. Virtual destructor. `friend class Payroll`.
- **Chef**: pay = base + (rate per dish × dishes cooked). Cooks orders, updates cooking queue.
- **Waiter**: pay = base + tips. Takes orders, serves food, receives tips.
- **Cashier**: pay = base + (overtime hours × overtime rate). Creates bills, takes payment.
- **Cleaner**: pay = base × days present / 26. Cleans tables/kitchen.
- **Manager**: pay = base + fixed allowance. Hires/fires/promotes staff, payroll, menu, reports.
- **HeadChef**: diamond class — both Chef and Manager. Pay = base + allowance + per-dish rate (base counted once via virtual base). This is the real multiple-inheritance + virtual-base-class case.
- **Dish**: menu item with name/price/stock.
- **Order**: table-based order moving through states Placed/Cooking/Ready/Served/Paid.
- **Bill**: receipt with GST, discount, tip.
- **Payslip**: gross, PF, tax, net for one staff member.
- **Payroll**: computes and prints payslips for all staff using base-class pointers (run-time polymorphism).
- **Repo<T>**: generic container (class template) used for menu and payslip history.
- **FileStore<T>**: generic binary file storage (class template) with append/readAt/updateAt/count/readAll using `seek` operations.
- **RestaurantException**: custom exception thrown on bad input / business errors.

## Mandatory Syllabus Features Covered

| Feature | Where |
|---------|-------|
| Classes & objects, constructors/destructors | Every class |
| Copy constructor + `this` pointer | `Staff`, `Dish`, `Repo<T>` assignment |
| Static members (auto-increasing ID) | `Staff::nextId`, `Dish::nextDishId`, `Order::nextOrderId`, `Payroll::PF_RATE`, `Manager::ALLOWANCE` |
| Friend function/class | `operator<<` friends, `Payroll` is friend of `Staff` |
| Function overloading | Constructors (default + parameterized), `readInt`/`readDouble` |
| Operator overloading | `<<` (printing), `+=` (add item to order), `<` (dish sort), `[]` and `=` (Repo) |
| Inheritance: single, multilevel, multiple | Staff → roles (single/multilevel); HeadChef : Chef, Manager (multiple) |
| Virtual base class + diamond problem | `Chef`/`Manager` : `virtual public Staff`; `HeadChef` shares one `Staff` |
| Virtual & pure virtual functions | `Staff` pure virtuals; `calculatePay()` overridden per role |
| Run-time polymorphism | `vector<Staff*>` roster; payroll calls virtual `calculatePay()` via base pointers |
| File handling + random access | `FileStore<T>` with `seekg`/`seekp`, fixed-size binary records |
| Class templates | `Repo<T>`, `FileStore<T>` |
| Function templates | `sortItems<T>`, `arraySum<T>` |
| Exception handling | `RestaurantException` — program never crashes on bad input |
| Namespace | `namespace restaurant` (one flat namespace) |

## Data Storage
- Staff → `staff.dat`, Menu → `menu.dat`
- Fixed-size records (binary), no `std::string` inside records
- Random access via `seekg`/`seekp` — record N at byte `N × sizeof(Record)`
- Delete = soft delete (`active` flag); update = `updateAt` in place
- First run creates one default Manager so someone can log in
- Data survives program restarts

## Access Control (role → menu after login by Staff ID)
- **Manager / Head Chef**: staff, menu, order, billing, payroll, reports
- **Chef / Head Chef**: cooking queue, mark cooking/ready
- **Waiter**: place order, view queue, my profile
- **Cashier**: billing, my profile
- **Cleaner**: my profile

## Pay Rules
- Manager: base + allowance
- Chef: base + rate per dish
- Waiter: base + tips
- Cashier: base + overtime pay
- Cleaner: base × days present / 26
- Head Chef: base + allowance + per-dish rate
