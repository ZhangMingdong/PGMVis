#include <QtGui>

#include "MainWindow.h"
#include "BGLink.h"
#include "BGSphere.h"
#include "BGSphereText.h"
#include "BGRect.h"
#include "BGLine.h"
#include "BGRectangleText.h"
#include "BGArc.h"
#include "LayersWidget.h"
#include "LayerItemList.h"

#include "mygraphicsview.h"

#include "BrushWidget.h"
#include "TransformWidget.h"
#include "GeometryWidget.h"
#include "LocationWidget.h"
#include "LinkPropertyWidget.h"
#include "PenWidget.h"
#include "ArcWidget.h"
#include "BGGroup.h"

#include "GroupWidget.h"
#include "DataWidget.h"

#include "XGSWitch.h"

#include "BGPolygon.h"

#include "MyVisView.h"
#include "MyVisScene.h"


#include "PGMNode.h"
#include "ProbabilityWidget.h"


#include <QGraphicsScene>
#include <QColorDialog>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QtSvg>
#include <QSettings>
#include <QQueue>

#include <stdlib.h>

const QString ShowGrid("ShowGrid");
const QString MostRecentFile("MostRecentFile");
const QString MimeType = "application/vnd.qtrac.polygon";

template<template<typename T> class S, typename T>
T min(const S<T> &sequence)
{
	Q_ASSERT(!sequence.isEmpty());
	T minimum = sequence.first();
	foreach(const T &item, sequence)
	if (item < minimum)
		minimum = item;
	return minimum;
}

template<template<typename T> class S, typename T>
T max(const S<T> &sequence)
{
	Q_ASSERT(!sequence.isEmpty());
	T maximum = sequence.first();
	foreach(const T &item, sequence)
	if (item > maximum)
		maximum = item;
	return maximum;
}

namespace AQP{

	template<typename T>
	bool okToClearData(bool (T::*saveData)(), T *parent,
		const QString &title, const QString &text,
		const QString &detailedText = QString())
	{
		Q_ASSERT(saveData && parent);
#if QT_VERSION >= 0x040600
		QScopedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#else
		QSharedPointer<QMessageBox> messageBox(new QMessageBox(parent));
#endif
		messageBox->setWindowModality(Qt::WindowModal);
		messageBox->setIcon(QMessageBox::Question);
		messageBox->setWindowTitle(QString("%1 - %2")
			.arg(QApplication::applicationName()).arg(title));
		messageBox->setText(text);
		if (!detailedText.isEmpty())
			messageBox->setInformativeText(detailedText);
		messageBox->addButton(QMessageBox::Save);
		messageBox->addButton(QMessageBox::Discard);
		messageBox->addButton(QMessageBox::Cancel);
		messageBox->setDefaultButton(QMessageBox::Save);
		messageBox->exec();
		if (messageBox->clickedButton() ==
			messageBox->button(QMessageBox::Cancel))
			return false;
		if (messageBox->clickedButton() ==
			messageBox->button(QMessageBox::Save))
			return (parent->*saveData)();
		return true;
	}

}

MainWindow::MainWindow() :minZ(0)
	, maxZ(0)
	, seqNumber(0)
	, m_bPaste(false)
	, m_nCurrentHistory(0)
	, m_nFrontHistory(0)
	, m_nBackHistory(0)
	, m_bOpening(false)
	, m_bMousePressed(false)
	, m_bElementDragged(false)

{
	createSceneAndView();
	createActions();
	createMenus();
	createToolBars();
	populateMenusAndToolBars();
	createDockWidgets();
	createConnections();

	QSettings settings;
	viewShowGridAction->setChecked(
		settings.value(ShowGrid, true).toBool());
	QString filename = settings.value(MostRecentFile).toString();
	if (filename.isEmpty() || filename == tr("Unnamed"))
		QTimer::singleShot(0, this, SLOT(fileNew()));
	else {
		setWindowFilePath(filename);
		QTimer::singleShot(0, this, SLOT(loadFile()));
	}

	updateActions();


}

void MainWindow::createSceneAndView(){
	_pGraphicScene = new QGraphicsScene(0, 0, 600, 500);
	_pGraphicScene->setSceneRect(0, 0, 600, 500);
	_pGraphicView = new MyGraphicsView();
	_pGraphicView->setScene(_pGraphicScene);
	_pGraphicView->setDragMode(QGraphicsView::RubberBandDrag);
	_pGraphicView->setRenderHints(QPainter::Antialiasing
		| QPainter::TextAntialiasing);
	_pGraphicView->setContextMenuPolicy(Qt::ActionsContextMenu);

	_pVisScene = new MyVisScene(/*0, 0, 600, 500*/);
	_pVisScene->setSceneRect(0, 0, 600, 500);
	_pVisView = new MyVisView();
	_pVisView->setScene(_pVisScene);


	/*
	_pSpliter = new QSplitter(this);
	_pSpliter->addWidget(_pGraphicView);
	_pSpliter->addWidget(_pVisView);
	setCentralWidget(_pSpliter);
	*/
	setCentralWidget(_pGraphicView);


}

void MainWindow::createDockWidgets(){
	setDockOptions(QMainWindow::AnimatedDocks);
	QDockWidget::DockWidgetFeatures features =
		QDockWidget::DockWidgetMovable |
		QDockWidget::DockWidgetFloatable;

	transformWidget = new TransformWidget;
	geometryWidget = new GeometryWidget;
	locationWidget = new LocationWidget;
	probabilityWidget = new ProbabilityWidget;
	linkPropertyWidget = new LinkPropertyWidget;
	m_pWidgetData = new DataWidget;
	groupWidget = new GroupWidget;
	arcWidget = new ArcWidget;
	brushWidget = new BrushWidget;
	penWidget = new PenWidget;

	// layer widget
	m_pCurrentLayer = new LayersWidget();


	toolBox = new QToolBox;
	toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
	toolBox->setMinimumWidth(m_pWidgetData->sizeHint().width());
	toolBox->addItem(transformWidget, tr("Transform"));
	toolBox->addItem(geometryWidget, tr("Geometry"));
	toolBox->addItem(locationWidget, tr("Location"));
	toolBox->addItem(probabilityWidget, tr("Probability"));
	toolBox->addItem(linkPropertyWidget, tr("Link Property"));
	toolBox->addItem(m_pWidgetData, tr("Data"));
	toolBox->addItem(groupWidget, tr("Group"));
	toolBox->addItem(arcWidget, tr("Arc"));
	toolBox->addItem(brushWidget, tr("Brush (Fill)"));
	toolBox->addItem(penWidget, tr("Pen (Outline)"));

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(toolBox);
	layout->addWidget(_pGraphicView);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);
	widget->setLayout(layout);
	setCentralWidget(widget);

}

void MainWindow::createConnections(){

	// file    
	connect(fileNewAction, SIGNAL(triggered()),	this, SLOT(fileNew()));
	connect(fileOpenAction, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(fileSaveAction, SIGNAL(triggered()), this, SLOT(fileSave()));   
	connect(fileSaveAsAction, SIGNAL(triggered()), this, SLOT(fileSaveAs()));

	connect(exportSVGAction, SIGNAL(triggered()), this, SLOT(exportSVG()));
	connect(setBackgroundAction, SIGNAL(triggered()), this, SLOT(setBackground()));
	connect(fileQuitAction, SIGNAL(triggered()), this, SLOT(close()));


	// state
	connect(drawSphereAction, SIGNAL(toggled(bool)), _pGraphicView, SLOT(drawItem(bool)));
	connect(drawRectAction, SIGNAL(toggled(bool)), _pGraphicView, SLOT(drawItem(bool)));
	connect(drawLineAction, SIGNAL(toggled(bool)), _pGraphicView, SLOT(drawItem(bool)));
	connect(drawPolygonAction, SIGNAL(toggled(bool)), _pGraphicView, SLOT(drawItem(bool)));
	connect(drawLinkAction, SIGNAL(toggled(bool)), _pGraphicView, SLOT(drawItem(bool)));
	connect(stateSelectAction, SIGNAL(toggled(bool)), _pGraphicView, SLOT(drawItem(bool)));
	connect(_pGraphicView, SIGNAL(DrawGraphicsItem(IAbstractItem*)), this, SLOT(drawGraphicsItem(IAbstractItem*)));
	connect(_pGraphicView, SIGNAL(DrawLink(IAbstractItem* , IAbstractItem* ))
		, this, SLOT(drawLink(IAbstractItem*, IAbstractItem*)));
	// layer
// 	connect(newLayerAction, SIGNAL(triggered()), this, SLOT(newLayer()));
// 	connect(m_pCurrentLayer, SIGNAL(changeVisibility(LayersWidget*, bool))
// 		, this, SLOT(visibilityChanged(LayersWidget*, bool)));
	connect(_pGraphicScene, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

	// edit
	connect(addSphereAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addRectAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addLineAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addTextAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addArcAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addSwitchAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addNodeAction, SIGNAL(triggered()), this, SLOT(editAddItem()));
	connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));
	connect(editDeleteAction, SIGNAL(triggered()), this, SLOT(editDel()));
	connect(editCutAction, SIGNAL(triggered()), this, SLOT(editCut()));
	connect(editCopyAction, SIGNAL(triggered()), this, SLOT(editCopy()));
	connect(editPasteAction, SIGNAL(triggered()), this, SLOT(editPaste()));
	connect(bringToFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));
	connect(sendToBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));
	connect(propertiesAction, SIGNAL(triggered()), this, SLOT(properties()));
	connect(editUndoAction, SIGNAL(triggered()), this, SLOT(undo()));
	connect(editRedoAction, SIGNAL(triggered()), this, SLOT(redo()));
	foreach(QAction *action, QList<QAction*>()
		<< editAlignLeftAction
		<< editAlignHCenterAction
		<< editAlignRightAction 
		<< editAlignTopAction
		<< editAlignVCenterAction
		<< editAlignBottomAction)
		connect(action, SIGNAL(triggered()), this, SLOT(editAlign()));
	foreach(QAction *action, QList<QAction*>()
		<< editGroupAlignLeftAction
		<< editGroupAlignHCenterAction
		<< editGroupAlignRightAction
		<< editGroupAlignTopAction
		<< editGroupAlignVCenterAction
		<< editGroupAlignBottomAction)
		connect(action, SIGNAL(triggered()), this, SLOT(editGroupAlign()));


	connect(editAlignHDistributeAction, SIGNAL(triggered()), this, SLOT(editDistribute()));
	connect(editAlignVDistributeAction, SIGNAL(triggered()), this, SLOT(editDistribute()));
	connect(editGroupAlignHDistributeAction, SIGNAL(triggered()), this, SLOT(editGroupDistribute()));
	connect(editGroupAlignVDistributeAction, SIGNAL(triggered()), this, SLOT(editGroupDistribute()));

	connect(editClearTransformsAction, SIGNAL(triggered()), this, SLOT(editClearTransforms()));
	connect(editCombineAction, SIGNAL(triggered()), this, SLOT(editCombine()));
	connect(editDetachAction, SIGNAL(triggered()), this, SLOT(editDetach()));
	connect(editEdgeCompress, SIGNAL(triggered()), this, SLOT(slotEditEdgeCompress()));

	foreach(QAction *action, QList<QAction*>()
		<< editLinkStyleAction << editLinkStyleLineAction << editLinkStyleCurveAction)
		connect(action, SIGNAL(triggered()), this, SLOT(editLinkStyle()));


	// view
	connect(viewShowGridAction, SIGNAL(toggled(bool)),	_pGraphicView, SLOT(viewShowGrid(bool)));
	connect(viewZoomInAction, SIGNAL(triggered()),	_pGraphicView, SLOT(zoomIn()));
	connect(viewZoomOutAction, SIGNAL(triggered()), _pGraphicView, SLOT(zoomOut()));
	connect(_pGraphicView, SIGNAL(MousePress()), this, SLOT(onMousePressed()));
	connect(_pGraphicView, SIGNAL(MouseRelease()), this, SLOT(onMouseReleased()));

}

