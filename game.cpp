#include "Lib\\TXLib.h"
#include "Lib\\directionFrameFrameTimer.cpp"
#include "Lib\\Bomzh.cpp"
#include "Lib\\consmenu.cpp"
#include "Lib\\Director.cpp"
#include "Lib\\Pitek.cpp"
#include "Lib\\Robot.cpp"
#include "Lib\\Stena.cpp"
#include "Lib\\Exit.cpp"
#include <windows.h>
#include <stdio.h>

using namespace std;





Bomzh b;

void catchCheckR(Bomzh* b, Robot r)
{
    if(abs(r.x - b->x) <= 30 && abs(r.y - b->y) <= 30)
    {
        b->life -= 1;//(b.life -= 10)
    }
}

void ConditionOfVictory(Bomzh* b)
{
    int xVictory = 666, yVictory = 555;

    if((b->x - xVictory) * (b->x - xVictory) +
       (b->y - yVictory) * (b->y - yVictory) <= 50 * 50)
    {
        txTextOut(0, 0, "You won");
        exit(1);
        txDestroyWindow();
    }

    if(Time - TimeBeg >= TimeVictory)
    {
        txTextOut(0, 0, "You won");
        exit(1);
        txDestroyWindow();
    }

    if(GameMode == 1)
    {
        txCircle(xVictory, yVictory, 50);
        char str[100];
        sprintf(str, "%d", b->x);
        txTextOut(b->x, 100, str);
        sprintf(str, "%d", b->y);
        txTextOut(b->x + 100, 100, str);
    }
}

void drawLevel(Stena* stena, Pitek* pi, Robot* r, Director* d, Bomzh b, int nomer_sten, int nomer_piteka, int nomer_robotov, int nomer_directorov)
{
     for (int nomer = 0; nomer < nomer_sten; nomer++)
        {
            drawStena(stena[nomer]);
        }
        for (int nomer = 0; nomer < nomer_piteka; nomer++)
        {
            drawPitek(pi[nomer]);
        }

        for (int nomer = 0; nomer < nomer_robotov; nomer++)
        {
            drawRobot(r[nomer], nomer_robotov);
        }

        drawBomzh(b);

        for (int nomer = 0; nomer < nomer_directorov; nomer++)
        {
            drawDirector(d[nomer]);
        }
}



