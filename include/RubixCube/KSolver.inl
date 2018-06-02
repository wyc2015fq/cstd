

#define KOCIEMBA
// Selects either Kociemba's or Winter's approach to the choice algorithm
// (comment out to choose the latter).  If this is changed, be sure to
// regenerate the tables involving "choice".


// Number of possiblities for each axis of the two triples
enum {
  Twists                        = 3 * 3 * 3 * 3 * 3 * 3 * 3, // 3^7  = 2187
  Flips                         = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2, // 2^11 = 2048
  Choices                       = 495,                 // 12 choose 4 = 495
  CornerPermutations            = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1, // 8! = 40320
  NonMiddleSliceEdgePermutations = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1, // 8! = 40320
  MiddleSliceEdgePermutations   = 4 * 3 * 2 * 1,       // 4! = 24
  NumberOfEncodedPermutations   = (1 << 12), // A 12 bit number, 1 bit for each edge
  NumberOfOrdinals              = 495                  // 12 choose 4 edges
};


// A cube model containing basic cube definitions with extensions for
// Kociemba's algorithm and cube operations. Applying a face maintains the
// proper cube permutation and orientation for each cubie.

// Cubies locations (offsets in array BYTE Cubies[20])
enum CKube_cubies {
  // Upper and Down corners
  CKube_URF, CKube_UFL, CKube_ULB, CKube_UBR, CKube_DFR, CKube_DLF, CKube_DBL, CKube_DRB,
  // Upper and Down slices edges
  CKube_UF, CKube_UL, CKube_UB, CKube_UR, CKube_DF, CKube_DL, CKube_DB, CKube_DR,
  // Middle slice edges
  CKube_RF, CKube_FL, CKube_LB, CKube_BR,

  CKube_invalidCubie
};

// Quarter and half turn moves
enum CKube_Move {
  CKube_R,  CKube_L,  CKube_U,  CKube_D,  CKube_F,  CKube_B,     // quarter turn moves
  CKube_Ri, CKube_Li, CKube_Ui, CKube_Di, CKube_Fi, CKube_Bi,    // reverse quarter turn moves
  CKube_R2, CKube_L2, CKube_U2, CKube_D2, CKube_F2, CKube_B2,    // half turn moves
  CKube_noop
};

typedef struct {
  BYTE CornerPermut[8];   // corners positions 8*(0..7)
  BYTE EdgePermut[12];    // edges positions 12*(0..11)
  WORD twist;             // corners twist (0..2187)
  WORD flip;              // edges flip (0..2148)
} KubeInfo;

typedef union {
  BYTE Cubies[8 + 12 + 2 + 2];
  KubeInfo tt;
} Kube;

// Cycle four cubies vector elements
inline void Kube_cycleFour(BYTE* Cubies, int c1, int c2, int c3, int c4)
{
  BYTE t = Cubies[c4];
  Cubies[c4] = Cubies[c3], Cubies[c3] = Cubies[c2], Cubies[c2] = Cubies[c1], Cubies[c1] = t;
}

// Half turn moves
inline void Kube_cycleSwap(BYTE* Cubies, int c1, int c2, int c3, int c4)
{
  BYTE t = Cubies[c4];
  Cubies[c4] = Cubies[c2], Cubies[c2] = t;
  t = Cubies[c3], Cubies[c3] = Cubies[c1], Cubies[c1] = t;
}

inline bool IsMiddleSliceEdgeCubie(BYTE cubie)
{
  return ((cubie >> 2) == 2);  // cubie / 4 == 2
}

struct CKube {
public:
  // The cubies data
  Kube tt;

  // Predicate to determine if a cubie is a middle slice edge cubie

  CKube();
  CKube(const CKube* pCube) {
    tt = pCube->tt;
    //memcpy(tt.Cubies, pCube->tt.Cubies, 24);
  }

  // Reset cube back to HOME position
  void BackToHome();

  // Apply face
  void ApplyMove(BYTE face, BYTE power = 1);

  // in phase 2, the moving codes are set for half turn
  inline static bool PowerPhase2(BYTE face) {
    static const bool powerPhase2[] = {true, true, false, false, true, true};
    ASSERT(face < 6);
    return powerPhase2[face];
  }

  // Overloaded equality test operator
  inline bool operator==(const CKube& c) const {
    return memcmp(tt.Cubies, c.tt.Cubies, 24) == 0;
  }
  // Overloaded inequality test operator
  inline bool operator!=(const CKube& c) const {
    return memcmp(tt.Cubies, c.tt.Cubies, 24) != 0;
  }

  // Overloaded equality test operator for compatibility or tests
  bool operator==(const BYTE c[40]) const;

  // assignment operator
  inline CKube operator=(const CKube& c) {
    memcpy(tt.Cubies, c.tt.Cubies, 24);
    return *this;
  }

  // assignment operator
  inline CKube operator=(const BYTE cubies[24]) {
    memcpy(tt.Cubies, cubies, 24);
    return *this;
  }

  // Debug function
  int Dump(char* out, int len) const;
  // void Test();

  // Phase 1 triple

  // Four middle slice edge positions (12 choose 4 = 495)
  WORD Choice() const;
  inline void Choice(WORD choice) {
    ChoicePermutation(choice, tt.tt.EdgePermut);
  }

  // Phase 2 triple

  // Permutation of the 8 corners (8! = 40320)
  inline WORD CornerPermutation() const {
    return PermutToOrdinal(tt.tt.CornerPermut, 8);
  }

  inline void CornerPermutation(WORD ordinal) {
    OrdinalToPermut(ordinal, tt.tt.CornerPermut, 8, 0);
  }

  // Permutation of the 8 non-middle slice edges (8! = 40320)
  inline WORD NonMiddleSliceEdgePermutation() const {
    return PermutToOrdinal(tt.tt.EdgePermut, 8);
  }

  // Note: None of the non middle slice edge cubies are allowed to be in the
  // middle slice prior to calling this function.  If that is not the case,
  // then you must first call Cube::BackToHome().
  inline void NonMiddleSliceEdgePermutation(WORD ordinal) {
    OrdinalToPermut(ordinal, tt.tt.EdgePermut, 8, 0);
  }

  // Permutation of the 4 middle slice edges (4! = 24)
  inline WORD MiddleSliceEdgePermutation() const {
    return PermutToOrdinal(tt.tt.EdgePermut + 8, 4);
  }

  // Note: All of the middle slice edge cubies must be in the middle slice
  // prior to calling prior to calling.  If that is not the case, then you
  // must first call Cube::BackToHome().
  inline void MiddleSliceEdgePermutation(WORD ordinal) {
    OrdinalToPermut(ordinal, tt.tt.EdgePermut + 8, 4, 8);
  }


#ifdef KOCIEMBA
  enum { methodId = 'K' };


  // Compute the choice ordinal from the choice permutation
  WORD ChoiceOrdinal(WORD choicePermut) const;

  // Compute the choice permutation from the choice ordinal
  void ChoicePermutation(WORD ordinal, BYTE choicePermutation[]) const;

#else // Dik Winter's method
  enum { methodId = 'W' };

  void InitializeChoice();
  static bool init;

  // Alternate approach to the above

  // ChoiceOrdinal - Compute a unique ordinal for each of the 495 (i.e. 12
  // Choose 4) possible middle slice edge permutations. This is simply
  // referred to as "choice".  The ordinal is in the range (0...494).

  inline WORD ChoiceOrdinal(WORD choicePermut) const {
    // Map each permutation (of edge cubicles containing a middle slice
    // edge) to the corresponding edge bits to construct an encoded edge
    // choice permutation.  Then lookup the associated choice ordinal.
    return EncodedPermutationToOrdinal[choicePermut];
  }

  // ChoicePermutation - Given a choice ordinal, compute the associated
  // choice permutation.  Cubicles that are not part of the supplied choice
  // are assigned an invalid cubie. Implemented via lookup table.

  inline void ChoicePermutation(WORD ordinal, BYTE choicePermutation[]) const {
    DecodeChoicePermutation(OrdinalToEncodedPermutation[ordinal], choicePermutation);
  }

  // Count the number of bits set in "value"
  inline int CountBits(int value) const {
    int s = value & 1;

    while (value >>= 1) {
      s += value & 1;
    }

    return s;
  }

  void DecodeChoicePermutation(WORD encodedChoicePermutation, BYTE decodedChoicePermutation[]) const;

  static WORD EncodedPermutationToOrdinal[NumberOfEncodedPermutations];
  // Maps the ordinal back to the edge permutation
  static WORD OrdinalToEncodedPermutation[NumberOfOrdinals];

#endif // KOCIEMBA

};


#ifdef KOCIEMBA

// The following algorithm implements the approach taken by Herbert Kociemba
// to map the permutation of the middle slice edges to a unique ordinal
// within the range (0..494).  This approach does not use much memory as no
// lookup tables are employed and it is reasonably efficient.

// ChoiceOrdinal - Compute a unique ordinal for each of the 495 (i.e. 12
// Choose 4) possible middle slice edge permutations.  This is simply
// referred to as "choice".  The ordinal is in the range (0...494) where 0
// corresponds to the [0,1,2,3] edge permutation and 494 corresponds to the
// [8,9,10,11] permutation. The algorithm below is best understood by a
// simple example.

// Consider 6 edges taken 3 at a time.  In lexicographic order, the
// permutations are :

