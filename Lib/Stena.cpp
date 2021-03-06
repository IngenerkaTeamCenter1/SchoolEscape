struct Stena
{
    int x1, x2;
    int y1, y2;
    CrashZone crash;
};

void drawStena(Stena s)
{
    txSetFillColor(TX_RED);
    txSetColour(TX_RED);
    txRectangle(s.x1 - absolutX, s.y1 - absolutY, s.x2 - absolutX, s.y2 - absolutY);
}

void fillCrashZone(Stena* stena)
{
    stena->crash.x1 = stena->x1;
    stena->crash.y1 = stena->y1;
    stena->crash.x2 = stena->x2;
    stena->crash.y2 = stena->y2;
}


void readStena(ifstream* Map, string stroka_Personage, Stena* stena, int* nomerStena)
{
    string stroka_X = "";
    string stroka_Y = "";
    if (strcmp(stroka_Personage.c_str(), "stena") == 0)
    {
        getline (*Map, stroka_X);
        stena[*nomerStena].x1 = atoi(stroka_X.c_str());
        getline (*Map, stroka_Y);
        stena[*nomerStena].y1 = atoi(stroka_Y.c_str());
        getline (*Map, stroka_X);
        stena[*nomerStena].x2 = atoi(stroka_X.c_str());
        getline (*Map, stroka_Y);
        stena[*nomerStena].y2 = atoi(stroka_Y.c_str());

        fillCrashZone(&stena[*nomerStena]);
        *nomerStena = *nomerStena + 1;
    }
}


Stena stena[10];
int nomerStena = 0;
