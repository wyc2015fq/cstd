
// SEQPARTITION(total, seq, labels, is_equal, TYPE)
{
  typedef struct PTREENODE {
    struct PTREENODE* parent;
    const elem_type* element;
    int rank;
  } PTREENODE;
  int i, j, class_idx = 0;
  PTREENODE* nodes;
  IMMALLOC(PTREENODE, nodes, total);

  for (i = 0; i < total; i++) {
    PTREENODE node = { 0, 0, 0 };
    node.element = (seq + i);
    nodes[i] = node;
  }

  /* The main O(N^2) pass. Merge connected components. */
  for (i = 0; i < total; i++) {
    PTREENODE* node = nodes + i;
    PTREENODE* root = node;

    /* find root */
    while (root->parent) {
      root = root->parent;
    }

    for (j = 0; j < total; j++) {
      PTREENODE* node2 = nodes + j;

      if (node2->element && node2 != node) {
        int out;
        is_equal_macro(node->element, node2->element, out);

        if (out) {
          PTREENODE* root2 = node2;

          /* unite both trees */
          while (root2->parent) {
            root2 = root2->parent;
          }

          if (root2 != root) {
            if (root->rank > root2->rank) {
              root2->parent = root;
            }
            else {
              root->parent = root2;
              root2->rank += root->rank == root2->rank;
              root = root2;
            }

            /* compress path from node2 to the root */
            while (node2->parent) {
              PTREENODE* temp = node2;
              node2 = node2->parent;
              temp->parent = root;
            }

            /* compress path from node to the root */
            node2 = node;

            while (node2->parent) {
              PTREENODE* temp = node2;
              node2 = node2->parent;
              temp->parent = root;
            }
          }
        }
      }
    }
  }

  /* Final O(N) pass (Enumerate classes) */
  /* Reuse reader one more time */
  for (i = 0; i < total; i++) {
    PTREENODE* node = nodes + i;
    int idx = -1;

    if (node->element) {
      while (node->parent) {
        node = node->parent;
      }

      if (node->rank >= 0) {
        node->rank = ~class_idx++;
      }

      idx = ~node->rank;
    }

    labels[i] = idx;
  }

  class_cnt = class_idx;
  IMFREE(PTREENODE, nodes, total);
}

#undef IMFREE
#undef IMMALLOC
#undef is_equal_macro