// 0) 012   5 Choose 2 =10 possibilites beginning with 0
// 1) 013   4 Choose 1 = 4 possibilities beginning with 01
// 2) 014
// 3) 015
// 4) 023   3 Choose 1 = 3 possibilities beginning with 02
// 5) 024
// 6) 025
// 7) 034   2 Choose 1 = 2 possibilities beginning with 03
// 8) 035
// 9) 045   1 Choose 1 = 1 possibility beginning with 04
// 10) 123   4 Choose 2 = 6 possibities beginning with 1
// 11) 124   3 Choose 1 = 3 possibilities beginning with 12
// 12) 125
// 13) 134   2 Choose 1 = 2 possibilities beginning with 13
// 14) 135
// 15) 145   1 Choose 1 = 1 possibility beginning with 14
// 16) 234   3 Choose 2 = 3 possibilites beginning with 2
// 17) 235   2 Choose 1 = 2 possibilities beginning with 23
// 18) 245   1 Choose 1 = 1 possibility beginning with 24
// 19) 345   2 Choose 2 = 1 possibility beginning with 3

// Since each permutation is monotonically increasing, it's easy to
// permutation see how to determine the number of possibilities for any given
// prefix.  All edge permutations to the right of a given edge permutations
// must be greater than the given permutation number, so the number of
// remaining choices can be reduced accordingly at that point (e.g. if edge 1
// is present then we are limited to choices 2,3,4,5 in the remaining two
// positions thus there are 4 choose 2 = 6 permutations beginning with 1.

// The edges are first sorted, using a radix sort(see the mark vector below).
// The edges are then scanned in ascending (lexicographic) order. If an edge
// is not present, then the ordinal is increased by the number of possible
// permutations for the current edge choices.

WORD CKube::ChoiceOrdinal(WORD choicePermut) const
{
  int edgesRemaining = 3; // Counts remaining edges (0 to 3)
  WORD ordinal = 0;       // The choice permutation ordinal
  int edge;               // The current edge

  // Scan the edges and compute the ordinal for this permutation
  for (edge = 0; choicePermut; edge++, choicePermut >>= 1) {
    if (choicePermut & 1) {
      edgesRemaining--;
    }
    else {
      ordinal += NChooseM(11 - edge, edgesRemaining);
    }
  }

  return ordinal;
}

// ChoicePermutation - Given a choice ordinal, compute the associated choice
// permutation.  Cubicles that are not part of the supplied choice are
// assigned an invalid cubie.
// The algorithm is essentially the inverse of the permutation to choice
// algorithm.

void CKube::ChoicePermutation(WORD ordinal, BYTE choicePermutation[]) const
{
  int edge;         // The current edge
  int digit = 0;    // The currend edge permutation "digit"
  WORD combinations;// Number of combinations prefixed with this "digit"

  // All other edges are unknown, so begin by initializing them to "invalid"
  for (edge = 0; edge < 12; edge++) {
    choicePermutation[edge] = CKube_invalidCubie;
  }

  // Advance four "digits"
  for (edge = 0; edge < 4; edge++) {

    // This is something like division where we divide by subtracting off
    // the number of combinations possible for the current "digit".
    for (;;) {
      // Initially starting at 0###, so this begins at 11 Choose 3
      // (0 is eliminated leaving 11 possibilites, and there are
      // 3 unassigned "digits")
      // N decreases each time we advance the "digit"
      // M decreases each time we face one "digit" to the right
      combinations = NChooseM(12 - 1 - digit++, 4 - 1 - edge);

      if (ordinal >= combinations) {
        ordinal -= combinations;
      }
      else {
        break;
      }
    }

    // Since digit is always bumped, must back up by one
    // Assign middle slice edges in ascending order
    choicePermutation[digit - 1] = BYTE(8 + edge);
  }
}

#else // Dik Winter's method

bool CKube_init = false;


// And here is an alternate approach...


// The following algorithm implements the approach taken by Dik Winter to map
// the permutation of the middle slice edges to a unique ordinal within the
// range (0..494).  This approach is very fast as it uses lookup tables. The
// lookup tables are not unreasonably large :

// Maps the edge permutations to an ordinal
WORD CKube_EncodedPermutationToOrdinal[NumberOfEncodedPermutations];

// Maps the ordinal back to the edge permutation
WORD CKube_OrdinalToEncodedPermutation[NumberOfOrdinals];


void CKube_DecodeChoicePermutation(WORD encodedPermutation,
    BYTE decodedPermutation[]) const
{
  for (int element = 0, edge = 0; element < 12;
      element++, encodedPermutation >>= 1)
    // Expand the edge bits into an integer vector
    decodedPermutation[element] =
        (encodedPermutation & 1) ? 8 + edge++ : invalidCubie;
}

// InitializeChoice - Initialize the lookup tables for mapping edge choice
// permutations to choice ordinal (and vice versa).

// Here's how it works:

// Consider a bit array of length 12.  Each of the twelve bits represents one
// of the 12 edge locations.  Now treat the bit array as an integer in the
// range (0...4095) and cycle through the entire range.  There will be exactly
// 495 (12 choose 4) values of this integer where exactly four bits (i.e.
// edges) are turned on.  Whenever one of these values is detected, assign it
// to an ordinal and then increment the ordinal since each of these values
// represents a unique permutation of four edges.  We can use these values to
// create two lookup tables, one which maps the edge permutations to an
// ordinal, the other which maps the ordinal back to the edge permutation.
// Thus we have our mapping.

void CKube_InitializeChoice()
{
  // Cycle through all possible 12 bit values
  for (int encodedPermutation = 0, ordinal = 0;
      encodedPermutation < NumberOfEncodedPermutations;
      encodedPermutation++) {
    // If exactly four edges present, then we found a unique permutation of
    // four edges
    if (CountBits(encodedPermutation) == 4) {

      // Add entry to choice permutation to choice ordinal lookup table
      EncodedPermutationToOrdinal[encodedPermutation] = ordinal;
      // Add entry to choice ordinal to choice permutation lookup table
      OrdinalToEncodedPermutation[ordinal++] = encodedPermutation;
    }
    else {

      // to signal bad choicePermutation sequence
      EncodedPermutationToOrdinal[encodedPermutation] = 0xFFFF;
    }
  }
}

// ChoiceOrdinal - Compute a unique ordinal for each of the 495 (i.e. 12
// Choose 4) possible middle slice edge permutations. This is simply referred
// to as "choice".  The ordinal is in the range (0...494).

#endif // KOCIEMBA


// KPruningTable struct definition

// Constructs a pruning table from a pair of move mapping tables. The pruning
// table contains an entry corresponding to each possible pairing of entries
// from the two tables. Thus the number of pruning table entries is the
// product of the number of entries of the two move mapping tables. A breadth
// first search is performed until the table is filled. Each table entry
// contains the number of moves away from the cube home configuration that is
// required to reach that particular configuration.  Since a breadth first
// search is performed, the distances are minimal and therefore they
// constitute an admissible heuristic.  When an admissible heuristic is used
// to prune a heuristic search such as IDA*, the search is guaranteed to find
// an optimal (i.e. least number of moves possible) solution.

// face mapping table classes
struct MvTable {
public:
  MvTable() {
    w = NULL, home = size = 0;
  }

  MvTable(WORD Size) : size(Size) {
    w = (size) ? new WORD[size][6] : NULL, home = 0;
  }

  ~MvTable() {
    if (w) {
      delete [] w;
    }
  }

  inline WORD* operator[](WORD ix) const {
    return w[ix];
  }

  inline void operator=(const MvTable& t) {
    size = t.size;

    if (w) {
      delete [] w;
    }

    w = new WORD[size][6];
    memcpy(w, t.w, size * 6 * 2);
  }

  inline bool operator==(const WORD table[][6]) const {
    return memcmp(w, table, size * 12) == 0;
  }

  // table size in bytes
  inline UINT SizeOf() const {
    return size * 6 * 2;
  }

  // for debug operations
  bool Compare(const WORD table[][6]) const {
    for (WORD i = 0; i < size; i++) {
      for (int j = 0; j < 6; j++) {
        if (w[i][j] != table[i][j]) {
          TRACE("Compare error table %d\n", size);

          if (i > 1) {
            TRACE("Compare err [%d][%d] %04x %04x\n", i - 2, j, w[i - 2][j], table[i - 2][j]);
          }

          if (i > 0) {
            TRACE("Compare err [%d][%d] %04x %04x\n", i - 1, j, w[i - 1][j], table[i - 1][j]);
          }

          TRACE("Compare err [%d][%d] %04x %04x\n", i, j, w[i][j], table[i][j]);

          if (i < size) {
            TRACE("Compare err [%d][%d] %04x %04x\n", i + 1, j, w[i + 1][j], table[i + 1][j]);
          }

          return false;
        }
      }
    }

    return true;
  }

  WORD(*w)[6];
  WORD size;
  WORD home;
};

struct KPruningTable {
public:
  // Constructor - Must provide a pair of move mapping tables and the
  // associated ordinal corresponding to the cube's "home" configuration.
  // The home ordinals correspond to the root node of the search.
  KPruningTable(const MvTable& table1, const MvTable& table2, char id);
  ~KPruningTable();

  // Initialize the pruning table by either generating it or loading it from
  // an existing file
  inline void GenerateOrLoad(FILE* file, bool load) {
    if (load) {
      fread(Table, 1, SizeOf(), file);
    }
    else {
      Generate(), fwrite(Table, 1, SizeOf(), file);
    }
  }

  // Convert a pruning table index to the associated pair of move mapping
  // table indices
  inline void PruningTableIxToMvTableId(UINT ix, WORD& ordi1, WORD& ordi2) const {
    ordi1 = WORD(ix / MvTable2Size);
    ordi2 = WORD(ix % MvTable2Size);
  }

  // Convert a pair of move mapping table indices to the associated pruning
  // table index
  inline UINT MvTableIdToPruningTableIx(WORD ordi1, WORD ordi2) const {
    return ordi1 * MvTable2Size + ordi2;
  }

