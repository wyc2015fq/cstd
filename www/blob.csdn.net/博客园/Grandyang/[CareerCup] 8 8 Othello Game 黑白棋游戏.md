# [CareerCup] 8.8 Othello Game 黑白棋游戏 - Grandyang - 博客园







# [[CareerCup] 8.8 Othello Game 黑白棋游戏](https://www.cnblogs.com/grandyang/p/4804629.html)







8.8 Othello is played as follows: Each Othello piece is white on one side and black on the other. When a piece is surrounded by its opponents on both the left and right sides, or both the top and bottom, it is said to be captured and its color is flipped. On your turn, you must capture at least one of your opponent's pieces. The game ends when either user has no more valid moves. The win is assigned to the person with the most pieces. Implement the object-oriented design for Othello.



这道题是经典的黑白棋游戏，我最早接触到这个游戏是在文曲星上，当年文曲星很火的时候，上面的各种游戏我都爱不释手啊，什么英雄坛说，华容道，汉诺塔啊，黑白棋啊都是我常玩的游戏，尤其这道黑白棋，总是玩不过困难模式的电脑，后来想想玩不过也应该，电脑应该可以把所有可以走的步骤都计算一遍，每次都选择最优的解，下不赢电脑也很正常嘛。今天才算深入了解这个游戏的设计原理啊，可参见下面代码：



```
enum Direction { Left, Right, Up, Down };
enum Color { White, Black };

class Piece {
public:
    Piece(Color c): _color(c) {}
    void flip() {
        if (_color == Color::Black) _color = Color::White;
        else _color = Color::Black;
    }
    Color getColor() { return _color; }

private:
    Color _color;
};

class Location {
public:
    Location(int r, int c): _row(r), _col(c) {}
    bool isSameAs(int r, int c) {
        return _row == r && _col == c;
    }
    int getRow() { return _row; }
    int getCol() { return _col; }
    
private:
    int _row;
    int _col;
};

class Board {
public:
    Board(int rows, int cols) {
        _board.resize(rows, vector<Piece*>(cols));
    }
    void initialize() {
        int midRow = _board.size() / 2;
        int midCol = _board[midRow].size() / 2;
        _board[midRow][midCol] = new Piece(Color::White);
        _board[midRow + 1][midCol] = new Piece(Color::Black);
        _board[midRow + 1][midCol + 1] = new Piece(Color::White);
        _board[midRow][midCol + 1] = new Piece(Color::Black);
        _blackCnt = 2;
        _whiteCnt = 2;
    } 
    bool placeColor(int row, int col, Color color) {
        if (_board[row][col] != nullptr) {
            return false;
        }
        vector<int> res(4, 0);
        res[0] = flipSection(row - 1, col, color, Direction::Up);
        res[1] = flipSection(row + 1, col, color, Direction::Down);
        res[2] = flipSection(row, col + 1, color, Direction::Right);
        res[3] = flipSection(row, col - 1, color, Direction::Left);
        int flipped = 0;
        for (auto a : res) {
            if (a > 0) flipped += a;
        }
        if (flipped < 0) return false;
        _board[row][col] = new Piece(color);
        updateScore(color, flipped + 1);
        return true;
    } 
    int getScoreForColor(Color c) {
        if (c == Color::Black) return _blackCnt;
        else return _whiteCnt;
    }
    void updateScore(Color newColor, int newPieces) {
        if (newColor == Color::Black) {
            _whiteCnt -= newPieces - 1;
            _blackCnt += newPieces;
        } else {
            _blackCnt -= newPieces - 1;
            _whiteCnt += newPieces;
        }
    } 
    void printBoard() {
        for (int r = 0; r < _board.size(); ++r) {
            for (int c = 0; c < _board[r].size(); ++c) {
                if (_board[r][c] == nullptr) {
                    cout << "_";
                } else if (_board[r][c]->getColor() == Color::White) {
                    cout << "W";
                } else {
                    cout << "B";
                }
            }
            cout << endl;
        }
    }

private:
    int _blackCnt = 0;
    int _whiteCnt = 0;
    vector<vector<Piece*> > _board;
    int flipSection(int row, int col, Color color, Direction d) {
        int r = 0, c = 0;
        switch (d) {
            case Direction::Up: r = -1; break;
            case Direction::Down: r = 1; break;
            case Direction::Left: c = -1; break;
            case Direction::Right: c = 1; break;
        }
        if (row < 0 || row >= _board.size() || col < 0 || col >= _board[row].size() || _board[row][col] == nullptr) {
            return -1;
        }
        if (_board[row][col]->getColor() == color) {
            return 0;
        }
        int flipped = flipSection(row + r, col + c, color, d);
        if (flipped < 0) return -1;
        _board[row][col]->flip();
        return flipped + 1;
    } 
};

class Player {
public:
    Player(Color c): _color(c) {}
    int getScore() {
    
    } // ...
    bool playPiece(int r, int c) {
        return Game::getInstance()->getBoard()->placeColor(r, c, _color);
    }

private:
    Color _color;
};

class Game {
public:
    static Game* getInstance() {
        if (_instance == nullptr) {
            _instance = new Game();
        }
        return _instance;
    }
    Board* getBoard() { return _board; }

private:
    vector<Player*> _players;
    static Game *_instance;
    Board *_board;
    const int _ROWS = 10;
    const int _COLUMNS = 10;
    Game() {
        _board = new Board(_ROWS, _COLUMNS);
        _players.resize(2, nullptr);
        _players[0] = new Player(Color::Black);
        _players[1] = new Player(Color::White);
    }
};
```














