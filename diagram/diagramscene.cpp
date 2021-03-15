
#include "diagramscene.h"
#include "arrow.h"

#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>

//! [0]
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = 0;
    textItem = 0;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;

    m_curitem =NULL;
    //////////////////////////////////////////////////////////////////////////
    m_inputtextitem = new DiagramTextItem();

    //m_inputtextitem->setFont(m_myFont);
    //m_inputtextitem->setTextInteractionFlags(Qt::TextEditorInteraction);
    //m_inputtextitem->setZValue(1000.0);
     connect(m_inputtextitem, SIGNAL(lostFocus(DiagramTextItem*)),
        this, SLOT(editorLostFocus(DiagramTextItem*)));
    connect(m_inputtextitem, SIGNAL(selectedChange(QGraphicsItem*)),
        this, SIGNAL(itemSelected(QGraphicsItem*)));
    addItem(m_inputtextitem);
    //m_inputtextitem->setDefaultTextColor(m_myTextColor);
    //m_inputtextitem->setPos(mouseEvent->scenePos());
    emit textInserted(m_inputtextitem);

}
//! [0]

//! [1]
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}
//! [1]

//! [2]
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}
//! [2]

//! [3]
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}
//! [3]

//! [4]
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myFont);
    }
}
//! [4]

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//! [5]
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}
//! [5]


//! [6]
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    DiagramItem *item;
    switch (myMode) {
        case InsertItem:
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());
            emit itemInserted(item);

            m_curitem = item;
            //////////////////////////////////////////////////////////////////////////
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);


            break;
//! [6] //! [7]
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;
//! [7] //! [8]
        case InsertText:
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(1000.0);
            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)),
                    this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),
                    this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
//! [8] //! [9]
    default:
        if(m_curitem!=0)
        {
            m_curitem->SetDiagramModelName(textItem->toPlainText());
            //m_inputtextitem->setPos(QPointF(-100,-100) );
            m_curitem->update();
            removeItem(textItem);
            delete textItem;
        }
        m_curitem =0
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
//! [9]

void DiagramScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        //QGraphicsItem *item = selectedItems();
        foreach (QGraphicsItem *item, selectedItems())
        {
            if ( item->data(1).type()== QVariant::String)
            {
                QString astr = item->data(1).toString();// pditem->GetDiagramModelName();

                emit  SelectState(astr);
                return;
                //if(astr ==strmodelname)
                //	pditem->setBrush(Qt::red);
            }
        }

        foreach (QGraphicsItem *item, selectedItems())
        {
            if ( item->data(2).type()== QVariant::String)
            {
                QString astr = item->data(2).toString();// pditem->GetDiagramModelName();

                emit  SelectConnect(astr);
                 return;
                //if(astr ==strmodelname)
                //	pditem->setBrush(Qt::red);
            }
        }
    }
}

//! [10]
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (myMode == MoveItem) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}
//! [10]

//! [11]
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine)
    {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first())
        {
            DiagramItem *startItem =
                qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem =
                qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            //////////////////////////////////////////////////////////////////////////
            //DiagramItem * pditemA = NULL;
            //DiagramItem * pditemB = NULL;
            //foreach (QGraphicsItem *item, items())
            //{
            //	if ( item->data(1).type()== QVariant::String)
            //	{
            //		QString astr = item->data(1).toString();
            //		DiagramItem*pditem = (DiagramItem*) item ;
            //		if(astr ==strmodelA)
            //			pditemA = pditem;
            //		if(astr ==strmodelB)
            //			pditemB = pditem;
            //	}
            //}
            QString strmodelB = startItem->data(1).toString();
            QString strmodelA = endItem->data(1).toString();

            QString astr_bstr=strmodelB+QString("_")+strmodelA;

            arrow->SetDiagramModelConnectName(astr_bstr);
//			emit newState(astr_bstr);
            //////////////////////////////////////////////////////////////////////////
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }
//! [12] //! [13]
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//! [13]

//! [14]
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
//! [14]

