/*
 Copyright 2009 Johannes Vuorinen
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 
 
 http://www.apache.org/licenses/LICENSE-2.0 
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef OpenGLESContext_H_
#define OpenGLESContext_H_

#include "OpenGLESImplementation.h"

#include "ofShader.h"
//#include "OpenGLESMath.h"

#include "OpenGLESDefines.h"

class ofShader;

/*************************************************************/

namespace OpenGLES {
	
	class OpenGLESContext {
	public:
		OpenGLESContext(int version, OpenGLESImplementation *impl);
		virtual ~OpenGLESContext();
		
		int getOpenGLESVersion();
		
		// OpenglES 1.0 functions
		virtual void glActiveTexture (GLenum texture) = 0;
		virtual void glAlphaFunc (GLenum func, GLclampf ref) = 0;
		virtual void glAlphaFuncx (GLenum func, GLclampx ref) = 0;
		virtual void glBindTexture (GLenum target, GLuint texture) = 0;
		virtual void glBlendFunc (GLenum sfactor, GLenum dfactor) = 0;
		virtual void glClear(GLbitfield mask) = 0;
		virtual void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) = 0;
		virtual void glClearColorx (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha) = 0;
		virtual void glClearDepthf (GLclampf depth) = 0;
		virtual void glClearDepthx (GLclampx depth) = 0;
		virtual void glClearStencil (GLint s) = 0;
		virtual void glClientActiveTexture (GLenum texture) = 0;
		virtual void glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) = 0;
		virtual void glColor4x (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha) = 0;
		virtual void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) = 0;
		virtual void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data) = 0;
		virtual void glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data) = 0;
		virtual void glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) = 0;
		virtual void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height) = 0;
		virtual void glCullFace (GLenum mode) = 0;
		virtual void glDeleteTextures (GLsizei n, const GLuint *textures) = 0;
		virtual void glDepthFunc (GLenum func) = 0;
		virtual void glDepthMask (GLboolean flag) = 0;
		virtual void glDepthRangef (GLclampf zNear, GLclampf zFar) = 0;
		virtual void glDepthRangex (GLclampx zNear, GLclampx zFar) = 0;
		virtual void glDisable (GLenum cap) = 0;
		virtual void glDisableClientState (GLenum array) = 0;
		virtual void glDrawArrays(GLenum mode, GLint first, GLsizei count) = 0;
		virtual void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) = 0;
		virtual void glEnable (GLenum cap) = 0;
		virtual void glEnableClientState (GLenum array) = 0;
		virtual void glFinish (void) = 0;
		virtual void glFlush (void) = 0;
		virtual void glFogf (GLenum pname, GLfloat param) = 0;
		virtual void glFogfv (GLenum pname, const GLfloat *params) = 0;
		virtual void glFogx (GLenum pname, GLfixed param) = 0;
		virtual void glFogxv (GLenum pname, const GLfixed *params) = 0;
		virtual void glFrontFace (GLenum mode) = 0;
		virtual void glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) = 0;
		virtual void glFrustumx (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) = 0;
		virtual void glGenTextures (GLsizei n, GLuint *textures) = 0;
		virtual GLenum glGetError (void) = 0;
		virtual void glGetIntegerv (GLenum pname, GLint *params) = 0;
		virtual const GLubyte *glGetString (GLenum name) = 0;
		virtual void glHint (GLenum target, GLenum mode) = 0;
		virtual void glLightModelf (GLenum pname, GLfloat param) = 0;
		virtual void glLightModelfv (GLenum pname, const GLfloat *params) = 0;
		virtual void glLightModelx (GLenum pname, GLfixed param) = 0;
		virtual void glLightModelxv (GLenum pname, const GLfixed *params) = 0;
		virtual void glLightf (GLenum light, GLenum pname, GLfloat param) = 0;
		virtual void glLightfv (GLenum light, GLenum pname, const GLfloat *params) = 0;
		virtual void glLightx (GLenum light, GLenum pname, GLfixed param) = 0;
		virtual void glLightxv (GLenum light, GLenum pname, const GLfixed *params) = 0;
		virtual void glLineWidth (GLfloat width) = 0;
		virtual void glLineWidthx (GLfixed width) = 0;
		virtual void glLoadIdentity (void) = 0;
		virtual void glLoadMatrixf (const GLfloat *m) = 0;
		virtual void glLoadMatrixx (const GLfixed *m) = 0;
		virtual void glLogicOp (GLenum opcode) = 0;
		virtual void glMaterialf (GLenum face, GLenum pname, GLfloat param) = 0;
		virtual void glMaterialfv (GLenum face, GLenum pname, const GLfloat *params) = 0;
		virtual void glMaterialx (GLenum face, GLenum pname, GLfixed param) = 0;
		virtual void glMaterialxv (GLenum face, GLenum pname, const GLfixed *params) = 0;
		virtual void glMatrixMode (GLenum mode) = 0;
		virtual void glMultMatrixf (const GLfloat *m) = 0;
		virtual void glMultMatrixx (const GLfixed *m) = 0;
		virtual void glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q) = 0;
		virtual void glMultiTexCoord4x (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q) = 0;
		virtual void glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz) = 0;
		virtual void glNormal3x (GLfixed nx, GLfixed ny, GLfixed nz) = 0;
		virtual void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar) = 0;
		virtual void glOrthox (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar) = 0;
		virtual void glPixelStorei (GLenum pname, GLint param) = 0;
		virtual void glPointSize (GLfloat size) = 0;
		virtual void glPointSizex (GLfixed size) = 0;
		virtual void glPolygonOffset (GLfloat factor, GLfloat units) = 0;
		virtual void glPolygonOffsetx (GLfixed factor, GLfixed units) = 0;
		virtual void glPopMatrix (void) = 0;
		virtual void glPushMatrix (void) = 0;
		virtual void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels) = 0;
		virtual void glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z) = 0;
		virtual void glRotatex (GLfixed angle, GLfixed x, GLfixed y, GLfixed z) = 0;
		virtual void glSampleCoverage (GLclampf value, GLboolean invert) = 0;
		virtual void glSampleCoveragex (GLclampx value, GLboolean invert) = 0;
		virtual void glScalef (GLfloat x, GLfloat y, GLfloat z) = 0;
		virtual void glScalex (GLfixed x, GLfixed y, GLfixed z) = 0;
		virtual void glScissor (GLint x, GLint y, GLsizei width, GLsizei height) = 0;
		virtual void glShadeModel (GLenum mode) = 0;
		virtual void glStencilFunc (GLenum func, GLint ref, GLuint mask) = 0;
		virtual void glStencilMask (GLuint mask) = 0;
		virtual void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass) = 0;
		virtual void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glTexEnvf (GLenum target, GLenum pname, GLfloat param) = 0;
		virtual void glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params) = 0;
		virtual void glTexEnvx (GLenum target, GLenum pname, GLfixed param) = 0;
		virtual void glTexEnvxv (GLenum target, GLenum pname, const GLfixed *params) = 0;
		virtual void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) = 0;
		virtual void glTexParameterf (GLenum target, GLenum pname, GLfloat param) = 0;
		virtual void glTexParameterx (GLenum target, GLenum pname, GLfixed param) = 0;
		virtual void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels) = 0;
		virtual void glTranslatef (GLfloat x, GLfloat y, GLfloat z) = 0;
		virtual void glTranslatex (GLfixed x, GLfixed y, GLfixed z) = 0;
		virtual void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glViewport (GLint x, GLint y, GLsizei width, GLsizei height) = 0;
		
		// OpenGL ES 1.1 functions
		virtual void glBindBuffer (GLenum target, GLuint buffer) = 0;
		virtual void glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage) = 0;
		virtual void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data) = 0;
		virtual void glClipPlanef(GLenum plane, const GLfloat *equation) = 0;
		virtual void glClipPlanex (GLenum plane, const GLfixed *equation) = 0;
		virtual void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha) = 0;
		virtual void glDeleteBuffers (GLsizei n, const GLuint *buffers) = 0;
		virtual void glGenBuffers (GLsizei n, GLuint *buffers) = 0;
		virtual void glGetClipPlanef(GLenum pname, GLfloat eqn[4]) = 0;
		virtual void glGetFloatv(GLenum pname, GLfloat *params) = 0;
		virtual void glGetLightfv (GLenum light, GLenum pname, GLfloat *params) = 0;
		virtual void glGetLightxv (GLenum light, GLenum pname, GLfixed *params) = 0;
		virtual void glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params) = 0;
		virtual void glGetMaterialxv (GLenum face, GLenum pname, GLfixed *params) = 0;
		virtual void glGetTexEnvfv (GLenum env, GLenum pname, GLfloat *params) = 0;
		virtual void glGetTexEnviv (GLenum env, GLenum pname, GLint *params) = 0;
		virtual void glGetTexEnvxv (GLenum env, GLenum pname, GLfixed *params) = 0;
		virtual void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params) = 0;
		virtual void glGetTexParameteriv (GLenum target, GLenum pname, GLint *params) = 0;
		virtual void glGetTexParameterxv (GLenum target, GLenum pname, GLfixed *params) = 0;
		virtual void glGetBooleanv (GLenum pname, GLboolean *params) = 0;
		virtual void glGetBufferParameteriv (GLenum target, GLenum pname, GLint *params) = 0;
		virtual void glGetFixedv (GLenum pname, GLfixed *params) = 0;
		virtual void glGetPointerv (GLenum pname, void **params) = 0;
		virtual GLboolean glIsBuffer (GLuint buffer) = 0;
		virtual GLboolean glIsEnabled (GLenum cap) = 0;
		virtual GLboolean glIsTexture (GLuint texture) = 0;
		virtual void glPointParameterf (GLenum pname, GLfloat param) = 0;
		virtual void glPointParameterfv (GLenum pname, const GLfloat *params) = 0;
		virtual void glPointParameterx (GLenum pname, GLfixed param) = 0;
		virtual void glPointParameterxv (GLenum pname, const GLfixed *params) = 0;
		virtual void glTexEnvi (GLenum target, GLenum pname, GLint param) = 0;
		virtual void glTexEnviv (GLenum target, GLenum pname, const GLint *params) = 0;
		virtual void glTexParameteri (GLenum target, GLenum pname, GLint param) = 0;
		virtual void glTexParameteriv (GLenum target, GLenum pname, const GLint *params) = 0;
		
		// OpenGL ES 1.1 extensions
		virtual void glCurrentPaletteMatrixOES (GLuint matrixpaletteindex) = 0;
		virtual void glLoadPaletteFromModelViewMatrixOES (void) = 0;
		virtual void glMatrixIndexPointerOES (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glWeightPointerOES (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glPointSizePointerOES (GLenum type, GLsizei stride, const GLvoid *pointer) = 0;
		virtual void glDrawTexsOES (GLshort x, GLshort y, GLshort z, GLshort width, GLshort height) = 0;
		virtual void glDrawTexiOES (GLint x, GLint y, GLint z, GLint width, GLint height) = 0;
		virtual void glDrawTexxOES (GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height) = 0;
		virtual void glDrawTexsvOES (const GLshort *coords) = 0;
		virtual void glDrawTexivOES (const GLint *coords) = 0;
		virtual void glDrawTexxvOES (const GLfixed *coords) = 0;
		virtual void glDrawTexfOES (GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height) = 0;
		virtual void glDrawTexfvOES (const GLfloat *coords) = 0;
		
		// OpenGL ES 2
		//virtual void glActiveTexture (GLenum texture); DUPLICATE
		virtual void glAttachShader (GLuint program, GLuint shader) = 0;
		virtual void glBindAttribLocation (GLuint program, GLuint index, const GLchar* name) = 0;
		//virtual void glBindBuffer (GLenum target, GLuint buffer); DUPLICATE
		virtual void glBindFramebuffer (GLenum target, GLuint framebuffer) = 0;
		virtual void glBindRenderbuffer (GLenum target, GLuint renderbuffer) = 0;
		//virtual void glBindTexture (GLenum target, GLuint texture); DUPLICATE
		virtual void glBlendColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) = 0;
		virtual void glBlendEquation ( GLenum mode ) = 0;
		virtual void glBlendEquationSeparate (GLenum modeRGB, GLenum modeAlpha) = 0;
		//virtual void glBlendFunc (GLenum sfactor, GLenum dfactor); DUPLICATE
		virtual void glBlendFuncSeparate (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) = 0;
		//virtual void glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage); DUPLICATE
		//virtual void glBufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data); DUPLICATE
		virtual GLenum glCheckFramebufferStatus (GLenum target) = 0;
		//virtual void glClear (GLbitfield mask); DUPLICATE
		//virtual void glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha); DUPLICATE
		//virtual void glClearDepthf (GLclampf depth); DUPLICATE
		//virtual void glClearStencil (GLint s); DUPLICATE
		//virtual void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha); DUPLICATE
		virtual void glCompileShader (GLuint shader) = 0;
		//virtual void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid* data); DUPLICATE
		//virtual void glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data); DUPLICATE
		//virtual void glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border); DUPLICATE
		//virtual void glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height); DUPLICATE
		virtual GLuint glCreateProgram (void) = 0;
		virtual GLuint glCreateShader (GLenum type) = 0;
		//virtual void glCullFace (GLenum mode); DUPLICATE
		//virtual void glDeleteBuffers (GLsizei n, const GLuint* buffers); DUPLICATE
		virtual void glDeleteFramebuffers (GLsizei n, const GLuint* framebuffers) = 0;
		virtual void glDeleteProgram (GLuint program) = 0;
		virtual void glDeleteRenderbuffers (GLsizei n, const GLuint* renderbuffers) = 0;
		virtual void glDeleteShader (GLuint shader) = 0;
		//virtual void glDeleteTextures (GLsizei n, const GLuint* textures); DUPLICATE
		//virtual void glDepthFunc (GLenum func); DUPLICATE
		//virtual void glDepthMask (GLboolean flag); DUPLICATE
		//virtual void glDepthRangef (GLclampf zNear, GLclampf zFar); DUPLICATE
		virtual void glDetachShader (GLuint program, GLuint shader) = 0;
		//virtual void glDisable (GLenum cap); DUPLICATE
		virtual void glDisableVertexAttribArray (GLuint index) = 0;
		//virtual void glDrawArrays (GLenum mode, GLint first, GLsizei count); DUPLICATE
		//virtual void glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices); DUPLICATE
		//virtual void glEnable (GLenum cap); DUPLICATE
		virtual void glEnableVertexAttribArray (GLuint index) = 0;
		//virtual void glFinish (void); DUPLICATE
		//virtual void glFlush (void); DUPLICATE
		virtual void glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) = 0;
		virtual void glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) = 0;
		//virtual void glFrontFace (GLenum mode); DUPLICATE
		//virtual void glGenBuffers (GLsizei n, GLuint* buffers); DUPLICATE
		virtual void glGenerateMipmap (GLenum target) = 0;
		virtual void glGenFramebuffers (GLsizei n, GLuint* framebuffers) = 0;
		virtual void glGenRenderbuffers (GLsizei n, GLuint* renderbuffers) = 0;
		//virtual void glGenTextures (GLsizei n, GLuint* textures); DUPLICATE
		virtual void glGetActiveAttrib (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) = 0;
		virtual void glGetActiveUniform (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, GLchar* name) = 0;
		virtual void glGetAttachedShaders (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders) = 0;
		virtual int glGetAttribLocation (GLuint program, const GLchar* name) = 0;
		//virtual void glGetBooleanv (GLenum pname, GLboolean* params); DUPLICATE
		//virtual void glGetBufferParameteriv (GLenum target, GLenum pname, GLint* params); DUPLICATE
		//virtual GLenum glGetError (void); DUPLICATE
		//virtual void glGetFloatv (GLenum pname, GLfloat* params); DUPLICATE
		virtual void glGetFramebufferAttachmentParameteriv (GLenum target, GLenum attachment, GLenum pname, GLint* params) = 0;
		//virtual void glGetIntegerv (GLenum pname, GLint* params); DUPLICATE
		virtual void glGetProgramiv (GLuint program, GLenum pname, GLint* params) = 0;
		virtual void glGetProgramInfoLog (GLuint program, GLsizei bufsize, GLsizei* length, GLchar* infolog) = 0;
		virtual void glGetRenderbufferParameteriv (GLenum target, GLenum pname, GLint* params) = 0;
		virtual void glGetShaderiv (GLuint shader, GLenum pname, GLint* params) = 0;
		virtual void glGetShaderInfoLog (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* infolog) = 0;
		virtual void glGetShaderPrecisionFormat (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision) = 0;
		virtual void glGetShaderSource (GLuint shader, GLsizei bufsize, GLsizei* length, GLchar* source) = 0;
		//virtual const GLubyte* glGetString (GLenum name); DUPLICATE
		//virtual void glGetTexParameterfv (GLenum target, GLenum pname, GLfloat* params); DUPLICATE
		//virtual void glGetTexParameteriv (GLenum target, GLenum pname, GLint* params); DUPLICATE
		virtual void glGetUniformfv (GLuint program, GLint location, GLfloat* params) = 0;
		virtual void glGetUniformiv (GLuint program, GLint location, GLint* params) = 0;
		virtual int glGetUniformLocation (GLuint program, const GLchar* name) = 0;
		virtual void glGetVertexAttribfv (GLuint index, GLenum pname, GLfloat* params) = 0;
		virtual void glGetVertexAttribiv (GLuint index, GLenum pname, GLint* params) = 0;
		virtual void glGetVertexAttribPointerv (GLuint index, GLenum pname, GLvoid** pointer) = 0;
		//virtual void glHint (GLenum target, GLenum mode); DUPLICATE
		//virtual GLboolean glIsBuffer (GLuint buffer); DUPLICATE
		//virtual GLboolean glIsEnabled (GLenum cap); DUPLICATE
		virtual GLboolean glIsFramebuffer (GLuint framebuffer) = 0;
		virtual GLboolean glIsProgram (GLuint program) = 0;
		virtual GLboolean glIsRenderbuffer (GLuint renderbuffer) = 0;
		virtual GLboolean glIsShader (GLuint shader) = 0;
		//virtual GLboolean glIsTexture (GLuint texture); DUPLICATE
		//virtual void glLineWidth (GLfloat width); DUPLICATE
		virtual void glLinkProgram (GLuint program) = 0;
		//virtual void glPixelStorei (GLenum pname, GLint param); DUPLICATE
		//virtual void glPolygonOffset (GLfloat factor, GLfloat units); DUPLICATE
		//virtual void glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels); DUPLICATE
		virtual void glReleaseShaderCompiler (void) = 0;
		virtual void glRenderbufferStorage (GLenum target, GLenum internalformat, GLsizei width, GLsizei height) = 0;
		//virtual void glSampleCoverage (GLclampf value, GLboolean invert); DUPLICATE
		//virtual void glScissor (GLint x, GLint y, GLsizei width, GLsizei height); DUPLICATE
		virtual void glShaderBinary (GLsizei n, const GLuint* shaders, GLenum binaryformat, const GLvoid* binary, GLsizei length) = 0;
		virtual void glShaderSource (GLuint shader, GLsizei count, const GLchar** string, const GLint* length) = 0;
		//virtual void glStencilFunc (GLenum func, GLint ref, GLuint mask); DUPLICATE
		virtual void glStencilFuncSeparate (GLenum face, GLenum func, GLint ref, GLuint mask) = 0;
		//virtual void glStencilMask (GLuint mask); DUPLICATE
		virtual void glStencilMaskSeparate (GLenum face, GLuint mask) = 0;
		//virtual void glStencilOp (GLenum fail, GLenum zfail, GLenum zpass); DUPLICATE
		virtual void glStencilOpSeparate (GLenum face, GLenum fail, GLenum zfail, GLenum zpass) = 0;
		//virtual void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels); DUPLICATE
		//virtual void glTexParameterf (GLenum target, GLenum pname, GLfloat param); DUPLICATE
		virtual void glTexParameterfv (GLenum target, GLenum pname, const GLfloat* params) = 0;
		//virtual void glTexParameteri (GLenum target, GLenum pname, GLint param); DUPLICATE
		//virtual void glTexParameteriv (GLenum target, GLenum pname, const GLint* params); DUPLICATE
		//virtual void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels); DUPLICATE
		virtual void glUniform1f (GLint location, GLfloat x) = 0;
		virtual void glUniform1fv (GLint location, GLsizei count, const GLfloat* v) = 0;
		virtual void glUniform1i (GLint location, GLint x) = 0;
		virtual void glUniform1iv (GLint location, GLsizei count, const GLint* v) = 0;
		virtual void glUniform2f (GLint location, GLfloat x, GLfloat y) = 0;
		virtual void glUniform2fv (GLint location, GLsizei count, const GLfloat* v) = 0;
		virtual void glUniform2i (GLint location, GLint x, GLint y) = 0;
		virtual void glUniform2iv (GLint location, GLsizei count, const GLint* v) = 0;
		virtual void glUniform3f (GLint location, GLfloat x, GLfloat y, GLfloat z) = 0;
		virtual void glUniform3fv (GLint location, GLsizei count, const GLfloat* v) = 0;
		virtual void glUniform3i (GLint location, GLint x, GLint y, GLint z) = 0;
		virtual void glUniform3iv (GLint location, GLsizei count, const GLint* v) = 0;
		virtual void glUniform4f (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w) = 0;
		virtual void glUniform4fv (GLint location, GLsizei count, const GLfloat* v) = 0;
		virtual void glUniform4i (GLint location, GLint x, GLint y, GLint z, GLint w) = 0;
		virtual void glUniform4iv (GLint location, GLsizei count, const GLint* v) = 0;
		virtual void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) = 0;
		virtual void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) = 0;
		virtual void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) = 0;
		virtual void glUseProgram (GLuint program) = 0;
		virtual void glValidateProgram (GLuint program) = 0;
		virtual void glVertexAttrib1f (GLuint indx, GLfloat x) = 0;
		virtual void glVertexAttrib1fv (GLuint indx, const GLfloat* values) = 0;
		virtual void glVertexAttrib2f (GLuint indx, GLfloat x, GLfloat y) = 0;
		virtual void glVertexAttrib2fv (GLuint indx, const GLfloat* values) = 0;
		virtual void glVertexAttrib3f (GLuint indx, GLfloat x, GLfloat y, GLfloat z) = 0;
		virtual void glVertexAttrib3fv (GLuint indx, const GLfloat* values) = 0;
		virtual void glVertexAttrib4f (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w) = 0;
		virtual void glVertexAttrib4fv (GLuint indx, const GLfloat* values) = 0;
		virtual void glVertexAttribPointer (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* ptr) = 0;
		//virtual void glViewport (GLint x, GLint y, GLsizei width, GLsizei height); DUPLICATE
		
		// OpenGL ES 2 Extensions
		virtual void glGetBufferPointervOES (GLenum target, GLenum pname, GLvoid **params) = 0;
		virtual GLvoid * glMapBufferOES (GLenum target, GLenum access) = 0;
		virtual GLboolean glUnmapBufferOES (GLenum target) = 0;
		
		virtual GLfloat* getModelViewMatrix()=0;
		virtual GLfloat* getProjectionMatrix()=0;
		//virtual GLfloat* getTextureMatrix(int index)=0;
		
		ofShader* overrideShader;
			
		
	protected:
		
		GLfloat tmpModelViewMatrix[16];
		GLfloat tmpProjectionMatrix[16];
		//GLfloat tmpTextureMatrix[16];		
		
		OpenGLESImplementation *implementation;
		int version;
	};
	
}

#endif
