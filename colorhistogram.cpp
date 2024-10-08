#include "colorhistogram.h"
#include <QtWidgets>
#include <imageviewer.h>

ColorHistogram::ColorHistogram(const QImage &_image): image(_image), rgbFreq(1<<24, 0), slices(256) {

    // LAYOUTS
    // set the horizontal box
    QHBoxLayout *horizLayout = new QHBoxLayout();
    setLayout(horizLayout);

    // add the ImageViewer to the left
    ImageViewer *imageViewer = new ImageViewer(image);
    horizLayout->addWidget(imageViewer);

    // add the vertical box to the right
    QVBoxLayout *histogramLayout = new QVBoxLayout();
    horizLayout->addLayout(histogramLayout);

    // add the histogram to the top of the right vertical layout
    histogram = new QLabel();
    histogramLayout->addWidget(histogram, 1, Qt::AlignHCenter);

    // add the slider to the top of the bottom right
    colorSlicer = new QSlider(Qt::Horizontal);
    colorSlicer->setTickInterval(8);
    colorSlicer->setRange(0, 255);
    connect(colorSlicer, &QSlider::sliderMoved, this, &ColorHistogram::sliderMoved);
    histogramLayout->addWidget(colorSlicer);

    // add shade color choice
    scaleChoice = new QComboBox;
    histogramLayout->addWidget(scaleChoice);
    scaleChoice->addItem("1");
    scaleChoice->addItem("2");
    scaleChoice->addItem("4");
    scaleChoice->addItem("8");
    scaleChoice->addItem("16");
    scaleChoice->addItem("32");
    scaleChoice->addItem("64");
    scaleChoice->addItem("128");
    scaleChoice->addItem("255");
    connect(scaleChoice, &QComboBox::currentTextChanged, this, &ColorHistogram::comboBoxChanged);

    // add choice of color to the bottom of the bottom right
    colorChoice = new QComboBox();
    histogramLayout->addWidget(colorChoice);
    colorChoice->addItem("Red");
    colorChoice->addItem("Green");
    colorChoice->addItem("Blue");
    connect(colorChoice, &QComboBox::currentTextChanged, this, &ColorHistogram::comboBoxChanged);

    getRgbFreq();
    buildPixmap();
}

void ColorHistogram::getRgbFreq() {
    //READ IMAGE AS VECTOR: loop through the image to find freqencies
    for (int iRow=0; iRow < image.height(); ++iRow) {
        for (int iCol=0; iCol < image.width(); ++iCol) {
            QRgb color = image.pixel(iCol, iRow); // 0xFFRRGGBB
            int rgbColor = color & 0xFFFFFF;
            ++rgbFreq[rgbColor];
        }
    }
}

void ColorHistogram::buildPixmap() {

    if (colorChoice->currentText() == "Red") { // green = col, blue = row
        for (int iSlice = 0; iSlice < 256; ++iSlice) { //slider value
            QImage blank(256, 256, QImage::Format_RGB32);
            for (int iCol = 0; iCol < 256; ++iCol) {
                for (int iRow = 0; iRow < 256; ++iRow) {
                    int freq = rgbFreq[(iSlice<<16)|(iCol<<8)|iRow];
                    freq *= scaleChoice->currentText().toInt();
                    if (freq > 255) freq = 255;
                    blank.setPixel(iCol, iRow, freq * 0x010101);
                }
            }
            slices[iSlice] = QPixmap::fromImage(blank);
        }
    }

    if (colorChoice->currentText() == "Green") { // red = col, blue = row
        for (int iSlice = 0; iSlice < 256; ++iSlice) {
            QImage blank(256, 256, QImage::Format_RGB32);
            for (int iCol = 0; iCol < 256; ++iCol) {
                for (int iRow = 0; iRow < 256; ++iRow) {
                    int freq = rgbFreq[(iCol<<16)|(iSlice<<8)|iRow];
                    freq *= scaleChoice->currentText().toInt();
                    if (freq > 255) freq = 255;
                    blank.setPixel(iCol, iRow, freq * 0x010101);
                }
            }
            slices[iSlice] = QPixmap::fromImage(blank);
        }
    }
    if (colorChoice->currentText() == "Blue") { // red = col, green = row
        for (int iSlice = 0; iSlice < 256; ++iSlice) {
            QImage blank(256, 256, QImage::Format_RGB32);
            for (int iCol = 0; iCol < 256; ++iCol) {
                for (int iRow = 0; iRow < 256; ++iRow) {
                    int freq = rgbFreq[(iCol<<16)|(iRow<<8)|iSlice];
                    freq *= scaleChoice->currentText().toInt();
                    if (freq > 255) freq = 255;
                    blank.setPixel(iCol, iRow, freq * 0x010101);
                }
            }
            slices[iSlice] = QPixmap::fromImage(blank);
        }
    }
    sliderMoved();
}

void ColorHistogram::sliderMoved() {
    histogram->setPixmap(slices[colorSlicer->sliderPosition()]);
}

void ColorHistogram::comboBoxChanged() {
    buildPixmap();
}


// SUM OF NUMBERS ADDITION:
// slider goes from 0 - 3*255
// QVector<pixmaps> needs 3*255 elements
// histogram needs to be made with loop 0-3*255, for each one, 256x256 image set similar to before
// except the color value being looked up is
// if red, green and blue are col and row, red is value is the sum minus the green & blue
// if red is less than 0 or more than 255, treat is as 0
// otherwise look up in freq table

// sum = 0 while sum <= 3*255
// inner loop: compute missing color freq
