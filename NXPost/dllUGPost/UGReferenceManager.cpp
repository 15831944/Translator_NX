#include "stdafx.h"
#include "PN_TYPE.h"
#include "MP_Utility.h"
#include "UGReferenceManager.h"


UGReferenceManager::UGReferenceManager()
{
	m_id = 0;
}

UGReferenceManager::~UGReferenceManager(void)
{
	// header�� vector ���� �����
}

/** TransCAD�� �ִ� ��� feature ������ ���� **/
int UGReferenceManager::AddNewFeature(TransCAD::IFeaturePtr spFeature)
{
	/** Input feature�� �̸��� Ÿ���� ���� **/
	std::string name = (std::string)spFeature->Name;
	TransCAD::FeatureType type = spFeature->Type;

	int id = 0;
	for ( int i = 0; i < (int)_listNameMap.size(); ++i )
	{
		if ( _listNameMap[i]._type == type )
			id++;
	}

	id++;

	NAME_MAP map;
	map._id = id;
	map._name = name;
	map._type = type;

	_listNameMap.push_back(map);

	return 1;
}

/** UG feature�� Journal Identifier�� ���� **/
int UGReferenceManager::AddNewJID(TransCAD::IFeaturePtr spFeature, std::string fJID, std::string sJID)
{
	std::string name = (std::string)spFeature->Name;

	int id = 0;
	for (int i = 0; i < (int)_listJIDMap.size(); ++i)
	{
		if (_listJIDMap[i]._name == name)
			id++;
	}

	id++;

	JID_MAP map;
	map._id = id;
	map._fJID = fJID;
	map._sJID = sJID;
	map._name = name;

	_listJIDMap.push_back(map);

	return 1;
}

int UGReferenceManager::AddSecJID(TransCAD::IFeaturePtr spFeature, std::string secJID)
{
	std::string name = (std::string)spFeature->Name;

	int id = 0;
	for (int i = 0; i < (int)_listJIDMap.size(); ++i)
	{
		if (_listJIDMap[i]._name == name)
			id++;
	}

	id++;

	JID_MAP map;
	map._id = id;
	map._sJID = secJID;
	map._name = name;

	_listJIDMap.push_back(map);

	return 1;
}

/** TransCADName���� FeatureType �˻� **/
int UGReferenceManager::GetFeatureTypeByTCName(std::string strTCName, TransCAD::FeatureType &type)
{
	for (int i = 0; i<(int)_listNameMap.size(); ++i)
	{
		if (_listNameMap[i]._name == strTCName)
		{
			type = _listNameMap[i]._type;
			return 1;
		}
	}

	return 0;
}

/** TransCADName���� FeatureID �˻� **/
int UGReferenceManager::GetFeatureIDByTCName(std::string strTCName, int &id)
{
	for (int i = 0; i<(int)_listNameMap.size(); ++i)
	{
		if (_listNameMap[i]._name == strTCName)
		{
			id = _listNameMap[i]._id;
			return 1;
		}
	}

	return 0;
}

/** TransCAD�� feature name���� JID �˻� **/
std::string UGReferenceManager::GetJIDByTCName(std::string strTCName)
{
	for ( int i = 0; i < (int)_listJIDMap.size(); ++i )
	{
		if ( _listJIDMap[i]._name == strTCName )
		{
			return _listJIDMap[i]._fJID;
		}
	}
}

std::string UGReferenceManager::GetJIDByTCName2(std::string strTCName)
{
	for ( int i = 0; i < (int)_listJIDMap.size(); ++i )
	{
		if ( _listJIDMap[i]._name == strTCName )
		{
			return _listJIDMap[i]._sJID;
		}
	}
}

/** "For FACE" TransCAD BRep name�� �̿��Ͽ� Feature name�� Ž�� **/
std::string UGReferenceManager::ConvertToBRepFeature(std::string strPersistentName)
{
	EN_F f;
	Parsing_F(strPersistentName, f);

	TransCAD::FeatureType type;
	int id;

	GetFeatureTypeByTCName(f._bn.feature_id, type);
	GetFeatureIDByTCName(f._bn.feature_id, id);

	if (f._sn.total_num != 0) {
		GetFeatureTypeByTCName(f._sn._bn_list[f._sn.total_num - 1].feature_id, type);
		GetFeatureIDByTCName(f._sn._bn_list[f._sn.total_num - 1].feature_id, id);
	}

	std::string featName = f._bn.feature_id;

	return featName;
}

/**  **/
std::string UGReferenceManager::ConvertToBRepFeature2(std::string strPersistentName)
{
	int c = count(strPersistentName.begin(), strPersistentName.end(), ';');  //;�� ���� ���

	char * buff;
	int len = strPersistentName.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, strPersistentName.c_str());

	char seps_temp[] = ";"; //������

	std::vector<string> _temp_str_list;

	_temp_str_list.push_back(strtok_s(buff, seps_temp, &context));
	for (int i = 0; i<c; ++i)
		_temp_str_list.push_back(strtok_s(NULL, seps_temp, &context));

	std::string secondFeaturePN = _temp_str_list[1];

	char * buff2;
	int len2 = secondFeaturePN.size();
	buff2 = new char[len2 + 1];
	char * context2 = NULL;
	strcpy_s(buff, len2 + 1, secondFeaturePN.c_str());

	char seps_temp2[] = " ,\t\n"; //������
	std::string secFeature_id = strtok_s(buff, seps_temp2, &context2);

	return secFeature_id;
}