void MainWindow::updateActions()
{
	// undo and redo
	editUndoAction->setEnabled(m_nCurrentHistory != (m_nBackHistory + 1) % MAX_HISTORY_LENGTH);
	editRedoAction->setEnabled(m_nFrontHistory!=m_nCurrentHistory);


	bool hasSelection = !_pGraphicScene->selectedItems().isEmpty();
	bool isNode = (selectedNode() != 0);
	bool isNodePair = (selectedNodePair() != BGPair());
	bool isGroup = (selectedGroup() != 0);
	bool isLink = (selectedLink() != 0);

	editCutAction->setEnabled(isNode);
	editCopyAction->setEnabled(isNode);
	addLinkAction->setEnabled(isNodePair);
	editDeleteAction->setEnabled(hasSelection);
	bringToFrontAction->setEnabled(isNode);
	sendToBackAction->setEnabled(isNode);
	propertiesAction->setEnabled(isNode);

	foreach(QAction *action, _pGraphicView->actions())
		_pGraphicView->removeAction(action);

	foreach(QAction *action, editMenu->actions()) {
		if (action->isEnabled())
			_pGraphicView->addAction(action);
	}

	foreach(QAction *action, arrangeMenu->actions()) {
		if (action->isEnabled())
			_pGraphicView->addAction(action);
	}

	fileSaveAction->setEnabled(isWindowModified());
	int selected = _pGraphicScene->selectedItems().count();
// 	bool hasItems = scene->items().length() > 0;

// 	fileSaveAsAction->setEnabled(hasItems);
// 	fileExportAction->setEnabled(hasItems);
// 	filePrintAction->setEnabled(hasItems);
// 	editSelectedItemAction->setEnabled(selected == 1);
	editCopyAction->setEnabled(selected >= 1);
	editCutAction->setEnabled(selected >= 1);
	QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();
	editPasteAction->setEnabled(mimeData &&
		(mimeData->hasFormat(MimeType) || mimeData->hasHtml() ||
		mimeData->hasText()));
// 	editAlignmentAction->setEnabled(selected >= 2);
	editAlignLeftAction->setEnabled(selected >= 2);
	editAlignHCenterAction->setEnabled(selected >= 2);
	editAlignRightAction->setEnabled(selected >= 2);
	editAlignTopAction->setEnabled(selected >= 2);
	editAlignVCenterAction->setEnabled(selected >= 2);
	editAlignBottomAction->setEnabled(selected >= 2);
	editAlignHDistributeAction->setEnabled(selected >= 2);
	editAlignVDistributeAction->setEnabled(selected >= 2);
	editGroupAlignLeftAction->setEnabled(selected >= 2);
	editGroupAlignHCenterAction->setEnabled(selected >= 2);
	editGroupAlignRightAction->setEnabled(selected >= 2);
	editGroupAlignTopAction->setEnabled(selected >= 2);
	editGroupAlignVCenterAction->setEnabled(selected >= 2);
	editGroupAlignBottomAction->setEnabled(selected >= 2);
	editGroupAlignHDistributeAction->setEnabled(selected >= 2);
	editGroupAlignVDistributeAction->setEnabled(selected >= 2);

	editClearTransformsAction->setEnabled(selected >= 1);
	transformWidget->setEnabled(selected >= 1);
// 	groupWidget->setEnabled(isLink);
	// link style
	editLinkStyleAction->setEnabled(isLink);
	if (isLink)
	{
		if (selectedLink()->GetStyle()==BGLink::LS_Curve)
		{
			editLinkStyleAction->setIcon(QIcon(":/images/curve.png"));
			editLinkStyleAction->setData(BGLink::LS_Curve);
		}
		else{
			editLinkStyleAction->setIcon(QIcon(":/images/line.png"));
			editLinkStyleAction->setData(BGLink::LS_Line);
		}
	}
	arcWidget->setEnabled(selected >= 1);
	editCombineAction->setEnabled(selected >= 2);
	editDetachAction->setEnabled(selectedGroup());	// Need to be mended.
	m_pWidgetData->setEnabled(selected == 1);
	bool hasBrushProperty;
	bool hasPenProperty;
	getSelectionProperties(&hasBrushProperty, &hasPenProperty);
	brushWidget->setEnabled(hasBrushProperty);
	penWidget->setEnabled(hasPenProperty);
}

