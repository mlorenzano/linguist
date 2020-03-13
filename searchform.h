#ifndef SEARCHFORM_H
#define SEARCHFORM_H

#include <QWidget>

namespace Ui {
class searchform;
}

class searchform : public QWidget
{
    Q_OBJECT

public:
    explicit searchform(QWidget *parent = nullptr);
    ~searchform();

private:
    Ui::searchform *ui;
};

#endif // SEARCHFORM_H
