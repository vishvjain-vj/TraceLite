#include "tracer.h"

#include <fstream>
#include <stack>
#include <vector>
#include <ctime>
#include <algorithm>
#include <windows.h>

static std::stack<std::string>& getCallStack() {
    static std::stack<std::string>* stack =
        new std::stack<std::string>();
    return *stack;
}

static bool firstEntry = true;

// Open JSON array once
static void openLogFile() {
    static bool opened = false;
    if (!opened) {
        std::ofstream file("../logs/trace_log.json", std::ios::trunc);
        file << "[\n";
        opened = true;
    }
}

// Close JSON array once (program end)
static void closeLogFile() {
    std::ofstream file("../logs/trace_log.json", std::ios::app);
    file << "\n]\n";
}

// Ensures JSON closes safely at shutdown
struct LogCloser {
    ~LogCloser() {
        closeLogFile();
    }
};

static LogCloser logCloser;

// =====================================================
// Tracer implementation
// =====================================================

Tracer::Tracer(const std::string& name)
    : functionName(name) {

    getCallStack().push(functionName);
    startTime = std::chrono::high_resolution_clock::now();
}

Tracer::~Tracer() {

    auto& callStack = getCallStack();
    if (callStack.empty()) return;

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::microseconds>(
            endTime - startTime
        ).count();

    std::time_t timestamp = std::time(nullptr);
    DWORD tid = GetCurrentThreadId();

    // Capture stack snapshot
    std::vector<std::string> stackSnapshot;
    std::stack<std::string> temp = callStack;

    while (!temp.empty()) {
        stackSnapshot.push_back(temp.top());
        temp.pop();
    }
    std::reverse(stackSnapshot.begin(), stackSnapshot.end());

    // Write log
    openLogFile();
    std::ofstream logFile("../logs/trace_log.json", std::ios::app);

    if (!firstEntry) {
        logFile << ",\n";
    }
    firstEntry = false;

    logFile << "{ \"function\": \"" << functionName
            << "\", \"time_us\": " << duration
            << ", \"timestamp\": " << timestamp
            << ", \"thread_id\": " << tid
            << ", \"stack\": [";

    for (size_t i = 0; i < stackSnapshot.size(); i++) {
        logFile << "\"" << stackSnapshot[i] << "\"";
        if (i + 1 < stackSnapshot.size()) logFile << ", ";
    }

    logFile << "] }";

    callStack.pop();
}