void MainWindow::createActions()
{
	fileNewAction = new QAction(QIcon(":/images/filenew.png"),
		tr("New..."), this);
	fileNewAction->setShortcuts(QKeySequence::New);
	fileOpenAction = new QAction(QIcon(":/images/fileopen.png"),
		tr("Open..."), this);
	fileOpenAction->setShortcuts(QKeySequence::Open);
	fileSaveAction = new QAction(QIcon(":/images/filesave.png"),
		tr("Save"), this);
	fileSaveAction->setShortcuts(QKeySequence::Save);
	fileSaveAsAction = new QAction(QIcon(":/images/filesave.png"),
		tr("Save As..."), this);
	fileSaveAsAction->setShortcuts(QKeySequence::SaveAs);
	fileExportAction = new QAction(QIcon(":/images/fileexport.png"),
		tr("Export..."), this);
	filePrintAction = new QAction(QIcon(":/images/fileprint.png"),
		tr("Print..."), this);
	fileQuitAction = new QAction(QIcon(":/images/filequit.png"),
		tr("Quit"), this);
	fileQuitAction->setShortcuts(QKeySequence::Quit);


	exportSVGAction = new QAction(QIcon(":/images/export32.png"), tr("ExportSVG"), this);
	setBackgroundAction = new QAction(QIcon(":/images/background.png"), tr("SetBackground"), this);

	// edit

	addSphereAction = new QAction(tr("Add Sphere"), this);
	addSphereAction->setData(SphereItemType);
	addSphereAction->setIcon(QIcon(":/images/Sphere.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));

	addRectAction = new QAction(tr("Add Rectangle"), this);
	addRectAction->setData(RectItemType);
	addRectAction->setIcon(QIcon(":/images/editaddbox.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));

	addLineAction = new QAction(tr("Add Line"), this);
	addLineAction->setData(LineItemType);
	addLineAction->setIcon(QIcon(":/images/solidlinestyle.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));

	addTextAction = new QAction(tr("Add Text"), this);
	addTextAction->setData(TextItemType);
	addTextAction->setIcon(QIcon(":/images/editaddtext.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));


	addArcAction = new QAction(tr("Add Arc"), this);
	addArcAction->setData(ArcItemType);
	addArcAction->setIcon(QIcon(":/images/Rainbow.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));
	addSwitchAction = new QAction(tr("Add Switch"), this);
	addSwitchAction->setData(SwitchItemType);
	addSwitchAction->setIcon(QIcon(":/images/switch.png"));


	addNodeAction = new QAction(tr("Add Node"), this);
	addNodeAction->setData(NodeItemType);
	addNodeAction->setIcon(QIcon(":/images/node.png"));

	addLinkAction = new QAction(tr("Add &Link"), this);
	addLinkAction->setIcon(QIcon(":/images/link.png"));
	addLinkAction->setShortcut(tr("Ctrl+L"));

	editDeleteAction = new QAction(tr("&Delete"), this);
	editDeleteAction->setIcon(QIcon(":/images/delete.png"));
	editDeleteAction->setShortcut(tr("Del"));

	editCutAction = new QAction(tr("Cu&t"), this);
	editCutAction->setIcon(QIcon(":/images/cut.png"));
	editCutAction->setShortcut(tr("Ctrl+X"));

	editCopyAction = new QAction(tr("&Copy"), this);
	editCopyAction->setIcon(QIcon(":/images/copy.png"));
	editCopyAction->setShortcut(tr("Ctrl+C"));

	editPasteAction = new QAction(tr("&Paste"), this);
	editPasteAction->setIcon(QIcon(":/images/paste.png"));
	editPasteAction->setShortcut(tr("Ctrl+V"));

	bringToFrontAction = new QAction(tr("Bring to &Front"), this);
	bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));

	sendToBackAction = new QAction(tr("&Send to Back"), this);
	sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));

	propertiesAction = new QAction(tr("P&roperties..."), this);
	propertiesAction->setIcon(QIcon(":/images/Draft.png"));

	editUndoAction = new QAction(tr("&Undo"), this);
	editUndoAction->setIcon(QIcon(":/images/Undo.png"));

	editRedoAction = new QAction(tr("&Rndo"), this);
	editRedoAction->setIcon(QIcon(":/images/Redo.png"));



	editClearTransformsAction = new QAction(QIcon(":/images/editcleartransforms.png"),tr("Clear Transformations"), this);
	editCombineAction = new QAction(QIcon(":/images/Combine.png"), tr("Combine Items"), this);
	editDetachAction = new QAction(QIcon(":/images/Detach.png"), tr("Detach Combined Items"), this);
	editEdgeCompress = new QAction(QIcon(":/images/edgeCompress.png"), tr("Edge Compress"), this);

// 	QMenu *alignmentMenu = new QMenu(tr("Align"), this);
// 	foreach(QAction *action, QList<QAction*>()
// 		<< editAlignLeftAction << editAlignRightAction
// 		<< editAlignTopAction << editAlignBottomAction)
// 		alignmentMenu->addAction(action);
// 	editAlignmentAction->setMenu(alignmentMenu);

	// arrange
// 	editAlignmentAction = new QAction(QIcon(":/images/align-left.png"),
// 		tr("Alignment"), this);
// 	editAlignmentAction->setData(Qt::AlignLeft);
	editAlignLeftAction = new QAction(QIcon(":/images/align-left.png"), tr("Left"), this);
	editAlignLeftAction->setData(Qt::AlignLeft);
	editAlignHCenterAction = new QAction(QIcon(":/images/align-hcenter.png"), tr("HCenter"), this);
	editAlignHCenterAction->setData(Qt::AlignHCenter);
	editAlignRightAction = new QAction(QIcon(":/images/align-right.png"), tr("Right"), this);
	editAlignRightAction->setData(Qt::AlignRight);
	editAlignTopAction = new QAction(QIcon(":/images/align-top.png"), tr("Top"), this);
	editAlignTopAction->setData(Qt::AlignTop);
	editAlignVCenterAction = new QAction(QIcon(":/images/align-vcenter.png"), tr("VCenter"), this);
	editAlignVCenterAction->setData(Qt::AlignVCenter);
	editAlignBottomAction = new QAction(QIcon(":/images/align-bottom.png"), tr("Bottom"), this);
	editAlignBottomAction->setData(Qt::AlignBottom);

	editAlignHDistributeAction = new QAction(QIcon(":/images/align-hdistribute.png"), tr("HDistribute"), this);
	editAlignHDistributeAction->setData(Qt::AlignHCenter);
	editAlignVDistributeAction = new QAction(QIcon(":/images/align-vdistribute.png"), tr("VDistribute"), this);
	editAlignVDistributeAction->setData(Qt::AlignVCenter);

	editGroupAlignLeftAction = new QAction(QIcon(":/images/align-group-left.png"), tr("Left"), this);
	editGroupAlignLeftAction->setData(Qt::AlignLeft);
	editGroupAlignHCenterAction = new QAction(QIcon(":/images/align-group-hcenter.png"), tr("HCenter"), this);
	editGroupAlignHCenterAction->setData(Qt::AlignHCenter);
	editGroupAlignRightAction = new QAction(QIcon(":/images/align-group-right.png"), tr("Right"), this);
	editGroupAlignRightAction->setData(Qt::AlignRight);
	editGroupAlignTopAction = new QAction(QIcon(":/images/align-group-top.png"), tr("Top"), this);
	editGroupAlignTopAction->setData(Qt::AlignTop);
	editGroupAlignVCenterAction = new QAction(QIcon(":/images/align-group-vcenter.png"), tr("VCenter"), this);
	editGroupAlignVCenterAction->setData(Qt::AlignVCenter);
	editGroupAlignBottomAction = new QAction(QIcon(":/images/align-group-bottom.png"), tr("Bottom"), this);
	editGroupAlignBottomAction->setData(Qt::AlignBottom);

	editGroupAlignHDistributeAction = new QAction(QIcon(":/images/align-group-hdistribute.png"), tr("HDistribute"), this);
	editGroupAlignHDistributeAction->setData(Qt::AlignHCenter);
	editGroupAlignVDistributeAction = new QAction(QIcon(":/images/align-group-vdistribute.png"), tr("VDistribute"), this);
	editGroupAlignVDistributeAction->setData(Qt::AlignVCenter);

	editLinkStyleAction = new QAction(QIcon(":/images/curve.png"),	tr("LinkStyle"), this);
	editLinkStyleAction->setData(BGLink::LS_Curve);
	editLinkStyleLineAction = new QAction(QIcon(":/images/line.png"),	tr("Line"), this);
	editLinkStyleLineAction->setData(BGLink::LS_Line);
	editLinkStyleCurveAction = new QAction(QIcon(":/images/curve.png"), tr("Curve"), this);
	editLinkStyleCurveAction->setData(BGLink::LS_Curve);
	
	QMenu *linkStyleMenu = new QMenu(tr("LinkStyle"), this);
	foreach(QAction *action, QList<QAction*>()<< editLinkStyleLineAction<< editLinkStyleCurveAction)
		linkStyleMenu->addAction(action);
	editLinkStyleAction->setMenu(linkStyleMenu);


	// layer
// 	newLayerAction = new QAction(tr("New"), this);

	// view
	viewShowGridAction = new QAction(tr("Show Grids"), this);
	viewShowGridAction->setIcon(QIcon(":/images/showgrid.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));
	viewShowGridAction->setCheckable(true);  
	viewZoomInAction = new QAction(QIcon(":/images/zoom-in.png"), tr("Zoom In"), this);
	viewZoomInAction->setShortcut(tr("+"));
	viewZoomOutAction = new QAction(QIcon(":/images/zoom-out.png"), tr("Zoom Out"), this);
	viewZoomOutAction->setShortcut(tr("-"));

	// state
	drawSphereAction = new QAction(tr("Draw Sphere"), this);
	drawSphereAction->setData(SphereItemType);
	drawSphereAction->setIcon(QIcon(":/images/Sphere.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));
	drawSphereAction->setCheckable(true);

	drawRectAction = new QAction(tr("Draw Rectangle"), this);
	drawRectAction->setData(RectItemType);
	drawRectAction->setIcon(QIcon(":/images/editaddbox.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));
	drawRectAction->setCheckable(true);

	drawLineAction = new QAction(tr("Draw Line"), this);
	drawLineAction->setData(LineItemType);
	drawLineAction->setIcon(QIcon(":/images/solidlinestyle.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));
	drawLineAction->setCheckable(true);

	drawPolygonAction = new QAction(tr("Draw Polygon"), this);
	drawPolygonAction->setData(PolygonItemType);
	drawPolygonAction->setIcon(QIcon(":/images/editselecteditem.png"));
	// 	addSphereAction->setShortcut(tr("Ctrl+N"));
	drawPolygonAction->setCheckable(true);

	drawLinkAction = new QAction(tr("Draw Link"), this);
	drawLinkAction->setIcon(QIcon(":/images/link.png"));
	// 	drawLinkAction->setShortcut(tr("Ctrl+L"));
	drawLinkAction->setData(LinkItemType);
	drawLinkAction->setCheckable(true);

	stateSelectAction = new QAction(tr("Draw Text"), this);
	stateSelectAction->setData(NullItemType);
	stateSelectAction->setIcon(QIcon(":/images/stateselect.png"));
	stateSelectAction->setCheckable(true);
	stateSelectAction->setChecked(true);

	QActionGroup* stateGroup = new QActionGroup(this);

	stateGroup->addAction(drawPolygonAction);
	stateGroup->addAction(drawSphereAction);
	stateGroup->addAction(drawRectAction);
	stateGroup->addAction(drawLineAction);
	stateGroup->addAction(drawLinkAction);
	stateGroup->addAction(stateSelectAction);
}

void MainWindow::createMenus()
{
}

void MainWindow::createToolBars()
{
}

void MainWindow::populateMenusAndToolBars()
{
	QAction *separator = 0;
	// file
	QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	QToolBar *fileToolBar = addToolBar(tr("File"));
	populateMenuAndToolBar(fileMenu, fileToolBar, QList<QAction*>()
		<< fileNewAction 
		<< fileOpenAction 
		<< fileSaveAction
// 		<< fileExportAction 
		<< exportSVGAction
		<< separator
		<< setBackgroundAction
		<< separator 
		<< filePrintAction);
	fileMenu->insertAction(exportSVGAction, fileSaveAsAction);
	fileMenu->addSeparator();
	fileMenu->addAction(fileQuitAction);

	// edit
	editMenu = menuBar()->addMenu(tr("&Edit"));
	QToolBar* editToolBar = addToolBar(tr("Edit"));
	populateMenuAndToolBar(editMenu,editToolBar, QList<QAction*>()
		<< addSphereAction 
		<< addRectAction 
		<< addLineAction
		<< addTextAction
		<< addArcAction
		<< addSwitchAction
		<< addNodeAction
		<< addLinkAction
		<< editDeleteAction
		<< separator
		<< editCutAction
		<< editCopyAction
		<< editPasteAction
		<< separator
		<< bringToFrontAction
		<< sendToBackAction
		<< separator
		<< editClearTransformsAction
		<< separator
		<< editCombineAction
		<< editDetachAction
		<< editEdgeCompress
		<< separator
		<< propertiesAction
		<< editUndoAction
		<< editRedoAction
		<< editLinkStyleAction);
	arrangeMenu = menuBar()->addMenu(tr("&Arrange"));
	QToolBar* arrangeToolBar = addToolBar(tr("Arrange"));
	populateMenuAndToolBar(arrangeMenu, arrangeToolBar, QList<QAction*>()
		<< editAlignLeftAction
		<< editAlignHCenterAction
		<< editAlignRightAction
		<< editAlignTopAction
		<< editAlignVCenterAction
		<< editAlignBottomAction
		<< editAlignHDistributeAction
		<< editAlignVDistributeAction
		<< editGroupAlignLeftAction
		<< editGroupAlignHCenterAction
		<< editGroupAlignRightAction
		<< editGroupAlignTopAction
		<< editGroupAlignVCenterAction
		<< editGroupAlignBottomAction
		<< editGroupAlignHDistributeAction
		<< editGroupAlignVDistributeAction);

	// view
	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
	QToolBar* viewToolBar = addToolBar(tr("View"));
	populateMenuAndToolBar(viewMenu, viewToolBar, QList<QAction*>()
		<< viewZoomInAction 
		<< viewZoomOutAction 
		<< separator
		<< viewShowGridAction);

	// state
	QMenu* stateMenu = menuBar()->addMenu(tr("State"));
	QToolBar* stateToolBar = addToolBar(tr("State"));
	populateMenuAndToolBar(stateMenu, stateToolBar, QList<QAction*>()
		<< drawSphereAction
		<< drawRectAction
		<< drawLineAction
		<< drawPolygonAction
		<< drawLinkAction
		<< stateSelectAction);

	// layer
// 	QMenu* layerMenu = menuBar()->addMenu(tr("&Layer"));
// 	layerMenu->addAction(newLayerAction);

}

void MainWindow::exportSVG(){
	QString path;
	QString newPath = QFileDialog::getSaveFileName(this, tr("Save SVG"),
		path, tr("SVG files (*.svg)"));

	if (newPath.isEmpty())
		return;

	path = newPath;
	int width = 1000;
	int height = 1000;

	QSvgGenerator generator;
	generator.setFileName(path);
	generator.setSize(QSize(width, height));
	generator.setViewBox(QRect(0, 0, width, height));
	generator.setTitle(tr("SVG Generator Example Drawing"));
	generator.setDescription(tr("An SVG drawing created by the SVG Generator "
		"Example provided with Qt."));
	QPainter painter;
	painter.begin(&generator);
	paintScene(&painter);
// 	view->render(&painter, QRectF(0, 0, width, height), QRect(0, 0, this->width(), this->height()));

	painter.end();
}

void MainWindow::fileOpen()
{
	m_nCurrentHistory = 0;
	m_nBackHistory = 0;
	m_nFrontHistory = 0;

	if (!okToClearData())
		return;
	const QString &filename = QFileDialog::getOpenFileName(this,
		tr("%1 - Open").arg(QApplication::applicationName()),
		".", tr("XML files (*.xml)"));

	if (filename.isEmpty())
		return;

	setWindowFilePath(filename);
	loadFile();

}

bool MainWindow::fileSave()
{
	const QString filename = windowFilePath();
	if (filename.isEmpty() || filename == tr("Unnamed"))
		return fileSaveAs();
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
		return false;
	QXmlStreamWriter xmlWriter(&file);
	saveFile(xmlWriter);
	file.close();
	if (file.error()) {
		// 		std::cerr << "Error: Cannot write file "
		// 			<< qPrintable(fileName) << ": "
		// 			<< qPrintable(file.errorString()) << std::endl;
		return false;
	}

	setDirty(false);
	return true;
}

void MainWindow::saveFile(QXmlStreamWriter& xmlWriter){
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("diagram");

	writeItems(xmlWriter, _pGraphicScene->items());
	writeLinks(xmlWriter);
	writeGroups(xmlWriter, _pGraphicScene->items());

	xmlWriter.writeEndElement();
	xmlWriter.writeEndDocument();

}

void MainWindow::fileNew(){
	m_nCurrentHistory = 0;
	m_nBackHistory = 0;
	m_nFrontHistory = 0;
	if (!okToClearData())
		return;
	clear();
	setWindowFilePath(tr("Unnamed"));
	setDirty(false);
	recordHistory();
}

bool MainWindow::fileSaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this,
		tr("%1 - Save As").arg(QApplication::applicationName()),
		".", tr("XML files (*.xml)"));
	if (filename.isEmpty())
		return false;
	if (!filename.toLower().endsWith(".xml"))
		filename += ".xml";
	setWindowFilePath(filename);
	return fileSave();

// 	QString path;
// 	QString newPath = QFileDialog::getSaveFileName(this, tr("Save XML"),
// 		path, tr("XML files (*.xml)"));
// 
// 	if (newPath.isEmpty())
// 		return false;
// 
// 	path = newPath;
// 
// 	QFile file(path);
// 	if (!file.open(QFile::WriteOnly | QFile::Text)) {
// 		// 		std::cerr << "Error: Cannot write file "
// 		// 			<< qPrintable(fileName) << ": "
// 		// 			<< qPrintable(file.errorString()) << std::endl;
// 		return false;
// 	}
// 
// 	setDirty(false);
// 
// 	return true;
	return true;
}

void MainWindow::loadFile(){

	QFile file(windowFilePath());
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		// 		std::cerr << "Error: Cannot read file " << qPrintable(fileName)
		// 			<< ": " << qPrintable(file.errorString())
		// 			<< std::endl;
		return;
	}
	QXmlStreamReader reader(&file);
	loadFile(reader);

	file.close();
	if (reader.hasError()) {
		// 		std::cerr << "Error: Failed to parse file "
		// 			<< qPrintable(fileName) << ": "
		// 			<< qPrintable(reader.errorString()) << std::endl;
		QString errString = reader.errorString();
		return;
	}
	else if (file.error() != QFile::NoError) {
		// 		std::cerr << "Error: Cannot read file " << qPrintable(fileName)
		// 			<< ": " << qPrintable(file.errorString())
		// 			<< std::endl;
		return;
	}
	recordHistory();
	setDirty(false);


// 	BGPolygon* poly = new BGPolygon();
// 	QPolygonF polygon;
// 	polygon << QPointF(100, 100);
// 	polygon << QPointF(100, 200);
// 	polygon << QPointF(200, 200);
// 	polygon << QPointF(200, 100);
// 	poly->setPolygon(polygon);
// 
// 	this->scene->addItem(poly);
}

void MainWindow::loadFile(QXmlStreamReader& reader){
	m_bOpening = true;
	clear();
	while (!reader.atEnd()) {
		if (reader.isStartElement()) {
			if (reader.name() == "diagram") {
				readDiagram(reader);
			}
			else {
				reader.raiseError(QObject::tr("Not a Diagram file"));
			}
		}
		else
		{
			reader.readNext();
		}
	}
	m_bOpening = false;

// 		BGPolygon* poly = new BGPolygon();
// 		QPolygonF polygon;
// 		polygon << QPointF(100, 100);
// 		polygon << QPointF(100, 300);
// 		polygon << QPointF(250, 220);
// 		polygon << QPointF(200, 90);
// 		poly->setPolygon(polygon);
// 		this->scene->addItem(poly);

// 		BGPolygon* poly2 = new BGPolygon();
// 		QPolygonF polygon2;
// 		polygon2 << QPointF(245, 222);
// 		polygon2 << QPointF(125, 265);
// 		polygon2 << QPointF(82, 146);
// 		polygon2 << QPointF(201, 103);
// 		poly2->setPolygon(polygon2);
// 		this->scene->addItem(poly2);
// 		bool b=poly->intersected(polygon2);
// 		int w = 111;

}

void MainWindow::writeItems(QXmlStreamWriter& writer, const QList<QGraphicsItem*> &items)
{
	writer.writeStartElement("items");
	for each (QGraphicsItem* item in items)
	{
		IBasicGraphicItem* g = dynamic_cast<IBasicGraphicItem*>(item);
		if (g)
		{
			g->Write(writer);
		}
	}
	writer.writeEndElement();
}

void MainWindow::writeLinks(QXmlStreamWriter& writer)
{
	writer.writeStartElement("links");
	for each (QGraphicsItem* item in _pGraphicScene->items())
	{
		BGLink* l = dynamic_cast<BGLink*>(item);
		if (l)
		{
			l->Write(writer);
		}
	}
	writer.writeEndElement();
}

void MainWindow::writeGroups(QXmlStreamWriter& writer, const QList<QGraphicsItem*> &items)
{
	writer.writeStartElement("groups");
	for each (QGraphicsItem* item in items)
	{
		BGGroup* g = dynamic_cast<BGGroup*>(item);
		if (g)
		{
			g->Write(writer);
		}
	}
	writer.writeEndElement();
}

void MainWindow::readDiagram(QXmlStreamReader& reader, bool bNewId)
{
	reader.readNext();					// skip <diagram>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {	//reach </diagram>
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "items") {
				readItems(reader,bNewId);
			}
			else if (reader.name() == "groups") {
				readGroups(reader);
			}
			else if (reader.name() == "links"){
				readLinks(reader);
			}
			else{
				IBasicGraphicItem::skipUnknownElement(reader);
			}
		}
		else {
			reader.readNext();			// skip space
		}
	}
	connectItemsAndGroups(bNewId);
}

void MainWindow::readGroups(QXmlStreamReader& reader)
{
	reader.readNext();					// skip <diagram>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {	//reach </diagram>
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "group") {
				readGroup(reader);
			}
			else{
				IBasicGraphicItem::skipUnknownElement(reader);
			}
		}
		else {
			reader.readNext();			// skip space
		}
	}
}

void MainWindow::readGroup(QXmlStreamReader& reader){
	BGGroup* group = new BGGroup();
	group->Read(reader);
	// 	scene->addItem(group);
// 	checkGroup(group);
	m_mapGroup[group->GetId()] = group;
}

void MainWindow::readItems(QXmlStreamReader& reader, bool bNewId)
{
	reader.readNext();					// skip <diagram>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {	//reach </diagram>
			reader.readNext();
			break;						
		}
		
		if (reader.isStartElement()) {
			IBasicGraphicItem *newItem = NULL;
			if (reader.name() == "sphere") {
// 				newItem = new BGSphere();
				newItem = new BGSphereText();
			}
			else if (reader.name() == "rect") {
				newItem = new BGRect();
			}
			else if (reader.name() == "switch") {
				newItem = new XGSWitch();
			}
			else if (reader.name() == "line") {
				newItem = new BGLine();
			}
			else if (reader.name() == "text") {
				newItem = new BGRectangleText();
			}
			else if (reader.name()=="arc")
			{
				newItem = new BGArc();
			}
			else{
				IBasicGraphicItem::skipUnknownElement(reader);
				continue;
			}
			newItem->Read_New(reader,bNewId);
			setupNode(newItem);
			checkGroup(newItem);
		}
		else {
			reader.readNext();			// skip space
		}
	}
}

void MainWindow::readLink(QXmlStreamReader& reader)
{
	QUuid idFrom;
	QUuid idTo;
	QUuid idFromPort;
	QUuid idToPort;
	int nFromPos;
	int nToPos;
	int style;
	int weight;
	reader.readNext();					// skip <diagram>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {	//reach </diagram>
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "from") {
				idFrom = QUuid(IBasicGraphicItem::readStringValue(reader));
			}
			else if (reader.name() == "to") {
				idTo = QUuid(IBasicGraphicItem::readStringValue(reader));
			}
			else if (reader.name() == "fromPort") {
				idFromPort = QUuid(IBasicGraphicItem::readStringValue(reader));
			}
			else if (reader.name() == "toPort") {
				idToPort = QUuid(IBasicGraphicItem::readStringValue(reader));
			}
			else if (reader.name() == "fromPortPos") {
				idFromPort = QUuid(IBasicGraphicItem::readStringValue(reader));
			}
			else if (reader.name() == "toPortPos") {
				idToPort = QUuid(IBasicGraphicItem::readStringValue(reader));
			}
			else if (reader.name() == "fromPos") {
				nFromPos = IBasicGraphicItem::readIntValue(reader);
			}
			else if (reader.name() == "toPos") {
				nToPos = IBasicGraphicItem::readIntValue(reader);
			}
			else if (reader.name() == "style"){
				style = IBasicGraphicItem::readIntValue(reader);
			}
			else if (reader.name() == "weight"){
				weight = IBasicGraphicItem::readIntValue(reader);
			}
			else{
				IBasicGraphicItem::skipUnknownElement(reader);
				continue;
			}
		}
		else {
			reader.readNext();			// skip space
		}
	}

	m_listReadedLinks.push_back({ idFrom, idTo, idFromPort, idToPort,nFromPos,nToPos,style,weight });
}

