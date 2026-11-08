# Build and Run

## Requirements

- `g++` with C++11 support
- Run the commands from the project root (the folder containing `main.cpp`)

## Build

```bash
g++ -I include main.cpp menus.cpp restaurant.cpp staff.cpp menu.cpp order.cpp payroll.cpp -std=c++11 -Wall -Wextra -o rms.exe
```

The build should finish with no warnings.

## Run

```bash
./rms.exe
```

Keep the `data/` folder next to `rms.exe`. The program reads and writes:

```
data/staff.dat
data/menu.dat
data/orders.dat
data/payslips.dat
```

The `.dat` files are created automatically on first use. If `data/` is
missing, the program reports that it cannot open the data file.

## First login

On a fresh `data/` folder, one default Manager is created:

```
Name: Default Manager
Staff ID: 1
```

Log in as ID `1`, then add a Chef, Waiter and HeadChef and some dishes.

## Quick test

1. Login as `1`.
2. Manage Staff -> add a Chef (role 2) and a Waiter (role 3).
3. Manage Menu -> add a dish with stock.
4. Logout, login as the Waiter's ID.
5. Place an order, view orders, mark it served, then make a bill
   (discount 10, payment larger than the total).
6. Logout, login as the Manager, run payroll and view payslip history.
7. Exit and restart; staff, menu, orders and payslips should still be there.

## Clean data

To start from scratch, delete the `.dat` files inside `data/` (not the folder
itself). The next run will create a new default Manager.
