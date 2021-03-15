
#include "arrow.h"
#include "diagramitem.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include "diagramtab.h"

#include <QtWidgets>

const int InsertTextButton = 10;

//! [0]
DiagramTab::DiagramTab(QWidget *parent)
{
    createActions();
    createToolBox();
    createMenus();

    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)),
            this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)),
            this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)),
            this, SLOT(itemSelected(QGraphicsItem*)));

    connect(scene, SIGNAL(SelectState(const QString&)),
        this, SLOT(DiagramStateItemSelect(const QString&)));
    connect(scene, SIGNAL(SelectConnect(const QString&)),
        this, SLOT(DiagramConnectItemSelect(const QString&)));


    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;

    view = new QGraphicsView(scene);
    layout->addWidget(view);
   layout->addWidget(toolBox);

    view->setMinimumHeight(380);
    view->setMinimumWidth(480);


     setLayout(layout);

    m_cleartimer.start(500,this);

    setBackground();
     //QWidget *widget = new QWidget;
   // widget->setLayout(layout);

    //setCentralWidget(widget);
   // setWindowTitle(tr("Diagramscene"));
  //  setUnifiedTitleAndToolBarOnMac(true);
}
//! [0]
void DiagramTab::setBackground()//const QString &picture)
{//":/res/qt4logo.png"
    //scene->setBackgroundBrush(QPixmap("F:\\QT\\4.8.7\\demos\\qtdemo\\images\\demobg.png"));//:/res/qt4logo.png"));
    scene->setBackgroundBrush(QPixmap(":/images/background3.png"));

    //m_scene->setBackgroundBrush(QPixmap(picture));
    scene->update();
    update();
}
//! [1]
void DiagramTab::backgroundButtonGroupClicked(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) {
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    else
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));

    scene->update();
    view->update();
}
//! [1]

//! [2]
void DiagramTab::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem);
    }
}
//! [2]

//! [3]
void DiagramTab::deleteItem()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->selectedItems()) {
         if (item->type() == DiagramItem::Type)
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
         scene->removeItem(item);
         delete item;
     }
}
//! [3]

//! [4]
void DiagramTab::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [4]

//! [5]
void DiagramTab::bringToFront()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [5]

//! [6]
void DiagramTab::sendToBack()
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}
//! [6]

//! [7]
void DiagramTab::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}
//! [7]

//! [8]
void DiagramTab::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}
//! [8]

//! [9]
void DiagramTab::currentFontChanged(const QFont &)
{
    handleFontChange();
}
//! [9]

//! [10]
void DiagramTab::fontSizeChanged(const QString &)
{
    handleFontChange();
}
//! [10]

//! [11]
void DiagramTab::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}
//! [11]

//! [12]
void DiagramTab::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/textpointer.png",
                                     qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}
//! [12]

//! [13]
void DiagramTab::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png",
                                     qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}
//! [13]

//! [14]
void DiagramTab::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png",
                                     qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}
//! [14]

//! [15]
void DiagramTab::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}
//! [15]

//! [16]
void DiagramTab::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}
//! [16]

//! [17]
void DiagramTab::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}
//! [17]

//! [18]
void DiagramTab::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}
//! [18]

//! [19]
void DiagramTab::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}
//! [19]

//! [20]
void DiagramTab::about()
{
    QMessageBox::about(this, tr("About Diagram Scene"),
                       tr("The <b>Diagram Scene</b> example shows "
                          "use of the graphics framework."));
}
//! [20]

//! [21]
void DiagramTab::createToolBox()
{

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    QGridLayout *textLayout = new QGridLayout;
    QGridLayout *backgroundLayout = new QGridLayout;
    QGridLayout *toolbarLayout = new QGridLayout;

    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step),0, 0);
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);
    layout->addWidget(createCellWidget(tr("String"), DiagramItem::String), 2, 0);
    layout->addWidget(createCellWidget(tr("Node"), DiagramItem::Node), 2, 1);
    layout->addWidget(createCellWidget(tr("Relation"), DiagramItem::Relation), 2, 2);


