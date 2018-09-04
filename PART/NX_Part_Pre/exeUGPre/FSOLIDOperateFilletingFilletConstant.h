#pragma once

#include "Feature.h"

class Part;

class FSOLIDOperateFilletingFilletConstant :
	public Feature
{
public:

	FSOLIDOperateFilletingFilletConstant(Part * part, NXOpen::Features::Feature* ugfeature, NXOpen::Features::FeatureCollection * featureList);
	virtual ~FSOLIDOperateFilletingFilletConstant(void);

	/*double GetRadius() const {return _filRadius;}
	void GetStaPnt(Point3D & sP) {sP=_sP;}
	void GetEndPnt(Point3D & eP) {eP=_eP;}
	void GetPickPnt(Point3D & pP) {pP=_pP;}*/
	PropagationType GetPropType() {return _PropType;}

	virtual void GetUGInfo();
	virtual void ToTransCAD();

private :
	static int _fillCnt;		// Fillet Count
protected:
	int _numChainSet;
	vector<double>	_filRadius;
	vector<int>     _edgeNumPerChainSet;//������ chainSet(���� radius�� ������ edge����)�� ���� edge���� ���
	vector<Point3D> _sP;
	vector<Point3D> _eP;
	vector<Point3D> _pP;	// picking point

	PropagationType	_PropType;		// Optional : Propagation Type
};
