#include "core.hpp"
#include "game.hpp"
using namespace std;
using namespace tool;

void Game::drawWall() {
    auto offset = Offset::fromWall();
    wchar_t SWALL = L'牆';

    wstring ceiling = wstring(wid + 2, SWALL);
    offset.mvPrint(0, 0, ceiling);
    offset.mvPrint(len + 1, 0, ceiling);
    for (int i = 1; i <= len; i++) {
        offset.mvPrint(i, 0, SWALL);
        offset.mvPrint(i, wid + 1, SWALL);
    }
}

void Game::prePlay() {
    hideCur();
    clear();
    drawDashboard();
    drawWall();

    auto offset = Offset::fromMap();
    for (int i = 3; i > 0; i--) {
        wstring s = fmt::format(L"［{}〕", toSnum(i));
        offset.mvPrint(midLen(), midWid(s), s);
        refresh();
        napms(1000);
    }

    nodelay(stdscr, true);
}

void Game::drawDashboard() {
    auto offset = Offset();
    offset.delLine(0);

    // 顯示溢出 if 99
    int scoreMaxN = intLen(len * wid * 99);
    wstring scoreStr =
        wstring(scoreMaxN - intLen(score), SNUMS[0]) + toSnum(score);
    offset.mvPrint(0, 0, scoreStr);

    if (boostLeftMillis > 0) {
        wstring boostStr = L'［' + toSnum(boostLeftMillis / 100) + L'］';
        boostStr.insert(boostStr.cend() - 2, L'．');
        offset.mvPrint(0, (wid + 2 - boostStr.length()) / 2, boostStr);
    }

    wstring livesStr = toSnum(lives) + L'命';
    offset.mvPrint(0, wid + 2 - livesStr.length(), livesStr);
}

void Game::drawEle() {
    auto offset = Offset::fromMap();

    offset.mvPrint(rat.y, rat.x, L'鼠');
    if (shrink.isAvailable())
        offset.mvPrint(shrink.y, shrink.x, L'縮');
    if (boost.isAvailable())
        offset.mvPrint(boost.y, boost.x, L'速');
    if (live.isAvailable())
        offset.mvPrint(live.y, live.x, L'命');
    for (const auto &obstacle : obstacles)
        offset.mvPrint(obstacle.y, obstacle.x, L'牆');
}

void Game::drawSnake(bool isdead) {
    auto offset = Offset::fromMap();

    for (int i = snake.size() - 1; i >= 0; i--) {
        wchar_t cell = L'身';
        if (isdead)
            cell = L'屍';
        else if (i == 0)
            cell = L"右下左上"[dir];
        else if (i == snake.size() - 1)
            cell = L'尾';
        offset.mvPrint(snake[i].y, snake[i].x, cell);
    }
}

void Game::eraseSnake() {
    auto offset = Offset::fromMap();
    for (const auto &cell : snake)
        offset.delChar(cell.y, cell.x);
}
