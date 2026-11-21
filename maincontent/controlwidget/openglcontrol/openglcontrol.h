/*****************************************
 * 作者: YYC
 * 日期: 2020-04-26
 * 功能：3D场景，用于3D模型的显示
 * ***************************************/

#ifndef OPENGLCONTROL_H
#define OPENGLCONTROL_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "shaderlibrary.h"
#include "texturelibrary.h"
#include "objectfactory.h"
#include "cameralibrary.h"

class OpenglControl : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit OpenglControl(QWidget *parent = nullptr);
    ~OpenglControl() override;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
private:
    QList <ObjectParent*> m_objectList;
    void wheelEvent(QWheelEvent *wheelEvent) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    CameraLibrary m_cameraLibrary;

    QPoint m_lastPos; // 记录鼠标上一次的位置

    bool m_isDragging = false; // 记录是否正在拖拽
};

#endif
