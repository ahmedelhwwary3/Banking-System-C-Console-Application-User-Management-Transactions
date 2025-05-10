# 🏦 Bank Management System (C++) - Console App

A clean, modular, and educational C++ banking system designed with a focus on **clarity**, **structure**, and **user experience**. The application simulates real-world banking operations including user and client management, permissions, and financial transactions.

---

## 🎯 Project Highlights

- ✅ Clean & modular code using **Divide and Conquer** approach  
- 🧩 Each function performs **only one task** (Single Responsibility Principle)
- 🔒 Permissions system using **Bitwise Byte Flags** for screen access control
- 🔍 Robust **case-sensitive validation**
- 🗃️ **Text file storage** using `fstream` + custom splitters
- 📋 Managed using `vector`, custom file parsers, and no external libraries
- 💡 Global variables for: `CurrentUser`, `UsersFileName`, `ClientsFileName`
- 🧮 Well-formatted console UI using `setw`, `\n`, and `system("cls")`
- ✨ Fully custom utilities (e.g., no built-in `split`, manual case checks)

---

## 🖥️ Screens / Modules

### 👤 Employee Panel
- Login screen with case-sensitive credential checks
- Role-based access control via bit flags

### 🧑‍💼 Client Management
- Add New Client
- Update Client Info
- Delete Client
- Search Clients
- List All Clients

### 🧑‍💼 User Management (Employees)
- Add New User
- Update User Info
- Delete User
- Search Users
- List All Users

### 💵 Financial Transactions
- Deposit & Withdraw Screens
- View Total Balances Across All Clients

### 🔄 Navigation
- Clear options to return to previous screens
- Consistent UI design and screen flow

---

## 🧪 Control Flow & Techniques Used

- `if/else`, `switch/case`, `for`, `while`, and `do-while` control structures
- `enum` for structured data and options
- `fstream` + `vector` for file handling
- Modular design for code reuse
- Global constants & variables for session and file management

---

## 📌 Notes

- Some validations are simplified for educational purposes
- The project focuses on readability, modularity, and logic clarity over complexity
- Entire project is **custom built** (no STL functions for split, lowercase checks, etc.)

---

## 📽️ Demo Video (Optional)

*Add a YouTube or Google Drive link here if you'd like to include a demonstration.*

---

## 📦 More Projects

🔗 I showcase this and other programming projects on my Telegram channel:  
👉 [https://t.me/ahmedelhwwary3](https://t.me/ahmedelhwwary3)

Feel free to join and explore more educational and practical C++ examples.

---

## 🧠 Author

Developed with 💻 by **Ahmed Elhwwary** – A self-taught programmer passionate about clarity, user experience, and low-level control.

