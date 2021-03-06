#ifndef CANVAS2D_H
#define CANVAS2D_H

#include "SupportCanvas2D.h"
#include "Brush.h"
#include "SmudgeBrush.h"
#include "PaintBucket.h"
//#include "InvertFilter.h"
#include "Filter.h"


class RayScene;

class Camera;

/**
 * @class Canvas2D
 *
 * 2D canvas that students will implement in the Brush and Filter assignments. The same canvas
 * will be used to display raytraced images in the Intersect and Ray assignments.
 */
class Canvas2D : public SupportCanvas2D
{
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    void setScene(RayScene *scene);

    // UI will call this from the button on the "Ray" dock
    void renderImage(Camera *camera, int width, int height);

    // This will be called when the settings have changed
    virtual void settingsChanged();
    bool m_stop_render;


public slots:
    // UI will call this from the button on the "Ray" dock
    void cancelRender();

    // UI will call this from the button on the "Filter" dock
    void filterImage();

protected:void makeMask();
    virtual void paintEvent(QPaintEvent *);  // Overridden from SupportCanvas2D.
    virtual void mouseDown(int x, int y);    // Called when left mouse button is pressed on canvas
    virtual void mouseDragged(int x, int y); // Called when left mouse button is dragged on canvas
    virtual void mouseUp(int x, int y);      // Called when left mouse button is released

    // Called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);



private:
    RayScene *m_scene;
    //TODO: [BRUSH, INTERSECT, RAY] Put your member variables here.

    Brush *m_cBrush;
    Brush *m_lBrush;
    Brush *m_qBrush;
    SmudgeBrush *m_sBrush;
    PaintBucket *m_pBucket;

    Filter *m_filter;

    int m_oldFilter;


};

#endif // CANVAS2D_H
