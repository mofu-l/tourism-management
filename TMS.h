#ifndef _TMS_H_
#define _TMS_H_
#include "memorize.h"

#define BUFLEN 256//������������� 
#define SHOWNUM 64//չʾ�Ĳ�Ʒ���� 
#define MAX_WIDTH 50//��Ʒ��Ϣÿ������ַ� 

enum SCALE {
    WINDOWWIDTH = 26,
    WINDOWHEIGHT = 16
};

enum USERSTATE {
    USER = 0, //�û�״̬
    OFFICIAL = 1 //�̼�״̬
};

enum PICTURE{
	PICTUREW=3,
	PICTUREH=3
};

enum SYSTEMSTATE {
    LOGIN = 1, //��¼����״̬
    HOME = 2, //��ҳ״̬
    DETAIL = 3 ,//��Ʒ����״̬
    ADD = 4,
    EDIT = 5
};

struct productframe {
    int selected;
    Product node;
} ;
typedef struct productframe ProductFrame;//��Ʒչʾ��

void InitProgram(void);//��ʼ������ 
void MouseEventProcess(int x, int y, int button, int event);
void KeyboardEventProcess(int key, int event);
void CharEventProcess(char ch);
void Display(void);
void DisplayLogIn(void);//��¼����
void DisplayHome(void);//��ҳ
void DisplayDetail(void);//��������ҳ��
void DisplayAdd(struct product* node);//���������Ʒ���� 
void TextOutput(char *text,double x,double y);//�ı����
Product searchByNameFuzzy( char *buf);//������������
int IsSimilar(char *name, char *buf);//�ȽϽڵ�
void DrawProductBox(Product linkedlist);//���Ʋ�Ʒ��
void DeleteProduct(void);//ɾ����Ʒ
void AddProduct(Product* node);//�����Ʒ
void EditProduct(void);//�޸���Ʒ��Ϣ 
void SortProductListByName(struct product** linkedlist);//����������
void SortProductListByDate(struct product** linkedlist);//���������� 
int CompareProducts(char* a, char* b); 

#endif
