#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "gnatreeitem.h"

//#include "projectview.h"
#include "sequencerview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void setDocumentName();
    void doClear();
    void doSave();
    void doLoad();
    ~MainWindow();

public slots:
    void changeStyle(const QString &styleName);
    void changePalette();
    void dataChanged();
    void New();
    void Open();
    void Save();
    void SaveAs();
    void Exit();
    void properties();
    void About();

private:
    Ui::MainWindow *ui;
    QString m_applicationName;
    QString m_windowTitle;
    QString m_filePath;
    QString m_documentName; //File Name
    gnaTreeItem m_Root;
    QPalette originalPalette;

    //ProjectView * m_projectView;
    SequencerView * m_sequencerView;
};

#endif // MAINWINDOW_H
