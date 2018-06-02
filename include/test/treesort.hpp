 #ifndef TREE_SORT_H
#define TREE_SORT_H
#include <algorithm>
namespace
{
template <class RandomAccessIterator>
struct treenode
{
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type T;
    treenode(const T& value)
        : value_(value), left_(NULL), right_(NULL)
    {
    }
    static treenode<RandomAccessIterator>* build(RandomAccessIterator first, RandomAccessIterator last)
    {
        treenode<RandomAccessIterator>* root = NULL;
        for (RandomAccessIterator it = first; it < last; ++it) {
            treenode<RandomAccessIterator>* node = new treenode<RandomAccessIterator>(*it);
            if (root == NULL) {
                root = node;
            }
            else {
                treenode<RandomAccessIterator>* current = root,* previous = NULL;
                bool less;
                while (current != NULL) {
                    less = node->value_ < current->value_;
                    previous = current;
                    current = less ? current->left_ : current->right_;
                }
                if (less) {
                    previous->left_ = node;
                }
                else {
                    previous->right_ = node;
                }
            }
        }
        return root;
    }
    void write(RandomAccessIterator* it) const
    {
        if (left_) {
            left_->write(it);
        }
        **it = value_;
        ++(*it);
        if (right_) {
            right_->write(it);
        }
    }
    void destroy()
    {
        if (left_) {
            left_->destroy();
        }
        if (right_) {
            right_->destroy();
        }
        delete this;
    }
    T value_;
    treenode<RandomAccessIterator>* left_;
    treenode<RandomAccessIterator>* right_;
};
};
template <class RandomAccessIterator>
void treesort(RandomAccessIterator first, RandomAccessIterator last)
{
    treenode<RandomAccessIterator>* tree = treenode<RandomAccessIterator>::build(first, last);
    tree->write(&first);
    tree->destroy();
}
#endif // TREE_SORT_H

