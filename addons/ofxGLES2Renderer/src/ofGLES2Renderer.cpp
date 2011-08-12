
#include "ofGLES2Renderer.h"
#include "ofMesh.h"
#include "ofPath.h"
#include "ofGraphics.h"
#include "ofAppRunner.h"
#include "ofMesh.h"
#include "ofTexture.h"
#include "ofBitmapFont.h"
#include "ofGLUtils.h"
#include "ofImage.h"
#include "ofFbo.h"

using namespace OpenGLES;

/*
static bool		bBitmapTexturePrepared = false;
ofTexture		glesBitmappedFontTexture;

static vector <GLfloat> coords;
static vector <GLfloat> verts;

static int vC = 0;
*/
 
//----------------------------------------------------------
ofGLES2Renderer::ofGLES2Renderer(int _glVersion){

	bBackgroundAuto = true;

	ofLogNotice() << "ofGLES2Renderer::ofGLES2Renderer";
	
	// We can make gles2-bc work in OpenGl 1.1 mode as well, could come in handy for debugging.
	if( _glVersion == 1 )
	{
		ofLogNotice() << "ofGLES2Renderer::ofGLES2Renderer  OpenGLES1::OpenGLES11Context() ";		
		gl = new OpenGLES1::OpenGLES11Context();
	}
	else if ( _glVersion == 2 )
	{
		ofLogNotice() << "ofGLES2Renderer::ofGLES2Renderer  OpenGLES2::OpenGLES20Context() ";				
		gl = new OpenGLES2::OpenGLES20Context();
	}
	else
	{
		ofLogError() << "ofGLES2Renderer::ofGLES2Renderer  GL version requested not recognised ";		
		gl = new OpenGLES1::OpenGLES11Context();
	}
	
	linePoints.resize(2);
	rectPoints.resize(4);
	triPoints.resize(3);

	currentFbo = NULL;
}

//----------------------------------------------------------
void ofGLES2Renderer::update(){

}

//----------------------------------------------------------
void ofGLES2Renderer::beginCustomShader( ofShader* _shader ){
	gl->overrideShader = _shader->getProgram();
}


//----------------------------------------------------------
void ofGLES2Renderer::endCustomShader(){
	gl->overrideShader = NULL;
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofMesh & vertexData){
	
	// add a color for each vertex if there are no colors in the incoming mesh
	if( vertexData.getNumColors() == 0 )
	{
		for( int i = 0; i < vertexData.getNumVertices(); i++ )
		{
			vertexData.addColor( currentColor );
		}
	}
	
	
	if(vertexData.getNumVertices()){
		 gl->glEnableClientState(GL_VERTEX_ARRAY);
		 gl->glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &vertexData.getVerticesPointer()->x);
	}
	if(vertexData.getNumNormals()){
		 gl->glEnableClientState(GL_NORMAL_ARRAY);
		 gl->glNormalPointer(GL_FLOAT, sizeof(ofVec3f), &vertexData.getNormalsPointer()->x);
	}
	if(vertexData.getNumColors()){
		 gl->glEnableClientState(GL_COLOR_ARRAY);
		 gl->glColorPointer(4,GL_FLOAT, sizeof(ofFloatColor), &vertexData.getColorsPointer()->r);
	}

	if(vertexData.getNumTexCoords()){
		 gl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		 gl->glTexCoordPointer(2, GL_FLOAT, sizeof(ofVec2f), &vertexData.getTexCoordsPointer()->x);
	}

	if(vertexData.getNumIndices()){
#ifdef TARGET_OPENGLES
		 gl->glDrawElements(ofGetGLPrimitiveMode(vertexData.getMode()), vertexData.getNumIndices(),GL_UNSIGNED_SHORT,vertexData.getIndexPointer());
#else
		 gl->glDrawElements(ofGetGLPrimitiveMode(vertexData.getMode()), vertexData.getNumIndices(),GL_UNSIGNED_INT,vertexData.getIndexPointer());
#endif
	}else{
		 gl->glDrawArrays(ofGetGLPrimitiveMode(vertexData.getMode()), 0, vertexData.getNumVertices());
	}
	if(vertexData.getNumColors()){
		 gl->glDisableClientState(GL_COLOR_ARRAY);
	}
	if(vertexData.getNumNormals()){
		 gl->glDisableClientState(GL_NORMAL_ARRAY);
	}
	if(vertexData.getNumTexCoords()){
		 gl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofMesh & vertexData, ofPolyRenderMode renderType)
{	
		// add a color for each vertex if there are no colors in the incoming mesh
		if( vertexData.getNumColors() == 0 )
		{
			for( int i = 0; i < vertexData.getNumVertices(); i++ )
			{
				vertexData.addColor( currentColor );
			}
		}		
	
	
#ifndef TARGET_OPENGLES
		 gl->glPushAttrib(GL_POLYGON_BIT);
		 gl->glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(renderType));
		draw(vertexData);
		 gl->glPopAttrib(); //TODO: GLES doesnt support polygon mode, add renderType to  gl->gl renderer?
