#!/usr/bin/env python3
"""Post-link patch for Emscripten's legacy GL emulation (in the generated JS).

Cro-Mag Rally relies on GL_COLOR_MATERIAL (GL_AMBIENT_AND_DIFFUSE): when lighting is
on, the current vertex color acts as the material's ambient & diffuse color.
Emscripten's emulated lighting shader ignores vertex color, so rewrite it to
use a_color in place of the ambient/diffuse material uniforms.
"""
import sys

path = sys.argv[1]
s = open(path, encoding="utf-8").read()

replacements = [
    # Cro-Mag Rally clips sprite edges with glAlphaFunc(GL_EQUAL, 1). An exact
    # float compare fails on some GPUs' fragment precision, so allow a hair of slack.
    ("if (!(gl_FragColor.a == u_alphaTestRef)) { discard; }",
     "if (abs(gl_FragColor.a - u_alphaTestRef) > 0.5/255.0) { discard; }"),
    ("v_color.w = u_materialDiffuse.w;", "v_color.w = a_color.w;"),
    ("u_lightModelAmbient.xyz * u_materialAmbient.xyz;", "u_lightModelAmbient.xyz * a_color.xyz;"),
    (".xyz * u_materialAmbient.xyz;", ".xyz * a_color.xyz;"),
    (".xyz * u_materialDiffuse.xyz;", ".xyz * a_color.xyz;"),
]

for old, new in replacements:
    if old not in s:
        if new in s:
            continue  # already patched
        sys.exit(f"patch_glemu: pattern not found: {old!r}")
    s = s.replace(old, new)

open(path, "w", encoding="utf-8").write(s)
print("patch_glemu: patched GL_COLOR_MATERIAL support into", path)
