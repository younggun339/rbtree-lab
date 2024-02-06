#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_


#ifndef _COMMON_LIST_DEF_
#define _COMMON_LIST_DEF_

#define TRUE		1
#define FALSE		0

#endif


rbtree *new_rbtree(void) {
  // rbtree 크기만큼 증가할때마다 1씩 할당.  
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // 여기서 nil도 일종의 노드 처럼 할당해서 검정 속성 부여.
  // 그냥 Null을 쓰는 대신 검은색 가짐!
  if (p != NULL){
    p->nil= (node_t*)calloc(1,sizeof(node_t));
    p->nil->color = RBTREE_BLACK;
    p->root = p->nil;
    // dangler poiner 로 해봤으나 먹히지 않았다..
    // p->nil->key = 0;
  
    return p;
  }
  else{
    // 그냥 별도로 True, False 0, 1로 헤더에서 설정해놔서 그렇다.
    return FALSE;
  }

}

// 왼쪽으로 회전.
//(x의 오른쪽 자식 노드 y가 x자리에,)
//(x는 y의 왼쪽 자식이 된다.)
void left_rotate(rbtree *t, node_t *x){

  node_t *y;
  
  // y가 x의 오른쪽 자식일때.
  y = x->right;

  if(y != t->nil){
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
    x->parent = y;}

}


// 오른쪽으로 회전.
// x는 부모 y의 왼쪽 자식인데
// x가 부모 y의 자리에 오고, y는 x의 오른쪽 자식이 된다.
void right_rotate(rbtree *t, node_t *y){

  node_t *x;

  x = y->left;
  if(x != t->nil){
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

}

// 중위 순회식으로 노드 지우기
void inorder_tree_free(rbtree *t, node_t *x){
    

    if(x!=t->nil){
  
        inorder_tree_free(t, x->left);
        free(x);
        inorder_tree_free(t,x->right);
    }
  
}

// 트리 지우기
void delete_rbtree(rbtree *t) {

  node_t *y= t->root;

  inorder_tree_free(t, y);
   //TODO: reclaim the tree nodes's memory

  free(t->nil);
  free(t);
  t = NULL;
}

//삽입 시 속성 위반
void *rbtree_insert_fixup(rbtree *t, node_t *z) {
    node_t *y;

    // 현재 부모가 레드라면
    while (z->parent->color == RBTREE_RED) {
        // 부모가 할아버지의 왼쪽 자식일때
        if (z->parent == z->parent->parent->left) {
            // y는 삼촌이라고 할 때
            y = z->parent->parent->right;

            // 삼촌도 레드면
            if (y->color == RBTREE_RED) {
                // 부모 블랙! 삼촌 블랙! 할아버지 레드!
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                // z를 할아버지로 설정!
                z = z->parent->parent;
            } else {
                // 현재 애가 부모의 오른쪽 자식이면(꺾인 상태면)
                if (z == z->parent->right) {
                    // 부모 기준으로 왼쪽 회전!
                    z = z->parent;
                    left_rotate(t, z);
                }
                // 현재 펴진 상태.
                // 부모 색깔 블랙! 할아버지 색깔 레드! 할아버지 기준 오른쪽 회전!
                z->parent->color = RBTREE_BLACK;
                if (z->parent != t->nil && z->parent->parent != t->nil) {
                    z->parent->parent->color = RBTREE_RED;
                    right_rotate(t, z->parent->parent);
                }
            }
        } else if (z->parent == z->parent->parent->right) {
            // 부모가 할아버지의 오른쪽 자식일 때
            y = z->parent->parent->left;

            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                if (z->parent != t->nil && z->parent->parent != t->nil) {
                    z->parent->parent->color = RBTREE_RED;
                    left_rotate(t, z->parent->parent);
                }
            }
        }
    }

    t->root->color = RBTREE_BLACK;

    return 0;
}


