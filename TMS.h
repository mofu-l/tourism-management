#ifndef _TMS_H_
#define _TMS_H_
#include "memorize.h"

#define BUFLEN 256//搜索框最大容量 
#define SHOWNUM 64//展示的产品数量 
#define MAX_WIDTH 50//产品信息每行最多字符 

enum SCALE {
    WINDOWWIDTH = 26,
    WINDOWHEIGHT = 16
};

enum USERSTATE {
    USER = 0, //用户状态
    OFFICIAL = 1 //商家状态
};

enum PICTURE{
	PICTUREW=3,
	PICTUREH=3
};

enum SYSTEMSTATE {
    LOGIN = 1, //登录界面状态
    HOME = 2, //主页状态
    DETAIL = 3 ,//商品详情状态
    ADD = 4,
    EDIT = 5
};

struct productframe {
    int selected;
    Product node;
} ;
typedef struct productframe ProductFrame;//产品展示框

void InitProgram(void);//初始化程序 
void MouseEventProcess(int x, int y, int button, int event);
void KeyboardEventProcess(int key, int event);
void CharEventProcess(char ch);
void Display(void);
void DisplayLogIn(void);//登录界面
void DisplayHome(void);//主页
void DisplayDetail(void);//绘制详情页面
void DisplayAdd(struct product* node);//绘制添加商品界面 
void TextOutput(char *text,double x,double y);//文本输出
Product searchByNameFuzzy( char *buf);//根据名字搜索
int IsSimilar(char *name, char *buf);//比较节点
void DrawProductBox(Product linkedlist);//绘制产品框
void DeleteProduct(void);//删除商品
void AddProduct(Product* node);//添加商品
void EditProduct(void);//修改商品信息 
void SortProductListByName(struct product** linkedlist);//按名字排序
void SortProductListByDate(struct product** linkedlist);//按日期排序 
int CompareProducts(char* a, char* b); 

#endif