void scene1(Bomzh b, Robot* r, Director* d, Point* p, Pitek* pi, Stena* stena, Exit e, int nomer_robotov, int nomer_directorov, int nomer_piteka, int nomer_sten)
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    TimeBeg = 60 * st.wMinute + st.wSecond;
    Time = 60 * st.wMinute + st.wSecond;

    for (int nomer = 0; nomer < nomer_directorov; nomer++)
    {
        p[nomer].y = 100;
        p[nomer].x1 = 600;
        p[nomer].y1 = 100;
        p[nomer].x2 = b.x;
        p[nomer].y2 = b.y;
        p[nomer].nomerPoint = 1;
        d[nomer].Stolknuls = 0;
    }

    int EscapeTimer = 0;

    HDC fon = txLoadImage("IMG\\Maps\\Level1\\canteen.bmp");
    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        txBitBlt(txDC(), 0, 0, 1200, 900, fon, 0, 0);
        txBegin();

        //Move & intersect

                if(GetAsyncKeyState('Q'))
        {

            drawExit(e);
            EscapeTimer++;
            if(EscapeTimer >= 30)
                break;
        }
        else
            EscapeTimer = 0;

        for (int nomer = 0; nomer < nomer_robotov; nomer++)
        {
            moveRobot(&r[nomer]);
            catchCheckR(&b, r[nomer]);
        }

        b.predX = b.x;
        b.predY = b.y;
        moveBomzh(&b);

        fillCrashZone(&b);

        for (int nomer = 0; nomer < nomer_directorov; nomer++)
        {
            d[nomer].predX = d[nomer].x;
            d[nomer].predY = d[nomer].y;
            moveDirector(&d[nomer], &p[nomer]);
            fillCrashZone(&d[nomer]);
        }

        for (int nomer = 0; nomer < nomer_piteka; nomer++)
        {
            if (intersect(b.crash, pi[nomer].crash))
            {
                b.x = b.predX;
                b.y = b.predY;
            }
        }

        for (int nomer = 0; nomer < nomer_sten; nomer++)
        {
            if (intersect(b.crash, stena[nomer].crash))
            {
                b.x = b.predX;
                b.y = b.predY;
            }
        }

        //Directors collision
        for (int nomer = 0; nomer < nomer_directorov - 1; nomer++)
        {
            for (int nomer1 = nomer + 1; nomer1 < nomer_directorov; nomer1++)
            {
                while ( abs (d[nomer].x - d[nomer1].x) < 15 && abs (d[nomer].y - d[nomer1].y) < 15)
                {
                    d[nomer1].x = d[nomer1].x + random(-5, 5);
                    d[nomer1].y = d[nomer1].y + random(-5, 5);
                }
            }
        }

        for (int nomer = 0; nomer < nomer_directorov; nomer++)
        {
            p[nomer].x2 = b.x;
            p[nomer].y2 = b.y;

            moveDirector(&d[nomer], &p[nomer]);
            fillCrashZone(&d[nomer]);

            catchCheck(b, d[nomer]);


            bool stolkn = false;
            for (int nomer1 = 0; nomer1 < nomer_piteka; nomer1++)
            {
                if (intersect(d[nomer].crash, pi[nomer1].crash))
                {
                    d[nomer].x = d[nomer].predX;
                    d[nomer].y = d[nomer].predY;
                    stolkn = true;
                }
            }

            for (int nomer1 = 0; nomer1 < nomer_sten; nomer1++)
            {
                if (intersect(d[nomer].crash, stena[nomer1].crash))
                {
                    d[nomer].x = d[nomer].predX;
                    d[nomer].y = d[nomer].predY;
                    stolkn = true;
                }
            }

            if (stolkn)
            {
                d[nomer].Stolknuls = d[nomer].Stolknuls + 1;
                if (d[nomer].Stolknuls > 2)
                {
                    d[nomer].Stolknuls = 0;
                }
            }
            else
            {
                d[nomer].Stolknuls = 0;
            }

        }

        drawLevel(stena, pi, r, d, b, nomerStena, nomerPiteka, nomerRobota, nomerDirector);


        //Time += 10;
        GetLocalTime(&st);



        Time = 60 * st.wMinute + st.wSecond;
        char stm[100];
        sprintf(stm, "%d", TimeVictory - (Time - TimeBeg));
        txTextOut(100, 50, stm);
        ConditionOfVictory(&b);

        char stt[100];
        sprintf(stt, "%d", b.life);
        txTextOut(50, 50, stt);

        txEnd();
        txSleep(10);

    }
    txDeleteDC(fon);
 }

/*void directionFrameFrameTimer(int *direction, int *frame, int *frameTimer)
{
    *direction = 0;
    *frame = 0;
    *frameTimer = 0;
}*/

void DeletePics(HDC* picDown, HDC* picUp, HDC* picLeft, HDC* picRight)
{
    txDeleteDC(*picDown);
    txDeleteDC(*picUp);
    txDeleteDC(*picLeft);
    txDeleteDC(*picRight);
 }

void MapSchitivanie()
{
    ifstream Map;
    string stroka_Personage;
    string stroka_X;
    string stroka_Y;
    Map.open("Lib\\Map.txt");

    while (Map.good()) {

        getline (Map, stroka_Personage);

        readRobot(&Map, stroka_Personage, robots, &nomerRobota);

        readStena(&Map, stroka_Personage, stena, &nomerStena);

        readDirector(&Map, stroka_Personage, director, &nomerDirector, p);

        readPitek(&Map, stroka_Personage, Piteks, &nomerPiteka);

        readBomzh(&Map, stroka_Personage, b);
        /*if (strcmp(stroka_Personage.c_str(), "bomzh") == 0)
        {
            getline (Map, stroka_X);
            b.x = atoi(stroka_X.c_str());
            getline (Map, stroka_Y);
            b.y = atoi(stroka_Y.c_str());
        }*/

        /*if (strcmp(stroka_Personage.c_str(), "pitek") == 0)
        {
            getline (Map, stroka_X);
            Piteks[nomerPiteka].x = atoi(stroka_X.c_str());
            getline (Map, stroka_Y);
            Piteks[nomerPiteka].y = atoi(stroka_Y.c_str());
            fillCrashZone(&Piteks[nomerPiteka]);
            nomerPiteka++;
        }*/
    }

    Map.close();
}

