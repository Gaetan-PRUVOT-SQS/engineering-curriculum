#!/usr/bin/env python3

import json
import subprocess
import sys


def errors(paths):
    proc = subprocess.run(
        ["norminette", "-f", "json", *paths],
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        check=False,
    )
    try:
        data = json.loads(proc.stdout)
    except json.JSONDecodeError:
        print(proc.stderr, file=sys.stderr, end="")
        raise SystemExit(1) from None
    found = []
    for item in data["files"]:
        for err in item["errors"]:
            found.append((item["path"], err["name"]))
    return found


def main():
    if len(sys.argv) < 2:
        print("usage: forme.py fichier...", file=sys.stderr)
        return 2
    bad = [
        f"{path}: {name}"
        for path, name in errors(sys.argv[1:])
        if name != "INVALID_HEADER"
    ]
    if bad:
        print("\n".join(bad), file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