//! [21]

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(25, 25));

    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);

    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);

    layout->addWidget(textWidget, 1, 1);
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));


    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                           ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                           ":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                           ":/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                           ":/images/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);
    //cxadd

        QWidget *cxitemWidget = new QWidget;
        cxitemWidget->setLayout(toolbarLayout);

        cxbackgroundButtonGroup = new QButtonGroup(this);
        connect(cxbackgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
                this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

        QGridLayout *cxbackgroundLayout = new QGridLayout;

        //cx begin
       // cxbackgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
        //                                                       ":/images/background1.png"), 0, 0);
      //  cxbackgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
        //                                                       ":/images/background2.png"), 0, 1);
       // cxbackgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
        //                                                       ":/images/background3.png"), 1, 0);
       // cxbackgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
            //                                                   ":/images/background4.png"), 1, 1);


        //cx next
      //  cxbackgroundLayout->addAction(deleteAction);
     //   cxbackgroundLayout->addAction(toFrontAction);
     //   cxbackgroundLayout->addAction(sendBackAction);

        fontCombo = new QFontComboBox();
        connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
                this, SLOT(currentFontChanged(QFont)));

        fontSizeCombo = new QComboBox;
        fontSizeCombo->setEditable(true);
        for (int i = 8; i < 30; i = i + 2)
            fontSizeCombo->addItem(QString().setNum(i));
        QIntValidator *validator = new QIntValidator(2, 64, this);
        fontSizeCombo->setValidator(validator);
        connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(fontSizeChanged(QString)));

        fontColorToolButton = new QToolButton;
        fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));

        textAction = fontColorToolButton->menu()->defaultAction();
        fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
        fontColorToolButton->setAutoFillBackground(true);
        connect(fontColorToolButton, SIGNAL(clicked()),
                this, SLOT(textButtonTriggered()));

    //! [26]
        fillColorToolButton = new QToolButton;
        fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
        fillAction = fillColorToolButton->menu()->defaultAction();
        fillColorToolButton->setIcon(createColorToolButtonIcon(
                                         ":/images/floodfill.png", Qt::white));
        connect(fillColorToolButton, SIGNAL(clicked()),
                this, SLOT(fillButtonTriggered()));
    //! [26]

        lineColorToolButton = new QToolButton;
        lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
        lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
        lineAction = lineColorToolButton->menu()->defaultAction();
        lineColorToolButton->setIcon(createColorToolButtonIcon(
                                         ":/images/linecolor.png", Qt::black));
        connect(lineColorToolButton, SIGNAL(clicked()),
                this, SLOT(lineButtonTriggered()));

       // textToolBar = addToolBar(tr("Font"));
        cxbackgroundLayout->addWidget(fontCombo);
        cxbackgroundLayout->addWidget(fontSizeCombo);
        //cxbackgroundLayout->addAction(boldAction);
        //cxbackgroundLayout->addAction(italicAction);
       // cxbackgroundLayout->addAction(underlineAction);

       // colorToolBar = addToolBar(tr("Color"));
        cxbackgroundLayout->addWidget(fontColorToolButton);
        cxbackgroundLayout->addWidget(fillColorToolButton);
        cxbackgroundLayout->addWidget(lineColorToolButton);

        QToolButton *pointerButton = new QToolButton;
        pointerButton->setCheckable(true);
        pointerButton->setChecked(true);
        pointerButton->setIcon(QIcon(":/images/pointer.png"));
        QToolButton *linePointerButton = new QToolButton;
        linePointerButton->setCheckable(true);
        linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

        pointerTypeGroup = new QButtonGroup(this);
        pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
        pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
        connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
                this, SLOT(pointerGroupClicked(int)));

        sceneScaleCombo = new QComboBox;
        QStringList scales;
        scales << tr("10%") << tr("25%") << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
        sceneScaleCombo->addItems(scales);
        sceneScaleCombo->setCurrentIndex(1);
        connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(sceneScaleChanged(QString)));
        sceneScaleCombo->setCurrentIndex(1);


        QPushButton *btf = new QPushButton;
        btf->setIcon(QIcon(":/images/bringtofront.png"));
        connect(btf, SIGNAL(clicked()), this, SLOT(bringToFront()));

        QPushButton   *btb = new QPushButton;
        btb->setIcon(QIcon(":/images/sendtoback.png"));
        connect(btb, SIGNAL(clicked()), this, SLOT(sendToBack()));


        QPushButton   *bdel = new QPushButton;
        bdel->setIcon(QIcon(":/images/delete.png"));
        connect(bdel, SIGNAL(clicked()), this, SLOT(deleteItem()));


        cxbackgroundLayout->addWidget(btf);
        cxbackgroundLayout->addWidget(btb);
        cxbackgroundLayout->addWidget(bdel);






      //  pointerToolbar = addToolBar(tr("Pointer type"));
        cxbackgroundLayout->addWidget(pointerButton);
        cxbackgroundLayout->addWidget(linePointerButton);
        cxbackgroundLayout->addWidget(sceneScaleCombo);



    //    cxbackgroundLayout->setRowStretch(2, 10);
    //    cxbackgroundLayout->setColumnStretch(2, 10);

        QWidget *cxbackgroundWidget = new QWidget;
        cxbackgroundWidget->setLayout(cxbackgroundLayout);


    //cxend

