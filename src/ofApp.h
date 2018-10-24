#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void saveFbo();
    void render();
    void loadSlices();
    void onShuffleSlicesPressed();
    void onSaveImagePressed();
    
    ofxIntSlider minAlpha;
    ofxIntSlider maxAlpha;
    ofxFloatSlider minRotation;
    ofxFloatSlider maxRotation;
    ofxFloatSlider minScale;
    ofxFloatSlider maxScale;
    ofxFloatSlider jitterScaleAmount;
    ofxFloatSlider jitterScaleFrequency;
    ofxFloatSlider noiseScale;
    ofxFloatSlider timeScale;
    ofxFloatSlider timeSkipAmount;
    ofxFloatSlider timeSkipFrequency;
    ofxIntSlider rendersPerFrame;
    ofxIntSlider minSliceIndex;
    ofxIntSlider maxSliceIndex;
    ofxFloatSlider randomSliceFrequency;
    ofxButton shuffleSlices;
    ofxButton saveImage;
    ofxPanel gui;
    ofFbo canvas;
    ofPoint randomPoint;
    
    float time;
    vector<ofImage> slices;
};
