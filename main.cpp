#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "Lex_Analysis.h"
#include "Grammar_Analysis.h"

using namespace std;

int main()
{
	
    //�ʷ���������
    initKeyMapping();
    initOperMapping();
    initLimitMapping();
    initNode();
    scanner();
    //�﷨��������
    initGrammer();
    First();
    Follow();
    Select();
    MTable();
    Analysis();

    return 0;
}
