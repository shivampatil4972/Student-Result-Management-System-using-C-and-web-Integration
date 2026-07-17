# 🎓 Full-Stack Student Result Management System

A robust, enterprise-grade Student Result Management System built with a native C++ backend, an embedded SQLite database, and a live-syncing modern Web Dashboard. 

This project bridges the gap between low-level memory management in C++ and responsive, full-stack web development, proving that powerful backend architectures can run seamlessly alongside beautiful, real-time user interfaces.

---

## ✨ Key Features

### 🖥️ Backend (C++)
* **Object-Oriented Architecture**: Clean, modular, and scalable class-based design.
* **Embedded SQLite Database**: Replaces unstable text files with a production-ready, ACID-compliant database for lightning-fast queries and secure data persistence.
* **Native Win32 Multi-threaded Web Server**: Features a custom-built HTTP server using raw WinSock. Every incoming connection spawns an independent thread, preventing UI blocking and allowing dozens of teachers to access the dashboard simultaneously.
* **Interactive Console Menu**: A styled, terminal-based CLI for data entry, searching, and managing student records with bulletproof input validation.

### 🌐 Frontend (Web Dashboard)
* **Glassmorphic UI Engine**: A stunning, modern web interface utilizing Vanilla HTML/CSS/JS, featuring dynamic theme switching (Dark/Light mode).
* **Live-Syncing Dashboard**: Implements background HTTP polling with smart JSON diffing. When a new student is added via the C++ terminal, the web dashboard automatically animates and draws the new data without requiring a page refresh!
* **CSV Export Pipeline**: Instantly generates and downloads Excel-compatible `.csv` reports directly from the browser memory.
* **Academic PDF Print Engine**: Uses advanced `@media print` CSS rules to completely restructure the web dashboard into a highly formalized, black-and-white, properly-aligned academic report card ready for physical printing.

---

## 🛠️ Technology Stack
* **Language:** C++ (Backend), JavaScript (Frontend)
* **Database:** SQLite3 (Embedded via `sqlite3.c` & `sqlite3.h`)
* **Networking:** Native Win32 Sockets (`winsock2.h`)
* **Web:** HTML5, CSS3, Vanilla JS
* **JSON Parsing:** Niels Lohmann's `json.hpp`

---

## 🚀 How to Run

### 1. Build the Application
Ensure you have the MinGW compiler installed. Simply run the provided build script from the root directory:
```powershell
.\build.bat
```
*(Note: The SQLite engine is pre-compiled as `sqlite3.o` to ensure nearly instantaneous build times!)*

### 2. Launch the System
Run the generated executable:
```powershell
.\output\main.exe
```

### 3. Start the Web Server
When the terminal menu appears:
1. Select Option **`6`** to boot the live background Web Server.
2. Open your favorite web browser (Chrome, Edge, Firefox) and navigate to:
   👉 **http://localhost:8080**

### 4. Watch it in Action!
Keep your web browser open on one side of your screen. Go back to the C++ Terminal and choose Option **`1`** to add a new student. 

The moment you hit enter in the terminal, watch the charts and grids on your Web Dashboard automatically expand and update in real-time!

---

## 📜 License
This project is licensed under the MIT License. Feel free to use, modify, and distribute it!
