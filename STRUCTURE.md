# Restaurant Management System — Structure & Inheritance Diagram

This file shows the **class inheritance hierarchy** and the **overall structure** of how the program works, in plain ASCII diagrams.

---

## 1. Inheritance Hierarchy (Who Inherits From Whom)

```
                         ┌─────────────────────┐
                         │      Staff          │
                         │   (Abstract Base)   │
                         │                     │
                         │  Pure virtuals:     │
                         │  calculatePay()     │
                         │  duties()           │
                         │  print()            │
                         │  role()             │
                         │                     │
                         │  Static: nextId     │
                         │  Friend: Payroll    │
                         └─────────┬───────────┘
                                   │ virtual
                                   │ inheritance
        ┌──────────┬──────────────┼──────────────┬──────────────┐
        │          │              │              │              │
        ▼          ▼              ▼              ▼              ▼
 ┌─────────┐ ┌────────┐   ┌────────────┐  ┌───────────┐ ┌──────────┐
 │  Chef   │ │ Waiter │   │   Cashier  │  │  Cleaner  │ │ Manager  │
 │         │ │        │   │            │  │           │ │          │
 │ dishes  │ │ tips   │   │ overtime   │  │           │ │ ALLOWANCE│
 │ Cooked  │ │        │   │   Hours    │  │           │ │ (static) │
 └────┬────┘ └────┬───┘   └─────┬──────┘  └─────┬─────┘ └────┬─────┘
      │           │             │               │            │
      │           │             │               │            │
      └───────────┴─────────────┴───────────────┴────────────┘
                                  │
                                  │ multiple inheritance
                                  ▼
                         ┌─────────────────┐
                         │   HeadChef      │  <-- DIAMOND PROBLEM
                         │                 │     (virtual base class)
                         │ inherits:       │     Chef & Manager both
                         │   Chef + Manager│     inherit virtually from Staff,
                         │                 │     so HeadChef has ONE shared
                         │ Pay = base +    │     Staff subobject (no duplicate
                         │   allowance +   │     baseSalary).
                         │   dishes*rate   │
                         └─────────────────┘
```

### Inheritance Types Used

| Type | Example |
|------|---------|
| **Single inheritance** | `Waiter : Staff`, `Cashier : Staff`, `Cleaner : Staff` |
| **Multilevel inheritance** | `Staff → Chef → HeadChef` (and `Staff → Manager → HeadChef`) |
| **Multiple inheritance** | `HeadChef : public Chef, public Manager` |
| **Virtual inheritance (diamond)** | `Chef : virtual public Staff`, `Manager : virtual public Staff` |

---

## 2. Class Responsibilities Overview

```
┌─────────────┐  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐
│    Staff    │  │   Chef      │  │   Waiter    │  │   Cashier   │
│ (abstract)  │  │             │  │             │  │             │
│ - id        │  │ - dishes    │  │ - tips      │  │ - overtime  │
│ - name      │  │   Cooked    │  │             │  │   Hours     │
│ - baseSalary│  │ Pay = base  │  │ Pay = base  │  │ Pay = base  │
│ - daysPres. │  │   + rate*d  │  │   + tips    │  │   + ot*rate │
│ - otHours   │  │             │  │             │  │             │
└──────┬──────┘  └──────┬──────┘  └──────┬──────┘  └──────┬──────┘
       │                │                │                │
       └────────────────┴────────────────┴────────────────┘
                                  │
                                  ▼
                         ┌─────────────────┐
                         │    Manager      │
                         │ Pay = base +    │
                         │   ALLOWANCE     │
                         └────────┬────────┘
                                  │
                                  ▼
                         ┌─────────────────┐
                         │   HeadChef      │
                         │ (Diamond:       │
                         │  Chef + Manager)│
                         └─────────────────┘
```

---

## 3. Supporting Classes

```
┌─────────────┐      ┌─────────────┐      ┌─────────────┐
│    Dish     │      │    Order    │      │     Bill    │
│             │      │             │      │             │
│ - id        │      │ - id        │      │ - orderId   │
│ - name      │      │ - tableNo   │      │ - waiterId  │
│ - price     │      │ - waiterId  │      │ - subtotal  │
│ - stock     │      │ - items[]   │      │ - discount% │
│ - active    │      │ - state     │      │ - gst%      │
│             │      │   Placed,   │      │ - tip       │
│ operator<<  │      │   Cooking,  │      │ - total     │
│ operator<   │      │   Ready,    │      └──────┬──────┘
│ reduceStock()│     │   Served,   │             │
│ static nextId│     │   Paid      │             │
└──────┬──────┘      │ operator+=  │             │
       │             │ (add item)  │             │
       └─────────────┴─────────────┴─────────────┘
                                             │
                                             ▼
                                    ┌───────────────┐
                                    │   Payslip     │
                                    │               │
                                    │ - staffId     │
                                    │ - staffName   │
                                    │ - gross, pf   │
                                    │ - tax, net    │
                                    └───────┬───────┘
                                            │
                                            ▼
                                    ┌───────────────┐
                                    │   Payroll     │
                                    │               │
                                    │ PF_RATE (static)│
                                    │ TAX slabs     │
                                    │ runPayroll()  │
                                    │ computePayslip│
                                    │ history[]     │
                                    │ Friend of Staff│
                                    └───────────────┘
```

