#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <list>
#include <QStringList>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_solveButton_clicked()
{
    int N = ui->nspinBox->value();
    int M = ui->mspinBox->value();

    vector<QStringList> states = josephusProblem(N, M);

    ui->tableWidget->setRowCount(states.size());
    ui->tableWidget->setColumnCount(states.size() > 0 ? states[0].size() : 0);

    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < states[i].size(); ++j) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(states[i][j]));
        }
    }

    QFile file("result.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream fout(&file);
        fout << "Последний оставшийся человек имеет номер " << states.back().back();
        file.close();
    } else {
        ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Не удалось открыть файл"));
    }
}

vector<QStringList> MainWindow::josephusProblem(int N, int M) {
    vector<QStringList> states;
    list<int> people;

    for (int i = 1; i <= N; i++) {
        people.push_back(i);
    }

    auto current = people.begin();

    while (people.size() > 1) {
        QStringList currentState;
        for (const int &person : people) {
            currentState << QString::number(person);
        }
        states.push_back(currentState);

        for (int count = 1; count < M; count++) {
            current++;
            if (current == people.end()) {
                current = people.begin();
            }
        }

        current = people.erase(current);
        if (current == people.end()) {
            current = people.begin();
        }
    }

    QStringList finalState;
    finalState << QString::number(people.front());
    states.push_back(finalState);

    return states;
}
