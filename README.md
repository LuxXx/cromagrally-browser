# Cro-Mag Rally in the browser

**Play it at https://cromagrally.tdbr.de**

This is a WebAssembly port of [Iliyas Jorio's modern Cro-Mag Rally port](https://github.com/jorio/CroMagRally)
(the 2000 Pangea Software game). The original C game code is compiled with [Emscripten](https://emscripten.org)
against SDL3 and WebGL, with small web-specific changes under `#ifdef __EMSCRIPTEN__`.

### How the web port works

- **Compiler:** Emscripten, using its SDL3 port for windowing, input, gamepads and audio.
- **Graphics:** the game uses fixed-function OpenGL (lighting, fog, alpha test, `glBegin`). It runs on top of
  Emscripten's `LEGACY_GL_EMULATION`, with a few fixes:
  - textures are converted to RGBA8 on upload (WebGL has no BGRA / packed 1-5-5-5 formats);
  - vertex arrays are interleaved and indices narrowed to 16-bit at draw time;
  - `GL_COLOR_MATERIAL` is patched into the emulated lighting shader (`web/patch_glemu.py`);
  - enable flags and the current color are tracked in C for `glIsEnabled`/`glGetFloatv`,
    and `glBegin`/`glEnd` vertices always carry a color (`Source/Headers/webgl_compat.h`);
  - `glGetError` and the state queries in `OGL_PushState` are answered from shadowed state instead of
    WebGL (each WebGL readback stalls on the GPU process; this took races from ~15 fps to 60 fps);
  - textures requested as `GL_RGB5_A1` get 1-bit alpha, which the item sprites' `GL_EQUAL` alpha cutouts rely on;
  - GL emulation is initialized after SDL creates its WebGL context.
- **Game loop:** the game's blocking loops run as-is thanks to `ASYNCIFY`; each frame waits for `requestAnimationFrame`, so it's in step with the display.
- **Saves:** prefs, progress and race times are kept in IndexedDB (`IDBFS`) and persist across visits.
- **Assets:** game data is split into packages below Cloudflare's 25 MiB per-file limit and cached in IndexedDB after the first visit.
- **Multiplayer:** local split-screen works (with gamepads); network play doesn't exist in the browser.

### Building the web version

```sh
# Install & activate the Emscripten SDK (https://emscripten.org/docs/getting_started/downloads.html)
source /path/to/emsdk/emsdk_env.sh

./web/build.sh              # outputs a static site to dist/
python3 -m http.server -d dist 8000   # test locally at http://localhost:8000

npx wrangler deploy         # deploy dist/ to Cloudflare (see wrangler.toml)
```

Web-specific files live in `web/`. Licensing is unchanged: CC BY-NC-SA 4.0 (see LICENSE.md).

---

# Cro-Mag Rally

## *The wildest racing game since man invented the wheel!*

This is a port of Pangea Software’s racing game **Cro-Mag Rally** to modern operating systems.

**Download the game for macOS, Windows and Linux here:** https://github.com/jorio/CroMagRally/releases

![Cro-Mag Rally Screenshot](docs/screenshot.webp)

## About Cro-Mag Rally

> In Cro-Mag Rally you are a speed-hungry caveman named Brog who races through the Stone, Bronze, and Iron Ages in primitive vehicles such as the Geode Cruiser, Bone Buggy, Logmobile, Trojan Horse, and many others. Brog has at his disposal an arsenal of primitive weaponry ranging from Bone Bombs to Chinese Bottle Rockets and Heat Seeking Homing Pigeons.
> 
> In addition to single-player racing where one player races against the computer, there are also several different multi-player modes including Tag, Capture the Flag, and Survival. Up to four players can play on a single computer in split-screen mode.

CMR was released in 2000 by Pangea Software as a Mac exclusive, and it was a pack-in game on Macs that came out around that time.

## About this port

This is a port of the original OS 9 version of the game. It aims to provide the best way to experience CMR on today’s computers. It is an “enhanced” version insofar as it fixes bugs that may hinder the experience, and it brings in a few new features in keeping with the spirit of the original game.

Some of the new features include:
- Up to 4 players in split-screen multiplayer (up from 2 in the original).
- The UI is subtly animated and has been tweaked to be pleasant to look at on modern widescreens.
- Enable a timer in race modes to hone your racing skills, and keep track of your records in the all-new scoreboard!

I haven’t had time to restore NetSprockets multiplayer from the OS 9 version yet, but that may come in a later release.

### More documentation

- [BUILD](BUILD.md) – How to build the game from source
- [CHANGELOG](CHANGELOG.md) – Cro-Mag Rally version history
- [LICENSE](LICENSE.md) – Licensing info (see also below)
- [SECRETS](SECRETS.md) – Cheat codes!

### Legal info

Cro-Mag Rally © 2000 Pangea Software, Inc. Cro-Mag Rally is a trademark of Pangea Software, Inc. This version was made and re-released here (https://github.com/jorio/CroMagRally) under permission from Pangea Software, Inc.

This version is licensed under [CC-BY-NC SA 4.0](LICENSE.md).

## More Pangea stuff!

Check out my ports of [Bugdom](https://github.com/jorio/Bugdom), [Nanosaur](https://github.com/jorio/Nanosaur), [Mighty Mike (Power Pete)](https://github.com/jorio/MightyMike) and [Otto Matic](https://github.com/jorio/OttoMatic).

All ports are free of charge! If you’d like to support the development of Pangea game ports, feel free to visit https://jorio.itch.io and name your own price for any of the games there. Much appreciated! 😊
