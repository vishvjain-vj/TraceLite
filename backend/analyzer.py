import json
from collections import defaultdict

LOG_FILE = "../logs/trace_log.json"

def analyze_logs():
    function_times = defaultdict(int)

    with open(LOG_FILE, "r") as f:
        for line in f:
            entry = json.loads(line)
            function = entry["function"]
            time_us = entry["time_us"]
            function_times[function] += time_us

    return function_times

if __name__ == "__main__":
    results = analyze_logs()

    print("Performance Hotspots:")
    for func, time in sorted(results.items(),
                              key=lambda x: x[1],
                              reverse=True):
        print(f"{func}: {time} microseconds")
