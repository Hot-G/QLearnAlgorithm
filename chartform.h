#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QDialog>
#include <vector>
#include <QPainter>

using namespace std;

namespace Ui {
class ChartForm;
}

class ChartForm : public QDialog
{
    Q_OBJECT

public:
    explicit ChartForm(QWidget *parent = nullptr);
    ~ChartForm();
    void SetData(vector<int> data);
    void SetTitle(QString _title);
    virtual void paintEvent(QPaintEvent* event);

private:
    Ui::ChartForm *ui;
    QString title;
    vector<int> Data;
    int MaxData;
    const int space = 200;

};

#endif // CHARTFORM_H
