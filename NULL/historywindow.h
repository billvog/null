#pragma once
#include <QDialog>
#include <QVector>
#include <QStringListModel>
#include <QAbstractListModel>

namespace Ui {
class historywindow;
}

class historywindow : public QDialog {
    Q_OBJECT

public:
    explicit historywindow(QWidget *parent = nullptr, QVector<QString> *calculations = nullptr);
    ~historywindow();

private:
    Ui::historywindow *ui;
    QVector<QString> *calculationList;
    QList<QString> emptyList;

private slots:
    void ClearList();
    void CloseWindow();
};