//! [22]
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(120);
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
    toolBox->addItem(cxbackgroundWidget, tr("Tool"));



}
//! [22]

//! [23]
void DiagramTab::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"),
                                tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
//! [23]

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

//! [24]
void DiagramTab::createMenus()
{
 /*   fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
    */
}
//! [24]

//! [25]

void DiagramTab::createToolbars()
{
//! [25]
/*
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
*/
    /*
    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));

    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));

//! [26]
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()),
            this, SLOT(fillButtonTriggered()));
//! [26]

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
                                     ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()),
            this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("Font"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
*/
//! [27]
}

//! [27]

//! [28]
QWidget *DiagramTab::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(25, 25));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [28]

//! [29]
QWidget *DiagramTab::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{

    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(25, 25));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}
//! [29]

//! [30]
QMenu *DiagramTab::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}
//! [30]

//! [31]
QIcon DiagramTab::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(25, 25);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centred horizontally on button.
    QRect target(1, 0, 24, 24);
    QRect source(0, 0, 24, 24);
    painter.fillRect(QRect(0, 25, 25, 25), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}
//! [31]

//! [32]
QIcon DiagramTab::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
//! [32]
void DiagramTab::DiagramStateItemSelect(const QString&statename)
{
    emit SelectState(statename);
}
void DiagramTab::DiagramConnectItemSelect(const QString&connectname)
{
    emit SelectConnect(connectname);
}

//  [12/24/2012 cxy]
void DiagramTab::SetmodleType(int itype)
{
    switch(itype)
    {
    case 1:
        m_myItemType = DiagramItem::Step;
        break;
    case 2:
        m_myItemType = DiagramItem::Conditional;
        break;
    case 3:
        m_myItemType = DiagramItem::StartEnd;
        break;
    case 4:
        m_myItemType = DiagramItem::Io;
        break;
    default:
        m_myItemType = DiagramItem::Conditional;
        break;
    }
}
void DiagramTab::setmodelstring(const QString &strmodelname)
{
    m_strmodelname = strmodelname;

}
void DiagramTab::AddModel()
{
    m_ivnum++;
    if(m_ivnum>7)
    {
        m_ivnum =1;
        m_ihnum++;
    }
   scene->AddModel(m_ivgapsize*m_ivnum,m_ihgapsize*m_ihnum,m_myItemType,m_strmodelname);
}
void DiagramTab::AddModelList(const QStringList&modellist)
{
 for (int i = 0; i < modellist.size(); ++i)
 {
     setmodelstring(modellist[i]);
     SetmodleType(1);
     AddModel();
 }
}
void DiagramTab::AddModelConnectList(const QStringList&modellist)
{
    for(int i=0;i<modellist.size();i++)
    {
        if (modellist[i].contains("_"))
        {
            QChar qchar_ ='_';
            int num_=0;
            int ifilenamesize =modellist[i].size();
            for(int j=0;j<ifilenamesize;j++)
            {
                if(qchar_==modellist[i].at(j))
                    num_ = j;
            }
            QString modellinka=modellist[i].left(num_);
            QString modellinkb=modellist[i].right(ifilenamesize-num_-1);
            setconnectmodel(modellinka);
            setconnecttomodel(modellinkb);
            AddModelConnect();
        }
    }
}
void DiagramTab::setconnectmodel(const QString &amodel)
{
    m_strconnecttomodelname = amodel;
}
void DiagramTab::setconnecttomodel(const QString &bmodel)
{
    m_strconnectmodelname = bmodel;
}
void DiagramTab::AddModelConnect()
{
    scene->AddModelConnect(m_strconnectmodelname,m_strconnecttomodelname);
}

