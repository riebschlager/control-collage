#include "ofApp.h"

void ofApp::setup()
{
    ofSetFrameRate(60);

    shuffleSlices.addListener(this, &ofApp::onShuffleSlicesPressed);
    saveImage.addListener(this, &ofApp::onSaveImagePressed);
    clear.addListener(this, &ofApp::onClearPressed);

    mCanvas.allocate(CANVAS_WIDTH, CANVAS_HEIGHT, GL_RGBA);
    mCanvas.begin();
    ofBackground(0);
    mCanvas.end();

    mTime = 0;
    mCurrentSourceIndex = 0;
    mIsDrawing = false;
    mIsGuiVisible = true;

    mBasePath = ofFilePath::getUserHomeDir() + "/Documents/Art/";

    loadSlices("mand", 600, 600);
    loadSlices("watercolor", 600, 600);
    loadSources("lit");

    mGui.setup();
    mGui.add(mAlpha.setup("Alpha", ofVec2f(0, 255), ofVec2f(0, 0), ofVec2f(255, 255)));
    mGui.add(randomAlphaFrequency.setup("randomAlphaFrequency", 0, 0, 0.1));
    mGui.add(mRotation.setup("Rotation", ofVec2f(-90, 90), ofVec2f(-360, -360), ofVec2f(360, 360)));
    mGui.add(randomRotationFrequency.setup("randomRotationFrequency", 0, 0, 1));
    mGui.add(lowPass.setup("lowPass", 0, 0, 1));
    mGui.add(highPass.setup("highPass", 1, 0, 1));
    mGui.add(mScale.setup("Scale", ofVec2f(-1, 1), ofVec2f(-2, -2), ofVec2f(2, 2)));
    mGui.add(jitterScaleAmount.setup("jitterScaleAmount", 0, 0, 5));
    mGui.add(jitterScaleFrequency.setup("jitterScaleFrequency", 0, 0, 1));
    mGui.add(mNoiseScale.setup("Noise Scale", ofVec3f(0.1, 0.1, 0.1), ofVec3f(0, 0, 0), ofVec3f(1, 1, 1)));
    mGui.add(timeScale.setup("timeScale", 5, 0, 30));
    mGui.add(timeSkipAmount.setup("timeSkipAmount", 0, 0, 1000));
    mGui.add(timeSkipFrequency.setup("timeSkipFrequency", 0, 0, 0.1));
    mGui.add(rendersPerFrame.setup("rendersPerFrame", 1000, 1, 5000));
    mGui.add(randomSliceFrequency.setup("randomSliceFrequency", 0, 0, 1));
    mGui.add(sourceChangeFrequency.setup("sourceChangeFrequency", 0, 0, 0.1));
    mGui.add(minSliceIndex.setup("minSliceIndex", 0, 0, mSlices.size() - 1));
    mGui.add(maxSliceIndex.setup("maxSliceIndex", mSlices.size(), 0, mSlices.size() - 1));
    mGui.add(randomColor.setup("randomColor", false));
    mGui.add(randomColorFrequency.setup("randomColorFrequency", 0, 0, 0.0001));
    mGui.add(shuffleSlices.setup("shuffleSlices"));
    mGui.add(saveImage.setup("saveImage"));
    mGui.add(clear.setup("clear"));
}

void ofApp::update()
{
    mTime += timeScale;

    if (ofRandom(1) < timeSkipFrequency)
    {
        mTime += timeSkipAmount;
    }

    if (ofRandom(1) < sourceChangeFrequency)
    {
        mCurrentSourceIndex = floor(ofRandom(mSources.size()));
    }

    for (int i = 0; i < rendersPerFrame; i++)
    {
        if (mIsDrawing)
        {
            randomPoint.x = ofRandom(0, CANVAS_WIDTH);
            randomPoint.y = ofRandom(0, CANVAS_HEIGHT);
            render(randomPoint.x, randomPoint.y);
        }
    }
}

void ofApp::draw()
{
    ofClear(0);
    mCanvas.draw(0, 0, APP_WIDTH, APP_HEIGHT);
    if (mIsGuiVisible)
    {
        mGui.draw();
    }
}