  // Get a pruning table value corresponding to the specified index
  inline BYTE operator[](UINT ix) const {
    return (ix & 1) ? Table[ix >> 1] >> 4 : Table[ix >> 1] & 0x0F;
  }

  // Set a pruning table value at the specified index
  inline void SetAt(UINT ix, BYTE value) {
    UINT i = ix >> 1;
    Table[i] = (ix & 1) ? (Table[i] & 0x0F) | (value << 4) :
        (Table[i] & 0xF0) | (value & 0x0F);
  }

  // Get a pruning table value corresponding to the specified indices
  inline BYTE GetAt(WORD ix1, WORD ix2) const {
    return (*this)[ix1 * MvTable2Size + ix2];
  }

  // Actual size, in bytes, allocated for the table
  inline int SizeOf() const {
    return (TableSize + 1) / 2;
  }

  // Dump table contents
  void Dump();
  enum { Empty = 0x0f };  // Empty table entry

  // Generate the table using breath first search
  void Generate();

  // Copies of important variables
  const MvTable& MvTable1;
  const MvTable& MvTable2;
  WORD MvTable2Size;
  char tableId;

  // Number of entries in the pruning table
  UINT TableSize;

  // The table pointer
  BYTE(*Table);
};


template <typename T>
struct MTable : public MvTable {
public:
  MTable(bool gen = true) : MvTable(T::size) {
    if (gen) {
      Generate();
    }
  }

  MTable(FILE* file, bool load)
    : MvTable(T::size) {
    GenerateOrLoad(file, load);
  }

  ~MTable() {}

  void GenerateOrLoad(FILE* file, bool load) {
    if (load) {
      fread(w, 1, size * 6 * 2, file);
    }
    else {
      Generate(), fwrite(w, 1, size * 6 * 2, file);
    }
  }

  void Delete() {
    size = 0;

    if (w) {
      delete [] w, w = NULL;
    }
  }


  void Generate() {
    home = T().State();

    for (WORD ordinal = 0; ordinal < size; ordinal++) {
      T obj(ordinal), tmp;

      for (BYTE face = 0; face < 6; face++) {
        tmp = obj;

        tmp.ApplyMove(face);
        w[ordinal][face] = tmp.State();
      }
    }

    TRACE("MTable %c home=%d size=%d Complete\n", T::tableId, home, size);
  }
};



// Phase 1 face mapping table classes

struct MovesPhase1 {
public:
  // MovesPhase1() { initOrient(); }
  ~MovesPhase1() {}

  // to simulate the rotation states of the corner and edge cubies
  void ApplyMove(BYTE face);

  inline void operator=(const MovesPhase1& c) {
    memcpy(cubies, c.cubies, 20);
  }

protected:
  inline void initOrient() {
    memset(cubies, 0, 20);
  }

  // Corners
  inline WORD Twist() const {
    WORD s = cubies[0]; // 2187 steps

    for (int i = 1; i < 7; i++) {
      s = s * 3 + cubies[i];
    }

    return s;
  }

  void Twist(WORD twist);

  // Edges
  inline WORD Flip() const {
    WORD s = cubies[8];  // 2048 steps

    for (int i = 9; i < 19; i++) {
      s = s * 2 + cubies[i];
    }

    return s;
  }

  void Flip(WORD flip);


  // Face rotation in two requests one for the corners, one for the edges
  void cycleFour(int cc1, int cc2, int cc3, int cc4,
      char mod, char op = 0);

  // Rotation states for the corners and edges cubies
  BYTE cubies[20];
};

struct CornerOrient : public MovesPhase1 {
public:
  CornerOrient() {
    initOrient();
  }
  CornerOrient(WORD ordinal) {
    initOrient(), Twist(ordinal);
  }
  inline WORD State() const {
    return Twist();
  }
  enum { size = 2187, tableId = '1' };
};

struct EdgeOrient : public MovesPhase1 {
public:
  EdgeOrient() {
    initOrient();
  }
  EdgeOrient(WORD ordinal) {
    initOrient(), Flip(ordinal);
  }
  inline WORD State() const {
    return Flip();
  }
  enum { size = 2048, tableId = '2' };
};

struct MiddleSliceEdgesPermut {
public:
  MiddleSliceEdgesPermut() {}
  MiddleSliceEdgesPermut(WORD ordinal) {
    cube.Choice(ordinal);
  }

  inline WORD State() const {
    return cube.Choice();
  }

  inline void ApplyMove(BYTE face) {
    cube.ApplyMove(face);
  }

  enum { size = 495, tableId = '3' };

  CKube cube;
};



// Phase 2 face mapping table classes

struct MovesPhase2 {
public:
  inline void ApplyMove(BYTE face) {
    cube.ApplyMove(face, CKube::PowerPhase2(face) ? 2 : 1);
  }

protected:
  CKube cube;
};

struct CornerPermutation : public MovesPhase2 {
public:
  CornerPermutation() {}
  CornerPermutation(WORD ordinal) {
    cube.CornerPermutation(ordinal);
  }

  inline WORD State() const {
    return cube.CornerPermutation();
  }

  enum { size = 40320, tableId = '4' };
};

struct NonMiddleSliceEdgePermutation : public MovesPhase2 {
public:
  NonMiddleSliceEdgePermutation() {}
  NonMiddleSliceEdgePermutation(WORD ordinal) {
    cube.NonMiddleSliceEdgePermutation(ordinal);
  }

  inline WORD State() const {
    return cube.NonMiddleSliceEdgePermutation();
  }

  enum { size = 40320, tableId = '5' };
};

struct MiddleSliceEdgePermutation : public MovesPhase2 {
public:
  MiddleSliceEdgePermutation() {}
  MiddleSliceEdgePermutation(WORD ordinal) {
    cube.MiddleSliceEdgePermutation(ordinal);
  }

  inline WORD State() const {
    return cube.MiddleSliceEdgePermutation();
  }

  enum { size = 24, tableId = '6' };
};



// Corners orientation table
extern MTable<CornerOrient> twistMoveTable;
// Edge orientation table
extern MTable<EdgeOrient> flipMoveTable;
// Middle slice edges permutations
extern MTable<MiddleSliceEdgesPermut> choiceMoveTable;

extern MTable<CornerPermutation> cornerPermutationMoveTable;
extern MTable<NonMiddleSliceEdgePermutation> nonMiddleSliceEdgePermutationMoveTable;
extern MTable<MiddleSliceEdgePermutation> middleSliceEdgePermutationMoveTable;

// Phase 1 pruning tables
extern KPruningTable TwistAndFlipPruningTable;
extern KPruningTable TwistAndChoicePruningTable;
extern KPruningTable FlipAndChoicePruningTable;

// Phase 2 pruning tables
extern KPruningTable CornerAndSlicePruningTable;
extern KPruningTable EdgeAndSlicePruningTable;


// Moving and pruning Tables

// Phase 1 face mapping tables
MTable<CornerOrient>          twistMoveTable;
MTable<EdgeOrient>            flipMoveTable;
MTable<MiddleSliceEdgesPermut>choiceMoveTable;

// Phase 2 face mapping tables
MTable<CornerPermutation> cornerPermutationMoveTable(false);
MTable<NonMiddleSliceEdgePermutation> nonMiddleSliceEdgePermutationMoveTable(false);
MTable<MiddleSliceEdgePermutation> middleSliceEdgePermutationMoveTable(false);

// Phase 1 pruning tables
KPruningTable TwistAndFlipPruningTable(twistMoveTable, flipMoveTable, 'a');
KPruningTable TwistAndChoicePruningTable(twistMoveTable, choiceMoveTable, 'b');
KPruningTable FlipAndChoicePruningTable(flipMoveTable, choiceMoveTable, 'c');

// Phase 2 pruning tables
KPruningTable CornerAndSlicePruningTable(cornerPermutationMoveTable,
    middleSliceEdgePermutationMoveTable, 'D');
KPruningTable EdgeAndSlicePruningTable(nonMiddleSliceEdgePermutationMoveTable,
    middleSliceEdgePermutationMoveTable, 'E');

// Class MovesPhase1
// generation of the tables

// Apply move to face
void MovesPhase1::ApplyMove(BYTE face)
{
  switch (face) {
  case CKube_R :
    cycleFour(CKube_URF, CKube_UBR, CKube_DRB, CKube_DFR, 3, 1);
    cycleFour(CKube_UR,  CKube_BR,  CKube_DR,  CKube_RF,  2, 1);
    break;

  case CKube_L :
    cycleFour(CKube_ULB, CKube_UFL, CKube_DLF, CKube_DBL, 3, 1);
    cycleFour(CKube_UL,  CKube_FL,  CKube_DL,  CKube_LB,  2, 1);
    break;

  case CKube_U :
    cycleFour(CKube_ULB, CKube_UBR, CKube_URF, CKube_UFL, 3, 0);
    cycleFour(CKube_UB,  CKube_UR,  CKube_UF,  CKube_UL,  2, 0);
    break;

  case CKube_D :
    cycleFour(CKube_DLF, CKube_DFR, CKube_DRB, CKube_DBL, 3, 0);
    cycleFour(CKube_DF,  CKube_DR,  CKube_DB,  CKube_DL,  2, 0);
    break;

  case CKube_F :
    cycleFour(CKube_UFL, CKube_URF, CKube_DFR, CKube_DLF, 3, 1);
    cycleFour(CKube_UF,  CKube_RF,  CKube_DF,  CKube_FL,  2, 0);
    break;

  case CKube_B :
    cycleFour(CKube_UBR, CKube_ULB, CKube_DBL, CKube_DRB, 3, 1);
    cycleFour(CKube_UB,  CKube_LB,  CKube_DB,  CKube_BR,  2, 0);
    break;
  }
}

