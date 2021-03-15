
#ifndef DIAGRAMTAB_H
#define DIAGRAMTAB_H

#include "diagramitem.h"

#include <QToolBar>
#include <QDialog>
#include <QBasicTimer>


class DiagramScene;

QT_BEGIN_NAMESPACE
class QTabWidget;
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE


class DiagramTab : public QWidget
{
    Q_OBJECT

public:
    explicit DiagramTab(QWidget *parent = 0);

private slots:
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(int id);
    void deleteItem();
    void pointerGroupClicked(int id);
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void sceneScaleChanged(const QString &scale);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem *item);
    void about();

private:
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    void timerEvent(QTimerEvent *event);
    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              DiagramItem::DiagramType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);

    DiagramScene *scene;
    QGraphicsView *view;

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;

    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QButtonGroup *cxbackgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;

    //new
    QString m_strmodelname;
    QString m_strconnectmodelname;
    QString m_strconnecttomodelname;

    QString m_colormodelstring;
    QString m_lastmodelcolorstring;

    QString m_colormodelconnectstring;
    QString m_lastmodelconnectcolorstring;



    DiagramItem::DiagramType m_myItemType;
    int m_ivgapsize;
    int m_ihgapsize;
    int m_ivnum;
    int m_ihnum;

    QBasicTimer m_cleartimer;
signals:

    void SelectState(const QString&statename);
    void SelectConnect(const QString&connectname);
    void NewModel(const QString &statename);
    void NewConnect(const QString &connectname);
public slots:
    void setmodelstring(const QString &strmodelname);
    void SetmodleType(int itype);
    void AddModel();
    void setconnectmodel(const QString &amodel);
    void setconnecttomodel(const QString &bmodel);
    void AddModelConnect();
    void AddModelList(const QStringList&modellist);
    void AddModelConnectList(const QStringList&modellist);

    void AddModelConnectA(const QString &bmodela);
    void AddModelConnectB(const QString &bmodelb);

    void HighLightModelColor();
    void HighLightConnectlColor();

    void NormalModelColor();
    void NormalConnectlColor();

    void HighLightModelColor(const QString &strmodel);
    void HighLightConnectlColor(const QString &strconnect);

    void NormalModelColor(const QString &strmodel);
    void NormalConnectlColor(const QString &strconnect);


    void HighlighterDebugModel();

    void SetModelColor(const QString&modelname);
    void SetModelColorA(const char *pmodelname);
    void SetModelConnectColor(const QString&modelname);
    void setBackground();//const QString &picture);

    void deleteAllItem();


    void DiagramStateItemSelect(const QString&statename);
    void DiagramConnectItemSelect(const QString&connectname);


};


#endif
