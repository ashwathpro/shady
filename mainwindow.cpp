/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "glwidget.h"
#include "mainwindow.h"
#include "SampleShape.h"
#include "spineshape.h"
#include "meshshape.h"
#include "Canvas.h"
#include "Patch.h"
#include "customdialog.h"

#include <QtWidgets>

MainWindow::MainWindow()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    glWidget = new GLWidget;

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(50, 50);

    createActions();
    createMenus();

    initTools();

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(glWidgetArea, 0, 0);

    centralWidget->setLayout(centralLayout);
    setWindowTitle(tr("Shady"));
    resize(1200, 900);
    initScene();
}

void MainWindow::initTools()
{

    QToolBar *toolbar = addToolBar("main toolbar");

    toolbar->addAction(shapeInsertGridAct);
    toolbar->addAction(shapeInsert2NGonAct);
    toolbar->addAction(shapeInsertSpineAct);

    toolbar->addSeparator();

    toolbar->addAction(dragAct);

    toolbar->addSeparator();

    toolbar->addAction(extrudeEdgeAct);
    toolbar->addAction(extrudeFaceAct);
    toolbar->addAction(insertSegmentAct);
    toolbar->addAction(deleteFaceAct);

    toolbar->addSeparator();
}

void MainWindow::initScene(){
    //init scene
    Canvas::get()->initLights();
    Patch::setN(8);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Shady"), tr("<b>Shady</b> is an application in progress that implements theoretical framework developed atTexas A&M University for rendering 2D scenes as if they are part of 3D."));
}

