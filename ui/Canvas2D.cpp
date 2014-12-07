/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include "Canvas2D.h"
#include "Canvas3D.h"
#include "Settings.h"
#include "RayScene.h"

#include <chrono>
#include <QCoreApplication>


#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "brush/PaintBucket.h"
#include "filter/InvertFilter.h"
#include "filter/GrayscaleFilter.h"
#include "filter/EdgeDetectFilter.h"
#include "filter/BlurFilter.h"
#include "filter/ScaleFilter.h"
#include "filter/SharpenFilter.h"
#include "filter/CropFilter.h"
#include "filter/TiltFilter.h"

Canvas2D::Canvas2D()
{
    // @TODO: Initialize any pointers in this class here.
    m_scene = NULL;

    BGRA nullColor;
    nullColor.b = 0;
    nullColor.g = 0;
    nullColor.r = 0;
    nullColor.a = 255;

    m_cBrush = NULL;
    m_lBrush = NULL;
    m_qBrush = NULL;
    m_sBrush = NULL;
    m_pBucket = NULL;
    m_filter = NULL;

    m_oldFilter = -1;
    m_stop_render = false;

}

Canvas2D::~Canvas2D()
{
    // @TODO: Be sure to release all memory that you allocate.
    delete m_scene;
    m_scene = NULL;
    delete m_cBrush;
    m_cBrush = NULL;
    delete m_lBrush;
    m_lBrush = NULL;
    delete m_qBrush;
    m_qBrush = NULL;
    delete m_sBrush;
    m_sBrush = NULL;
    delete m_pBucket;
    m_pBucket = NULL;
    delete m_filter;
    m_filter = NULL;
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {

}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.

    SupportCanvas2D::paintEvent(e);

}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************

void Canvas2D::mouseDown(int x, int y)
{
    //bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

    BGRA currentColor;
    currentColor.b = settings.brushBlue;
    currentColor.g = settings.brushGreen;
    currentColor.r = settings.brushRed;
    currentColor.a = 255;

    switch(settings.brushType) {

    case BRUSH_SOLID:
    {
        m_cBrush = new ConstantBrush(currentColor, settings.brushAlpha, settings.brushRadius);
        break;
    }

    case BRUSH_LINEAR:
    {
        m_lBrush = new LinearBrush(currentColor, settings.brushAlpha, settings.brushRadius);
        break;
    }

    case BRUSH_QUADRATIC :
    {
        m_qBrush = new QuadraticBrush(currentColor, settings.brushAlpha, settings.brushRadius);
        break;
    }

    case BRUSH_SMUDGE :
    {
        m_sBrush = new SmudgeBrush(currentColor, settings.brushAlpha, settings.brushRadius);
        m_sBrush->pickUpPaint(x, y, this);
        break;
    }

    case BRUSH_SPECIAL_1 :
    {
        m_pBucket = new PaintBucket(currentColor, settings.brushAlpha, settings.brushRadius);
        break;
    }

    default:
    {
        m_cBrush = new ConstantBrush(currentColor, settings.brushAlpha, settings.brushRadius);
        break;
    }
    }
}

