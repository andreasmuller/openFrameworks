
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	//ofSetLogLevel( OF_LOG_NOTICE );
	ofSetLogLevel( OF_LOG_VERBOSE );
		
	int overrideGLVersion = 2;
#ifndef OPENGLES_VERSION_2
	overrideGLVersion = 1;
#endif	
	
	//ofSetCurrentRenderer(ofPtr<ofBaseRenderer>(new ofGLES2Renderer(overrideGLVersion)));
	
	/*gles2Renderer = ofPtr<ofGLES2Renderer>( new ofGLES2Renderer(overrideGLVersion) );
	ofSetCurrentRenderer( gles2Renderer );	*/
	
	//ofxiPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	//ipad doesn't need no scale ;) 
	appIphoneScale = 1.0;

	ofBackground(200,200,200);	
	ofSetFrameRate(60);
	
	ofSetCircleResolution( 50 );
	
	nCurveVertexes = 7;
	
	curveVertices[0].x = 326;
	curveVertices[0].y = 209;
	curveVertices[1].x = 306;
	curveVertices[1].y = 279;
	curveVertices[2].x = 265;
	curveVertices[2].y = 331;
	curveVertices[3].x = 304;
	curveVertices[3].y = 383;
	curveVertices[4].x = 374;
	curveVertices[4].y = 383;
	curveVertices[5].x = 418;
	curveVertices[5].y = 309;
	curveVertices[6].x = 345;
	curveVertices[6].y = 279;
	
	for (int i = 0; i < nCurveVertexes; i++){
		curveVertices[i].bOver 			= false;
		curveVertices[i].bBeingDragged 	= false;
		curveVertices[i].radius = 4;
	}

	//testImage.loadImage("boy_10.tga");
	testImage.loadImage("tdf_1972_poster.jpg");	
	
	ofDisableArbTex();
	testImage2.loadImage("bikers.jpg");
	ofEnableArbTex();
	
	testImageAlpha.loadImage("AlphaTest.png");
	
	ofSetGlobalAmbientColor( ofColor( 40.0f, 40.0f, 40.0f) );		
	
	pointLight.setDiffuseColor( ofColor( 0.0f, 255.0f, 0.0f) );
	pointLight.setSpecularColor(ofColor(255, 255, 255));
	pointLight.setPosition( 0.0f, 10.0f, 0.0f );	
	pointLight.setPointLight();
	
	
	spotLight.setDiffuseColor( ofColor(255, 0, 0) );
	spotLight.setSpecularColor( ofColor(255, 255, 255) );
	spotLight.setSpotlight(40, 6);
	
	directionalLight.setDiffuseColor( ofColor(0, 0, 255) );
	directionalLight.setSpecularColor( ofColor(255, 255, 255) );
	directionalLight.setDirectional();
	directionalLight.setOrientation( ofVec3f(0, 90, 0) );	
	
	material.setSpecularColor(ofColor(255, 255, 255, 255));	
	material.setShininess( 120 );
	 
	ofSetSmoothLighting(true);
	
	bPointLight = bSpotLight = bDirLight = true;
	
	ofLightingModel(GL_SMOOTH);
	
	
	/*
	 // we need GL_TEXTURE_2D for our models coords.
	 ofDisableArbTex();
	 
	 if(model.loadModel("astroBoy_walk.dae",true))
	 {
		 model.setAnimation(0);
		 model.setPosition(ofGetWidth()/2, (float)ofGetHeight() * 0.75 , 0);
		 //model.createLightsFromAiModel();
		 //model.disableTextures();
		 //model.disableMaterials();
		 
		 mesh = model.getMesh(0);
		 position = model.getPosition();
		 normScale = model.getNormalizedScale();
		 scale = model.getScale();
		 sceneCenter = model.getSceneCenter();
		 material = model.getMaterialForMesh(0);
		 tex = model.getTextureForMesh(0);
	 }
	 
	 ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	 */
	 
	 //some model / light stuff
	 //glShadeModel(GL_SMOOTH);
	 //light.enable();
	 //ofEnableSeparateSpecularLight();
	
	franklinBook14.loadFont("frabk.ttf", 14);
	franklinBook14.setLineHeight(18.0f);
	franklinBook14.setLetterSpacing(1.037);	
	
	typeStr = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789,:&!?";
	
	
	ofFbo::Settings tmpSettings = ofFbo::Settings(); 
	tmpSettings.width  = 480;
	tmpSettings.height = 320;	
	tmpSettings.useDepth = false;
	tmpSettings.useStencil = false;
	tmpSettings.internalformat = GL_RGB;

