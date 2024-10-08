#include "mainwindow.h"
#include <QtWidgets>
#include <imageviewer.h>
#include <colorhistogram.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    setWindowTitle("Color Histogram");

    // OPEN AN IMAGE FILE
    QAction *openImageAct = new QAction("open image file");
    connect(openImageAct, &QAction::triggered, this, &MainWindow::openImageSlot);
    openImageAct->setShortcut(Qt::CTRL | Qt::Key_O); //slot triggered with ctrl O key

    QMenu *fileMenu = new QMenu("&file");
    fileMenu->addAction(openImageAct);
    menuBar()->addMenu(fileMenu);

    QSettings settings("DRT", "graphics");
    lastDir = settings.value("lastDirectory", "").toString();
}

MainWindow::~MainWindow() {
    QSettings settings("DRT", "graphics");
    settings.value("lastDirectory", lastDir);
}

void MainWindow::openImageSlot() {
    //open file as pixmap and put on screen
    QString fName = QFileDialog::getOpenFileName(this, "select image file", lastDir, "image files (*.png *.jpg *.bmp *.jpeg)");
    if (fName.isEmpty()) return;
    QImage image(fName);
    if (image.isNull()) return;

    lastDir = QFileInfo(fName).absolutePath(); //figure out what direcotry it is in and save for later

    ColorHistogram *colorHistogram = new ColorHistogram(image);
    setCentralWidget(colorHistogram);
}
