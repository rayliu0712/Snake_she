#ifndef GAME_HPP
#define GAME_HPP

#include "tool.hpp"
#include <deque>
#include <vector>

class Game {
  public:
    Game(int, char **);

    ~Game();

    void welcome();

    void adjustMapSize();

    void playLoop();

  private:
    struct Point {
        int y = -2, x;

        inline void destroy() { y = -2; }

        inline bool isAvailable() const { return y != -2; }

        bool operator==(const Point &other) const {
            return y == other.y && x == other.x;
        }
    };

    enum Dir { UP, LEFT, DOWN, RIGHT };

    enum Status { RUNNING, DEAD, WIN };

    // can set by flag
    int INIT_FPS = 10;
    int REWARD_FREQUENCY = 5;

    // map
    int len = 5;
    int wid = 10;

    // snake
    int score = 0;
    int lives = 1;
    int boostLeftMillis;
    int fps;
    int dir;

    Point rat;
    Point boost;
    Point shrink;
    Point live;
    std::deque<Point> snake;
    std::vector<Point> obstacles;

    inline int midLen(int n = 1) { return tool::mid(len, n); }

    inline int midWid(const std::wstring &s) {
        return tool::mid(wid, s.length());
    }

    void dieReset() {
        boostLeftMillis = 0;
        fps = INIT_FPS;
        dir = Dir::RIGHT;

        gen(rat);
        boost.destroy();
        shrink.destroy();
        live.destroy();

        snake.clear();
        snake.push_back({midLen(), wid / 3 + 1}); // head
        snake.push_back({midLen(), wid / 3});     // body
        snake.push_back({midLen(), wid / 3 - 1}); // tail

        obstacles.clear();
    }

    // draw

    void drawWall();

    void prePlay();

    void drawDashboard();

    void drawEle();

    void drawSnake(bool);

    void eraseSnake();

    // logic

    Point nextHead(int); // return head, accept key

    bool isDead();

    void gen(Point &);

    void genObstcales();

    Status play();
};

#endif