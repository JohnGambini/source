#include "gnatreeitem.h"

gnaTreeItem::gnaTreeItem()
{
    m_parent = 0;
    m_editFlags = 0;
    m_resID = -1;
    m_styleFlags = 0;
    m_viewIndx = -1;
    //Have at least one child list to start
    m_childItems.push_back(new vector<gnaTreeItem *>);
}

gnaTreeItem::gnaTreeItem( gnaTreeItem & right )
{
    m_parent = 0;
    m_editFlags = 0;
    m_resID = -1;
    m_styleFlags = 0;
    m_viewIndx = -1;
    //Have at least one child list to start
    m_childItems.push_back(new vector<gnaTreeItem *>);

    treeCopy(&right);
}

gnaTreeItem::gnaTreeItem( const gnaVariant &value, gnaTreeItem * parent, short editFlags, int resID, gnaint32 styleFlags, int viewIndx)
{
    m_parent = parent;
    m_itemData.push_back(value);
    m_editFlags = editFlags;
    m_resID = resID;
    m_styleFlags = styleFlags;
    m_viewIndx = viewIndx;
    //Have at least one child list to start
    m_childItems.push_back(new vector<gnaTreeItem *>);
}

gnaTreeItem::gnaTreeItem(vector<gnaVariant> &data, gnaTreeItem *parent )
{
    m_parent = parent;
    m_itemData = data;
    //Have at least one child list to start
    m_childItems.push_back(new vector<gnaTreeItem *>);
}

gnaTreeItem::~gnaTreeItem()
{
    for(vector<vector<gnaTreeItem *> * >::iterator childList = m_childItems.begin(); childList < m_childItems.end(); childList++)
    {
        for(vector<gnaTreeItem *>::iterator it = (*childList)->begin(); it < (*childList)->end(); it++ )
            delete (*it);

        delete (*childList);
    }
    m_itemData.clear();
    m_childItems.clear();}

gnaTreeItem & gnaTreeItem::operator = (gnaTreeItem & right)
{
    this->clear();

    treeCopy(&right);

    return * this;
}

void gnaTreeItem::treeCopy( gnaTreeItem * source)
{
    for(int i = 0; i < source->columnCount(); i++)
    {
        gnaVariant newVar = source->data(i);
        this->setData(i,newVar);
    }

    for(int i = 0; i < source->childCount(); i++ )
    {
        gnaTreeItem * childItem = source->child(i);
        gnaTreeItem * newItem = new gnaTreeItem(*childItem);
            this->addChild(newItem);
    }
}

void gnaTreeItem::clear()
{
    for(vector<vector<gnaTreeItem *> * >::iterator childList = m_childItems.begin(); childList < m_childItems.end(); childList++)
    {
        for(vector<gnaTreeItem *>::iterator it = (*childList)->begin(); it < (*childList)->end(); it++ )
            delete (*it);

        delete (*childList);
    }
    m_itemData.clear();
    m_childItems.clear();
    //Have at least one child list in intital condition
    m_childItems.push_back(new vector<gnaTreeItem *>);
}

gnaTreeItem * gnaTreeItem::parent()
{
    return m_parent;
}

gnaTreeItem * gnaTreeItem::child(gnaint32 number, gnaint32 listIdx )
{
    if( listIdx > m_childItems.size() - 1)
        return NULL;

    vector<gnaTreeItem *> * childList = *(m_childItems.begin() + listIdx);

    if(number < childList->size())
        return *(childList->begin() + number);

    return NULL;

}

int gnaTreeItem::childCount(gnaint32 listIdx) const
{
    if( listIdx > m_childItems.size() - 1)
        return 0;

    vector<gnaTreeItem *> * childList = *(m_childItems.begin() + listIdx);
    return (int)childList->size();
}

int gnaTreeItem::columnCount() const
{
    int ret = (int)m_itemData.size();
    return ret;
}

int gnaTreeItem::childNumber(gnaint32 listIdx)
{
    if(m_parent)
    {
        for( int l = 0; l < m_parent->childCount(listIdx); l++)
            if( m_parent->child(l,listIdx) == this)
                return l;
    }

    return 0;
}

void gnaTreeItem::setParent(gnaTreeItem * parent)
{
    m_parent = parent;
}

gnaTreeItem * gnaTreeItem::addChild(gnaTreeItem * child, gnaint32 listIdx )
{
    if( listIdx > m_childItems.size() - 1)
        return 0;

    gnaTreeItem * ret = child;
    child->setParent(this);
    m_childItems[listIdx]->push_back(child);
    return ret;
}

gnaVariant gnaTreeItem::data( gnaint32 column) const
{
    if( column < m_itemData.size())
        return m_itemData[column];

    return gnaVariant();
}

