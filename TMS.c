#include <stdio.h>
#include <stdlib.h>
#include "extgraph.h"
#include "imgui.h"
#include "TMS.h"
#include "memorize.h"


int SystemState = LOGIN;
int UserState = -1;
ProductFrame isselected[SHOWNUM] ;
extern Product head;
int productnum = 0;
int index = -1;
Product test = NULL;
double Offset = 0; //��Ʒλ��ƫ��ֵ
Product node;//�޸Ĳ�Ʒ�Ļ����� 


// �Ƚ�����product��name�ֶ�
int CompareProducts(char* a, char* b) {
    return strcmp(a, b);
}

void SortProductListByName(struct product** linkedlist) {
    if (linkedlist == NULL || *linkedlist == NULL) {
        return;
    }

    struct product* sorted = NULL;
    struct product* current = *linkedlist;

    while (current != NULL) {
        struct product* next = current->next;
        struct product* prev = NULL;
        struct product* currentSorted = sorted;

        // ���Ҳ���λ��
        while (currentSorted != NULL && CompareProducts(currentSorted->name, current->name) < 0) {
            prev = currentSorted;
            currentSorted = currentSorted->next;
        }

        current->next = currentSorted;
        if (prev == NULL) {
            sorted = current;
        } else {
            prev->next = current;
        }

        current = next;
    }

    *linkedlist = sorted; // ����ͷ�ڵ�
    WriteToFile(&head);
}

void SortProductListByDate(struct product** linkedlist) {
    if (linkedlist == NULL || *linkedlist == NULL) {
        return;
    }

    struct product* sorted = NULL;
    struct product* current = *linkedlist;

    while (current != NULL) {
        struct product* next = current->next;
        struct product* prev = NULL;
        struct product* currentSorted = sorted;

        // ���Ҳ���λ��
        while (currentSorted != NULL && CompareProducts(currentSorted->date, current->date) < 0) {
            prev = currentSorted;
            currentSorted = currentSorted->next;
        }

        current->next = currentSorted;
        if (prev == NULL) {
            sorted = current;
        } else {
            prev->next = current;
        }

        current = next;
    }

    *linkedlist = sorted; // ����ͷ�ڵ�
    WriteToFile(&head);
}


void TextOutput(char *text, double x, double y) {
    DrawTextString(text);
}

int IsSimilar(char *name, char *buf) {
    return !(strstr(name, buf) == 0);
}

void DeleteProduct(void) {
    if(head == NULL) {
        index = -1;
        return;
    }
    if(index == -1) {
        return;
    }
    if(index == 0) {
        head = isselected[index + 1].node;
        free(isselected[index].node);
    } else {
        isselected[index - 1].node->next = isselected[index].node->next;
        free(isselected[index].node);
    }
    productnum--;
    index = -1;
}

