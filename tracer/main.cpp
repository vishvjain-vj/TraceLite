#include "tracer.h"
#include <windows.h>

// Simulate heavy work (~40 ms)
void heavyComputation() {
    TRACE_FUNCTION();
    Sleep(40);
}

// Simulate light work (~5 ms)
void lightComputation() {
    TRACE_FUNCTION();
    Sleep(5);
}

// Simulate I/O work (~20 ms)
void databaseCall() {
    TRACE_FUNCTION();
    Sleep(20);
}

// Business logic layer
void serviceLayer() {
    TRACE_FUNCTION();
    lightComputation();
    databaseCall();
}

// Request handler
void requestHandler() {
    TRACE_FUNCTION();
    serviceLayer();
    heavyComputation();
}

int main() {
    // Simulate multiple requests
    for (int i = 0; i < 3; i++) {
        requestHandler();
    }
    return 0;
}
