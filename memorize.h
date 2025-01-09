#include <stdio.h>
#ifndef _MEMORIZE_H_
#define _MEMORIZE_H_

#define MAXN 256//��Ʒ��Ϣ��󳤶� 
enum SHOWNPRODUCT {
    RESERVED = 1,
    UNRESERVED = 0
};

struct product {
    char name[MAXN];
    char date[MAXN];
    char information[MAXN];
    double price;
    int reserved;
    struct product* next;
} product;

typedef struct product *Product;

void AddDada(Product head, Product node); //��Ӳ�Ʒ
void DeleteData(Product head, Product node); //ɾ����Ʒ
int CmpData(Product ptr1, Product ptr2); //�жϲ�Ʒ�Ƿ���ͬ
void ReadData(FILE *fp);//���ļ��ж�ȡ��Ʒ����
void WriteToFile(struct product** linkedlist);//�������ݵ����� 
#endif
