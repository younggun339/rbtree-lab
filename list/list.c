#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 리스트노드는 자신의 자료형을 갖는 next의 주소와, data값 정수형을 갖는다.
typedef struct listNode
{
    struct listNode *next;
    int data;
    
}listNode;

listNode *init_list()
{
    // 처음에 listNode형 의 head라는 이름의 포인터를 만들고
    listNode *head;
    // 그 포인터의 크기는 listNode의 크기만큼 malloc으로 동적 할당하되, 자료형은 listNode형으로.
    head = (listNode *)malloc(sizeof(listNode));
    // 그리고 head 포인터의 next 속성은 NULL로 설정.
    head->next = NULL;
    // 실제 init 사용법은 listNode *myList = init_list(); 등으로 

    return (head);

}

// listNode *find_end(listNode *h){

// }

// int node_len(listNode *h){

// }

// listNode *read_node_idx(listNode *h, int num){

// }

// listNode *read_node_data(listNode *h, int data){


// }

// void add_first_node(listNode *h, int data){}

// void add_last_node(listNode *h, int data){}

// void insert_node(listNode *h, int n, int data){}

// void edit_node(listNode *h, int search, int modify){

// }

// void delete_first_node(listNode *h){}

// void delete_node(listNode *h, int n){}

// void delete_all_node(listNode *h){

// }

// void print_all_node(listNode *h){


// }