#else
		if(vertexData.getNumVertices()){
			 gl->glEnableClientState(GL_VERTEX_ARRAY);
			 gl->glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), vertexData.getVerticesPointer());
		}
		if(vertexData.getNumNormals()){
			 gl->glEnableClientState(GL_NORMAL_ARRAY);
			 gl->glNormalPointer(GL_FLOAT, 0, vertexData.getNormalsPointer());
		}
		if(vertexData.getNumColors()){
			 gl->glEnableClientState(GL_COLOR_ARRAY);
			 gl->glColorPointer(4,GL_FLOAT, sizeof(ofFloatColor), vertexData.getColorsPointer());
		}

		if(vertexData.getNumTexCoords()){
			 gl->glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			 gl->glTexCoordPointer(2, GL_FLOAT, 0, vertexData.getTexCoordsPointer());
		}

		GLenum drawMode;
		switch(renderType){
		case OF_MESH_POINTS:
			drawMode = GL_POINTS;
			break;
		case OF_MESH_WIREFRAME:
			drawMode = GL_LINES;
			break;
		case OF_MESH_FILL:
			drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
			break;
		default:
			drawMode = ofGetGLPrimitiveMode(vertexData.getMode());
			break;
		}

		if(vertexData.getNumIndices()){
			 gl->glDrawElements(drawMode, vertexData.getNumIndices(),GL_UNSIGNED_SHORT,vertexData.getIndexPointer());
		}else{
			 gl->glDrawArrays(drawMode, 0, vertexData.getNumVertices());
		}
		if(vertexData.getNumColors()){
			 gl->glDisableClientState(GL_COLOR_ARRAY);
		}
		if(vertexData.getNumNormals()){
			 gl->glDisableClientState(GL_NORMAL_ARRAY);
		}
		if(vertexData.getNumTexCoords()){
			 gl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
#endif

}

//----------------------------------------------------------
void ofGLES2Renderer::draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){
	if(!vertexData.empty()) {
		 gl->glEnableClientState(GL_VERTEX_ARRAY);
		 gl->glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &vertexData[0].x);
		 gl->glDrawArrays(ofGetGLPrimitiveMode(drawMode), 0, vertexData.size());
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofPolyline & poly){
	if(!poly.getVertices().empty()) {
		// use smoothness, if requested:
		if (bSmoothHinted) startSmoothing();

		 gl->glEnableClientState(GL_VERTEX_ARRAY);
		 gl->glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &poly.getVertices()[0].x);
		 gl->glDrawArrays(poly.isClosed()?GL_LINE_LOOP:GL_LINE_STRIP, 0, poly.size());

		// use smoothness, if requested:
		if (bSmoothHinted) endSmoothing();
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofPath & shape){
	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = ofGetStyle().color;
	}
	if(shape.isFilled()){
		ofMesh & mesh = shape.getTessellation();
		if(shape.getUseShapeColor()){
			setColor( shape.getFillColor() * ofGetStyle().color,shape.getFillColor().a/255. * ofGetStyle().color.a);
		}
		draw(mesh);
	}
	if(shape.hasOutline()){
		float lineWidth = ofGetStyle().lineWidth;
		if(shape.getUseShapeColor()){
			setColor( shape.getStrokeColor() * ofGetStyle().color, shape.getStrokeColor().a/255. * ofGetStyle().color.a);
		}
		setLineWidth( shape.getStrokeWidth() );
		vector<ofPolyline> & outlines = shape.getOutline();
		for(int i=0; i<(int)outlines.size(); i++)
			draw(outlines[i]);
		setLineWidth(lineWidth);
	}
	if(shape.getUseShapeColor()){
		setColor(prevColor);
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofImage & image, float x, float y, float z, float w, float h){
	if(image.isUsingTexture()){
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			
			ofTextureData texData = tex.getTextureData();
			
			//tex.draw(x,y,z,w,h);
			
			// -------- largely taken from from ofTexture::draw ------------
			
			gl->glActiveTexture(GL_TEXTURE0);
			
			// Enable texturing
			gl->glEnable(texData.textureTarget);
			
			// bind the texture
			gl->glBindTexture( texData.textureTarget, (GLuint)texData.textureID );
			
			GLfloat px0 = 0.0f;		// up to you to get the aspect ratio right
			GLfloat py0 = 0.0f;
			GLfloat px1 = w;
			GLfloat py1 = h;
			
			if (texData.bFlipTexture == true){
				GLint temp = (GLint)py0;
				py0 = py1;
				py1 = temp;
			}
			
			// for rect mode center, let's do this:
			if (ofGetRectMode() == OF_RECTMODE_CENTER){
				px0 = -w/2;
				py0 = -h/2;
				px1 = +w/2;
				py1 = +h/2;
			}

			//we translate our drawing points by our anchor point.
			//we still respect ofRectMode so if you have rect mode set to
			//OF_RECTMODE_CENTER your anchor will be relative to that.
			
			GLfloat anchorX = 0.0f;
			GLfloat anchorY = 0.0f;
			
			
			// Andreas: TODO, support anchor
			
			/*
			if(bAnchorIsPct){
				anchorX = anchor.x * w;
				anchorY = anchor.y * h;
			}else{
				anchorX = anchor.x;
				anchorY = anchor.y;
			}
			*/
			
			px0 -= anchorX;
			py0 -= anchorY;
			px1 -= anchorX;
			py1 -= anchorY;

			// -------------------------------------------------
			// complete hack to remove border artifacts.
			// slightly, slightly alters an image, scaling...
			// to remove the border.
			// we need a better solution for this, but
			// to constantly add a 2 pixel border on all uploaded images
			// is insane..
			
			GLfloat offsetw = 0.0f;
			GLfloat offseth = 0.0f;
			
			if (texData.textureTarget == GL_TEXTURE_2D && ofGetTextureEdgeHackEnabled() ) {
				offsetw = 1.0f / (texData.tex_w);
				offseth = 1.0f / (texData.tex_h);
			}
			// -------------------------------------------------
			
			GLfloat tx0 = 0+offsetw;
			GLfloat ty0 = 0+offseth;
			GLfloat tx1 = texData.tex_t - offsetw;
			GLfloat ty1 = texData.tex_u - offseth;
			
			ofPushMatrix(); 
			
			ofTranslate(x,y,z);
			
			scratchMesh.clear();
			
			scratchMesh.addColor( currentColor );
			scratchMesh.addTexCoord( ofVec2f(tx0,ty0) );
			scratchMesh.addVertex( ofVec3f(px0,py0, 0.0f));

			scratchMesh.addColor( currentColor );			
			scratchMesh.addTexCoord( ofVec2f(tx1,ty0) );
			scratchMesh.addVertex( ofVec3f(px1,py0, 0.0f));
			
			scratchMesh.addColor( currentColor );			
			scratchMesh.addTexCoord( ofVec2f(tx1,ty1) );
			scratchMesh.addVertex( ofVec3f(px1,py1, 0.0f));			
			
			scratchMesh.addColor( currentColor );			
			scratchMesh.addTexCoord( ofVec2f(tx0,ty1) );			
			scratchMesh.addVertex( ofVec3f(px0,py1, 0.0f));			
		
			scratchMesh.setMode( OF_PRIMITIVE_TRIANGLE_FAN );
			
			draw( scratchMesh );
			
			ofPopMatrix();
			gl->glDisable(texData.textureTarget);			
			
		} else {
			ofLogWarning() << "ofGLES2Renderer::draw(): texture is not allocated";
		}
	}
}



