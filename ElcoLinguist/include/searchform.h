#pragma once

#include <QWidget>

namespace Ui {
class searchform;
}

class searchform final : public QWidget
{
    Q_OBJECT

public:
    explicit searchform(QWidget *parent = nullptr);
    ~searchform() override;

Q_SIGNALS:
    void textChanged(const QString &str);
    void sensitivityChanged(int value);

private:
    Ui::searchform *ui;
};
