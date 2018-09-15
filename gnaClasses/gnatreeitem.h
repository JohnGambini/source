#ifndef GNATREEITEM_H
#define GNATREEITEM_H

#include "gnavariant.h"

#include <vector>

class GNALIBSHARED_EXPORT gnaTreeItem
{
public:
    explicit gnaTreeItem();
    gnaTreeItem( gnaTreeItem & right );
    explicit gnaTreeItem( const gnaVariant &value, gnaTreeItem * parent = 0, short editFlags = 0, int resID = -1, gnaint32 styleFlags = 0, int viewIndx = -1);
    explicit gnaTreeItem(vector<gnaVariant> &data, gnaTreeItem *parent );
    virtual ~gnaTreeItem();
    gnaTreeItem & operator = (gnaTreeItem & right);
    virtual void clear();
    gnaTreeItem *parent();
    gnaTreeItem *child(gnaint32 number, gnaint32 listIdx = 0 );
    int childCount(gnaint32 listIdx = 0) const;
    int columnCount() const;
    int childNumber(gnaint32 listIdx = 0);
    void setParent(gnaTreeItem * parent);
    gnaTreeItem * addChild(gnaTreeItem * child, gnaint32 listIdx = 0 );
    gnaVariant data( gnaint32 column) const;
    bool insertChildren(gnaint32 position, int count, int columns, gnaint32 listIdx = 0);
    bool removeChildren(gnaint32 position, int count, gnaint32 listIdx = 0 );
    bool insertColumns(gnaint32 position, int columns, gnaint32 listIdx = 0 );
    bool setData( int column, const gnaVariant &value, short editFlags = 0, int resID = -1, gnaint32 styleFlags = 0, int viewIdnx = -1);
	gnaint32 styleFlags() {return m_styleFlags;}
    virtual void Serialize( gnaFile & fl );

protected:
    void treeCopy( gnaTreeItem * source);

private:
    short m_editFlags;
    int m_resID;
    gnaint32 m_styleFlags;
    int m_viewIndx;
    vector<vector<gnaTreeItem *> * > m_childItems;
    vector<gnaVariant> m_itemData;
    gnaTreeItem * m_parent;

};

#if defined(_WINDOWS)
template class GNALIBSHARED_EXPORT std::vector<gnaTreeItem *>;
template class GNALIBSHARED_EXPORT std::vector<gnaVariant>;
#endif

#endif // GNATREEITEM_H