void ofApp::render(int x, int y)
{
    if (randomColor && ofRandom(1) < randomColorFrequency)
    {
        int x = ofRandom(APP_WIDTH);
        int y = ofRandom(APP_HEIGHT);
        mColor = mSources[mCurrentSourceIndex].getColor(x, y);
    }
    else if (!randomColor)
    {
        mColor = mSources[mCurrentSourceIndex].getColor(x, y);
    }

    ofVec3f ns = mNoiseScale * 0.005;

    float noise = ofNoise(x * ns.x, y * ns.y, mTime * ns.z);
    float scale = ofMap(noise, lowPass, highPass, mScale->x, mScale->y);

    if (ofRandom(1) < jitterScaleFrequency)
    {
        scale *= ofRandom(jitterScaleAmount);
    }

    float rotation = ofMap(noise, lowPass, highPass, mRotation->x, mRotation->y);

    if (ofRandom(1) < randomRotationFrequency)
    {
        rotation = ofRandom(-360, 360);
    }

    int sliceIndex = floor(ofMap(noise, lowPass, highPass, minSliceIndex, maxSliceIndex, true));

    if (ofRandom(1) < randomSliceFrequency)
    {
        sliceIndex = floor(ofRandom(mSlices.size() - 1));
    }

    float alpha = ofMap(noise, lowPass, highPass, mAlpha->x, mAlpha->y);

    if (ofRandom(1) < randomAlphaFrequency)
    {
        alpha = ofRandom(255);
    }

    if (noise < lowPass || noise > highPass)
    {
        return;
    }

    mCanvas.begin();
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofPushMatrix();
    ofTranslate(x, y);
    ofScale(scale, scale);
    ofRotateDeg(rotation);
    ofSetColor(mColor, alpha);
    if (sliceIndex < mSlices.size())
    {
        mSlices[sliceIndex].draw(0, 0);
    }
    ofPopMatrix();
    mCanvas.end();
}

void ofApp::onShuffleSlicesPressed()
{
    std::random_shuffle(mSlices.begin(), mSlices.end());
}

void ofApp::onSaveImagePressed()
{
    saveFbo();
}

void ofApp::onClearPressed()
{
    mCanvas.begin();
    ofClear(0);
    mCanvas.end();
}

void ofApp::saveFbo()
{
    ofFbo img;
    ofPixels pixels;
    img.allocate(CANVAS_WIDTH, CANVAS_HEIGHT, GL_RGBA);
    img.begin();
    ofBackground(0);
    mCanvas.draw(0, 0);
    img.end();
    img.readToPixels(pixels);
    ofSaveImage(pixels, "output/image" + ofToString(ofGetUnixTime()) + ".tif");
}

void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        mIsDrawing = !mIsDrawing;
    }
    if (key == 's')
    {
        saveFbo();
    }
    if (key == 'c')
    {
        onClearPressed();
    }
    if (key == 'g')
    {
        mIsGuiVisible = !mIsGuiVisible;
    }
    if (key == 'M')
    {
        mGui.minimizeAll();
        mGui.maximizeAll();
    }
    if (key == 'm')
    {
        mGui.minimizeAll();
    }
}

void ofApp::keyReleased(int key)
{
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
    if (ofGetKeyPressed('x'))
    {
        int nx = floor(ofMap(x, 0, APP_WIDTH, 0, CANVAS_WIDTH, true));
        int ny = floor(ofMap(y, 0, APP_HEIGHT, 0, CANVAS_HEIGHT, true));
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

ofVec2f ofApp::resizeProportionally(float srcWidth, float srcHeight, float maxWidth, float maxHeight)
{
    float ratio = min(maxWidth / srcWidth, maxHeight / srcHeight);
    return ofVec2f(srcWidth * ratio, srcHeight * ratio);
}

void ofApp::loadSlices(string folderName)
{
    string path = mBasePath + "slices/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        mSlices.push_back(img);
    }
}

void ofApp::loadSlices(string folderName, int maxWidth, int maxHeight)
{
    string path = mBasePath + "slices/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        ofVec2f r = resizeProportionally(img.getWidth(), img.getHeight(), maxWidth, maxHeight);
        img.resize(r.x, r.y);
        mSlices.push_back(img);
    }
}

void ofApp::loadSlicesFromSource(string folderName, int numberOfSlices, int maxWidth, int maxHeight)
{
    string path = mBasePath + "sources/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        for (int j = 0; j < numberOfSlices; j++)
        {
            ofImage slice;
            int w = ofRandom(maxWidth);
            int h = ofRandom(maxHeight);
            int x = ofRandom(0, img.getWidth() - w);
            int y = ofRandom(0, img.getHeight() - h);
            slice.cropFrom(img, x, y, w, h);
            mSlices.push_back(slice);
        }
    }
}

void ofApp::loadSources(string folderName)
{
    string path = mBasePath + "sources/" + folderName;
    ofDirectory dir(path);
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
    {
        ofImage img;
        img.load(dir.getPath(i));
        img.resize(CANVAS_WIDTH, CANVAS_HEIGHT);
        mSources.push_back(img);
    }
}
