// WebGL compatibility shims for Emscripten's legacy GL emulation.
//
// The emulation ignores glEnable/glDisable for some fixed-function caps
// (e.g. GL_TEXTURE_2D, GL_NORMALIZE), returns 0 from glIsEnabled for them,
// and can't answer glGetFloatv(GL_CURRENT_COLOR). OGL_PushState/PopState rely
// on all of these, so track that state ourselves.
//
// The emulation also textures any geometry that carries texcoords, regardless
// of GL_TEXTURE_2D. So, drop texcoords while texturing is disabled.
//
// Reading state back from WebGL (glGetError, glGetIntegerv...) stalls until the
// GPU process answers, and the game does it after nearly every draw call, which
// made it run at a fraction of the frame rate. So, don't check for errors, and
// answer the state queries the game makes from our own copy.
//
// Between glBegin/glEnd, the emulation writes each glColor call straight into
// the interleaved vertex stream, so the stream gets misaligned unless every
// vertex has its own color. So, re-send the current color before each vertex.

#pragma once

#ifdef __EMSCRIPTEN__

extern bool		gWebTexture2DEnabled;

void			WebGL_Enable(GLenum cap);
void			WebGL_Disable(GLenum cap);
GLboolean		WebGL_IsEnabled(GLenum cap);
void			WebGL_GetFloatv(GLenum pname, GLfloat* params);
void			WebGL_Color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void			WebGL_Hint(GLenum target, GLenum mode);
void			WebGL_Begin(GLenum mode);
void			WebGL_End(void);
void			WebGL_EmitVertexColor(void);
void			WebGL_BlendFunc(GLenum sfactor, GLenum dfactor);
void			WebGL_DepthMask(GLboolean flag);
void			WebGL_GetIntegerv(GLenum pname, GLint* params);
void			WebGL_GetBooleanv(GLenum pname, GLboolean* params);

#define glEnable(cap)					WebGL_Enable(cap)
#define glDisable(cap)					WebGL_Disable(cap)
#define glIsEnabled(cap)				WebGL_IsEnabled(cap)
#define glGetFloatv(pname, params)		WebGL_GetFloatv(pname, params)
#define glColor4f(r, g, b, a)			WebGL_Color4f(r, g, b, a)
#define glColor3f(r, g, b)				WebGL_Color4f(r, g, b, 1.0f)
#define glColor4fv(v)					WebGL_Color4f((v)[0], (v)[1], (v)[2], (v)[3])
#define glHint(target, mode)			WebGL_Hint(target, mode)
#define glGetError()					((GLenum) GL_NO_ERROR)
#define glBlendFunc(sfactor, dfactor)	WebGL_BlendFunc(sfactor, dfactor)
#define glDepthMask(flag)				WebGL_DepthMask(flag)
#define glGetIntegerv(pname, params)	WebGL_GetIntegerv(pname, params)
#define glGetBooleanv(pname, params)	WebGL_GetBooleanv(pname, params)
#define glTexCoord2f(u, v)				(gWebTexture2DEnabled ? (glTexCoord2f)(u, v) : (void) 0)
#define glBegin(mode)					WebGL_Begin(mode)
#define glEnd()							WebGL_End()
#define glVertex2f(x, y)				(WebGL_EmitVertexColor(), (glVertex2f)(x, y))
#define glVertex3f(x, y, z)				(WebGL_EmitVertexColor(), (glVertex3f)(x, y, z))

#endif
