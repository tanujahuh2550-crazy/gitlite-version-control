# gitlite-version-control

# VersionVault - A Mini Version Control System

## 📌 Project Overview

**VersionVault** is a console-based Mini Version Control System developed in **C++** as a Project-Based Learning (PBL) project for the **Data Structures and Algorithms (DSA)** course.

The project is inspired by the fundamental concepts of modern version control systems such as Git. Rather than replicating Git, it demonstrates how core data structures can be applied to build a simplified version control system capable of managing project versions, commit history, branches, searching, and rollback operations.

The primary objective is to understand the practical implementation of Data Structures through a real-world software engineering application.

---

# 🎯 Objectives

* Understand the practical use of Data Structures.
* Simulate the working of a basic Version Control System.
* Implement major DSA concepts in a single integrated project.
* Develop a modular application using Object-Oriented Programming in C++.
* Gain experience with collaborative software development using Git and GitHub.

---

# ✨ Features

* Create and initialize repositories
* Track project files
* Create commits with messages
* View complete commit history
* Undo (Rollback) the latest commit
* Create and manage branches
* Search commits and files
* Sort commit history
* Save repository data using files
* Load previous repository data

---

# 🏗️ Data Structures Used

| Feature               | Data Structure           |
| --------------------- | ------------------------ |
| Repository Management | Array / Vector           |
| Commit History        | Linked List              |
| Undo Operation        | Stack                    |
| Merge Request Queue   | Queue                    |
| Branch Management     | Tree                     |
| File & Commit Search  | Hash Table               |
| Commit Lookup         | Binary Search Tree (BST) |
| Sorting History       | Merge Sort / Quick Sort  |
| Data Persistence      | File Handling            |

---

# ⚙️ Project Workflow

```text
Create Repository
        │
        ▼
Add Files
        │
        ▼
Create Commit
        │
        ▼
Store Commit History
        │
        ▼
Create / Switch Branch
        │
        ▼
Search / Sort
        │
        ▼
Undo Commit
        │
        ▼
Save Repository
```

---

# 📁 Project Structure

```text
VersionVault/
│
├── main.cpp
├── repository.cpp
├── repository.h
├── commit.cpp
├── commit.h
├── branch.cpp
├── branch.h
├── search.cpp
├── search.h
├── hash.cpp
├── hash.h
├── bst.cpp
├── bst.h
├── stack.cpp
├── stack.h
├── queue.cpp
├── queue.h
├── filemanager.cpp
├── filemanager.h
└── README.md
```

---

# 👥 Team Members & Responsibilities

### Tanuja (Team Leader)

* Project Architecture
* System Integration
* Repository Management
* File Handling
* Testing & Debugging
* Documentation & Presentation

### Lakshit

* Commit Engine
* Linked List Implementation
* Stack (Undo)
* Branch Logic

### Yashasvi

* Hash Table
* Binary Search Tree
* Search Module
* Sorting Algorithms

### Kavya

* User Interface
* Menu System
* File Operations
* Repository & Branch Management

---

# 🛠️ Technologies Used

* C++
* Object-Oriented Programming (OOP)
* File Handling
* Git
* GitHub

---

# 📚 Learning Outcomes

After completing this project, the team will understand:

* Practical applications of Data Structures
* Modular software development
* Version control concepts
* Object-Oriented Programming
* Team collaboration using GitHub
* Code integration through Pull Requests

---

# 🚀 Future Enhancements

* User authentication
* Commit comparison (Diff)
* Merge conflict simulation
* Multiple repositories
* Tagging releases
* Graphical User Interface (GUI)
* Remote repository simulation

---

# 📖 Conclusion

VersionVault demonstrates how multiple Data Structures work together to solve a real-world software engineering problem. By integrating linked lists, stacks, queues, trees, hashing, searching, sorting, and file handling into a single application, the project provides a practical understanding of DSA concepts beyond textbook implementations.

---

## © Academic Project

**Course:** Data Structures and Algorithms (PBL)

**Language:** C++

**Project Type:** Console-Based Application

**Inspired by:** Modern Version Control Systems (Educational Purpose Only)
