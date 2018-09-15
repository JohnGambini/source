#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "timingmasterwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void changeStyle(const QString &styleName);
    void changePalette();
    void ShowSettings();
    void Exit();
    void About();
    void errMsg(QString header, QString msg);
    void errLog(QString errMsg);
    void transportButtonPressed(int value);


private:
    void loadApplicationSettings();
    void saveApplicationSettings();
    void openMidiPorts();
    void closeMidiPorts();


private:
    Ui::MainWindow *ui;
    gnaFile m_errLog;
    gnaTreeItem m_applicationSettings;
    QString m_applicationFilePath;
    QString m_projectFilesDirectory;
    TimingMasterWidget * m_timingMaster;
};

#endif // MAINWINDOW_H
