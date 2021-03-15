
#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include "diagramitem.h"
#include "diagramtextitem.h"

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
class QEditor;
QT_END_NAMESPACE

//! [0]
class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    explicit DiagramScene(QMenu *itemMenu, QObject *parent = 0);
    QFont font() const { return myFont; }
    QColor textColor() const { return myTextColor; }
    QColor itemColor() const { return myItemColor; }
    QColor lineColor() const { return myLineColor; }
    void setLineColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

    void AddModel(int ix,int iy,DiagramItem::DiagramType type ,QString strmodel);
    void AddModelConnect(const QString & strmodelA,const QString & strmodelB);


    void HighLighModleColor(const QString & strmodelname);
    void HighLighModleConnectColor(const QString & strconnectname);
    void NormalModleColor(const QString & strmodelname);
    void NormalModleConnectColor(const QString & strconnectname);
    void NormalAllModleColor();
public slots:
    void setMode(Mode mode);
    void setItemType(DiagramItem::DiagramType type);
    void editorLostFocus(DiagramTextItem *item);
signals:
    void SelectState( const QString& statename);
    void SelectConnect( const QString& connectname);
    //void newConnect(const QString&connectname);
    //void newState(const QString&connectname);

signals:
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);

    DiagramItem::DiagramType myItemType;
    QMenu *myItemMenu;
    Mode myMode;
    bool leftButtonDown;

    QPointF startPoint;
    QGraphicsLineItem *line;
    QFont myFont;
    DiagramTextItem *textItem;

    DiagramTextItem * m_inputtextitem;
    DiagramItem*  m_curitem;

    QColor myTextColor;
    QColor myItemColor;
    QColor myLineColor;


    QEditor *m_qeditor;


};
//! [0]

#endif // DIAGRAMSCENE_H
