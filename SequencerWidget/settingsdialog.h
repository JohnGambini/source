#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    
signals:
    
public slots:

private:
    Ui::Dialog *ui;

    
};

#endif // SETTINGSDIALOG_H
