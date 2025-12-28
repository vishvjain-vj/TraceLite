#ifndef TRACER_H
#define TRACER_H

#include <string>
#include <chrono>

class Tracer {
private:
    std::string functionName;
    std::chrono::high_resolution_clock::time_point startTime;

public:
    explicit Tracer(const std::string& name);
    ~Tracer();
};

#define TRACE_FUNCTION() Tracer ___tracer_instance___(__FUNCTION__)

#endif
