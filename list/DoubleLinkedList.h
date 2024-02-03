typedef int LData;

typedef struct _dNode {
    LData data;           // 노드의 데이터
    struct _dNode * prev; // 자기 참조 포인터. 이전 노드를 가리킵니다.
    struct _dNode * next; // 자기 참조 포인터. 다음 노드를 가리킵니다.
} Node;

// LinkedList 구조체 선언
typedef struct _double_linked_list {
    Node * head;    // 연결 리스트의 헤더
    Node * tail;    // 연결 리스트의 꼬리
    int size;       // 연결 리스트의 크기
} DoubleLinkedList;

// LinkedList List로 타입 재지정
typedef DoubleLinkedList List;

void LInit(List * pList);

// - 리스트를 초기화 합니다.
// - 리스트 생성 시 제일 먼저 호출됩니다.

void LDestroy(List * pList);

// - 리스트를 제거합니다.
// - 할당된 메모리를 모두 회수합니다.


LData LGet(List * pList, int index);

// - 해당 인덱스의 원소를 가져옵니다.
// - 인덱스의 원소가 없을 경우 에러를 반환합니다.


void LSet(List * pList, int index, LData data);

// - 해당 인덱스의 원소를 data로 수정합니다.
// - 인덱스의 원소가 없을 경우 에러를 반환합니다.

int LSize(List * pList);

// - 리스트의 크기를 반환합니다.
 
void LInsertHeader(List * pList, LData data);

// - 리스트 머리 부분에 data를 삽입합니다.

void LInsertIndex(List * pList, int index, LData data);

// - 리스트 index 위치에 data를 삽입합니다.
// - 만약 리스트 크기보다 크면 에러를 반환합니다.


void LInsertTail(List * pList, LData data);

// - 리스트 꼬리 부분에 data를 삽입합니다.

LData LRemoveHeader(List * pList);

// - 리스트 머리 부분에 위치한 data를 삭제합니다.

LData LRemoveIndex(List * pList, int index);

// - 리스트 index에 위치한 data를 삭제합니다.
// - 만약 리스트 크기보다 크면 에러를 반환합니다.

LData LRemoveTail(List * pList);

// - 리스트 꼬리 부분에 위치한 data를 삭제합니다.

#ifndef _COMMON_LIST_DEF_
#define _COMMON_LIST_DEF_

#define TRUE		1
#define FALSE		0

#endif