#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{

public:
  void setup();
  void update();
  void draw();

  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void saveFbo();
  void render(int x, int y);
  void loadSlices();
  void loadSlicesFromSource();
  void loadSources();
  void onShuffleSlicesPressed();
  void onSaveImagePressed();
  void onClearPressed();
  ofVec2f resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight);

  ofxIntSlider minAlpha;
  ofxIntSlider maxAlpha;
  ofxFloatSlider randomAlphaFrequency;
  ofxFloatSlider minRotation;
  ofxFloatSlider maxRotation;
  ofxFloatSlider randomRotationFrequency;
  ofxFloatSlider lowPass;
  ofxFloatSlider highPass;
  ofxFloatSlider minScale;
  ofxFloatSlider maxScale;
  ofxFloatSlider jitterScaleAmount;
  ofxFloatSlider jitterScaleFrequency;
  ofxFloatSlider noiseScaleX;
  ofxFloatSlider noiseScaleY;
  ofxFloatSlider noiseScaleT;
  ofxFloatSlider timeScale;
  ofxFloatSlider timeSkipAmount;
  ofxFloatSlider timeSkipFrequency;
  ofxIntSlider rendersPerFrame;
  ofxIntSlider minSliceIndex;
  ofxIntSlider maxSliceIndex;
  ofxFloatSlider randomSliceFrequency;
  ofxFloatSlider sourceChangeFrequency;
  ofxToggle randomColor;
  ofxFloatSlider randomColorFrequency;
  ofxButton shuffleSlices;
  ofxButton saveImage;
  ofxButton clear;
  ofxPanel gui;
  ofFbo canvas;
  ofPoint randomPoint;

  float time;
  vector<ofImage> slices;
  vector<ofImage> sources;
  int currentSourceIndex;
  bool isDrawing;
  bool isMouseDrawing;
  ofColor color;
};
