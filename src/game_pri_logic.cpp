#include "core.hpp"
#include "game.hpp"
using namespace std;
using namespace tool;

Game::Point Game::nextHead(int key)
{
    if (is(key, KEY_UP, 'w') && dir != Dir::DOWN)
        dir = Dir::UP;
    else if (is(key, KEY_LEFT, 'a') && dir != Dir::RIGHT)
        dir = Dir::LEFT;
    else if (is(key, KEY_DOWN, 's') && dir != Dir::UP)
        dir = Dir::DOWN;
    else if (is(key, KEY_RIGHT, 'd') && dir != Dir::LEFT)
        dir = Dir::RIGHT;

    Point head = snake.front();
    if (dir == Dir::UP)
        head.y--;
    else if (dir == Dir::LEFT)
        head.x--;
    else if (dir == Dir::DOWN)
        head.y++;
    else if (dir == Dir::RIGHT)
        head.x++;
    return head;
}

bool Game::isDead()
{
    auto head = snake.front();
    return is(head.y, -1, len) || is(head.x, -1, wid) || in(obstacles, head) || in(snake | views::drop(1), head);
}

void Game::gen(Point& ele)
{
    vector<Point> eles = { rat, shrink, boost, live };

    auto it = ranges::find(eles, ele);
    if (it != eles.end())
        eles.erase(it);

    Point point;
    while (true) {
        point = { rand() % len, rand() % wid };
        if (!in(eles, point) && !in(snake, point) && !in(obstacles, point))
            break;
    }
    ele = point;
}

void Game::genObstcales()
{
    Point point;
    while (true) {
        gen(point);
        for (auto& obstacle : obstacles)
            if (obstacle == point)
                continue;
        break;
    }
    obstacles.push_back(point);
}

// hardcoded value !
Game::Status Game::play()
{
    bool isdead = isDead();
    drawDashboard();
    drawEle();
    drawSnake(isdead);
    refresh();
    napms(1000 / fps);

    if (isdead)
        return Status::DEAD;
    eraseSnake();

    if (boostLeftMillis > 0) {
        boostLeftMillis -= 1000 / fps;
        if (boostLeftMillis <= 0) {
            boostLeftMillis = 0;
            fps = INIT_FPS;
        }
    }

    auto head = nextHead(getch());
    snake.push_front(head);

    bool eatRat;
    if (head == rat) {
        eatRat = true;
        score++;

        if (snake.size() == (len * wid - obstacles.size()) / 2)
            return Status::WIN;

        gen(rat);
    } else {
        eatRat = false;

        if (head == boost) {
            boost.destroy();
            fps *= 1.5;
            if (boostLeftMillis == 0)
                boostLeftMillis = 10'000;
            else
                boostLeftMillis *= 1.5;

        } else if (head == shrink && snake.size() > 4) {
            shrink.destroy();
            for (int i = 1; i <= snake.size() / 2; i++)
                snake.pop_back();

        } else if (head == live) {
            live.destroy();
            lives++;
        }
        snake.pop_back();
    }

    if (eatRat && score % F_REWARD == 0) {
        int p = rand() % 100;
        if (p >= 100 - 40)
            genObstcales();
        else if (p >= 100 - 40 - 30)
            gen(boost);
        else if (p >= 100 - 40 - 30 - 20)
            gen(shrink);
        else
            gen(live);
    }

    return Status::RUNNING;
}