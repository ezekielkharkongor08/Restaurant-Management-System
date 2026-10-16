# Restaurant Management System (C++ OOP Project)

A console-based **Restaurant Management System** written in pure **C++ (C++14)**.
It manages staff and payroll, the menu and stock, tables, orders and billing,
using only the topics from syllabus Modules 1-4 (classes, constructors,
destructors, copy constructor, static members, friend functions, inheritance,
virtual inheritance / diamond problem, virtual & pure virtual functions,
abstract classes, operator overloading, file handling with random access,
templates, exception handling and namespaces).

## 1. Requirements

* A C++ compiler. This project was built and tested with **MinGW g++ 6.3.0**
  (the compiler in `C:\MinGW\bin`).
* PowerShell (for `build.ps1` / `run.ps1`).

> **No virtual environment and no third-party libraries are needed.** The whole
> program is standard C++ and uses only the standard template library that ships
> with the compiler. There is nothing to install (no `.venv`, no pip packages).

## 2. How to build and run (single entry point)

From this folder in PowerShell:

```powershell
.\run.ps1
```

`run.ps1` calls `build.ps1` (which compiles every `.cpp` under `src\`) and then
launches the program.

To build only:

```powershell
.\build.ps1
```

If you prefer to compile by hand:

```powershell
g++ -std=c++14 -I include (Get-ChildItem src -Recurse -Filter *.cpp).FullName -o build\restaurant.exe
.\build\restaurant.exe
```

The single entry point of the program is `src/main.cpp`, which creates the
`ui::RestaurantSystem` object and calls `run()`.

## 3. First run

The first time the program runs it creates a `data\` folder and **seeds demo
data** (staff, menu, ingredients, tables) so every feature can be tried
immediately. Later runs load the saved data back from disk.

### Default logins (ID / password)

| ID | Role            | Password     |
|----|-----------------|--------------|
| 1  | Manager         | `manager`    |
| 2  | Cashier         | `cashier`    |
| 3  | Waiter          | `waiter`     |
| 4  | Chef            | `chef`       |
| 5  | Cleaner         | `cleaner`    |
| 6  | Head Chef       | `headchef`   |
| 7  | Shift Supervisor| `supervisor` |

## 4. What each role can do

* **Manager** - hire / fire staff, update menu and stock, mark attendance, run
  payroll, view payslips, see reports (daily sales, top dish, best waiter,
  monthly salary expense), view orders and tables.
* **Cashier** - view open orders, create a bill (GST + discount + tip), take
  payment, view all bills and tables.
* **Waiter** - view free tables, take an order, view own orders and tips, mark
  attendance.
* **Chef** - view the cooking queue, move an order to Cooking then Ready.
* **Cleaner** - view/add cleaning tasks, mark attendance.
* **Head Chef / Shift Supervisor** - the diamond-problem roles; they log in with
  the manager / cashier menus because they combine two roles.

## 5. Suggested demo flow

1. Log in as **waiter (3/waiter)** -> `Take an order` -> table `1` -> add dish
   `2` (Chicken Curry) x2, dish `4` (Masala Chai) x2 -> finish.
2. Log in as **chef (4/chef)** -> start cooking order 1 -> mark it ready.
3. Log in as **cashier (2/cashier)** -> create bill for order 1 with a discount
   and a tip.
4. Log in as **manager (1/manager)** -> run payroll -> view reports.
5. Quit and run again to confirm the data was saved.

## 6. Where the data is stored

All data is kept in binary files inside `data\`:

```
data\staff.dat         data\dishes.dat       data\ingredients.dat
data\orders.dat        data\tables.dat       data\bills.dat
data\payslips.dat
```

Each record is `[id][type tag][size][data]` and an ID -> file-position index is
used for random access (`seekg` / `seekp`), so a single record can be updated
without rewriting the whole file. Deleting `data\` resets the program to a fresh
seeded state.

## 7. Documentation

See **`PROJECT_DOCUMENTATION.md`** for a full description of every source file,
every class and the functionality each one provides.