void MoveSimilarNodes(struct product **head, char *buf) {
    struct product *prev = NULL;
    struct product *curr = *head;

    while (curr) {
        if (IsSimilar(curr->name, buf)) {
            if (prev) {
                prev->next = curr->next;
                curr->next = *head;
                *head = curr;
                curr = prev->next;
            } else {
                curr = curr->next;
            }
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void DrawProductBox(Product linkedlist) {
    if(linkedlist != NULL) {
        Product node = linkedlist;
        double BoxW = 0.15 * WINDOWWIDTH;
        double BoxH = 0.1 * WINDOWHEIGHT;
        double BoxGapX = 0.08 * WINDOWWIDTH;
        double BoxGapY = 0.05 * WINDOWHEIGHT;
        char PenColor[MAXN] = "Black";
        int count = 0;
        int ProductNumX, ProductNumY;
        for(ProductNumY = 1; ProductNumY <= ((productnum < SHOWNUM ? productnum : SHOWNUM) - 1) / 4 + 1; ProductNumY++) {
            for(ProductNumX = 1; ProductNumX <= 4; ProductNumX++) {
                if(count >= productnum) {
                    break;
                }
                isselected[ProductNumX - 1 + 4 * (ProductNumY - 1)].node = node;
                if(isselected[ProductNumX - 1 + 4 * (ProductNumY - 1)].selected) {
                    strcpy(PenColor, "Red");
                    SetPenColor("Red");
                } else {
                    strcpy(PenColor, "Black");
                    SetPenColor("Black");
                }
                drawBox(ProductNumX * BoxGapX + (ProductNumX - 1)*BoxW, 0.75 * WINDOWHEIGHT - ProductNumY * (BoxH + BoxGapY) + Offset,
                        BoxW, BoxH, 0, node->name, "M", PenColor);
                count++;
                node = node->next;
            }
        }

    }
}

void AddProduct(Product* node) {
    Product curr = head;
    if(head == NULL) {
        return;
    }
    while((curr->next) != NULL) {
        curr = curr->next;
    }
    curr->next = (*node);
    productnum++;
}

void EditProduct(void) {
    double fontAscent  = GetFontAscent();
    double TextBoxH = fontAscent * 3;


    SetPenColor("Black");
    char *str1 = "��Ʒ���ƣ�" ;
    double tw1 = TextStringWidth(str1);
    static char EditTextbuf1[MAXN] = {};
    strcpy(EditTextbuf1, node->name );
    double TextBoxW1 = 0.6 * WINDOWWIDTH;
    double TextBoxX1 = WINDOWWIDTH * 0.1 + tw1;
    double TextBoxY1 = WINDOWHEIGHT * 0.85 - TextBoxH / 2;
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.85 - fontAscent / 2);
    DrawTextString(str1);
    if( textbox(GenUIID(0), TextBoxX1, TextBoxY1, TextBoxW1, TextBoxH, EditTextbuf1, BUFLEN) ) {
        strcpy(node->name, EditTextbuf1);

    }


    SetPenColor("Black");
    char *str2 = "���ڣ�";
    double tw2 = TextStringWidth(str2);
    static char EditTextbuf2[MAXN] = {};
    strcpy(EditTextbuf2, node->date );
    double TextBoxW2 = 0.3 * WINDOWWIDTH;
    double TextBoxX2 = WINDOWWIDTH * 0.1 + tw1;
    double TextBoxY2 = WINDOWHEIGHT * 0.75 - TextBoxH / 2;
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.75 - fontAscent / 2);
    DrawTextString(str2);
    if( textbox(GenUIID(0), TextBoxX2, TextBoxY2, TextBoxW2, TextBoxH, EditTextbuf2, BUFLEN) ) {
        strcpy(node->date, EditTextbuf2);

    }


    SetPenColor("Black");
    char *str3 = "��Ʒ�۸�";
    double tw3 = TextStringWidth(str3);
    static char EditTextbuf3[BUFLEN + 1] = {};
    static char Price[BUFLEN + 1];
    double TextBoxW3 = 0.3 * WINDOWWIDTH;
    double TextBoxX3 = WINDOWWIDTH * 0.5 + tw3;
    double TextBoxY3 = WINDOWHEIGHT * 0.75 - TextBoxH / 2;
    static char Num[BUFLEN + 1];
    static char *num;
    MovePen(WINDOWWIDTH * 0.5, WINDOWHEIGHT * 0.75 - fontAscent / 2);
    DrawTextString(str3);
    if( textbox(GenUIID(0), TextBoxX3, TextBoxY3, TextBoxW3, TextBoxH, EditTextbuf3, BUFLEN) ) {
        
        node->price = atof(EditTextbuf3);
    }

    SetPenColor("Black");
    char *str4 = "��Ʒ���飺";
    double tw4 = TextStringWidth(str4);
    static char EditTextbuf4[MAXN] = {};
    strcpy(EditTextbuf4, node->information );
    double TextBoxW4 = 0.7 * WINDOWWIDTH;
    double TextBoxX4 = WINDOWWIDTH * 0.1 + tw3;
    double TextBoxY4 = WINDOWHEIGHT * 0.65 - TextBoxH / 2;
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.65 - fontAscent / 2);
    DrawTextString(str4);
    double InformationH = TextBoxH;
    if( textbox(GenUIID(0), TextBoxX4, TextBoxY4, TextBoxW4, InformationH, EditTextbuf4, BUFLEN) ) {
        strcpy(node->information, EditTextbuf4);

    }

    double ButtonW = WINDOWWIDTH * 0.1;
    double ButtonH = WINDOWHEIGHT * 0.1;
    double EditX = WINDOWWIDTH * 0.5 - ButtonW / 2;
    double EditY = WINDOWHEIGHT * 0.15 - ButtonH / 2;
    if(button(GenUIID(0), EditX, EditY, ButtonW, ButtonH, "ȷ��")) {
        if((*(node->name)!= '\0') && (*(node->name) != '\0') && (*(node->information) != '\0')) {
            strcpy(isselected[index].node->name, node->name);
            strcpy(isselected[index].node->date, node->date);
            isselected[index].node->price = node->price;
            strcpy(isselected[index].node->information, node->information);
            WriteToFile(&head);
        }
    }

    if( button(GenUIID(0), 0, WINDOWHEIGHT * 0.9, WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.1, "����") ) {
        SystemState = HOME;
        strcpy(EditTextbuf1, "");
        strcpy(EditTextbuf2, "");
        strcpy(EditTextbuf3, "");
        strcpy(EditTextbuf4, "");
        index = -1;
        free(node);
        return;
    }
}



