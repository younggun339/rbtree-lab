#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  if (p != NULL){
    p->nil= (node_t*)calloc(1,sizeof(node_t));
    p->nil->color = RBTREE_BLACK;
    p->root = p->nil;
    
    return p;
  }
  else{
    return FALSE;
  }
  // TODO: initialize struct if needed

}

void left_rotate(rbtree *t, node_t *x){

  node_t *y;
  
  y = x->right;
  x->right = y->left;

  if (y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;

}

void right_rotate(rbtree *t, node_t *x){

  node_t *y;

  y = x->parent;
  x->right = y->left;

  if(y->left != t->nil){
    x->right->parent = x;
  }

  x->parent = y->parent;
  if(y->parent == t->nil){
    t->root = x;
  }
  else if (y == y->parent->left){
    y->parent->left = x;
  }
  else{
    y->parent->right = x;
  }

  x->right = y;
  y->parent = x;
}

void delete_rbtree(rbtree *t) {


  // TODO: reclaim the tree nodes's memory
  free(t->nil);
  free(t);
}

// node_t *rbtree_insert(rbtree *t, const key_t key) {

//   node_t *y = t->nil;
//   node_t *x = t->root;
//   node_t *z;

//   z->key = key;
//   z->color = RBTREE_RED;


//   while (x != t->nil)
//   {
//     y = x;
//     if(z->key < x->key){
//       x = x->left;
//     }
//     else{
//       x = x->right;
//     }
//   }

//   z->parent = y;
//   if (y == t->nil){
//     t->root = z;
//   }
//   else if (z->key < y->key)
//   {
//     y->left = z;
//   }
//   else{
//     y->right = z;
//   }
//   z->left = t->nil;
//   z->right = t->nil;
//   z->color = RBTREE_RED;
// rbtree_insert_fixup(t,z);
//   // TODO: implement insert
//   return t->root;
// }

// void *rbtree_insert_fixup(rbtree *t, node_t *z){

// //  while ()
// //  {
// //    /* code */
// // }
  

// }

// node_t *rbtree_find(const rbtree *t, const key_t key) {
//   // TODO: implement find
//   return t->root;
// }

// node_t *rbtree_min(const rbtree *t) {
//   // TODO: implement find
//   return t->root;
// }

// node_t *rbtree_max(const rbtree *t) {
// //   // TODO: implement find
//    return t->root;
// }

// int rbtree_erase(rbtree *t, node_t *p) {
// //   // TODO: implement erase
//  return 0;
// }

// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
// //   // TODO: implement to_array
// return 0;
// }