//	testFBO.allocate( tmpSettings ); 

	
	ofFbo::Settings tmpScreenFBOSettings = ofFbo::Settings(); 
	tmpScreenFBOSettings.width  = 1024;
	tmpScreenFBOSettings.height = 1024;	// Andreas: For now FBO dimensions have to be a power of two, investigating
	tmpScreenFBOSettings.useDepth = true;
	tmpScreenFBOSettings.useStencil = false;
	tmpScreenFBOSettings.internalformat = GL_RGB;
	
	screenFBO.allocate( tmpScreenFBOSettings ); 	
	
	
	testShader.load("shaders/TestShader");
	
	mouseX = 400;
	mouseY = 400;	
	
	counter = 0;
	
	//string extensions = (char*)glGetString(GL_EXTENSIONS);
	//ofLog(OF_LOG_VERBOSE,extensions);

	//string glEsVersion = (char*)glGetString(GL_VERSION);
	//ofLog(OF_LOG_VERBOSE,glEsVersion);	
	
	//string glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	//ofLog(OF_LOG_VERBOSE,glslVersion);
	
	
}


//--------------------------------------------------------------
void testApp::update()
{
	counter++;
	
	float radius = 70.f; 
	//ofVec3f center(800.0f,ofGetHeight()/2.0f, 0);	
	ofVec3f center(0.0f,0.0f, 0);		
	
	
	pointLight.setPosition( cos(ofGetElapsedTimef()*0.8f) * radius * 2 + center.x, 
						    sin(ofGetElapsedTimef()*1.2f) * radius * 2 + center.y, 
						   -cos(ofGetElapsedTimef()*1.2f) * radius * 2 + center.z ); 
	
	
	spotLight.setOrientation( ofVec3f( 0, cos(ofGetElapsedTimef()) * RAD_TO_DEG, 0) );
	spotLight.setPosition( mouseX, mouseY, 200);	
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofScale(appIphoneScale, appIphoneScale, 1.0);

	/*
	screenFBO.begin();	
	
	ofClear( 100.0f, 100.0f, 100.0f, 255.0f );
	
	drawScene();
	
	// Try a shader
	testShader.begin();
	
	testShader.setUniform1f("u_time", ofGetElapsedTimef() );	
	
	ofSetColor(255,255,255,255);		

	float extraFrac = 0.0; //AMathHelpers::cosZeroToOne( ofGetElapsedTimef() );
	float extraSize = 30.0f * extraFrac;
	
	float imageDrawWidth  =  (testImage.getWidth() / 1.0f) + ( extraFrac * (testImage.getWidth() / 4.0f));
	float imageDrawHeight = (testImage.getHeight() / 1.0f) + ( extraFrac * (testImage.getHeight() / 4.0f));
	
	testImage.draw( 100.0f, 100.0f, imageDrawWidth, imageDrawHeight );
	
	testShader.end();
	
	screenFBO.end();
	 
	 ofPushMatrix();
		 ofRotate( 90.0f );
	 	screenFBO.draw(0.0f, -768, 1024, 1024 ); // this is completely wrong, 1024 in height, have a look at this..
	 ofPopMatrix(); 
	 */
	
	
	screenFBO.begin();	
	ofClear( 100.0f, 100.0f, 100.0f, 255.0f );
	drawScene();
	screenFBO.end();
	
	testShader.begin();
	testShader.setUniform1f("u_time", ofGetElapsedTimef() );
	ofPushMatrix();
		ofRotate( 90.0f );
		screenFBO.draw(0.0f, -768, screenFBO.getWidth(), screenFBO.getHeight() ); 
	ofPopMatrix(); 
	testShader.end();
	
}


