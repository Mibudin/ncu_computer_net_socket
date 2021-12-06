// Connect in `preinit`

// Check mode SET in `setMap`

// Check mode RUN in `loop`

// Check mode CLOSE in 'deinit'


/// Check mode:
/// - Server
///   1. Into mode
///   2. Send mode
/// - Client
///   1. Wait mode
///   2. Receive mode
///   3. Into mode

/// Key handling:
/// - Active
///   1. Press key
///   2. Operations
///   3. Send Key
/// - Passive
///   1. Receive Key
///   2. Operations

/// Close:
/// - Active
///   1. Send close
///   2. Close
/// - Passive
///   1. Receive close
///   2. Close

#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<thread>
#include"config.hpp"
#include"cell.hpp"
#include"world.hpp"
#include"textarea.hpp"
#include"screenio.hpp"
#include"keyio.hpp"
#include"renderer.hpp"
#include"renderee.hpp"
#include"pane.hpp"
#include"client.hpp"
#include"msgpkt.hpp"


void preinit();
void init();
void setMap(const int turn);
void loop();
void deinit();


gol::World*    wld;
gol::Screenio* sio;
gol::Keyio*    kio;
gol::Renderer* rer;

gol::WorldPane*   wldp;
gol::TitlePane*   ttlp;
gol::InfoPane*    infp;
gol::NetworkPane* netp;

gol::Thread* thW;
gol::Thread* thR;

std::chrono::steady_clock::duration dur;
std::chrono::steady_clock::time_point tp;

// gol::GolServer* gols;
gol::GolClient* golc;


int main()
{
    preinit();

    init();

    setMap(0);

    loop();

    deinit();

    return 0;
}

void preinit()
{
    signal(SIGINT,  [](int sig){deinit(); exit(1);});
    signal(SIGTERM, [](int sig){deinit(); exit(1);});

    printf("[Client]: Loading configurations...\n");
    gol::loadConfig(DEFAULT_CONFIG_FILE);

    golc = new gol::GolClient();
    golc->createSocket();
    printf("[Client]: Waiting for the server...\n");
    if(!golc->connectServer())
    {
        printf("[Client]: The server does not run.\n");
        golc->closeSocket();
        deinit();
        exit(1);
    }

    return;
}