//----------------------------------------------------------
void ofGLES2Renderer::draw(ofFloatImage & image, float x, float y, float z, float w, float h){
	if(image.isUsingTexture()){
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.draw(x,y,z,w,h);
		} else {
			ofLogWarning() << "ofGLES2Renderer::draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::draw(ofShortImage & image, float x, float y, float z, float w, float h){
	if(image.isUsingTexture()){
		ofTexture& tex = image.getTextureReference();
		if(tex.bAllocated()) {
			tex.draw(x,y,z,w,h);
		} else {
			ofLogWarning() << "ofGLES2Renderer::draw(): texture is not allocated";
		}
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::setCurrentFBO(ofFbo * fbo){
	currentFbo = fbo;
}

//----------------------------------------------------------
void ofGLES2Renderer::pushView() {
	GLint viewport[4];
	 gl->glGetIntegerv(GL_VIEWPORT, viewport);

	ofRectangle currentViewport;
	currentViewport.set(viewport[0], viewport[1], viewport[2], viewport[3]);
	viewportHistory.push(currentViewport);


	 gl->glMatrixMode(GL_PROJECTION);
	 gl->glPushMatrix();
	 gl->glMatrixMode(GL_MODELVIEW);
	 gl->glPushMatrix();
}


//----------------------------------------------------------
void ofGLES2Renderer::popView() {
	if( viewportHistory.size() ){
		ofRectangle viewRect = viewportHistory.top();
		viewport(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
		viewportHistory.pop();
	}

	 gl->glMatrixMode(GL_PROJECTION);
	 gl->glPopMatrix();
	 gl->glMatrixMode(GL_MODELVIEW);
	 gl->glPopMatrix();
}

//----------------------------------------------------------
void ofGLES2Renderer::viewport(ofRectangle viewport_){
	viewport(viewport_.x, viewport_.y, viewport_.width, viewport_.height,true);
}

//----------------------------------------------------------
void ofGLES2Renderer::viewport(float x, float y, float width, float height, bool invertY) {
	if(width == 0) width = ofGetWindowWidth();
	if(height == 0) height = ofGetWindowHeight();

	if (invertY){
		if(currentFbo){
			y = currentFbo->getHeight() - (y + height);
		}else{
			y = ofGetWindowHeight() - (y + height);
		}
	}
	 gl->glViewport(x, y, width, height);	
}

//----------------------------------------------------------
ofRectangle ofGLES2Renderer::getCurrentViewport(){

	// I am using opengl calls here instead of returning viewportRect
	// since someone might use  gl->glViewport instead of ofViewport...

	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	 gl->glGetIntegerv(GL_VIEWPORT, viewport);
	ofRectangle view;
	view.x = viewport[0];
	view.y = viewport[1];
	view.width = viewport[2];
	view.height = viewport[3];
	return view;

}

//----------------------------------------------------------
int ofGLES2Renderer::getViewportWidth(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	 gl->glGetIntegerv(GL_VIEWPORT, viewport);
	return viewport[2];
}

//----------------------------------------------------------
int ofGLES2Renderer::getViewportHeight(){
	GLint viewport[4];					// Where The Viewport Values Will Be Stored
	 gl->glGetIntegerv(GL_VIEWPORT, viewport);
	return viewport[3];
}

//----------------------------------------------------------
void ofGLES2Renderer::setCoordHandedness(ofHandednessType handedness) {
	coordHandedness = handedness;
}

//----------------------------------------------------------
ofHandednessType ofGLES2Renderer::getCoordHandedness() {
	return coordHandedness;
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreenPerspective(float width, float height, ofOrientation orientation, bool vFlip, float fov, float nearDist, float farDist) {
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	 gl->glMatrixMode(GL_PROJECTION);
	 gl->glLoadIdentity();
	 //gl->gluPerspective(fov, aspect, nearDist, farDist);	
	 perspectiveMatrix.makePerspectiveMatrix(fov, aspect, nearDist, farDist);
	 gl->glMultMatrixf( perspectiveMatrix.getPtr() );


	 gl->glMatrixMode(GL_MODELVIEW);
	 gl->glLoadIdentity();
	//gl->gluLookAt(eyeX, eyeY, dist, eyeX, eyeY, 0, 0, 1, 0);
	lookAtMatrix.makeLookAtViewMatrix( ofVec3f(eyeX, eyeY, dist), ofVec3f(eyeX, eyeY, 0), ofVec3f(0, 1, 0) );
	gl->glMultMatrixf( lookAtMatrix.getPtr() );
	
	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	if(ofDoesHWOrientation()){
		if(vFlip){
			 gl->glScalef(1, -1, 1);
			 gl->glTranslatef(0, -height, 0);
		}
	}else{
		if( orientation == OF_ORIENTATION_UNKNOWN ) orientation = ofGetOrientation();
		switch(orientation) {
			case OF_ORIENTATION_180:
				 gl->glRotatef(-180, 0, 0, 1);
				if(vFlip){
					 gl->glScalef(1, -1, 1);
					 gl->glTranslatef(-width, 0, 0);
				}else{
					 gl->glTranslatef(-width, -height, 0);
				}

				break;

			case OF_ORIENTATION_90_RIGHT:
				 gl->glRotatef(-90, 0, 0, 1);
				if(vFlip){
					 gl->glScalef(-1, 1, 1);
				}else{
					 gl->glScalef(-1, -1, 1);
					 gl->glTranslatef(0, -height, 0);
				}
				break;

			case OF_ORIENTATION_90_LEFT:
				 gl->glRotatef(90, 0, 0, 1);
				if(vFlip){
					 gl->glScalef(-1, 1, 1);
					 gl->glTranslatef(-width, -height, 0);
				}else{
					 gl->glScalef(-1, -1, 1);
					 gl->glTranslatef(-width, 0, 0);
				}
				break;

			case OF_ORIENTATION_DEFAULT:
			default:
				if(vFlip){
					 gl->glScalef(1, -1, 1);
					 gl->glTranslatef(0, -height, 0);
				}
				break;
		}
	}

}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreenOrtho(float width, float height, ofOrientation orientation, bool vFlip, float nearDist, float farDist) {
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
	
	float viewW = ofGetViewportWidth();
	float viewH = ofGetViewportHeight();
	
	 gl->glMatrixMode(GL_PROJECTION);
	 gl->glLoadIdentity();

	ofSetCoordHandedness(OF_RIGHT_HANDED);
#ifndef TARGET_OPENGLES
	if(vFlip) {
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}
	
	 gl->glOrtho(0, viewW, 0, viewH, nearDist, farDist);

#else
	if(vFlip) {
		ofMatrix4x4 ortho = ofMatrix4x4::newOrthoMatrix(0, width, height, 0, nearDist, farDist);
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}
	
	ofMatrix4x4 ortho = ofMatrix4x4::newOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);
	 gl->glMultMatrixf(ortho.getPtr());	
#endif

	 gl->glMatrixMode(GL_MODELVIEW);
	 gl->glLoadIdentity();

	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	if(ofDoesHWOrientation()){
		if(vFlip){
			 gl->glScalef(1, -1, 1);
			 gl->glTranslatef(0, -height, 0);
		}
	}else{
		if( orientation == OF_ORIENTATION_UNKNOWN ) orientation = ofGetOrientation();
		switch(orientation) {
			case OF_ORIENTATION_180:
				 gl->glRotatef(-180, 0, 0, 1);
				if(vFlip){
					 gl->glScalef(1, -1, 1);
					 gl->glTranslatef(-width, 0, 0);
				}else{
					 gl->glTranslatef(-width, -height, 0);
				}

				break;

			case OF_ORIENTATION_90_RIGHT:
				 gl->glRotatef(-90, 0, 0, 1);
				if(vFlip){
					 gl->glScalef(-1, 1, 1);
				}else{
					 gl->glScalef(-1, -1, 1);
					 gl->glTranslatef(0, -height, 0);
				}
				break;

			case OF_ORIENTATION_90_LEFT:
				 gl->glRotatef(90, 0, 0, 1);
				if(vFlip){
					 gl->glScalef(-1, 1, 1);
					 gl->glTranslatef(-width, -height, 0);
				}else{
					 gl->glScalef(-1, -1, 1);
					 gl->glTranslatef(-width, 0, 0);
				}
				break;

			case OF_ORIENTATION_DEFAULT:
			default:
				if(vFlip){
					 gl->glScalef(1, -1, 1);
					 gl->glTranslatef(0, -height, 0);
				}
				break;
		}
	}

}

//----------------------------------------------------------
//Resets openGL parameters back to OF defaults
void ofGLES2Renderer::setupGraphicDefaults(){

	 gl->glEnableClientState(GL_VERTEX_ARRAY);
	 gl->glDisableClientState(GL_NORMAL_ARRAY);
	 gl->glDisableClientState(GL_COLOR_ARRAY);
	 gl->glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//----------------------------------------------------------
void ofGLES2Renderer::setupScreen(){
	setupScreenPerspective();	// assume defaults
}

//----------------------------------------------------------
void ofGLES2Renderer::setCircleResolution(int res){
	if((int)circlePolyline.size()!=res+1){
		circlePolyline.clear();
		circlePolyline.arc(0,0,0,1,1,0,360,res);
		circlePoints.resize(circlePolyline.size());
	}
}

//our openGL wrappers
//----------------------------------------------------------
void ofGLES2Renderer::pushMatrix(){
	 gl->glPushMatrix();
}

//----------------------------------------------------------
void ofGLES2Renderer::popMatrix(){
	 gl->glPopMatrix();
}

//----------------------------------------------------------
void ofGLES2Renderer::translate(const ofPoint& p){
	 gl->glTranslatef(p.x, p.y, p.z);
}


//----------------------------------------------------------
void ofGLES2Renderer::translate(float x, float y, float z){
	 gl->glTranslatef(x, y, z);
}

//----------------------------------------------------------
void ofGLES2Renderer::scale(float xAmnt, float yAmnt, float zAmnt){
	 gl->glScalef(xAmnt, yAmnt, zAmnt);
}

//----------------------------------------------------------
void ofGLES2Renderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	 gl->glRotatef(degrees, vecX, vecY, vecZ);
}

//----------------------------------------------------------
void ofGLES2Renderer::rotateX(float degrees){
	 gl->glRotatef(degrees, 1, 0, 0);
}

//----------------------------------------------------------
void ofGLES2Renderer::rotateY(float degrees){
	 gl->glRotatef(degrees, 0, 1, 0);
}

//----------------------------------------------------------
void ofGLES2Renderer::rotateZ(float degrees){
	 gl->glRotatef(degrees, 0, 0, 1);
}

//same as ofRotateZ
//----------------------------------------------------------
void ofGLES2Renderer::rotate(float degrees){
	 gl->glRotatef(degrees, 0, 0, 1);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(const ofColor & color){
	setColor(color.r,color.g,color.b,color.a);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(const ofColor & color, int _a){
	setColor(color.r,color.g,color.b,_a);
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(int _r, int _g, int _b){
	//gl->glColor4f(_r/255.f,_g/255.f,_b/255.f,1.f);
	currentColor.set(_r/255.f,_g/255.f,_b/255.f,1.f); // store the color to add to the vertex buffers later
}


//----------------------------------------------------------
void ofGLES2Renderer::setColor(int _r, int _g, int _b, int _a){
	//gl->glColor4f(_r/255.f,_g/255.f,_b/255.f,_a/255.f);
	currentColor.set(_r/255.f,_g/255.f,_b/255.f,_a/255.f); // store the color to add to the vertex buffers later
}

//----------------------------------------------------------
void ofGLES2Renderer::setColor(int gray){
	setColor(gray, gray, gray);
}

//----------------------------------------------------------
void ofGLES2Renderer::setHexColor(int hexColor){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

//----------------------------------------------------------
void ofGLES2Renderer::clear(float r, float g, float b, float a) {
	 gl->glClearColor(r / 255., g / 255., b / 255., a / 255.);
	 gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------
void ofGLES2Renderer::clear(float brightness, float a) {
	clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofGLES2Renderer::clearAlpha() {
	 gl->glColorMask(0, 0, 0, 1);
	 gl->glClearColor(0, 0, 0, 1);
	 gl->glClear(GL_COLOR_BUFFER_BIT);
	 gl->glColorMask(1, 1, 1, 1);
}

//----------------------------------------------------------
void ofGLES2Renderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofGLES2Renderer::bClearBg(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor & ofGLES2Renderer::getBgColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofGLES2Renderer::background(const ofColor & c){
	bgColor = c;
	// if we are in not-auto mode, then clear with a bg call...
	if (bClearBg() == false){
		 gl->glClearColor(bgColor[0],bgColor[1],bgColor[2], bgColor[3]);
		 gl->glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::background(float brightness) {
	background(brightness);
}

//----------------------------------------------------------
void ofGLES2Renderer::background(int hexColor, float _a){
	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofGLES2Renderer::background(int r, int g, int b, int a){
	background(ofColor(r,g,b,a));
}

//----------------------------------------------------------
void ofGLES2Renderer::setFillMode(ofFillFlag fill){
	bFilled = fill;
}

//----------------------------------------------------------
ofFillFlag ofGLES2Renderer::getFillMode(){
	return bFilled;
}

//----------------------------------------------------------
void ofGLES2Renderer::setRectMode(ofRectMode mode){
	rectMode = mode;
}

//----------------------------------------------------------
ofRectMode ofGLES2Renderer::getRectMode(){
	return rectMode;
}

//----------------------------------------------------------
void ofGLES2Renderer::setLineWidth(float lineWidth){
	 gl->glLineWidth(lineWidth);
}

//----------------------------------------------------------
void ofGLES2Renderer::setLineSmoothing(bool smooth){
	bSmoothHinted = smooth;
}


//----------------------------------------------------------
void ofGLES2Renderer::startSmoothing(){
	#ifndef TARGET_OPENGLES
		 gl->glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
	#endif

	 gl->glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	 gl->glEnable(GL_LINE_SMOOTH);

	//why do we need this?
	 gl->glEnable(GL_BLEND);
	 gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


//----------------------------------------------------------
void ofGLES2Renderer::endSmoothing(){
	#ifndef TARGET_OPENGLES
		 gl->glPopAttrib();
	#endif
}

//----------------------------------------------------------
void ofGLES2Renderer::setBlendMode(ofBlendMode blendMode){
	switch (blendMode){

		case OF_BLENDMODE_ALPHA:{
			 gl->glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				 gl->glBlendEquation(GL_FUNC_ADD);
			#endif
			 gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}

		case OF_BLENDMODE_ADD:{
			 gl->glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				 gl->glBlendEquation(GL_FUNC_ADD);
			#endif
			 gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}

		case OF_BLENDMODE_MULTIPLY:{
			 gl->glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				 gl->glBlendEquation(GL_FUNC_ADD);
			#endif
			 gl->glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA /* GL_ZERO or GL_ONE_MINUS_SRC_ALPHA */);
			break;
		}

		case OF_BLENDMODE_SCREEN:{
			 gl->glEnable(GL_BLEND);
			#ifndef TARGET_OPENGLES
				 gl->glBlendEquation(GL_FUNC_ADD);
			#endif
			 gl->glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ONE);
			break;
		}

		case OF_BLENDMODE_SUBTRACT:{
			 gl->glEnable(GL_BLEND);
		#ifndef TARGET_OPENGLES
			 gl->glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		#else
			ofLog(OF_LOG_WARNING, "OF_BLENDMODE_SUBTRACT not currently supported on OpenGL/ES");
		#endif
			 gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}


		default:
			break;
	}
}

//----------------------------------------------------------
void ofGLES2Renderer::enablePointSprites(){
#ifdef TARGET_OPENGLES
	 gl->glEnable(GL_POINT_SPRITE_OES);
	 gl->glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
	// does look like this needs to be enabled in ES because
	// it is always eneabled...
	// gl->glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

#else
	 gl->glEnable(GL_POINT_SPRITE);
	 gl->glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	 gl->glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
}

//----------------------------------------------------------
void ofGLES2Renderer::disablePointSprites(){
#ifdef TARGET_OPENGLES
	 gl->glDisable(GL_POINT_SPRITE_OES);
#else
	 gl->glDisable(GL_POINT_SPRITE);
#endif
}

//----------------------------------------------------------
void ofGLES2Renderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	
	scratchMesh.clear();
	
	//linePoints[0].set(x1,y1,z1);
	//linePoints[1].set(x2,y2,z2);

	scratchMesh.addVertex( ofVec3f(x1,y1,z1) );
	scratchMesh.addVertex( ofVec3f(x2,y2,z2) );
	
	// add colors
	for( int i = 0; i < scratchMesh.getNumVertices(); i++ )
	{
		scratchMesh.addColor( currentColor );
	}	
	
	// use smoothness, if requested:
	if (bSmoothHinted) startSmoothing();

	//gl->glEnableClientState(GL_VERTEX_ARRAY);
	//gl->glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &linePoints[0].x);
	//gl->glDrawArrays(GL_LINES, 0, 2);

	scratchMesh.setMode( OF_PRIMITIVE_LINES );
	
	draw( scratchMesh );	
	
	// use smoothness, if requested:
	if (bSmoothHinted) endSmoothing();

}

//----------------------------------------------------------
void ofGLES2Renderer::drawRectangle(float x, float y, float z,float w, float h){

	scratchMesh.clear();
	
	if (rectMode == OF_RECTMODE_CORNER){
		scratchMesh.addVertex( ofVec3f(x,y,z) );
		scratchMesh.addVertex( ofVec3f(x+w, y, z) );
		scratchMesh.addVertex( ofVec3f(x+w, y+h, z) );
		scratchMesh.addVertex( ofVec3f(x, y+h, z) );		
	}else{		
		scratchMesh.addVertex( ofVec3f(x-w/2.0f, y-h/2.0f, z) );
		scratchMesh.addVertex( ofVec3f(x+w/2.0f, y-h/2.0f, z) );
		scratchMesh.addVertex( ofVec3f(x+w/2.0f, y+h/2.0f, z) );
		scratchMesh.addVertex( ofVec3f(x-w/2.0f, y+h/2.0f, z) );		
	}
	
	// add colors
	for( int i = 0; i < scratchMesh.getNumVertices(); i++ )
	{
		scratchMesh.addColor( currentColor );
	}

	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	scratchMesh.setMode( (bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP );
	
	draw( scratchMesh );
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLES2Renderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	
	scratchMesh.clear();	
	
	scratchMesh.addVertex( ofVec3f(x1,y1,z1) );
	scratchMesh.addVertex( ofVec3f(x2,y2,z2) );
	scratchMesh.addVertex( ofVec3f(x3,y3,z3) );

	// add colors
	for( int i = 0; i < scratchMesh.getNumVertices(); i++ )
	{
		scratchMesh.addColor( currentColor );
	}	

	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	scratchMesh.setMode( (bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP );
	
	draw( scratchMesh );	
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLES2Renderer::drawCircle(float x, float y, float z,  float radius){
	
	scratchMesh.clear();		
	
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		scratchMesh.addVertex( ofVec3f(radius*circleCache[i].x+x,radius*circleCache[i].y+y,z) );	
		scratchMesh.addColor( currentColor );		
	}
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	scratchMesh.setMode( (bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP );
	
	draw( scratchMesh );		
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLES2Renderer::drawEllipse(float x, float y, float z, float width, float height){

	scratchMesh.clear();		
	
	float radiusX = width*0.5;
	float radiusY = height*0.5;
	vector<ofPoint> & circleCache = circlePolyline.getVertices();
	for(int i=0;i<(int)circleCache.size();i++){
		scratchMesh.addVertex( ofVec3f(radiusX*circlePolyline[i].x+x,radiusY*circlePolyline[i].y+y,z) );	
		scratchMesh.addColor( currentColor );		
	}

	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) startSmoothing();

	scratchMesh.setMode( (bFilled == OF_FILLED) ? OF_PRIMITIVE_TRIANGLE_FAN : OF_PRIMITIVE_LINE_LOOP );
	draw( scratchMesh );	
	
	// use smoothness, if requested:
	if (bSmoothHinted && bFilled == OF_OUTLINE) endSmoothing();

}

//----------------------------------------------------------
void ofGLES2Renderer::drawString(string textString, float x, float y, float z, ofDrawBitmapMode mode){
	
	
	//cout << "bBitmapTexturePrepared " << bBitmapTexturePrepared << endl;
	
	// this is copied from the ofTrueTypeFont
	//GLboolean blend_enabled =  gl->glIsEnabled(GL_BLEND); //TODO: this is not used?
	GLint blend_src, blend_dst;
	
	gl->glGetIntegerv( GL_BLEND_SRC, &blend_src );
	gl->glGetIntegerv( GL_BLEND_DST, &blend_dst );
	
	ofBlendMode prevBlendMode = ofGetStyle().blendingMode;
	
	//gl->glEnable(GL_BLEND);
	//gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setBlendMode( OF_BLENDMODE_ALPHA );

	int len = (int)textString.length();
	//float yOffset = 0;
	float fontSize = 8.0f;
	bool bOrigin = false;

	float sx = 0;
	float sy = -fontSize;


	///////////////////////////
	// APPLY TRANSFORM / VIEW
	///////////////////////////
	//

	bool hasModelView = false;
	bool hasProjection = false;
	bool hasViewport = false;

	ofRectangle rViewport;

	switch (mode) {

		case OF_BITMAPMODE_SIMPLE:

			sx += x;
			sy += y;
			break;

		case OF_BITMAPMODE_SCREEN:

			hasViewport = true;
			ofPushView();

			rViewport = ofGetWindowRect();
			ofViewport(rViewport);

			gl->glMatrixMode(GL_PROJECTION);
			gl->glLoadIdentity();
			gl->glMatrixMode(GL_MODELVIEW);
			gl->glLoadIdentity();

			gl->glTranslatef(-1, 1, 0);
			gl->glScalef(2/rViewport.width, -2/rViewport.height, 1);

			ofTranslate(x, y, 0);
			break;

		case OF_BITMAPMODE_VIEWPORT:

			rViewport = ofGetCurrentViewport();

			hasProjection = true;
			gl->glMatrixMode(GL_PROJECTION);
			gl->glPushMatrix();
			gl->glLoadIdentity();

			hasModelView = true;
			gl->glMatrixMode(GL_MODELVIEW);
			gl->glPushMatrix();
			gl->glLoadIdentity();

			gl->glTranslatef(-1, 1, 0);
			gl->glScalef(2/rViewport.width, -2/rViewport.height, 1);

			ofTranslate(x, y, 0);
			break;

		case OF_BITMAPMODE_MODEL:

			hasModelView = true;
			gl->glMatrixMode(GL_MODELVIEW);
			gl->glPushMatrix();

			ofTranslate(x, y, z);
			ofScale(1, -1, 0);
			break;

		case OF_BITMAPMODE_MODEL_BILLBOARD:
			//our aim here is to draw to screen
			//at the viewport position related
			//to the world position x,y,z

			// ***************
			// this will not compile for opengl ES
			// ***************
#ifndef TARGET_OPENGLES
			// gl->gluProject method
			GLdouble modelview[16], projection[16];
			GLint view[4];
			double dScreenX, dScreenY, dScreenZ;
			gl->glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			gl->glGetDoublev(GL_PROJECTION_MATRIX, projection);
			gl->glGetIntegerv(GL_VIEWPORT, view);
			view[0] = 0; view[1] = 0; //we're already drawing within viewport
			gl->gluProject(x, y, z, modelview, projection, view, &dScreenX, &dScreenY, &dScreenZ);

			rViewport = ofGetCurrentViewport();

			hasProjection = true;
			gl->glMatrixMode(GL_PROJECTION);
			gl->glPushMatrix();
			gl->glLoadIdentity();

			hasModelView = true;
			gl->glMatrixMode(GL_MODELVIEW);
			gl->glPushMatrix();
			gl->glLoadIdentity();

			gl->glTranslatef(-1, -1, 0);
			gl->glScalef(2/rViewport.width, 2/rViewport.height, 1);

			gl->glTranslatef(dScreenX, dScreenY, 0);
			gl->glScalef(1, -1, 1);
#endif
			break;

		default:
			break;
	}
	//
	///////////////////////////


	// (c) enable texture once before we start drawing each char (no point turning it on and off constantly)
	//We do this because its way faster
	ofDrawBitmapCharacterStart(textString.size());

	for(int c = 0; c < len; c++){
		if(textString[c] == '\n'){

			sy += bOrigin ? -1 : 1 * (fontSize*1.7);
			sx = x;

			// gl->glRasterPos2f(x,y + (int)yOffset);
		} else if (textString[c] >= 32){
			// < 32 = control characters - don't draw
			// solves a bug with control characters
			// getting drawn when they ought to not be
			ofDrawBitmapCharacter(textString[c], (int)sx, (int)sy);
						
			sx += fontSize;
		}
	}
	//We do this because its way faster
	ofDrawBitmapCharacterEnd();


	if (hasModelView)
		 gl->glPopMatrix();

	if (hasProjection)
	{
		 gl->glMatrixMode(GL_PROJECTION);
		 gl->glPopMatrix();
		 gl->glMatrixMode(GL_MODELVIEW);
	}

	if (hasViewport)
		ofPopView();

	setBlendMode( prevBlendMode );
	//gl->glBlendFunc(blend_src, blend_dst);
}


//----------------------------------------------------------
void ofGLES2Renderer::setLightingModel( unsigned int model ){

	
	if( model == GL_SMOOTH ) 
	{ 
		gl->glShadeModel( GL_SMOOTH );
		
		// http://code.google.com/p/gles2-bc/ glHint(GL_LIGHTING_HINT, GL_NICEST) uses per-fragment lighting if using an OpenGL ES 2.0 context	
		gl->glHint(GL_LIGHTING_HINT, GL_NICEST); 
	} 
	else if( model == GL_FLAT ) 
	{
		// From http://www.johannesvuorinen.com/stuff/cost_efficient_development_with_various_opengles_apis_-_Johannes_Vuorinen.pdf
		// "Flat shading is hard to implement efﬁciently in OpenGL ES 2.0. OpenGL ES 2.0 does not have an option to not interpolate varyings so the faces get always smooth shaded."
		gl->glShadeModel( GL_FLAT ); 
	}
	
}

//----------------------------------------------------------
void ofGLES2Renderer::_enable( unsigned int capability ){
	gl->glEnable( capability );
}

//----------------------------------------------------------
void ofGLES2Renderer::_disable( unsigned int capability ){
	gl->glDisable( capability );	
}

//----------------------------------------------------------
void ofGLES2Renderer::_matrixMode( unsigned int mode ){
	gl->glMatrixMode( mode );
}


//----------------------------------------------------------
void ofGLES2Renderer::_alphaFunc( unsigned int func, float ref ){
	gl->glAlphaFunc( func, ref );
}

//----------------------------------------------------------
void ofGLES2Renderer::_lightModelfv(unsigned int pname, const float *params ){
	
	//cout << "ofGLES2Renderer::_lightModelfv " << params[0] << ", " << params[1] << ", " << params[2] << ", " << params[3] << endl;
	
	gl->glLightModelfv( pname, params );
}

//----------------------------------------------------------
void ofGLES2Renderer::_lightf(unsigned int light, unsigned int pname, float param){
	gl->glLightf( light, pname, param );
}

//----------------------------------------------------------
void ofGLES2Renderer::_lightfv (unsigned int light, unsigned int pname, const float *params){
	gl->glLightfv( light, pname, params );
}

//----------------------------------------------------------
void ofGLES2Renderer::_getMaterialfv(unsigned int face, unsigned int pname, float *params){
	gl->glGetMaterialfv( face, pname, params );
}

//----------------------------------------------------------
void ofGLES2Renderer::_materialfv(unsigned int face, unsigned int pname, const float *params){
	gl->glMaterialfv( face, pname, params );
}

//----------------------------------------------------------
void ofGLES2Renderer::_materialf(unsigned int face, unsigned int pname, float param){
	gl->glMaterialf( face, pname, param );
}

//----------------------------------------------------------
void ofGLES2Renderer::_activeTexture( unsigned int texture ){
	gl->glActiveTexture( texture );
}

//----------------------------------------------------------
void ofGLES2Renderer::_bindTexture(unsigned int target, unsigned int texture){
	gl->glBindTexture( target, texture );
}

//----------------------------------------------------------
void ofGLES2Renderer::_texImage2D (unsigned int target, int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void *pixels ){
	gl->glTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

//----------------------------------------------------------
void ofGLES2Renderer::_texEnvf(unsigned int target, unsigned int pname, unsigned int param){
	gl->glTexEnvf( target, pname, param );
}


/*
//---------------------------------------------------------------------
void ofGLES2Renderer::gles2OfDrawBitmapCharacter(int character, int x , int y){
	
	if(!bBitmapTexturePrepared){
		prepareBitmapTexture();
	}
	
	if (character < 128) {		
		//TODO: look into a better fix. 
		//old ofDrawBitmapString was 3 pixels higher, so this version renders text in a different position. 
		//3 pixel adjustment corrects that. 
		y -= 3;
		
		float posTexW = (float)(character % 16)/16.0f;
		float posTexH = ((int)(character / 16.0f))/16.0f;
		
		coords[vC]		= posTexW;
		coords[vC+1]	= posTexH;
		coords[vC+2]	= posTexW + widthTex;
		coords[vC+3]	= posTexH;
		coords[vC+4]	= posTexW+widthTex;
		coords[vC+5]	= posTexH+heightTex;
		
		
		coords[vC+6]	= posTexW + widthTex;
		coords[vC+7]	= posTexH+heightTex;
		coords[vC+8]	= posTexW;
		coords[vC+9]	= posTexH+heightTex;
		coords[vC+10]	= posTexW;
		coords[vC+11]	= posTexH;
		
		verts[vC]	= x;
		verts[vC+1]	= y;
		verts[vC+2]	= x+8;
		verts[vC+3]	= y;
		verts[vC+4]	= x+8;
		verts[vC+5]	= y+14;
		
		verts[vC+6]	= x+8;
		verts[vC+7]	= y+14;
		verts[vC+8] = x;
		verts[vC+9] = y+14;
		verts[vC+10] = x;
		verts[vC+11] = y;
		
		vC += 12;
	}	
}

//---------------------------------------------------------------------
void ofGLES2Renderer::gles2OfDrawBitmapCharacterStart(int stringLength){
	
	verts.clear();
	coords.clear();
	
	verts.assign(12 * (stringLength+1), 0);
	coords.assign(12 * (stringLength+1), 0);
	
	if(!bBitmapTexturePrepared){
		prepareBitmapTexture();
	}
	
	glesBitmappedFontTexture.bind();
	
#ifndef TARGET_OPENGLES
	// this temporarily enables alpha testing,
	// which discards pixels unless their alpha is 1.0f
	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_EQUAL, 1.0f);
#endif
	
	glEnableClientState( GL_VERTEX_ARRAY );		
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glTexParameterf(glesBitmappedFontTexture.getTextureData().textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	vC = 0;
}

//---------------------------------------------------------------------
void ofGLES2Renderer::gles2OfDrawBitmapCharacterEnd(){
	if( vC > 0 ){
		glTexCoordPointer(2, GL_FLOAT, 0, &coords[0] );
		glVertexPointer(2, GL_FLOAT, 0, &verts[0] );
		glDrawArrays(GL_TRIANGLES, 0, vC/2 );
	}
	
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
#ifndef TARGET_OPENGLES
	glPopAttrib();
#endif
	glesBitmappedFontTexture.unbind();
}
*/


