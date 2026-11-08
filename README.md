# Restaurant Management System (C++)

A console-based Restaurant Management System written in C++ (C++11). This project simulates the core operations of a restaurant, including staff management, role-based access, menu and inventory tracking, order processing, billing, and monthly payroll calculation with persistent binary storage.

## Features

- **Role-Based Login**: Access is granted based on staff role (Manager, HeadChef, Chef, Waiter).
- **Staff Management**: Add, view, update, and remove staff members.
- **Menu & Stock Tracking**: Manage dishes, update prices, and automatically reduce stock when orders are placed.
- **Order Processing & Billing**: Waiters can place table orders, track their status (Placed -> Served -> Paid), apply discounts, and generate final bills.
- **Payroll System**: Computes gross salary, 12% provident fund (PF) deduction, and net pay for all active staff, maintaining a history of generated payslips.
- **Binary Data Persistence**: Uses binary files with random access (`seekp`/`seekg`) so that staff, menu items, orders, and payslips persist across restarts.

## Object-Oriented Programming (OOP) Concepts Used

- **Encapsulation & Abstraction**: Class design hiding data representation behind public interfaces.
- **Inheritance**:
  - Hierarchical inheritance: `Staff` base class extended by `Chef`, `Waiter`, and `Manager`.
  - Multiple and virtual inheritance: `HeadChef` inherits from both `Chef` and `Manager` virtually, resolving the Diamond Problem.
- **Runtime Polymorphism**: Abstract base class (`Staff`) with pure virtual functions (`calculateSalary()`, `display()`) called through base-class pointers (`Staff*`).
- **Templates**:
  - Class templates: `Repo<T>` (dynamic array container implementing the Rule of Three) and `FileStore<T>` (generic binary file handler).
  - Function template: `sortItems<T>` to sort items using overloaded comparison operators.
- **Operator Overloading**:
  - `+=` to add dish items into an order.
  - `[]` and `=` in `Repo<T>`.
  - `<<` stream insertion operator for formatted output of dishes, orders, bills, and payslips.
- **Exception Handling**: Custom exception class (`RestaurantException`) and standard exception handling for input validation and business logic constraints.

## Project Structure

```
├── include/
│   ├── exceptions.h      # Custom exception class
│   ├── templates.h       # Generic Repo<T> and FileStore<T> templates
│   ├── staff.h           # Staff class hierarchy (Manager, Chef, Waiter, HeadChef)
│   ├── menu.h            # Dish class
│   ├── order.h           # Order and Bill classes
│   ├── payroll.h         # Payslip and Payroll classes
│   ├── restaurant.h      # Central controller class
│   └── menus.h           # Console menus and input helper functions
├── main.cpp              # Application entry point and role router
├── menus.cpp             # Sub-menu displays and input validation
├── restaurant.cpp        # Core restaurant logic and file operations
├── staff.cpp             # Staff hierarchy implementation
├── menu.cpp              # Dish implementation
├── order.cpp             # Order and billing implementation
├── payroll.cpp           # Payroll calculations and payslip history
├── data/                 # Directory where binary data files (.dat) are saved
├── instruction.md        # Quick compile and run steps
└── README.md             # Project documentation
```

## How to Build and Run

### Requirements
- GCC / G++ supporting C++11 or later.

### Compilation
From the project root folder:

```bash
g++ -I include main.cpp menus.cpp restaurant.cpp staff.cpp menu.cpp order.cpp payroll.cpp -std=c++11 -Wall -Wextra -o rms.exe
```

### Execution
```bash
./rms.exe
```

### Default Login
On the first run with an empty `data/` folder, a default Manager account is created automatically:
- **Staff ID**: `1`

Log in using ID `1` to add staff members and menu items.
