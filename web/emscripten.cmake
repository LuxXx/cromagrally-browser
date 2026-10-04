# Emscripten (WebAssembly) build settings for Cro-Mag Rally.
# Included from the top-level CMakeLists.txt when building with emcmake.

set_target_properties(${GAME_TARGET} PROPERTIES SUFFIX ".js")

target_compile_options(${GAME_TARGET} PRIVATE -Wno-unused-parameter -Wno-shadow)

target_link_options(${GAME_TARGET} PRIVATE
	-fexceptions
	-sLEGACY_GL_EMULATION=1			# The game uses fixed-function OpenGL (lighting, fog, glBegin...)
	-sGL_UNSAFE_OPTS=0				# unsafe opts cause rendering glitches with the game's state changes
	-sASYNCIFY=1					# The game has many blocking loops; yield to the browser on each frame
	-sASYNCIFY_STACK_SIZE=262144
	-sSTACK_SIZE=4MB
	-sALLOW_MEMORY_GROWTH=1
	-sINITIAL_MEMORY=256MB
	-sFORCE_FILESYSTEM=1
	-sEXIT_RUNTIME=0
	-sENVIRONMENT=web
	-sEXPORTED_RUNTIME_METHODS=FS,addRunDependency,removeRunDependency,callMain
	-lidbfs.js
	--pre-js ${CMAKE_SOURCE_DIR}/web/pre.js
)

# Game data is packaged separately (see web/build.sh) so each file stays
# under Cloudflare's 25 MiB per-asset limit.

target_link_options(${GAME_TARGET} PRIVATE --js-library ${CMAKE_SOURCE_DIR}/web/glemu_extras.js)

add_custom_command(TARGET ${GAME_TARGET} POST_BUILD
	COMMAND python3 ${CMAKE_SOURCE_DIR}/web/patch_glemu.py $<TARGET_FILE:${GAME_TARGET}>)
