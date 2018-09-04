#include "stdafx.h"
#include "FDatumPlaneOffset.h"
#include "UGReferenceManager.h"

#include "Part.h"

/* UG NXOpen headers */
#include <NXOpen\Features_DatumPlaneBuilder.hxx>
#include <NXOpen\Features_FeatureCollection.hxx>
#include <NXOpen\DatumCollection.hxx>
#include <NXOpen\DatumPlane.hxx>
#include <NXOpen\Plane.hxx>
#include <NXOpen\Face.hxx>

using namespace std;

FDatumPlaneOffset::FDatumPlaneOffset(Part * pPart, TransCAD::IFeaturePtr spFeature)
	: Feature(pPart, spFeature)
{
	
}

FDatumPlaneOffset::~FDatumPlaneOffset(void) {}

void FDatumPlaneOffset::GetInfo()
{
	TransCAD::IStdDatumPlaneOffsetFeaturePtr spFeature = GetTransCADFeature();
	TransCAD::IReferencePtr spRefernce = spFeature->GetSelectedPlane();
	
	_featurename = spRefernce->ReferenceeName;
	_distance = spFeature->Distance;
	_isFlip = spFeature->IsFlip ? true : false;

	if(_isFlip)
		_distance=(-1)*_distance;
	
	cout << endl;
	cout << "	" << "Name           : " << spFeature->Name << endl;
	cout << "	" << "Distance       : " << spFeature->Distance << endl;
	cout << "	" << "ReferenceeName : " << spRefernce->ReferenceeName << endl;
	cout << "	" << "Type           : " << spRefernce->Type << endl;

	// �ش� ���İ� ��ǥ����� ���, �Ʒ��� ���� ����. �� �� �Ʒ��� ���� ���·� ���̱� ����
	// UG Datum_CSYS���� XY�� ���� ��, YZ�� ǥ���....
	if (_featurename == "XYPlane")
		_coordinate = "YZ plane";
	else if (_featurename == "YZPlane")
		_coordinate = "XZ plane";
	else if (_featurename == "ZXPlane")
		_coordinate = "XY plane";
}

void FDatumPlaneOffset::ToUG()
{
	using namespace NXOpen;
	
	Features::DatumPlaneBuilder * DPbuilder;
	DPbuilder = _Part->_nxPart->Features()->CreateDatumPlaneBuilder(NULL);

	/** DatumPlane�� ������ ��, ���Ǵ� plane�� refPlane�� �ְ� ���� ������ ������Ʈ **/
	Plane * refPlane;
	refPlane = DPbuilder->GetPlane();
	refPlane->SetMethod(PlaneTypes::MethodTypeDistance);		// Plane�� �����ϴ� ��� ����

	/** TransCAD���� ���� Reference plane������ �̿��Ͽ� JID�� ã�� **/
	
	NXString refJID = g_pRefManager->GetJIDByTCName(_featurename);
	DatumPlane * refDP = (dynamic_cast<DatumPlane *>(_Part->_nxPart->Datums()->FindObject(refJID)));
	
	/** ����ó��: Reference datum plane�� ������ �ִ��� ������ Ȯ�� **/
	if (refDP->IsBlanked())
		MessageBoxA(NULL, "Empty reference datum plane.. plase check reference datum plane", "Warning", MB_OK);

	/** setGeometry�� ���� ������ Ÿ�� ��ȯ: DatumPlane -> NXObject **/
	vector<NXObject *> objects;
	objects.push_back(refDP);
	refPlane->SetGeometry(objects);
	
	/** NXstring type�� Offset ���� ����ϱ� ���� �� ��ȯ: double -> NXString(=string) **/
	string offsetVal = to_string((_distance));
	Expression * exp = refPlane->Expression();
	exp->SetRightHandSide(offsetVal);

	refPlane->Evaluate();
	/** DatumPlane feature�� �����ϱ� ���� commit **/
	Features::Feature * dpFeat;
	dpFeat = DPbuilder->CommitFeature();
	// DEBUG cout << "DatumPlaneOffset's JID : " << dpFeat->JournalIdentifier().GetUTF8Text() << endl;

	/** Reference manager�� JID �߰� **/
	g_pRefManager->AddNewJID(_spFeature, dpFeat->JournalIdentifier().GetUTF8Text(), "");

	// DEBUG cout << "	" << "Name	: " << dpFeat->JournalIdentifier().GetUTF8Text() << endl;

	DPbuilder->Destroy();


}