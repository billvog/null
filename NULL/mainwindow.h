#pragma once

#include <QMainWindow>
#include <QMenu>
#include <QClipboard>
#include <QMessageBox>
#include <QVector>
#include <QSettings>
#include <QStandardPaths>
#include <QCloseEvent>
#include <QDebug>
#include <math.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const char* version = "1.0";


private:
    Ui::MainWindow *ui;

    QString settingsFile;

    QVector<QString> calculations;
    double calcVal = 0;
    bool divTrigger = false;
    bool multTrigger = false;
    bool addTrigger = false;
    bool subTrigger = false;
    bool percentTrigger = false;
    bool moduloTrigger = false;
    bool powerofTrigger = false;

    QRegExp isNum = QRegExp("[-]?[0-9.]*");

private slots:
    void NumPressed();
    void MathButtonPressed();
    void EqualButtonPressed();
    void ChangeNumberSign();
    void ClearButton();
    void ClearEntryButton();
    void AddDecimalButton();
    void SquareRootButtonPressed();
    QString GenerateCalculationText(QString num1, QString num2, QString op);

    void OpenHistoryWindow();
    void OpenAboutWindow();

    void DisplayContextMenu(const QPoint &pos);
    void PasteValue();

    void SaveSettings();

protected:
    void closeEvent(QCloseEvent *event);
};
