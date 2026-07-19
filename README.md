# Pharmacy Management System

A command-line (CMD-based) Pharmacy Management System built in **C++** using the **MySQL Connector/C** library. No GUI, no browser, no React — pure console application connecting directly to a MySQL database.

---

## 📋 Features

- **User Authentication** — Login system with session tracking (knows who is logged in for every action)
- **Manage Users** — Full CRUD (Add, View, Search, Update, Delete)
- **Manage Medicines** — Full CRUD with supplier linkage
- **Manage Suppliers** — Full CRUD
- **Manage Customers** — Full CRUD
- **Sell Medicine** — Multi-item cart-based sales system with automatic stock reduction, transaction safety (rollback on error), and sale deletion with stock restoration
- **Reports** — Daily sales, all-time sales, low stock alerts, expiring-soon medicines, top-selling medicines, sales by specific date
- **System Overview Dashboard** — One-screen summary of total users, customers, suppliers, medicines, stock value, revenue, profit, and alerts
- **Smart ID Selection** — Every place that requires an ID (customer, medicine, supplier, user) first displays a list of available records with names, so you select instead of guessing

---

## 🗄️ Database Schema

Database name: `pharmacy_db`

| Table | Columns |
|---|---|
| `users` | user_id, full_name, username, password, phone, role, created_at |
| `customers` | customer_id, full_name, phone, email, address, created_at |
| `suppliers` | supplier_id, company_name, contact_person, phone, email, address, created_at |
| `medicines` | medicine_id, medicine_name, category, supplier_id, buy_price, sell_price, quantity, expiry_date, created_at |
| `sales` | sale_id, customer_id, user_id, sale_date, grand_total |
| `sale_items` | sale_item_id, sale_id, medicine_id, quantity, price, subtotal |

**Relationships:**
- `medicines.supplier_id` → `suppliers.supplier_id` (a medicine must belong to an existing supplier)
- `sales.customer_id` → `customers.customer_id` (nullable — supports walk-in customers)
- `sales.user_id` → `users.user_id` (which staff member made the sale)
- `sale_items.sale_id` → `sales.sale_id`
- `sale_items.medicine_id` → `medicines.medicine_id`

> ⚠️ Because of the foreign key on `medicines.supplier_id`, you must add at least **one supplier** before you can add any medicine.

---

## 📁 Project Structure

```
PharmacySystem/
│
├── main.cpp                # Entry point, main menu, dashboard menu
├── database.h / database.cpp   # DB connection handling (connectDB, disconnectDB, conn)
├── login.h / login.cpp     # Authentication + currentUserId session tracking
│
├── users.h / users.cpp             # Manage Users (CRUD)
├── medicine.h / medicine.cpp       # Manage Medicines (CRUD)
├── supplier.h / supplier.cpp       # Manage Suppliers (CRUD)
├── customer.h / customer.cpp       # Manage Customers (CRUD)
├── sale.h / sale.cpp               # Sell Medicine (multi-item sales + stock control)
├── report.h / report.cpp           # Reports (sales, stock, expiry)
└── dashboard_report.h / dashboard_report.cpp   # System Overview dashboard
```

---

## ⚙️ Requirements

- Windows OS
- **MinGW g++ compiler**
- **MySQL Server** running locally, with the `pharmacy_db` database created and the tables above set up
- **MySQL Connector/C 6.1** installed (used for `mysql.h`, `libmysql.lib`, etc.)

---

## 🛠️ Build Instructions

Compile all source files together from the `PharmacySystem` folder:

```bash
g++ main.cpp database.cpp login.cpp medicine.cpp supplier.cpp customer.cpp sale.cpp users.cpp report.cpp dashboard_report.cpp -o pharmacy ^
-I"C:\Program Files\MySQL\MySQL Connector C 6.1\include" ^
-L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib" ^
-lmysql
```

Then run it:

```bash
pharmacy.exe
```

---

## 🚀 Usage Flow

1. **Login** with a valid username/password from the `users` table.
2. From the dashboard, choose a module:
   - **Manage Users** — add staff accounts, assign roles
   - **Manage Suppliers** — add suppliers *(do this before adding medicines)*
   - **Manage Medicines** — add stock, linked to a supplier
   - **Manage Customers** — register customers *(optional — sales support walk-ins)*
   - **Sell Medicine** — build a cart of medicines, confirm, and complete a sale
   - **Reports** — view sales/stock/expiry insights
   - **System Overview** — see a full snapshot of the business at a glance

3. **Logout** or **Exit** to close the database connection safely.

### Example: Making a Sale
1. Go to **Sell Medicine → New Sale**
2. System shows the list of customers — pick an ID, or `0` for walk-in
3. System shows the list of medicines with stock and price — pick an ID and quantity
4. Repeat for more items, or finish
5. Confirm the sale — stock is automatically reduced and the sale is recorded

If anything fails mid-sale (e.g., a database error), the entire transaction is **rolled back** — no partial sales or incorrect stock changes.

---

## 📊 Reports Available

| Report | Description |
|---|---|
| Today's Sales | Number of sales and revenue made today, with per-sale details |
| All-Time Sales | Total sales count and total revenue since the system started |
| Low Stock Report | Medicines below a stock threshold you choose |
| Expiring Soon Report | Medicines expiring within a number of days you choose |
| Top Selling Medicines | Best-selling medicines ranked by units sold |
| Sales By Specific Date | All sales on a chosen date, with a day total |

## 📈 System Overview Dashboard

Displays at a glance:
- Total Users, Customers, Suppliers
- Total Medicine Types & Total Stock Units
- Total Stock Cost Value (what was paid to suppliers for current stock)
- Potential Stock Value (what current stock would earn if fully sold)
- Total Sales Made & Total Revenue Earned
- Total Units Sold (all-time)
- Estimated Total Profit (revenue minus cost of goods sold)
- Today's Sales & Revenue
- Low Stock Alerts (< 10 units by default)
- Expiring Soon Alerts (within 30 days by default)

---

## 🔒 Notes on Design Decisions

- **Sales are not editable** — once a sale is made, it can only be **deleted** (which restores stock automatically). This avoids desynced totals that partial edits could cause.
- **Smart ID lookups** — anywhere the system needs an ID (customer, medicine, supplier, user), it prints a short list of existing records first so you never have to guess an ID.
- **Transactions** — Sell Medicine and Delete Sale use SQL transactions (`START TRANSACTION` / `COMMIT` / `ROLLBACK`) to guarantee the database never ends up in a half-updated state.
- **Walk-in customers** — sales can be recorded without a registered customer by entering `0`.

---

## 🔮 Possible Future Improvements

- Password hashing (currently stored in plain text — fine for a school/learning project, not for production)
- Role-based access control (e.g., only admins can manage users)
- Export reports to CSV or PDF
- Purchase orders / restocking workflow tied to suppliers
- Pagination for large tables in View/List outputs

---

## 👤 Author

Pharmacy Management System — C++ / MySQL console application.
