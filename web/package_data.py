#!/usr/bin/env python3
"""Split Cro-Mag Rally's Data folder into several Emscripten preload packages.

Usage: package_data.py <DataDir> <DistDir> <file_packager> <maxBytes>

Writes data-N.data / data-N.js into DistDir plus data-manifest.json listing
the loader scripts that index.html must include before CroMagRally.js.
"""
import json, os, subprocess, sys, time

data_dir, dist, packager, max_bytes = sys.argv[1], sys.argv[2], sys.argv[3], int(sys.argv[4])

files = []
for root, _, names in os.walk(data_dir):
    for n in sorted(names):
        full = os.path.join(root, n)
        rel = os.path.relpath(full, data_dir).replace(os.sep, "/")
        files.append((os.path.getsize(full), rel, full))
files.sort(reverse=True)

# First-fit decreasing bin packing
bins = []
for size, rel, full in files:
    for b in bins:
        if b["size"] + size <= max_bytes:
            b["files"].append((rel, full)); b["size"] += size
            break
    else:
        bins.append({"size": size, "files": [(rel, full)]})

scripts = []
for i, b in enumerate(bins):
    name = f"data-{i}"
    args = [packager, f"{name}.data", f"--js-output={name}.js", "--use-preload-cache", "--no-node"]
    args += ["--preload"] + [f"{full}@/Data/{rel}" for rel, full in b["files"]]
    subprocess.run(args, cwd=dist, check=True)
    scripts.append(f"{name}.js")
    print(f"{name}: {len(b['files'])} files, {b['size']/1e6:.1f} MB")

with open(os.path.join(dist, "data-manifest.json"), "w") as f:
    json.dump({"version": str(int(time.time())), "scripts": scripts, "totalBytes": sum(b["size"] for b in bins)}, f)