//--------------------------------------------------------------
void testApp::drawScene()
{
	
	/*
	// draw into an FBO
	ofSetColor(255,255,255, 255);
	testFBO.begin();
	ofClear( 190, 190, 190, 255 );
	ofPushMatrix();
	ofTranslate(testFBO.getWidth() / 2.0f, testFBO.getHeight() / 2.0f, 00.f);
	ofRotate(ofGetElapsedTimef()*1.6 * RAD_TO_DEG, 1, 0, 0);
	ofRotate(ofGetElapsedTimef()*1.8 * RAD_TO_DEG, 0, 1, 0);			
	ofBox(0, 0, 0, 150);
	ofPopMatrix();
	testFBO.end();
	
	// draw the FBO
	ofSetColor(255,255,255,255);	
	testFBO.draw( 512.0f, 600.0f, testFBO.getWidth() / 3.0f, testFBO.getHeight() / 3.0f );
	
	ofSetColor(255,0,0,255);	
	testFBO.draw( 512.0f + testFBO.getWidth() / 3.0f, 600.0f, testFBO.getWidth() / 3.0f, testFBO.getHeight() / 3.0f );
	*/
	
	ofFill();			
	
	// test
	ofFill();	
	ofSetColor(255,0,255, 255);
	
	ofEnableAlphaBlending();
	
	ofCircle( 700.0f, 512.0f, 0.0f, 50.0f );
	
	ofSetColor(0,255,255, 100);	
	ofCircle( 700.0f, 560.0f, 0.0f, 20.0f );	
	
	ofSetColor(255,0,255, 255);	
	ofEllipse( 790.0f, 512.0f, 0.0f, 50.0f, 90.0f );	
	
	ofTriangle( 850.0f, 512.0f, 880.0f, 512.0f, 850.0f, 600.0f );		
	
	ofLine( 900.0f, 512.0f, 900.0f, 600.0f );			
	
	ofRect( 920.0f, 512.0f, 50.0f, 50.0f );	
	
	ofDisableAlphaBlending();
	
	_ofEnable( GL_DEPTH_TEST );
	
	_ofEnable( GL_COLOR_MATERIAL );	
	
	_ofEnable( GL_NORMALIZE );	
	
	ofSetColor(255,255,255, 255);	
	
	ofEnableLighting();
	
	//	material.begin();
	
	if (bPointLight) pointLight.enable();	
	//	if (bSpotLight) spotLight.enable();
	//	if (bDirLight) directionalLight.enable();
	
	ofPushMatrix();
	
	//ofTranslate(800.0f, ofGetHeight() / 2.0f, cos(ofGetElapsedTimef()*1.4) * 50.f);
	//ofTranslate(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f, cos(ofGetElapsedTimef()*1.4) * 50.f);	
	ofTranslate(800.0f, ofGetHeight() / 2.0f, 00.f);
	
	
	ofRotate(ofGetElapsedTimef()*.6 * RAD_TO_DEG, 1, 0, 0);
	ofRotate(ofGetElapsedTimef()*.8 * RAD_TO_DEG, 0, 1, 0);
	
	//ofRotate( 55.0f, 1, 0, 0);
	//ofRotate( 30.0f, 0, 1, 0);
	
	//ofBox(0, 0, 0, 120);
	
	//model.drawFaces();
	
	drawCube( 120.0f );
	//ofRect( 0.0, 0.0f, 100.0f, 100.0f );
	//drawIcosahedron( 120.0f );
	
	ofPopMatrix();
	pointLight.disable();
	//	spotLight.disable();
	//	directionalLight.disable();
	
	//	material.end();
	
	ofDisableLighting();
	
	ofSetColor( pointLight.getDiffuseColor() );
	if(bPointLight) pointLight.draw();
	
	//	ofSetColor( spotLight.getDiffuseColor() );
	//	if(bSpotLight) spotLight.draw();
	
	_ofDisable( GL_CULL_FACE );
	_ofDisable( GL_DEPTH_TEST );
	
	
	ofEnableAlphaBlending();	
	
	ofSetColor(255,255,255);	
	testImage.draw( 870.0f, 20.0f, testImage.getWidth() / 3.0f, testImage.getHeight() / 3.0f );
	
	//;
	
	ofSetColor(255,255,255, AMathHelpers::cosZeroToOne( ofGetElapsedTimef() ) * 255 );		
	testImage2.draw( 870.0f, 200.0f, testImage2.getWidth() / 3.0f, testImage2.getHeight() / 3.0f );
	
	ofSetColor(255,255,255, 255 - (AMathHelpers::cosZeroToOne( ofGetElapsedTimef() ) * 255) );
	testImageAlpha.draw( 960.0f, 350.0f, testImageAlpha.getWidth() / 2.0f, testImageAlpha.getHeight() / 2.0f );
	
	ofDisableAlphaBlending();	
	
	
	
	ofSetColor(225);
	franklinBook14.drawString("franklin book 14pt - ", 30, 700);
	franklinBook14.drawString(typeStr, 30, 720);
	
	
	ofFill();
	ofSetHexColor(0xe0be21);
	
	//------(a)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding odd, the default rule
	//
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	ofSetPolyMode(OF_POLY_WINDING_ODD);	// this is the normal mode
	ofBeginShape();
	ofVertex(200,135);
	ofVertex(15,135);
	ofVertex(165,25);
	ofVertex(105,200);
	ofVertex(50,25);
	ofEndShape();
	
	
	//------(b)--------------------------------------
	// 
	// 		draw a star
	//
	// 		use poly winding nonzero
	//
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	ofSetHexColor(0xb5de10);
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);
	ofBeginShape();
	ofVertex(400,135);
	ofVertex(215,135);
	ofVertex(365,25);
	ofVertex(305,200);
	ofVertex(250,25);
	ofEndShape();
	//-------------------------------------
	
	
	
	//------(c)--------------------------------------
	// 
	// 		draw a star dynamically
	//
	// 		use the mouse position as a pct
	//		to calc nPoints and internal point radius
	//
	float xPct = (float)(mouseX) / (float)(ofGetWidth());
	float yPct = (float)(mouseY) / (float)(ofGetHeight());
	int nTips = 5 + xPct * 60;
	int nStarPts = nTips * 2;
	float angleChangePerPt = TWO_PI / (float)nStarPts;
	float innerRadius = 0 + yPct*80;
	float outerRadius = 80;
	float origx = 525;
	float origy = 100;
	float angle = 0;
	
	ofSetHexColor(0xa16bca);
	ofBeginShape();
	for (int i = 0; i < nStarPts; i++){
		if (i % 2 == 0) {
			// inside point:
			float x = origx + innerRadius * cos(angle);
			float y = origy + innerRadius * sin(angle);
			ofVertex(x,y);
		} else {
			// outside point
			float x = origx + outerRadius * cos(angle);
			float y = origy + outerRadius * sin(angle);
			ofVertex(x,y);
		}
		angle += angleChangePerPt;
	}
	ofEndShape();
	//-------------------------------------
	
	//------(d)--------------------------------------
	// 
	// 		poylgon of random points
	//
	// 		lots of self intersection, 500 pts is a good stress test
	// 
	// 
	ofSetHexColor(0x0cb0b6);
	ofSetPolyMode(OF_POLY_WINDING_ODD);
	ofBeginShape();
	for (int i = 0; i < 10; i++){
		ofVertex(ofRandom(650,850), ofRandom(20,200));
	}
	ofEndShape();
	//-------------------------------------
	
	
	//------(e)--------------------------------------
	// 
	// 		use sin cos and time to make some spirally shape
	//
	ofPushMatrix();
	ofTranslate(100,300,0);
	ofSetHexColor(0xff2220);
	ofFill();
	ofSetPolyMode(OF_POLY_WINDING_ODD);
	ofBeginShape();
	float angleStep 	= TWO_PI/(100.0f + sin(ofGetElapsedTimef()/5.0f) * 60); 
	float radiusAdder 	= 0.5f;
	float radius 		= 0;
	for (int i = 0; i < 200; i++){
		float anglef = (i) * angleStep;
		float x = radius * cos(anglef);
		float y = radius * sin(anglef); 
		ofVertex(x,y);
		radius 	+= radiusAdder; 
	}
	ofEndShape(OF_CLOSE);
	ofPopMatrix();
	//-------------------------------------
	
	//------(f)--------------------------------------
	// 
	// 		ofCurveVertex
	// 
	// 		because it uses catmul rom splines, we need to repeat the first and last 
	// 		items so the curve actually goes through those points
	//
	
	ofSetHexColor(0x2bdbe6);
	ofBeginShape();
	
	for (int i = 0; i < nCurveVertexes; i++){
		
		
		// sorry about all the if/states here, but to do catmull rom curves
		// we need to duplicate the start and end points so the curve acutally 
		// goes through them.
		
		// for i == 0, we just call the vertex twice
		// for i == nCurveVertexes-1 (last point) we call vertex 0 twice
		// otherwise just normal ofCurveVertex call
		
		if (i == 0){
			ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
			ofCurveVertex(curveVertices[0].x, curveVertices[0].y); // we need to duplicate 0 for the curve to start at point 0
		} else if (i == nCurveVertexes-1){
			ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
			ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// to draw a curve from pt 6 to pt 0
			ofCurveVertex(curveVertices[0].x, curveVertices[0].y);	// we duplicate the first point twice
		} else {
			ofCurveVertex(curveVertices[i].x, curveVertices[i].y);
		}
	}
	
	ofEndShape();
	
	
	// show a faint the non-curve version of the same polygon:
	ofEnableAlphaBlending();
	ofNoFill();
	ofSetColor(0,0,0,40);
	ofBeginShape();
	for (int i = 0; i < nCurveVertexes; i++){
		ofVertex(curveVertices[i].x, curveVertices[i].y);
	}
	ofEndShape(true);
	
	
	ofSetColor(0,0,0,80);
	for (int i = 0; i < nCurveVertexes; i++){
		if (curveVertices[i].bOver == true) ofFill();
		else ofNoFill();
		ofCircle(curveVertices[i].x, curveVertices[i].y,4);
	}
	ofDisableAlphaBlending();
	//-------------------------------------
	
	
	//------(g)--------------------------------------
	// 
	// 		ofBezierVertex
	// 
	// 		with ofBezierVertex we can draw a curve from the current vertex
	//		through the the next three vertexes we pass in.
	//		(two control points and the final bezier point)
	//		
	
	float x0 = 500;
	float y0 = 300;
	float x1 = 550+50*cos(ofGetElapsedTimef()*1.0f);
	float y1 = 300+100*sin(ofGetElapsedTimef()/3.5f);
	float x2 = 600+30*cos(ofGetElapsedTimef()*2.0f);
	float y2 = 300+100*sin(ofGetElapsedTimef());
	float x3 = 650;
	float y3 = 300;
	
	
	
	ofFill();
	ofSetHexColor(0xFF9933);
	ofBeginShape();
	ofVertex(x0,y0);
	ofBezierVertex(x1,y1,x2,y2,x3,y3);
	ofEndShape();
	
	
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(0,0,0,40);
	ofCircle(x0,y0,4);
	ofCircle(x1,y1,4);
	ofCircle(x2,y2,4);
	ofCircle(x3,y3,4);
	ofDisableAlphaBlending();
	
	
	
	//------(h)--------------------------------------
	// 
	// 		holes / ofNextContour
	// 
	// 		with ofNextContour we can create multi-contour shapes
	// 		this allows us to draw holes, for example... 
	//
	ofFill();
	ofSetHexColor(0xd3ffd3);
	ofRect(80,480,140,70);
	ofSetHexColor(0xff00ff);
	
	ofBeginShape();
	
	ofVertex(100,500);
	ofVertex(180,550);
	ofVertex(100,600);
	
	ofNextContour(true);
	
	ofVertex(120,520);
	ofVertex(160,550);
	ofVertex(120,580);
	
	ofEndShape(true);
	//-------------------------------------
	
	
	//------(i)--------------------------------------
	// 
	// 		CSG / ofNextContour
	// 
	// 		with different winding rules, you can even use ofNextContour to 
	// 		perform constructive solid geometry 
	// 		
	// 		be careful, the clockwiseness or counter clockwisenss of your multiple
	// 		contours matters with these winding rules.
	//
	// 		for csg ideas, see : http://glprogramming.com/red/chapter11.html
	// 
	// 		info about the winding rules is here:
	//		http://glprogramming.com/red/images/Image128.gif
	// 
	ofNoFill();
	
	
	ofPushMatrix();
	
	ofSetPolyMode(OF_POLY_WINDING_ODD);
	
	ofBeginShape();
	
	ofVertex(300,500);
	ofVertex(380,550);
	ofVertex(300,600);
	
	ofNextContour(true);
	
	for (int i = 0; i < 20; i++){
		float anglef = ((float)i / 19.0f) * TWO_PI;
		float x = 340 + 30 * cos(anglef);
		float y = 550 + 30 * sin(anglef); 
		ofVertex(x,y);
		radius 	+= radiusAdder; 
	}
	
	
	ofEndShape(true);
	
	ofTranslate(100,0,0);
	
	ofSetPolyMode(OF_POLY_WINDING_NONZERO);	
	ofBeginShape();
	
	ofVertex(300,500);
	ofVertex(380,550);
	ofVertex(300,600);
	
	ofNextContour(true);
	
	for (int i = 0; i < 20; i++){
		float anglef = ((float)i / 19.0f) * TWO_PI;
		float x = 340 + 30 * cos(anglef);
		float y = 550 + 30 * sin(anglef); 
		ofVertex(x,y);
		radius 	+= radiusAdder; 
	}
	
	ofEndShape(true);
	
	ofTranslate(100,0,0);
	ofSetPolyMode(OF_POLY_WINDING_ABS_GEQ_TWO);
	ofBeginShape();
	ofVertex(300,500);
	ofVertex(380,550);
	ofVertex(300,600);
	ofNextContour(true);
	
	for (int i = 0; i < 20; i++){
		float anglef = ((float)i / 19.0f) * TWO_PI;
		float x = 340 + 30 * cos(anglef);
		float y = 550 + 30 * sin(anglef); 
		ofVertex(x,y);
		radius 	+= radiusAdder; 
	}
	
	
	ofEndShape(true);
	
	ofPopMatrix();
	//-------------------------------------
	
	
	//ofSetHexColor(0x000000);
	//ofDrawBitmapString("ABCDEFGHIJKLMNOPQRSTUVWXYZ\nabcdefghijklmnopqrstuvwxyz\n0123456789,:&!?", 20,300);	
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(a) star\nwinding rule odd", 20,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(b) star\nwinding rule nonzero", 220,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(c) dynamically\ncreated shape", 420,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(d) random points\npoly", 670,210);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(e) fun with sin/cos", 20,410);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(f) ofCurveVertex\nuses catmull rom\nto make curved shapes", 220,410);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(g) ofBezierVertex\nuses bezier to draw curves", 460,410);
	
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(h) ofNextContour\nallows for holes", 20,610);
	
	ofSetHexColor(0x000000);
	ofDrawBitmapString("(i) ofNextContour\ncan even be used for CSG operations\nsuch as union and intersection", 260,620);
	
	
}

