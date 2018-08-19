#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H

#include <QMainWindow>
#include <QPair>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QUuid>
#include <QMap>

#include <QSplitter>

class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsItemGroup;
class QToolBox;


class IAbstractItem;
class MyGraphicsView;
class MyVisView;
class BGLink;
class IBasicGraphicItem;
class LayersWidget;
class QVBoxLayout;
// class BGSphere;

class BrushWidget;
class PenWidget;
class TransformWidget;
class GeometryWidget;
class LocationWidget;
class ProbabilityWidget;
class LinkPropertyWidget;
class ArcWidget;
class BGGroup;
class GroupWidget;
class DataWidget;

#define MAX_HISTORY_LENGTH 10

class MainWindow : public QMainWindow
{
    Q_OBJECT

	struct LinkInfo 
	{
		QUuid from;
		QUuid to;
		QUuid fromPort;
		QUuid toPort;
		int fromPos;
		int toPos;
		int style;
		int weight;
	};
public:
    MainWindow();
private:
	void createSceneAndView();
	void createActions();
	void createDockWidgets();
	void createConnections();
	void createMenus();
	void createToolBars();
	void populateMenusAndToolBars();

private slots:
	void exportSVG();
	void setBackground();
	void addLink();
	void editAddItem();
    void editDel();
    void editCut();
	void editCopy();
    void editPaste();
    void bringToFront();
    void sendToBack();
    void properties();
	void updateActions();
	void editAlign();
	void slotEditEdgeCompress();
	void editGroupAlign();
	void editDistribute();
	void editGroupDistribute();
// 	void editHDistribute();
// 	void editVDistribute();
// 	void editGroupHDistribute();
// 	void editGroupVDistribute();
	void editClearTransforms();
	void undo();
	void redo();
	void editLinkStyle();

	void fileNew();
	void fileOpen();
	bool fileSave();
	bool fileSaveAs();
	void loadFile();

	void newLayer();

	void visibilityChanged(LayersWidget* panel,bool visible);

	void drawGraphicsItem(IAbstractItem* item);
	BGLink* drawLink(IAbstractItem* from, IAbstractItem* to);

	void selectionChanged();

	void editCombine();
	void editDetach();

	// wait mouse event from the view
	void onMousePressed();
	void onMouseReleased();
	void elementCallDirty();

private:
	typedef QPair<IAbstractItem *, IAbstractItem *> BGPair;

    void setZValue(int z);
    void setupNode(IBasicGraphicItem *node);
	void setNodePos(IBasicGraphicItem *node);
	IBasicGraphicItem *selectedNode() const;
	IAbstractItem *selectedAbstractNode() const;
	BGGroup *selectedGroup() const;
    BGLink *selectedLink() const;
	BGPair selectedNodePair() const;

	// file
	QAction *fileNewAction;
	QAction *fileOpenAction;
	QAction *fileSaveAction;
	QAction *fileSaveAsAction;
	QAction *fileExportAction;
	QAction *filePrintAction;
	QAction *fileQuitAction;
	QAction *exportSVGAction;
	QAction *setBackgroundAction;
	// edit
	QMenu* editMenu;
	QAction *addSphereAction;
	QAction *addRectAction;
	QAction *addLineAction;
	QAction *addTextAction;
	QAction *addArcAction;
	QAction *addSwitchAction;
	QAction *addNodeAction;
    QAction *addLinkAction;
    QAction *editDeleteAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *bringToFrontAction;
    QAction *sendToBackAction;
	QAction *propertiesAction;
	QAction *editUndoAction;
	QAction *editRedoAction;

	QAction *editClearTransformsAction;
	QAction *editCombineAction;
	QAction *editDetachAction;
	QAction *editEdgeCompress;

	QAction *editLinkStyleAction;
	QAction *editLinkStyleLineAction;
	QAction *editLinkStyleCurveAction;

	// arrange
	QMenu* arrangeMenu;
	QAction *editAlignLeftAction;
	QAction *editAlignHCenterAction;
	QAction *editAlignRightAction;
	QAction *editAlignTopAction;
	QAction *editAlignVCenterAction;
	QAction *editAlignBottomAction;
	QAction *editAlignHDistributeAction;
	QAction *editAlignVDistributeAction;

