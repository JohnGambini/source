#include "sequencerwidget.h"

#include <QThread>
#include <QHeaderView>
#include <QBoxLayout>
#include <QLabel>

#include "notewidget.h"
#include "sequencerheader.h"

SequencerWidget::SequencerWidget(QWidget *parent) :
    QTableWidget(parent)
{
    m_isPlaying = false;
    m_root = NULL;

    setColumnCount(16);

    QHeaderView * hhv = horizontalHeader();
    hhv->setHidden(true);
    QHeaderView * hvv = verticalHeader();
    hvv->setHidden(true);

    connect(this,SIGNAL(itemSelectionChanged()), this, SLOT(sequencerSelectionChanged()));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setSelectionMode(QAbstractItemView::MultiSelection);

    //QPalette* palette = new QPalette();
    //palette->setColor(QPalette::Background,Qt::black);
    //setPalette(*palette);

    setStyleSheet("QTableWidget { color:white;background-color:rgb(42,42,64) }");
}

void SequencerWidget::clear()
{
    while(rowCount())
    {
        removeRow(rowCount()-1);
    }
}

void SequencerWidget::noteOn(int x, int y, int chnl, int port)
{
    NoteWidget * noteWidget = qobject_cast<NoteWidget *>(cellWidget(y,x));
    if( noteWidget)
        noteWidget->noteOn(true);
}

void SequencerWidget::noteOff(int x, int y, int chnl, int port)
{
    NoteWidget * noteWidget = qobject_cast<NoteWidget *>(cellWidget(y,x));
    if( noteWidget)
        noteWidget->noteOff(true);
}

void SequencerWidget::channelChange(int track,int chnl)
{
    NoteWidget * noteWidget = 0;

    for(int i = 0; i < 16; i++)
    {
        noteWidget = qobject_cast<NoteWidget *>(cellWidget(track,i));
        if(noteWidget != NULL)
        {
            if(chnl == 10)
                noteWidget->setDrums();
            else
                noteWidget->setNotes();
        }
    }
}

void SequencerWidget::setRoot(gnaTreeItem * root )
{
    m_root = root;
    NoteWidget * noteWidget;
    gnaTreeItem * childItem;

    blockSignals(true);
    if( m_root == NULL )
    {
        for(int i = 0; i < rowCount(); i++ )
        {
            for(int j = 0; j < columnCount(); j++ )
            {
                noteWidget = qobject_cast<NoteWidget *>(cellWidget(i,j));
                noteWidget->setRoot(NULL);
            }
        }
    }
    else
    {
        //Patch because going from three tracks to four tracks
        while( m_root->childCount() < rowCount())
        {
            gnaTreeItem * newTrack = m_root->addChild(new gnaTreeItem());
            for(int i = 0; i < 16; i++)
                newTrack->addChild(new gnaTreeItem());
        }

        for( int i = 0; i < rowCount(); i++ )
        {
            childItem = m_root->child(i);

            for(int j = 0; j < childItem->childCount(); j++)
            {
                noteWidget = qobject_cast<NoteWidget *>(cellWidget(i,j));
                if( noteWidget )
                    noteWidget->setRoot(childItem->child(j));

            }

        }
    }
    blockSignals(false);
}

void SequencerWidget::sequencerSelectionChanged()
{
    QModelIndexList selectedSequencerIndexes = selectedIndexes();
    emit selectionChange(selectedSequencerIndexes);
}

void SequencerWidget::setSelectedIndexes(QModelIndexList & indexes)
{
    /*
    for(int i = 0; i < 5; i++ )
    {
        blockSignals(true);
        for(int j = 0; j < 16; j++)
        {
            QWidget * pWidget = cellWidget(i,j);
            if(pWidget->isSisSelected())
            setCurrentCell(index.row(),j);
        }
        blockSignals(false);
    }
    //select
    blockSignals(true);
    for(int i = 0; i < indexes.size(); i++ )
    {
        QModelIndex & index = indexes[i];
        for(int j = 0; j < 16; j++)
        {
            setCurrentCell(index.row(),j);
        }
    }
    blockSignals(false);
    */
}

