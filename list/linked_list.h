#ifndef _LINKEDLIST_
#define _LINKEDLIST_

// 노드 정의, data와 포인터(링크)로 되어있다.
typedef struct ListNodeType
{
	int data;
	struct ListNodeType* pLink;
} ListNode;

// 리스트 정의, 헤더 노드와 총 길이로 되어있다.
typedef struct LinkedListType
{
	int currentElementCount;	// ���� ����� ������ ����
	ListNode headerNode;		// ��� ���(Header Node)
} LinkedList;

// 다항식을 항 하나를 polynode로 나타낸 형태. 
// 다항식의 항을 동적으로 추가하거나 제거하기 용이해서...? 연산 등이 용이하다는데.
typedef struct PolyNode
{
	int coef;
	int degree;
	struct ListNodeType* pLink;
} PolyNode;

// 완전히 동일한 것같아서 넘어감.
// typedef struct LinkedListType
// {
// 	int currentElementCount;	// ���� ����� ������ ����
// 	ListNode headerNode;		// ��� ���(Header Node)
// } LinkedList;


LinkedList* createLinkedList();
int addLLElement(LinkedList* pList, int position, ListNode element);
int removeLLElement(LinkedList* pList, int position);
ListNode* getLLElement(LinkedList* pList, int position);

void clearLinkedList(LinkedList* pList);
int getLinkedListLength(LinkedList* pList);
void deleteLinkedList(LinkedList* pList);
#endif

#ifndef _COMMON_LIST_DEF_
#define _COMMON_LIST_DEF_

#define TRUE		1
#define FALSE		0

#endif