#include "linked_list.h"
#include <stdio.h>


LinkedList* createLinkedList(){

    // 주소 변수 선언.
    LinkedList *plist;
    // 주소 할당.
    plist = (LinkedList *)malloc(sizeof(LinkedList));
    // 주소를 할당 받지 못했다면.
    if (plist == NULL)
        return (NULL);
    else
    {
        // 구조체 초기화 함수. 시작 주소를 가리키는 포인터, 설정하려는 값, 설정하려는 바이트 수.
        memset(plist, 0, sizeof(LinkedList));
    }
    // 현재 길이와 헤더노드 다음 설정.
    return (plist);
};



int addLLElement(LinkedList* pList, int position, ListNode element){

    // if (pList->headerNode.pLink == NULL)
    //     pList->headerNode.pLink = element.data;
    //     element.pLink = (ListNode *)malloc(sizeof(ListNode));
        
    //     pList->currentElementCount += 1;
    //     pList->headerNode.pLink =
    
    // 이전과 뒤 정의.
    ListNode *prev;
    ListNode *new;

    // // 할당 받지 못한 리스트거나, 넣는 위치가 음수거나, 넣는 위치가 현재 길이보다 크면 실패.
    // // 코드에 따라 오류를 나누네. 일단 이건 넘기고 그걸로 써봐야겠다.
    // if (pList == NULL || position < 0 || position > pList->currentElementCount)
    //     return FALSE;

    // // 새 노드 자리 주려고 할당했는데 안 되면 실패.
    // new = (ListNode *)malloc(sizeof(ListNode));
    // if (new == NULL);
    //     return(FALSE);

    // (새삼 element 자체를 그대로 넣지 않고 안에서 타입형을 한번 정의하네. 왜지?)
    // 데이터를 미리 할당해놓는 템플릿에 가까운 개념인 것 같다.
    int i = 0;
    // 잘 할당받은 리스트인지 체크.
    if(pList!= NULL)
    {
        // 앞으로 넣을 노드에 메모리 할당. (근데 여기서 만들면 실패해도 말록 안 지워지지않음?)
        new = (ListNode *)malloc(sizeof(ListNode));
        // 인덱스 밖이 아닌지 체크.
        if(position>=0 && position <= pList->currentElementCount)
        {
            // 그 주소 안에 값으로 element를 넣고.
            *new = element;
            // 요소 뒤에 뭘 가리키는지 지정해두고.
            new->pLink = NULL;

            //현재 아직 갱신하지 않은 헤더 노드가 이전 노드일테니까.
            prev = &(pList->headerNode);
            // 현재 넣을 노드의 앞 노드 찾기 위해 loop로 반복 돌려서.
            // (pLink로 타고 들어가서 찾음.)
            for(i=0; i<position; i++){
                prev = prev->pLink;
            // 이전 노드가 갖고 있던 포인터는 계승하고, 이전 노드는 새로운 노드를 가리킴.
            new->pLink = prev->pLink;
            prev->pLink = new;

            pList->currentElementCount++;
            return TRUE;
            }
        }
        else{
            printf("인덱스 오류");
            return FALSE;
        }
    }
    else{
        printf("메모리할당 오류");
        return FALSE;
    }
    

};


int removeLLElement(LinkedList* pList, int position){

    ListNode *prev;
    ListNode *remove;
    int i = 0;
    if(pList != NULL)
    {
        if(position>= 0 && position <= pList->currentElementCount)
        {

            prev = &(pList->headerNode);
            
            for(i= 0; i<position; i++){
                prev = prev->pLink;
            }
            remove = prev->pLink;
            prev->pLink = remove->pLink;
            free(remove);
            pList->currentElementCount--;
            return TRUE;
        }
    }

};


ListNode* getLLElement(LinkedList* pList, int position){

    ListNode *prev;
    ListNode *get;

    int i = 0;
    if(pList != NULL)
    {
        if(position>=0 && position <= pList->currentElementCount)
        {
            prev = &(pList->headerNode);

            for(i=0; i<position; i++){
                prev = prev->pLink;
            }
            get = prev->pLink;
            
        }
    }
    return (get);
};

void clearLinkedList(LinkedList* pList){

    if(pList != NULL)
        if(pList->currentElementCount >0)
            removeLLElement(pList, 0);
};


int getLinkedListLength(LinkedList* pList){

    int length = 0;

    if(pList != NULL)
        length = pList->currentElementCount;

    return length;
};


void deleteLinkedList(LinkedList* pList){

    if(pList != NULL)
    {
        clearLinkedList(pList);
        free(pList);
    }

};