void MainWindow::readLinks(QXmlStreamReader& reader){
	reader.readNext();					// skip <diagram>
	while (!reader.atEnd()) {
		if (reader.isEndElement()) {	//reach </diagram>
			reader.readNext();
			break;
		}

		if (reader.isStartElement()) {
			if (reader.name() == "link") {
				readLink(reader);
			}
			else{
				IBasicGraphicItem::skipUnknownElement(reader);
				continue;
			}
		}
		else {
			reader.readNext();			// skip space
		}
	}
}

void MainWindow::readSphere(QXmlStreamReader& reader)
{
}

void MainWindow::checkGroup(IAbstractItem* item)
{
	QUuid groupid = item->GetGroupId();
	if (groupid != QUuid(""))
	{
		m_mapItemsInGroup[item] = groupid;
	}
}

void MainWindow::readRect(QXmlStreamReader& reader)
{
}

void MainWindow::setBackground(){
	QString fileName = QFileDialog::getOpenFileName(this);
	_pGraphicScene->setBackgroundBrush(QBrush(QImage(fileName)));
}

void MainWindow::editAddItem()
{
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;
	QObject *item = 0;
	int type = action->data().toInt();
	switch (type)
	{
	case TextItemType:
	{					
		BGRectangleText *item = new BGRectangleText();
		item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		setupNode(item);
		break;
	}
	case LineItemType:
	{
		BGLine *item = new BGLine();
		// 	item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		QPointF pos = item->pos();
		item->SetFrom(QPointF( - 10,  - 10));
		item->SetTo(QPointF( + 10,  + 10));
		setupNode(item);
		break;
	}
	case RectItemType:
	{
		BGRect *item = new BGRect();
		// 	item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		QPointF pos = item->pos();

		setupNode(item);
		break;
	}
	case SphereItemType:
	{
// 		BGSphere *item = new BGSphere();
// 		// 	item->setText(tr("Node %1").arg(seqNumber + 1));
// 		setNodePos(item);
// 		setupNode(item);

		BGSphereText *item = new BGSphereText();
		// 	item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		setupNode(item);
		break;
	}
	case ArcItemType:
	{
		BGArc *item = new BGArc();
		// 	item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		setupNode(item);
		break;	
	}
	case SwitchItemType:
	{
		XGSWitch *item = new XGSWitch();
		// 	item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		setupNode(item);
		break;	
	}
	case NodeItemType:
	{
		PGMNode *item = new PGMNode();
		item->setText(tr("Node %1").arg(seqNumber + 1));
		setNodePos(item);
		setupNode(item);
		break;
	}
	default:
		return;
	}
// 	if (type == BoxItemType)
// 		item = new BoxItem(QRect(position(), QSize(90, 30)), scene);
// 	else if (type == SmileyItemType)
// 		item = new SmileyItem(position(), scene);
// 	else if (type == TextItemType) {
// 		TextItemDialog dialog(0, position(), scene, this);
// 		if (dialog.exec())
// 			item = dialog.textItem();
// 	}
// 	if (item) {
// 		connectItem(item);
// 		setDirty(true);
// 	}
}

