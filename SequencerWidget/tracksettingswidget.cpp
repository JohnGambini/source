#include <QHeaderView>

#include "tracksettingswidget.h"
#include "patchchnlwidget.h"

#include "pm_common/portmidi.h"

TrackSettingsWidget::TrackSettingsWidget(QWidget *parent) :
    QTableWidget(parent)
{
    m_root = NULL;
    setColumnCount(1);

    QHeaderView * hhv = horizontalHeader();
    hhv->setHidden(true);

    setColumnWidth(0, columnWidth(0) * 3.5 );

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //connect(this,SIGNAL(itemSelectionChanged()), this, SLOT(trackSelectionChanged()));

    m_midiPortsToUse.push_back(0); //These should be set by a dialog box
    m_midiPortsToUse.push_back(1); //That lets the user decide which ports to use
    m_midiPortsToUse.push_back(2);

    Pm_Initialize();

    /* list device information */
    for (int i = 0; i < Pm_CountDevices(); i++)
    {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        if(info->output == true)
            m_midiDeviceNames << info->name;
    }

    Pm_Terminate();

    setStyleSheet("QTableWidget { color:white;background-color:rgb(42,42,64) }");
}

void TrackSettingsWidget::trackSelectionChanged()
{
    QModelIndexList selectedTrackIndexes = selectedIndexes();
    emit selectionChange(selectedTrackIndexes);
}

void TrackSettingsWidget::setSelectedIndexes(QModelIndexList &)
{
    /*
    blockSignals(true);
    setCurrentIndex(QModelIndex());
    blockSignals(false);
    */
}

void TrackSettingsWidget::setRoot(gnaTreeItem * root)
{
    m_root = root;
    PatchChnlWidget * patchChnlWidget;
    gnaTreeItem * childItem;

    if( m_root == NULL )
    {
        for(int i = 0; i < rowCount(); i++ )
        {
            patchChnlWidget = qobject_cast<PatchChnlWidget *>(cellWidget(i,0));
            patchChnlWidget->setRoot(NULL);
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


        for( int i = 0; i < rowCount() ; i++ )
        {
            childItem = m_root->child(i);
            gnaVariant chnl = childItem->data(1);
            gnaVariant volume = childItem->data(5);
            gnaVariant pan = childItem->data(6);
            if( chnl.toInt() == 0)
            {
                chnl = i+1;
                childItem->setData(1,chnl);
            }
            if(volume.toInt() == 0)
            {
                volume = 128;
                childItem->setData(5,volume);
            }
            if(pan.toInt() == 0)
            {
                pan = 64;
                childItem->setData(6,pan);
            }

            patchChnlWidget = qobject_cast<PatchChnlWidget *>(cellWidget(i,0));
            if( patchChnlWidget )
                patchChnlWidget->setRoot(childItem);

        }
    }

}

int TrackSettingsWidget::getPortID(int index )
{
    if(index >= m_midiPortsToUse.size())
        return -1;

    return m_midiPortsToUse[index];
}

int TrackSettingsWidget::addTrack(int row)
{
    vector<gnaVariant> itemData;
    gnaTreeItem * trackItem = NULL;
    gnaVariant ptch;
    gnaVariant chnnl;
    gnaVariant port;
    gnaVariant rate;
    gnaVariant mute;
    gnaVariant vlcty;

    ptch = 80;
    chnnl = 1;
    port = false;
    rate = 2;
    mute = false;
    vlcty = 128;

    itemData.push_back(ptch);
    itemData.push_back(chnnl);
    itemData.push_back(port);
    itemData.push_back(rate);
    itemData.push_back(mute);
    itemData.push_back(vlcty);

    insertRow(row);

    if(m_root)
        trackItem = m_root->addChild(new gnaTreeItem(itemData,m_root));

    PatchChnlWidget * pcWidget = new PatchChnlWidget;
    pcWidget->setPortList( m_midiDeviceNames, m_midiPortsToUse);
    setCellWidget(row,0,pcWidget );
    pcWidget->setRoot(trackItem);
    pcWidget->setChannel(row+1);
    //connect( pcWidget, SIGNAL(channelChange(int,int)), this, SLOT(channelChange(int,int)));
    connect(pcWidget,SIGNAL(noteLabelsChanged(bool,int)), this, SIGNAL(noteLabelsChanged(bool,int)));
    connect(pcWidget,SIGNAL(rateChange(int,int)),SIGNAL(rateChange(int,int)));
    connect( pcWidget, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));

    resizeRowToContents(row);
    int height = int(rowHeight(row)*.95);
    setRowHeight(row, height);
    return height;
}