void MovesPhase1::cycleFour(int cc1, int cc2, int cc3, int cc4,
    char mod,     // modulo of rotation (2, 3)
    char op)      // operator argument (0, 1, 2)
{
  BYTE temp   = (cubies[cc4] + op + mod) % mod;
  cubies[cc4] = (cubies[cc3] - op + mod) % mod;
  cubies[cc3] = (cubies[cc2] + op + mod) % mod;
  cubies[cc2] = (cubies[cc1] - op + mod) % mod;
  cubies[cc1] = temp;
}

void MovesPhase1::Twist(WORD twist)
{
  int i, sum;       // For calculating corner parity

  for (sum = 24, i = 7; --i >= 0; twist /= 3) {
    sum -= (cubies[i] = BYTE(twist % 3));
  }

  // Derive the flip of the last edge cubie from the current total corner
  // parity.  (total corner parity must be a multiple of 3) 3-ParitySum%3 is
  // the amount to add to round up to the next multiple of 3.  Note that in
  // the case where ParitySum%3 equals zero we want the amount to be zero,
  // not three.  Therefore we compute (3-ParitySum%3)%3.
  cubies[7] = (BYTE)sum % 3;
}

void MovesPhase1::Flip(WORD flip)
{
  int i, sum;       // For calculating edge parity

  for (sum = 24, i = 19; --i >= 8; flip >>= 1) {
    sum -= (cubies[i] = BYTE(flip & 1));
  }

  // Derive the flip of the last edge cubie from the current total edge
  // parity (total edge parity must be even).
  cubies[19] = (BYTE)sum & 1;
}


CKube::CKube()
{
  ASSERT(twistMoveTable[0][0] != 0 && flipMoveTable[0][0] != 0);
#ifndef KOCIEMBA

  // Initialize the choice lookup tables (Dik Winter's method)
  if (!init) {
    InitializeChoice(), init = true;
  }

#endif
  BackToHome();
}


// Reset cube back to HOME position
void CKube::BackToHome()
{
  tt.tt.twist = tt.tt.flip = 0;

  for (int i = 12; --i >= 0;) {
    tt.tt.CornerPermut[i] = tt.tt.EdgePermut[i] = (BYTE)i;
  }
}

// Apply move to face
void CKube::ApplyMove(BYTE face, BYTE power)
{
  ASSERT(face < 6 && power > 0 && power < 4);

  while (--power < 3) {
    tt.tt.twist = twistMoveTable[tt.tt.twist][face];
    tt.tt.flip = flipMoveTable[tt.tt.flip][face];

    switch (face) {
    case CKube_R :
      Kube_cycleFour(tt.Cubies, CKube_URF, CKube_UBR, CKube_DRB, CKube_DFR), Kube_cycleFour(tt.Cubies, CKube_UR, CKube_BR, CKube_DR, CKube_RF);
      break;

    case CKube_L :
      Kube_cycleFour(tt.Cubies, CKube_ULB, CKube_UFL, CKube_DLF, CKube_DBL), Kube_cycleFour(tt.Cubies, CKube_UL, CKube_FL, CKube_DL, CKube_LB);
      break;

    case CKube_B :
      Kube_cycleFour(tt.Cubies, CKube_UBR, CKube_ULB, CKube_DBL, CKube_DRB), Kube_cycleFour(tt.Cubies, CKube_UB, CKube_LB, CKube_DB, CKube_BR);
      break;

    case CKube_F :
      Kube_cycleFour(tt.Cubies, CKube_UFL, CKube_URF, CKube_DFR, CKube_DLF), Kube_cycleFour(tt.Cubies, CKube_UF, CKube_RF, CKube_DF, CKube_FL);
      break;

    case CKube_U :
      Kube_cycleFour(tt.Cubies, CKube_ULB, CKube_UBR, CKube_URF, CKube_UFL), Kube_cycleFour(tt.Cubies, CKube_UB, CKube_UR, CKube_UF, CKube_UL);
      break;

    case CKube_D :
      Kube_cycleFour(tt.Cubies, CKube_DLF, CKube_DFR, CKube_DRB, CKube_DBL), Kube_cycleFour(tt.Cubies, CKube_DF, CKube_DR, CKube_DB, CKube_DL);
      break;
    }
  }
}

// Choice of the four middle slice edge positions.
// Note that "choice" is for the permutation of those edge cubicles occupied
// by middle slice edge cubies, as opposed to a choice of all edge cubies
// within edge cubicles.

WORD CKube::Choice() const
{
  static const WORD edgeBits[12] = { 1 << 0, 1 << 1, 1 << 2, 1 << 3, 1 << 4,
      1 << 5, 1 << 6, 1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11
                                   };
  WORD choicePermut = 0;  // Permutation of the four middle slice edges

  // Scan for middle slice edges to construct the choice permutation vector
  for (int edge =  0; edge < 12; edge++) {
    if (IsMiddleSliceEdgeCubie(tt.tt.EdgePermut[edge])) {
      choicePermut |= edgeBits[edge];
    }
  }

  return ChoiceOrdinal(choicePermut);
}
// Debug function
int CKube::Dump(char* msg, int len) const
{
  static const char* sep = " ";
  static const char* cornerN[24] = {
    "URF", "UFL", "ULB", "UBR", "DFR", "DLF", "DBL", "DRB",
    "RFU", "FLU", "LBU", "BRU", "FRD", "LFD", "BLD", "RBD",
    "FUR", "LUF", "BUL", "RUB", "RDF", "FDL", "LDB", "BDR"
  };
  static const char* edgeN[24] = {
    "UF", "UL", "UB", "UR", "DF", "DL", "DB", "DR", "RF", "FL", "LB", "BR",
    "FU", "LU", "BU", "RU", "FD", "LD", "BD", "RD", "FR", "LF", "BL", "RB"
  };
  int i, tw, fl, sum;    // For calculating corner parity
  BYTE temp[12];

  for (sum = 24, i = 7, tw = tt.tt.twist; --i >= 0; tw /= 3) {
    sum -= (temp[i] = (BYTE)(tw % 3));
  }

  temp[7] = sum % 3;

  STRcpy(msg, len, cornerN[temp[0]], -1);

  for (i = 1; i < 8; i++) {
    STRcat2(msg, len, sep, -1, cornerN[tt.tt.CornerPermut[i] + temp[i] * 8], -1);
  }

  for (sum = 24, i = 11, fl = tt.tt.flip; --i >= 0; fl >>= 1) {
    sum -= (temp[i] = fl & 1);
  }

  temp[11] = sum & 1;

  STRcat(msg, len, sep, -1);

  for (i = 0; i < 12; i++) {
    STRcat2(msg, len, sep, -1, edgeN[tt.tt.EdgePermut[i] + temp[i] * 12], -1);
  }

  return 0;
}


// *******************************
// *  Kociemba Cube Solver 1.0   *
// * - written by Greg Schmidt - *
// *******************************

// Why write another cube solver that implements Kociemba's algorithm ?
// Well I wanted to develop a more complete understanding of the algorithm
// and, as Mike Reid suggested, one way to do that is to write a program to
// implement the algorithm.  So this program was written primarily as a
// learning exercise.  It offers a simple console interface and has no fancy
// graphics although I have tried to make cube entry simple.  However, if you
// are more interested in the the algorithm itself, it is my hope that you
// will find this program useful as I have tried to document it well enough
// so that others may use this program as a reference for under-standing how
// the algorithm works.  If you are completely unfamiliar with Kociemba's
// algorithm, you may first want to browse through the cube.lovers archive at
// ftp://ftp.ai.mit.edu/pub/cube-lovers as there are several messages there
// describing the basic principles.

//Files
//-----

// KMCube.*    - Kociemba Cube Solver main program
// KFacelet.*  - KFacelet struct, validates the a given cube configuration in
// its facelet representation. Also converts the cube to a
// cubie permutation/orientation form.
// Kube.*      - CKube struct, basic Rubik's Cube model and extensions to
// support Kociemba's algorithm.
// KSolver.*   - CSolver struct, performs the two phase search a solution.
// KState.*    - CSolver struct, stores the steps of the resolution.
// KTables.*   - MvTable struct, generates and manages the move mapping tables
// KPruningt.* - KPruningTable struct, generates and manages the pruning tables


// I would like to thank Herbert Kociemba for his patience in providing me
// with enough details about his algorithm to allow me to write this program.
// I would also like to credit Dik Winter as I implemented a version of his
// choice numbering algorithm in the "KociembaCube" struct, as an alternate
// approach, for illustration.


// Feedback of any kind, both positive and negative, regarding this program
// is welcome :                              Greg Schmidt
// Greg.Schmidt@ab.com                 498 Catlin Rd.
// Work: (216) 646-3597                Richmond Hts., OH  44143
// Home: (216) 481-6749                USA



// To measure and display the time for a resolution validate this macro

// Solver return codes
enum {
  NOT_FOUND,        // A solution was not found (0)
  FOUND,            // A solution was found
  OPTIMUM_FOUND,    // An optimal solution was found
  ABORT,            // The search was aborted
  // (i.e. phase 2 did not yield an improved solution)
  HOME,
  USER_ABORT        // user request
};
enum { maxState = 26 }; // length of step table


// Class to store the current state of the resolution of Rubik's Cube

// One step of the resolution
struct Step {
public:
  WORD twist, flip, choice;        // Cube ordinals phase 1
  WORD corner, nmsEdge, msEdge;    // Cube ordinals phase 2
  BYTE face, power;                // Rubik's Cube orders