void MainWindow::addLink()
{
	BGPair items = selectedNodePair();
	if (items == BGPair())
        return;

    drawLink(items.first, items.second);
}

void MainWindow::editDel()
{
	// get selected items
    QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
// 	scene->clearSelection();
	// delete the link of the items
// 	for each (QGraphicsItem* var in items)
// 	{
// 		IAbstractItem* item = dynamic_cast<IAbstractItem*>(var);
// 		item->addLink()
// 	}
	// delete items
	delItems(items);
	// record the changing
	setDirty(true);
}

void MainWindow::delItems(QList<QGraphicsItem *> items){
	QMutableListIterator<QGraphicsItem *> i(items);
	while (i.hasNext()) {
		BGLink *link = dynamic_cast<BGLink *>(i.next());
		if (link) {
			delete link;
			i.remove();
		}
	}

	qDeleteAll(items);
}

void MainWindow::editCut()
{
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	if (items.isEmpty())
		return;
	copyItems(items);
	QListIterator<QGraphicsItem*> i(items);
	while (i.hasNext()) {
		QScopedPointer<QGraphicsItem> item(i.next());
		_pGraphicScene->removeItem(item.data());
	}
	setDirty(true);
}

void MainWindow::editCopy()
{
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	if (items.isEmpty())
		return;

	QListIterator<QGraphicsItem*> i(items);
	while (i.hasNext()) {
		QGraphicsItemGroup* group = dynamic_cast<QGraphicsItemGroup*>(i.next());
		if (group)
		{
			for each (QGraphicsItem *child in group->childItems())
			{
				items.append(child);
			}
		}
	}

	copyItems(items);
}

void MainWindow::editPaste()
{
	QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();
	if (!mimeData)
		return;

	if (mimeData->hasFormat(MimeType)) {
		QByteArray copiedItems = mimeData->data(MimeType);
		QXmlStreamReader reader(copiedItems);
		while (!reader.atEnd()) {
			if (reader.isStartElement()) {
				if (reader.name() == "diagram") {
					m_bPaste = true;
					readDiagram(reader,true);
					m_bPaste = false;
				}
				else {
					reader.raiseError(QObject::tr("known format"));
				}
			}
			else
			{
				reader.readNext();
			}
		}
// 		readItems(in, pasteOffset, true);
// 		pasteOffset += OffsetIncrement;
	}
	else if (mimeData->hasHtml() || mimeData->hasText()) {
// 		TextItem *textItem = new TextItem(position(), scene);
// 		connectItem(textItem);
// 		if (mimeData->hasHtml())
// 			textItem->setHtml(mimeData->html());
// 		else
// 			textItem->setPlainText(mimeData->text());
	}
	else
		return;
	setDirty(true);
}

void MainWindow::bringToFront()
{
    ++maxZ;
    setZValue(maxZ);
}

void MainWindow::sendToBack()
{
    --minZ;
    setZValue(minZ);
}

void MainWindow::properties()
{
// 	IBasicGraphicItem *item = selectedNode();
//     Link *link = selectedLink();
// 
//     if (item) {
// 		BGRectBase* rect = dynamic_cast<BGRectBase *>(item);
// 		if (rect)
// 		{
// 			RectPropertiesDialog dialog(rect, this);
// 			dialog.exec();
// 		}
// 		else
// 		{
// 			PropertiesDialog dialog(item, this);
// 			dialog.exec();
// 		}
//     } else if (link) {
//         QColor color = QColorDialog::getColor(link->color(), this);
//         if (color.isValid())
//             link->setColor(color);
//     }
}

void MainWindow::newLayer()
{
	m_pCurrentLayer = new LayersWidget();
	connect(m_pCurrentLayer, SIGNAL(changeVisibility(LayersWidget*, bool))
		, this, SLOT(visibilityChanged(LayersWidget*, bool)));
	m_listLayers.append(m_pCurrentLayer);
	m_rightLayout->addWidget(m_pCurrentLayer);
}

void MainWindow::setZValue(int z)
{
	IBasicGraphicItem *item = selectedNode();
    if (item)
        item->setZValue(z);
}

void MainWindow::setNodePos(IBasicGraphicItem *item)
{
    item->setPos(QPoint(80 + (100 * (seqNumber % 5)),
                        80 + (50 * ((seqNumber / 5) % 7))));
    ++seqNumber;
}

IBasicGraphicItem *MainWindow::selectedNode() const
{
    QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
    if (items.count() == 1) {
		return dynamic_cast<IBasicGraphicItem *>(items.first());
    } else {
        return 0;
    }
}
IAbstractItem *MainWindow::selectedAbstractNode() const
{
    QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
    if (items.count() == 1) {
		return dynamic_cast<IAbstractItem *>(items.first());
    } else {
        return 0;
    }
}

BGGroup *MainWindow::selectedGroup() const{
	QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
	if (items.count() == 1) {
		return dynamic_cast<BGGroup *>(items.first());
	}
	else {
		return 0;
	}
}

BGLink *MainWindow::selectedLink() const
{
    QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
    if (items.count() == 1) {
		return dynamic_cast<BGLink *>(items.first());
    } else {
        return 0;
    }
}

MainWindow::BGPair MainWindow::selectedNodePair() const
{
    QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
    if (items.count() == 2) {
		IAbstractItem *first = dynamic_cast<IAbstractItem *>(items.first());
		IAbstractItem *second = dynamic_cast<IAbstractItem *>(items.last());
        if (first && second)
			return BGPair(first, second);
    }
	return BGPair();
}

void MainWindow::visibilityChanged(LayersWidget* panel, bool visible)
{
	int count = panel->m_pListWidget->count();
	for (int index = 0;
		index < count;
		index++)
	{
		QListWidgetItem * item = panel->m_pListWidget->item(index);
		setGraphicItemVisible(item->text(), visible);
		// A wild item has appeared
	}
}

void MainWindow::setGraphicItemVisible(QString strId, bool bVisible)
{
	for each (QGraphicsItem* item in _pGraphicScene->items())
	{
		IBasicGraphicItem* g = dynamic_cast<IBasicGraphicItem *>(item);
		if (g && g->CheckId(strId))
		{
			g->setVisible(bVisible);
			return;
		}
	}
}

void MainWindow::drawGraphicsItem(IAbstractItem* item)
{
	bringToFront();

	connect(item, SIGNAL(dirty()), this, SLOT(elementCallDirty()));
	setDirty(true);
	const QMetaObject *metaObject = item->metaObject();
	if (metaObject->indexOfProperty("brush") > -1)
		connect(brushWidget, SIGNAL(brushChanged(const QBrush&)),
		item, SLOT(setBrush(const QBrush&)));
	if (metaObject->indexOfProperty("pen") > -1)
		connect(penWidget, SIGNAL(penChanged(const QPen&)),
		item, SLOT(setPen(const QPen&)));
	if (metaObject->indexOfProperty("angle") > -1) {
		connect(transformWidget, SIGNAL(angleChanged(double)), item, SLOT(setAngle(double)));
		connect(transformWidget, SIGNAL(shearChanged(double, double)), item, SLOT(setShear(double, double)));
	}
	if (metaObject->indexOfProperty("width") > -1) {
		connect(geometryWidget, SIGNAL(updateGeometry(int, int)), item, SLOT(setGeometry(int, int)));
	}
	if (metaObject->indexOfProperty("x") > -1) {
		connect(locationWidget, SIGNAL(updateLocation(int, int)), item, SLOT(setLocation(int, int)));
	}
	if (metaObject->indexOfProperty("weight") > -1) {
		connect(linkPropertyWidget, SIGNAL(updateWeight(int)), item, SLOT(setWeight(int)));
	}
	if (metaObject->indexOfProperty("DataId") > -1) {
		connect(m_pWidgetData, SIGNAL(dataIdChanged(int)), item, SLOT(SetDataId(int)));
	}
	if (metaObject->indexOfProperty("fromIndex") > -1) {
		connect(groupWidget, SIGNAL(updateIndex(int, int)), item, SLOT(setLinkIndex(int, int)));
	}
	if (metaObject->indexOfProperty("angleStart") > -1) {
		connect(arcWidget, SIGNAL(angleStartChanged(double)), item, SLOT(setAngleStart(double)));
	}
	if (metaObject->indexOfProperty("angleLen") > -1) {
		connect(arcWidget, SIGNAL(angleLenChanged(double)), item, SLOT(setAngleLen(double)));
	}
	// add to layer
	m_pCurrentLayer->m_pListWidget->addItem(item->GetId());
}

BGLink* MainWindow::drawLink(IAbstractItem* from, IAbstractItem* to){
	BGLink *link = new BGLink(from, to);
	_pGraphicScene->addItem(link);
	connect(groupWidget, SIGNAL(updateIndex(int, int)), link, SLOT(setLinkIndex(int, int)));
	connect(linkPropertyWidget, SIGNAL(updateWeight(int)), link, SLOT(setWeight(int)));
	setDirty(true);



	return link;
}

void MainWindow::setupNode(IBasicGraphicItem *item)
{
	_pGraphicScene->addItem(item);

	_pGraphicScene->clearSelection();
	item->setSelected(true);

	drawGraphicsItem(item);
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
	QSize size = event->size();
	_pGraphicView->setSceneRect(0, 0, size.width(),size.height());
	_pGraphicScene->setSceneRect(0, 0, size.width(), size.height());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (okToClearData()) {
		QSettings settings;
		settings.setValue(ShowGrid, viewShowGridAction->isChecked());
		QString str = windowFilePath();
		settings.setValue(MostRecentFile, windowFilePath());
		event->accept();
	}
	else
		event->ignore();
}

bool MainWindow::okToClearData()
{
	if (isWindowModified())
		return AQP::okToClearData(&MainWindow::fileSave, this,tr("Unsaved changes"), tr("Save unsaved changes?"));
	return true;
}

void MainWindow::clear()
{
	QList<QGraphicsItem*> items = this->_pGraphicScene->items();
	if (!items.empty())
	{
		delItems(items);
	}
	for each (LayersWidget* layer in m_listLayers)
	{
		layer->m_pListWidget->clear();
	}
}

void MainWindow::paintScene(QPainter *painter)
{
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	_pGraphicScene->clearSelection();

	_pGraphicScene->render(painter);

	foreach(QGraphicsItem *item, items)
		item->setSelected(true);
	selectionChanged();
}

