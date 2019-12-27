#ifndef _LEX_ANALYSIS_H  
#define _LEX_ANALYSIS_H  
  
//�ؼ���  
#define AUTO 1  
#define BREAK 2  
#define CASE 3  
#define CHAR 4  
#define CONST 5  
#define CONTINUE 6  
#define DEFAULT 7  
#define DO 8  
#define DOUBLE 9  
#define ELSE 10  
#define ENUM 11  
#define EXTERN 12  
#define FLOAT 13  
#define FOR 14  
#define GOTO 15  
#define IF 16  
#define INT 17  
#define LONG 18  
#define REGISTER 19  
#define RETURN 20  
#define SHORT 21  
#define SIGNED 22  
#define SIZEOF 23  
#define STATIC 24  
#define STRUCT 25  
#define SWITCH 26  
#define TYPEDEF 27  
#define UNION 28  
#define UNSIGNED 29  
#define VOID 30  
#define VOLATILE 31  
#define WHILE 32  
#define KEY_DESC "�ؼ���"  
  
//��־��  
#define IDENTIFER 40  
#define IDENTIFER_DESC "��ʶ��"  
  
//����  
#define INT_VAL 51 //���γ���  
#define CHAR_VAL 52 //�ַ�����  
#define FLOAT_VAL 53 //����������  
#define STRING_VAL 54 //˫���ȸ���������  
#define MACRO_VAL 55 //�곣��  
#define CONSTANT_DESC "����"  
  
//�����  
#define NOT 61   // !  
#define BYTE_AND 62 //&  
#define COMPLEMENT 63 // ~  
#define BYTE_XOR  64 // ^  
#define MUL 65 // *  
#define DIV 66// /  
#define MOD 67 // %  
#define ADD 68 // +  
#define SUB 69 // -  
#define LES_THAN 70 // <  
#define GRT_THAN 71 // >  
#define ASG 72 // =  
#define ARROW 73 // ->  
#define SELF_ADD 74 // ++  
#define SELF_SUB 75 // --  
#define LEFT_MOVE 76 // <<  
#define RIGHT_MOVE 77 // >>  
#define LES_EQUAL 78 // <=  
#define GRT_EQUAL 79 // >=  
#define EQUAL 80 // ==  
#define NOT_EQUAL 81 // !=  
#define AND 82 // &&  
#define OR 83 // ||  
#define COMPLETE_ADD 84 // +=  
#define COMPLETE_SUB 85 // -=  
#define COMPLETE_MUL 86 // *=  
#define COMPLETE_DIV 87 // /=  
#define COMPLETE_BYTE_XOR 88 // ^=  
#define COMPLETE_BYTE_AND 89 // &=  
#define COMPLETE_COMPLEMENT 90 // ~=  
#define COMPLETE_MOD 91 //%=  
#define BYTE_OR 92 // |  
  
#define OPE_DESC "���"  
  
//�޽��  
#define LEFT_BRA 100 // (  
#define RIGHT_BRA 101 // )  
#define LEFT_INDEX 102 // [  
#define RIGHT_INDEX 103 // ]  
#define L_BOUNDER 104 //  {  
#define R_BOUNDER 105 // }  
#define POINTER 106 // .  
#define JING 107 // #  
#define UNDER_LINE 108 // _  
#define COMMA 109 // ,  
#define SEMI 110 // ;  
#define SIN_QUE 111 // '  
#define DOU_QUE 112 // "  
  
#define CLE_OPE_DESC "�޽��"  
  
#define NOTE1 120 // "/**/"ע��  
#define NOTE2 121 // "//"ע��  
#define NOTE_DESC "ע�ͺ�"  
  
  
#define HEADER 130 //ͷ�ļ�  
#define HEADER_DESC "ͷ�ļ�"  
  
//��������  
#define FLOAT_ERROR "float��ʾ����"  
#define FLOAT_ERROR_NUM 1  
#define DOUBLE_ERROR "double��ʾ����"  
#define DOUBLE_ERROR_NUM 2  
#define NOTE_ERROR "ע��û�н�����"  
#define NOTE_ERROR_NUM 3  
#define STRING_ERROR "�ַ�������û�н�����"  
#define STRING_ERROR_NUM 4  
#define CHARCONST_ERROR "�ַ�����û�н�����"  
#define CHARCONST_ERROR_NUM 5  
#define CHAR_ERROR "�Ƿ��ַ�"  
#define CHAR_ERROR_NUM 6  
#define LEFT_BRA_ERROR "'('û�ж�Ӧ��"  
#define LEFT_BRA_ERROR_NUM 7  
#define RIGHT_BRA_ERROR "')'û�ж�Ӧ��"  
#define RIGHT_BRA_ERROR_NUM 8  
#define LEFT_INDEX_ERROR "'['û�ж�Ӧ��"  
#define LEFT_INDEX_ERROR_NUM 9  
#define RIGHT_INDEX_ERROR "']'û�ж�Ӧ��"  
#define RIGHT_INDEX_ERROR_NUM 10  
#define L_BOUNDER_ERROR "'{'û�ж�Ӧ��"  
#define L_BOUNDER_ERROR_NUM 11  
#define R_BOUNDER_ERROR "'}'û�ж�Ӧ��"  
#define R_BOUNDER_ERROR_NUM 12  
#define PRE_PROCESS_ERROR "Ԥ��������" //ͷ�ļ����ߺ궨�����  
#define PRE_PROCESS_ERROR_NUM  13  
  
#define _NULL "��"  
  
#define DESCRIBE 4000  
#define TYPE 4001  
#define STRING 4002  
#define DIGIT 4003  
  
struct NormalNode  
{  
    char content[30];//����  
    char kind[30];//����  
    int type;//�ֱ���  
    int addr;//��ڵ�ַ  
    int line;//��������  
    NormalNode * next;//��һ���ڵ�  
};  
  
void initKeyMapping();  
void initOperMapping();  
void initLimitMapping();  
  
void initNode();  
void createNewNode(const char * content,const char *descirbe,int type,int addr,int line);  
void createNewError(const char * content,const char *descirbe,int type,int line);  
int createNewIden(char * content,const char *descirbe,int type,int addr,int line);  
void printNodeLink();  
void printErrorLink();  
void printIdentLink();  
int mystrlen(char * word);  
void preProcess(char * word,int line);  
void close();
int seekKey(char * word);  
void scanner();  
void BraMappingError();  
  
  
#endif 