bool gnaTreeItem::insertChildren(gnaint32 position, int count, int columns, gnaint32 listIdx )
{
    if( listIdx > m_childItems.size() - 1)
        return false;

    vector<gnaTreeItem *> * childList = m_childItems[listIdx];

    if (position < 0 || position > childList->size())
        return false;

    vector<gnaVariant> data(columns);

    childList->insert(childList->begin() + position, count, new gnaTreeItem(data, this));

    return true;
}

bool gnaTreeItem::removeChildren(gnaint32 position, int count, gnaint32 listIdx )
{
    if( listIdx > m_childItems.size() - 1)
        return false;

    vector<gnaTreeItem *> * childList = m_childItems[listIdx];

    if (position < 0 || position + count > childList->size())
        return false;

    childList->erase(childList->begin()+position,childList->begin()+position+count);

    return true;

}

bool gnaTreeItem::insertColumns(gnaint32 position, int columns, gnaint32 listIdx )
{
    vector<gnaTreeItem *> * childList = m_childItems[listIdx];

    if (position < 0 || position > m_itemData.size())
        return false;

    vector<gnaVariant>::iterator itd = m_itemData.begin();

    m_itemData.insert(itd + position, columns, gnaVariant());

    for(vector<gnaTreeItem *>::iterator it = childList->begin(); it < childList->end(); it++ )
        (*it)->insertColumns(position, columns, listIdx );

    return true;
}

bool gnaTreeItem::setData( int column, const gnaVariant &value, short editFlags, int resID, gnaint32 styleFlags, int viewIdnx)
{
    int currcount = (int)m_itemData.size();

    if (column < 0 )
        return false;

    if(column >= currcount )
        insertColumns( currcount, column - currcount + 1);

    m_itemData[column] = value;

    m_editFlags = editFlags;
    m_resID =resID;
    m_styleFlags = styleFlags;
    m_viewIndx = viewIdnx;

    return true;
}

void gnaTreeItem::Serialize( gnaFile & fl )
{
    size_t collectionSize = 0;

    if( !fl.is_open() )
        return;

    if( fl.isStoring())
    {
        fl.write((GNACHAR *)&m_editFlags,sizeof(short));
        fl.write((GNACHAR *)&m_resID,sizeof(int));
        fl.write((GNACHAR *)&m_styleFlags,sizeof(gnaint32));
        fl.write((GNACHAR *)&m_viewIndx,sizeof(int));
        collectionSize = m_itemData.size();
        fl.write((GNACHAR *)&collectionSize,sizeof(gnaint32));
        vector<gnaVariant>::iterator idit;
        for( idit = m_itemData.begin(); idit < m_itemData.end(); idit++ )
            (*idit).Serialize(fl);

        collectionSize = m_childItems.size();
        fl.write((GNACHAR *)&collectionSize,sizeof(gnaint32));
        vector<vector<gnaTreeItem *>*>::iterator ciit;
        for( ciit = m_childItems.begin(); ciit < m_childItems.end(); ciit++ )
        {
            collectionSize = (*ciit)->size();
            fl.write((GNACHAR *)&collectionSize,sizeof(gnaint32));
            vector<gnaTreeItem *>::iterator it;
            for( it = (*ciit)->begin(); it < (*ciit)->end(); it++)
            {
                (*it)->Serialize(fl);
            }
        }
    }
    else
    {
        if( fl.m_fileMinorVersion > 0 )
        {
            fl.read((GNACHAR *)&m_editFlags,sizeof(short));
            fl.read((GNACHAR *)&m_resID,sizeof(int));
            fl.read((GNACHAR *)&m_styleFlags,sizeof(gnaint32));
            fl.read((GNACHAR *)&m_viewIndx,sizeof(int));
        }

        fl.read((GNACHAR *)&collectionSize,sizeof(gnaint32));
        for( gnaint32 l = 0; l < collectionSize; l++ )
        {
            gnaVariant gnaVar;
            gnaVar.Serialize(fl);
            setData(l, gnaVar);
        }

        fl.read((GNACHAR *)&collectionSize,sizeof(gnaint32));
        for( gnaint32 l = 0; l < collectionSize; l++ )
        {
            gnaint32 collectionSize2;
            m_childItems.push_back(new vector<gnaTreeItem *>);
            fl.read((GNACHAR *)&collectionSize2,sizeof(gnaint32));
            for( gnaint32 m = 0; m < collectionSize2; m++ )
            {
                gnaTreeItem *  pItem = addChild(new gnaTreeItem(), l );
                pItem->Serialize(fl);
            }
        }

    }
}