QByteArray MainWindow::items2Array(QList<QGraphicsItem*> items){
	QByteArray copiedItems;
	QXmlStreamWriter xmlWriter(&copiedItems);
	xmlWriter.writeStartElement("diagram");
	writeItems(xmlWriter, items);
	writeGroups(xmlWriter, items);
	xmlWriter.writeEndElement();
	return copiedItems;
}

void MainWindow::copyItems(const QList<QGraphicsItem*> &items)
{
	QByteArray copiedItems=items2Array(items);
// 	QXmlStreamWriter xmlWriter(&copiedItems);
// 	xmlWriter.writeStartElement("diagram");
// 	writeItems(xmlWriter, items);
// 	writeGroups(xmlWriter, items);
// 	xmlWriter.writeEndElement();

	QMimeData *mimeData = new QMimeData;
	mimeData->setData(MimeType, copiedItems);
	QClipboard *clipboard = QApplication::clipboard();
	clipboard->setMimeData(mimeData);
}

void MainWindow::populateMenuAndToolBar(QMenu *menu, QToolBar *toolBar, QList<QAction*> actions)
{
	foreach(QAction *action, actions) {
		if (!action) {
			menu->addSeparator();
			toolBar->addSeparator();
		}
		else {
			menu->addAction(action);
			toolBar->addAction(action);
		}
	}
}

void MainWindow::setDirty(bool on)
{
	if (on && !m_bOpening)
	{
		recordHistory();
	}
	setWindowModified(on);
	updateActions();
}

void MainWindow::recordHistory(){
	QByteArray history;
	QXmlStreamWriter xmlWriter(&history);
	saveFile(xmlWriter);
	m_arrHistory[m_nCurrentHistory] = history;
	m_nCurrentHistory = (m_nCurrentHistory + 1) % MAX_HISTORY_LENGTH;
	m_nFrontHistory = m_nCurrentHistory;
	if (m_nBackHistory == m_nCurrentHistory)
	{
		m_nBackHistory = (m_nBackHistory + 1) % MAX_HISTORY_LENGTH;
	}
}

void MainWindow::undo(){
	m_nCurrentHistory = (m_nCurrentHistory + MAX_HISTORY_LENGTH - 2) % MAX_HISTORY_LENGTH;
	QXmlStreamReader reader(m_arrHistory[m_nCurrentHistory]);
	loadFile(reader);
	updateActions();
	m_nCurrentHistory = (m_nCurrentHistory + 1) % MAX_HISTORY_LENGTH;
}

void MainWindow::redo(){
	QXmlStreamReader reader(m_arrHistory[m_nCurrentHistory]);
	loadFile(reader);
	updateActions();
	m_nCurrentHistory = (m_nCurrentHistory + 1) % MAX_HISTORY_LENGTH;
}

void MainWindow::editLinkStyle(){
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;
	BGLink* link = this->selectedLink();
	if (!link)
		return;
	BGLink::LinkStyle style = static_cast<BGLink::LinkStyle>(action->data().toInt());
	
	if (link->ChangeStyle(style))
	{		
		update();
		updateActions();
	}

}

void MainWindow::selectionChanged()
{
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	if (items.count() == 1) {
		if (QObject *item = dynamic_cast<QObject*>(items.at(0))) {
			if (item->property("brush").isValid())
				brushWidget->setBrush(
				item->property("brush").value<QBrush>());
			if (item->property("pen").isValid())
				penWidget->setPen(
				item->property("pen").value<QPen>());
			if (item->property("angle").isValid()) {
				transformWidget->setAngle(
					item->property("angle").toDouble());
				transformWidget->setShear(
					item->property("shearHorizontal").toDouble(),
					item->property("shearVertical").toDouble());
			}
			if (item->property("width").isValid()) {
				geometryWidget->setW(item->property("width").toInt());
				geometryWidget->setH(item->property("height").toInt());
			}
			if (item->property("x").isValid())
			{
				locationWidget->setX(item->property("x").toInt());
				locationWidget->setY(item->property("y").toInt());
			}
			if (item->property("weight").isValid())
			{
				linkPropertyWidget->SetWeight(item->property("weight").toInt());
			}
			if (item->property("DataId").isValid())
			{
				m_pWidgetData->SetDataId(item->property("DataId").toInt());
			}
			if (item->property("fromIndex").isValid()) {
				groupWidget->setFromIndexRange(item->property("fromRange").toInt());
				groupWidget->setToIndexRange(item->property("toRange").toInt());
				groupWidget->setFromIndex(item->property("fromIndex").toInt());
				groupWidget->setToIndex(item->property("toIndex").toInt());
			}
			if (item->property("angleStart").isValid()) {
				arcWidget->setAngleStart(item->property("angleStart").toDouble());
			}
			if (item->property("angleLen").isValid()) {
				arcWidget->setAngleLen(item->property("angleLen").toDouble());
			}
		}
	}
	updateActions();
}

void MainWindow::getSelectionProperties(bool *hasBrushProperty, bool *hasPenProperty) const
{
	Q_ASSERT(hasBrushProperty && hasPenProperty);
	*hasBrushProperty = false;
	*hasPenProperty = false;
	foreach(QGraphicsItem *item, _pGraphicScene->selectedItems()) {
		if (QObject *object = dynamic_cast<QObject*>(item)) {
			const QMetaObject *metaObject = object->metaObject();
			if (metaObject->indexOfProperty("brush") > -1)
				*hasBrushProperty = true;
			if (metaObject->indexOfProperty("pen") > -1)
				*hasPenProperty = true;
			if (*hasBrushProperty && *hasPenProperty)
				break;
		}
	}
}

void MainWindow::editClearTransforms()
{
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	Q_ASSERT(!items.isEmpty());
	QListIterator<QGraphicsItem*> i(items);
	while (i.hasNext()) {
		if (QObject *item = dynamic_cast<QObject*>(i.next())) {
			if (item->property("angle").isValid()) {
				item->setProperty("angle", 0.0);
				item->setProperty("shearHorizontal", 0.0);
				item->setProperty("shearVertical", 0.0);
			}
		}
	}
	transformWidget->setAngle(0.0);
	transformWidget->setShear(0.0, 0.0);
	setDirty(true);
}

void MainWindow::editCombine()
{
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	_pGraphicScene->clearSelection();
	QList<QGraphicsItem*> nodes;
	for each (QGraphicsItem* item in items)
	{
		if (dynamic_cast<BGLink*>(item)==NULL)
		{
			nodes.append(item);
		}
	}
	BGGroup* group = new BGGroup();
	group->SetUpGroup(nodes);

// 	connect(group, SIGNAL(dirty()), this, SLOT(setDirty()));
	_pGraphicScene->addItem(group);
	drawGraphicsItem(group);
}

void MainWindow::editDetach()
{

	QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
	if (items.count() == 1) {
		BGGroup* group = dynamic_cast<BGGroup *>(items.first());
		group->RemoveLinks();
		_pGraphicScene->destroyItemGroup(group);
	}
}

void MainWindow::connectItemsAndGroups(bool bNewId){
	_pGraphicScene->clearSelection();
	// add item to group
	for (QMap<IAbstractItem*, QUuid>::const_iterator ite = m_mapItemsInGroup.constBegin();
		ite != m_mapItemsInGroup.constEnd(); ite++)
	{
		if (m_mapGroup.contains(ite.value()))
		{
			m_mapGroup[ite.value()]->AddItem(dynamic_cast<QGraphicsItem*>(ite.key()));
		}
// 		else{
// 			int w = 0;
// 		}
	}

	// add group to group, and initialize group
	for (QMap<QUuid, BGGroup*>::const_iterator ite = m_mapGroup.constBegin();
		ite != m_mapGroup.constEnd(); ite++)
	{
		if (!m_mapGroup.contains(ite.value()->GetGroupId())){
			BGGroup* group = ite.value();
			initializeGroup(group, bNewId);
		}
	}
	m_mapGroup.clear();
	m_mapItemsInGroup.clear();

	// link
	if (!m_listReadedLinks.empty())
	{
		for each (LinkInfo info in m_listReadedLinks)
		{
			IAbstractItem* from = getNodeById(info.from);
			IAbstractItem* to = getNodeById(info.to);
			if (from && to)
			{
				BGLink* link = drawLink(from, to);
// 				link->InitPorts(info.fromPort, info.toPort);
				link->ChangeStyle((BGLink::LinkStyle)info.style);
				link->ChangeWeight(info.weight);
			}
		}
		m_listReadedLinks.clear();
	}
}

void MainWindow::initializeGroup(BGGroup* group,bool bNewId){
	if (bNewId)
	{
		group->GenerateNewId();
	}
	for (QMap<QUuid, BGGroup*>::const_iterator ite = m_mapGroup.constBegin();
		ite != m_mapGroup.constEnd(); ite++)
	{
		if (ite.value()->GetGroupId()==group->GetId())
		{
			BGGroup* childGroup = ite.value();
			initializeGroup(childGroup, bNewId);
			group->AddItem(childGroup);
		}
	}
	group->InitPosition();

	if (group->GetGroupId() == QUuid())
	{
		_pGraphicScene->addItem(group);
		drawGraphicsItem(group);
	}
}

bool MainWindow::selectedGroup()
{
	QList<QGraphicsItem *> items = _pGraphicScene->selectedItems();
	if (items.count() == 1) {
		return NULL!=(dynamic_cast<BGGroup *>(items.first()));
	}
	return false;
}

IAbstractItem* MainWindow::getNodeById(QUuid id)
{
	for each (QGraphicsItem* item in _pGraphicScene->items())
	{
		IAbstractItem* g = dynamic_cast<IAbstractItem*>(item);
		if (g && g->CheckId(id))
		{
			return g;
		}
	}
	return NULL;
}

void MainWindow::onMousePressed(){
	m_bMousePressed = true;
}

void MainWindow::onMouseReleased(){
	if (m_bElementDragged)
	{
		m_bElementDragged = false;
		setDirty(true);
	}
	m_bMousePressed = false;
}

void MainWindow::elementCallDirty()
{
	IAbstractItem* node = selectedAbstractNode();
	if (node && node->property("x").isValid())
	{
		locationWidget->setX(node->property("x").toInt());
		locationWidget->setY(node->property("y").toInt());
		locationWidget->update();
	}

	m_bElementDragged = true;
}



bool sortByLeft(const QGraphicsItem* item1, const QGraphicsItem* item2){
	QRectF rect1 = item1->sceneBoundingRect();
	QRectF rect2 = item2->sceneBoundingRect();
	return rect1.x() < rect2.x();
}
bool sortByRight(const QGraphicsItem* item1, const QGraphicsItem* item2){
	QRectF rect1 = item1->sceneBoundingRect();
	QRectF rect2 = item2->sceneBoundingRect();
	return rect1.x() + rect1.width()> rect2.x() + rect2.width();
}
bool sortByTop(const QGraphicsItem* item1, const QGraphicsItem* item2){
	QRectF rect1 = item1->sceneBoundingRect();
	QRectF rect2 = item2->sceneBoundingRect();
	return rect1.y() < rect2.y();
}
bool sortByBottom(const QGraphicsItem* item1, const QGraphicsItem* item2){
	QRectF rect1 = item1->sceneBoundingRect();
	QRectF rect2 = item2->sceneBoundingRect();
	return rect1.y() + rect1.height()> rect2.y() + rect2.height();
}

