#include "stdafx.h"

#include "MP_Utility.h"
#include <algorithm>
#include <math.h>
#include <string.h>

using namespace std;


void Parsing_F(string _str, EN_F & _f)
{
	string _bn_str, _OSI_str, _sn_str;
	Delim_F(_str, _bn_str, _OSI_str, _sn_str);
	Parsing_BN(_bn_str, _f._bn);
	Parsing_OSI(_OSI_str, _f._osi);
	Parsing_SN(_sn_str, _f._sn);

}

void Parsing_E(string str, EN_E & e)
{
	string _face1, _face2, _osi;

	Delim_E(str, _face1, _face2, _osi);
	Parsing_F(_face1, e._f1);
	Parsing_F(_face2, e._f2);
	//Parsing_OSI(_osi, e._osi);

}

void Parsing_V(string str, EN_V & v)
{
	string _face1, _face2, _face3, _osi;

	Delim_V(str, _face1, _face2, _face3, _osi);
	Parsing_F(_face1, v._f1);
	Parsing_F(_face2, v._f2);
	Parsing_F(_face3, v._f3);
	Parsing_OSI(_osi, v._osi);
}


void Parsing_BN(string _str, basic_naming & _bn)
{
	char * buff;
	int len = _str.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, _str.c_str());

	char seps_temp[] = " ,\t\n"; //������
	_bn.feature_id = strtok_s(buff, seps_temp, &context); //ù��° �ܾ�
	_bn.id1 = strtok_s(NULL, seps_temp, &context);  //�ι�° �ܾ�
	_bn.id2 = strtok_s(NULL, seps_temp, &context);  //����° �ܾ�
	_bn.id3 = strtok_s(NULL, seps_temp, &context);  //�׹�° �ܾ�	
	_bn.id4 = strtok_s(NULL, seps_temp, &context);  //����° �ܾ�
	_bn.id5 = strtok_s(NULL, seps_temp, &context);  //�׹�° �ܾ�	
	string _test = (strtok_s(NULL, seps_temp, &context));  //�׹�° �ܾ�	

	if (_test == "Sweep_Feature") _bn.option = Sweep_Feature;
	if (_test == "Extrude_Feature") _bn.option = Extrude_Feature;
	if (_test == "Revolve_Feature") _bn.option = Revolve_Feature;
	if (_test == "Loft_Feature") _bn.option = Loft_Feature;
	if (_test == "Fillet_Feature") _bn.option = Fillet_Feature;
	if (_test == "Chamfer_Feature") _bn.option = Chamfer_Feature;
	if (_test == "Shell_Feature") _bn.option = Shell_Feature;
	if (_test == "Pattern_Feature") _bn.option = Pattern_Feature;
	if (_test == "Datum_Feature") _bn.option = Datum_Feature;
	if (_test == "Sketch_Segment") _bn.option = Sketch_Segment;

	delete[] buff;

}


void Parsing_OSI(string _str, OSI & _osi)
{
	char * buff;
	int len = _str.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, _str.c_str());

	char seps_temp[] = " ,\t\n"; //������
	_osi.order = atoi(strtok_s(buff, seps_temp, &context)); //ù��° �ܾ�
	_osi.total_num = atoi(strtok_s(NULL, seps_temp, &context));  //�ι�° �ܾ�

	delete[] buff;

}

void Parsing_SN(string _str, secondary_name & _sn)
{
	int c = count(_str.begin(), _str.end(), ';');  //;�� ���� ���

	char * buff;
	int len = _str.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, _str.c_str());

	char seps_temp[] = ";"; //������

	std::vector<string> _temp_str_list;

	_temp_str_list.push_back(strtok_s(buff, seps_temp, &context));
	for (int i = 0; i<c; ++i)
		_temp_str_list.push_back(strtok_s(NULL, seps_temp, &context));

	_sn.total_num = atoi(_temp_str_list[0].c_str());  //Merging Face�� ����

	basic_naming _temp_bn;
	if (_sn.total_num > 0) // Merging �� Face�� ����
	{
		for (int j = 0; j< c; ++j)
		{
			Parsing_BN(_temp_str_list[j + 1], _temp_bn);
			_sn._bn_list.push_back(_temp_bn);
		}
	}

	delete[] buff;
}

