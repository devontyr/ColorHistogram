#ifndef COLORHISTOGRAM_H
#define COLORHISTOGRAM_H
#include <QtWidgets>

class ColorHistogram : public QWidget
{
    Q_OBJECT
    QImage image;

    QLabel *histogram;
    QSlider *colorSlicer;
    QComboBox *scaleChoice;
    QComboBox *colorChoice;

    QVector<int> rgbFreq;
    QVector<QPixmap> slices;

public:
    ColorHistogram(const QImage &_image);
    void getRgbFreq();
    void buildPixmap();

public slots:
    void sliderMoved();
    void comboBoxChanged();
};

#endif // COLORHISTOGRAM_H
