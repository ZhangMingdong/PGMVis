#include "LayerItemList.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>

LayerItemList::LayerItemList(QWidget *parent)
    : QListWidget(parent)
{
    setAcceptDrops(true);
}

void LayerItemList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        startPos = event->pos();
    QListWidget::mousePressEvent(event);
}

void LayerItemList::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            performDrag();
    }
    QListWidget::mouseMoveEvent(event);
}

void LayerItemList::dragEnterEvent(QDragEnterEvent *event)
{
    LayerItemList *source =
            qobject_cast<LayerItemList *>(event->source());
    if (source && source != this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void LayerItemList::dragMoveEvent(QDragMoveEvent *event)
{
    LayerItemList *source =
            qobject_cast<LayerItemList *>(event->source());
    if (source && source != this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void LayerItemList::dropEvent(QDropEvent *event)
{
    LayerItemList *source =
            qobject_cast<LayerItemList *>(event->source());
    if (source && source != this) {
        addItem(event->mimeData()->text());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void LayerItemList::performDrag()
{
    QListWidgetItem *item = currentItem();
    if (item) {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(item->text());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/images/person.png"));
        if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
            delete item;
    }
}
