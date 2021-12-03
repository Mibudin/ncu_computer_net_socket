#include<stdio.h>
#include<unistd.h>
#include<thread>
#include"config.hpp"
#include"cell.hpp"
#include"world.hpp"
#include"textarea.hpp"
#include"screenio.hpp"
#include"keyio.hpp"
#include"renderer.hpp"
#include"renderee.hpp"


void init();
void setMap(const int turn);
void loop();
void deinit();


gol::World*    wld;
gol::Textarea* tta;
gol::Screenio* sio;
gol::Keyio*    kio;
gol::Renderer* rer;

std::thread* thW;
std::thread* thR;
std::thread* thK;

std::chrono::steady_clock::duration dur;
std::chrono::steady_clock::time_point tp;


int main()
{
    init();

    setMap(0);

    loop();

    deinit();

    return 0;
}

void init()
{
    wld = new gol::World(WORLD_WIDTH, WORLD_HEIGHT);
    tta = new gol::Textarea(wld);
    sio = new gol::Screenio();
    kio = new gol::Keyio();
    rer = new gol::Renderer();

    dur = std::chrono::milliseconds(gol::turnPeriod);

    wld->setSampleMap();

    sio->initTty();

    rer->addRenderee(wld);
    rer->addRenderee(tta);

    rer->renderInit();

    return;
}

void setMap(const int turn)
{
    bool nextWait = true;
    bool reCount  = false;
    bool reRender = false;
    bool goDeinit = false;
    char c;
    int x = 0, y = 0;
    int t = turn & 1;

    tta->setMode(gol::ModeType::SET);

    rer->renderAll();

    ANSIES(CUP(3, 6) CUS);

    while(nextWait && kio->blockWaitKey())
    {
        switch(c = kio->getLastKey())
        {
            case 'W': case 'w':
                if(y > 0){ANSIES(CUU(1)); y--;}
                break;

            case 'S': case 's':
                if(y < WORLD_HEIGHT - 1){ANSIES(CUD(1)); y++;}
                break;

            case 'A': case 'a':
                if(x > 0){ANSIES(CUB(2)); x--;}
                break;

            case 'D': case 'd':
                if(x < WORLD_WIDTH - 1){ANSIES(CUF(2)); x++;}
                break;

            case '[':
                wld->getCell(x, y)->setStatus(gol::CellStatus::DEAD, t);
                reCount = true;
                reRender = true;
                break;

            case ']':
                wld->getCell(x, y)->setStatus(gol::CellStatus::LIVE, t);
                reCount = true;
                reRender = true;
                break;
            
            case ';':
                if(gol::turnPeriod > 50) gol::turnPeriod -= 50;
                dur = std::chrono::milliseconds(gol::turnPeriod);
                reRender = true;
                break;

            case '\'':
                if(gol::turnPeriod < 1000) gol::turnPeriod += 50;
                dur = std::chrono::milliseconds(gol::turnPeriod);
                reRender = true;
                break;

            case '\\':
                nextWait = false;
                break;
            
            case 'P': case 'p':
                nextWait = false;
                goDeinit = true;
                break;

            case -1: default:
                // printf("> %d\n", c);
                break;
        }

        if(reCount)
        {
            wld->countAllCells(t);
            reCount = false;
        }

        if(reRender)
        {
            ANSIES(CUH SCP);
            rer->renderAll();
            ANSIES(RCP CUS);
            reRender = false;
        }
    }

    ANSIES(CUH);

    if(goDeinit) deinit();

    return;
}

void loop()
{
    bool contLoop = true;
    bool backSet = false;
    bool goDeinit = false;

    tta->setMode(gol::ModeType::RUN);

    kio->startWait();

    for(int i = 0; contLoop && i < TURN_MAX; i++)
    {
        tp = std::chrono::steady_clock::now() + dur;

        thW = new std::thread(&gol::World::goTurn, wld);
        thR = new std::thread(&gol::Renderer::renderAll, rer);

        if(kio->waitKeyAsync(tp))
        {
            bool nextWait = true;
            switch(kio->getLastKey())
            {
                case ';':
                    if(gol::turnPeriod > 50) gol::turnPeriod -= 50;
                    dur = std::chrono::milliseconds(gol::turnPeriod);
                    break;

                case '\'':
                    if(gol::turnPeriod < 1000) gol::turnPeriod += 50;
                    dur = std::chrono::milliseconds(gol::turnPeriod);
                    break;

                case '\\':
                    backSet = true;
                    nextWait = false;
                    break;

                case 'P': case 'p':
                    goDeinit = true;
                    nextWait = false;
                    contLoop = false;
                    break;

                case -1: default:
                    break;
            }
            if(nextWait) kio->startWait();
        }

        thW->join();  // Process turn i + 1 complete
        thR->join();  // Render turn i complete

        free(thW); thW = nullptr;
        free(thR); thR = nullptr;

        std::this_thread::sleep_until(tp);

        // Enter other mode?
        if(backSet)
        {
            backSet = false;
            setMap(wld->getTurn());
            tta->setMode(gol::ModeType::RUN);
            wld->backTurn();

            kio->startWait();
        }

        wld->nextTurn();
    }

    if(goDeinit) deinit();

    return;
}

void deinit()
{
    wld->deinit();
    sio->deinitTty();

    free(wld);
    free(sio);
    free(kio);
    free(rer);

    if(thW != nullptr) free(thW);
    if(thR != nullptr) free(thR);

    exit(0);
    return;
}
