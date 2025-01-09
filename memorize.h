#include <stdio.h>
#ifndef _MEMORIZE_H_
#define _MEMORIZE_H_

#define MAXN 256//产品信息最大长度 
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

void AddDada(Product head, Product node); //添加产品
void DeleteData(Product head, Product node); //删除产品
int CmpData(Product ptr1, Product ptr2); //判断产品是否相同
void ReadData(FILE *fp);//从文件中读取产品数据
void WriteToFile(struct product** linkedlist);//保存数据到本地 
#endif
