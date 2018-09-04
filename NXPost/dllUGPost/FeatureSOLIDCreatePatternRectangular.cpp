#include "stdafx.h"
#include "FeatureSOLIDCreatePatternRectangular.h"

/** UG Post's header files **/
#include "Part.h"
#include "UGReferenceManager.h"

/** UG NXOpen Header files **/
#include <NXOpen\NXException.hxx>

#include <NXOpen\Body.hxx>
#include <NXOpen\BodyCollection.hxx>
#include <NXOpen\Features_PatternFeature.hxx>
#include <NXOpen\Features_FeatureCollection.hxx>
#include <NXOpen\Section.hxx>
#include <NXOpen\SectionCollection.hxx>

#include <NXOpen\Features_Extrude.hxx>
#include <NXOpen\Features_ExtrudeBuilder.hxx>
#include <NXOpen\Features_Revolve.hxx>
#include <NXOpen\Features_RevolveBuilder.hxx>
#include <NXOpen\Features_SweepAlongGuide.hxx>
#include <NXOpen\Features_SweepAlongGuideBuilder.hxx>


#include <NXOpen\Features_SelectFeatureList.hxx>
#include <NXOpen\GeometricUtilities_PatternOrientation.hxx>

#include <NXOpen\Direction.hxx>
#include <NXOpen\DirectionCollection.hxx>
#include <NXOpen\SmartObject.hxx>
#include <NXOpen\Point.hxx>
#include <NXOpen\PointCollection.hxx>

#include <NXOpen\DatumAxis.hxx>
#include <NXOpen\DatumCollection.hxx>

//////////�� �߰�����

using namespace std;

FeatureSOLIDCreatePatternRectangular::FeatureSOLIDCreatePatternRectangular(Part *pPart,TransCAD::IFeaturePtr spFeature):Feature(pPart,spFeature) {}

FeatureSOLIDCreatePatternRectangular::~FeatureSOLIDCreatePatternRectangular()
{}

void FeatureSOLIDCreatePatternRectangular::GetInfo()
{
    TransCAD::IStdSolidOperatePatternRectangularFeaturePtr spFeature=GetTransCADFeature();
    
	_numRow=spFeature->GetRowNumber();
    _numCol=spFeature->GetColumnNumber();

    _rowSpace=spFeature->GetRowSpacing();
    _colSpace=spFeature->GetColumnSpacing();

    spFeature->GetRowDirection(&_rowDir[0],&_rowDir[1],&_rowDir[2]);
    spFeature->GetColumnDirection(&_colDir[0],&_colDir[1],&_colDir[2]);

    TransCAD::IReferencesPtr spReferences=spFeature->GetTargetFeatures();
	_numFtr=spReferences->GetCount(); // ��� Ư¡ ���� ����

	/** ��� Ư¡ ���� ���� ���� ���� **/
	_targetFtrs=new string[_numFtr];

	for (int i=0;i<_numFtr;++i)
	{
		_targetFtrs[i]=spReferences->GetItem(i+1)->GetReferenceeName(); // GetItem�� �Ű������� 1���� ����, 0�� �ѱ�� ��Ÿ�� ���� �߻�
	}
}
   
void FeatureSOLIDCreatePatternRectangular::ToUG()
{
	try
	{
		using namespace NXOpen;

		Features::Feature *rptrnFeat;
		Features::PatternFeatureBuilder *builder;
		builder=this->patternBuilderSet();

		////////////////////////////////////////////////////////////////////////////////////////���� �ʿ�

		/*vector<Body *> targetBodies;
		
		Feature *tempFeat=_Part->GetFeatureByName(_targetFeature);

		if(tempFeat->GetTransCADFeature()->Type==3)
		{
			string strBF=g_pRefManager->GetJIDByTCName2(_targetFeature);
			Body *targetBody=_Part->_nxPart->Bodies()->FindObject(strBF);
			targetBodies.push_back(targetBody);
		}
		else
		{
			string strBF=g_pRefManager->GetJIDByTCName(_targetFeature);
			Body *targetBody=_Part->_nxPart->Bodies()->FindObject(strBF);
			targetBodies.push_back(targetBody);
		}*/

		rptrnFeat=builder->CommitFeature();

		//g_pRefManager->AddNewJID(_spFeature,rptrnFeat->JournalIdentifier().GetUTF8Text(),targetBodies[0]->JournalIdentifier().GetUTF8Text());
		cout<<"rectangular feature JID: "<<rptrnFeat->JournalIdentifier().GetUTF8Text()<<endl;
		builder->Destroy();
	}
	catch(const NXOpen::NXException &ex)
	{
		cout<<"Error location [rectangular pattern feature]"<<endl;
		cout<<"Error code->"<<ex.ErrorCode()<<endl;
		cout<<"Error message->"<<ex.Message()<<endl;
	}
	delete []_targetFtrs;
}


NXOpen::Features::PatternFeatureBuilder *FeatureSOLIDCreatePatternRectangular::patternBuilderSet()
{
	try
	{
		using namespace NXOpen;

		Features::PatternFeatureBuilder *builder;
		builder=_Part->_nxPart->Features()->CreatePatternFeatureBuilder(NULL);
		
		/** ��� Ư¡������ ���� **/
		for (int i=0;i<_numFtr;++i)
		{
			_targetFeature=_targetFtrs[i];
			Features::Feature * target_ftr(_Part->_nxPart->Features()->FindObject(g_pRefManager->GetJIDByTCName(_targetFeature)));
			builder->FeatureList()->Add(target_ftr);
		}
		builder->SetParentFeatureInternal(false);
		
		Point3d pOrg(0.0,0.0,0.0); //���� ������ ���� �Ű����� (�ǹ� ����)
		Vector3d xDir(_rowDir[0],_rowDir[1],_rowDir[2]);
		Vector3d yDir(_colDir[0],_colDir[1],_colDir[2]);
		/********* ROW Setting***********/
		Direction *pattern_dir1;
		pattern_dir1=_Part->_nxPart->Directions()->CreateDirection(pOrg,xDir,SmartObject::UpdateOptionWithinModeling); // row ���� ������ ���� �Ű����� ����
		builder->PatternService()->RectangularDefinition()->SetXDirection(pattern_dir1); // row ���� ����
		
		builder->PatternService()->RectangularDefinition()->XSpacing()->NCopies()->SetRightHandSide(to_string(_numRow)); // row ���� ���� ���� ����
		builder->PatternService()->RectangularDefinition()->XSpacing()->PitchDistance()->SetRightHandSide(to_string(_rowSpace)); //row ���� ���� ����
		/************** COL Setting**********/		
		builder->PatternService()->RectangularDefinition()->SetUseYDirectionToggle(true);
		
		Direction *pattern_dir2;
		pattern_dir2=_Part->_nxPart->Directions()->CreateDirection(pOrg,yDir,SmartObject::UpdateOptionWithinModeling); // col ���� ������ ���� �Ű����� ����
		builder->PatternService()->RectangularDefinition()->SetYDirection(pattern_dir2); // col ���� ����
		
		builder->PatternService()->RectangularDefinition()->YSpacing()->NCopies()->SetRightHandSide(to_string(_numCol)); // col ���� ���� ���� ����
		builder->PatternService()->RectangularDefinition()->YSpacing()->PitchDistance()->SetRightHandSide(to_string(_colSpace)); //col ���� ���� ����
		
		return builder;
	}

	catch(const NXOpen::NXException &ex)
	{
		std::cout << ex.Message() <<std::endl;
	}

	return 0;
}