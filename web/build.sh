#!/usr/bin/env bash
# Build Cro-Mag Rally for the web. Requires an activated Emscripten SDK (emcc in PATH).
# Output: dist/ — a static site ready to be served (e.g. Cloudflare Pages).
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="$ROOT/build-web"
DIST="$ROOT/dist"
PACKAGER="$(dirname "$(command -v emcc)")/tools/file_packager"

emcmake cmake -S "$ROOT" -B "$BUILD" -DCMAKE_BUILD_TYPE=Release
cmake --build "$BUILD" -j"$(nproc)"

rm -rf "$DIST"
mkdir -p "$DIST"
cp "$BUILD/CroMagRally.js" "$BUILD/CroMagRally.wasm" "$DIST/"
cp "$ROOT/web/index.html" "$DIST/"
cp "$ROOT/web/_headers" "$DIST/"
cp "$ROOT/packaging/io.jor.cromagrally.png" "$DIST/icon.png"

# Package game data in chunks below Cloudflare's 25 MiB per-file limit.
python3 "$ROOT/web/package_data.py" "$ROOT/Data" "$DIST" "$PACKAGER" 23000000

echo "Built $DIST:"
ls -la "$DIST"