// rb트리 삽입.
 node_t *rbtree_insert(rbtree *t, const key_t key) {

   node_t *y = t->nil;
   node_t *x = t->root;
   node_t *z = (node_t *)malloc(sizeof(node_t));
   
  
// 넣을 노드는 빨간색. 노드 형에 값을 넣어줘서 z노드를 만든다.
   z->key = key;
   z->color = RBTREE_RED;
   z->left = t->nil;
   z->right = t->nil;
   z->parent = t->nil;

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


// rb트리에서 값을 찾는 함수. 
 node_t *rbtree_find(const rbtree *t, const key_t key) {
//    printf("%d", 3);
  node_t *x = t->root;
  node_t *y;
//   printf("%d", 4);
  while (x != t->nil)
  {
    if(key < x->key){
      y = x;
      x = x->left;
    }
    else if(key > x->key){
      y = x;
      x = x->right;
    }
    else{
    //   printf("%d", x->key);
      return x;
    } 
  }
  return NULL;

 }

// rb트리에서 가장 작은 값을 찾는 함수.
 node_t *rbtree_min(const rbtree *t) {

  node_t *x = t->root;
  node_t *z;

  z = x;

  while(x != t->nil){
    z = x;
    x = x->left;
  }

  return z;
 }


// rb트리에서 가장 큰 값을 찾는 함수.
 node_t *rbtree_max(const rbtree *t) {

  node_t *x = t->root;
  node_t *z;

  z = x;

  while (x != t->nil)
  {
    z = x;
    x = x->right;
  }
  
  return z;
 }

// rb트리에서 두 노드의 자리를 바꾸는 함수.
// 정확히는 u자리에 v만 들어갈뿐임.(v자리는 아무도 안들어감.)
void rb_transplant(rbtree *t, node_t *u, node_t *v){

// 루트면 본인.
  if (u->parent == t->nil){
    t->root = v;
  }
  // 어느 자식이었는지에 따라 설정.
  else if ( u == u->parent->left)
  {
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  // 부모 통일.
  v->parent = u->parent;
  
}
// 삭제시 위반 속성을 검사하는 함수.
 void rbtree_delete_fixup(rbtree *t, node_t *x){

  node_t *w;

// x가 루트가 아니고 삭제색깔이 검정이라면.
  while (x != t->root && x->color== RBTREE_BLACK){
    // x가 부모의 왼쪽 자식 일 경우.
    if(x == x->parent->left){
      w = x->parent->right;
      // 형제가 RED
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
      else { 
        
        if (w->right->color == RBTREE_BLACK){
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotate(t, w);
            w = x->parent->right;
        }
        // 형제의 펴진 자식이 RED
        else{
            w->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            w->right->color = RBTREE_BLACK;
            left_rotate(t,x->parent);
            x = t->root;
        }
      }
    }

    // x가 부모의 오른쪽 자식일 경우.
    else{
      w = x->parent->left;
      // 형제가 RED
      if(w->color == RBTREE_RED){
        w->color =RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }

      // 형제, 형제 자식 둘 다 블랙.
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // 형제 꺾인 자식 RED
      else{ 
        
        if (w->left->color == RBTREE_BLACK){
            w->right->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            left_rotate(t, w);
            w = x->parent->left;
        }
        // 형제 펴진 자식 RED
        else{
            w->color = x->parent->color;
            x->parent->color = RBTREE_BLACK;
            w->left->color = RBTREE_BLACK;
            right_rotate(t,x->parent);
            x = t->root;
        }
      }
    }

  }
  x->color = RBTREE_BLACK;
 }


 node_t *tree_minimum(rbtree *t,  node_t *x){
  
  while(x->left != t->nil){
    x = x->left;
  }

  return x;
 }


 int rbtree_erase(rbtree *t, node_t *z) {

  node_t *y;
  node_t *x;

  color_t succesor;

  y = z;
  succesor = y->color;

//   if(z->left == t->nil && z->right == t->nil){
//     x = t->nil;
//     if(z == t->root){
//         t->root = t->nil;
//     }
//   }
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
    y = tree_minimum(t,z->right);
    succesor = y->color;
 
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
  
  if(z == z->parent->left){
    z->parent->left = t->nil;
  }
  else if(z == z->parent->right){
    z->parent->right = t->nil;
  }

  free(z);
  z = t->nil;
  
  return 0;
 }

 void array_rbtree(const rbtree *t, key_t *arr, node_t *root, int *index_x){

    if(root != t->nil){
        array_rbtree(t, arr, root->left, index_x);

        arr[*index_x] = root->key;
        (*index_x)++;

        array_rbtree(t, arr, root->right, index_x);
    }

 }

 int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {

 if(arr != NULL && t != NULL){
    int index_x = 0;
    array_rbtree(t, arr, t->root, &index_x);
 }
 
 return 0;
 }



int pow_2(int x) {
  // 2**x 계산 함수
  return 1 << x;
}
void record_keys(rbtree *t, node_t *p, node_t **node_arr, int *node_idx_list, int cur_depth, int render_depth) {
  // visualize_tree에서 사용하기위한 배열을 재귀적으로 기록
  if (cur_depth > render_depth) {
    return;
  }
  node_arr[cur_depth * pow_2(render_depth) + node_idx_list[cur_depth]] = p;
  node_idx_list[cur_depth] += 1;
  if (p == t->nil) {
    record_keys(t, t->nil, node_arr, node_idx_list, cur_depth +1, render_depth);
    record_keys(t, t->nil, node_arr, node_idx_list, cur_depth +1, render_depth);
  } else {
    record_keys(t, p->left, node_arr, node_idx_list, cur_depth +1, render_depth);
    record_keys(t, p->right, node_arr, node_idx_list, cur_depth +1, render_depth);
  }
}
void visualize_tree(rbtree *t, int render_depth) {
  // 트리 시각화 함수
  // depth는 루트를 0으로 계산
  // key_arr[i][j]: 깊이가 i인 (j+1)번째 원소의 키 **2차원 배열 접근이 불가능해 1차원 배열로 구현
    node_t **node_arr = calloc( (render_depth +1) * pow_2(render_depth), sizeof(node_t));
  // key_idx_list[i]: key_arr[i]의 현재 인덱스
  int *node_idx_list = (int *) calloc(render_depth +1, sizeof(int));
  char *filler = "  ";
  // 키를 배열에 기록
  record_keys(t, t->root, node_arr, node_idx_list, 0, render_depth);
  // 기록된 키를 출력
  for (int i = 0; i < render_depth +1; i++) {
    for (int j = 0; j < pow_2(i); j++) {
      for (int k = 0; k < pow_2(render_depth - i); k++) { // front buffer
        printf("%s", filler);
      }
      if (node_arr[i * pow_2(render_depth) + j] == t->nil) {
        printf("\x1b[0m" "%2s", ".");
      } else if (node_arr[i * pow_2(render_depth) + j]->color == RBTREE_BLACK) {
        // 검정 노드는 흰색으로 출력
        printf("\x1b[0m" "%2d", node_arr[i * pow_2(render_depth) + j]->key);
      } else {
        // 빨강 노드는 빨간색으로 출력
        printf("\x1b[31m" "%2d", node_arr[i * pow_2(render_depth) + j]->key);
      }
      for (int k = 0; k < pow_2(render_depth - i) -1; k++) { // back buffer
        printf("%s", filler);
      }
    }
    printf("\n");
  }
  printf("\n");
  free(node_arr);
  free(node_idx_list);
}

// new_rbtree should return rbtree struct with null root node
void test_init(void) {
  rbtree *t = new_rbtree();
  assert(t != NULL);
// #ifdef SENTINEL
  assert(t->nil != NULL);
  assert(t->root == t->nil);
// #else
//   assert(t->root == NULL);
// #endif
  delete_rbtree(t);
}

// root node should have proper values and pointers
void test_insert_single(const key_t key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);
  // assert(p->color == RBTREE_BLACK);  // color of root node should be black
// #ifdef SENTINEL
  assert(p->left == t->nil);
  assert(p->right == t->nil);
  assert(p->parent == t->nil);
// #else
//   assert(p->left == NULL);
//   assert(p->right == NULL);
//   assert(p->parent == NULL);
// #endif
  delete_rbtree(t);
}

// find should return the node with the key or NULL if no such node exists
void test_find_single(const key_t key, const key_t wrong_key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);

  node_t *q = rbtree_find(t, key);
  assert(q != NULL);
  assert(q->key == key);
  assert(q == p);

  q = rbtree_find(t, wrong_key);
  assert(q == NULL);

  delete_rbtree(t);
}