void Delim_V(string _input, string & _F1, string & _F2, string & _F3, string & _OSI)
{
	char * buff;
	int len = _input.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, _input.c_str());

	char seps_temp[] = "#"; //������
	_F1 = strtok_s(buff, seps_temp, &context); //ù��° �ܾ�
	_F2 = strtok_s(NULL, seps_temp, &context);  //�ι�° �ܾ�
	_F3 = strtok_s(NULL, seps_temp, &context);  //����° �ܾ�
	_OSI = strtok_s(NULL, seps_temp, &context);  //�׹�° �ܾ�

	delete[] buff;

}

void Delim_E(string  _input, string & _F1, string & _F2, string & _OSI)
{
	char * buff;
	int len = _input.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, _input.c_str());

	char seps_temp[] = "#"; //������
	_F1 = strtok_s(buff, seps_temp, &context); //ù��° �ܾ�
	_F2 = strtok_s(NULL, seps_temp, &context);  //�ι�° �ܾ�

												// 2011-02-28, Suchul Shin, _OSI (0,0 �̷���,, �� ����(Acis) TransCAD������ ��������)
												//_OSI =  strtok_s( NULL, seps_temp, &context );  //����° �ܾ�	

	delete[] buff;

}

void Delim_F(string _input, string & _bn, string & _OSI, string & _sn)
{
	char * buff;
	int len = _input.size();
	buff = new char[len + 1];
	char * context = NULL;
	strcpy_s(buff, len + 1, _input.c_str());

	char seps_temp[] = ":"; //������
	_bn = strtok_s(buff, seps_temp, &context); //ù��° �ܾ�
	_OSI = strtok_s(NULL, seps_temp, &context);  //�ι�° �ܾ�
	_sn = strtok_s(NULL, seps_temp, &context);  //����° �ܾ�

	delete[] buff;
}

string Print_BN(basic_naming bn)
{
	string bn_s = bn.feature_id + "," + bn.id1 + "," + bn.id2 + "," + bn.id3 + "," + bn.id4 + "," + bn.id5 + ",";

	switch (bn.option)
	{
	case 0: //Sweep_Feature
		bn_s = bn_s + "Sweep_Feature";
		break;
	case 1: //Extrude_Feature,
		bn_s = bn_s + "Extrude_Feature";
		break;
	case 2: //Revolve_Feature,
		bn_s = bn_s + "Revolve_Feature";
		break;
	case 3: //Loft_Feature,
		bn_s = bn_s + "Loft_Feature";
		break;
	case 4: //	Fillet_Feature,
		bn_s = bn_s + "Fillet_Feature";
		break;
	case 5: //	Chamfer_Feature,
		bn_s = bn_s + "Chamfer_Feature";
		break;
	case 6: //	Shell_Feature,
		bn_s = bn_s + "Shell_Feature";
		break;
	case 7: //	Pattern_Feature,
		bn_s = bn_s + "Pattern_Feature";
		break;
	case 8: //	Datum_Feature,
		bn_s = bn_s + "Datum_Feature";
		break;
	case 9: //	Sketch_Segment
		bn_s = bn_s + "Sketch_Segment";
		break;

	default:
		cerr << "Strange Feature Type for Basic Naming\n";
		break;
	}

	return bn_s;
}

string Print_OSI(OSI osi)
{
	string osi_s;
	char _order_s[10], _total_num_s[10];

	_itoa_s(osi.total_num, _total_num_s, 10);
	_itoa_s(osi.order, _order_s, 10);

	osi_s.append(_order_s);
	osi_s = osi_s + ",";
	osi_s.append(_total_num_s);

	return osi_s;
}

string Print_SN(secondary_name sn)
{

	char temp[10];
	_itoa_s(sn.total_num, temp, 10);

	string sn_s = temp;

	if (sn.total_num == 0)
	{
		sn_s = sn_s + ";0";
	}
	else
	{
		for (int i = 0; i< sn.total_num; ++i)
		{
			sn_s = ";" + Print_BN(sn._bn_list[i]);
		}
	}

	return sn_s;
}





string Print_F(EN_F f)
{
	string _f_s = Print_BN(f._bn) + ":" + Print_OSI(f._osi) + ":" + Print_SN(f._sn);

	return _f_s;
}

string Print_E(EN_E e)
{
	string _e_s = Print_F(e._f1) + "#" + Print_F(e._f2) + "#" + Print_OSI(e._osi);

	return _e_s;

}

string Print_V(EN_V v)
{
	string _v_s = Print_F(v._f1) + "#" + Print_F(v._f2) + "#" + Print_F(v._f3) + "#" + Print_OSI(v._osi);;


	return _v_s;

}