//--------------------------------------------------------------
void testApp::debugDraw()
{
	ofGetGLES2Context()->glMatrixMode(GL_PROJECTION);
    
	ofGetGLES2Context()->glLoadIdentity();
	ofGetGLES2Context()->glOrthof(-1.0f, 1.0f, -1.5f, 1.5f, -1.0f, 1.0f);
	
	ofGetGLES2Context()->glMatrixMode(GL_MODELVIEW);
    
    // Enable lighting
	ofGetGLES2Context()->glEnable(GL_LIGHTING);
	
    // Turn the first light on
	ofGetGLES2Context()->glEnable(GL_LIGHT0);
    
    // Define the ambient component of the first light
    static const Color3D light0Ambient[] = {{0.4, 0.4, 0.4, 1.0}};
    ofGetGLES2Context()->glLightfv(GL_LIGHT0, GL_AMBIENT, (const GLfloat *)light0Ambient);
    
    // Define the diffuse component of the first light
    static const Color3D light0Diffuse[] = {{1.0, 1.0, 1.0, 1.0}};
    ofGetGLES2Context()->glLightfv(GL_LIGHT0, GL_DIFFUSE, (const GLfloat *)light0Diffuse);
    
    // Define the specular component and shininess of the first light
    static const Color3D light0Specular[] = {{0.7, 0.7, 0.7, 1.0}};
    ofGetGLES2Context()->glLightfv(GL_LIGHT0, GL_SPECULAR, (const GLfloat *)light0Specular);
	
	// 	float radius = 100.f; 
	//	float tmpX =  cos(counter*.006f) * radius * 2; 
	//	float tmpY =  sin(counter*.008f) * radius * 2; 
	//	float tmpZ = -cos(counter*.008f) * radius * 2; 
	
    // Define the position of the first light
	// const GLfloat light0Position[] = {10.0, 10.0, 10.0}; 
    static const Vertex3D light0Position[] = {{0.0, 0.0, 10.0}};
	//   Vertex3D light0Position[] = {{tmpX, tmpY, tmpZ}};	
	

    ofGetGLES2Context()->glLoadIdentity();
	
	
    ofGetGLES2Context()->glEnable( GL_DEPTH_TEST );
	
    ofGetGLES2Context()->glLightfv(GL_LIGHT0, GL_POSITION, (const GLfloat *)light0Position);
	
    ofGetGLES2Context()->glRotatef(counter, 0.0f, 0.0f, 1.0f);
    ofGetGLES2Context()->glRotatef(counter / 1.4f, 0.0f, 1.0f, 0.0f);
	
	
	drawIcosahedron( 1.0f );
	
	ofGetGLES2Context()->glDisable(GL_LIGHTING);
}