void CharEventProcess(char ch) {
    uiGetChar(ch);
    Display();
}

void KeyBoardEventProcess(int key, int event) {
    uiGetKeyboard(key,  event);
    Display();
}

void MouseEventProcess(int x, int y, int button, int event) {
    uiGetMouse(x, y, button, event);
    //�ж���Ʒѡ��״̬
    int i;
    double xx = ScaleXInches(x);
    double yy = ScaleYInches(y);
    double slip = Offset - (WINDOWHEIGHT * 0.15) * (Offset / (WINDOWHEIGHT * 0.15));
    if(SystemState == HOME && event == BUTTON_DOWN && button == LEFT_BUTTON) {


        double BoxW = 0.15 * WINDOWWIDTH;
        double BoxH = 0.1 * WINDOWHEIGHT;
        double BoxGapX = 0.08 * WINDOWWIDTH;
        double BoxGapY = 0.05 * WINDOWHEIGHT;
        for(i = 0; i < (productnum < SHOWNUM ? productnum : SHOWNUM); i++) {
            if((xx <= (BoxGapX + BoxW) * (i % 4 + 1)) && (xx >= (BoxGapX + BoxW) * (i % 4 + 1) - BoxW) &&
                    (yy >= 0.75 * WINDOWHEIGHT - (BoxGapY + BoxH) * (i / 4 + 1) + Offset) && (yy <= 0.75 * WINDOWHEIGHT - (BoxGapY + BoxH) * (i / 4 + 1) + BoxH + Offset) && (yy < 0.75 * WINDOWHEIGHT) && (yy > 0.75 * WINDOWHEIGHT - (BoxGapY + BoxH) * 3)) {
                isselected[i].selected = isselected[i].selected ? 0 : 1;
                index = i;
                continue;
            } else if(UserState == OFFICIAL) {
                if(((x > WINDOWWIDTH * 0.45) && (x < WINDOWWIDTH * 0.55) && (y > WINDOWHEIGHT * 0.1) && (y < WINDOWHEIGHT * 0.2)) &&
                        (((x > WINDOWWIDTH * 0.25) && (x < WINDOWWIDTH * 0.35) && (y > WINDOWHEIGHT * 0.1) && (y < WINDOWHEIGHT * 0.2) && (UserState == OFFICIAL))) &&
                        (((x > WINDOWWIDTH * 0.65) && (x < WINDOWWIDTH * 0.75) && (y > WINDOWHEIGHT * 0.1) && (y < WINDOWHEIGHT * 0.2) && (UserState == OFFICIAL))))
                    index = index;
            } else if(UserState == USER) {
                if((x > WINDOWWIDTH * 0.45) && (x < WINDOWWIDTH * 0.55) && (y > WINDOWHEIGHT * 0.1) && (y < WINDOWHEIGHT * 0.2))
                    index = -1;
            } else {
                index = -1;
            }
            //�����Ʒʱ��ȡ�������Ʒ��ѡ��״̬�������������ȡ�����е�ѡ��״̬
            if(isselected[i].selected == 1) {
                isselected[i].selected = 0;
            }
        }
    }
    //�����ж�
    if(SystemState == HOME && ((yy < 0.75 * WINDOWHEIGHT) && (yy > 0.25 * WINDOWHEIGHT )) && (productnum > 12)) {
        if(event == ROLL_UP) {
            if((Offset >= WINDOWHEIGHT * 0.05) && (Offset <= (WINDOWHEIGHT * 0.15) * ((productnum - 1) / 4 + 1)))
                Offset -= WINDOWHEIGHT * 0.05;
        } else if(event == ROLL_DOWN) {
            if((Offset >= 0) && (Offset <= (WINDOWHEIGHT * 0.15) * ((productnum - 1) / 4 - 1.5) - WINDOWHEIGHT * 0.05))
                Offset += WINDOWHEIGHT * 0.05;
        }
        printf("%f\n", Offset);
    }
    Display();
}

