#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStyleFactory>
#include <QScrollArea>
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>

#include <settingsdialog.h>

//#include "datamodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_applicationName = "SequencerWidget";
    m_windowTitle = "SequencerWidget[*]";
    gnaVariant header = "Sequence List";
    m_Root.setData(0,header);

    connect(ui->actionNew,SIGNAL(triggered()), this, SLOT(New()));
    connect(ui->actionOpen,SIGNAL(triggered()), this, SLOT(Open()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(Save()));
    connect(ui->actionSave_as, SIGNAL(triggered()), this, SLOT(SaveAs()));
    connect(ui->actionExit,SIGNAL(triggered()), this, SLOT(Exit()));
    connect(ui->actionProperties,SIGNAL(triggered()),this,SLOT(properties()));
    //connect(ui->actionUndo, SIGNAL(triggered()), this, SLOT(Undo()));
    //connect(ui->actionRedo, SIGNAL(triggered()), this, SLOT(Redo()));
    //connect(ui->actionInstruments, SIGNAL(triggered()), this, SLOT(Instruments()));
    //connect(ui->actionDirectories, SIGNAL(triggered()), this, SLOT(Directories()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(About()));

    m_sequencerView = new SequencerView;

    setCentralWidget(m_sequencerView);

    m_sequencerView->addTrack(0);
    m_sequencerView->addTrack(1);
    m_sequencerView->addTrack(2);
    m_sequencerView->addTrack(3);

    gnaVariant data = "SequencerWidget";
    gnaTreeItem * sequencerItem = m_Root.addChild(new gnaTreeItem(data));
    m_sequencerView->setRoot(sequencerItem);

    /*
    QSplitter * centralWidget = new QSplitter;
    centralWidget->setHandleWidth(6);

    // Left Pane
    m_projectView = new ProjectView(new DataModel);
    centralWidget->addWidget(m_projectView);
    // Right Pane
    m_sequencerView = new SequencerView;
    centralWidget->addWidget(m_sequencerView);

    setCentralWidget(centralWidget);

    QList<int> iSizes2;
    iSizes2 << 100 << 750;
    centralWidget->setSizes(iSizes2);

    m_projectView->setRoot(&m_Root);

    m_sequencerView->addTrack(0);
    m_sequencerView->addTrack(1);
    m_sequencerView->addTrack(2);
    m_sequencerView->addTrack(3);
    m_sequencerView->addTrack(4);
    m_sequencerView->addTrack(5);

    connect(m_projectView,SIGNAL(newDocument()), this, SLOT(New()));
    connect(m_projectView,SIGNAL(openDocument()), this, SLOT(Open()));
    connect(m_projectView, SIGNAL(saveDocument()), this, SLOT(Save()));

    connect(m_projectView, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    */
    connect(m_sequencerView, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
    /*
    connect(m_projectView, SIGNAL(newRoot(gnaTreeItem *)), m_sequencerView, SLOT(setRoot(gnaTreeItem *)));
    connect(m_projectView,SIGNAL(itemDeleted(gnaTreeItem*)),m_sequencerView, SLOT(itemDeleted(gnaTreeItem*)));
    */

    //changeStyle("Motif");
    //changeStyle("CDE");
    //changeStyle("cleanLooks");
    changeStyle("Plastique");
    changePalette();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeStyle(const QString &styleName)
{
    QApplication::setStyle(QStyleFactory::create(styleName));
}

void MainWindow::changePalette()
{
    QApplication::setPalette(QApplication::style()->standardPalette());
}

void MainWindow::dataChanged()
{
    setWindowModified(true);
}

void MainWindow::setDocumentName()
{
    int pos = m_filePath.lastIndexOf("/",-1);
    if( pos != -1 )
        m_documentName = m_filePath.right(m_filePath.length() - pos - 1 );
    else
        m_documentName = m_filePath;

    pos = m_documentName.lastIndexOf(".",-1);
    if( pos != -1)
        m_documentName = m_documentName.left(pos);

    QString wTitle = m_documentName;
    wTitle += "[*]";
    setWindowTitle(wTitle);
}

void MainWindow::doClear()
{
    m_Root.clear();
    gnaVariant header = "Sequences";
    m_Root.setData(0,header);
    gnaVariant data = "SequencerWidget";
    gnaTreeItem * sequencerItem = m_Root.addChild(new gnaTreeItem(data));
    m_sequencerView->setRoot(sequencerItem);

    setWindowModified(false);
    setWindowTitle(m_windowTitle);

}

void MainWindow::doSave()
{
    gnaFile aFile;
    if( aFile.Open(m_filePath.toStdString(), gna::FileWrite))
    {
        m_Root.Serialize(aFile);
        setWindowModified(false);
        setDocumentName();
    }
    else
    {
        QString errStrg = "<b>";
        errStrg += m_applicationName;
        errStrg +=  "<b> could not save the document to the specified file name.";
        QMessageBox::information(this, "Error saving file.", errStrg );
    }
}

void MainWindow::doLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        m_filePath = fileName;
        gnaFile aFile;
        if(aFile.Open(m_filePath.toStdString(), gna::FileRead))
        {
            doClear();
            m_Root.Serialize(aFile);
            gnaTreeItem * sequencerItem = m_Root.child(0);
            if(sequencerItem)
                m_sequencerView->setRoot(sequencerItem);
            setDocumentName();
            setWindowModified(false);
        }
        else
        {
            //m_filePath.clear();
            QMessageBox::critical(this,"Could not open file.","Could not open file.");
        }
    }

}


void MainWindow::New()
{
    bool bDoClear = true;

    if(isWindowModified())
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch(msgBox.exec())
        {
        case QMessageBox::Save:
            Save();
            if(isWindowModified())
                bDoClear = false;
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            bDoClear = false;
            break;
        default:
            break;
        }

    }

    if( bDoClear)
    {
        m_filePath = "";
        doClear();
    }

}

void MainWindow::Open()
{
    bool bDoLoad = true;

    //Check to see if current document is modified, if so ask to save it
    if(isWindowModified())
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch(msgBox.exec())
        {
        case QMessageBox::Save:
            Save();
            if(isWindowModified())
                bDoLoad = false;
            //break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            bDoLoad = false;
            break;
        default:
            break;
        }
    }

    if(bDoLoad)
        doLoad();

}

void MainWindow::Save()
{
    if(!m_filePath.length())
        SaveAs();

    else if(isWindowModified())
            doSave();
}

void MainWindow::SaveAs()
{
    QString aString;
    aString = QFileDialog::getSaveFileName(this);
    if(aString.size() == 0)
        return;
    m_filePath = aString;
    doSave();
}

void MainWindow::Exit()
{
    if( isWindowModified())
    {
        QMessageBox msgBox;
        msgBox.setText("This document has been modified...");
        msgBox.setInformativeText("Do you want to save it before exiting the program.");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        switch(msgBox.exec())
        {
        case QMessageBox::Save:
            Save();
            qApp->quit();
            break;
        case QMessageBox::Discard:
            qApp->quit();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }

    }
    else
        qApp->quit();
}

void MainWindow::properties()
{
    SettingsDialog * pDialog = new SettingsDialog(this);
    pDialog->show();
    //sDialog.show();
}

void MainWindow::About()
{
    QString msg = "<b>";
    msg += m_applicationName;
    msg += "<b>. This is version 0.00001";
    QMessageBox::about(this, tr("About Sequencer"), msg );

}