void DiagramTab::AddModelConnectA(const QString &bmodela)
{
 //   NormalConnectlColor(m_strconnectmodelname+QString("_")+m_strconnecttomodelname);
    m_strconnectmodelname = bmodela;
}
void DiagramTab::AddModelConnectB(const QString &bmodelb)
{
    m_strconnecttomodelname = bmodelb;
    scene->AddModelConnect(m_strconnectmodelname,m_strconnecttomodelname);
    HighLightConnectlColor(m_strconnectmodelname+QString("_")+m_strconnecttomodelname);
}



void DiagramTab::SetModelColor(const QString&modelname)
{
    m_colormodelstring = modelname;
}
void DiagramTab::SetModelColorA(const char *pmodelname)
{
    m_colormodelstring = QString(pmodelname);
}

void DiagramTab::SetModelConnectColor(const QString&modelname)
{
    m_colormodelconnectstring = modelname;
}
void DiagramTab::HighLightModelColor( )
{
    scene->HighLighModleColor(m_colormodelstring);
    scene->update();
    view->update();
}
void DiagramTab::HighLightConnectlColor( )
{
    scene->HighLighModleConnectColor(m_colormodelconnectstring);
    scene->update();
    view->update();
}
void DiagramTab::NormalModelColor( )
{
    scene->NormalModleColor(m_colormodelstring);
    scene->update();
    view->update();
}
void DiagramTab::NormalConnectlColor()
{
    scene->NormalModleConnectColor(m_colormodelconnectstring);
    scene->update();
    view->update();
}

void DiagramTab::HighLightModelColor(const QString &strmodel)
{
    scene->HighLighModleColor(strmodel);
    scene->update();
    view->update();

}
void DiagramTab::HighLightConnectlColor(const QString &strconnect)
{
    scene->HighLighModleConnectColor(strconnect);
    scene->update();
    view->update();
}
void DiagramTab::NormalModelColor(const QString &strmodel)
{
    scene->NormalModleColor(strmodel);
    scene->update();
    view->update();
}
void DiagramTab::NormalConnectlColor(const QString &strconnect)
{
    scene->NormalModleConnectColor(strconnect);
    scene->update();
    view->update();
}
void DiagramTab::HighlighterDebugModel()
{
    if(m_colormodelstring!=m_lastmodelcolorstring)
    {
        QString qcurstring = m_colormodelstring;
        m_colormodelstring = m_lastmodelcolorstring;
        NormalModelColor();
        m_colormodelstring =qcurstring;
        HighLightModelColor();
        m_lastmodelcolorstring=qcurstring;
    }
    if(m_colormodelconnectstring!=m_lastmodelconnectcolorstring)
    {
        QString qcurstring = m_colormodelconnectstring;
        m_colormodelconnectstring = m_lastmodelconnectcolorstring;
        NormalConnectlColor();
        m_colormodelconnectstring =qcurstring;
        HighLightConnectlColor();
        m_lastmodelconnectcolorstring=qcurstring;
    }
}
void DiagramTab::deleteAllItem()
{
    m_ivgapsize = 150;
    m_ihgapsize = 150;
    m_ivnum = 1;
    m_ihnum = 1;

    foreach (QGraphicsItem *item, scene->items()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scene->items()) {
        if (item->type() == DiagramItem::Type) {
            qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
        }
        scene->removeItem(item);
        delete item;
    }

}
//! [3]
void DiagramTab::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_cleartimer.timerId())
    {
        scene->NormalAllModleColor();
        scene->update();
    }
    else
        QObject::timerEvent(event);
}
