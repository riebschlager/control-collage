#include "ofApp.h"

void ofApp::setup()
{
    shuffleSlices.addListener(this, &ofApp::onShuffleSlicesPressed);
    saveImage.addListener(this, &ofApp::onSaveImagePressed);
    clear.addListener(this, &ofApp::onClearPressed);

    canvas.allocate(1920 * 2, 1080 * 2, GL_RGBA);
    canvas.begin();
    ofBackground(0, 0, 0);
    canvas.end();

    time = 0;
    currentSourceIndex = 0;
    isDrawing = false;

    loadSlices();
    loadSources();

    gui.setup();
    gui.add(minAlpha.setup("minAlpha", 0, 0, 255));
    gui.add(maxAlpha.setup("maxAlpha", 255, 0, 255));
    gui.add(randomAlphaFrequency.setup("randomAlphaFrequency", 0, 0, 0.1));
    gui.add(minRotation.setup("minRotation", -180, -360, 360));
    gui.add(maxRotation.setup("maxRotation", 180, -360, 360));
    gui.add(randomRotationFrequency.setup("randomRotationFrequency", 0, 0, 1));
    gui.add(lowPass.setup("lowPass", 0, 0, 1));
    gui.add(highPass.setup("highPass", 1, 0, 1));
    gui.add(minScale.setup("minScale", -1, -2, 2));
    gui.add(maxScale.setup("maxScale", 1, -2, 2));
    gui.add(jitterScaleAmount.setup("jitterScaleAmount", 0, 0, 5));
    gui.add(jitterScaleFrequency.setup("jitterScaleFrequency", 0, 0, 1));
    gui.add(noiseScaleX.setup("noiseScaleX", 0.0001, 0, 0.005));
    gui.add(noiseScaleY.setup("noiseScaleY", 0.0001, 0, 0.005));
    gui.add(noiseScaleT.setup("noiseScaleT", 0.0001, 0, 0.005));
    gui.add(timeScale.setup("timeScale", 5, 0, 30));
    gui.add(timeSkipAmount.setup("timeSkipAmount", 0, 0, 1000));
    gui.add(timeSkipFrequency.setup("timeSkipFrequency", 0, 0, 0.1));
    gui.add(rendersPerFrame.setup("rendersPerFrame", 1000, 1, 5000));
    gui.add(randomSliceFrequency.setup("randomSliceFrequency", 0, 0, 1));
    gui.add(sourceChangeFrequency.setup("sourceChangeFrequency", 0, 0, 0.1));
    gui.add(minSliceIndex.setup("minSliceIndex", 0, 0, slices.size()));
    gui.add(maxSliceIndex.setup("maxSliceIndex", slices.size(), 0, slices.size() - 1));
    gui.add(randomColor.setup("randomColor", false));
    gui.add(randomColorFrequency.setup("randomColorFrequency", 0, 0, 0.0001));
    gui.add(shuffleSlices.setup("shuffleSlices"));
    gui.add(saveImage.setup("saveImage"));
    gui.add(clear.setup("clear"));
}

ofVec2f ofApp::resizeProportionally(int srcWidth, int srcHeight, int maxWidth, int maxHeight)
{
    float ratio = min(maxWidth / srcWidth, maxHeight / srcHeight);
    return ofVec2f(srcWidth * ratio, srcHeight * ratio);
}

void ofApp::loadSlices()
{
    string path = "./slices/vangogh";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        //ofVec2f r = resizeProportionally(img.getWidth(), img.getHeight(), 300, 300);
        //img.resize(r.x, r.y);
        slices.push_back(img);
    }
}

void ofApp::loadSlicesFromSource()
{
    string path = "./sources/face";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        for (int j = 0; j < 10; j++)
        {
            ofImage slice;
            slice.cropFrom(img, ofRandom(img.getWidth()), ofRandom(img.getHeight()), ofRandom(300), ofRandom(300));
            slices.push_back(slice);
        }
    }
}

void ofApp::loadSources()
{
    string path = "./sources/vangogh";
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        img.resize(canvas.getWidth(), canvas.getHeight());
        sources.push_back(img);
    }
}

void ofApp::render(int x, int y)
{

    if (randomColor && ofRandom(1) < randomColorFrequency)
    {
        int x = ofRandom(ofGetWidth());
        int y = ofRandom(ofGetHeight());
        color = sources[currentSourceIndex].getColor(x, y);
    }
    else if (!randomColor)
    {
        color = sources[currentSourceIndex].getColor(x, y);
    }

    float noise = ofNoise(x * noiseScaleX, y * noiseScaleY, time * noiseScaleT);
    float scale = ofMap(noise, lowPass, highPass, minScale, maxScale);

    if (ofRandom(1) < jitterScaleFrequency)
    {
        scale *= ofRandom(jitterScaleAmount);
    }

    float rotation = ofMap(noise, lowPass, highPass, minRotation, maxRotation);

    if (ofRandom(1) < randomRotationFrequency)
    {
        rotation = ofRandom(-360, 360);
    }

    int sliceIndex = floor(ofMap(noise, lowPass, highPass, minSliceIndex, maxSliceIndex));

    if (ofRandom(1) < randomSliceFrequency)
    {
        sliceIndex = floor(ofRandom(slices.size() - 1));
    }

    ofClamp(sliceIndex, 0, slices.size() - 1);

    float alpha = ofMap(noise, lowPass, highPass, minAlpha, maxAlpha);

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
    ofSetColor(color, alpha);
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

void ofApp::keyReleased(int key)
{
    isMouseDrawing = false;
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
    if (isMouseDrawing)
    {
        int nx = floor(ofMap(x, 0, ofGetWidth(), 0, canvas.getWidth()));
        int ny = floor(ofMap(y, 0, ofGetHeight(), 0, canvas.getHeight()));
        render(nx, ny);
    }
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}
