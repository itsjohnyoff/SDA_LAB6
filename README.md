# SDA Lab 6 — Stack & Queue ADT with Citizen Records

> **Course:** Data Structures and Algorithms (SDA)  
> **Lab No.:** 6 — Solving problems using Stack / Queue (Simple, Double Ended, Circular, Priority) ADT  
> **Language:** C (procedural style)

---

## Table of Contents

- [Overview](#overview)
- [Problem Statement](#problem-statement)
- [Custom Data Type (CDT)](#custom-data-type-cdt)
- [Project Structure](#project-structure)
- [How It Works](#how-it-works)
  - [Version A — Dynamic Stack](#version-a--dynamic-stack-based-on-list-adt)
  - [Version B — Dynamic Queues](#version-b--dynamic-queues-based-on-list-adt)
  - [File Tools](#file-tools)
- [Building & Running](#building--running)
  - [Prerequisites](#prerequisites)
  - [Compile](#compile)
  - [Run](#run)
  - [Automated Demo (auto_input.txt)](#automated-demo-auto_inputtxt)
- [Menu Reference](#menu-reference)
- [File I/O Formats](#file-io-formats)
- [Key Design Decisions](#key-design-decisions)

---

## Overview

This program implements **Stack** and **Queue** abstract data structures using a **doubly-linked list** as the underlying data structure. All operations work on a custom `Citizen` record type (structs with nested structs), using pointers and dynamic memory allocation throughout.

The solution is split into two main versions:

| Version | Data Structure | Description |
|---------|---------------|-------------|
| **A** | Dynamic Stack | LIFO structure — push, pop, traverse, search, delete |
| **B** | Dynamic Queues | All four queue variants — Simple, Double Ended (Deque), Circular, and Priority |

Both versions support **file persistence** — records can be saved to and loaded from `.txt` (text) and `.bin` (binary) files.

---

## Problem Statement

Develop a procedural-style program in C/C++ that:

1. Uses **pointers** for all data manipulation.
2. Organizes data processing based on **dynamic memory allocation** (`malloc`, `realloc`, `free`).
3. Implements a **dynamic Stack based on a linked-list ADT** (Version A).
4. Implements **all four Queue types** (Simple, Double Ended, Circular, Priority) **based on a linked-list ADT** (Version B).
5. Provides operations for: **creating**, **traversing/displaying**, **inserting**, **searching** (by position and by value), **deleting** (by position and by value), and **file registration** (save/load in text and binary modes).
6. Structures the solution using **header files** called from `main()` through a complete menu system.

---

## Custom Data Type (CDT)

The core data type is `Citizen`, built from nested structs declared with `typedef`:

```
Citizen
├── name[50]              (char array)
├── surname[50]           (char array)
├── dob                   (Date struct: day, month, year)
├── gender                (char: M / F / O)
├── home                  (Address struct: city, street, postCode)
├── work                  (Address struct: city, street, postCode)
├── ageYears              (int — computed from DOB)
├── ageMonths             (int — computed from DOB)
├── ageDays               (int — computed from DOB)
├── category              (enum: Child / Adult / Senior)
└── amountPaid            (float — derived from category)
```

**Derived fields** are computed automatically:
- **Age** is calculated from the date of birth relative to the current system date.
- **Category**: Child (< 18), Adult (18–59), Senior (60+).
- **Amount paid**: Child → 500, Adult → 1000, Senior → 2000.

The linked-list node (`Node`) wraps `Citizen` with `next`/`prev` pointers and an optional `priority` field.

---

## Project Structure

```
sda-assignment/
├── main.c            # Entry point, menu system, input helpers
├── data_types.h      # Citizen, Date, Address, Node struct definitions
├── data_types.c      # Display/print functions for citizen records
├── stack.h           # Stack ADT interface
├── stack.c           # Stack implementation (doubly-linked list)
├── queue.h           # Queue ADT interface (all 4 types)
├── queue.c           # Queue implementation (simple, deque, circular, priority)
├── file_io.h         # File I/O interface
├── file_io.c         # Save/load/display/delete for text and binary files
├── auto_input.txt    # Pre-filled input for automated demo runs
├── .gitignore        # Excludes binaries and generated output files
└── README.md         # This file
```

All logic is organized through **header files** (`data_types.h`, `stack.h`, `queue.h`, `file_io.h`) that are included in `main.c`.

---

## How It Works

### Version A — Dynamic Stack based on List ADT

The stack is implemented as a **singly-direction traversal over a doubly-linked list** (each node has `next` and `prev` pointers). The `top` pointer always references the most recently pushed element.

**Supported operations:**

| # | Operation | Description |
|---|-----------|-------------|
| 1 | Create stack | Read N citizen records and build the stack from scratch |
| 2 | Push | Insert a new citizen on top of the stack |
| 3 | Pop | Remove and display the top citizen |
| 4 | Traverse | Display all records from top to bottom |
| 5 | Search by position | Find a citizen at a given position from the top |
| 6 | Search by surname | Find the first citizen matching a surname |
| 7 | Delete by position | Remove a citizen at a specific position |
| 8 | Delete by surname | Remove the first citizen matching a surname |
| 9 | Save to text | Write the stack to a `.txt` file |
| 10 | Save to binary | Write the stack to a `.bin` file |
| 11 | Load from binary | Read citizens from a `.bin` file into the stack |

### Version B — Dynamic Queues based on List ADT

Four queue types are implemented, selectable at runtime:

| Queue Type | Enqueue | Dequeue | Notes |
|------------|---------|---------|-------|
| **Simple** | Rear | Front | Standard FIFO |
| **Double Ended (Deque)** | Front or Rear | Front or Rear | User chooses which end |
| **Circular** | Rear | Front | `rear→next` loops back to `front`, `front→prev` loops to `rear` |
| **Priority** | Sorted insert | Front (highest priority) | Priority derived from category (Child=1 < Adult=2 < Senior=3) |

All four queues share the same `Queue` struct and the same menu. The user can switch between queue types at any time.

**Supported operations** (same as stack, adapted for queue semantics):

| # | Operation |
|---|-----------|
| 1 | Switch queue type |
| 2 | Create queue from N records |
| 3 | Enqueue (insert element) |
| 4 | Dequeue (remove element) |
| 5 | Traverse and display |
| 6 | Search by position |
| 7 | Search by surname |
| 8 | Delete by position |
| 9 | Delete by surname |
| 10 | Save to text file |
| 11 | Save to binary file |
| 12 | Load from binary file |

### File Tools

A dedicated submenu for standalone file management:

- **Create** empty text or binary files
- **Open/reopen** and display text files or binary stack/queue files
- **Delete** files from disk

---

## Building & Running

### Prerequisites

- A C compiler supporting C99 or later (GCC, MinGW, MSVC, Clang)
- Windows / Linux / macOS

### Compile

Using **GCC** (or MinGW on Windows):

```bash
gcc -o program main.c data_types.c stack.c queue.c file_io.c -Wall -Wextra
```

Using **MSVC** (Developer Command Prompt):

```bash
cl main.c data_types.c stack.c queue.c file_io.c /Fe:program.exe
```

### Run

```bash
./program        # Linux / macOS
program.exe      # Windows
```

The program launches an interactive text-based menu. Navigate by entering the number of the desired option.

### Automated Demo (auto_input.txt)

A pre-filled input file is included to quickly demonstrate the program without manual typing:

```bash
./program < auto_input.txt
```

This will:
1. Enter the **Stack** menu and create 4 citizen records (Ana Munteanu, Andrei Popa, Elena Rusu, Ion Ionescu)
2. Save the stack to `stack_auto.bin`
3. Enter the **Queue** menu, select **Priority Queue**, and create 4 citizen records (Mihai Ceban, Maria Lungu, Grigore Balan, Daniela Cojocaru)
4. Save the queue to `queue_auto.bin`
5. Exit the program

---

## Menu Reference

```
================ MAIN MENU =================
1. Version A: Dynamic Stack based on List ADT
2. Version B: Dynamic Queues based on List ADT
3. File create/open/reopen/delete tools
0. Exit
```

Selecting option **1** opens the Stack submenu, option **2** opens the Queue submenu (which first asks you to pick a queue type), and option **3** opens the file management tools.

---

## File I/O Formats

| Format | Stack | Queue |
|--------|-------|-------|
| **Text (`.txt`)** | Human-readable, one citizen per line with all fields | Same, plus priority column for Priority Queue |
| **Binary (`.bin`)** | Raw `Citizen` structs written sequentially | `QueueFileRecord` (Citizen + priority int) written sequentially |

- **Text files** can be opened in any text editor.
- **Binary files** can be reloaded into the program. When loading, the user can choose to **replace** the current data or **append** to it.
- Files can be **created**, **opened/reopened**, and **deleted** through the File Tools menu.

---

## Key Design Decisions

1. **Doubly-linked list nodes** — Both stack and queue use `Node` with `next` and `prev` pointers. This allows O(1) deletion from arbitrary positions when the node reference is known, and simplifies the circular queue's wrap-around logic.

2. **Shared `Node` type** — A single `Node` struct (defined in `data_types.h`) is shared across stack and queue implementations to avoid code duplication.

3. **Priority from category** — The Priority Queue automatically derives priority from the citizen's age category (`Child=1`, `Adult=2`, `Senior=3`), so lower values are dequeued first (youngest categories get highest priority).

4. **Circular queue pointer invariant** — In the circular queue, `rear→next` always points to `front` and `front→prev` always points to `rear`, forming a closed ring. Traversal uses a counter rather than `NULL` checks to know when to stop.

5. **Dynamic memory throughout** — All nodes are heap-allocated with `malloc()` and freed with `free()`. The stack creation buffer and queue load buffer also use `malloc()`/`realloc()` for flexible sizing.

6. **Input validation** — All user input is validated: dates are checked against calendar rules and the system clock, integers are range-checked with `strtol()`, and string fields cannot be empty.