void MainWindow::createActions()
{
    //file Actions
    fileNewAct = new QAction(tr("New"), this);
    fileNewAct->setShortcuts(QKeySequence::New);
    connect(fileNewAct, SIGNAL(triggered()), this, SLOT(newFile()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    dragAct = new QAction(tr("Drag"), this);
    dragAct->setShortcut(Qt::Key_Space);
    connect(dragAct, SIGNAL(triggered()), this, SLOT(flipDrag()));

    //view Act
    shadingOnAct = new QAction(tr("&Shading On"), this);
    shadingOnAct->setShortcut('S');
    shadingOnAct->setCheckable(true);
    connect(shadingOnAct, SIGNAL(triggered()), this, SLOT(toggleShading()));

    normalsOnAct = new QAction(tr("Show &Normals"), this);
    normalsOnAct->setShortcut('N');
    normalsOnAct->setCheckable(true);
    connect(normalsOnAct, SIGNAL(triggered()), this, SLOT(toggleNormals()));

    patchesOnAct = new QAction(tr("Show &Patches"), this);
    patchesOnAct->setShortcut('P');
    patchesOnAct->setCheckable(true);
    connect(patchesOnAct, SIGNAL(triggered()), this, SLOT(togglePathces()));

    //Mesh Shape Actions
    extrudeEdgeAct = new QAction(tr("&Extrude Edge"), this);
    extrudeEdgeAct->setShortcut(tr("Ctrl+E"));
    connect(extrudeEdgeAct, SIGNAL(triggered()), this, SLOT(selectExtrudeEdge()));

    extrudeFaceAct = new QAction(tr("Extrude &Face"), this);
    extrudeFaceAct->setShortcut(tr("Ctrl+Q"));
    connect(extrudeFaceAct, SIGNAL(triggered()), this, SLOT(selectExtrudeFace()));

    insertSegmentAct = new QAction(tr("&Insert Segment"), this);
    insertSegmentAct->setShortcut(tr("Ctrl+I"));
    connect(insertSegmentAct, SIGNAL(triggered()), this, SLOT(selectInsertSegment()));

    deleteFaceAct = new QAction(tr("&Delete Face"), this);
    deleteFaceAct->setShortcut(tr("Ctrl+D"));
    connect(deleteFaceAct, SIGNAL(triggered()), this, SLOT(selectDeleteFace()));

    QActionGroup* toolset = new QActionGroup(this);

    dragAct->setCheckable(true);
    dragAct->setChecked(true);

    extrudeEdgeAct->setCheckable(true);
    extrudeFaceAct->setCheckable(true);
    insertSegmentAct->setCheckable(true);
    deleteFaceAct->setCheckable(true);

    extrudeEdgeAct->setActionGroup(toolset);
    extrudeFaceAct->setActionGroup(toolset);
    insertSegmentAct->setActionGroup(toolset);
    deleteFaceAct->setActionGroup(toolset);

    //SHAPE ACTIONS
    shapeInsertGridAct = new QAction(tr("Grid"), this);
    connect(shapeInsertGridAct, SIGNAL(triggered()), this, SLOT(newGrid()));

    shapeInsert2NGonAct = new QAction(tr("2NGon"), this);
    connect(shapeInsert2NGonAct, SIGNAL(triggered()), this, SLOT(new2NGon()));

    shapeInsertSpineAct = new QAction(tr("Spine"), this);
    connect(shapeInsertSpineAct, SIGNAL(triggered()), this, SLOT(newSpine()));

    shapeLockAct = new QAction(tr("&Lock"), this);
    shapeLockAct->setShortcut(tr("Ctrl+L"));
    connect(shapeLockAct, SIGNAL(triggered()), this, SLOT(toggleLockShape()));

    shapeMoveFrontAct = new QAction(tr("Move Front"), this);
    shapeMoveFrontAct->setShortcut(Qt::Key_PageUp);
    connect(shapeMoveFrontAct, SIGNAL(triggered()), this, SLOT(moveShapeToFront()));

    shapeMoveBackAct = new QAction(tr("Move Back"), this);
    shapeMoveBackAct->setShortcut(Qt::Key_PageDown);
    connect(shapeMoveBackAct, SIGNAL(triggered()), this, SLOT(moveShapeToBack()));


    shapeSendFrontAct = new QAction(tr("Send &Front"), this);
    shapeSendFrontAct->setShortcut(tr("Ctrl+F"));
    connect(shapeSendFrontAct, SIGNAL(triggered()), this, SLOT(sendShapeFront()));

    shapeSendBackAct = new QAction(tr("Send &Back"), this);
    shapeSendBackAct->setShortcut(tr("Ctrl+B"));
    connect(shapeSendBackAct, SIGNAL(triggered()), this, SLOT(sendShapeBack()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));   
    fileMenu->addAction(fileNewAct);
    fileMenu->addAction("Open");
    fileMenu->addAction("Save");
    fileMenu->addAction("Save as");

    fileMenu->addAction("Import");
    fileMenu->addAction("Export");

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    viewMenu  = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(normalsOnAct);
    viewMenu->addAction(patchesOnAct);
    viewMenu->addAction(shadingOnAct);

    shapeMenu  = menuBar()->addMenu(tr("&Shape"));

    insertMenu =  shapeMenu->addMenu("Insert");
    shapeMenu->addAction(shapeLockAct);

    QMenu* arrangeMenu = shapeMenu->addMenu("Arrange");
    arrangeMenu->addAction(shapeMoveFrontAct);
    arrangeMenu->addAction(shapeMoveBackAct);
    arrangeMenu->addAction(shapeSendFrontAct);
    arrangeMenu->addAction(shapeSendBackAct);

    shapeMenu->addAction("Group");
    shapeMenu->addAction("Parent");
    shapeMenu->addAction("Transform");
    shapeMenu->addAction("Rasterize");

    insertMenu->addAction(shapeInsertGridAct);
    insertMenu->addAction(shapeInsert2NGonAct);
    insertMenu->addAction(shapeInsertSpineAct);

    toolsMenu  = menuBar()->addMenu(tr("Tools"));
    toolsMenu->addAction(extrudeEdgeAct);
    toolsMenu->addAction(extrudeFaceAct);
    toolsMenu->addAction(insertSegmentAct);
    toolsMenu->addAction(deleteFaceAct);

    selectMenu  = menuBar()->addMenu(tr("Select"));
    selectMenu->addAction("Select All");
    selectMenu->addAction("Select Inverse");
    selectMenu->addAction("Grow Selection");
    selectMenu->addAction("Clear Selection");

    renderMenu  = menuBar()->addMenu(tr("Render"));
    renderMenu->addAction("Render Options");
    renderMenu->addAction("Render");

    helpMenu    = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter pixmap size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
            return QSize(width, height);
    }

    return glWidget->size();
}

void MainWindow::keyPressEvent(QKeyEvent *event){

    bool ctrl = (event->modifiers() & Qt::ControlModifier);

    int key = event->key();

    if (key > 48 && key < 57 )
    {
        Canvas::MODE = (Canvas::EditMode_e)(key - 49);
    }

    switch(key){

        case Qt::Key_Delete :
             Canvas::get()->removeActive();
        break;

        case Qt::Key_N:
            {
                MeshShape* ms = new MeshShape();
                Canvas::get()->insert(ms);
                Canvas::get()->activate(ms);
            }
        break;

       case Qt::Key_PageUp:
            Canvas::get()->activeUp();
       break;

        case Qt::Key_PageDown:
             Canvas::get()->activeDown();
        break;

    }
    glWidget->updateGL();
}

void MainWindow::newFile(){
    Canvas::get()->clear();
    glWidget->updateGL();
}


void MainWindow::selectExtrudeEdge()
{
    MeshShape::setOPMODE(MeshShape::EXTRUDE_EDGE);
    unselectDrag();
}

void MainWindow::selectExtrudeFace()
{
    MeshShape::setOPMODE(MeshShape::EXTRUDE_FACE);
    unselectDrag();
}

void MainWindow::selectDeleteFace()
{
    MeshShape::setOPMODE(MeshShape::DELETE_FACE);
    unselectDrag();
}

void MainWindow::selectInsertSegment()
{
    createCustomDialog("Insert Segment", "input1","input2","input3");

    MeshShape::setOPMODE(MeshShape::INSERT_SEGMENT);
    unselectDrag();
}

void MainWindow::flipDrag()
{
    Canvas::get()->flipMode();
    glWidget->updateGL();
}

void MainWindow::unselectDrag()
{
    Canvas::get()->isDragMode = false;
    dragAct->setChecked(false);
    glWidget->updateGL();
}

void MainWindow::toggleNormals(){
    Canvas::get()->isNormalsOn = normalsOnAct->isChecked();
    glWidget->updateGL();
}

void MainWindow::togglePathces(){
    Canvas::get()->isWireframeOn = patchesOnAct->isChecked();
    glWidget->updateGL();
}

void MainWindow::toggleShading(){
    Canvas::get()->isShadingOn = shadingOnAct->isChecked();
    glWidget->updateGL();
}



//Shape Menu Call backs

void MainWindow::new2NGon()
{
    createCustomDialog("Create NGon", "input1","input2","input3");
    MeshShape* pM = MeshShape::newMeshShape(Point(0,0), MeshShape::NGON);
    Canvas::get()->insert(pM);
    glWidget->updateGL();
}

void MainWindow::newGrid()
{
    createCustomDialog("Create New Grid", "input1","input2","input3");
    MeshShape* pM = MeshShape::newMeshShape(Point(0,0), MeshShape::SQUARE);
    Canvas::get()->insert(pM);
    glWidget->updateGL();
}

void MainWindow::newSpine()
{
    createCustomDialog("Create New Spine", "input1","input2","input3");
    //MeshShape* pM = MeshShape::newMeshShape(Point(0,0),MeshShape::SPINE);
    //Canvas::get()->insert(pM);
}

void MainWindow::toggleLockShape(){
    Shape_p shape = Canvas::get()->active();
    if (!shape)
        return;
    shape->flipLock();
    glWidget->updateGL();
}

void MainWindow::transformShape(){
    Shape_p shape = Canvas::get()->active();
    if (!shape)
        return;
    //transform
    glWidget->updateGL();
}

void MainWindow::parentShape(){
    Shape_p shape = Canvas::get()->active();
     if (!shape)
         return;


     glWidget->updateGL();
}

void MainWindow::groupShape(){
    Shape_p shape = Canvas::get()->active();
     if (!shape)
         return;
}

void MainWindow::moveShapeToFront(){
    Canvas::get()->activeUp();
    glWidget->updateGL();
}

void MainWindow::moveShapeToBack(){
    Canvas::get()->activeDown();
    glWidget->updateGL();
}

void MainWindow::sendShapeBack(){
    Canvas::get()->sendToBack();
    glWidget->updateGL();
}

void MainWindow::sendShapeFront(){
    Canvas::get()->sendToFront();
    glWidget->updateGL();
}

void MainWindow::createCustomDialog(QString title, QString input1,QString input2,QString input3)
{
    string  Value0 = "Value";            // NOTE: these lines of code (the variables you wish to change)
    bool    Value1  = true;              //  probably exist in your program already and so it is only
    int     Value2      = 20;
    int     Value3 = 1;
    int     ScaleValue = 10;
    CustomDialog d(title, this);                            // Title for custom dialog
    d.addLabel    ("Please enter the details below ...");           // The first element is just a text label (non interactive).
    d.addLineEdit (input1+"  ", &Value0, "Tooltip value");             // line edit.
    d.addCheckBox (input2+"  ", &Value1, "my tooltip");       // A checkbox with a tooltip (optional last argument).
    d.addSpinBox  (input3+"  ", 1, 120, &Value2, 1);                   // A number spin box for age.
    d.addComboBox ("Value: ", "Value1|Value2|Value3", &Value3);   // A combo box with three options (separated by '|').

    scaleSetting = new QSlider(Qt::Horizontal, this);
//            output = new Output();

    scaleSetting->setRange(1, 20);
    scaleSetting->setValue(5);
    connect(scaleSetting, SIGNAL(valueChanged(int)), this, SLOT(SetScale(int)));

    d.addSlider(scaleSetting,tr("Scale Setting"),&ScaleValue,tr("Scale"));

    d.exec();                             // Execution stops here until user closes dialog

    if(d.wasCancelled())                // If the user hit cancel, your values stay the same
        return;                           // and you probably don't need to do anything
//     cout << "Thanks " << name << end;   // and here it's up to you to do stuff with your new values!
}