void Canvas2D::mouseDragged(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.


    switch(settings.brushType) {

    case BRUSH_SOLID:
    {
        m_cBrush->setBlue(settings.brushBlue);
        m_cBrush->setGreen(settings.brushGreen);
        m_cBrush->setRed(settings.brushRed);
        m_cBrush->setFlow(settings.brushAlpha);
        m_cBrush->setRadius(settings.brushRadius);
        m_cBrush->paintOnce(x, y, this);
        break;
    }

    case BRUSH_LINEAR:
    {
        m_lBrush->setBlue(settings.brushBlue);
        m_lBrush->setGreen(settings.brushGreen);
        m_lBrush->setRed(settings.brushRed);
        m_lBrush->setFlow(settings.brushAlpha);
        m_lBrush->setRadius(settings.brushRadius);
        m_lBrush->paintOnce(x, y, this);
        break;
    }

    case BRUSH_QUADRATIC :
    {
        m_qBrush->setBlue(settings.brushBlue);
        m_qBrush->setGreen(settings.brushGreen);
        m_qBrush->setRed(settings.brushRed);
        m_qBrush->setFlow(settings.brushAlpha);
        m_qBrush->setRadius(settings.brushRadius);
        m_qBrush->paintOnce(x, y, this);
        break;
    }

    case BRUSH_SMUDGE :
    {
        m_sBrush->setBlue(settings.brushBlue);
        m_sBrush->setGreen(settings.brushGreen);
        m_sBrush->setRed(settings.brushRed);
        m_sBrush->setFlow(settings.brushAlpha);
        m_sBrush->setRadius(settings.brushRadius);
        m_sBrush->paintOnce(x, y, this);
        break;
    }

    case BRUSH_SPECIAL_1 :
    {
        break;
    }

    default:
    {
        m_cBrush->setBlue(settings.brushBlue);
        m_cBrush->setGreen(settings.brushGreen);
        m_cBrush->setRed(settings.brushRed);
        m_cBrush->setFlow(settings.brushAlpha);
        m_cBrush->setRadius(settings.brushRadius);
        m_cBrush->paintOnce(x, y, this);
        break;

    }
    }
}

void Canvas2D::mouseUp(int x, int y)
{
    // TODO: [BRUSH] Mouse interaction for Brush.
    if(settings.brushType == BRUSH_SPECIAL_1)
    {
        m_pBucket->paintOnce(x, y, this);
    }
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage()
{
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.

//    if(m_oldFilter != settings.filterType)
//    {
//        m_oldFilter = settings.filterType;
        float tmpX = settings.scaleX;
        float tmpY = settings.scaleY;
        switch (settings.filterType) {

        case FILTER_INVERT:
            m_filter = new InvertFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            break;

        case FILTER_GRAYSCALE:
            m_filter = new GrayscaleFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            break;

        case FILTER_EDGE_DETECT:
            m_filter = new GrayscaleFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            m_filter->applyFilter();
            delete m_filter;
            m_filter = new EdgeDetectFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            break;

        case FILTER_BLUR:
            m_filter = new BlurFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            break;

        case FILTER_SCALE:
            //first crop image
            m_filter = new CropFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            m_filter->applyFilter();
            delete m_filter;
            m_filter = NULL;
            settings.scaleY = 1;
            //then scale in x direction
            m_filter = new ScaleFilter(this);
            m_filter->applyFilter();
            delete m_filter;
            m_filter = NULL;
            //then scale in Y direction
            settings.scaleX = tmpY;
            settings.scaleY = 1;
            m_filter = new ScaleFilter(this);
            break;

        case FILTER_SPECIAL_1:
            m_filter = new SharpenFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            break;

        case FILTER_SPECIAL_2:
            m_filter = new CropFilter(this, marqueeStart().x(), marqueeStart().y(), marqueeStop().x(), marqueeStop().y());
            break;

        case FILTER_SPECIAL_3:
            m_filter = new TiltFilter(this);
            break;
        }
    //}
    m_filter->applyFilter();
    update();

    delete m_filter;
    m_filter = NULL;

    settings.scaleX = tmpX;
    settings.scaleY = tmpY;
}

void Canvas2D::setScene(RayScene *scene)
{
    delete m_scene;
    m_scene = scene;
}


void Canvas2D::renderImage(Camera *camera, int width, int height)
{
    if (m_scene)
    {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.
        resize(width, height);
        auto time1 = std::chrono::high_resolution_clock::now();
        m_scene->paintCanvas(this, width, height, *camera);
        auto time2 = std::chrono::high_resolution_clock::now();
        cout << "render time: "
             << 0.001 * std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1).count()
             << " seconds" << endl;

    }
}

void Canvas2D::cancelRender()
{
    // TODO: cancel the raytracer (optional)
    m_stop_render = true;
}



void Canvas2D::settingsChanged() {

    // TODO: Process changes to the application settings.

}
