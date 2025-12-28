from flask import Flask, jsonify
from flask_cors import CORS
import json
import os

app = Flask(__name__)
CORS(app)

TRACE_FILE = "../logs/trace_log.json"

def load_traces():
    if not os.path.exists(TRACE_FILE):
        return []
    with open(TRACE_FILE) as f:
        return json.load(f)

@app.route("/traces")
def traces():
    return jsonify(load_traces())

@app.route("/stats")
def stats():
    traces = load_traces()

    if not traces:
        return jsonify({})

    total_calls = len(traces)
    slowest = max(traces, key=lambda x: x["time_us"])

    per_function = {}

    for t in traces:
        name = t["function"]
        time_us = t["time_us"]

        if name not in per_function:
            per_function[name] = {
                "count": 0,
                "total_us": 0
            }

        per_function[name]["count"] += 1
        per_function[name]["total_us"] += time_us

    # Compute averages
    for name in per_function:
        per_function[name]["avg_us"] = (
            per_function[name]["total_us"] //
            per_function[name]["count"]
        )
        del per_function[name]["total_us"]

    return jsonify({
        "total_calls": total_calls,
        "slowest_function": {
            "name": slowest["function"],
            "time_us": slowest["time_us"]
        },
        "per_function": per_function
    })

if __name__ == "__main__":
    app.run(debug=True)