	QAction *editGroupAlignLeftAction;
	QAction *editGroupAlignHCenterAction;
	QAction *editGroupAlignRightAction;
	QAction *editGroupAlignTopAction;
	QAction *editGroupAlignVCenterAction;
	QAction *editGroupAlignBottomAction;
	QAction *editGroupAlignHDistributeAction;
	QAction *editGroupAlignVDistributeAction;
	// view
	QAction *viewZoomInAction;
	QAction *viewZoomOutAction;
	QAction *viewShowGridAction;
	// layer
// 	QAction *newLayerAction;
	// state
	QAction *drawSphereAction;
	QAction *drawRectAction;
	QAction *drawLineAction;
	QAction *drawLinkAction;
	QAction *drawPolygonAction;
	QAction *stateSelectAction;

    QGraphicsScene *_pGraphicScene;
    MyGraphicsView *_pGraphicView;

	QGraphicsScene *_pVisScene;
	MyVisView *_pVisView;

	QSplitter* _pSpliter;

    int minZ;
    int maxZ;
    int seqNumber;
	// layer list
	QList<LayersWidget* > m_listLayers;
	LayersWidget* m_pCurrentLayer;
	TransformWidget *transformWidget;
	GeometryWidget *geometryWidget;
	LocationWidget *locationWidget;
	ProbabilityWidget *probabilityWidget;
	LinkPropertyWidget *linkPropertyWidget;
	DataWidget *m_pWidgetData;
	ArcWidget* arcWidget;
	GroupWidget* groupWidget;
	BrushWidget *brushWidget;
	PenWidget *penWidget;
	QVBoxLayout* m_rightLayout;



	QToolBox *toolBox;
private:
	void setGraphicItemVisible(QString strId, bool bVisible);
	void writeItems(QXmlStreamWriter& writer, const QList<QGraphicsItem*> &items);
	void writeGroups(QXmlStreamWriter& writer, const QList<QGraphicsItem*> &items);
	void writeLinks(QXmlStreamWriter& writer);
	void copyItems(const QList<QGraphicsItem*> &items);
	void readDiagram(QXmlStreamReader& reader, bool bNewId = false);
	void readItems(QXmlStreamReader& reader, bool bNewId = false);
	void readLinks(QXmlStreamReader& reader);
	void readLink(QXmlStreamReader& reader);
	void readGroups(QXmlStreamReader& reader);
	void readGroup(QXmlStreamReader& reader);
	void readSphere(QXmlStreamReader& reader);
	void readRect(QXmlStreamReader& reader);
	bool okToClearData();
	void clear();
	void paintScene(QPainter *painter);  
	void populateMenuAndToolBar(QMenu *menu, QToolBar *toolBar, QList<QAction*> actions);
	void populateCoordinates(const Qt::Alignment &alignment, QVector<double> *coordinates, 
		const QList<QGraphicsItem*> &items);
	void getSelectionProperties(bool *hasBrushProperty,	bool *hasPenProperty) const;
	void connectItemsAndGroups(bool bNewId = false);
	void checkGroup(IAbstractItem* item);
	bool selectedGroup();
	void setDirty(bool on = true);
private:
	// the two map used for connect the items and their group while loading or paste
	QMap<QUuid, BGGroup*> m_mapGroup;
	QMap<IAbstractItem*, QUuid> m_mapItemsInGroup;
	QList<LinkInfo> m_listReadedLinks;
	bool m_bPaste;	// this variable is used to indicate the current reading operation is for paste but not loading
	QByteArray m_arrHistory[MAX_HISTORY_LENGTH];
	int m_nCurrentHistory;
	int m_nFrontHistory;
	int m_nBackHistory;
	bool m_bOpening;
	// for dragging
	bool m_bMousePressed;
	bool m_bElementDragged;
protected:
	virtual void resizeEvent(QResizeEvent * event);  
	virtual void closeEvent(QCloseEvent *event);
	IAbstractItem* getNodeById(QUuid id);
private:
	void delItems(QList<QGraphicsItem *> items);
private:
	void doAlign(QList<QGraphicsItem*> items,Qt::Alignment alignment);
	void doDistribute(QList<QGraphicsItem*> items, Qt::Alignment alignment);
	void doAlignGroup(QList<QGraphicsItem*> items, Qt::Alignment alignment);
	QByteArray items2Array(QList<QGraphicsItem*> items);
	void saveFile(QXmlStreamWriter& writer);
	void loadFile(QXmlStreamReader& reader);
	void recordHistory();
	void initializeGroup(BGGroup* group, bool bNewId);
	QList<QGraphicsItem*> getSelectedNodes();
	void compress(QList<IAbstractItem*> items);
	void matchingNeighbors(QList<IAbstractItem*> nodes,int arr[50][50]);
	void modularDecompose(QList<IAbstractItem*> nodes, int arr[50][50]);
};


#endif