  // Construction/Destruction
  Step() {
    power = face = 0x80;
  }
  ~Step() {}

  // Update the Rubik's Cube with this order
  inline void ApplyMove(CKube& cube) const {
    cube.ApplyMove(face, power);
  }

  // Translates moves (quarter turn moves 0 to 5) from a internal (face,
  // power) representation to a single code representation (e.g. R,3
  // becomes cRr).
  inline t_Command Move() const {
    static const BYTE translateMove[4][6] = {
      { 0, 0, 0, 0, 0, 0},
      { Move_cR,  Move_cL,  Move_cU, Move_cD, Move_cF, Move_cB },
      { Move_cR2, Move_cL2, Move_cU2, Move_cD2, Move_cF2, Move_cB2 },
      { Move_cRi, Move_cLi, Move_cUi, Move_cDi, Move_cFi, Move_cBi }
    };
    ASSERT(face < 6 && power > 0 && power < 4);
    return translateMove[power][face];
  }

  // Management ordinals

  inline Step* Phase1Set(const CKube& cube) {
    twist = cube.tt.tt.twist, flip = cube.tt.tt.flip, choice = cube.Choice();
    return this;
  }

  inline Step* Phase1Set(Step* pN, BYTE Move) {
    face = Move, power = 1, twist = twistMoveTable[pN->twist][face],
    flip = flipMoveTable[pN->flip][face],
    choice = choiceMoveTable[pN->choice][face];
    return this;
  }

  inline Step* Phase2Set(const CKube& cube) {
    corner = cube.CornerPermutation(),
    nmsEdge = cube.NonMiddleSliceEdgePermutation(),
    msEdge = cube.MiddleSliceEdgePermutation();
    return this;
  }

  inline Step* Phase2Set(Step* pN, BYTE Move, BYTE Power) {
    face = Move, power = Power,
    corner = cornerPermutationMoveTable[pN->corner][face],
    nmsEdge = nonMiddleSliceEdgePermutationMoveTable[pN->nmsEdge][face],
    msEdge = middleSliceEdgePermutationMoveTable[pN->msEdge][face];
    return this;
  }

  // Progress in the tables defining the state of the Rubik's Cube

  inline void Phase1Power() {
    power++;
    ASSERT(power > 1 && power < 4);
    twist = twistMoveTable[twist][face],
    flip = flipMoveTable[flip][face],
    choice = choiceMoveTable[choice][face];
  }

  inline void Phase2Power() {
    power++;
    ASSERT(power > 1 && power < 4);
    corner = cornerPermutationMoveTable[corner][face],
    nmsEdge = nonMiddleSliceEdgePermutationMoveTable[nmsEdge][face],
    msEdge = middleSliceEdgePermutationMoveTable[msEdge][face];
  }

  // Combining admissible heuristics by taking their maximum produces an
  // improved admissible heuristic.

  inline BYTE Phase1Cost() const {
    BYTE cost  = TwistAndFlipPruningTable.GetAt(twist, flip);
    BYTE cost1 = TwistAndChoicePruningTable.GetAt(twist, choice);
    BYTE cost2 = FlipAndChoicePruningTable.GetAt(flip, choice);

    if (cost1 > cost) {
      cost = cost1;
    }

    return (cost2 > cost) ? cost2 : cost;
  }

  inline BYTE Phase2Cost() const {
    BYTE cost  = CornerAndSlicePruningTable.GetAt(corner, msEdge);
    BYTE cost1 = EdgeAndSlicePruningTable.GetAt(nmsEdge, msEdge);
    return cost1 > cost ? cost1 : cost;
  }
};

// Minimum solution length found so far
static BYTE minSolutionLength = 0;   // Minimum solution length found so far

typedef struct CSolver {
  // struct used in second solution search task
  bool invert;

  // for multi search
  CKube cube;

  // Perform the two phase search subroutines

  // Current heuristic threshold (cutoff)
  BYTE threshold1, threshold2;
  // New threshold as determined by current search pass
  BYTE newThreshold1, newThreshold2;

  bool m_kAbort;
  // current resolution states
  Step* pCur, *pCur1, states[maxState];

  // Cube to solve
  const CKube* pCube;

  // Resolution of the inverse Cube
  bool revers;
  t_Matrix orient;

  char old[256];
} CSolver;
typedef Step* PSTATE;

void CSolver_Set(CSolver* s, bool rev = false)
{
  s->pCur = s->pCur1 = NULL, s->pCube = NULL,
     s->revers = rev;
  s->orient = 1;
  s->m_kAbort = false;
}

inline void CSolver_Init(CSolver* s)
{
  s->pCur = s->pCur1 = s->states + 1;
}

inline void CSolver_Init(CSolver* s, const CKube& scrambledCube, t_Matrix or = 1)
{
  s->pCube = &scrambledCube, CSolver_Init(s), s->orient = or ,
     s->states[1].Phase1Set(scrambledCube);
}

inline Step* CSolver_Phase1Next(CSolver* s, BYTE face)
{
  Step* pOld = s->pCur++;
  ASSERT(s->pCur > s->states && s->pCur < s->states + maxState);
  return s->pCur->Phase1Set(pOld, face);
}

inline void CSolver_Phase1Power(CSolver* s, Step* pS)
{
  s->pCur = pS, pS->Phase1Power();
}

// Phase 1 cost heuristics
inline BYTE CSolver_Phase1Cost(const CSolver* s)
{
  return s->pCur->Phase1Cost();
}

// Applies to the Rubik's Cube the solution found
inline void CSolver_ApplyMove(const CSolver* s, CKube& cube)
{
  for (const Step* pC = s->states + 1; ++pC <= s->pCur;) {
    pC->ApplyMove(cube);
  }
}

// Phase 2
inline Step* CSolver_Phase2Create(CSolver* s)
{
  CKube cube(s->pCube);
  CSolver_ApplyMove(s, cube);
  s->pCur1 = s->pCur;
  return s->pCur->Phase2Set(cube);
}

inline Step* CSolver_Phase2Next(CSolver* s, BYTE face, BYTE power = 1)
{
  Step* pOld = s->pCur++;
  ASSERT(s->pCur > s->states && s->pCur < s->states + maxState);
  return s->pCur->Phase2Set(pOld, face, power);
}

inline void CSolver_Phase2Power(CSolver* s, Step* pS)
{
  s->pCur = pS, pS->Phase2Power();
}

// Phase 2 cost heuristics
inline BYTE CSolver_Phase2Cost(const CSolver* s)
{
  return s->pCur->Phase2Cost();
}

inline BYTE CSolver_Depth(const CSolver* s)
{
  return BYTE(s->pCur - s->states) - 1;
}
inline BYTE CSolver_Phase1Depth(const CSolver* s)
{
  return BYTE(s->pCur1 - s->states) - 1;
}
inline BYTE CSolver_Phase2Depth(const CSolver* s)
{
  return BYTE(s->pCur - s->pCur1);
}


// Predicate to determine if a face is redundant (leads to a node that is
// explored elsewhere) and should therefore be disallowed.
inline bool CSolver_Disallowed(const CSolver* s, BYTE face)
{
  static const BYTE select[6] = { CKube_L, CKube_noop,// R, L
      CKube_D, CKube_noop, CKube_B, CKube_noop
                                };// U, D, F, B
  // Disallow successive moves of a single face (RR2 is same as R')
  // or disallow a face of an opposite face if the current face moved
  // is B,L, or D. (BF, LR, DU are same as FB,RL,UD)
  return (s->pCur->face == face || select[face] == s->pCur->face
      // Disallow 3 or more consecutive moves of opposite faces (UDU is
      // same as DU2 and U2D)
      || (s->pCur[-1].face == face && s->pCur->face == (face ^ 1)));
}

// Extract the solution of the search sequence
inline int CSolver_Solution(const CSolver* s, char* cmd, int len)
{
  cmd[0] = 0;

  for (const Step* pC = s->states + 1; ++pC <= s->pCur;) {
    STRcat_c(cmd, len, pC->Move());
  }

  if (s->orient > 1) {
    CmdRotate(cmd, len, cmd, s->orient);
  }

  if (s->revers) {
    CmdReverse(cmd);
  }

  return 0;
}

void KPrint(int noLine, const char* text)
{
  printf("%s%s", text, " \r\n \r\n \r\n \r\n \r\n");
  //TRACE("Print%d %s\n", noLine, text);
}

// Put the solution of the search sequence
int CSolver_PrintSolution(CSolver* s)
{
  char cmd[256] = {0};
  char text[256] = {0};
  char tmp[256] = {0};
  char tmp2[256] = {0};
  CSolver_Solution(s, cmd, 256);

  // When you find the same solution several times, this indicates that this
  // is the best and it prevents a deadly loop
  if (0 != strcmp(cmd, s->old)) {
    strcpy(s->old, cmd);
  }
  else {
    return OPTIMUM_FOUND;
  }

  // display only the number of steps for the solution
  _snprintf(tmp, 256, " (%d)", CSolver_Depth(s));
  // translate the command string
  CmdConvert(tmp2, 256, cmd, false);
  text[0] = 0;
  STRcat2(text, 256, tmp2, -1, tmp, -1);
  TRACE("CSolver_PrintSolution%d %d.%d >%s\n", s->revers, CSolver_Depth(s), CSolver_Phase1Depth(s), text);
  // put the solution

  if (minSolutionLength > CSolver_Depth(s)) {
    KPrint(2, text), minSolutionLength = CSolver_Depth(s);
  }

  s->m_kAbort = 1;

  return NOT_FOUND;
}