void SequencerWidget::resizeEvent(QResizeEvent * event)
{
    QRect rect = geometry();
    int colWidth = int(rect.width() / 16.25);
    if(colWidth < 48)
        colWidth = 48;

    setColumnWidth(0, colWidth );
    setColumnWidth(1, colWidth );
    setColumnWidth(2, colWidth );
    setColumnWidth(3, colWidth );
    setColumnWidth(4, colWidth );
    setColumnWidth(5, colWidth );
    setColumnWidth(6, colWidth );
    setColumnWidth(7, colWidth );
    setColumnWidth(8, colWidth );
    setColumnWidth(9, colWidth );
    setColumnWidth(10, colWidth );
    setColumnWidth(11, colWidth );
    setColumnWidth(12, colWidth );
    setColumnWidth(13, colWidth );
    setColumnWidth(14, colWidth );
    setColumnWidth(15, colWidth );
}

void SequencerWidget::setNoteLabels(bool value, int track)
{
    NoteWidget * noteWidget = NULL;

    for( int i = 0; i < 16; i++)
    {
        if((noteWidget = qobject_cast<NoteWidget *>(cellWidget(track,i))) != NULL)
        {
            if( value )
                noteWidget->setDrums();
            else
                noteWidget->setNotes();
        }
    }
}

void SequencerWidget::rateChange(int track, int rate)
{
    if(m_root)
    {
        gnaTreeItem * chnlPatch = m_root->child(track);
        gnaVariant data = rate;
        chnlPatch->setData(3,data);
    }
}

void SequencerWidget::resizeSection(int logicalIndex, int oldSize, int newSize)
{
    QHeaderView * vh = verticalHeader();
    vh->resizeSection(logicalIndex,newSize);
}


void SequencerWidget::addTrack(int row, int height)
{
    vector<gnaVariant> itemData;
    gnaTreeItem * trackItem = NULL;
    gnaVariant ptch;
    gnaVariant chnnl;
    gnaVariant port;
    gnaVariant note;
    gnaVariant rate;
    gnaVariant mute;

    note = 39; //C4 = 60 - 21

    itemData.push_back(ptch);
    itemData.push_back(chnnl);
    itemData.push_back(port);
    itemData.push_back(rate);
    itemData.push_back(mute);

    insertRow(row);

    if(m_root)
        trackItem = m_root->child(row);

    NoteWidget * noteWidget = new NoteWidget;
    noteWidget->setLabel("1");
    setCellWidget(row,0, noteWidget);
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,1, noteWidget);
    noteWidget->setLabel("2");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,2, noteWidget);
    noteWidget->setLabel("3");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,3, noteWidget);
    noteWidget->setLabel("4");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,4, noteWidget);
    noteWidget->setLabel("5");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,5, noteWidget);
    noteWidget->setLabel("6");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,6, noteWidget);
    noteWidget->setLabel("7");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,7, noteWidget);
    noteWidget->setLabel("8");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,8, noteWidget);
    noteWidget->setLabel("9");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,9, noteWidget);
    noteWidget->setLabel("10");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,10, noteWidget);
    noteWidget->setLabel("11");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,11, noteWidget);
    noteWidget->setLabel("12");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,12, noteWidget);
    noteWidget->setLabel("13");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,13, noteWidget);
    noteWidget->setLabel("14");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,14, noteWidget);
    noteWidget->setLabel("15");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    noteWidget = new NoteWidget;
    setCellWidget(row,15, noteWidget);
    noteWidget->setLabel("16");
    noteWidget->noteOff(true);
    if(trackItem)
        noteWidget->setRoot( trackItem->addChild(new gnaTreeItem(note,trackItem)));
    connect(noteWidget,SIGNAL(dataChanged()),this,SIGNAL(dataChanged()));

    resizeRowToContents(row);
    setRowHeight(row, height );

}

