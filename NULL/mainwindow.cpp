#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "historywindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    settingsFile = QStandardPaths::writableLocation(QStandardPaths::CacheLocation).replace("Calculator", "NULL") + "/conig.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);
    QRect StartUpLocation = settings.value("location", QRect()).toRect();

    if (StartUpLocation != QRect()) {
        this->setGeometry(StartUpLocation);
    }

    ui->VersionLabel->setText("Build " + QString(version));
    ui->StatusLabel->clear();

    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];
    for (int i = 0; i < 10; i++) {
        QString btnName = "Button" + QString::number(i);
        numButtons[i] = MainWindow::findChild<QPushButton *>(btnName);
        connect(numButtons[i], SIGNAL(released()), this, SLOT(NumPressed()));
    }

    connect(ui->Display, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(DisplayContextMenu(const QPoint &)));

    connect(ui->Equals, SIGNAL(released()), this, SLOT(EqualButtonPressed()));
    connect(ui->AllClear, SIGNAL(released()), this, SLOT(ClearButton()));
    connect(ui->ClearEntry, SIGNAL(released()), this, SLOT(ClearEntryButton()));
    connect(ui->ChangeSign, SIGNAL(released()), this, SLOT(ChangeNumberSign()));
    connect(ui->Decimal, SIGNAL(released()), this, SLOT(AddDecimalButton()));

    connect(ui->Add, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Substract, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Divide, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Multiply, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Percentage, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->Modulo, SIGNAL(released()), this, SLOT(MathButtonPressed()));
    connect(ui->PowerOf, SIGNAL(released()), this, SLOT(MathButtonPressed()));

    connect(ui->SquareRoot, SIGNAL(released()), this, SLOT(SquareRootButtonPressed()));

    connect(ui->OpenHistory, SIGNAL(released()), this, SLOT(OpenHistoryWindow()));
    connect(ui->VersionLabel, SIGNAL(released()), this, SLOT(OpenAboutWindow()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SaveSettings() {
    QSettings settings(settingsFile, QSettings::IniFormat);
    settings.setValue("location", this->geometry());
}

void MainWindow::closeEvent(QCloseEvent *event) {
    SaveSettings();
    event->accept();
}

void MainWindow::NumPressed() {
    QPushButton *button = static_cast<QPushButton *>(sender());
    QString btnValue = button->text();

    QString displayValue = ui->Display->text();
    if ((displayValue == "0") || (!isNum.exactMatch(displayValue))) {
        ui->Display->clear();
    }

    QString newValue = ui->Display->text() + btnValue;
    ui->Display->setText(newValue);
}

void MainWindow::MathButtonPressed() {
    if (!isNum.exactMatch(ui->Display->text()) || ui->Display->text().isEmpty())
        return;

    if (calcVal != 0 && !ui->Display->text().isEmpty())
        EqualButtonPressed();

    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    percentTrigger = false;
    moduloTrigger = false;
    powerofTrigger = false;

    QString displayValue = ui->Display->text();
    bool changeOperator = displayValue.isEmpty();

    if (!changeOperator) {
        if (displayValue.endsWith('.'))
            displayValue.remove(displayValue.length() - 1, 1);

        calcVal = displayValue.toDouble();

        if (calcVal == 0)
            return;
    }

    QPushButton *button = static_cast<QPushButton *>(sender());
    QString btnText = button->text();

    if (QString::compare(btnText, "+", Qt::CaseInsensitive) == 0) {
        addTrigger = true;
    }
    else if (QString::compare(btnText, "-", Qt::CaseInsensitive) == 0) {
        subTrigger = true;
    }
    else if (QString::compare(btnText, "÷", Qt::CaseInsensitive) == 0) {
        divTrigger = true;
    }
    else if (QString::compare(btnText, "×", Qt::CaseInsensitive) == 0) {
        multTrigger = true;
    }
    else if (QString::compare(btnText, "(per) %", Qt::CaseInsensitive) == 0) {
        percentTrigger = true;
    }
    else if (QString::compare(btnText, "(mod) %", Qt::CaseInsensitive) == 0) {
        moduloTrigger = true;
    }
    else if (QString::compare(btnText, "x²", Qt::CaseInsensitive) == 0) {
        powerofTrigger = true;
        btnText = "^";
    }

    if (!changeOperator) {
        ui->Display->setText("0");
    }

    ui->StatusLabel->setText(QString::number(calcVal) + " " + btnText);
}

void MainWindow::EqualButtonPressed() {
    QString displayValue = ui->Display->text();
    QString calcText;

    if (!isNum.exactMatch(displayValue))
        return;

    double displayV = displayValue.toDouble();
    double result;

    if (displayValue.endsWith('.'))
        displayValue.remove(displayValue.length() - 1, 1);

    if (calcVal != 0 && (addTrigger || subTrigger || divTrigger || multTrigger || percentTrigger || moduloTrigger || powerofTrigger)) {
        if (addTrigger) {
            result = calcVal + displayV;
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "+");
        }
        else if (subTrigger) {
            result = calcVal - displayV;
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "-");
        }
        else if (divTrigger) {
            result = calcVal / displayV;
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "÷");
        }
        else if (multTrigger) {
            result = calcVal * displayV;
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "×");
        }
        else if (percentTrigger) {
            result = calcVal * displayV / 100;
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "(per) %");
        }
        else if (moduloTrigger) {
            result = fmod(calcVal, displayV);
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "(mod) %");
        }
        else {
            result = pow(calcVal, displayV);
            calcText = GenerateCalculationText(QString::number(calcVal), displayValue, "^");
        }

        calculations.push_back(calcText);
        ui->StatusLabel->setText(calcText);
        ui->Display->setText(QString::number(result));
        calcVal = 0;
    }
}