KPruningTable::KPruningTable(const MvTable& mvTable1,
    const MvTable& mvTable2,
    char id)
  : MvTable1(mvTable1),
    MvTable2(mvTable2),
    tableId(id)
{
  // Initialize table sizes
  MvTable2Size = MvTable2.size;
  TableSize = MvTable1.size * MvTable2Size;

  // Allocate the table round up to an int and determine the number of bytes
  // to be allocated
  // The following seems like it should work, but leads to deallocation
  // problems when TwstChce.ptb is regenerated -- why?
  // AllocationSize = TableSize/2;
  Table = new BYTE[SizeOf()];
}

KPruningTable::~KPruningTable()
{
  // Deallocate table storage
  delete [] Table;
}


// Performs a breadth first search to fill the pruning table

void KPruningTable::Generate()
{
  UINT HomeIndex = MvTableIdToPruningTableIx(MvTable1.home, MvTable2.home);
  BYTE depth, move, power, powerLimit;
  bool Phase2 = isupper(tableId) != 0;
  UINT numberOfNodes, old;                  // Number of nodes generated
  WORD ordinal1, ordinal2;                  // Tables coordinates
  UINT index, childIx;                      // Generated table indices
  char msg[256] = {0};

  // Initialize all tables entries to "empty"
  memset(Table, 0xFF, SizeOf());

  // Get root coordinates of search tree and initialize to zero
  SetAt(HomeIndex, 0), numberOfNodes = 1; // Count root node here

  // While empty table entries exist...
  for (depth = 0; numberOfNodes < TableSize; depth++) {
    ASSERT(depth < 14);

    old = numberOfNodes;

    // Scan all entries looking for entries corresponding to the current
    // depth
    for (index = 0; index < TableSize; index++) {
      // Expand the nodes at the current depth only
      if ((*this)[index] != depth) {
        continue;
      }

      // Apply each possible move
      for (move = 0; move < 6; move++) {
        powerLimit = (Phase2 && CKube::PowerPhase2(move)) ? 2 : 4;

        // Computs the ordinals corresponding to the index
        PruningTableIxToMvTableId(index, ordinal1, ordinal2);

        // Apply each of the three quarter turns
        for (power = 1; power < powerLimit; power++) {

          // Use the move mapping table to find the child node
          ordinal1 = MvTable1[ordinal1][move];
          ordinal2 = MvTable2[ordinal2][move];
          childIx = MvTableIdToPruningTableIx(ordinal1, ordinal2);

          // Update previously unexplored nodes only
          if ((*this)[childIx] == Empty) {
            SetAt(childIx, depth + 1), numberOfNodes++;
          }
        }
      }
    }

    _snprintf(msg, 256, "/ Table %c, Completed Depth = %d, nodes = %d, rest = %d",
        tableId, depth, numberOfNodes - old,
        TableSize - numberOfNodes);
    KPrint(2, msg);
  }
}


// Output the pruning table in human readable form

void KPruningTable::Dump()
{
  TRACE("KPruningTable::Dump table_%c\n", tableId);

  for (UINT index = 0; index < TableSize; index++) {
    TRACE("%8d : %2d\n", index, (*this)[index]);
  }
}





// FaceletCube
// Cube Solver struct definition

// This struct performs a two phase IDA* search for a solution to the
// scrambled cube.

// Phase 1 searches the group spanned by <U,D,R,L,F,B> until a configuration
// is discovered where the three coordinates of twist, flip, and choice are
// "correct" with respect to a solved cube. This means that no edge cubie is
// twisted, no corner cubie is flipped, and the four middle slice edge cubies
// are in the middle slice (but not necessarily in their correct permutation
// within that slice).  At this point, we have found a member of an element
// of the phase two group.

// Phase 2 uses the resulting phase 1 configuration as the starting point for
// a search of the group spanned by <U,D,R2,L2,F2,B2>,the goal being to reach
// <I>, the identity (i.e. the solved configuration).  Note that this group
// preserves the three coordinates of the phase 1 search since in phase 2 it
// is impossible to alter the twist, flip, or choice aspects of the cube.
// The U, D moves do not alter corner or edge parity and do not affect the
// choice of the middle slice edge cubies. The same is true for the R2, L2,
// F2, B2 moves.  This can be verified by considering the effect these moves
// have on cube parity (see cube.cpp for details on the parity frame of
// reference used).

// In search parlance, the pruning tables (a.k.a. pattern databases)
// constitute an "admissible" heuristic.  This means that they always
// underestimate the distance (i.e. number of moves required) to reach the
// goal state.  It can be proven that any search, such as IDA*, that examines
// nodes at progressively increasing cost values and employs an admissible
// heuristic is "optimal". This means that the first solution found by the
// search is guaranteed to be of minimal distance required to reach the
// target, or goal, state.

// Since the search is split into two sequential IDA* search phases, the
// optimality condition above does not always hold. However, since we allow
// the phase 1 search to iteratively deepen, if let run long enough, it will
// eventually deepen to the point where it is capable of finding a complete
// solution. At this point, we know we have an optimal solution as we have
// degenerated to a single IDA* search of the cube space, but this takes a
// very long time to occur. The main strength of the two phase search is that
// it finds a near optimal solution very quickly and outputs successively
// better solutions until it eventually finds one that is optimal. In most
// cases though, the search is terminated early (due to lack of patience)
// once an "adequate" solution is found.

// For more information concerning IDA* and admissibility, see the paper
// "Depth-First Iterative-Deepening: An Optimal Admissable Tree Search" by
// Richard E. Korf. This paper appears in volume 25 of "Artificial
// Intelligence" 1985, pp. 97-109.  Also, there are many texts on AI
// (Artificial Intelligence) or books on search techniques that cover these
// topics in depth.

// The original algorithm has been rewritten with a new code, improves the
// search speed. Then, in a second time, it is supplemented by taking
// advantage of multiprocessor performance of new PCs. Indeed, the first
// solution found to create a new Cube symmetrical to the original, which
// then search for a solution is conducted in parallel, allowing faster to
// find the shortest solution.

bool tablesLoaded = false;

void CSolver_Init(CSolver* s, bool inv)
{
  CSolver_Set(s, !inv);
  ASSERT(tablesLoaded);
  s->invert = inv;
}

// Perform the two phase search procedure
//int Solve(const CKube& scrambledCube, t_Matrix or = 1);



// KFacelet struct definition

// The KFacelet represents the cube by the markings of the 54 individual
// facelets.  The KFaceCube can then be asked to validate the cube to
// determine if it is in a legal, and thus solvable, configuration.

// Validation return codes
enum {
  KFacelet_VALID,
  KFacelet_INVALID_MARKER,
  KFacelet_INVALID_FACELETCOUNT,
  KFacelet_DUPLICATE_CENTER_MARKING,
  KFacelet_INVALID_CORNER_MARKINGS,
  KFacelet_INVALID_CORNER_PARITY,
  KFacelet_INVALID_EDGE_MARKINGS,
  KFacelet_INVALID_EDGE_PARITY,
  KFacelet_INVALID_TOTAL_PARITY,
  NumberOfErrors
};

struct KFacelet : public CKube {
public:
  KFacelet(const char* msg);

  // Validate markings, permutation, and parity
  int Validate();

  // Return the text associated with an error return code
  static char* ErrorText(unsigned int error);

  // Validation sub functions
  int ValidateCenters();
  int ValidateFacelets();
  int ValidateCorners();
  int ValidateEdges();
  int PermutationParity();

  int PermutationParity(const BYTE permutations[], int numberOfCubies)const;

  int FaceletOffsetToMarking(int offset) const;

  // The 9 markings for each of the 6 faces
  char faceletMarkings[6 * 9];
  // Markings mapped to each face
  char faceMarkings[6];
  // The cubies values
  BYTE CornerPermut[8], EdgePermut[12];
  WORD twist, flip;

  static char* errorText[NumberOfErrors];
};

void InitializeTables(const char* fileName)
{
  static const char ident[6] = {'K', 'S', CKube::methodId, '1', '2', 0};
  int tableSize;
  char rdId[6];
  FILE* file = NULL;
  bool load;

  if (!tablesLoaded) {
    tableSize = 6     // ident + type + version
        // Phase 2 face mapping tables
        + cornerPermutationMoveTable.SizeOf()
        + nonMiddleSliceEdgePermutationMoveTable.SizeOf()
        + middleSliceEdgePermutationMoveTable.SizeOf()

        // Phase 1 pruning tables
        + TwistAndFlipPruningTable.SizeOf()
        + TwistAndChoicePruningTable.SizeOf()
        + FlipAndChoicePruningTable.SizeOf()

        // Phase 2 pruning tables
        + CornerAndSlicePruningTable.SizeOf()
        + EdgeAndSlicePruningTable.SizeOf();

    // Create or read the KSolver tables
    file = fopen(fileName, "rb");

    if (!(load = (file && fsize(file) == tableSize && fread(rdId, 1, 6, file)
        && memcmp(ident, rdId, 6) == 0))) {

      if (file == NULL) {
        KPrint(1, "/ Tables not found, generate it");
      }
      else {
        fclose(file), KPrint(1, "/ Invalid tables, update it");
      }

      // create and write a new tables
      file = fopen(fileName, "wb");
      fwrite(ident, 6, 1, file);
      fclose(file);
    }

    // Phase 2 face mapping tables
    cornerPermutationMoveTable.GenerateOrLoad(file, load);
    nonMiddleSliceEdgePermutationMoveTable.GenerateOrLoad(file, load);
    middleSliceEdgePermutationMoveTable.GenerateOrLoad(file, load);

    // Phase 1 pruning tables
    TwistAndFlipPruningTable.GenerateOrLoad(file, load);
    TwistAndChoicePruningTable.GenerateOrLoad(file, load);
    FlipAndChoicePruningTable.GenerateOrLoad(file, load);

    // Phase 2 pruning tables
    // Obviously a CornerAndEdgePruningTable doesn't make sense as it's
    // size would be extremely large (i.e. 8!*8!)

    CornerAndSlicePruningTable.GenerateOrLoad(file, load);
    EdgeAndSlicePruningTable.GenerateOrLoad(file, load);

    tablesLoaded = true;
  }

  minSolutionLength = maxState - 1;
  KPrint(1, "/ Tables init done");
}