void DiagramScene::AddModel(int ix,int iy,DiagramItem::DiagramType type ,QString strmodel)
{
    DiagramItem *item;
    item = new DiagramItem(type, myItemMenu);
    item->SetDiagramModelName(strmodel);
    item->setBrush(myItemColor);
    addItem(item);
    QPoint posbegin(ix+2200,iy+2280);
    item->setPos(posbegin);
    emit itemInserted(item);
}

void DiagramScene::AddModelConnect(const QString &strmodelA,const QString &strmodelB)
{
    DiagramItem * pditemA = NULL;
    DiagramItem * pditemB = NULL;

    foreach (QGraphicsItem *item, items())
    {
        //;
        if ( item->data(1).type()== QVariant::String)
        {
            QString astr = item->data(1).toString();// pditem->GetDiagramModelName();

            DiagramItem*pditem = (DiagramItem*) item ;
            if(astr ==strmodelA)
                pditemA = pditem;

            if(astr ==strmodelB)
                pditemB = pditem;
        }
    }
    if(NULL ==pditemA||NULL ==pditemB)
        return;
    QString astr_bstr=strmodelB+QString("_")+strmodelA;
    Arrow *arrow = new Arrow(pditemB, pditemA);
    arrow->setColor(myLineColor);
    arrow->SetDiagramModelConnectName(astr_bstr);
    pditemA->addArrow(arrow);
    pditemB->addArrow(arrow);
    arrow->setZValue(-1000.0);
    addItem(arrow);
    arrow->updatePosition();

}
void DiagramScene::HighLighModleColor(const QString & strmodelname)
{
/*
m_myItemColor = color;
if (isItemChange(DiagramItem::Type)) {
DiagramItem *item =
qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
item->setBrush(m_myItemColor);
}
*/
    foreach (QGraphicsItem *item, items())
    {
        //;
        if ( item->data(1).type()== QVariant::String)
        {
            QString astr = item->data(1).toString();// pditem->GetDiagramModelName();
             DiagramItem*pditem = (DiagramItem*) item ;
            if(astr ==strmodelname)
                pditem->setBrush(Qt::red);
        }
    }
}
void DiagramScene::HighLighModleConnectColor(const QString & strconnectname)
{
    foreach (QGraphicsItem *item, items())
    {
        if ( item->data(2).type()== QVariant::String)
        {
            QString astr = item->data(2).toString();// pditem->GetDiagramModelName();
            Arrow*pditem = (Arrow*) item ;
            if(astr ==strconnectname)
                pditem->setColor(Qt::red);
        }
    }
}
void DiagramScene::NormalAllModleColor( )
{
    foreach (QGraphicsItem *item, items())
    {
        //;
       // if ( item->data(1).type()== QVariant::String)
        {
           QString astr = item->data(1).toString();// pditem->GetDiagramModelName();
             DiagramItem*pditem = (DiagramItem*) item ;
            if(!
                    astr.isEmpty())
             pditem->setBrush(Qt::white);
        }
    }
}

void DiagramScene::NormalModleColor(const QString & strmodelname)
{
/*
or;
if (isItemChange(DiagramItem::Type)) {
DiagramItem *item =
qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
item->setBrush(m_myItemColor);
}
*/
    foreach (QGraphicsItem *item, items())
    {
        //;
        if ( item->data(1).type()== QVariant::String)
        {
            QString astr = item->data(1).toString();// pditem->GetDiagramModelName();
             DiagramItem*pditem = (DiagramItem*) item ;
            if(astr ==strmodelname)
                pditem->setBrush(Qt::white);
        }
    }
}
void DiagramScene::NormalModleConnectColor(const QString & strconnectname)
{
    foreach (QGraphicsItem *item, items())
    {
        //;
        if ( item->data(2).type()== QVariant::String)
        {
            QString astr = item->data(2).toString();// pditem->GetDiagramModelName();
            Arrow*pditem = (Arrow*) item ;
            if(astr ==strconnectname)
                pditem->setColor(Qt::black);
        }
    }
}