---

## 4. Template Classes

```
┌─────────────────────┐      ┌─────────────────────┐
│     Repo<T>         │      │    FileStore<T>     │
│ (generic container) │      │ (generic binary file│
│                     │      │  storage w/ random  │
│ - items[]           │      │  access)            │
│ - add(item)         │      │                     │
│ - operator[]        │      │ - append(record)    │
│ - size()            │      │ - readAt(index)     │
│ - removeAt()        │      │ - updateAt(index)   │
│ - deep copy ctor    │      │ - count()           │
│   + operator=       │      │ - readAll()         │
└─────────┬───────────┘      └──────────┬──────────┘
          │                             │
          ▼                             ▼
    Used for:                  Used for:
    - menu (Repo<Dish>)        - staff.dat
    - payslip history          - menu.dat
      (Repo<Payslip>)          seekg/seekp on fixed-size records
```

---

## 5. Utility & Exception

```
┌─────────────────┐      ┌─────────────────┐
│    utils.h      │      │  exceptions.h   │
│                 │      │                 │
│ readInt()       │      │ RestaurantException│
│ readDouble()    │      │ (custom, 1 class)│
│ trim()          │      │ thrown on bad   │
│ copyToCharArray()│     │ input so program│
│ sortItems<T>()  │      │ never crashes   │
│ arraySum<T>()   │      └─────────────────┘
└─────────────────┘
```

---

## 6. Core System Class

```
┌─────────────────────────────────────┐
│       Restaurant                    │
│                                     │
│ - staffList   : vector<Staff*>      │  ← run-time polymorphism
│ - menuRepo    : Repo<Dish>          │  ← class template
│ - orders      : vector<Order>       │
│ - payHistory  : Repo<Payslip>       │  ← class template
│ - staffStore  : FileStore<StaffRec> │  ← class template, binary
│ - menuStore   : FileStore<DishRec>  │  ← class template, binary
│                                     │
│ Methods:                            │
│ login(), addStaff(), viewStaff(),   │
│ updateStaff(), deleteStaff(),       │
│ promoteStaff(), markAttendance(),   │
│ addDish(), viewMenu(), updateDish(),│
│ placeOrder(), markCooking(),        │
│ markReady(), markServed(),          │
│ makeBill(), runPayroll(),           │
│ showReports(), load/save files      │
└─────────────────────────────────────┘
```

---

## 7. Program Flow (How It Works at Runtime)

```
START
  │
  ▼
┌─────────────┐
│ Restaurant  │  ← constructor
│   loads     │     loadAllStaff()  → reads staff.dat
│   data      │     loadAllMenu()   → reads menu.dat
│   (first    │     if staff empty → seedDefaultManager()
│      run)   │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  Login      │  ← enter Staff ID
│  screen     │     valid ID? ──No──┐
│             │                      ▼
│             │              "Login failed"
│             │                      │
│             │     valid ID? ──Yes──┘
│             │                      │
│             ▼                      │
┌─────────────────────┐              │
│  Role-Based Menu    │◄─────────────┘
│  (Manager/Chef/     │
│   Waiter/Cashier/   │
│   Cleaner/HeadChef) │
│                     │
│   ┌─────┬─────┬─────┼─────┬─────┐
│   │Staff│Menu │Order│Bill │Payroll│
│   │     │     │     │     │       │
│   ▼     ▼     ▼     ▼     ▼       ▼
│ CRUD  CRUD  Place→  Make  Run     Reports
│       Menu  Cook→   Bill  Payroll │
│           Ready→         Tip→     │
│           Served   Waiter          │
│                                    ▼
└─────────────────────┬──────────────┘
                      │
                      ▼
             ┌─────────────────┐
             │  Save to files  │  ← saveAllStaff(), saveAllMenu()
             │  (binary,       │     on every change
             │   random access)│
             └─────────────────┘
                      │
                      ▼
                   END (or Logout → back to Login)
```

---

## 8. Order Lifecycle State Machine