/*
*/

struct LinkNode
{
	int _pres;
	QList<int> _subs;
};

struct Overlap
{
	int _index1;
	int _index2;
	double _distance;
};

bool sortByDistance(const Overlap& ol1, const Overlap& ol2){
	return ol1._distance < ol2._distance;
}

QList<QGraphicsItem*> MainWindow::getSelectedNodes(){
	QList<QGraphicsItem*> items = _pGraphicScene->selectedItems();
	QList<QGraphicsItem*> itemsNodes;
	for each (QGraphicsItem* item in items)
	{
		if (dynamic_cast<IAbstractItem*>(item))
		{
			itemsNodes.append(item);
		}
	}
	return itemsNodes;
}

// arrangement

void MainWindow::editAlign()
{
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;

	Qt::Alignment alignment = static_cast<Qt::Alignment>(
		action->data().toInt());

	// original approach, directly align the items
	doAlign(getSelectedNodes(), alignment);

	setDirty(true);
}

// improved approach, after reading the paper and understood incorrectly.
/*
void MainWindow::editAlign()
{
QAction *action = qobject_cast<QAction*>(sender());
if (!action)
return;

Qt::Alignment alignment = static_cast<Qt::Alignment>(
action->data().toInt());
if (action != editAlignmentAction) {
editAlignmentAction->setData(action->data());
editAlignmentAction->setIcon(action->icon());
}
QList<QGraphicsItem*> items = scene->selectedItems();

switch (alignment)
{
case Qt::AlignLeft:
qSort(items.begin(), items.end(), sortByLeft);
break;
case Qt::AlignRight:
qSort(items.begin(), items.end(), sortByRight);
break;
case Qt::AlignTop:
qSort(items.begin(), items.end(), sortByTop);
break;
case Qt::AlignBottom:
qSort(items.begin(), items.end(), sortByBottom);
break;
}
int size = items.size();
QList<LinkNode> nodeList;
for (int i = 0; i < size; i++)
{
LinkNode node;
node._pres = 0;
nodeList.append(node);
}
for (int i = 0; i < size;i++)
{
QRectF rect1 = items[i]->sceneBoundingRect();
for (int j = i+1; j < size;j++)
{
QRectF rect2 = items[j]->sceneBoundingRect();

switch (alignment)
{
case Qt::AlignLeft:
case Qt::AlignRight:
if (rect1.y() < rect2.y() + rect2.height() && rect2.y() < rect1.y() + rect1.height())
{
nodeList[i]._subs.append(j);
nodeList[j]._pres++;
}
break;
case Qt::AlignTop:
case Qt::AlignBottom:
if (rect1.x() < rect2.x() + rect2.width() && rect2.x() < rect1.x() + rect1.width())
{
nodeList[i]._subs.append(j);
nodeList[j]._pres++;
}
break;
}
}
}
int nleaving = size;
while (nleaving>0)
{
QList<int> listIndex;
for (int i = 0; i < size;i++)
{
if (nodeList[i]._pres==0)
{
listIndex.append(i);
nodeList[i]._pres = -1;
}
}
QList<QGraphicsItem*> tempList;
for each (int index in listIndex)
{
tempList.append(items[index]);
for each (int subIndex in nodeList[index]._subs)
{
nodeList[subIndex]._pres--;
}
}
nleaving -= tempList.size();
doAlign(tempList, alignment);
}

setDirty(true);
}
*/

// method of the paper
// basic idea:
// 1. construct the raw group, regardless the overlap, put them into doAlignGroup.
// 2. in doAlignGroup, check overlap. if find overlaps, find the nearest overlapped element
//    and partition the group between them, then call doAlignGroup recursively.
// 3. if there's no overlap, just align them.
void MainWindow::editGroupAlign()
{
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;

	Qt::Alignment alignment = static_cast<Qt::Alignment>(
		action->data().toInt());
// 	if (action != editAlignmentAction) {
// 		editAlignmentAction->setData(action->data());
// 		editAlignmentAction->setIcon(action->icon());
// 	}
	QList<QGraphicsItem*> items = getSelectedNodes();

	// 1.graph construction
// 	switch (alignment)
// 	{
// 	case Qt::AlignLeft:
// 	case Qt::AlignRight:
// 		qSort(items.begin(), items.end(), sortByTop);
// 		break;
// 	case Qt::AlignTop:
// 	case Qt::AlignBottom:
// 		qSort(items.begin(), items.end(), sortByLeft);
// 		break;
// 	}
	int size = items.size();
	QList<LinkNode> nodeList;
	for (int i = 0; i < size; i++)
	{
		LinkNode node;
		node._pres = 0;
		nodeList.append(node);
	}
	for (int i = 0; i < size; i++)
	{
		QRectF rect1 = items[i]->sceneBoundingRect();
		for (int j = i + 1; j < size; j++)
		{
			QRectF rect2 = items[j]->sceneBoundingRect();

			switch (alignment)
			{
			case Qt::AlignLeft:
			case Qt::AlignHCenter:
			case Qt::AlignRight:
				if (rect1.x() < rect2.x() + rect2.width() && rect2.x() < rect1.x() + rect1.width())
				{
					nodeList[i]._subs.append(j);
					nodeList[j]._pres++;
					nodeList[j]._subs.append(i);
					nodeList[i]._pres++;
				}
				break;
			case Qt::AlignTop:
			case Qt::AlignVCenter:
			case Qt::AlignBottom:
				if (rect1.y() < rect2.y() + rect2.height() && rect2.y() < rect1.y() + rect1.height())
				{
					nodeList[i]._subs.append(j);
					nodeList[j]._pres++;
					nodeList[j]._subs.append(i);
					nodeList[i]._pres++;
				}
				break;
			}
		}
	}
	// put each raw group into doAlignGroup() to do graph partition and align
	int nleaving = size;
	QQueue<int> queue;
	QList<int> listCollected;
	listCollected.append(0);
	queue.enqueue(0);
	nodeList[0]._pres = -1;
	while (!queue.empty())
	{
		QList<QGraphicsItem*> group;
		while (!queue.empty())
		{
			int nIndex = queue.dequeue();
			group.append(items[nIndex]);
			for each (int sub in nodeList[nIndex]._subs)
			{
				if (!listCollected.contains(sub))
				{
					listCollected.append(sub);
					queue.enqueue(sub);
					nodeList[sub]._pres = -1;
				}
			}
		}
		doAlignGroup(group, alignment);
		for (int i = 0; i < size; i++)
		{
			if (nodeList[i]._pres != -1)
			{
				listCollected.append(i);
				queue.enqueue(i);
				nodeList[i]._pres = -1;
				break;
			}
		}
	}

	setDirty(true);
}


void MainWindow::editDistribute(){
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;

	Qt::Alignment alignment = static_cast<Qt::Alignment>(action->data().toInt());

	doDistribute(getSelectedNodes(), alignment);
}

// check if target is adjacent with pos
bool checkAdjacent(const QList<LinkNode>& nodeList, int pos,int target){
	if (nodeList[pos]._subs.contains(target))
	{
		return false;
	}
	for each (int i in nodeList[pos]._subs)
	{
		if (i > target&&i < pos)
		{
			if (!checkAdjacent(nodeList,i,target))
			{
				return false;
			}
		}

	}
	return true;
}

// void MainWindow::editVDistribute(){
// 
// }
void MainWindow::editGroupDistribute(){
	QAction *action = qobject_cast<QAction*>(sender());
	if (!action)
		return;

	Qt::Alignment alignment = static_cast<Qt::Alignment>(action->data().toInt());
	QList<QGraphicsItem*> items = getSelectedNodes();
	// 1.graph construction
	switch (alignment)
	{
	case Qt::AlignHCenter:
		qSort(items.begin(), items.end(), sortByLeft);
		break;
	case Qt::AlignVCenter:
		qSort(items.begin(), items.end(), sortByTop);
		break;
	}
	int size = items.size();
	QList<LinkNode> nodeList;
	for (int i = 0; i < size; i++)
	{
		LinkNode node;
		node._pres = 0;
		nodeList.append(node);
	}
	for (int i = 1; i < size; i++)
	{
		QRectF rect1 = items[i]->sceneBoundingRect();
		for (int j = i - 1; j >= 0; j--)
		{
// 			bool bAdjacent = true;
// 			for each (int k in nodeList[i]._subs)
// 			{
// 				if (nodeList[k]._subs.contains(j))
// 				{
// 					bAdjacent = false;
// 					break;
// 				}
// 			}
			bool bAdjacent = checkAdjacent(nodeList, i, j);
			if (bAdjacent)
			{
				QRectF rect2 = items[j]->sceneBoundingRect();

				switch (alignment)
				{
				case Qt::AlignHCenter:
					if (rect1.y() < rect2.y() + rect2.height() && rect2.y() < rect1.y() + rect1.height())
					{
						nodeList[i]._subs.append(j);
						nodeList[j]._pres++;
						nodeList[j]._subs.append(i);
						nodeList[i]._pres++;
					}
					break;
				case Qt::AlignVCenter:
					if (rect1.x() < rect2.x() + rect2.width() && rect2.x() < rect1.x() + rect1.width())
					{
						nodeList[i]._subs.append(j);
						nodeList[j]._pres++;
						nodeList[j]._subs.append(i);
						nodeList[i]._pres++;
					}
					break;
				}
			}
		}
	}
	// 2.graph has been built, now pick the groups
	int current = 0;
	while (current < size)
	{
		QList<QGraphicsItem*> group;
		group.append(items[current]);
		while (nodeList[current]._pres>0)
		{
			int next = -1;
			for each (int sub in  nodeList[current]._subs)
			{
				if (sub>current)
				{
					next = sub;
					break;
				}
			}
			if (next==-1)
			{
				break;
			}
			else{
				group.append(items[next]);
				nodeList[current]._subs.removeOne(next);
				nodeList[current]._pres--;
				nodeList[next]._subs.removeOne(current);
				nodeList[next]._pres--;
				if (nodeList[next]._pres==1)
				{
					current = next;
				}
				else{
					break;
				}
			}
		}
		doDistribute(group, alignment);
		current = 0;
		while (current < size&&nodeList[current]._pres == 0)current++;
	}
}
// void MainWindow::editGroupVDistribute(){
// 
// }

