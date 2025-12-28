# TraceLite

TraceLite is a lightweight **C++ function tracing system** with a **Python (Flask) analytics backend** and a **web-based visualization layer**.  
It is designed to instrument function execution, capture timing and call stack information, and present structured performance insights through a simple dashboard.

The project focuses on correctness, safety, and real-world engineering constraints rather than synthetic benchmarks.

---

## Overview

TraceLite instruments C++ functions using RAII to automatically record:
- execution duration
- call hierarchy
- thread context (Windows-safe)

Trace data is written in a streaming-friendly format and consumed by a backend service that exposes both raw traces and aggregated performance statistics.

---

## Architecture

The system is composed of three loosely coupled layers:

1. **Tracer (C++)**
   - Function-level instrumentation
   - High-resolution timing
   - Call stack tracking
   - NDJSON logging

2. **Backend (Flask)**
   - Reads trace logs safely
   - Provides REST APIs for analysis
   - Aggregates performance metrics

3. **Frontend (HTML/JavaScript)**
   - Visualizes trace data
   - Displays summary statistics

This separation allows each component to evolve independently.

---

## Project Structure

TraceLite/
├── tracer/ # C++ tracing implementation
│ ├── tracer.h
│ ├── tracer.cpp
│ └── main.cpp
│
├── backend/ # Flask backend service
│ └── app.py
│
├── frontend/ # Web-based dashboard
│ └── index.html
│
├── logs/ # Runtime logs (ignored in Git)
│
├── .gitignore
└── README.md


---

## How It Works

### C++ Tracing Layer

Each function is instrumented using an RAII object (`Tracer`) created at function entry and destroyed at exit.  
This guarantees that timing and logging occur even in the presence of early returns.

On function exit, the tracer records:
- function name
- execution time (microseconds)
- call stack snapshot
- thread identifier

Each trace entry is written as a single JSON object on its own line (NDJSON).

---

### Backend Analytics Layer

The Flask backend reads the trace log line-by-line to avoid partial read issues during active writes.  
It exposes two endpoints:

- `/traces`  
  Returns raw trace entries.

- `/stats`  
  Returns aggregated statistics including:
  - total function calls
  - per-function call counts
  - average execution time
  - slowest observed function

---

### Frontend Visualization

The frontend fetches data from the backend APIs and presents:
- a tabular view of function traces
- a summary section with aggregated metrics

The UI is intentionally minimal to keep the focus on correctness and data clarity.

---

## Running the Project

### Prerequisites

- Windows
- MinGW with C++17 support
- Python 3.9+
- Flask

---

### Build and Run the Tracer

bash
cd tracer
g++ main.cpp tracer.cpp -o app -std=c++17
./app
### This generates trace logs in the logs directory.

### start the Backend 
cd backend
python app.py
Backend runs at http://127.0.0.1:5000

### Opening of Dashboard 
frontend/index.html

### Design Decisions
JSON for Logging:
JSON arrays are unsafe for live writes and concurrent reads.
Using newline-delimited JSON ensures that each trace entry is independently readable and resilient to partial writes.

RAII-Based Instrumentation:
RAII ensures that tracing logic executes reliably on every function exit without manual bookkeeping.

File-Based Communication:
Using the filesystem as an integration boundary keeps the tracer independent of the backend and simplifies debugging.

### Limitations
  Auto-refresh is disabled due to short-lived execution windows
  Single-process tracing only
  File-based storage (no database backend)

### Future Work
-> Live streaming via WebSockets
-> Pause and resume tracing
-> Ring-buffered trace storage
-> Timeline visualization
-> Export functionality (CSV / JSON)

### Author
Vishvkumar Jain
