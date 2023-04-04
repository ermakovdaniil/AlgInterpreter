/*
 *	Script.cpp
 *  ��������� ������ ����������� ����������.
 *  ������ ���������� �������������� ������������ BASIC-���������
 *  ����� ����������������, ������������� � �������� �������
 *  ������������ ������ � ����� �� ����.
 */

#include <fstream>
#include <iostream>
#include <string>
#include "stdafx.h"
#include "executor.h"


int _tmain(int argc, _TCHAR* argv[])
{
    std::system("color F0");
    // TODO: ����������� ������ ����� ���� � ����� �� ��������
    std::string path /*= "test1.txt"*/;
    std::cout << "Input script path: ";
    std::cin >> path;
    //if (argc < 2) {
    //    std::cout << "Usage: " << std::endl;
    //    std::cout << "Script filename.ext" << std::endl;
    //    return 1;
    //}

    try {
        //std::ifstream in(argv[1]);
        // TODO: ����������� ������
        std::ifstream in(path); // �������� ������ �����
        if (!in) // ���� �� ���������� ������� ����� �����
            throw "Can't open file"; // ����� ���������� � ���, ��� ���������� ������� ����
        execute_script(in); // ���������� �������
    }
    catch (const char* s) {
        std::cerr << std::endl << "ERROR: " << s << std::endl; // ����� ���������� �� ������
    }

    return 0;
}