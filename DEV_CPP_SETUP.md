# Motherland Pharmacy: Dev-C++ setup

## Confirmed project dependencies

This is a C11 console program. It uses the MySQL Connector/C 6.1 C API directly:

- Header: `C:\Program Files\MySQL\MySQL Connector C 6.1\include\mysql.h`
- Import library: `C:\Program Files\MySQL\MySQL Connector C 6.1\lib\libmysql.lib`
- Runtime DLL: `libmysql.dll` in this project folder
- Database server: XAMPP MySQL/MariaDB at `127.0.0.1:3306`
- Database name: `pharmacy_db`
- Current database login in `database.c`: user `root`, blank password

The connector import library and the included `libmysql.dll` are **64-bit**. Use a 64-bit Dev-C++ compiler set. A 32-bit compiler cannot link them.

## Source files in the Dev-C++ project

`main.c`, `database.c`, `login.c`, `medicine.c`, `supplier.c`, `customer.c`, `sale.c`, `users.c`, `report.c`, `dashboard_report.c`, and `ui.c`.

Headers are `database.h`, `login.h`, `medicine.h`, `supplier.h`, `customer.h`, `sale.h`, `users.h`, `report.h`, `dashboard_report.h`, and `ui.h`.

## Open the supplied project

1. Install/open a **64-bit** Dev-C++ version with a MinGW-w64 compiler set.
2. In Dev-C++, select **File > Open Project or File**.
3. Open `motherland_pharmacy.dev` from this folder.
4. Check **Tools > Compiler Options > Settings** and select the 64-bit compiler set.
5. Choose **Execute > Compile & Run** (or press `F11`).

The project uses these exact compiler/linker settings:

```text
Compiler: -std=c11 -Wall -Wextra
Include directory: C:\Program Files\MySQL\MySQL Connector C 6.1\include
Library directory: C:\Program Files\MySQL\MySQL Connector C 6.1\lib
Library: mysql
```

If your Dev-C++ release does not load the `.dev` settings, create a Console Application project and add the eleven source files above. Then set the same values under **Project > Project Options**:

- Parameters > C Compiler: `-std=c11 -Wall -Wextra`
- Directories > Include Directories: `C:\Program Files\MySQL\MySQL Connector C 6.1\include`
- Directories > Library Directories: `C:\Program Files\MySQL\MySQL Connector C 6.1\lib`
- Parameters > Linker: `-lmysql`

Do not add the header files as build units. They are included by the `.c` files.

## Runtime requirements

Keep `libmysql.dll` beside the executable. This project already has it in its root folder; set the executable output directory to the project directory or copy the DLL next to your chosen output `.exe`.

`libmysql.dll` also requires the Microsoft Visual C++ runtime (`VCRUNTIME140.dll`). Install the current Microsoft Visual C++ 2015-2022 Redistributable **x64** if Windows reports that DLL as missing.

## Import and start the database

1. Start **MySQL** in the XAMPP Control Panel.
2. Import `pharmacy_db.sql` into phpMyAdmin, or from a terminal:

```powershell
C:\xampp\mysql\bin\mysql.exe -u root < pharmacy_db.sql
```

3. If your MySQL root user has a password, edit the third connection argument in `database.c` (the current value is `""`) to that password, then rebuild.
4. Run `pharmacy.exe` from Dev-C++. The included demo login is `admin` / `admin123`.

## Expected build command

The equivalent working command was verified in this folder:

```powershell
gcc -std=c11 -Wall -Wextra main.c database.c login.c medicine.c supplier.c customer.c sale.c users.c report.c dashboard_report.c ui.c -o pharmacy.exe -I"C:\Program Files\MySQL\MySQL Connector C 6.1\include" -L"C:\Program Files\MySQL\MySQL Connector C 6.1\lib" -lmysql
```

## Troubleshooting

- `mysql.h: No such file or directory`: add the include directory shown above.
- `cannot find -lmysql`: add the library directory and make sure `libmysql.lib` is present.
- `file format not recognized` or `i386:x86-64`: Dev-C++ is using a 32-bit compiler. Switch/install a 64-bit MinGW-w64 compiler set.
- `libmysql.dll was not found`: copy the project’s `libmysql.dll` beside `pharmacy.exe`.
- `VCRUNTIME140.dll was not found`: install the x64 Visual C++ 2015-2022 Redistributable.
- `Database Connection Failed`: start XAMPP MySQL and check host, username, password, port, and database name in `database.c`.