// align the groups deprived from step 1
void MainWindow::doAlignGroup(QList<QGraphicsItem*> items, Qt::Alignment alignment){
	// graph partition
	QList<Overlap> listOverlaps;
	int size = items.size();
	for (int i = 0; i < size; i++)
	{
		QRectF rect1 = items[i]->sceneBoundingRect();
		for (int j = i + 1; j < size; j++)
		{
			QRectF rect2 = items[j]->sceneBoundingRect();

			switch (alignment)
			{
			case Qt::AlignLeft:
				if (rect1.y() < rect2.y() + rect2.height() && rect2.y() < rect1.y() + rect1.height())
				{
					listOverlaps.append(Overlap{ i, j, abs(rect1.x() - rect2.x()) });
				}
				break;
			case Qt::AlignHCenter:
				if (rect1.y() < rect2.y() + rect2.height() && rect2.y() < rect1.y() + rect1.height())
				{
					listOverlaps.append(Overlap{ i, j, abs(rect1.x() + rect1.width() / 2.0 - rect2.x() - rect2.width() / 2.0) });
				}
				break;
			case Qt::AlignRight:
				if (rect1.y() < rect2.y() + rect2.height() && rect2.y() < rect1.y() + rect1.height())
				{
					listOverlaps.append(Overlap{ i, j, abs(rect1.x() + rect1.width() - rect2.x() - rect2.width()) });
				}
				break;
			case Qt::AlignTop:
				if (rect1.x() < rect2.x() + rect2.width() && rect2.x() < rect1.x() + rect1.width())
				{
					listOverlaps.append(Overlap{ i, j, abs(rect1.y() - rect2.y()) });
				}
				break;
			case Qt::AlignVCenter:
				if (rect1.x() < rect2.x() + rect2.width() && rect2.x() < rect1.x() + rect1.width())
				{
					listOverlaps.append(Overlap{ i, j, abs(rect1.y() + rect1.height() / 2.0 - rect2.y() - rect2.height() / 2.0) });
				}
				break;
			case Qt::AlignBottom:
				if (rect1.x() < rect2.x() + rect2.width() && rect2.x() < rect1.x() + rect1.width())
				{
					listOverlaps.append(Overlap{ i, j, abs(rect1.y() + rect1.height() - rect2.y() - rect2.height()) });
				}
				break;
			}
		}
	}
	if (listOverlaps.empty())
	{
		doAlign(items, alignment);
	}
	else{
		qSort(listOverlaps.begin(), listOverlaps.end(), sortByDistance);
		QList<QGraphicsItem*> list1;
		QList<QGraphicsItem*> list2;
		QGraphicsItem* item1 = items[listOverlaps[0]._index1];
		QGraphicsItem* item2 = items[listOverlaps[0]._index2];
		list1.append(item1);
		list2.append(item2);
		items.removeOne(item1);
		items.removeOne(item2);
		QRectF rect1 = item1->sceneBoundingRect();
		QRectF rect2 = item2->sceneBoundingRect();
		QMutableListIterator<QGraphicsItem *> i(items);
		while (i.hasNext()) {
			QGraphicsItem* item = i.next();
			QRectF rect = item->sceneBoundingRect();
			switch (alignment)
			{
			case Qt::AlignLeft:
				if (abs(rect1.x() - rect.x())<abs(rect2.x() - rect.x()))
				{
					list1.append(item);
				}
				else{
					list2.append(item);
				}
				break;
			case Qt::AlignHCenter:
				if (abs(rect1.x() + rect1.width() / 2.0 - rect.x() - rect.width() / 2.0) 
					< abs(rect2.x() + rect2.width() / 2.0 - rect.x() - rect.width() / 2.0))
				{
					list1.append(item);
				}
				else{
					list2.append(item);
				}
				break;
			case Qt::AlignRight:
				if (abs(rect1.x() + rect1.width() - rect.x() - rect.width()) < abs(rect2.x() + rect2.width() - rect.x() - rect.width()))
				{
					list1.append(item);
				}
				else{
					list2.append(item);
				}
				break;
			case Qt::AlignTop:
				if (abs(rect1.y() - rect.y()) < abs(rect2.y() - rect.y()))
				{
					list1.append(item);
				}
				else{
					list2.append(item);
				}
				break;
			case Qt::AlignVCenter:
				if (abs(rect1.y() + rect1.height() / 2.0 - rect.y() - rect.height() / 2.0)
					< abs(rect2.y() + rect2.height() / 2.0 - rect.y() - rect.height() / 2.0))
				{
					list1.append(item);
				}
				else{
					list2.append(item);
				}
				break;
			case Qt::AlignBottom:
				if (abs(rect1.y() + rect1.height() - rect.y() - rect.height()) < abs(rect2.y() + rect2.height() - rect.y() - rect.height()))
				{
					list1.append(item);
				}
				else{
					list2.append(item);
				}
				break;
			}
			i.remove();
		}
		doAlignGroup(list1, alignment);
		doAlignGroup(list2, alignment);
	}
}

// directly align these items
void MainWindow::doAlign(QList<QGraphicsItem*> items, Qt::Alignment alignment){

	QVector<double> coordinates;
	populateCoordinates(alignment, &coordinates, items);
	double offset;
	if (alignment == Qt::AlignLeft || alignment == Qt::AlignTop)
		offset = min(coordinates);
	else if (alignment == Qt::AlignRight || alignment == Qt::AlignBottom)
		offset = max(coordinates);
	else
		offset = (max(coordinates) + min(coordinates))/2.0;

	if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight||alignment==Qt::AlignHCenter) {
		for (int i = 0; i < items.count(); ++i)
			items.at(i)->moveBy(offset - coordinates.at(i), 0);
	}
	else {
		for (int i = 0; i < items.count(); ++i)
			items.at(i)->moveBy(0, offset - coordinates.at(i));
	}
}
// directly distribute these items
void MainWindow::doDistribute(QList<QGraphicsItem*> items, Qt::Alignment alignment){

	int nSize = items.count();
	if (nSize < 2) return;
	if (alignment == Qt::AlignHCenter)
	{
		qSort(items.begin(), items.end(), sortByLeft);
	}
	else{
		qSort(items.begin(), items.end(), sortByTop);
	}

	QVector<double> coordinates;
	populateCoordinates(alignment, &coordinates, items);
	double space = (coordinates[nSize - 1] - coordinates[0]) / (nSize - 1);

	if (alignment == Qt::AlignHCenter)
	{
		for (int i = 1; i < nSize - 1; ++i)
			items.at(i)->moveBy(coordinates[0] + space*i - coordinates.at(i), 0);
	}
	else{
		for (int i = 1; i < nSize - 1; ++i)
			items.at(i)->moveBy(0, coordinates[0] + space*i - coordinates.at(i));
	}

}

void MainWindow::populateCoordinates(const Qt::Alignment &alignment
	, QVector<double> *coordinates
	, const QList<QGraphicsItem*> &items)
{
	QListIterator<QGraphicsItem*> i(items);
	while (i.hasNext()) {
		QRectF rect = i.next()->sceneBoundingRect();
		switch (alignment) {
		case Qt::AlignLeft:
			coordinates->append(rect.x()); break;
		case Qt::AlignHCenter:
			coordinates->append(rect.x() + rect.width()/2.0); break;
		case Qt::AlignRight:
			coordinates->append(rect.x() + rect.width()); break;
		case Qt::AlignTop:
			coordinates->append(rect.y()); break;
		case Qt::AlignVCenter:
			coordinates->append(rect.y() + rect.height()/2.0); break;
		case Qt::AlignBottom:
			coordinates->append(rect.y() + rect.height()); break;
		
		}
	}
}

void MainWindow::slotEditEdgeCompress(){
	// 1.get nodes and links
	QList<IAbstractItem*> nodes;
	QList<BGLink*> links;
	for each (QGraphicsItem* item in _pGraphicScene->items())
	{
		if (item->parentItem())
		{
			continue;
		}
		IAbstractItem* node = dynamic_cast<IAbstractItem*>(item);
		if (node)
		{			
			nodes.append(node);
		}
		else{
			BGLink* link = dynamic_cast<BGLink*>(item);
			if (link)
			{
				links.append(link);
			}
		}
	}
	int nLen = nodes.length();
	if (nLen > 50)
	{
		nLen = 50;
	}
	// 2.calculate array
	int arr[50][50];
	for (int i = 0; i < nLen; i++)
	{
		for (int j = 0; j < nLen; j++)
		{
			arr[i][j] = 0;
		}
	}
	for each (BGLink* link in links)
	{
		int p1 = nodes.indexOf(link->fromNode());
		int p2 = nodes.indexOf(link->toNode());
		arr[p1][p2] = 1;
	}
	// 3. matching neighbors based on the array
	matchingNeighbors(nodes, arr);

// 	modularDecompose(nodes, arr);
}

void MainWindow::matchingNeighbors(QList<IAbstractItem*> nodes, int arr[50][50]){
	int nLen = nodes.length();
	QList<QList<int> > listIndices;
	QList<int> groupedIndices;
	for (int i = 0; i < nLen; i++)
	{
		if (groupedIndices.contains(i))
		{
			continue;
		}
		QList<int> indices;
		indices.append(i);
		for (int j = i + 1; j < nLen; j++)
		{
			if (groupedIndices.contains(j))
			{
				continue;
			}
			bool bSame = true;
			for (int k = 0; k < nLen; k++)
			{
				if (arr[i][k] != arr[j][k] || arr[k][i] != arr[k][j])
				{
					bSame = false;
					break;
				}
			}
			if (bSame)
			{
				indices.append(j);
			}
		}
		if (indices.length() > 1)
		{
			listIndices.append(indices);
			groupedIndices.append(indices);
		}
	}
	for each (QList<int> indices in listIndices)
	{
		QList<IAbstractItem*> compressedNodes;
		for each (int index in indices)
		{
			compressedNodes.append(nodes[index]);
		}
		compress(compressedNodes);
	}
}

void MainWindow::modularDecompose(QList<IAbstractItem*> nodes, int arr[50][50]){
	int nLen = nodes.length();

// 	for (int i=0)
// 	{
// 	}
	QList<QList<int> > listIndices;
	QList<int> groupedIndices;
	for (int i = 0; i < nLen; i++)
	{
		if (groupedIndices.contains(i))
		{
			continue;
		}
		QList<int> indices;
		indices.append(i);
		for (int j = i + 1; j < nLen; j++)
		{
			if (groupedIndices.contains(j))
			{
				continue;
			}
			bool bSame = true;
			for (int k = 0; k < nLen; k++)
			{
				if (arr[i][k] != arr[j][k] || arr[k][i] != arr[k][j])
				{
					bSame = false;
					break;
				}
			}
			if (bSame)
			{
				indices.append(j);
			}
		}
		if (indices.length() > 1)
		{
			listIndices.append(indices);
			groupedIndices.append(indices);
		}
	}
	for each (QList<int> indices in listIndices)
	{
		QList<IAbstractItem*> compressedNodes;
		for each (int index in indices)
		{
			compressedNodes.append(nodes[index]);
		}
		compress(compressedNodes);
	}
}

// compress items into a single group
void MainWindow::compress(QList<IAbstractItem*> items)
{
	BGGroup* group = new BGGroup();
	QList<QGraphicsItem*> gItems;
	bool bFirst = true;
	for each (IAbstractItem* item in items)
	{
		if (bFirst)
		{
			item->MoveLinkTo(group);
		}
		else{
			item->RemoveLinks();
		}
		gItems.append(dynamic_cast<QGraphicsItem*>(item));
	}
	group->SetUpGroup(gItems);

	// 	connect(group, SIGNAL(dirty()), this, SLOT(setDirty()));
	_pGraphicScene->addItem(group);
	drawGraphicsItem(group);
	group->updateLink();

}