void DisplayLogIn(void) {
    if(SystemState != LOGIN) {
        return;
    }//ȷ�ϵ�¼״̬

    double LOGINUSERX = 3.5;
    double LOGINUSERY = 6.5;
    double LOGINOFFIX = 16.5;
    double LOGINOFFIY = 6.5;
    double LOGINW = 7;
    double LOGINH = 3;

    if( button(GenUIID(0), LOGINUSERX, LOGINUSERY, LOGINW, LOGINH, "�û�") ) {
        SystemState = HOME;
        UserState = USER;
        return;
    }
    if( button(GenUIID(0), LOGINOFFIX, LOGINOFFIY, LOGINW, LOGINH, "�̼�") ) {
        SystemState = HOME;
        UserState = OFFICIAL;
        return;
    }
}

void DisplayHome(void) {
    DisplayClear();
    //��Ʒչʾ
    DrawProductBox(head);

    //���η�Χ��Ĳ�Ʒ
    SetPenColor("White");
    drawRectangle(0, WINDOWHEIGHT * 0.75, WINDOWWIDTH, WINDOWHEIGHT * 0.25, 1);
    drawRectangle(0, 0, WINDOWWIDTH, WINDOWHEIGHT * 0.25, 1);

    //���ƹ�����
    SetPenColor("Black");
    double ScrollW = WINDOWWIDTH * 0.01;
    double ScrollH = WINDOWHEIGHT * 0.5 * WINDOWHEIGHT * 0.5 / ((WINDOWHEIGHT * 0.15) * ((productnum - 1) / 4 + 1) + WINDOWHEIGHT * 0.05);
    drawRectangle(WINDOWWIDTH * 0.99, -Offset * WINDOWHEIGHT * 0.5 / ((WINDOWHEIGHT * 0.15) * ((productnum - 1) / 4 + 1) + WINDOWHEIGHT * 0.05) + WINDOWHEIGHT * 0.75 - ScrollH , ScrollW, ScrollH , 1);

    static char * menuListStyle[] = {"����",
                                     "б��",
                                     "��������",
                                     "�Ӵ�"
                                    };
    static char * menuListState[] = {"״̬",
                                     "����"
                                    };
    static char * menuListSort[] = {"����",
                                    "����������",
                                    "����������"
                                   };
    static char * selectedLabel = NULL;
    double ww = GetWindowWidth();
    double wh = GetWindowHeight();
    double fH = GetFontHeight();
    double h = fH * 1.5; // �ؼ��߶�
    double w = TextStringWidth(menuListStyle[2]) * 2; // �ؼ����
    double wlist = TextStringWidth(menuListStyle[2]) * 1.2;
    double xindent = wh / 20; // ����
    int    selection;

    //����˵�
    double StyleX = WINDOWWIDTH * 0.3;
    double StyleY = wh;

    drawMenuBar(0, StyleY - h, ww , h);
    selection = menuList(GenUIID(0), 0,  StyleY - h, w, wlist, h, menuListStyle, sizeof(menuListStyle) / sizeof(menuListStyle[0]));
    if( selection == 1 )
        SetStyle(2);
    if(selection == 2)
        SetStyle(0);
    if(selection == 3)
        SetStyle(1);

    //���ز˵�
    double StateX = w;
    double StateY = wh;
    selection = menuList(GenUIID(0), StateX, StateY - h, w, wlist, h, menuListState, sizeof(menuListState) / sizeof(menuListState[0]));
    if( selection == 1 ) {
        SystemState = LOGIN;
        UserState = -1;
        Offset = 0;
        return;
    }

    //����˵�
    double SortW = TextStringWidth(menuListSort[2]) * 2;
    double SortX = SortW;
    double SortY = wh;
    double Sortwlist = TextStringWidth(menuListSort[1]) * 1.2;
    selection = menuList(GenUIID(0), StateX + w, SortY - h, SortW, Sortwlist, h, menuListSort, sizeof(menuListSort) / sizeof(menuListSort[0]));
    if( selection == 1 )
        SortProductListByName(&head);
    if( selection == 2 )
        SortProductListByDate(&head);

    FILE *fp;
    fp = fopen("product.txt", "r");
    ReadData(fp);
    fclose(fp);
    fp = NULL;

    //������
    static char Textbuf[BUFLEN + 1];
    double TextBoxW = 0.6 * WINDOWWIDTH;
    double TextBoxH = WINDOWHEIGHT / 10;
    double TextBoxX = WINDOWWIDTH / 2 - TextBoxW / 2;
    double TextBoxY = WINDOWHEIGHT * 0.8 - TextBoxH / 2;
    static char buf[BUFLEN + 1] = {};
    int i;

    if( textbox(GenUIID(0), TextBoxX, TextBoxY, TextBoxW, TextBoxH, Textbuf, BUFLEN) ) {
        strcpy(buf, Textbuf);
    }
    if(button(GenUIID(0), WINDOWWIDTH * 0.85, TextBoxY, WINDOWWIDTH * 0.1, TextBoxH, "����")) {
        MoveSimilarNodes(&head, buf);
    }


    double ButtonW = WINDOWWIDTH * 0.1;
    double ButtonH = WINDOWHEIGHT * 0.1;
    //�û���ҳ
    if(UserState == USER) {
        //�鿴��ϸ��Ϣ
        double ViewX = WINDOWWIDTH / 2 - ButtonW / 2;
        double ViewY = WINDOWHEIGHT * 0.15 - ButtonH / 2;

        if(button(GenUIID(0), ViewX, ViewY, ButtonW, ButtonH, "�鿴��ϸ��Ϣ")) {
            if(index >= 0) {
                SystemState = DETAIL;
                DisplayClear();
            }
        }
    }

    //�̼���ҳ
    if(UserState == OFFICIAL) {
        //ɾ����Ʒ
        double DeleteX = WINDOWWIDTH / 2 - ButtonW / 2;
        double DeleteY = WINDOWHEIGHT * 0.15 - ButtonH / 2;
        if(button(GenUIID(0), DeleteX, DeleteY, ButtonW, ButtonH, "ɾ����Ʒ")) {
            if(index >= 0) {
                DeleteProduct();
                WriteToFile(&head);
            }
            index = -1;
        }

        //�����Ʒ
        double AddX = WINDOWWIDTH * 0.3 - ButtonW / 2;
        double AddY = WINDOWHEIGHT * 0.15 - ButtonH / 2;
        if(button(GenUIID(0), AddX, AddY, ButtonW, ButtonH, "�����Ʒ")) {
            SystemState = ADD;
            DisplayClear();
            Product node;
            if((node = (struct product*)malloc(sizeof(struct product))) == NULL) {
                exit(1);
            }
            node->next = NULL;
            node->reserved = 0;
            test = node;
            index = -1;
        }

        //�޸���Ʒ��Ϣ
        double EditX = WINDOWWIDTH * 0.7 - ButtonW / 2;
        double EditY = WINDOWHEIGHT * 0.15 - ButtonH / 2;
        if(button(GenUIID(0), EditX, EditY, ButtonW, ButtonH, "�޸���Ʒ��Ϣ")) {
            if(index >= 0) {

                if((node = (struct product*)malloc(sizeof(struct product))) == NULL) {
                    return;
                }
                strcpy(node->name, isselected[index].node->name);
                strcpy(node->date, isselected[index].node->date);
                strcpy(node->information, isselected[index].node->information);
                node->price = isselected[index].node->price;
                SystemState = EDIT;
                DisplayClear();
            }
        }
    }
}



