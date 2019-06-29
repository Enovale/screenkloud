#include "editordialog.h"
#include "ui_editordialog.h"
#include <QScrollBar>

EditorDialog::EditorDialog(QWidget *parent, QImage *image) :
    QDialog(parent),
    ui(new Ui::EditorDialog)
{
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));

    toolkit = new KaptionGraphicsToolkit(ui->propertyToolbar, this);
    //Set img
    this->img = image;
    this->pixmap = QPixmap::fromImage(*image);
    setupUi();
}

EditorDialog::~EditorDialog()
{
    disconnect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
    delete ui;
    delete toolkit;
}

void EditorDialog::setupUi()
{
    setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    //Try to use theme icons if available
    ui->formatTextBtn->setIcon(QIcon::fromTheme("draw-text", ui->formatTextBtn->icon()));
    /*
    ui->arrowBtn->setIcon(QIcon::fromTheme("draw-arrow-right", ui->arrowBtn->icon()));
    ui->boxBtn->setIcon(QIcon::fromTheme("draw-rectangle", ui->boxBtn->icon()));
    ui->ellipseBtn->setIcon(QIcon::fromTheme("draw-circle", ui->ellipseBtn->icon()));
    ui->textBtn->setIcon(QIcon::fromTheme("draw-text", ui->textBtn->icon()));
    ui->numberedBtn->setIcon(QIcon::fromTheme("draw-number", ui->numberedBtn->icon()));
    */

    //Set default color
    ui->colorBtn->setColor(QColor(225, 50, 50));

    toolkit->bindButtonToGraphicsItem<ArrowGraphicsItem>(ui->arrowBtn, true);
    toolkit->bindButtonToGraphicsItem<BoxGraphicsItem>(ui->boxBtn);
    toolkit->bindButtonToGraphicsItem<EllipseGraphicsItem>(ui->ellipseBtn);
    toolkit->bindButtonToGraphicsItem<NumberedItem>(ui->numberedBtn);
    toolkit->bindButtonToGraphicsItem<BoxTextGraphicsItem>(ui->textBtn);

    toolkit->bindPropertyTool(new ColorPropertyToolEditor(ui->colorBtn, this), "color");
    toolkit->bindPropertyTool(new ScalePropertyToolEditor(ui->widthSlider, this), "size");
    toolkit->bindPropertyTool(new FontPropertyToolEditor(ui->formatTextBtn, this), "font");
    toolkit->bindPropertyTool(new NumberPropertyToolEditor(this), "number");

    toolkit->updateUi();

    ui->snapshotCanvas->setToolkit(toolkit);
    ui->snapshotCanvas->setPixmap(this->pixmap);
    this->adjustSize();
}

void EditorDialog::dialogAccepted()
{
    QPainter p(this->img);
    p.setRenderHint(QPainter::Antialiasing);
    if(ui->snapshotCanvas->isTransformed())
    {
        ui->snapshotCanvas->verticalScrollBar()->setValue(0);
        ui->snapshotCanvas->horizontalScrollBar()->setValue(0);
    }
    ui->snapshotCanvas->deselectItems();
    ui->snapshotCanvas->render(&p, QRectF(), this->img->rect());
}
