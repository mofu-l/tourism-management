#include <string.h>
#include <stdlib.h>
#include "memorize.h"
#include "linkedlist.h"
#include "boolean.h"
#include <stdio.h>

Product head = NULL;
extern int productnum;

int CmpData(Product ptr1, Product ptr2) {
    return (!strcmp(ptr1->name, ptr2->name)) && (!strcmp(ptr1->date, ptr2->date)) && (!strcmp(ptr1->information, ptr2->information)) && (ptr1->price == ptr2->price);
}

void AddDada(Product head, Product node) {
    Product curr = head;
    if(node == NULL) {
        return;
    }
    if(head == NULL) {
        head = node;
        return;
    }
    while(curr->next) {
        curr = curr->next;
    }
    curr->next = node;
    node->next = NULL;
}

void DeleteData(Product head, Product node) {
    Product curr = head;
    if((head == NULL) || (node == NULL)) {
        return;
    }
    while(curr->next) {
        if(CmpData(curr->next, node)) {
            curr->next = node->next;
            free(node);
            node = NULL;
        }
    }
}


void ReadData(FILE *fp) {
    Product node, curr;
    if(head != NULL) {
        return;
    }
    while (1) {
        if ((node = (Product)malloc(sizeof(struct product))) == NULL) {
            // 处理内存分配失败的情况 
            fprintf(stderr, "Memory allocation failed.\n");
            return;
        }
        if(feof(fp)) {
            free(node);
            return;
        }
        if (fscanf(fp, "%s %s %s %lf %d", node->name, node->date, node->information, &node->price, &node->reserved) != 5) {
            // 处理文件读取错误 
            fprintf(stderr, "File read error.\n");
            free(node); // 释放分配的内存 
            return;
        }
        node->next = NULL;
        if (head == NULL) {
            head = node;
            curr = node;
        } else {
            curr->next = node;
            curr = node;
        }
        productnum++;
    }
}

void WriteToFile(struct product** linkedlist) {
    if((*linkedlist) == NULL) {
        return;
    }
    FILE* file = fopen("product.txt", "w");
    if (file == NULL) {
        printf("Error opening file!");
        return;
    }
    struct product* current = *linkedlist;
    while (current != NULL) {
        fprintf(file, "%s %s %s %.2f %d\n", current->name, current->date, current->information, current->price, current->reserved);
        current = current->next;
    }
    fclose(file);
}