void DisplayDetail(void) {

    char *str = "��Ʒ���ƣ�" ;
    char *str1 = "��Ԥ��";
    char *str2 = "δԤ��";
    char *str3 = "�۸�";
    char *str4 = "��ϸ��Ϣ��";
    static char *DetailColor = "Black";
    char *colors[] = {"Red",    "Green",  "Blue",    "Brown", "Orange",
                      "Yellow", "Violet", "Magenta", "Cyan", "Amber Yellow", "DeepSkyBlue"
                     };

    //�˵�
    static char * menuListColor[] = {"������ɫ",
                                     "������",
                                     "�����",
                                     "������ɫ",
                                     "��ɫ" ,
                                     "���ɫ",
                                     "��ɫ"
                                    };
    static char * menuListState[] = {"״̬",
                                     "����"
                                    };
    static char * DetailselectedLabel = NULL;
    double ww = GetWindowWidth();
    double wh = GetWindowHeight();
    double fH = GetFontHeight();
    double h = fH * 1.5; // �ؼ��߶�
    double w = TextStringWidth(menuListColor[3]) * 2; // �ؼ����
    double wlist = TextStringWidth(menuListColor[3]) * 1.2;
    double xindent = wh / 20; // ����
    int    selection;
    //��ɫ�˵�
    double ColorX = WINDOWWIDTH * 0.3;
    double ColorY = wh;

    drawMenuBar(0, ColorY - h, ww , h);
    selection = menuList(GenUIID(0), 0,  ColorY - h, w, wlist, h, menuListColor, sizeof(menuListColor) / sizeof(menuListColor[0]));
    if( selection == 1 )
        DetailColor = "DeepSkyBlue";
    if(selection == 2)
        DetailColor = "Amber Yellow";
    if(selection == 3)
        DetailColor = "Violet";
    if(selection == 4)
        DetailColor = "Cyan";
    if(selection == 5)
        DetailColor = "Magenta";
    if(selection == 6)
        DetailColor = "Black";

    //״̬�˵�
    double StateX = w;
    double StateY = wh;
    selection = menuList(GenUIID(0), StateX, StateY - h, w, wlist, h, menuListState, sizeof(menuListState) / sizeof(menuListState[0]));
    if( selection == 1 ) {
        SystemState = HOME;
        WriteToFile(&head);
        index = -1;
        return;
    }

    double fontAscent  = GetFontAscent();
    double tw = TextStringWidth(str);
    //����
    SetPenColor(DetailColor);
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.85 - fontAscent / 2);
    DrawTextString(str);
    MovePen(WINDOWWIDTH * 0.1 + tw, WINDOWHEIGHT * 0.85 - fontAscent / 2);
    DrawTextString(isselected[index].node->name);
    //Ԥ�����
    SetPenColor("Red");
    MovePen(WINDOWWIDTH * 0.6, WINDOWHEIGHT * 0.85 - fontAscent / 2);
    if(isselected[index].node->reserved == RESERVED) {
        DrawTextString(str1);
    } else {
        DrawTextString(str2);
    }
    //��ϸ����
    SetPenColor(DetailColor);
    double tw4 = TextStringWidth(str4);
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.75 - fontAscent / 2);
    DrawTextString(str4);
    MovePen(WINDOWWIDTH * 0.1 + tw4, WINDOWHEIGHT * 0.75 - fontAscent / 2);

    TextOutput(isselected[index].node->information, WINDOWWIDTH * 0.1 + tw4, WINDOWHEIGHT * 0.75 - fontAscent / 2);

    //Ԥ����ȡ��Ԥ����ť
    if(button(GenUIID(0), WINDOWWIDTH * 0.3, WINDOWHEIGHT * 0.05, WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.1, "Ԥ��")) {
        isselected[index].node->reserved = RESERVED;
    }
    if(button(GenUIID(0), WINDOWWIDTH * 0.6, WINDOWHEIGHT * 0.05, WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.1, "ȡ��Ԥ��")) {
        isselected[index].node->reserved = UNRESERVED;
    }
    //�۸�
    char price[BUFLEN + 1];
    double tw3 = TextStringWidth(str3);
    sprintf(price, "%.2f", isselected[index].node->price);
    SetPenColor(DetailColor);
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.8 - fontAscent / 2);
    DrawTextString(str3);
    MovePen(WINDOWWIDTH * 0.1 + tw3, WINDOWHEIGHT * 0.8 - fontAscent / 2);
    DrawTextString(price);
}

