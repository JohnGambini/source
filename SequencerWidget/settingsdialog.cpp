#include "settingsdialog.h"
#include "ui_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}
