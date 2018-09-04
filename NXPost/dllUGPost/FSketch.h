#pragma once

/** UG Post's header files **/
#include "dllUGPost.h"
#include "UGReferenceManager.h"
#include "Feature.h"

#include <vector>

#include <NXOpen\Point.hxx>

class Part;
class SKETCHItem;

class FSketch : public Feature
{
public:
	FSketch(Part * pPart, TransCAD::IFeaturePtr spFeature);
	~FSketch(void);
	void Clear();

	/**  �ɹ����� ���� �Լ� **/
	SKETCHItem * GetSketchItem(int index) { return _sketchItemList.at(index); }
	size_t GetSketchItemsSize() const { return _sketchItemList.size(); }
	Direct3D GetSketchNormal() { return _sketNormal; }
	string GetRefName() { return _refName; }

	virtual void GetInfo();
	virtual void ToUG();

	SKETCHItem * CreateSketchItem(TransCAD::IStdSketchGeometryPtr spItem);

	string GetFaceJID(Point3D origin, Vector3D normal);
	double polygonEdge(std::vector<NXOpen::Point3d> spList, std::vector<NXOpen::Point3d> epList, Point3D origin);
	void circularEdge();
	string GetSketchName() { return _sketchName; }

public:
	Part * _Part;
	TransCAD::IReferencePtr	_spReference;				// ����ġ ���۷����� ������
	TransCAD::IStdSketchGeometriesPtr _spGeometries;	// ����ġ�� ���� ��ü ��ü�� ������

	vector<SKETCHItem *> _sketchItemList;	// List of sketch items
	double _CSYS[9];	// 0~2 : Xdir, 3~5 : Ydir, 6~8 : Origin
	string	_refName;

	/**
	 * enumerator to indicate type of reference
	 * Feature = 0,
     * ExplicitModelObject = 1,
     * Brep = 2
	**/
	int		_refType;
	Direct3D _sketNormal;
	string _sketchName;		// Sketch class�� JID != sketch feature JID
};
