#include "rbtree.h"

#include <stdlib.h>



rbtree *new_rbtree(void) {
  // rbtree 크기만큼 증가할때마다 1씩 할당.  
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // 여기서 nil도 일종의 노드 처럼 할당해서 검정 속성 부여.
  // 그냥 Null을 쓰는 대신 검은색 가짐!
  if (p != NULL){
    p->nil= (node_t*)calloc(1,sizeof(node_t));
    p->nil->color = RBTREE_BLACK;
    p->root = p->nil;
    
    return p;
  }
  else{
    return FALSE;
  }
   //TODO: initialize struct if needed

}

// 왼쪽으로 회전.
//(x의 오른쪽 자식 노드가 x자리에,)
//(x는 y의 왼쪽 자식이 된다.)
void left_rotate(rbtree *t, node_t *x){

  node_t *y;
  
  // y가 x의 오른쪽 자식일때.
  y = x->right;


// (손자 정리 시즌)
  // x의 오른쪽에는, 오른쪽 자식y의 왼쪽 자식(손자) 트리를 붙인다.
  x->right = y->left;

 // 근데 y의 왼쪽 자식이 nil이 아니라면 그 노드의 부모도 설정해준다.
  if (y->left != t->nil){
    y->left->parent = x;
  }

// (y 이동 시즌 )
  // 그리고 y도 자리를 옮겨, y도 부모를 설정해준다.
  y->parent = x->parent;

  // 만약 y가 최고 조상이었으면, root임을 알려준다.
  if (x->parent == t->nil){
    t->root = y;
  }
  // 조상의 왼쪽 자식이었으면, y도 왼쪽 자식.
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  // 조상의 오른쪽 자식이었으면, y도 오른쪽 자식.
  else {
    x->parent->right = y;
  }
// (x 이동 시즌)
// y의 자식으로 x를, x부모에도 y를 설정.
  y->left = x;
  x->parent = y;

}


// 오른쪽으로 회전.
// x는 부모 y의 왼쪽 자식인데
// x가 부모 y의 자리에 오고, y는 x의 오른쪽 자식이 된다.
void right_rotate(rbtree *t, node_t *y){

  node_t *x;

  x = y->left;

  y->left = x->right;

  if (x->right != t->nil){
    x->right->parent = y;
  }

  x->parent = y->parent;

  if(y->parent == t->nil){
    t->root = x;
  }
  else if (y == y->parent->left)
  {
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;

}

void delete_rbtree(rbtree *t) {


   //TODO: reclaim the tree nodes's memory
  free(t->nil);
  free(t);
}

 node_t *rbtree_insert(rbtree *t, const key_t key) {

   node_t *y = t->nil;
   node_t *x = t->root;
   node_t *z;

// 넣을 노드는 빨간색. 노드 형에 값을 넣어줘서 z노드를 만든다.
   z->key = key;
   z->color = RBTREE_RED;


// 근데 루트가 안 비어있다면,
   while (x != t->nil)
   {
    // y로 현재 지점은 저장하고, 맞는 위치까지, nil을 만날 때까지 트리 아래로 하강.
     y = x;
     if(z->key < x->key){
       x = x->left;
     }
     else{
       x = x->right;
     }
   }

  // 찾았을테니, 현재 지점을 z의 부모로 설정하는데.... 
   z->parent = y;

  // 만약 그 자리가 최고 조상이면 z가 루트.
   if (y == t->nil){
     t->root = z;
   }
  //크기에 따라 z가 왼쪽 자식인지 오른쪽 자식인지 지정.
   else if (z->key < y->key)
   {
     y->left = z;
   }
   else{
     y->right = z;
   }
   // 잘 했으니 z에게 nil 들을 붙여주고, z 색도 다시 빨강!
   z->left = t->nil;
   z->right = t->nil;
   z->color = RBTREE_RED;
 rbtree_insert_fixup(t,z);
    //TODO: implement insert
   return t->root;
 }

 void *rbtree_insert_fixup(rbtree *t, node_t *z){

  node_t *y;

  // 현재 부모가 레드라면
   while (z->parent->color == RBTREE_RED)
   {
    // 부모가 할아버지의 왼쪽 자식일때
    if (z->parent == z->parent->parent->left){
      // y는 삼촌이라고 할 때
      y = z->parent->parent->right;
      // 삼촌도 레드면
      if(y->color == RBTREE_RED){
        // 부모 블랙! 삼촌 블랙! 할아버지 레드! 
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // z를 할아버지로 설정!
        z = z->parent->parent;
      }
      // 현재 애가 부모의 오른쪽 자식이면(꺾인 상태면)
      else if (z == z->parent->right)
      {
        // 부모 기준으로 왼쪽 회전!
        z = z->parent;
        left_rotate(t,z);
      }
      // 현재 펴진 상태.
      // 부모 색깔 블랙! 할아버지 색깔 레드! 할아버지 기준 오른쪽 회전! 
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t,z->parent->parent);
    }

    // 부모가 할아버지의 오른쪽 자식일 때
    else{
      y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else if (z == z->parent->left)
      {
        z = z->parent;
        left_rotate(t,z);
      }
      z->parent->color = RBTREE_BLACK;
      z->parent->parent->color = RBTREE_RED;
      right_rotate(t,z->parent->parent);
    }
  }
  
  t->root->color= RBTREE_BLACK;

 }

 node_t *rbtree_find(const rbtree *t, const key_t key) {
    //TODO: implement find
   return t->root;
 }

 node_t *rbtree_min(const rbtree *t) {
    //TODO: implement find
   return t->root;
 }

 node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    return t->root;
 }

void rb_transplant(rbtree *t, node_t *u, node_t *v){

  if (u->parent == t->nil){
    t->root = v;
  }
  else if ( u == u->parent->left)
  {
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u ->parent;
  
}

 int rbtree_erase(rbtree *t, node_t *z) {

  node_t *y;
  node_t *x;

  color_t *succesor;

  y = z;
  succesor = y->color;
  // y-original-color = y->color...

  if(z->left == t->nil){
    x = z->right;
    rb_transplant(t,z,z->right);
  }
  else if (z->right == t->nil)
  {
    x = z->left;
    rb_transplant(t,z,z->left);
  }
  else{
    y = rbtree_min(z->right);
    succesor = y->color;
    // y-originalcolor = y->color
    x = y->right;
    if(y->parent == z){
      x->parent = y;
    }
    else{
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  
  if(succesor == RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }
  // if(y-origincolor == black){}
// delete-fixup(t,x)
    // TODO: implement erase
  return 0;
 }

 void rbtree_delete_fixup(rbtree *t, node_t *x){

  node_t *w;

  while (x != t->root && x->color== RBTREE_BLACK){
    // 형제가 RED.
    if(x == x->parent->left){
      w = x->parent->right;
      if(w->color == RBTREE_RED){
        w->color =RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }

      // 형제, 형제의 자식 둘다 블랙.
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // 형제의 꺾인 자식이 RED
      else if (w->right->color == RBTREE_BLACK){
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t, w);
        w = x->parent->right;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->right->color = RBTREE_BLACK;
      left_rotate(t,x->parent);
      x = t->root;
    }
    else{
      w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color =RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else if (w->left->color == RBTREE_BLACK){
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t, w);
        w = x->parent->left;
      }
      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->left->color = RBTREE_BLACK;
      right_rotate(t,x->parent);
      x = t->root;
    }
  }
  x->color = RBTREE_BLACK;
 }

 int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
 return 0;
 }
