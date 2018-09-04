#include "stdafx.h"
#include "FDatumPlane.h"
#include "UGReferenceManager.h"

#include "Part.h"

/* UG NXOpen headers */
#include "NXOpen\DatumCollection.hxx"
#include "NXOpen\DatumPlane.hxx"


using namespace std;

/* �� Ŭ���� �����ڴ� ������� �ʴ´�. */
FDatumPlane::FDatumPlane(Part * pPart, Point3D &origin, Direct3D &normal)
	: Feature(pPart, NULL)
{
	_origin = origin;
	_normal = normal;
}

/* Part.cpp���� ����ϴ� Ŭ���� �����ڴ� �̰� */
FDatumPlane::FDatumPlane(Part * pPart, TransCAD::IFeaturePtr spFeature)
	: Feature(pPart, spFeature)
{

}

FDatumPlane::~FDatumPlane(void) {}

void FDatumPlane::GetInfo()
{
	string tcfeatureName = GetFeatureName();
	
	_origin = Point3D(0, 0, 0);

	if ( tcfeatureName == string("XYPlane") )
	{
		_normal = Direct3D(0, 0, 1);
		// DEBUG
		// cout << " -> is translated ! " << endl;
	}
	else if ( tcfeatureName == string("YZPlane") )
	{
		_normal = Direct3D(1, 0, 0);
		// DEBUG
		//cout << " -> is translated ! " << endl;
	}
	else if ( tcfeatureName == string("ZXPlane") )
	{
		_normal = Direct3D(0, 1, 0);
		// DEBUG
		//cout << " -> is translated ! " << endl;
	}
}

void FDatumPlane::ToUG()
{
	//cout << "Basis Datum coordinate system already exist" << endl;
	
	/** DatumPlane JID ��� **/
	string planeName;

	// UG�� CSYS���� XY�� �����ϸ� JID�� YZ�� ǥ��
	if ( _normal == Direct3D(0, 0, 1) )
		planeName = "YZ";

	else if ( _normal == Direct3D(1, 0, 0) )
		planeName = "XZ";

	else if ( _normal == Direct3D(0, 1, 0) )
		planeName = "XY";

	NXOpen::DatumCollection * datumList = _Part->_nxPart->Datums();
	NXOpen::DatumCollection::iterator itr;

	for ( itr = datumList->begin(); itr != datumList->end(); ++itr )
	{
		NXOpen::DatumPlane * dP = (NXOpen::DatumPlane *)* itr;
		
		string JID = dP->JournalIdentifier().GetUTF8Text();

		if ( JID.find(planeName) != -1 )	// ������ ���ڸ� ã���� index, ã�� ���ϸ� -1 ����
		{
			/** Reference manager�� JID �߰� **/
			g_pRefManager->AddNewJID(_spFeature, JID, "");

			// DEBUG
			// cout << "Basis Datum_CSYS JID : " << JID << endl;
		}
	}
}