int CSolver_Search1(CSolver* s);
int CSolver_Solve2(CSolver* s);
int CSolver_Search2(CSolver* s);

// To solve the Rubik's Cube
int CSolver_Solve(CSolver* s, const CKube& scrambledCube,  // Cube to solve
    t_Matrix or)                 // Cube orientation

{
  int result, iteration = 0;    // interative deepenings count

  CSolver_Init(s, scrambledCube, or);

  // Establish initial cost estimate to goal state
  s->threshold1 = CSolver_Phase1Cost(s);

  CSolver_Phase2Create(s);

  if (s->threshold1 + CSolver_Phase2Cost(s) == 0) {
    KPrint(2, "/ Cube already ordered");
    return HOME;
  }

  do {
    s->newThreshold1 = 255;        // Any cost will be less than this
    CSolver_Init(s);

    // Perform the phase 1 recursive IDA* search
    result = CSolver_Search1(s);

    // Establish a new threshold for a deeper search
    s->threshold1 = s->newThreshold1;
  }
  while (result == NOT_FOUND);

  if (!s->revers) {
    KPrint(3, result == USER_ABORT ? "/ Abort requested" : "/ Complete");
  }

  return result;
}

// Task and associated mechanism for solving the Rubik's Cube

int CSolver_Search2(CSolver* s)
{
  // Compute cost estimate to goal state
  BYTE totalCost, face, cost = CSolver_Phase2Cost(s);  // h
  int result;

  if (cost == 0) {
    // Solution found...

    result = CSolver_PrintSolution(s); // Output the solution

    if (s->invert && result == NOT_FOUND) {
      // Launches a new task of research
      s->invert = false;
      // Mix the Cube with the command sequence already found
      CSolver_ApplyMove(s, s->cube);

      if (s->cube != s->pCube) {
        CSolver s1;
        CSolver_Init(&s1, false);
        CSolver_Solve(&s1, &s->cube, 1);
      }
    }

    return result;
  }

  if (s->m_kAbort) {
    return USER_ABORT;
  }

  // See if node should be expanded (g + h)
  if ((totalCost = CSolver_Phase2Depth(s) + cost) <= s->threshold2) {
    // Expand node

    // No point in continuing to search for solutions of equal or greater
    // length than the current best solution
    if (CSolver_Depth(s) + 1 >= minSolutionLength) {
      return OPTIMUM_FOUND;
    }

    for (face = 0; face < 6; face++) {
      if (CSolver_Disallowed(s, face)) {
        continue;
      }

      if (CKube::PowerPhase2(face)) {

        // the moving tables are set for half turn for this face
        PSTATE pS = CSolver_Phase2Next(s, face, 2);

        if (result = CSolver_Search2(s)) {
          return result;
        }

        s->pCur = (pS - 1);
      }
      else {

        PSTATE pS = CSolver_Phase2Next(s, face);// power = 1

        if (result = CSolver_Search2(s)) {
          return result;
        }

        CSolver_Phase2Power(s, pS);                      // power = 2

        if (result = CSolver_Search2(s)) {
          return result;
        }

        CSolver_Phase2Power(s, pS);                      // power = 3

        if (result = CSolver_Search2(s)) {
          return result;
        }

        s->pCur = (pS - 1);
      }
    }

  }
  else if (totalCost < s->newThreshold2) {
    // Maintain minimum cost exceeding threshold
    s->newThreshold2 = totalCost;
  }

  return NOT_FOUND;
}

int CSolver_Solve2(CSolver* s)
{
  // We need an appropriately initialized cube in order to begin phase 2.
  // First, create a new cube that is a copy of the initial scrambled
  // cube.  Then we apply the phase 1 face sequence to that cube. The
  // phase 2 search can then determine the initial phase 2 coordinates
  // (corner, edge, and slice permutation) from this cube.
  PSTATE pS = CSolver_Phase2Create(s);
  int result;

  // Note : No attempt is made to merge moves of the same face adjacent
  // to the phase 1 & phase 2 boundary since the shorter sequence will
  // quickly be found.

  // Establish initial cost estimate to goal state for the phase 2
  s->threshold2 = CSolver_Phase2Cost(s);

  do {
    s->newThreshold2 = 255;  // Any cost will be less than this

    // Perform the phase 2 recursive IDA* search
    result = CSolver_Search2(s);

    // Establish a new threshold for a deeper search
    s->threshold2 = s->newThreshold2;
    s->pCur = (pS);
  }
  while (result == NOT_FOUND);

  return (result != OPTIMUM_FOUND) ? result : NOT_FOUND;
}

int CSolver_Search1(CSolver* s)
{
  // Compute cost estimate to phase 1 goal state
  BYTE face, totalCost, cost = CSolver_Phase1Cost(s); // h
  int result;

  if (cost == 0) {              // Phase 1 solution found...
    CSolver_Solve2(s);
  }

  if (s->m_kAbort) {
    return USER_ABORT;
  }

  // See if node should be expanded (g + h)
  if ((totalCost = CSolver_Depth(s) + cost) <= s->threshold1) {   // Expand node

    // If this happens, we should have found the optimal solution at this
    // point, so we can exit indicating such.
    // Note: the first complete solution found in phase1 is optimal due to
    // it being an addmissible IDA* search.
    if (CSolver_Depth(s) >= minSolutionLength) {
      return OPTIMUM_FOUND;
    }

    for (face = 0; face < 6; face++) {
      if (CSolver_Disallowed(s, face)) {
        continue;
      }

      // Apply the face
      PSTATE pS = CSolver_Phase1Next(s, face);

      // Perform the phase 1 recursive IDA* search
      if (result = CSolver_Search1(s)) {
        return result;
      }

      CSolver_Phase1Power(s, pS);

      // Perform the phase 1 recursive IDA* search
      if (result = CSolver_Search1(s)) {
        return result;
      }

      CSolver_Phase1Power(s, pS);

      // Perform the phase 1 recursive IDA* search
      if (result = CSolver_Search1(s)) {
        return result;
      }

      s->pCur = (pS - 1);
    }
  }
  else if (totalCost < s->newThreshold1) {
    // Maintain minimum cost exceeding threshold
    s->newThreshold1 = totalCost;
  }

  return NOT_FOUND;
}

// Kociemba Cube Solver main program
// This program implements an algorithm devised by Herbert Kociemba to solve
// Rubik's Cube.

// This program was written for educational purposes and may be freely
// distributed provided this header remains intact.  The author assumes no
// responsibility for its use.

// When run, the program will first load the move mapping and pruning tables
// (it will regenerate them if not found) then perform the search.
// Successively shorter solutions will be printed out until an optimal
// solution is found (that may take a very long time) and the program
// terminates, or the program is aborted by the user.
// Solver
bool KSolve(const char* msg, void* pEdit)
{
  unsigned int status;
  char text[256];

  _snprintf(text, 256, "/ Kociemba Cube Solver %c-1.20", CKube::methodId);
  KPrint(0, text);

  // Parse the input and initialize a "FaceletCube". The FaceletCube
  // represents the cube by the markings of the 54 individual facelets.
  KFacelet faceCube(msg);

  // Validate the facelet representation in terms  of legal cubie markings,
  // permutation, and parity and initialize a "standard" cube. The standard
  // cube represents the cube state in terms of cubie permutation and parity
  if ((status = faceCube.Validate()) != KFacelet_VALID) {
    _snprintf(text, 256, "/ Error : %s", faceCube.ErrorText(status));
    KPrint(1, text);
    return false;
  }

  // Initialize the move mapping and pruning tables, and invoke the search
  // for a solution.  Since the cube is in a valid configuration at this
  // point, a solution should always be found.
  InitializeTables("KSolveTables.ktb");

  // Perform the two phase search
  CSolver s1;
  CSolver_Init(&s1, true);
  CSolver_Solve(&s1, faceCube, 1);

  return true;
}

// Create and initialize this faces in the KFacelet
KFacelet::KFacelet(const char* msg)
{
  static const char* faceNames = ("U:D:L:R:F:B:");
  int face, facelet, i, len = strlen(msg) - 10;

  for (i = 0; i < 54; i++) {
    faceletMarkings[i] = ' ';
  }

  for (i = 0; i < len; i++) {
    // Convert face name to enumeration offset
    if ((face = STRfind(faceNames, -1, msg + i, 2, 0)) < 0) {
      continue;
    }

    // Initialize this face in the KFacelet
    for (i++, facelet = 0; facelet < 9; facelet++) {
      faceletMarkings[(face * 9) / 2 + facelet] = msg[++i];
    }
  }
}

// Validate markings, permutation, and parity
int KFacelet::Validate()
{
  int status;

  twist = flip = 0;
  // Must validate centers first!

  if ((status = ValidateCenters())  == KFacelet_VALID
      && (status = ValidateFacelets()) == KFacelet_VALID
      && (status = ValidateCorners())  == KFacelet_VALID
      && (status = ValidateEdges())    == KFacelet_VALID
      && (status = PermutationParity()) == KFacelet_VALID) {
    // Return a properly initialized cube model
    tt.tt.twist = twist;
    tt.tt.flip = flip;
    memcpy(tt.tt.CornerPermut, CornerPermut, 8);
    memcpy(tt.tt.EdgePermut, EdgePermut, 12);
  }

  return status;
}

