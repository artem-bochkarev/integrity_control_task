#ifndef SIMPLERUNNER_H
#define SIMPLERUNNER_H

#include <QtGui/QMainWindow>
#include "ui_simplerunner.h"
#include "Database/Database.h"

class SimpleRunner : public QMainWindow
{
    Q_OBJECT

public:
    SimpleRunner(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~SimpleRunner();

private slots:
    void selectPressed();
    void runPressed();

private:
    Ui::SimpleRunnerClass ui;
    DatabasePtr database;

    void changeSlashes( QString& ) const;
};

#endif // SIMPLERUNNER_H
