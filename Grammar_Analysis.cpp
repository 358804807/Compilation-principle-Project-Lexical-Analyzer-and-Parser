#include <iostream>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <conio.h>  
#include <fstream>  
#include <vector>  
#include <conio.h>  
#include "Lex_Analysis.h"  
#include "Grammar_Analysis.h"  
#include<iomanip>


using namespace std;  
  
#define Max_Proc 500  
#define Max_Length 500  
  
#define Max_NonTer 60  
#define Max_Ter 60  
#define Max_Length2 100  
  
int pd_num = 0;  
//proc��ά�����Ǵ�1��ʼ��  
#define MAX_PD 500
int pd_word[MAX_PD][MAX_PD];//����ʽ�����飬��ߴ洢���ս�����߷��ս����Ӧ�ı��  
int first[MAX_PD][MAX_PD];  // first��
int follow[MAX_PD][MAX_PD]; // follow��
int select[MAX_PD][MAX_PD]; // select��
int M[Max_NonTer][Max_Ter][Max_Length2];  //Ԥ�������
  
int connectFirst[Max_Length];//��ĳЩFirst����������ļ���  
  
  
int firstVisit[Max_Proc];//��¼ĳ���ս����First���Ƿ��Ѿ����  
int followVisit[Max_Proc];//��¼ĳ���ս����Follow���Ƿ��Ѿ����  
  
int Empty[Max_Proc];//���Ƴ��յķ��ս���ı��  
int emptyRecu[Max_Proc];//������Ƴ��յķ��ս���ı�ż�ʱʹ�õķ��εݹ�ļ���  
int followRecu[Max_Proc];//����Follow��ʱʹ�õķ��εݹ�ļ���  
  
//extern�Ĳ��ִ�����ܳ��ֵ��ս��������  
extern vector<pair<const char *,int> > keyMap;  
extern vector<pair<const char *,int> > operMap;  
extern vector<pair<const char *,int> > limitMap;  
  
extern NormalNode * normalHead;//�׽��  
  
fstream resultfile;  
  
vector<pair<const char *,int> > NotEndDic;			//���ս��ӳ���			,�����ظ���  
vector<pair<const char *,int> > EndDic;				//�ս��ӳ���				,�����ظ���  
vector<pair<const char *,int> > SpecialDic;			//�ķ��е��������ӳ���	������-> | $(��)  
  
  
void initSpecialMapping()  
{  
    SpecialDic.clear();  
    SpecialDic.push_back(make_pair("->",GRAMMAR_ARROW));  
    SpecialDic.push_back(make_pair("|",GRAMMAR_OR));  
    SpecialDic.push_back(make_pair("$",GRAMMAR_NULL));  
    SpecialDic.push_back(make_pair("#",GRAMMAR_SPECIAL));  
  
}  
const char * searchMapping(int num)  
{  
    //��־��  
    if(num == IDENTIFER)  
    {  
        return "ID";  
    }  
    //�����ķ��е��������  
    for(int i = 0; i<SpecialDic.size(); i++)  
    {  
        if(SpecialDic[i].second == num)  
        {  
            return SpecialDic[i].first;  
        }  
    }  
    //������ս��  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        if(NotEndDic[i].second == num)  
        {  
            return NotEndDic[i].first;  
        }  
    }  
    //�����ս��  
    for(int i=0; i<EndDic.size(); i++)  
    {  
        if(EndDic[i].second == num)  
        {  
            return EndDic[i].first;  
        }  
    }  
  
}  
  
