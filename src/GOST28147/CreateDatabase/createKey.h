#ifndef CREATEKEY_H
#define CREATEKEY_H

#include <QtGui/QMainWindow>
#include "ui_createKey.h"
#include "GOST28147/gost28147.h"

class CreateKey : public QDialog
{
    Q_OBJECT

public:
    CreateKey(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~CreateKey();

    gost::key getKey() const;
    gost::replace_key getRKey() const;

private slots:
    void generatePressed();

private:
    Ui::Dialog ui;

    QString intToHex( uint a ) const;

    gost::replace_key rkey;
    gost::key key;
};

#endif // CREATEKEY_H
