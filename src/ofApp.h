#pragma once

#define APP_WIDTH 1272
#define APP_HEIGHT 1080
#define CANVAS_WIDTH 7632
#define CANVAS_HEIGHT 6480

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{

public:
  void setup();
  void update();
  void draw();

  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
  void keyPressed(int key);
  void keyReleased(int key);
  void loadSlices(string folderName, int maxWidth, int maxHeight);
  void loadSlices(string folderName);
  void loadSlicesFromSource(string folderName, int numberOfSlices, int maxWidth, int maxHeight);
  void loadSources(string folderName);
  void mouseDragged(int x, int y, int button);
  void mouseEntered(int x, int y);
  void mouseExited(int x, int y);
  void mouseMoved(int x, int y);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void onClearPressed();
  void onSaveImagePressed();
  void onShuffleSlicesPressed();
  void render(int x, int y);
  void saveFbo();
  void windowResized(int w, int h);
  ofVec2f resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight);

  bool mIsGuiVisible;
  bool mIsDrawing;
  bool mIsMouseDrawing;
  float mTime;
  int mCurrentSourceIndex;
  ofColor mColor;
  ofFbo mCanvas;
  ofPoint randomPoint;
  ofxButton clear;
  ofxButton saveImage;
  ofxButton shuffleSlices;
  ofxFloatSlider highPass;
  ofxFloatSlider jitterScaleAmount;
  ofxFloatSlider jitterScaleFrequency;
  ofxFloatSlider lowPass;
  ofxFloatSlider randomAlphaFrequency;
  ofxFloatSlider randomColorFrequency;
  ofxFloatSlider randomRotationFrequency;
  ofxFloatSlider randomSliceFrequency;
  ofxFloatSlider sourceChangeFrequency;
  ofxFloatSlider timeScale;
  ofxFloatSlider timeSkipAmount;
  ofxFloatSlider timeSkipFrequency;
  ofxIntSlider maxSliceIndex;
  ofxIntSlider minSliceIndex;
  ofxIntSlider rendersPerFrame;
  ofxPanel mGui;
  ofxToggle randomColor;
  ofxVec2Slider mAlpha;
  ofxVec2Slider mRotation;
  ofxVec2Slider mScale;
  ofxVec3Slider mNoiseScale;
  string mBasePath;
  vector<ofImage> mSlices;
  vector<ofImage> mSources;
};
