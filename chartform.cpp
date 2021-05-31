#include "chartform.h"
#include "ui_chartform.h"

ChartForm::ChartForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChartForm)
{
    ui->setupUi(this);
}

ChartForm::~ChartForm()
{
    delete ui;
}

void ChartForm::SetData(vector<int> data){
    Data = data;
    MaxData = 0;

    for(int i = 0, n = Data.size();i < n;i++){
        if(Data.at(i) > MaxData)
            MaxData = Data.at(i);
    }
}

void ChartForm::SetTitle(QString _title)
{
    title = _title;
}

void ChartForm::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    QPen pen(Qt::green);
    pen.setWidth(2);

    paint.setPen(pen);

    for(int i = 0, n = Data.size();i < n;i++){
            paint.drawLine(i + space, space, i + space, (double)Data.at(i) * 300.0 / MaxData + space);
    }

    pen.setColor(Qt::white);
    paint.setPen(pen);

    paint.drawLine(space, space, space + Data.size() + 50, space);
    paint.drawLine(space, space, space, 450);

    paint.drawText(QPoint(space - 10, space - 10), "0");
    paint.drawText(QPoint(space + Data.size() - 5, space - 10), QString::number(Data.size()));
    paint.drawText(QPoint(20, 300.0 + space), QString::number(MaxData));

    paint.drawText(QPoint(250, 40), title);
}