```
PLACE ORDER (Waiter)
      │
      ▼
┌───────────┐
│  Placed   │───Chef marks Cooking───►
└───────────┘                          │
                                       ▼
                                 ┌───────────┐
                                 │  Cooking  │───Chef marks Ready───►
                                 └───────────┘                          │
                                                                         ▼
                                                                   ┌───────────┐
                                                                   │  Ready    │───Waiter marks Served───►
                                                                   └───────────┘                          │
                                                                                                         ▼
                                                                                                   ┌───────────┐
                                                                                                   │  Served   │───Cashier makes Bill───►
                                                                                                   └───────────┘                          │
                                                                                                                                             ▼
                                                                                                                                    ┌───────────┐
                                                                                                                                    │  Paid     │
                                                                                                                                    └───────────┘
                                                                                                                                      │
                                                                                                                                      ▼
                                                                                                                               Order closed, tip added to waiter
```

---

## 9. Payroll Calculation Flow (per role)

```
Staff* → calculatePay()  [virtual dispatch / run-time polymorphism]
         │
         ├─ Manager     → baseSalary + ALLOWANCE (5000)
         ├─ Chef        → baseSalary + (dishesCooked * 40)
         ├─ Waiter      → baseSalary + tips
         ├─ Cashier     → baseSalary + (overtimeHours * 30)
         ├─ Cleaner     → baseSalary * daysPresent / 26
         └─ HeadChef    → baseSalary + ALLOWANCE + (dishesCooked * 40)
                               (diamond: base counted once via virtual Staff)

         │
         ▼
   Gross Pay
         │
         ├─ PF (12% static)  ───────────► subtract
         ├─ Tax slab:
         │     gross <= 25000  → 0%
         │     25000 < g <= 50000 → 5%
         │     gross > 50000   → 10%
         └─ Net Pay = Gross - PF - Tax
                    │
                    ▼
           Payslip printed + stored in history
```

---

## 10. Data Persistence (Binary Files)

```
┌─────────────┐                    ┌─────────────┐
│  staff.dat  │                    │  menu.dat   │
│             │                    │             │
│ Fixed-size  │                    │ Fixed-size  │
│ records:    │                    │ records:    │
│             │                    │             │
│ StaffRecord │                    │ DishRecord  │
│ - id (int)  │                    │ - id (int)  │
│ - name[32]  │                    │ - name[32]  │
│ - role (int)│                    │ - price (dbl)│
│ - baseSalary│                    │ - stock (int)│
│ - daysPresent│                   │ - active (bool)│
│ - otHours   │                    │             │
│ - tips      │                    │             │
│ - dishesCooked│                  │             │
│ - active (bool)│                │             │
└──────┬──────┘                    └──────┬──────┘
       │                                 │
       │ random access                   │ random access
       │ seekg/seekp                     │ seekg/seekp
       │ record N at byte N*sizeof()     │ record N at byte N*sizeof()
       ▼                                 ▼
   FileStore<StaffRecord>          FileStore<DishRecord>
   - append/updateAt/readAt/       - append/updateAt/readAt/
     count/readAll                 count/readAll
       │                                 │
       └───────────────┬─────────────────┘
                       │
                       ▼
              loadAll() at startup
              saveAll() on every change
              Delete = soft (active = false)
```

---

## 11. File-to-Class Map

| File | Classes / Contents |
|------|-------------------|
| `main.cpp` | `main()` entry point, try/catch wrapper |
| `include/staff.h/.cpp` | `Staff` (abstract), `Chef`, `Waiter`, `Cashier`, `Cleaner` |
| `include/manager.h/.cpp` | `Manager`, `HeadChef` (diamond) |
| `include/menu.h/.cpp` | `Dish` |
| `include/order.h/.cpp` | `OrderItem`, `Order` |
| `include/billing.h/.cpp` | `Bill` |
| `include/payroll.h/.cpp` | `Payslip`, `Payroll` |
| `include/restaurant.h/.cpp` | `Restaurant` (core system) |
| `operations.cpp` | Terminal menus (`runApp`, `staffMenu`, `menuMenu`, `orderMenu`, `billingMenu`, `payrollMenu`) |
| `include/templates.h` | `Repo<T>`, `FileStore<T>`, `sortItems<T>`, `arraySum<T>` |
| `include/records.h` | `StaffRecord`, `DishRecord` (fixed-size binary) |
| `include/exceptions.h` | `RestaurantException` |
| `include/utils.h/.cpp` | `readInt`, `readDouble`, `trim`, `copyToCharArray` |

---

## 12. Access Control (Login → Role → Menu)

| Staff ID Role | Menu Options |
|---------------|--------------|
| **1 = Manager** | Staff, Menu, Order, Billing, Payroll, Reports, My Profile, Logout |
| **2 = Chef** | Order (cooking queue), My Profile, Logout |
| **3 = Waiter** | Order (place/serve), Billing View, My Profile, Logout |
| **4 = Cashier** | Billing, My Profile, Logout |
| **5 = Cleaner** | My Profile, Logout |
| **6 = HeadChef** | Everything Manager has + Chef cooking queue |

---

**End of STRUCTURE.md**