void DisplayAdd(struct product* node) {


    double fontAscent  = GetFontAscent();
    double TextBoxH = fontAscent * 3;

    SetPenColor("Black");
    char *str1 = "��Ʒ���ƣ�" ;
    double tw1 = TextStringWidth(str1);
    static char Textbuf1[MAXN] = {};
    double TextBoxW1 = 0.6 * WINDOWWIDTH;
    double TextBoxX1 = WINDOWWIDTH * 0.1 + tw1;
    double TextBoxY1 = WINDOWHEIGHT * 0.85 - TextBoxH / 2;
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.85 - fontAscent / 2);
    DrawTextString(str1);
    if( textbox(GenUIID(0), TextBoxX1, TextBoxY1, TextBoxW1, TextBoxH, Textbuf1, BUFLEN) ) {
        strcpy(node->name, Textbuf1);
    }


    SetPenColor("Black");
    char *str2 = "���ڣ�";
    double tw2 = TextStringWidth(str2);
    static char Textbuf2[MAXN] = {};
    double TextBoxW2 = 0.3 * WINDOWWIDTH;
    double TextBoxX2 = WINDOWWIDTH * 0.1 + tw1;
    double TextBoxY2 = WINDOWHEIGHT * 0.75 - TextBoxH / 2;
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.75 - fontAscent / 2);
    DrawTextString(str2);
    if( textbox(GenUIID(0), TextBoxX2, TextBoxY2, TextBoxW2, TextBoxH, Textbuf2, BUFLEN) ) {
        sprintf(node->date, "%s", Textbuf2);
    }


    SetPenColor("Black");
    char *str3 = "��Ʒ�۸�";
    double tw3 = TextStringWidth(str3);
    static char Textbuf3[BUFLEN + 1] = {};
    double TextBoxW3 = 0.3 * WINDOWWIDTH;
    double TextBoxX3 = WINDOWWIDTH * 0.5 + tw3;
    double TextBoxY3 = WINDOWHEIGHT * 0.75 - TextBoxH / 2;
    const char Num[BUFLEN + 1];
    char *num;
    MovePen(WINDOWWIDTH * 0.5, WINDOWHEIGHT * 0.75 - fontAscent / 2);
    DrawTextString(str3);
    if( textbox(GenUIID(0), TextBoxX3, TextBoxY3, TextBoxW3, TextBoxH, Textbuf3, BUFLEN) ) {
        strcpy(Num, Textbuf3);
        node->price = strtod(Num, &num);
    }

    SetPenColor("Black");
    char *str4 = "��Ʒ���飺";
    double tw4 = TextStringWidth(str4);
    static char Textbuf4[MAXN] = {};
    double TextBoxW4 = 0.7 * WINDOWWIDTH;
    double TextBoxX4 = WINDOWWIDTH * 0.1 + tw3;
    double TextBoxY4 = WINDOWHEIGHT * 0.65 - TextBoxH / 2;
    MovePen(WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.65 - fontAscent / 2);
    DrawTextString(str4);
    if( textbox(GenUIID(0), TextBoxX4, TextBoxY4, TextBoxW4, TextBoxH, Textbuf4, BUFLEN) ) {
        sprintf(node->information , "%s", Textbuf4);
    }
    double ButtonW = WINDOWWIDTH * 0.1;
    double ButtonH = WINDOWHEIGHT * 0.1;
    double AddX = WINDOWWIDTH * 0.5 - ButtonW / 2;
    double AddY = WINDOWHEIGHT * 0.15 - ButtonH / 2;
    if(button(GenUIID(0), AddX, AddY, ButtonW, ButtonH, "�����Ʒ")) {
        if((node->name != NULL) && (node->date != NULL) && (node->information != NULL))
            AddProduct(&node);
    }


    //���ؼ�
    if( button(GenUIID(0), 0, WINDOWHEIGHT * 0.9, WINDOWWIDTH * 0.1, WINDOWHEIGHT * 0.1, "����") ) {
        SystemState = HOME;
        strcpy(Textbuf1, "");
        strcpy(Textbuf2, "");
        strcpy(Textbuf3, "");
        strcpy(Textbuf4, "");
        if((node->name != NULL) && (node->date != NULL) && (node->information != NULL)) {
            WriteToFile(&head);

        } else {
            free(node);
        }

        return;
    }
}

