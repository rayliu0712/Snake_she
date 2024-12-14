#include "core.hpp"
#include "game.hpp"
#include <csignal>
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace tool;

void handler(int sig) {
    endwin();

    if (sig == SIGINT)
        cout << "偵測到 Ctrl+C / Command+C，已退出\n";
    else if (sig == SIGWINCH)
        cout << "偵測到調整視窗，已退出\n";

    exit(0);
}

Game::Game(int argc, char **argv) {
    try {
        for (int i = 1; i < argc; i++) {
            string s = argv[i];

            if (s == "--fps" && i < argc - 1)
                INIT_FPS = stoi(argv[++i]);
            else if (s == "--reward-frequency" && i < argc - 1)
                REWARD_FREQUENCY = stoi(argv[++i]);
            else
                throw 0;
        }
    } catch (...) {
        cout << "參數無效\n";
        exit(1);
    }

    signal(SIGINT, handler);
    signal(SIGWINCH, handler);
    setlocale(LC_ALL, "");
    srand(time(nullptr));

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
}

Game::~Game() {
    if (!isendwin())
        endwin();
}

void Game::welcome() {
    auto offset = Offset();

    wstring s = L"［蛇］";
    int y = mid(LINES);
    int x = mid(COLUMNS, s.length());

    offset.mvPrint(y, x, s);
    blinkCur();

    int konamiCmd[] = {KEY_UP,    KEY_UP,   KEY_DOWN,  KEY_DOWN, KEY_LEFT,
                       KEY_RIGHT, KEY_LEFT, KEY_RIGHT, 'b',      'a'};

    bool isCombo = true;
    for (int i = 0; i < 10; i++) {
        int key = getch();
        int cmd = konamiCmd[i];

        if ((i < 8 && key != cmd) || (i >= 8 && tolower(key) != cmd)) {
            isCombo = false;
            break;
        }
    }

    if (isCombo) {
        lives = 30;
        s = L"＃蛇＃";

        offset.mvPrint(y, x, s);
        offset.mv(y, x + 1);
        getch();
    }

    clearNow();
    hideCur();
}

void Game::adjustMapSize() {
    MEVENT event;
    mmask_t &bstate = event.bstate;
    int &mouseY = event.y;
    int &mouseX = event.x;

    while (true) {
        clear();
        drawWall();

        auto offset = Offset::fromMap();

        wstring s1 = L"（Ｓｈｉｆｔ）＋滾輪";
        offset.mvPrint(-offset.y, midWid(s1), s1);

        wstring s2 = L"［確認］";
        offset.mvPrint(midLen(), midWid(s2), s2);
        int btnY = offset.y + midLen();
        int btnStartX = 2 * (offset.x + midWid(s2));
        int btnEndX = btnStartX + 2 * s2.length();

        int key = getch();
        Dir arrow;

        if (key == KEY_MOUSE) {
            if (getmouse(&event) != OK)
                continue;
            else if (bstate & BUTTON5_PRESSED && bstate & BUTTON_SHIFT)
                arrow = Dir::LEFT;
            else if (bstate & BUTTON5_PRESSED)
                arrow = Dir::UP;
            else if (bstate & BUTTON4_PRESSED && bstate & BUTTON_SHIFT)
                arrow = Dir::RIGHT;
            else if (bstate & BUTTON4_PRESSED)
                arrow = Dir::DOWN;
            else if (bstate & BUTTON1_CLICKED && mouseY == btnY &&
                     coInterval(btnStartX, mouseX, btnEndX))
                break;
            else
                continue;
        } else {
            if (is(key, 'w', KEY_UP))
                arrow = Dir::UP;
            else if (is(key, 'a', KEY_LEFT))
                arrow = Dir::LEFT;
            else if (is(key, 's', KEY_DOWN))
                arrow = Dir::DOWN;
            else if (is(key, 'd', KEY_RIGHT))
                arrow = Dir::RIGHT;
            else if (is(key, '\n', KEY_ENTER))
                break;
            else
                continue;
        }

        if (arrow == Dir::UP && len > 5)
            len--;
        else if (arrow == Dir::LEFT && wid > 10)
            wid--;
        else if (arrow == Dir::DOWN && len < LINES - 4)
            len++;
        else if (arrow == Dir::RIGHT && wid < COLUMNS - 2)
            wid++;
    }

    clearNow();
}

void Game::playLoop() {
    auto offset = Offset::fromMap();
    dieReset();
    prePlay();
    time_t startTime = time(nullptr);
    time_t lastTime = startTime;

    while (true) {
        Status status = play();

        if (status == Status::DEAD) {
            lives--;
            drawDashboard();

            if (lives > 0) {
                score /= 2;
                dieReset();

                wstring s =
                    fmt::format(L"存活{}秒", toSnum(time(nullptr) - lastTime));

                Offset::fromMap().mvPrint(midLen(), midWid(s), s);
                refresh();
                blinkCur();
                waitSpaceEnter();

                prePlay();
                lastTime = time(nullptr);

            } else {
                wstring s = L"遊戲結束";
                offset.mvPrint(midLen(), midWid(s), s);
                refresh();
                blinkCur();
                waitSpaceEnter();

                break;
            }

        } else if (status == Status::WIN) {
            wstring s =
                fmt::format(L"用時{}秒獲勝", toSnum(time(nullptr) - startTime));
            offset.mvPrint(midLen(), midWid(s), s);
            refresh();
            blinkCur();
            waitSpaceEnter();

            break;
        }
    }
}