// erase should delete root node
void test_erase_root(const key_t key) {
  rbtree *t = new_rbtree();
  node_t *p = rbtree_insert(t, key);
  assert(p != NULL);
  assert(t->root == p);
  assert(p->key == key);

  rbtree_erase(t, p);
// #ifdef SENTINEL
  assert(t->root == t->nil);
// #else
//   assert(t->root == NULL);
// #endif

  delete_rbtree(t);
}

static void insert_arr(rbtree *t, const key_t *arr, const size_t n) {
  for (size_t i = 0; i < n; i++) {
    rbtree_insert(t, arr[i]);
  }
}

static int comp(const void *p1, const void *p2) {
  const key_t *e1 = (const key_t *)p1;
  const key_t *e2 = (const key_t *)p2;
  if (*e1 < *e2) {
    return -1;
  } else if (*e1 > *e2) {
    return 1;
  } else {
    return 0;
  }
};

// min/max should return the min/max value of the tree
void test_minmax(key_t *arr, const size_t n) {
  // null array is not allowed
  assert(n > 0 && arr != NULL);

  rbtree *t = new_rbtree();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);
#ifdef SENTINEL
  assert(t->root != t->nil);
#endif

  qsort((void *)arr, n, sizeof(key_t), comp);
  node_t *p = rbtree_min(t);
  assert(p != NULL);
  assert(p->key == arr[0]);

  node_t *q = rbtree_max(t);
  assert(q != NULL);
  assert(q->key == arr[n - 1]);

  rbtree_erase(t, p);
  p = rbtree_min(t);
  assert(p != NULL);
  assert(p->key == arr[1]);

  if (n >= 2) {
    rbtree_erase(t, q);
    q = rbtree_max(t);
    assert(q != NULL);
    assert(q->key == arr[n - 2]);
  }

  delete_rbtree(t);
}