//--------------------------------------------------------------
void testApp::drawCube( float _scale )
{
	
	static const float _vertices[] =
	{
		// front
		-0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
		
		// right
		0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f,
		
		// back
		0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		
		// left
		-0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
		
		// top
		-0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f,
		
		// bottom
		-0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f
	};

	static const float _normals[] =
	{
		/* front */ 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
		/* right */ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,
		/* back */ 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
		/* left */ -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0,
		/* top */ 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,
		/* bottom */ 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0
	};

	static const float _colors[] =
	{
		/* front  – white  */  1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
		/* right  – red    */  1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1, 1,0,0,1,
		/* back   – green  */  0,1,0,1, 0,1,0,1, 0,1,0,1, 0,1,0,1, 0,1,0,1, 0,1,0,1,
		/* left   – blue   */  0,0,1,1, 0,0,1,1, 0,0,1,1, 0,0,1,1, 0,0,1,1, 0,0,1,1,
		/* top    - yellow */  1,1,0,1, 1,1,0,1, 1,1,0,1, 1,1,0,1, 1,1,0,1, 1,1,0,1,
		/* bottom - magenta*/  1,0,1,1, 1,0,1,1, 1,0,1,1, 1,0,1,1, 1,0,1,1, 1,0,1,1
	};
	
	int _vertexCount = 36;
	
	ofGetGLES2Context()->glPushMatrix();
	
	ofGetGLES2Context()->glScalef( _scale, _scale , _scale );
		
	ofGetGLES2Context()->glEnableClientState(GL_VERTEX_ARRAY);
	ofGetGLES2Context()->glEnableClientState(GL_NORMAL_ARRAY);
	ofGetGLES2Context()->glEnableClientState(GL_COLOR_ARRAY);
		
	ofGetGLES2Context()->glVertexPointer(3, GL_FLOAT, 0, _vertices);
	ofGetGLES2Context()->glNormalPointer(GL_FLOAT, 0, _normals);
	ofGetGLES2Context()->glColorPointer(4, GL_FLOAT, 0, _colors);
	ofGetGLES2Context()->glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	ofGetGLES2Context()->glDisableClientState(GL_VERTEX_ARRAY);
	ofGetGLES2Context()->glDisableClientState(GL_NORMAL_ARRAY);
	ofGetGLES2Context()->glDisableClientState(GL_COLOR_ARRAY);
		
	ofGetGLES2Context()->glPopMatrix();
	
}

