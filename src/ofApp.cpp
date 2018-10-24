#include "ofApp.h"

void ofApp::setup()
{
    shuffleSlices.addListener(this, &ofApp::onShuffleSlicesPressed);
    saveImage.addListener(this, &ofApp::onSaveImagePressed);

    gui.setup();
    gui.add(minAlpha.setup("minAlpha", 0, 0, 255));
    gui.add(maxAlpha.setup("maxAlpha", 255, 0, 255));
    gui.add(minRotation.setup("minRotation", -180, -360, 360));
    gui.add(maxRotation.setup("maxRotation", 180, -360, 360));
    gui.add(minScale.setup("minScale", -1, -5, 5));
    gui.add(maxScale.setup("maxScale", 1, -5, 5));
    gui.add(jitterScaleAmount.setup("jitterScaleAmount", 0, 0, 10));
    gui.add(jitterScaleFrequency.setup("jitterScaleFrequency", 0, 0, 1));
    gui.add(noiseScale.setup("noiseScale", 0.0001, 0.00001, 0.001));
    gui.add(timeScale.setup("timeScale", 5, 0, 30));
    gui.add(timeSkipAmount.setup("timeSkipAmount", 0, 0, 1000));
    gui.add(timeSkipFrequency.setup("timeSkipFrequency", 0, 0, 0.1));
    gui.add(rendersPerFrame.setup("rendersPerFrame", 1000, 1, 5000));
    gui.add(randomSliceFrequency.setup("randomSliceFrequency", 0, 0, 1));
    gui.add(shuffleSlices.setup("shuffleSlices"));
    gui.add(saveImage.setup("saveImage"));

    canvas.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    canvas.begin();
    ofBackground(0, 0, 0);
    canvas.end();

    time = 0;
    loadSlices();
}

void ofApp::loadSlices()
{
    string path = "./slices/klimt";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        slices.push_back(img);
    }
    gui.add(minSliceIndex.setup("minSliceIndex", 0, 0, slices.size()));
    gui.add(maxSliceIndex.setup("maxSliceIndex", slices.size(), 0, slices.size()));
}

void ofApp::render()
{
    randomPoint.x = ofRandom(canvas.getWidth());
    randomPoint.y = ofRandom(canvas.getHeight());

    float noise = ofNoise(randomPoint.x * noiseScale, randomPoint.y * noiseScale, time * noiseScale, time * noiseScale);
    float scale = ofMap(noise, 0, 1, minScale, maxScale);

    if (ofRandom(1) < jitterScaleFrequency)
    {
        scale *= ofRandom(jitterScaleAmount);
    }

    float rotation = ofMap(noise, 0, 1, minRotation, maxRotation);
    int sliceIndex = floor(ofMap(noise, 0, 1, minSliceIndex, maxSliceIndex));

    if (ofRandom(1) < randomSliceFrequency)
    {
        sliceIndex = floor(ofRandom(slices.size()));
    }

    float alpha = ofMap(noise, 0, 1, minAlpha, maxAlpha);

    canvas.begin();
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofPushMatrix();
    ofTranslate(randomPoint.x, randomPoint.y);
    ofScale(scale, scale);
    ofRotateDeg(rotation);
    ofSetColor(alpha);

    if (noise > 0.5)
    {
        slices[sliceIndex].draw(0, 0);
    }

    ofPopMatrix();
    canvas.end();
}

void ofApp::update()
{
    time += timeScale;

    if (ofRandom(1) < timeSkipFrequency)
    {
        time += timeSkipAmount;
    }

    for (int i = 0; i < rendersPerFrame; i++)
    {
        render();
    }
}

void ofApp::draw()
{
    ofClear(0, 0, 0);
    canvas.draw(0, 0, ofGetWidth(), ofGetHeight());
    gui.draw();
}

void ofApp::onShuffleSlicesPressed()
{
    std::random_shuffle(slices.begin(), slices.end());
}

void ofApp::onSaveImagePressed()
{
    saveFbo();
}

void ofApp::saveFbo()
{
    ofFbo img;
    ofPixels pixels;
    img.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    img.begin();
    ofBackground(0, 0, 0);
    canvas.draw(0, 0);
    img.end();
    img.readToPixels(pixels);
    ofSaveImage(pixels, "output/image" + ofToString(ofGetUnixTime()) + ".tif");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