void test_to_array(rbtree *t, const key_t *arr, const size_t n) {
  assert(t != NULL);

  insert_arr(t, arr, n);
  qsort((void *)arr, n, sizeof(key_t), comp);

  key_t *res = calloc(n, sizeof(key_t));
  rbtree_to_array(t, res, n);
  for (int i = 0; i < n; i++) {
    assert(arr[i] == res[i]);
  }
  free(res);
}

void test_multi_instance() {
  rbtree *t1 = new_rbtree();
  assert(t1 != NULL);
  rbtree *t2 = new_rbtree();
  assert(t2 != NULL);

  key_t arr1[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n1 = sizeof(arr1) / sizeof(arr1[0]);
  insert_arr(t1, arr1, n1);
  qsort((void *)arr1, n1, sizeof(key_t), comp);

  key_t arr2[] = {4, 8, 10, 5, 3};
  const size_t n2 = sizeof(arr2) / sizeof(arr2[0]);
  insert_arr(t2, arr2, n2);
  qsort((void *)arr2, n2, sizeof(key_t), comp);

  key_t *res1 = calloc(n1, sizeof(key_t));
  rbtree_to_array(t1, res1, n1);
  for (int i = 0; i < n1; i++) {
    assert(arr1[i] == res1[i]);
  }

  key_t *res2 = calloc(n2, sizeof(key_t));
  rbtree_to_array(t2, res2, n2);
  for (int i = 0; i < n2; i++) {
    assert(arr2[i] == res2[i]);
  }

  free(res2);
  free(res1);
  delete_rbtree(t2);
  delete_rbtree(t1);
}

// Search tree constraint
// The values of left subtree should be less than or equal to the current node
// The values of right subtree should be greater than or equal to the current
// node

static bool search_traverse(const node_t *p, key_t *min, key_t *max,
                            node_t *nil) {
  if (p == nil) {
    return true;
  }

  *min = *max = p->key;

  key_t l_min, l_max, r_min, r_max;
  l_min = l_max = r_min = r_max = p->key;

  const bool lr = search_traverse(p->left, &l_min, &l_max, nil);
  if (!lr || l_max > p->key) {
    return false;
  }
  const bool rr = search_traverse(p->right, &r_min, &r_max, nil);
  if (!rr || r_min < p->key) {
    return false;
  }

  *min = l_min;
  *max = r_max;
  return true;
}

void test_search_constraint(const rbtree *t) {
  assert(t != NULL);
  node_t *p = t->root;
  key_t min, max;
// #ifdef SENTINEL
  node_t *nil = t->nil;
// #else
//   node_t *nil = NULL;
// #endif
  assert(search_traverse(p, &min, &max, nil));
}

// Color constraint
// 1. Each node is either red or black. (by definition)
// 2. All NIL nodes are considered black.
// 3. A red node does not have a red child.
// 4. Every path from a given node to any of its descendant NIL nodes goes
// through the same number of black nodes.

bool touch_nil = false;
int max_black_depth = 0;

static void init_color_traverse(void) {
  touch_nil = false;
  max_black_depth = 0;
}

static bool color_traverse(const node_t *p, const color_t parent_color,
                           const int black_depth, node_t *nil) {
  if (p == nil) {
    if (!touch_nil) {
      touch_nil = true;
      max_black_depth = black_depth;
    } else if (black_depth != max_black_depth) {
      return false;
    }
    return true;
  }
  if (parent_color == RBTREE_RED && p->color == RBTREE_RED) {
    return false;
  }
  int next_depth = ((p->color == RBTREE_BLACK) ? 1 : 0) + black_depth;
  return color_traverse(p->left, p->color, next_depth, nil) &&
         color_traverse(p->right, p->color, next_depth, nil);
}

void test_color_constraint(const rbtree *t) {
  assert(t != NULL);
#ifdef SENTINEL
  node_t *nil = t->nil;
#else
  node_t *nil = NULL;
#endif
  node_t *p = t->root;
  assert(p == nil || p->color == RBTREE_BLACK);

  init_color_traverse();
  assert(color_traverse(p, RBTREE_BLACK, 0, nil));
}

// rbtree should keep search tree and color constraints
void test_rb_constraints(const key_t arr[], const size_t n) {
  rbtree *t = new_rbtree();
  assert(t != NULL);

  insert_arr(t, arr, n);
  assert(t->root != NULL);

  test_color_constraint(t);
  test_search_constraint(t);

  delete_rbtree(t);
}

// rbtree should manage distinct values
void test_distinct_values() {
  const key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_rb_constraints(entries, n);
}

// rbtree should manage values with duplicate
void test_duplicate_values() {
  const key_t entries[] = {10, 5, 5, 34, 6, 23, 12, 12, 6, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_rb_constraints(entries, n);
}

void test_minmax_suite() {
  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_minmax(entries, n);
}

void test_to_array_suite() {
  rbtree *t = new_rbtree();
  assert(t != NULL);

  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  test_to_array(t, entries, n);

  delete_rbtree(t);
}

void test_find_erase(rbtree *t, const key_t *arr, const size_t n) {
  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
    // visualize_tree(t,4);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_find(t, arr[i]);
    printf("arr[%d] = %d\n", i, arr[i]);
    assert(p != NULL);
    assert(p->key == arr[i]);
    // visualize_tree(t,4);
    rbtree_erase(t, p);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_find(t, arr[i]);
    assert(p == NULL);
  }

  for (int i = 0; i < n; i++) {
    node_t *p = rbtree_insert(t, arr[i]);
    assert(p != NULL);
    node_t *q = rbtree_find(t, arr[i]);
    assert(q != NULL);
    assert(q->key == arr[i]);
    assert(p == q);
    rbtree_erase(t, p);
    q = rbtree_find(t, arr[i]);
    assert(q == NULL);
  }
}

void test_find_erase_fixed() {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  rbtree *t = new_rbtree();
  assert(t != NULL);

  test_find_erase(t, arr, n);

  delete_rbtree(t);
}

void test_find_erase_rand(const size_t n, const unsigned int seed) {
  srand(seed);
  rbtree *t = new_rbtree();
  key_t *arr = calloc(n, sizeof(key_t));
  for (int i = 0; i < n; i++) {
    arr[i] = rand();
  }

  test_find_erase(t, arr, n);

  free(arr);
  delete_rbtree(t);
}

int main(void) {
  test_init();
  test_insert_single(1024);
  test_find_single(512, 1024);
  test_erase_root(128);
  test_find_erase_fixed();
  test_minmax_suite();
  test_to_array_suite();
  test_distinct_values();
  test_duplicate_values();
  test_multi_instance();
  test_find_erase_rand(10000, 17);
  printf("Passed all tests!\n");
}