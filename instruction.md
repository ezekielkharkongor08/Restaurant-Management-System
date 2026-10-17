# Build and Run Instructions

## Prerequisites
- MinGW `g++` (C++11) installed. No Python, no `.venv`, no third-party libraries are needed — this is plain C++.

## Build (single command)
Open a terminal in the project folder (`C:\Users\Ezekiel\Desktop\OOPs project`) and run:

```bash
g++ -I include main.cpp staff.cpp manager.cpp menu.cpp order.cpp billing.cpp payroll.cpp restaurant.cpp operations.cpp utils.cpp -std=c++11 -Wall -Wextra -o rms.exe
```

This compiles all source files with warnings enabled and produces `rms.exe`. All warnings must be fixed (the project compiles warning-free).

## Run
```bash
.\rms.exe
```

## First Run
On the very first run, `staff.dat` does not exist, so the program automatically creates **one default Manager**:
- Staff ID: `1`
- Name: Default Manager
- Base salary: `50000`

Use Staff ID `1` to log in the first time.

## How to Test Every Feature (scripted terminal session)

### 1. Login
- Run the program, choose `1. Login`, enter Staff ID `1` (default manager).
- Wrong ID → shows "Login failed: invalid Staff ID" (exception handling, no crash).

### 2. Staff CRUD (Manager / Head Chef)
- `1. Staff` → `1. Add Staff` → choose role (1=Manager, 2=Chef, 3=Waiter, 4=Cashier, 5=Cleaner, 6=HeadChef), enter name and base salary.
- `2. View Staff` → see all staff with gross pay and duties.
- `3. Update Staff` → change name and base salary.
- `4. Delete Staff` → soft-delete (marked inactive, data kept in file).
- `5. Promote/Change Role` → change a staff member's role.
- `6. Mark Attendance` → set days present and overtime hours.
- Logout → data is saved to `staff.dat`.

### 3. Restart & Persistence Check
- Run `.\rms.exe` again.
- Login as ID `1`. Choose `2. View Staff` → all previously added staff are still there (loaded from `staff.dat`). Data survives restart.

### 4. Menu Management (Manager / Head Chef)
- `2. Menu` → `1. Add Dish` → enter dish name, price, stock.
- `2. View Menu` → dishes shown sorted by price (uses function template `sortItems`).
- `3. Update Dish` → change price/stock.
- `4. Delete Dish` → soft-delete dish.
- Saved to `menu.dat`.

### 5. Placing an Order (Waiter)
- Login as a Waiter.
- `1. Order` → `1. Place Order` → enter table number, add dish IDs and quantities, enter `0` to finish.
- If quantity exceeds stock → error "Not enough stock!" (exception handling).
- Stock reduces when the order is placed.

### 6. Cooking Queue (Chef / Head Chef)
- Login as a Chef.
- `1. Order` → `2. View Cooking Queue` → see Placed/Cooking/Ready orders.
- `3. Mark Cooking` → order moves Placed → Cooking.
- `4. Mark Ready` → order moves Cooking → Ready; chef's `dishesCooked` increases.

### 7. Serving (Waiter)
- Login as the waiter who owns the order.
- `1. Order` → `5. Mark Served` → order moves Ready → Served.

### 8. Billing (Cashier)
- Login as a Cashier.
- `1. Billing` → `1. Make Bill` → enter Order ID, discount % (0–100), tip amount.
- Bill shows: subtotal, discount, GST (5%), tip, total.
- Enter payment amount. If payment < total → error (exception handling).
- Tip is added to the waiter's pay data (saved to `staff.dat`).
- Order moves Served → Paid.

### 9. Payroll (Manager / Head Chef)
- `5. Payroll` → `1. Run Payroll` → computes gross pay per role, subtracts PF (12%) and tax slab, prints each payslip, shows total gross/net.
- `2. View Payslip History` → see all generated payslips.

### 10. Reports (Manager / Head Chef)
- `6. Reports` → total staff count, estimated total payroll, total revenue from paid orders.

### 11. Bad-Input Tests (exception handling)
- Enter letters when a number is expected → "Invalid input" message, program does not crash.
- Discount > 100 → error.
- Payment < total → error.
- Unknown Staff ID / Dish ID / Order ID → "not found" error.

## Files Created at Runtime
- `staff.dat` — staff records (binary, fixed-size)
- `menu.dat` — menu records (binary, fixed-size)
- `rms.exe` — compiled program

Delete `staff.dat` and `menu.dat` to reset to a fresh first-run state (default manager recreated).
