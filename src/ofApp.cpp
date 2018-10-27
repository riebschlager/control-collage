#include "ofApp.h"

void ofApp::setup()
{
    shuffleSlices.addListener(this, &ofApp::onShuffleSlicesPressed);
    saveImage.addListener(this, &ofApp::onSaveImagePressed);
    clear.addListener(this, &ofApp::onClearPressed);

    gui.setup();
    gui.add(minAlpha.setup("minAlpha", 0, 0, 255));
    gui.add(maxAlpha.setup("maxAlpha", 255, 0, 255));
    gui.add(randomAlphaFrequency.setup("randomAlphaFrequency", 0, 0, 1));
    gui.add(minRotation.setup("minRotation", -180, -360, 360));
    gui.add(maxRotation.setup("maxRotation", 180, -360, 360));
    gui.add(randomRotationFrequency.setup("randomRotationFrequency", 0, 0, 1));
    gui.add(lowPass.setup("lowPass", 0, 0, 1));
    gui.add(highPass.setup("highPass", 1, 0, 1));
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
    gui.add(sourceChangeFrequency.setup("sourceChangeFrequency", 0, 0, 1));
    gui.add(shuffleSlices.setup("shuffleSlices"));
    gui.add(saveImage.setup("saveImage"));
    gui.add(clear.setup("clear"));

    canvas.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    canvas.begin();
    ofBackground(0, 0, 0);
    canvas.end();

    time = 0;
    currentSourceIndex = 0;
    isDrawing = false;

    loadSlices();
    loadSources();
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

void ofApp::loadSources()
{
    string path = "./sources/face-bright";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        img.setImageType(OF_IMAGE_GRAYSCALE);
        img.resize(canvas.getWidth(), canvas.getHeight());
        sources.push_back(img);
    }
}

void ofApp::render(int x, int y)
{
    ofColor color = sources[currentSourceIndex].getColor(x, y);

    float noise = ofNoise(x * noiseScale, y * noiseScale, time * noiseScale);
    float scale = ofMap(noise, 0, 1, minScale, maxScale);

    if (ofRandom(1) < jitterScaleFrequency)
    {
        scale *= ofRandom(jitterScaleAmount);
    }

    float rotation = ofMap(noise, 0, 1, minRotation, maxRotation);

    if (ofRandom(1) < randomRotationFrequency)
    {
        rotation = ofRandom(-360, 360);
    }

    int sliceIndex = floor(ofMap(noise, 0, 1, minSliceIndex, maxSliceIndex));

    if (ofRandom(1) < randomSliceFrequency)
    {
        sliceIndex = floor(ofRandom(slices.size()));
    }

    float alpha = ofMap(noise, 0, 1, minAlpha, maxAlpha);

    if (ofRandom(1) < randomAlphaFrequency)
    {
        alpha = ofRandom(255);
    }

    if (noise < lowPass || noise > highPass)
    {
        return;
    }

    canvas.begin();
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(scale, scale);
    ofRotateDeg(rotation);
    ofSetColor(color.getBrightness(), alpha);
    slices[sliceIndex].draw(0, 0);
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

    if (ofRandom(1) < sourceChangeFrequency)
    {
        currentSourceIndex = floor(ofRandom(sources.size()));
    }

    for (int i = 0; i < rendersPerFrame; i++)
    {
        if (isDrawing)
        {
            randomPoint.x = ofRandom(0, canvas.getWidth());
            randomPoint.y = ofRandom(0, canvas.getHeight());
            render(randomPoint.x, randomPoint.y);
        }
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

void ofApp::onClearPressed()
{
    canvas.begin();
    ofClear(0, 0, 0);
    canvas.end();
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
    if (key == ' ')
    {
        isDrawing = !isDrawing;
    }
    if (key == 's')
    {
        isMouseDrawing = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    isMouseDrawing = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    if (isMouseDrawing)
    {
        int nx = floor(ofMap(x, 0, ofGetWidth(), 0, canvas.getWidth()));
        int ny = floor(ofMap(y, 0, ofGetHeight(), 0, canvas.getHeight()));
        ofLogNotice(ofToString(x), ofToString(y));
        render(nx, ny);
    }
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
