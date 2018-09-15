#pragma once

#include "gnaGlobal.h"
#include "gnaTreeItem.h"

#include <list>

template < typename _indxtyp >
class gnaUndoItem;

template < typename _indxtyp >
class gnaUndoHandler
{
public:
	gnaUndoHandler(void){ m_undoItemID = 100;}
	gnaint32 getNextItemID() { return m_undoItemID++; }
	int UndoItemCount() { return (int)m_undoItems.size(); }
	int RedoItemCount() { return (int)m_redoItems.size(); }
	bool Add(gnaUndoItem< _indxtyp > * undoItem) {
		if( undoItem->Do() )
		{
			m_undoItems.push_back(undoItem);
			undoItem->setHandler(this);
			undoItem->setMyID(m_undoItemID);
			setSiblingIndexRecord(m_undoItemID++, undoItem->m_indxSibling);
			while(m_redoItems.size())
			{
				list<gnaUndoItem< _indxtyp > *>::iterator it = m_redoItems.begin();
				gnaUndoItem< _indxtyp > * pItem = (*it);
				delete pItem;
				m_redoItems.pop_front();
			}
		}
		return true;
	}

	bool Undo()
	{
		list<gnaUndoItem< _indxtyp > *>::iterator it = m_undoItems.end();
		it--;

		(*it)->Undo();

		gnaUndoItem< _indxtyp > * pUndoItem = (*it);

		m_undoItems.pop_back();

		m_redoItems.push_back(pUndoItem);

		return true;
	}

	bool Redo()
	{
		list<gnaUndoItem< _indxtyp > *>::iterator it = m_redoItems.end();
		it--;

		(*it)->Do();

		gnaUndoItem< _indxtyp > * pUndoItem = (*it);

		m_redoItems.pop_back();

		m_undoItems.push_back(pUndoItem);

		return true;
	}

private:
	//The value of sibling indexes will change as items are added and deleted from the control. So...
	//When performing an Add, add a _siblingIndex to the list consisting of itemID and sibling index.
	//When performing a delete, check the list for a matching indexValue and set the siblingID or add a record
	//to the list consisting of just a siblingID.
	//When performing an insert check the list for a matching sibling ID and set the indexValue to the new value and
	//	check the list for a matching itemID and use the indexValue for the sibling index.
	//Undo's and redo's: An Undo for an insert object is a delete
	//and an undo for a delete object is an insert. alguh..alguh..alguh..phhhhsssttt

	gnaint32 m_undoItemID;		//Just count up and assign the next number to a new undo item.
								//These only have to be unique within the context of an instance of an undo handler
	class _siblingIndex
	{
	public:
		_siblingIndex(gnaint32 itemID, gnaint32 siblingID, _indxtyp indexValue )
			{ m_itemID = itemID; m_siblingID = siblingID; m_indexValue = indexValue; }
		gnaint32 m_itemID;
		gnaint32 m_siblingID;
		_indxtyp m_indexValue;
	};
	list<_siblingIndex> m_Indexes;
	void setSiblingIndexRecord(gnaint32 itemID, _indxtyp indexValue )
	{
		_siblingIndex indexRecord( itemID, 0, indexValue );
		m_Indexes.push_back(indexRecord);
	}
	void setSiblingID(gnaint32 siblingID, _indxtyp indexValue )
	{
		for( list<_siblingIndex>::iterator it = m_Indexes.begin(); it < m_Indexes.end(); it++ )
		{
			if( (*it).m_indexValue == indexValue )
			{
				(*it).m_siblingID = siblingID;
				break;
			}
		}
	}

	void setSiblingIndex( _indxtyp indexValue );

private:
	list<gnaUndoItem< _indxtyp > *> m_undoItems;
	list<gnaUndoItem< _indxtyp > *> m_redoItems;
};

template < typename _indxtyp >
class gnaUndoItem
{
public:
	gnaUndoItem( _indxtyp indxSibling, gnaint32 pos ) { m_indxSibling = indxSibling; m_pos = pos; }
    virtual bool Do() = 0;
    virtual void Undo() = 0;
	void setHandler(gnaUndoHandler< _indxtyp > * pHandler) { m_handler = pHandler; }
	void setMyID( gnaint32 itemID ) { m_MyID = itemID; }
	gnaint32 getMyID() { return m_MyID; }

protected:
	gnaint32 m_MyID;
	gnaint32 m_pos;
	_indxtyp m_indxSibling;
	gnaUndoHandler< _indxtyp > * m_handler;
	gnaTreeItem m_theItem;
	friend class gnaUndoHandler< _indxtyp >;
};