QString MainWindow::GenerateCalculationText(QString num1, QString num2, QString op) {
    QString result;
    bool isNum1Neg = num1.contains('-');
    bool isNum2Neg = num2.contains('-');

    if (isNum1Neg && isNum2Neg) {
        result = "(" + num1 + ") " + op + " (" + num2 + ")";
    }
    else if (isNum1Neg) {
        result = "(" + num1 + ") " + op + " " + num2;
    }
    else if (isNum2Neg) {
        result = num1 + " " + op + " (" + num2 + ")";
    }
    else {
        result = num1 + " " + op + " " + num2;
    }

    return result;
}

void MainWindow::ChangeNumberSign() {
    QString displayValue = ui->Display->text();

    if (isNum.exactMatch(displayValue)) {
        double newVal = -1 * displayValue.toDouble();
        ui->Display->setText(QString::number(newVal));
    }
}

void MainWindow::AddDecimalButton() {
    QString displayValue = ui->Display->text();

    if (!isNum.exactMatch(displayValue))
        return;

    if (displayValue.contains('.')) {
        double result = round(displayValue.toDouble());
        ui->Display->setText(QString::number(result));
    }
    else {
        if (displayValue.isEmpty())
            ui->Display->setText("0");

        ui->Display->setText(ui->Display->text() + ".");
    }
}

void MainWindow::SquareRootButtonPressed() {
    QString displayValue = ui->Display->text();
    QString calcText;

    if (!isNum.exactMatch(displayValue))
        return;

    if (displayValue.endsWith('.'))
        displayValue.remove(displayValue.length() - 1, 1);

    if (displayValue.toDouble() < 0) {
        ui->Display->setText("NaN");
        calcText = "√(" + QString::number(displayValue.toDouble()) + ") = NaN";
    }
    else {
        double result = sqrt(displayValue.toDouble());
        calcText = "√(" + QString::number(displayValue.toDouble()) + ") = " + QString::number(result);
        ui->Display->setText(QString::number(result));
    }

    calculations.push_back(calcText);
}

void MainWindow::OpenHistoryWindow() {
    historywindow histroryWindow(this, &calculations);
    histroryWindow.setModal(true);
    histroryWindow.exec();
}

void MainWindow::OpenAboutWindow() {
    QString aboutInfo = "NULL is an advanced tool for doing your calculations, developed by <strong>BILLVOG</strong>.<br>"
                        "It's made entirely in C++ with Qt's framework. You can find the source code in Github. NULL is licensed under \"<strong>GNU GENERAL PUBLIC LICENSE</strong>\".<br><br>"
                        "<a href=\"https://github.com/billvog\">BILLVOG'S Github</a>, <a href=\"http://w-project.epizy.com/\">W-Project Official</a><br><br>"
                        "(Build " + QString(version) + ")";

    QMessageBox::about(this, "About NULL", aboutInfo);
}

void MainWindow::ClearEntryButton() {
    ui->Display->setText("0");

    if (calcVal == 0)
        ui->StatusLabel->clear();
}

void MainWindow::ClearButton() {
    ui->Display->setText("0");
    ui->StatusLabel->clear();

    calcVal = 0;
    divTrigger = false;
    multTrigger = false;
    addTrigger = false;
    subTrigger = false;
    percentTrigger = false;
    moduloTrigger = false;
    powerofTrigger = false;
}

void MainWindow::DisplayContextMenu(const QPoint &pos) {
    QAction *pasteAction = new QAction("Paste", this);
    pasteAction->setShortcut(QKeySequence(Qt::Key_V | Qt::CTRL));
    pasteAction->setShortcutContext(Qt::ApplicationShortcut);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(PasteValue()));

    QMenu *defaul_contextMenu = ui->Display->createStandardContextMenu();
    QMenu *m_contextMenu = new QMenu(this);
    (*m_contextMenu).addAction(pasteAction);

    m_contextMenu->addActions(defaul_contextMenu->actions());

    (*m_contextMenu).exec(this->mapToGlobal(pos));
}

void MainWindow::PasteValue() {
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString pasteValue = clipboard->text();

    if (isNum.exactMatch(pasteValue)) {
        ui->Display->setText(pasteValue);
    }
    else {
        QMessageBox::warning(this, "Invalid paste value.", "The value you're trying to paste is not a number.", QMessageBox::Ok, QMessageBox::Ok);
    }
}