//��̬���ɷ��ս�����ڻ���Ļ����ϣ�ȷ�������ս����ͻ  
int Gene_NotEnd(char *word)  
{  
    int i = 0;  
    int dynamicNum;  
    for(i=0; i<NotEndDic.size(); i++)  
    {  
        if(strcmp(word,NotEndDic[i].first) == 0)  
        {  
            return NotEndDic[i].second;  
        }  
    }  
    if(i == NotEndDic.size())  
    {  
        if(i == 0)  
        {  
            dynamicNum = GRAMMAR_BASE;  
            NotEndDic.push_back(make_pair(word,dynamicNum));  
        }  
        else  
        {  
            dynamicNum = NotEndDic[NotEndDic.size()-1].second + 1;  
            NotEndDic.push_back(make_pair(word,dynamicNum));  
        }  
    }  
    return dynamicNum;  
}  
//�ж�ĳ������ǲ��Ƿ��ս���ı��,1�����ǣ�0�����  
int _NotEnd(int n)  
{  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        if(NotEndDic[i].second == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  
//�ж�ĳ������ǲ����ս���ı�ţ�1�����ǣ�0�����  
int inTer(int n)  
{  
    for(int i=0; i<EndDic.size(); i++)  
    {  
        if(EndDic[i].second == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  
//�ж�ĳ������ڲ��ڴ�ʱ��Empty���У�1�����ǣ�0�����  
int inEmpty(int n)  
{  
    //��ǰEmpty���ĳ���  
    int emptyLength = 0;  
    for(emptyLength = 0;; emptyLength++)  
    {  
        if(Empty[emptyLength] == -1)  
        {  
            break;  
        }  
    }  
    for(int i = 0; i<emptyLength; i++)  
    {  
        if(Empty[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
  
}  
//�ж�ĳ������ڲ��ڴ�ʱ��emptyRecu���У�1�����ǣ�0�����  
int inEmptyRecu(int n)  
{  
    //��ǰEmpty���ĳ���  
    int emptyLength = 0;  
    for(emptyLength = 0;; emptyLength++)  
    {  
        if(emptyRecu[emptyLength] == -1)  
        {  
            break;  
        }  
    }  
    for(int i = 0; i<emptyLength; i++)  
    {  
        if(emptyRecu[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  
//�ж�ĳ������ڲ��ڴ�ʱ��followRecu���У�1�����ǣ�0�����  
int inFollowRecu(int n)  
{  
    int followLength = 0;  
    for(followLength = 0;; followLength++)  
    {  
        if(followRecu[followLength] == -1)  
        {  
            break;  
        }  
    }  
    for(int i = 0; i<followLength; i++)  
    {  
        if(followRecu[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  
  
//�ж�ĳ������ǲ����ڲ���ʽ���ұ�  
int inProcRight(int n,int * p)  
{  
    //ע������Ĭ���Ǵ�3��ʼ  
    for(int i=3;; i++)  
    {  
        if(p[i] == -1)  
        {  
            break;  
        }  
        if(p[i] == n)  
        {  
            return 1;  
        }  
    }  
    return 0;  
}  
  
int seekCodeNum(char * word)  
{  
    //�����ķ��е��������  
    for(int i = 0; i<SpecialDic.size(); i++)  
    {  
        if(strcmp(word,SpecialDic[i].first) == 0)  
        {  
            return SpecialDic[i].second;  
        }  
    }  
    //�������ս��ӳ�������û�д��ս��  
    for(int i=0; i<EndDic.size(); i++)  
    {  
        if(strcmp(word,EndDic[i].first) == 0)  
        {  
            return EndDic[i].second;  
        }  
    }  
    for(int i = 0; i<keyMap.size(); i++)  
    {  
        if(strcmp(word,keyMap[i].first) == 0)  
        {  
            EndDic.push_back(make_pair(word,keyMap[i].second));  
            return keyMap[i].second;  
        }  
    }  
  
    for(int i = 0; i<operMap.size(); i++)  
    {  
        if(strcmp(word,operMap[i].first) == 0)  
        {  
            EndDic.push_back(make_pair(word,operMap[i].second));  
            return operMap[i].second;  
        }  
    }  
  
    for(int i = 0; i<limitMap.size(); i++)  
    {  
        if(strcmp(word,limitMap[i].first) == 0)  
        {  
            EndDic.push_back(make_pair(word,limitMap[i].second));  
            return limitMap[i].second;  
        }  
    }  
  
    if(strcmp(word,"ID")==0)  
    {  
        //�����־��  
        EndDic.push_back(make_pair(word,IDENTIFER));  
        return IDENTIFER;  
    }  
    else  
    {  
        //����ؼ��֡���������޽���������ս��  
        return Gene_NotEnd(word);  
    }  
}  
//�ָ�" | "�ķ�  
void splitProc(int p[][Max_Length],int &line,int orNum)  
{  
    if(p[line][1] == -1 || orNum == 0)  
    {  
        return;  
    }  
    int head = p[line][1];  
    int push = p[line][2];  
    int length = 0;  
    int right,left;  
    int lineTrue = line + orNum;  
    for(length = 3;;length++)  
    {  
        if(p[line][length] == -1)  
        {  
            break;  
        }  
    }  
    length--;  
    for(left = length,right = length;left>=2;)  
    {  
        if(p[line][left] == GRAMMAR_OR || left == 2)  
        {  
            p[line + orNum][1] = head;  
            p[line + orNum][2] = push;  
            for(int i=left+1;i<=right;i++)  
            {  
                p[line+orNum][i-left+2] = p[line][i];  
            }  
            p[line+orNum][right-left+3] = -1;  
            right = left = left-1;  
            orNum--;  
        }  
        else  
        {  
            left--;  
        }  
    }  
    line = lineTrue;  
}  
void initGrammer()  
{  
    FILE * infile;  
    char ch;  
    char array[30];  
    char * word;  
    int i;  
    int codeNum;  
    int line = 1;  
    int count = 0;  
    int orNum = 0;  
    infile = fopen("wenfa.txt","r");  
    if(!infile)  
    {  
        printf("�ķ���ʧ�ܣ�\n");  
        return;  
    }  
    initSpecialMapping();  
    NotEndDic.clear();  
    EndDic.clear();  
  
    memset(pd_word,-1,sizeof(pd_word));  
    memset(first,-1,sizeof(first));  
    memset(follow,-1,sizeof(follow));  
    memset(select,-1,sizeof(select));  
  
    memset(connectFirst,-1,sizeof(connectFirst));  
  
    memset(firstVisit,0,sizeof(firstVisit));	//���ս����first����δ���  
    memset(followVisit,0,sizeof(followVisit));	//���ս����follow����δ���  
  
    memset(Empty,-1,sizeof(Empty));  
    memset(emptyRecu,-1,sizeof(emptyRecu));  
    memset(followRecu,-1,sizeof(followRecu));  
  
    memset(M,-1,sizeof(M));  
  
    ch = fgetc(infile);  
    i = 0;  
    while(ch!=EOF)  
    {  
        i = 0;  
        while(ch == ' ' || ch == '\t')  
        {  
            ch = fgetc(infile);  
        }  
        while(ch!=' ' && ch!= '\n' && ch!=EOF)  
        {  
            array[i++] = ch;  
            ch = fgetc(infile);  
        }  
        while(ch == ' ' || ch == '\t')  
        {  
            ch = fgetc(infile);  
        }  
        word = new char[i+1];  
        memcpy(word,array,i);  
        word[i] = '\0';  
        codeNum = 0;  
        codeNum = seekCodeNum(word);  
        if(codeNum!=0)  
        {  
            count++;  
            if(codeNum == GRAMMAR_OR)  
            {  
                orNum++;  
            }  
            pd_word[line][count] = codeNum;  
  
        }  
												//ԭ����Ҫ����һ���ַ��������������ַ���������  
        if(ch == '\n')  
        {  
            splitProc(pd_word,line,orNum);			//��" | "�ķ����в��  
            count = 0;  
            orNum = 0;  
            line++;  
            ch = fgetc(infile);  
        }  
    }  
    pd_num = line - 1;  
	
    printf("\n�ս������:\n\n");  
    for(int i=0; i<EndDic.size(); i++)  
    {  
        cout<<setw(20)<<EndDic[i].first; 
		if ((i+1) % 4 == 0)
			cout << endl;
    }  
    printf("\n");  
    printf("\n���ս������:\n\n");  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        cout<<setw(20)<<NotEndDic[i].first; 
		if ((i+1) % 4 == 0)
			cout << endl;
    }  
    printf("\n");  
}  
//��s���Ϻϲ���d�����У�type = 1��������գ�$��,type = 2����������  
void merge(int *d,int *s,int type)  
{  
    int flag = 0;  
    for(int i = 0;; i++)  
    {  
        flag = 0;  
        if(s[i] == -1)  
        {  
            break;  
        }  
        int j = 0;  
        for(j = 0;; j++)  
        {  
            if(d[j] == -1)  
            {  
                break;  
            }  
            if(d[j] == s[i])  
            {  
                flag = 1;  
                break;  
            }  
        }  
        if(flag == 1)  
        {  
            continue;  
        }  
        if(type == 1)  
        {  
            d[j] = s[i];  
        }  
        else  
        {  
            if(s[i] != GRAMMAR_NULL)  
            {  
                d[j] = s[i];  
            }  
        }  
        d[j + 1] = -1;  
    }  
}  
  
void nullSet(int currentNum)  
{  
    int temp[2];  
    for(int j = 1; j<=pd_num; j++)  
    {  
        //����ұߵĵ�һ���Ǹ��ַ������ҳ���ֻ��1  
        if(pd_word[j][3] == currentNum && pd_word[j][4] == -1)  
        {  
            temp[0] = pd_word[j][1];  
            temp[1] = -1;  
            merge(Empty,temp,1);  
            nullSet(pd_word[j][1]);  
        }  
    }  
}  
//�жϸ÷��ս���Ƿ����Ƴ��գ����ս��Ҳ���ܴ��룬��û��ϵ  
int reduNull(int currentNon)  
{  
    int temp[2];  
    int result = 1;  
    int mark = 0;  
    temp[0] = currentNon;  
    temp[1] = -1;  
    merge(emptyRecu,temp,1);//�Ƚ��˷��Ų�����ݹ鼯����  
    if(inEmpty(currentNon) == 1)  
    {  
        return 1;  
    }  
  
    for(int j = 1; j<=pd_num; j++)  
    {  
        if(pd_word[j][1] == currentNon)  
        {  
            int rightLength = 0;  
            //������Ҳ��ĳ���  
            for(rightLength = 3;; rightLength++)  
            {  
                if(pd_word[j][rightLength] == -1)  
                {  
                    break;  
                }  
            }  
            rightLength--;  
            //�������Ϊ1�������Ѿ����  
            if(rightLength - 2 == 1 && inEmpty(pd_word[j][rightLength]))  
            {  
                return 1;  
            }  
            //�������Ϊ1���������ս��  
            else if(rightLength -2 == 1 && inTer(pd_word[j][rightLength]))  
            {  
                return 0;  
            }  
            //������ȳ�����2  
            else  
            {  
                for(int k=3; k<=rightLength; k++)  
                {  
                    if(inEmptyRecu(pd_word[j][k]))  
                    {  
                        mark = 1;  
                    }  
                }  
                if(mark == 1)  
                {  
                    continue;  
                }  
                else  
                {  
                    for(int k=3; k<=rightLength; k++)  
                    {  
                        result*= reduNull(pd_word[j][k]);  
                        temp[0] = pd_word[j][k];  
                        temp[1] = -1;  
                        merge(emptyRecu,temp,1);//�Ƚ��˷��Ų�����ݹ鼯����  
                    }  
                }  
            }  
            if(result == 0)  
            {  
                continue;  
            }  
            else if(result == 1)  
            {  
                return 1;  
            }  
        }  
    }  
    return 0;  
}  
  
//��first��������Ĳ������ڷ��ս�������е����  
void Gene_first(int i)  
{  
    int k = 0;  
    int currentNon = NotEndDic[i].second;	//��ǰ�ķ��ս�����  
											//���α���ȫ������ʽ  
    for(int j = 1; j<=pd_num; j++)			//j����ڼ�������ʽ  
    {  
        //�ҵ��÷��ս���Ĳ���ʽ  
        if(currentNon == pd_word[j][1])		//ע���1��ʼ  
        {  
											//���ұߵĵ�һ�����ս�����߿յ�ʱ��  
            if(inTer(pd_word[j][3]) == 1 || pd_word[j][3] == GRAMMAR_NULL)  
            {  
											//���뵱ǰ���ս����first����  
                int temp[2];  
                temp[0] = pd_word[j][3];  
                temp[1] = -1;				//��ʵ��ģ���ַ����������ֶ�  
                merge(first[i],temp,1);  
            }  
											//���ұߵĵ�һ���Ƿ��ս����ʱ��  
            else if(_NotEnd(pd_word[j][3]) == 1)  
            {  
											//��¼���ұߵ�һ�����ս���ڷ��ս��ӳ����е�λ��  
                for(k=0;; k++)  
                {  
                    if(NotEndDic[k].second == pd_word[j][3])  
                    {  
                        break;  
                    }  
                }  
											//���ұߵ�һ�����ս����δ���ʹ���ʱ��  
                if(firstVisit[k] == 0)  
                {  
                    Gene_first(k);  
                    firstVisit[k] = 1;  
                }  
                merge(first[i],first[k],2);		//���first[k]��ʱ�п�ֵ�Ļ�����ʱ���ѿ�ֵ����first[i]��  
                int r_Length = 0;  
                for(r_Length = 3;; r_Length++)  
                    if(pd_word[j][r_Length] == -1)  
                        break;  

												//��ĿǰΪֹ��ֻ������ұߵĵ�һ��(���������յĲ���)��Forѭ������֮���  
                for(k = 3; k<r_Length; k++)  
                {  
                    emptyRecu[0] = -1;//�൱�ڳ�ʼ��������ݹ鼯��  
  
												//����Ҳ��ĵ�ǰ�ַ����Ƴ��ղ��һ��������һ���ַ����ͽ�֮���һ���ַ�����First����  
                    if(reduNull(pd_word[j][k]) == 1 && k<r_Length -1)  
                    {  
                        int u = 0;  
                        for(u=0;; u++)  
                        {  
												//ע���Ǽ�¼��һ�����ŵ�λ��  
                            if(NotEndDic[u].second == pd_word[j][k+1])  
                            {  
                                break;  
                            }  
                        }  
                        if(firstVisit[u] == 0)  
                        {  
                            Gene_first(u);  
                            firstVisit[u] = 1;  
                        }  
                        merge(first[i],first[u],2);  
                    }  
													//�������һ���ַ������Ҳ���ʽ�Ҳ������Ƴ���,��$����First����  
                    else if(reduNull(pd_word[j][k]) == 1 && k == r_Length -1)  
                    {  
                        int temp[2];  
                        temp[0] = GRAMMAR_NULL;  
                        temp[1] = -1;			
                        merge(first[i],temp,1);  
                    }  
                    else  
                    {  
                        break;  
                    }  
                }  
            }  
        }  
    }  
    firstVisit[i] = 1;						//��Ϊ�Ѷ�
}  
void First()  
{  
											//�������ֱ���Ƴ��յķ��ս������  
    nullSet(GRAMMAR_NULL);  
    printf("\n");  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        Gene_first(i);  
    }  
    printf("\nFirst����\n\n");  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        printf("First��%s�� = ",NotEndDic[i].first);  
        for(int j=0;; j++)  
        {  
            if(first[i][j] == -1)  
            {  
                break;  
            }  
            printf("%s ",searchMapping(first[i][j]));  
        }  
        printf("\n");  
    }  
}  
											//��First��������ĺ���  
void connectFirstSet(int *p)  
{  
    int i = 0;  
    int flag = 0;  
    int temp[2];  
    //���P�ĳ���Ϊ1  
    if(p[1] == -1)  
    {  
        if(p[0] == GRAMMAR_NULL)  
        {  
            connectFirst[0] = GRAMMAR_NULL;  
            connectFirst[1] = -1;  
        }  
        else  
        {  
            for(i=0; i<NotEndDic.size(); i++)  
            {  
                if(NotEndDic[i].second == p[0])  
                {  
                    flag = 1;  
                    merge(connectFirst,first[i],1);  
                    break;  
                }  
            }  
            //Ҳ�������ս��  
            if(flag == 0)  
            {  
                for(i=0; i<EndDic.size(); i++)  
                {  
                    if(EndDic[i].second == p[0])  
                    {  
                        temp[0] = EndDic[i].second;  
                        temp[1] = -1;  
                        merge(connectFirst,temp,2);//�ս����First�������䱾��  
                        break;  
                    }  
                }  
            }  
        }  
    }  
    //���p�ĳ��ȴ���1  
    else  
    {  
        for(i=0; i<NotEndDic.size(); i++)  
        {  
            if(NotEndDic[i].second == p[0])  
            {  
                flag = 1;  
                merge(connectFirst,first[i],2);  
                break;  
            }  
        }  
        //Ҳ�������ս��  
        if(flag == 0)  
        {  
            for(i=0; i<EndDic.size(); i++)  
            {  
                if(EndDic[i].second == p[0])  
                {  
                    temp[0] = EndDic[i].second;  
                    temp[1] = -1;  
                    merge(connectFirst,temp,2);//�ս����First�������䱾��  
                    break;  
                }  
            }  
        }  
        flag = 0;  
        int length = 0;  
        for(length = 0;; length++)  
        {  
            if(p[length] == -1)  
            {  
                break;  
            }  
        }  
        for(int k=0; k<length; k++)  
        {  
            emptyRecu[0] = -1;//�൱�ڳ�ʼ��������ݹ鼯��  
  
            //����Ҳ��ĵ�ǰ�ַ����Ƴ��ղ��һ��������һ���ַ����ͽ�֮���һ���ַ�����First����  
            if(reduNull(p[k]) == 1 && k<length -1)  
            {  
                int u = 0;  
                for(u=0; u<NotEndDic.size(); u++)  
                {  
                    //ע���Ǽ�¼��һ�����ŵ�λ��  
                    if(NotEndDic[u].second == p[k+1])  
                    {  
                        flag = 1;  
                        merge(connectFirst,first[u],2);  
                        break;  
                    }  
                }  
                //Ҳ�������ս��  
                if(flag == 0)  
                {  
                    for(u=0; u<EndDic.size(); u++)  
                    {  
                        //ע���Ǽ�¼��һ�����ŵ�λ��  
                        if(EndDic[u].second == p[k+1])  
                        {  
                            temp[0] = EndDic[i].second;  
                            temp[1] = -1;  
                            merge(connectFirst,temp,2);  
                            break;  
                        }  
                    }  
                }  
                flag = 0;  
            }  
            //�������һ���ַ������Ҳ���ʽ�Ҳ������Ƴ���,��$����First����  
            else if(reduNull(p[k]) == 1 && k == length -1)  
            {  
                temp[0] = GRAMMAR_NULL;  
                temp[1] = -1;//��ʵ��ģ���ַ����������ֶ�  
                merge(connectFirst,temp,1);  
            }  
            else  
            {  
                break;  
            }  
        }  
    }  
}  
void Gene_follow(int i)  
{  
    int currentNon = NotEndDic[i].second;//��ǰ�ķ��ս�����  
    int temp[2];  
    int result = 1;  
    temp[0] = currentNon;  
    temp[1] = -1;  
    merge(followRecu,temp,1);//����ǰ��ż�����ݹ鼯����  
  
    //�����ǰ���ž��ǿ�ʼ����,��������ż�����Follow����  
    if(pd_word[1][1] == currentNon)  
    {  
        temp[0] = GRAMMAR_SPECIAL;//���Ҳ��Ҫ�����  
        temp[1] = -1;  
        merge(follow[i],temp,1);  
    }  
    for(int j = 1; j<=pd_num; j++) //j����ڼ�������ʽ  
    {  
        //����÷��ս����ĳ������ʽ���Ҳ�����  
        if(inProcRight(currentNon,pd_word[j]) == 1)  
        {  
            int rightLength = 1;  
            int k = 0;//kΪ�÷��ս���ڲ���ʽ�Ҳ������  
            int flag = 0;  
            int leftNum = pd_word[j][1];//����ʽ�����  
            int h = 0;  
            int kArray[Max_Length2];  
            memset(kArray,-1,sizeof(kArray));  
            for(h = 0; h < NotEndDic.size(); h++)  
            {  
                if(NotEndDic[h].second == leftNum)  
                {  
                    break;  
                }  
            }  
			//���ҵ����ս���ڷ��ռ�������λ��
  
            for(rightLength = 1;; rightLength++)  
            {  
                if(currentNon == pd_word[j][rightLength+2])		//��3��ʼ��������ԭ���ս��������λ��
                {  
                    kArray[k++] = rightLength;  
                }  
                if(pd_word[j][rightLength+2] == -1)  
                {  
                    break;  
                }  
            }  
            rightLength--;									//���ʽ�Ҳ��ܳ�
            for(int y=0;; y++)  
            {  
                if(kArray[y] == -1)  
                {  
                    break;  
                }  
															//����÷��ս�����Ҳ�����ʽ�����  
                if(kArray[y] == rightLength)  
                {  
  
                    if(inFollowRecu(leftNum) == 1)  
                    {  
                        merge(follow[i],follow[h],1);  
                        continue;  
                    }  
                    if(followVisit[h] == 0)  
                    {  
                        Gene_follow(h);  
                        followVisit[h] = 1;  
                    }  
                    merge(follow[i],follow[h],1);  
                }  
                //����������  
                else  
                {  
                    int n = 0;  
                    result = 1; 
                    for(n=kArray[y]+1; n<=rightLength; n++)  
                    {  
                        emptyRecu[0] = -1;  
                        result *= reduNull(pd_word[j][n+2]);  
                    }  
                    if(result == 1)  
                    {  
                        if(inFollowRecu(leftNum) == 1)  
                        {  
                            merge(follow[i],follow[h],1);				//��Ӧ�����A->BC �Ҵ���C->$
                            continue;  
                        }  
                        if(followVisit[h] == 0)  
                        {  
                            Gene_follow(h);  
                            followVisit[h] = 1;  
                        }  
                        merge(follow[i],follow[h],1);  
                    }  
                    int temp2[Max_Length];  
                    memset(temp2,-1,sizeof(temp2));  
                    for(n=kArray[y]+1; n<=rightLength; n++)  
                    {  
                        temp2[n-kArray[y]-1] = pd_word[j][n+2];  
                    }  
                    temp2[rightLength-kArray[y]] = -1;  
					connectFirst[0] = -1;								//����
                    connectFirstSet(temp2);  
                    merge(follow[i],connectFirst,2);  
                }  
            }  
        }  
    }  
    followVisit[i] = 1;  
}  
  
//�����з��ս����Follow��  
void Follow()  
{  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        followRecu[0] = -1;  
        Gene_follow(i);  
    }  
    printf("\nFollow����\n\n");  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        printf("Follow��%s�� = ",NotEndDic[i].first);  
        for(int j=0;; j++)  
        {  
            if(follow[i][j] == -1)  
            {  
                break;  
            }  
            printf("%s ",searchMapping(follow[i][j]));  
        }  
        printf("\n");  
    }  
}  
//���Ѿ��ֽ�Ĳ���ʽ��Ӧ��Select��,ע��Select���в��ܺ��п�($),���Type=2  
void Select()  
{  
    for(int i = 1; i<=pd_num; i++) //j����ڼ�������ʽ  
    {  
        int leftNum = pd_word[i][1];	//����ʽ�����  
        int h = 0;  
        int result = 1;  
        for(h = 0; h < NotEndDic.size(); h++)  
        {  
            if(NotEndDic[h].second == leftNum)  
            {  
                break;  
            }  
        }  
  
        int rightLength = 1;  
        for(rightLength = 1;; rightLength++)  
        {  
            if(pd_word[i][rightLength+2] == -1)  
            {  
                break;  
            }  
        }  
        rightLength--;  
        //����Ҳ��Ƴ�ʽ�ĳ���Ϊ1�����ǿ�,select[i-1] = follow[���]  
        if(rightLength == 1 && pd_word[i][rightLength + 2] == GRAMMAR_NULL)  
        {  
            merge(select[i-1],follow[h],2);  
        }  
        //����Ҳ����ǿյ�ʱ��,select[i-1] = first[�Ҳ�ȫ��]  
        //����Ҳ��ܹ��Ƴ��գ�select[i-1] = first[�Ҳ�ȫ��] ^ follow[���]  
        else  
        {  
            int temp2[Max_Length];  
            int n = 0;  
            memset(temp2,-1,sizeof(temp2));  
            for(n=1; n<=rightLength; n++)  
            {  
                temp2[n-1] = pd_word[i][n+2];  
            }  
            temp2[rightLength] = -1;  
            connectFirst[0] = -1;//Ӧ�����³�ʼ��һ��  
            connectFirstSet(temp2);  
            merge(select[i-1],connectFirst,2);  
            for(n=1; n<=rightLength; n++)  
            {  
                emptyRecu[0] = -1;  
                result *= reduNull(pd_word[i][n+2]);  
            }  
            //����Ҳ����Ƴ��գ���follow[���]����select[i-1]��  
            if(result == 1)  
            {  
                merge(select[i-1],follow[h],2);  
            }  
        }  
    }  
    printf("\nSelect����\n\n");  
    for(int i=0; i<pd_num; i++)  
    {  
        printf("Select��%d�� = ",i+1);  
        for(int j=0;; j++)  
        {  
            if(select[i][j] == -1)  
            {  
                break;  
            }  
            printf("%s ",searchMapping(select[i][j]));  
        }  
        printf("\n");  
    }  
}  
//���Ԥ�������  
void MTable()  
{  
    fstream outfile;  
    outfile.open("MTable.txt",ios::out);  
  
    for(int i=0; i<pd_num; i++)  
    {  
        int m = 0;//���ս�������  
        for(int t=0; t<NotEndDic.size(); t++)  
        {  
            if(NotEndDic[t].second == pd_word[i+1][1])  
            {  
                m = t;  
                break;  
            }  
        }  
  
        for(int j=0;; j++)  
        {  
            if(select[i][j] == -1)  
            {  
                break;  
            }  
            for(int k=0; k<EndDic.size(); k++)  
            {  
                if(EndDic[k].second == select[i][j])  
                {  
                    int n = 0;  
                    for(n=1; n<=Max_Length2; n++)  
                    {  
                        M[m][k][n-1] = pd_word[i+1][n];  
                        if(pd_word[i+1][n] == -1)  
                        {  
                            break;  
                        }  
                    }  
                    break;  
                }  
            }  
        }  
    }  
    //printf("\n*********************************Ԥ�������******************************\n\n");  
    outfile<<endl<<"*********************************Ԥ�������******************************"<<endl;  
    for(int i=0; i<NotEndDic.size(); i++)  
    {  
        for(int j=0; j<EndDic.size(); j++)  
        {  
            outfile<<"M["<<NotEndDic[i].first<<"]["<<EndDic[j].first<<"] = ";  
            //printf("M[%s][%s] = ",nonTerMap[i].first,terMap[j].first);  
            for(int k=0;; k++)  
            {  
                if(M[i][j][k] == -1)  
                {  
                    break;  
                }  
                outfile<<searchMapping(M[i][j][k]);  
                //printf("%s ",searchMapping(M[i][j][k]));  
            }  
            outfile<<endl;  
            //printf("\n");  
        }  
        outfile<<endl<<endl;  
        //printf("\n\n");  
    }  
    outfile.close();  
}  
  
void InitStack(SeqStack *S)    /*��ʼ��˳��ջ*/  
{  
    S->top = -1;  
}  
int Push(SeqStack *S,int x)   /*��ջ*/  
{  
    if(S->top ==Stack_Size-1)  
        return 0;  
    S->top++;  
    S->elem[S->top]=x;  
    return 1;  
}  
int Pop(SeqStack *S)   /*��ջ*/  
{  
    if(S->top==-1)  
        return 0;  
    else  
    {  
        S->top--;  
        return 1;  
    }  
}  
int GetTop(SeqStack *S,int *x)   /*ȡջ��Ԫ��*/  
{  
    if(S->top==-1)  
        return 0;  
    else  
    {  
        *x=S->elem[S->top];  
        return 1;  
    }  
}  
void ShowStack1(SeqStack *S)   /*��ʾջ���ַ��������ջ��Ԫ��*/  
{  
  
    int i;  
    for(i=S->top; i>=0; i--)  
    {  
        //printf("%s ",searchMapping(S->elem[i]));  
        resultfile<<searchMapping(S->elem[i])<<" ";  
    }  
}  
void ShowStack2(SeqStack *S)   /*��ʾջ���ַ��������ջ��Ԫ��*/  
{  
  
    int i;  
    for(i=S->top; i>=0; i--)  
    {  
        //printf("%s ",searchMapping(S->elem[i]));  
        resultfile<<searchMapping(S->elem[i])<<" ";  
    }  
}  
//����Դ����  
void Analysis()  
{  
    //����������  
  
    resultfile.open("result.txt",ios::out);  
  
    SeqStack s1,s2;  
    int c1,c2;  
    int i = 0;  
    int reserve[Stack_Size];//����ջ������ջ  
    NormalNode * p = normalHead;  
    int s1Length = 0;  
    memset(reserve,-1,sizeof(reserve));  
  
    InitStack(&s1);  /*��ʼ������ջ�����봮*/  
    InitStack(&s2);  
    Push(&s1,GRAMMAR_SPECIAL);  
    Push(&s1,pd_word[1][1]);  
    Push(&s2,GRAMMAR_SPECIAL);  
  
    p = p->next;  
    while(p!=NULL)  
    {  
  
        if(p->type == AUTO || p->type == CONST || p->type == UNSIGNED || p->type == SIGNED  
                || p->type ==STATIC || p->type == VOLATILE )  
        {  
            reserve[i++] =  DESCRIBE;  
            //Push(&s2,DESCRIBE);  
        }  
        else if(p->type == INT_VAL)  
        {  
            reserve[i++] =  DIGIT;  
            //Push(&s2,DIGIT);  
        }  
        else if(p->type == CHAR || p->type == DOUBLE || p->type == FLOAT || p->type == INT ||  
                p->type == LONG || p->type == SHORT || p->type == VOID)  
        {  
            reserve[i++] =  TYPE;  
            //Push(&s2,TYPE);  
        }  
        else if(p->type == STRING_VAL)  
        {  
            reserve[i++] =  STRING;  
            //Push(&s2,STRING);  
        }  
        else if(p->type == DOU_QUE || p->type == SIN_QUE)  
        {  
  
        }  
        else  
        {  
            reserve[i++] =  p->type;  
            //Push(&s2,p->type);  
        }  
        p = p->next;  
    }  
    //�����ջ�ĳ���  
    for(s1Length = 0;; s1Length++)  
    {  
        if(reserve[s1Length] == -1)  
        {  
            break;  
        }  
    }  
    //������ջ  
    for(i = s1Length; i>0; i--)  
    {  
        Push(&s2,reserve[i-1]);  
    }  
  
    for(i=0;; i++)   /*����*/  
    {  

        int flag = 0;  
        int h1;  
        int h2;  

        resultfile<<"��"<<i + 1<<"��"<<endl;  

        resultfile<<"ջ1-����ջ:";  
        ShowStack1(&s1);  

        resultfile<<endl;  
  
        resultfile<<"ջ2-����ջ:";  
        ShowStack2(&s2);  
        //printf("\n");  
        resultfile<<endl;  
  
        GetTop(&s1,&c1);   /*ȡջ��Ԫ�أ���Ϊc1��c2*/  
        GetTop(&s2,&c2);  
        if(c1 == GRAMMAR_SPECIAL && c2 == GRAMMAR_SPECIAL)  /*������ջ������ջ��ʣ��#ʱ�������ɹ�*/  
        {  

            resultfile<<"�﷨�����ɹ���"<<endl;  
            break;  
        }  
        if(c1 == GRAMMAR_SPECIAL && c2!= GRAMMAR_SPECIAL)  /*������ջʣ��#�������봮δ����ʱ������ʧ�� */  
        {  
            //printf("ʧ��!\n");  
            resultfile<<"ʧ��!"<<endl;  
            break;  
        }  
        if(c1 == c2)/*����ջ��ջ��Ԫ�غ����봮��ջ��Ԫ����ͬʱ��ͬʱ����*/  
        {  
            Pop(&s1);  
            Pop(&s2);  
            flag = 1;  
        }  
  
        else /*��Ԥ�������*/  
        {  
            //��¼�·��ս����λ��  
            for(h1=0; h1<NotEndDic.size(); h1++)  
            {  
                if(NotEndDic[h1].second == c1)  
                {  
                    break;  
                }  
            }  
            //��¼���ս����λ��  
            for(h2=0; h2<EndDic.size(); h2++)  
            {  
                if(EndDic[h2].second == c2)  
                {  
                    break;  
                }  
            }  
            if(M[h1][h2][0] == -1)  
            {  
                //printf("Error\n");  
                resultfile<<"Error"<<endl;  
                break;//�������Ļ���ֱ����ֹ����  
            }  
            else  
            {  
                int length = 0;  
                //��¼���Ƶ�ʽ�ĳ���  
                for(length = 0;; length++)  
                {  
                    if(M[h1][h2][length] == -1)  
                    {  
                        break;  
                    }  
                }  
                Pop(&s1);  
                //������ǿյĻ���������ջ  
                if(M[h1][h2][2] != GRAMMAR_NULL)  
                {  
                    for(int k = length-1; k>=2; k--)  
                    {  
                        Push(&s1,M[h1][h2][k]);  
                    }  
                }  
            }  
        }  
        if(flag == 1)  
        {   
            resultfile<<"�þ�ɹ�ƥ��!"<<endl;  
        }  
        else  
        {  
            resultfile<<"����ʽ��";  
    
            int w = 0;  
            //��¼���Ƶ�ʽ�ĳ���  
            for(w = 0;; w++)  
            {  
                if(M[h1][h2][w] == -1)  
                {  
                    break;  
                }  

                resultfile<<searchMapping(M[h1][h2][w]);  
            }  

            resultfile<<endl<<endl;  
        }  
    }  
    resultfile.close();  
	cout << "�﷨��������Ѿ�����result.txt�ļ��У�\n";
}  
