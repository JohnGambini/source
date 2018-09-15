#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gnatreeitem.h"
#include "eventlistwidget.h"

#include <QMainWindow>

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
    void pollQueue();
    void Exit();
    void properties();
    void About();
    void loadApplicationSettings();
    void saveApplicationSettings();
    void openMidiPorts();
    void closeMidiPorts();

private:
    Ui::MainWindow *ui;
    QString m_applicationName;
    QString m_windowTitle;
    QString m_applicationFilePath;
    QString m_projectFilesDirectory;
    gnaTreeItem m_applicationSettings;
    gnaTreeItem m_Root;

    EventListWidget * m_eventList;
};

#endif // MAINWINDOW_H
