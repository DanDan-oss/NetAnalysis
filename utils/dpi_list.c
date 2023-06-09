#include "dpi_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 创建一个链表
proto_list_t* proto_list_create()
{
    proto_list_t* new_list = malloc(sizeof(proto_list_t));
    proto_node_t* node_head = malloc(sizeof(proto_node_t));

    if(NULL == new_list || NULL == node_head)
    {
        if(node_head)
            free(node_head);
        if(new_list)
            free(new_list);
        return NULL;
    }
    memset(new_list, 0, sizeof(proto_list_t));
    memset(node_head, 0, sizeof(proto_node_t));
    
    new_list->head = node_head;
    node_head->Back =  node_head->Next = node_head;
    return new_list;
}

void proto_list_delete(proto_list_t* list)
{
    proto_node_t* node_head = NULL;
    proto_node_t* node_last = NULL;

    if(NULL == list->head || NULL == list)
    {
   	    if(list)
            free(list);
        return;
    }

    node_head = list->head; 
    node_last = node_head->Next;
    // 从头节点的下一个节点开始遍历,循环到回到头节点或者节点地址NULL
    while (node_last != node_head && NULL != node_last)
    {
        proto_node_t* temp = node_last;
        node_last = node_last->Next;
        if(temp->data)
            free(temp->data);
        free(temp);
    }
    // 释放头节点和链表结构
    if(node_head->data)
        free(node_head->data);
    free(node_head);
    list->head = NULL;
    free(list);
    return;
}

// 添加一个新数据到链表中(尾插)
int proto_list_addNode(proto_list_t* list, void* data)
{
    proto_node_t* new_node = NULL;

    if(NULL == list->head || NULL == list || NULL == data )
        return 0;
    
    new_node = malloc(sizeof(proto_node_t));
    if(NULL == new_node)
        return 0;

    memset(new_node, 0, sizeof(proto_node_t));
    new_node->data = data;
    
    // 新节点的前一个节点=头节点的前一个节点, 新节点的后一个节点=头节点
    new_node->Back = list->head->Back;
    new_node->Next = list->head;
    
    // 新节点的前节点的后节点=新节点, 新节点的后节点(头节点)的前节点=新节点
    new_node->Back->Next = new_node;
    new_node->Next->Back = new_node;

    list->node_count++;
    return list->node_count;
}

// 移除某一个节点
int proto_list_delNode(proto_list_t* list, void* data)
{
    proto_node_t* node_head = NULL;
    proto_node_t* node_last = NULL; 
    if(NULL == list || NULL == data)
        return 0;

    node_head = list->head;
    node_last = node_head->Next;

    while (node_last != node_head && NULL != node_last )
    {
	    if(node_last->data != data)
        {
            node_last = node_last->Next;
            continue;
        }
            
        node_last->Back->Next = node_last->Next;
        node_last->Next->Back = node_last->Back; 

	    if(node_last->data)
            free(node_last->data);
        free((node_last));
        list->node_count--;
        return list->node_count;
    }
    return 0;
}

int proto_list_delNode_compar(proto_list_t* list, compar_node_callback compar_callback, void* data)
{
    proto_node_t* node_head = NULL;
    proto_node_t* node_last = NULL; 
    if(NULL == list || NULL == data || NULL == compar_callback)
        return 0;

    node_head = list->head;
    node_last = node_head->Next;

    while (node_last != node_head && NULL != node_last )
    {
	    if(0 != compar_callback(node_last->data, data))
        {
            node_last = node_last->Next;
            continue;
        }
            
        node_last->Back->Next = node_last->Next;
        node_last->Next->Back = node_last->Back; 

	    if(node_last->data)
            free(node_last->data);
        free((node_last));
        list->node_count--;
        return list->node_count;
    }
    return 0;
}

void* proto_list_findNode_compar(proto_list_t* list, compar_node_callback call_back, void* data)
{
    proto_node_t* node_head = NULL;
    proto_node_t* node_last = NULL; 
    if(NULL == list || NULL == data || NULL == call_back)
        return 0;

    node_head = list->head;
    node_last = node_head->Next;

    while (node_last != node_head && NULL != node_last )
    {
	    if(0 != call_back(node_last->data, data))
        {
            node_last = node_last->Next;
            continue;
        }
        return node_last;
    }
    return NULL;
}

void ProtoListPrint(proto_list_t* list, list_print_callback callback)
{
    proto_node_t* node_head = NULL;
    proto_node_t* node_last = NULL;

    if(NULL == list->head || NULL == list)
        return;
    
    node_head = list->head;
    node_last = node_head->Next;
    while (node_head != node_last && NULL != node_last)
    {
        proto_node_t* temp = node_last;
        node_last = node_last->Next;

        if(NULL == temp->data)
            continue;

        callback(temp->data);
    }
    return ;
}



//===================================================
// 测试
void print_test(void* node)
{
    int* str = (int*)node;
    printf("straddr:%p, value:%d\n", str, *str);    

}

void ProtoDubgPrint()
{
	// 测试链表
	int* arr[10] = {0};
	for(int i=0; i<10; ++i)
	{
		arr[i] = malloc(sizeof(int)*10);
		arr[i][0] = i+1;
	}

	proto_list_t* list = proto_list_create();
	for(int i=0; i<10; ++i)
		proto_list_addNode(list, arr[i]);
	
	ProtoListPrint(list, print_test);

	proto_list_delNode(list, arr[4]);

	printf("\n\n");
	ProtoListPrint(list, print_test);

	proto_list_delete(list);

    ProtoListPrint(list, print_test);
	printf("哈哈\n");
}