void Display() {
    DisplayClear();
    if((SystemState == LOGIN) && (UserState == -1))
        DisplayLogIn();
    if((SystemState == HOME) && ((UserState == USER) || (UserState == OFFICIAL)))
        DisplayHome();
    if((SystemState == DETAIL) && ((UserState == USER) || (UserState == OFFICIAL)))
        DisplayDetail();
    if((SystemState == ADD) &&  (UserState == OFFICIAL))
        DisplayAdd(test);
    if((SystemState == EDIT) &&  (UserState == OFFICIAL))
        EditProduct();
}

void InitProgram(void) {
    DefineColor("Amber Yellow", 1, .749, 0);
    DefineColor("DeepSkyBlue", 0, .749, 1);
    registerMouseEvent(MouseEventProcess);
    registerKeyboardEvent(KeyBoardEventProcess);
    registerCharEvent(CharEventProcess);
    int ShowNum;
    for(ShowNum = 0; ShowNum < SHOWNUM; ShowNum++) {
        isselected[ShowNum].selected = 0;
        isselected[ShowNum].node = NULL;
    }
}
void Main() {
    SetWindowTitle("Tourism Management System");
    SetWindowSize(WINDOWWIDTH, WINDOWHEIGHT);
    InitGraphics();
    InitProgram();
    Display();
}






