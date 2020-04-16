#include "historywindow.h"
#include "ui_historywindow.h"

historywindow::historywindow(QWidget *parent, QVector<QString> *calculations) :
    QDialog(parent),
    ui(new Ui::historywindow) {

    ui->setupUi(this);

    calculationList = calculations;
    emptyList.append("NO HISTORY YET");

    if (QList<QString>::fromVector(*calculationList).length() == 0) {
        ui->calculationsList->setModel(new QStringListModel(emptyList));
    }
    else {
       ui->calculationsList->setModel(new QStringListModel(QList<QString>::fromVector(*calculationList)));
    }

    connect(ui->Clear, SIGNAL(released()), this, SLOT(ClearList()));
    connect(ui->Close, SIGNAL(released()), this, SLOT(CloseWindow()));
}

historywindow::~historywindow() {
    delete ui;
}

void historywindow::ClearList() {
    calculationList->clear();
    ui->calculationsList->setModel(new QStringListModel(emptyList));
}

void historywindow::CloseWindow() {
    this->close();
}
