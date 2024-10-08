#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    w.show();
    return a.exec();
}

// histogram...
// step 1: make white if occurs, black if doesn't
// step 2: make into gradient
// step 3: make a choice for the "contrast"
// each spot is a pixel... colored based on how many times that pixel color value occurs

// constructor: QImage

//slider change = signal fire -- slider = total of RGB (slices from 0 to 3*255)
//generate all slices at once

//store freq data in QVector<int> hist (2^24 = 1<<24, 0) & have black and white (threashold = 1)
//while regenerate image... use status bar

// build widget


// PLAN: layout, read image as vector, histogram (black for 0 white for all else), contrast slider
