# Person Data Manager (C++)

## About

Person Data Manager is a console-based application written in C++ for managing a list of individuals and their associated numeric scores. The program reads data from text files, allows real-time modifications in memory, and exports the updated dataset back to a file.

## Purpose and Key Features

The program automates basic CRUD operations on a list of entries formatted as `FirstName Surname Score`. Key features include:

* **File I/O:** Load data from text files with automatic `.txt` extension appending and path resolution relative to the working directory or executable location.
* **Data Validation:** Strict verification of first names, surnames, and scores to ensure correct formatting and prevent invalid input.
* **Record Management:** Modify existing records, add new individuals, or delete entries using zero-based indexing.
* **Interactive Menu:** Menu system to display records, execute updates, and save changes upon exit or demand.

## How to Run

### Prerequisites
* C++ Compiler

### Steps:

1. **Compile the source code:**
   ```bash
   g++ -std=c++11 -o person_manager DataManager.cpp