int KFacelet::ValidateCenters()
{
  int face, faceName;

  // Establish face markings and make sure each face marking is unique
  for (face = 0; face < 6; face++) {
    // Set another center face
    faceMarkings[face] = faceletMarkings[face * 9 + 4];

    for (faceName = 0; faceName < face; faceName++)
      if (faceMarkings[faceName] == faceMarkings[face]) {
        return KFacelet_DUPLICATE_CENTER_MARKING;  // Duplicate!
      }
  }

  return KFacelet_VALID;
}

int KFacelet::ValidateFacelets()
{
  int face, facelet, facelets[6] = {0, 0, 0, 0, 0, 0};

  // Validate all 54 facelets
  for (facelet = 0; facelet < 54; facelet++, facelets[face]++)
    if ((face = FaceletOffsetToMarking(facelet)) < 0) {
      return KFacelet_INVALID_MARKER;
    }

  // Each face must containe exactly 9 facelets
  for (face = 0; face < 6; face++)
    if (facelets[face] != 9) {
      return KFacelet_INVALID_FACELETCOUNT;
    }

  return KFacelet_VALID;
}

// Refer to the diagrams below to determine the array index for any given
// facelet.
// Direction key:
// Up
// Left  Front  Right  Back
// Down
// Facelet indices: order of faces: Up, Down, Left, Right, Front, Back
#if 0
enum Facelet {
  u0, u1, u2, u3, u4, u5, u6, u7, u8, d0, d1, d2, d3, d4, d5, d6, d7, d8, // Up / Down
  l0, l1, l2, l3, l4, l5, l6, l7, l8, r0, r1, r2, r3, r4, r5, r6, r7, r8, // Left / Right
  f0, f1, f2, f3, f4, f5, f6, f7, f8, b0, b1, b2, b3, b4, b5, b6, b7, b8
};// Front / Back
#endif

// u0 u1 u2
// u3 u4 u5
// u6 u7 u8

// l0 l1 l2   f0 f1 f2   r0 r1 r2   b0 b1 b2
// l3 l4 l5   f3 f4 f5   r3 r4 r5   b3 b4 b5
// l6 l7 l8   f6 f7 f8   r6 r7 r8   b6 b7 b8

// d0 d1 d2
// d3 d4 d5
// d6 d7 d8

int KFacelet::ValidateCorners()
{
  // The following macros are used to construct a unique number for each
  // corner cubie from its facelets.
#define FacesToCorner(face1, face2, face3) ((face1*6 + face2)*6 + face3)
#define cornerIncr(p, o) (o = (o == 2? p++, 0: o + 1))

  // Corner facelet locations
  static const int cornerFacelets[][3] = {
    {u8, r0, f2}, {u6, f0, l2}, {u0, l0, b2}, {u2, b0, r2}, // URF,UFL,ULB,UBR
    {d2, f8, r6}, {d0, l8, f6}, {d6, b8, l6}, {d8, r8, b6}
  };  // DFR,DLF,DBL,DRB
  // Table of valid facelet twist orientations
  static const int twistTable[3][3] = { {0, 1, 2}, {1, 2, 0}, {2, 0, 1} };

  int cornerParity = 0, cornerUsage = 0, cubicle, cubie, perm, or;

  // For all corner cubies...
  for (cubicle = 0; cubicle < 8; cubicle++) {

    // Compute corner cubie number directly from its facelets
    cubie = FacesToCorner(
        FaceletOffsetToMarking(cornerFacelets[cubicle][0]),
        FaceletOffsetToMarking(cornerFacelets[cubicle][1]),
        FaceletOffsetToMarking(cornerFacelets[cubicle][2])
        );

    // Search the table of valid corner facelet orientations
    for (perm = or = 0; cubie != FacesToCorner(
        cornerFacelets[perm][twistTable[ or ][0]] / 9,
        cornerFacelets[perm][twistTable[ or ][1]] / 9,
        cornerFacelets[perm][twistTable[ or ][2]] / 9
        ); cornerIncr(perm, or))
      if (perm * 3 + or == 23)
        // Facelet orientation not found in table: error
      {
        return KFacelet_INVALID_CORNER_MARKINGS;
      }

    // Set corner permutation and orientation
    CornerPermut[cubicle] = (BYTE)perm;
    twist = twist * 3 + or;
    // Accumulate corner parity sum
    cornerParity += or , cornerUsage |= 1 << perm;
  }

  twist /= 3;    // the last orientation is not in Twist

  // Checks the presence of all corner cubies
  if (cornerUsage != (1 << 8) - 1) {
    return KFacelet_INVALID_CORNER_MARKINGS;
  }

  // Total corner orientation parity (COP) must be zero
  if (cornerParity % 3 != 0) {
    return KFacelet_INVALID_CORNER_PARITY;
  }

  return KFacelet_VALID;
}

int KFacelet::ValidateEdges()
{
  // The following macros are used to construct a unique number for each
  // edge cubie from its facelets.
#define FacesToEdge(face1, face2) (face1*6 + face2)
#define edgeIncr(p, o) (o = (o == 1? p++, 0: 1))

  // Edge facelet locations
  static const int edgeFacelets[][2] = {
    {u7, f1}, {u3, l1}, {u1, b1}, {u5, r1}, {d1, f7}, {d3, l7}, // UF,UL,UB,UR,DF,DL
    {d7, b7}, {d5, r7}, {r3, f5}, {l5, f3}, {l3, b5}, {r5, b3}
  }; // DB,DR,RF,LF,LB,RB
  // Table of valid twist edge facelet orientations
  static const int flipTable[2][2] = { {0, 1}, {1, 0} };

  int edgeParity = 0, edgeUsage = 0, cubicle, cubie, perm, or;

  // For all edge cubies...
  for (cubicle = 0; cubicle < 12; cubicle++) {

    // Compute edge cubie number directly from its facelets
    cubie = FacesToEdge(
        FaceletOffsetToMarking(edgeFacelets[cubicle][0]),
        FaceletOffsetToMarking(edgeFacelets[cubicle][1])
        );

    // Search the table of valid edge facelet orientations
    for (perm = or = 0; cubie != FacesToEdge(
        edgeFacelets[perm][flipTable[ or ][0]] / 9,
        edgeFacelets[perm][flipTable[ or ][1]] / 9
        ); edgeIncr(perm, or))
      if (perm * 2 + or == 23) {
        return KFacelet_INVALID_EDGE_MARKINGS;
      }

    // Set edge permutation and orientation
    EdgePermut[cubicle] = (BYTE)perm;
    flip = flip * 2 + or;
    // Accumulate edge parity sum
    edgeParity += or , edgeUsage |= 1 << perm;
  }

  flip /= 2;     // the last orientation is not write in Flip

  // Checks the presence of all edge cubies
  if (edgeUsage != (1 << 12) - 1) {
    return KFacelet_INVALID_EDGE_MARKINGS;
  }

  // Total edge orientation parity (EOP) must be zero
  if (edgeParity % 2 != 0) {
    return KFacelet_INVALID_EDGE_PARITY;
  }

  return KFacelet_VALID;
}

int KFacelet::PermutationParity()
{
  int pp = PermutationParity(EdgePermut, 12)
      + PermutationParity(CornerPermut, 8);

  // Total corner permutation parity must equal total edge permutation
  // parity.

  if (pp & 1) {
    return KFacelet_INVALID_TOTAL_PARITY;
  }

  return KFacelet_VALID;
}

// Permutation parity can be computed by counting the number of reversals in
// the permutation sequence, - i.e., the number of pairs (p,q) such that p>q
// and p precedes q.  Then determine if the result is even or odd.  Do this
// for both edges (EPP) and corners (CPP). A configuration is reachable
// if EPP=CPP.  (August/September cube.lovers - Vanderschel/Saxe)

int KFacelet::PermutationParity(const BYTE permutations[],
    int numberOfCubies) const
{
  int permutationParity = 0, p, q;
  char t;

  for (p = 1; p < numberOfCubies; p++)
    for (q = p, t = permutations[p - 1]; q < numberOfCubies; q++)
      if (t > permutations[q]) {
        permutationParity++;
      }

  return permutationParity;
}

// Translate the contens of the KFacelet at the givend offset

int KFacelet::FaceletOffsetToMarking(int offset) const  // 0..53
{
  for (int face = 0, no = faceletMarkings[offset]; face < 6; face++)
    if (no == faceMarkings[face]) {
      return face;
    }

  return -1;
}

// Return the text associated with an error return code

char* KFacelet::ErrorText(unsigned int error)
{
  if (error >= NumberOfErrors) {
    error = 0;
  }

  return errorText[error];
}

char* KFacelet::errorText[NumberOfErrors] = {
  "",
  "Facelet marking does not match any center marking",
  "There must be 9 facelets for each marking",
  "Duplicate center marking",
  "Invalid corner markings",
  "Invalid corner orientation parity",
  "Invalid edge markings",
  "Invalid edge orientation parity",
  "Invalid total permutation parity"
};
/*
char* KFacelet::errorText[NumberOfErrors] = {
  "",
  "Facelet marking does not match any center marking",
  "There must be 9 facelets for each marking",
  "Duplicate center marking",
  "Invalid corner markings",
  "Les orientations des sommets sont incompatibles",
  "Invalid edge markings",
  "Les orientations des arêtes sont incompatibles",
  "Le nombre de permutations est incompatible"
};*/
