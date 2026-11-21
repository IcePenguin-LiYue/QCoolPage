/*****************************************
 * 作者: YYC
 * 日期: 2020-04-26
 * 功能：3D场景，用于3D模型的显示
 * ***************************************/

#include "openglcontrol.h"
#include <QMouseEvent>
#include <QVector3D>
#include <QDebug>
#include "opengldef.h"
#include <QtGlobal>

OpenglControl::OpenglControl(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

OpenglControl::~OpenglControl()
{
    this->makeCurrent();
    for (ObjectParent* objectModel : m_objectList)
    {
        objectModel->deleteLater();
    }
    this->doneCurrent();
}

void OpenglControl::initializeGL()
{
    this->initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    const int OBJECT_NUMBER = 35;
    for (int i = 0; i < OBJECT_NUMBER; i++)
    {
        m_objectList.append(OBJECT_FACTORY->makeObject(ObjectFactory::OBJECT_CUBE));
    }

    m_cameraLibrary.setupCamera(OpenglSpace::CAMERA_POS, OpenglSpace::TARGET_POS, OpenglSpace::WORLD_UP);
    for (int i = 0; i < m_objectList.size(); i++)
    {
        m_objectList.at(i)->setWindowSize(width(), height());
        m_objectList.at(i)->setCameraLibrary(&m_cameraLibrary);
#ifdef Q_OS_WIN
        m_objectList.at(i)->setupShader(OpenglSpace::VERTEX_WIN_PATH, OpenglSpace::FRAMENT_WIN_PATH);
#else
        m_objectList.at(i)->setupShader(OpenglSpace::VERTEX_UNIX_PATH, OpenglSpace::FRAMENT_UNIX_PATH);
#endif
        m_objectList.at(i)->setupPerspective(45.0f);
        m_objectList.at(i)->makeObject();
        m_objectList.at(i)->setOpenGLFunctions(this);
        m_objectList.at(i)->setOpenGLWidget(this);
        m_objectList.at(i)->setupTexture(OpenglSpace::CONTAINER_PATH);
        m_objectList.at(i)->translateByX(rand() % 50 - 25.0f);
        m_objectList.at(i)->translateByY(rand() % 50 - 25.0f);
        m_objectList.at(i)->translateByZ(rand() % 100);
        m_objectList.at(i)->rotateByY(rand() % 100 - 100.0f);
    }
}

void OpenglControl::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenglControl::paintGL()
{
    glClearColor(static_cast<GLclampf>(OpenglSpace::CLEAR_COLOR.redF())
                 , static_cast<GLclampf>(OpenglSpace::CLEAR_COLOR.greenF())
                 , static_cast<GLclampf>(OpenglSpace::CLEAR_COLOR.blueF())
                 , static_cast<GLclampf>(OpenglSpace::CLEAR_COLOR.alphaF()));


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto objectModel : m_objectList)
    {
        objectModel->drawObject();
    }
    glFlush();
}

void OpenglControl::wheelEvent(QWheelEvent *wheelEvent)
{
    if(wheelEvent->delta() > 0)
    {
        m_cameraLibrary.moveToForward();
    }
    else
    {
        m_cameraLibrary.moveToBackward();
    }
    this->update();
}

void OpenglControl::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_isDragging = true;
        m_lastPos = event->pos();
    }
}

void OpenglControl::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_isDragging)
    {
        return ;
    }

    // 计算鼠标移动的增量（距离上一次移动了多少）
    float deltaX = event->pos().x() - m_lastPos.x();
    float deltaY = event->pos().y() - m_lastPos.y();


    // 左键拖拽：平移相机（或者你也可以用中键）
    if (event->buttons() == Qt::LeftButton)
    {
        // 判断往哪边拖（这里按你底层库函数自己调，注意左右方向可能需要乘 -1 或不变）
        if (deltaX > 0)
            m_cameraLibrary.moveToLeft();
        else if (deltaX < 0)
            m_cameraLibrary.moveToRight();

        if (deltaY > 0)
            m_cameraLibrary.moveToUp();
        else if (deltaY < 0)
            m_cameraLibrary.moveToDown();
    }

    // 更新鼠标位置，准备下一次计算
    m_lastPos = event->pos();

    // 触发重绘
    this->update();
}

void OpenglControl::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isDragging = false; // 踩刹车，重置状态
    }
}