int main(int argc, char *argv[])
{
    const char* nomerPerson;
    if (argc > 1)
    {
        nomerPerson = argv[1];
    }
    else
    {
        nomerPerson = "2";
    }

    txCreateWindow (1090, 654);
    MapSchitivanie();
    txBegin();

    Exit e;
    e.x = 60;
    e.y = 10;
    e.Exit = txLoadImage("IMG\\ImgMainMenu\\Exit0.bmp");

    b.x = 300;
    b.y = 200;
    b.width = 62;
    b.height = 84;
    b.speed = 6;
    b.PointStartX1 = 27;
    b.PointStartX2 = 24;
    b.PointStartY = 74;
    b.manyframeRight = 4;
    b.manyframeLeft = 4;
    b.manyframeUp = 4;
    b.manyframeDown = 4;
    b.life = 100;
    directionFrameFrameTimer(&b.direction, &b.frame, &b.frameTimer);
    b.frameTimer = 0;
    if(strcmp(nomerPerson, "3") == 0)
    {
        b.picDown = txLoadImage ("IMG\\Men\\Robot\\RobotDown.bmp");
        b.picUp = txLoadImage ("IMG\\Men\\Robot\\RobotUp.bmp");
        b.picLeft = txLoadImage ("IMG\\Men\\Robot\\RobotLeft.bmp");
        b.picRight = txLoadImage ("IMG\\Men\\Robot\\RobotRight.bmp");
    }
    else if(strcmp(nomerPerson, "2") == 0)
    {
        b.picDown = txLoadImage ("IMG\\Men\\Homeless\\HomelessDown.bmp");
        b.picUp = txLoadImage ("IMG\\Men\\Homeless\\HomelessUp.bmp");
        b.picLeft = txLoadImage ("IMG\\Men\\Homeless\\HomelessLeft.bmp");
        b.picRight = txLoadImage ("IMG\\Men\\Homeless\\HomelessRight.bmp");
    }
    else if(strcmp(nomerPerson, "1") == 0)
    {
        b.picDown = txLoadImage ("IMG\\Men\\Girl\\GirlDown.bmp");
        b.picUp = txLoadImage ("IMG\\Men\\Girl\\GirlUp.bmp");
        b.picLeft = txLoadImage ("IMG\\Men\\Girl\\GirlLeft.bmp");
        b.picRight = txLoadImage ("IMG\\Men\\Girl\\GirlRight.bmp");
    }

    for (int nomer = 0; nomer < nomerDirector; nomer++)
    {
        director[nomer].speed = 1;
        director[nomer].width = 61;
        director[nomer].height = 96;
        director[nomer].PointStartX1 = 27;
        director[nomer].PointStartX2 = 24;
        director[nomer].PointStartY = 74;
        director[nomer].manyframeRight = 4;
        director[nomer].manyframeLeft = 4;
        director[nomer].manyframeUp = 4;
        director[nomer].manyframeDown = 4;
        directionFrameFrameTimer(&director[nomer].direction, &director[nomer].frame, &director[nomer].frameTimer);
        director[nomer].radius = 170;
        if (nomer > 0)
        {
            director[nomer].picDown = director[0].picDown;
            director[nomer].picUp = director[0].picUp;
            director[nomer].picLeft = director[0].picLeft;
            director[nomer].picRight = director[0].picRight;
        }
        else
        {
            director[nomer].picDown = txLoadImage ("IMG\\Men\\Girl\\GirlDown.bmp");
            director[nomer].picUp = txLoadImage ("IMG\\Men\\Girl\\GirlUp.bmp");
            director[nomer].picLeft = txLoadImage ("IMG\\Men\\Girl\\GirlLeft.bmp");
            director[nomer].picRight = txLoadImage ("IMG\\Men\\Girl\\GirlRight.bmp");
        }
    }

    for ( int nomer = 0; nomer < nomerPiteka; nomer++)
    {
        Piteks[nomer].height = 78;
        Piteks[nomer].width = 226;
        if (nomer > 0)
        {
            Piteks[nomer].picDown = Piteks[0].picDown;
        }
        else
        {
            Piteks[nomer].picDown = txLoadImage("IMG\\Items\\Pitek.bmp");
        }
    }

    scene1(b, robots, director, p, Piteks, stena, e, nomerRobota, nomerDirector, nomerPiteka, nomerStena);

    DeletePics(&b.picDown, &b.picUp, &b.picLeft, &b.picRight);
    for (int nomer = 0; nomer < nomerRobota; nomer++)
    {
        DeletePics(&robots[nomer].picDown, &robots[nomer].picUp, &robots[nomer].picLeft, &robots[nomer].picRight);
    }

    for (int nomer = 0; nomer < nomerPiteka; nomer++)
    {
        txDeleteDC(Piteks[nomer].picDown);
    }

    for (int nomer = 0; nomer < nomerDirector; nomer++)
    {
        DeletePics(&director[nomer].picDown, &director[nomer].picUp, &director[nomer].picLeft, &director[nomer].picRight);
    }
}