void init()
{
    gol::cfg->mode = gol::ModeType::INIT;

    wld = new gol::World(gol::cfg->worldWidth, gol::cfg->worldHeight);
    sio = new gol::Screenio();
    kio = new gol::Keyio();
    rer = new gol::Renderer();

    wldp = new gol::WorldPane(wld, 2, 1);
    ttlp = new gol::TitlePane(    16,  3, (gol::cfg->worldWidth << 1) + 6, 1);
    infp = new gol::InfoPane(wld, 16, 21, (gol::cfg->worldWidth << 1) + 6, 4);
    netp = new gol::NetworkPane((gol::cfg->worldWidth << 1) + 3, 3, 2, gol::cfg->worldHeight + 3);
    netp->setConnect(golc->getServerIPAddr(), golc->getServerPort());

    dur = std::chrono::milliseconds(gol::cfg->turnPeriod);

    wld->setSampleMap();

    sio->initTty();

    rer->addRenderee(wldp);
    rer->addRenderee(ttlp);
    rer->addRenderee(infp);
    rer->addRenderee(netp);

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

    // infp->setMode(gol::ModeType::SET);  // TODO: Mode in main logic
    gol::cfg->mode = gol::ModeType::SET;
    netp->setNetworkState(gol::NetworkState::RECV_SET);
    netp->emergRender();
    golc->checkMode(gol::ModeType::SET);
    netp->setNetworkState(gol::NetworkState::SET);

    rer->renderAll();

    ANSIES(CUP(2, 4) CUS);

    while(nextWait && kio->blockWaitKey())  // TODO: thread race condition
    {
        switch(c = kio->getLastKey())
        {
            case 'W': case 'w':
                if(y > 0){ANSIES(CUU(1)); y--;}
                break;

            case 'S': case 's':
                if(y < gol::cfg->worldHeight - 1){ANSIES(CUD(1)); y++;}
                break;

            case 'A': case 'a':
                if(x > 0){ANSIES(CUB(2)); x--;}
                break;

            case 'D': case 'd':
                if(x < gol::cfg->worldWidth - 1){ANSIES(CUF(2)); x++;}
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
                if(gol::cfg->turnPeriod > 50) gol::cfg->turnPeriod -= 50;
                dur = std::chrono::milliseconds(gol::cfg->turnPeriod);
                reRender = true;
                break;

            case '\'':
                if(gol::cfg->turnPeriod < 1000) gol::cfg->turnPeriod += 50;
                dur = std::chrono::milliseconds(gol::cfg->turnPeriod);
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

    // infp->setMode(gol::ModeType::RUN);  // TODO: Mode in main logic
    gol::cfg->mode = gol::ModeType::RUN;
    netp->setNetworkState(gol::NetworkState::RECV_RUN);
    netp->emergRender();
    golc->checkMode(gol::ModeType::RUN);
    netp->setNetworkState(gol::NetworkState::RUN);

    kio->startWait();

    thW = new gol::Thread({
        [](void* ths){return ((gol::World*)ths)->goTurn();}, wld});
    thR = new gol::Thread({
        [](void* ths){((gol::Renderer*)ths)->renderAll(); return 0;}, rer});

    for(int i = 0; contLoop && i < gol::cfg->turnMax; i++)
    {
        tp = std::chrono::steady_clock::now() + dur;

        thW->nextPromise(); thW->storrs();
        thR->nextPromise(); thR->storrs();

        while(kio->waitKeyAsync(tp))
        {
            bool nextWait = true;
            switch(kio->getLastKey())
            {
                case ';':
                    if(gol::cfg->turnPeriod > 50) gol::cfg->turnPeriod -= 50;
                    dur = std::chrono::milliseconds(gol::cfg->turnPeriod);
                    break;

                case '\'':
                    if(gol::cfg->turnPeriod < 1000) gol::cfg->turnPeriod += 50;
                    dur = std::chrono::milliseconds(gol::cfg->turnPeriod);
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
            else         break;
        }

        thW->currentPromise()->wait();  // Process turn i + 1 complete
        thR->currentPromise()->wait();  // Render turn i complete

        std::this_thread::sleep_until(tp);

        // Enter other mode?
        if(backSet)
        {
            backSet = false;
            setMap(wld->getTurn());

            // infp->setMode(gol::ModeType::RUN);  // TODO: Mode in main logic
            gol::cfg->mode = gol::ModeType::RUN;
            netp->setNetworkState(gol::NetworkState::SEND_RUN);
            netp->emergRender();
            golc->checkMode(gol::ModeType::RUN);
            netp->setNetworkState(gol::NetworkState::RUN);

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
    gol::cfg->mode = gol::ModeType::CLOSE;
    if(netp) netp->setNetworkState(gol::NetworkState::SEND_CLOSE);
    if(netp) netp->emergRender();
    if(golc) golc->checkMode(gol::ModeType::CLOSE);
    if(netp) netp->setNetworkState(gol::NetworkState::CLOSE);
    if(netp) netp->emergRender();

    if(golc) golc->closeSocket();
    if(golc) delete golc;

    if(wld) wld->deinit();
    if(sio) sio->deinitTty();

    if(wld) delete wld;
    if(sio) delete sio;
    if(kio) delete kio;
    if(rer) delete rer;

    if(wldp) delete wldp;
    if(ttlp) delete ttlp;
    if(infp) delete infp;
    if(netp) delete netp;

    if(thW) delete thW;
    if(thR) delete thR;

    return;
}