//--------------------------------------------------------------
void testApp::drawIcosahedron( float _scale )
{
	
	// This is the same result as using Vertex3D, just faster to type and
    // can be made const this way
    static const Vertex3D vertices[]= {
        {0, -0.525731, 0.850651},             // vertices[0]
        {0.850651, 0, 0.525731},              // vertices[1]
        {0.850651, 0, -0.525731},             // vertices[2]
        {-0.850651, 0, -0.525731},            // vertices[3]
        {-0.850651, 0, 0.525731},             // vertices[4]
        {-0.525731, 0.850651, 0},             // vertices[5]
        {0.525731, 0.850651, 0},              // vertices[6]
        {0.525731, -0.850651, 0},             // vertices[7]
        {-0.525731, -0.850651, 0},            // vertices[8]
        {0, -0.525731, -0.850651},            // vertices[9]
        {0, 0.525731, -0.850651},             // vertices[10]
        {0, 0.525731, 0.850651}               // vertices[11]
    };
    
    static const Color3D colors[] = {
        {1.0, 0.0, 0.0, 1.0},
        {1.0, 0.5, 0.0, 1.0},
        {1.0, 1.0, 0.0, 1.0},
        {0.5, 1.0, 0.0, 1.0},
        {0.0, 1.0, 0.0, 1.0},
        {0.0, 1.0, 0.5, 1.0},
        {0.0, 1.0, 1.0, 1.0},
        {0.0, 0.5, 1.0, 1.0},
        {0.0, 0.0, 1.0, 1.0},
        {0.5, 0.0, 1.0, 1.0},
        {1.0, 0.0, 1.0, 1.0},
        {1.0, 0.0, 0.5, 1.0}
    };
    
    static const GLubyte icosahedronFaces[] = {
        1, 2, 6,
        1, 7, 2,
        3, 4, 5,
        4, 3, 8,
        6, 5, 11,
        5, 6, 10,
        9, 10, 2,
        10, 9, 3,
        7, 8, 9,
        8, 7, 0,
        11, 0, 1,
        0, 11, 4,
        6, 2, 10,
        1, 6, 11,
        3, 5, 10,
        5, 4, 11,
        2, 7, 9,
        7, 1, 0,
        3, 9, 8,
        4, 8, 0,
    };
    
    static const Vector3D normals[] = {
        {0.000000, -0.417775, 0.675974},
        {0.675973, 0.000000, 0.417775},
        {0.675973, -0.000000, -0.417775},
        {-0.675973, 0.000000, -0.417775},
        {-0.675973, -0.000000, 0.417775},
        {-0.417775, 0.675974, 0.000000},
        {0.417775, 0.675973, -0.000000},
        {0.417775, -0.675974, 0.000000},
        {-0.417775, -0.675974, 0.000000},
        {0.000000, -0.417775, -0.675973},
        {0.000000, 0.417775, -0.675974},
        {0.000000, 0.417775, 0.675973},
    };	

	ofGetGLES2Context()->glPushMatrix();

		ofGetGLES2Context()->glScalef( _scale, _scale , _scale );
		
		ofGetGLES2Context()->glEnableClientState(GL_VERTEX_ARRAY);
		ofGetGLES2Context()->glEnableClientState(GL_COLOR_ARRAY);
		ofGetGLES2Context()->glEnableClientState(GL_NORMAL_ARRAY);
		
		ofGetGLES2Context()->glVertexPointer(3, GL_FLOAT, 0, vertices);
		ofGetGLES2Context()->glColorPointer(4, GL_FLOAT, 0, colors);
		ofGetGLES2Context()->glNormalPointer(GL_FLOAT, 0, normals);
		
		ofGetGLES2Context()->glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_BYTE, icosahedronFaces);
		
		ofGetGLES2Context()->glDisableClientState(GL_VERTEX_ARRAY);
		ofGetGLES2Context()->glDisableClientState(GL_COLOR_ARRAY);
		ofGetGLES2Context()->glDisableClientState(GL_NORMAL_ARRAY);
	
	ofGetGLES2Context()->glPopMatrix();
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	
	
	ofLogNotice() << "touchDown id: " << touch.id;
	
	if( touch.id == 0 ){
		for (int i = 0; i < nCurveVertexes; i++){
			float diffx = touch.x/appIphoneScale - curveVertices[i].x;
			float diffy = touch.y/appIphoneScale - curveVertices[i].y;
			float dist = sqrt(diffx*diffx + diffy*diffy);
			if (dist < 45 ){
				curveVertices[i].bBeingDragged = true;
			} else {
				curveVertices[i].bBeingDragged = false;
			}	
		}
	}
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	if( touch.id == 0 ){
		for (int i = 0; i < nCurveVertexes; i++){
		if (curveVertices[i].bBeingDragged == true){
			curveVertices[i].x = touch.x/appIphoneScale;
			curveVertices[i].y = touch.y/appIphoneScale;
		}
	}
	}
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	if( touch.id == 0 ){
		for (int i = 0; i < nCurveVertexes; i++){
			curveVertices[i].bBeingDragged = false;	
		}
	}
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}
