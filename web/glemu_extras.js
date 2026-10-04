// Small additions to Emscripten's legacy GL emulation for Cro-Mag Rally.
addToLibrary({
  // Emscripten's legacy GL emulation lacks glColorMaterial. The game always uses
  // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE), which
  // patch_glemu.py bakes directly into the emulated lighting shader.
  glColorMaterial: (face, mode) => {},

});
