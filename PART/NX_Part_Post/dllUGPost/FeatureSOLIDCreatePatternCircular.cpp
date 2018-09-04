#include "stdafx.h"
#include "FeatureSOLIDCreatePatternCircular.h"

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
#include <NXOpen\Axis.hxx>
#include <NXOpen\AxisCollection.hxx>

#include <NXOpen\DatumAxis.hxx>
#include <NXOpen\DatumCollection.hxx>

//////////�� �߰�����

using namespace std;

FeatureSOLIDCreatePatternCircular::FeatureSOLIDCreatePatternCircular(Part *pPart,TransCAD::IFeaturePtr spFeature):Feature(pPart,spFeature) {}

FeatureSOLIDCreatePatternCircular::~FeatureSOLIDCreatePatternCircular()
{}

void FeatureSOLIDCreatePatternCircular::GetInfo()
{
    TransCAD::IStdSolidOperatePatternCircularFeaturePtr spFeature=GetTransCADFeature();
    
	_angleIncrement=spFeature->GetAngleIncrement(); // �� ���� ����
	_angleNum=spFeature->GetAngleNumber(); // �� ���� ����
	_radiNum=spFeature->GetRadialNumber(); // ������ ���� ����
	_spaceByRadi=spFeature->GetRadialSpacing(); // ������ ���� ����
	_isAssign=spFeature->GetIsRadialAnignment(); // ���� ���� ��ġ ����
	spFeature->GetCenterAxis(&_centerAxis[0][0], &_centerAxis[0][1], &_centerAxis[0][2], &_centerAxis[1][0], &_centerAxis[1][1], &_centerAxis[1][2]); // �߽� �� ����
	
    TransCAD::IReferencesPtr spReferences=spFeature->GetTargetFeatures();
	_numFtr=spReferences->GetCount();// ��� Ư¡ ���� ����
    
	_targetFtrs=new string[_numFtr];
	/** ��� Ư¡ ���� ���� ���� ���� **/
	for (int i=0;i<_numFtr;++i)
	{
		_targetFtrs[i]=spReferences->GetItem(i+1)->GetReferenceeName(); // GetItem�� �Ű������� 1���� ����, 0�� �ѱ�� ��Ÿ�� ���� �߻�
	}
}
   
void FeatureSOLIDCreatePatternCircular::ToUG()
{
	try
	{
		using namespace NXOpen;

		Features::Feature *cptrnFeat;
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

		cptrnFeat=builder->CommitFeature();

		//g_pRefManager->AddNewJID(_spFeature,cptrnFeat->JournalIdentifier().GetUTF8Text(),targetBodies[0]->JournalIdentifier().GetUTF8Text());
		cout<<"Circular feature JID: "<<cptrnFeat->JournalIdentifier().GetUTF8Text()<<endl;
		builder->Destroy();
	}
	catch(const NXOpen::NXException &ex)
	{
		cout<<"Error location [circular pattern feature]"<<endl;
		cout<<"Error code->"<<ex.ErrorCode()<<endl;
		cout<<"Error message->"<<ex.Message()<<endl;
	}
}


NXOpen::Features::PatternFeatureBuilder *FeatureSOLIDCreatePatternCircular::patternBuilderSet()
{
	try
	{
		using namespace NXOpen;

		Features::PatternFeatureBuilder *builder;
		builder=_Part->_nxPart->Features()->CreatePatternFeatureBuilder(NULL);
		
		for (int i=0;i<_numFtr;++i)
		{
			_targetFeature=_targetFtrs[i];
			Features::Feature * target_ftr(_Part->_nxPart->Features()->FindObject(g_pRefManager->GetJIDByTCName(_targetFeature)));
			builder->FeatureList()->Add(target_ftr);
		}

		builder->SetParentFeatureInternal(false);
		builder->PatternService()->SetPatternType(GeometricUtilities::PatternDefinition::PatternEnumCircular);
				
		Point3d centerAxisStart(_centerAxis[0][0],_centerAxis[0][1],_centerAxis[0][2]);
		Point3d centerAxisEnd(_centerAxis[1][0],_centerAxis[1][1],_centerAxis[1][2]);
		
		Point *startpoint=_Part->_nxPart->Points()->CreatePoint(centerAxisStart);
		Point *endpoint=_Part->_nxPart->Points()->CreatePoint(centerAxisEnd);

		Direction *centerVec;
		centerVec=_Part->_nxPart->Directions()->CreateDirection(startpoint,endpoint,SmartObject::UpdateOptionWithinModeling); // �߽��� ������ ���� ���� ���� ����
		Point *nullpoint(NULL);
		Axis *ctrAxis=_Part->_nxPart->Axes()->CreateAxis(nullpoint,centerVec,SmartObject::UpdateOptionWithinModeling); // �߽��� ����
		builder->PatternService()->CircularDefinition()->SetRotationAxis(ctrAxis); // ������ �߽��� ����
		builder->PatternService()->CircularDefinition()->SetRotationCenter(startpoint); // ���� ���Ͽ� �ʿ��� ȸ�� �߽��� ����
		/** �� ���� ���� **/
		builder->PatternService()->CircularDefinition()->AngularSpacing()->NCopies()->SetRightHandSide(to_string(_angleNum)); // ����
		builder->PatternService()->CircularDefinition()->AngularSpacing()->PitchAngle()->SetRightHandSide(to_string(_angleIncrement)); // ����
		/** ������ ���� ���� **/
		builder->PatternService()->CircularDefinition()->SetUseRadialDirectionToggle(true);
		builder->PatternService()->CircularDefinition()->RadialSpacing()->NCopies()->SetRightHandSide(to_string(_radiNum)); // ����
		builder->PatternService()->CircularDefinition()->RadialSpacing()->PitchDistance()->SetRightHandSide(to_string(_spaceByRadi)); // ����
		
		return builder;
	}

	catch(const NXOpen::NXException &ex)
	{
		std::cout << ex.Message() <<std::endl;
	}

	return 0;
}