
class PolyNode;
typedef std::vector< PolyNode* > PolyNodes;

class PolyNode
{
public:
  Path Contour;
  PolyNodes Childs;
  PolyNode* Parent;

  unsigned Index; //node index in Parent.Childs
  bool m_IsOpen;
  JoinType m_jointype;
  EndType m_endtype;
  friend class Clipper; //to access Index
  friend class ClipperOffset;
  // PolyNode methods ...

  PolyNode::PolyNode(): Childs(), Parent(0), Index(0), m_IsOpen(false) {
  }


  int PolyNode::ChildCount() const {
    return (int)Childs.size();
  }


  void PolyNode::AddChild(PolyNode& child) {
    unsigned cnt = (unsigned)Childs.size();
    Childs.push_back(&child);
    child.Parent = this;
    child.Index = cnt;
  }


  PolyNode* PolyNode::GetNext() const {
    if (!Childs.empty()) {
      return Childs[0];
    }
    else {
      return GetNextSiblingUp();
    }
  }


  PolyNode* PolyNode::GetNextSiblingUp() const {
    if (!Parent) { //protects against PolyTree.GetNextSiblingUp()
      return 0;
    }
    else if (Index == Parent->Childs.size() - 1) {
      return Parent->GetNextSiblingUp();
    }
    else {
      return Parent->Childs[Index + 1];
    }
  }


  bool PolyNode::IsHole() const {
    bool result = true;
    PolyNode* node = Parent;

    while (node) {
      result = !result;
      node = node->Parent;
    }

    return result;
  }

  bool PolyNode::IsOpen() const {
    return m_IsOpen;
  }
};

class PolyTree: public PolyNode
{
public:
  PolyNodes AllNodes;
  friend class Clipper; //to access AllNodes
public:
  ~PolyTree() {
    Clear();
  };

  void PolyTree::Clear() {
    for (PolyNodes::size_type i = 0; i < AllNodes.size(); ++i) {
      delete AllNodes[i];
    }

    AllNodes.resize(0);
    Childs.resize(0);
  }

  PolyNode* PolyTree::GetFirst() const {
    if (!Childs.empty()) {
      return Childs[0];
    }
    else {
      return 0;
    }
  }


  int PolyTree::Total() const {
    return (int)AllNodes.size();
  }
};
