#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Sabit deðerler
#define Labirent_X 21
#define Labirent_Y 23
#define Blok_Boyut 20
#define PacMan_Boyut 8
#define Hayalet_Boyut 8
#define Hayalet_Sayi 4
#define Dosya "YuksekSkor.txt"
#define Canlar 3

//Renkler
#define Duvar_Renk BLUE
#define Nokta_Renk YELLOW
#define PacMan_Renk YELLOW
#define Guclendirici_Renk WHITE
#define Arkaplan_Renk BLACK
#define Hayalet_Kirmizi RED
#define Hayalet_Pembe MAGENTA
#define Hayalet_Turkuaz CYAN
#define Hayalet_Kahve BROWN

//Panel sabitleri
#define Panel_Baslat_X (Labirent_X * Blok_Boyut + 20)
#define Panel_Y 200

//Hayalet struct
typedef struct
{
    int x, y;
    int yon;
    int renk;
    int kork;
} Hayalet;

//Genel deðiþkenler
int pacman_X = 8, pacman_Y = 17;
int yon = 0;
int puan = 0;
int YuksekSkor = 0;
int KalanNokta = 0;
int guclendiriciZaman = 0;
int KalanCanlar = Canlar;
Hayalet hayaletler[Hayalet_Sayi];
int agizAcik = 1;
int oyunBitti = 0;
int kazandiniz = 0;
int respawnZaman = 0;

//Labirent haritasý
int orijinalLabirent[Labirent_Y][Labirent_X] = 
{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1},
    {1, 3, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 3, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1},
    {1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
    {1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 0, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 2, 1, 0, 1, 0, 0, 0, 1, 0, 1, 2, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1},
    {1, 3, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 3, 1},
    {1, 1, 2, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 2, 1, 1},
    {1, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1},
    {1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1},
    {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

//Oyun sýrasýnda kullanýlan harita
int Labirent[Labirent_Y][Labirent_X];

//Fonksiyon bildirimleri
void YuksekSkorYukle();
void YuksekSkorKayit();
void reset();
void BaslaOyun();
void BaslatHayaletler();
void CizLab();
void CizPacman();
void CizHayaletler();
void CizPanel();
void CizCanlar();
void giris();
int PacmanHareketTakip(int x, int y);
int HayaletHareketTakip(int x, int y);
void PacmanHareket();
void HayaletHareket();
void CarpismaTakip();
void NoktaTakip();
void YazSkor();
void GosterOyunBitti();
void GosterKazandin();
void Oyun();
void RespawnPacman();

//Fonksiyonlar
void YuksekSkorYukle()
{
    FILE *file = fopen(Dosya, "r");
    if(file != NULL)
    {
        fscanf(file, "%d", &YuksekSkor);
        fclose(file);
    }
    else
        YuksekSkor = 0;
}

void YuksekSkorKayit()
{
    if(puan > YuksekSkor)
    {
        YuksekSkor = puan;
        FILE *file = fopen(Dosya, "w");
        if(file != NULL)
        {
            fprintf(file, "%d", YuksekSkor);
            fclose(file);
        }
    }
}

void BaslatHayaletler()
{
    int i;
    int renkler[4] = {Hayalet_Kirmizi, Hayalet_Pembe, Hayalet_Turkuaz, Hayalet_Kahve};
    int basla_X[4] = {10, 11, 12, 10};
    int basla_Y[4] = {11, 11, 11, 12};
    
    for(i = 0; i < Hayalet_Sayi; i++)
    {
        hayaletler[i].x = basla_X[i];
        hayaletler[i].y = basla_Y[i];
        hayaletler[i].yon = rand() % 4;
        hayaletler[i].renk = renkler[i];
        hayaletler[i].kork = 0;
    }
}

void reset()
{	
    //Genel deðiþkenleri sýfýrlar
    int i, j;
    pacman_X = 8;
    pacman_Y = 17;
    yon = 0;
    puan = 0;
    KalanNokta = 0;
    guclendiriciZaman = 0;
    KalanCanlar = Canlar;
    agizAcik = 1;
    oyunBitti = 0;
    kazandiniz = 0;
    respawnZaman = 0;
    
    // Haritayý ilk haline geri getirir
    for(i = 0; i < Labirent_Y; i++)
    {
        for(j = 0; j < Labirent_X; j++)
        {
            Labirent[i][j] = orijinalLabirent[i][j];
            if(Labirent[i][j] == 2 || Labirent[i][j] == 3) 
                KalanNokta++;
        }
    }
    
    BaslatHayaletler();
}

void RespawnPacman()
{
    pacman_X = 8;
    pacman_Y = 17;
    yon = 0;
    respawnZaman = 0;
    
    BaslatHayaletler();
}

void CizLab()
{
    int i, j;
    
    for(i = 0; i < Labirent_Y; i++)
    {
        for(j = 0; j < Labirent_X; j++)
        {
            int x = j * Blok_Boyut;
            int y = i * Blok_Boyut;
            
            if(Labirent[i][j] == 1)
            {
                //Duvar
                setcolor(Duvar_Renk);
                rectangle(x + 4, y + 4, x + Blok_Boyut - 4, y + Blok_Boyut - 4);
                setfillstyle(SOLID_FILL, Duvar_Renk);
                bar(x + 5, y + 5, x + Blok_Boyut - 5, y + Blok_Boyut - 5);
            } 
            else if(Labirent[i][j] == 2)
            {
                //Noktalar
                setcolor(Nokta_Renk);
                setfillstyle(SOLID_FILL, Nokta_Renk);
                fillellipse(x + Blok_Boyut / 2, y + Blok_Boyut / 2, 2, 2);
            }
            else if(Labirent[i][j] == 3)
            {
                //Güçlendiriciler
                setcolor(Guclendirici_Renk);
                setfillstyle(SOLID_FILL, Guclendirici_Renk);
                fillellipse(x + Blok_Boyut / 2, y + Blok_Boyut / 2, 6, 6);
            }
        }
    }
}

void BaslaOyun()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "C:\\TC\\BGI");
    
    srand(time(NULL));
    
    YuksekSkorYukle();
    
    reset();
    
    setbkcolor(Arkaplan_Renk);
    cleardevice();
}

void CizPacman()
{
    if(respawnZaman > 0) 
        return;
        
    int x = pacman_X * Blok_Boyut + Blok_Boyut / 2;
    int y = pacman_Y * Blok_Boyut + Blok_Boyut / 2;
    
    setcolor(PacMan_Renk);
    setfillstyle(SOLID_FILL, PacMan_Renk);
    
    fillellipse(x, y, PacMan_Boyut, PacMan_Boyut);
    
    if(agizAcik)
    {
        setcolor(Arkaplan_Renk);
        setfillstyle(SOLID_FILL, Arkaplan_Renk);
        
        switch(yon)
        {
            case 0: //Sað
                pieslice(x, y, 330, 30, PacMan_Boyut);
                break;
            case 1: //Aþaðý
                pieslice(x, y, 60, 120, PacMan_Boyut);
                break;
            case 2: //Sol
                pieslice(x, y, 150, 210, PacMan_Boyut);
                break;
            case 3: //Yukarý
                pieslice(x, y, 240, 300, PacMan_Boyut);
                break;
        }
    }
}

void CizHayaletler()
{
    int i, j;
    
    for(i = 0; i < Hayalet_Sayi; i++)
    {
        int x = hayaletler[i].x * Blok_Boyut + Blok_Boyut / 2;
        int y = hayaletler[i].y * Blok_Boyut + Blok_Boyut / 2;
        
        int hayaletRenk = hayaletler[i].kork ? LIGHTBLUE : hayaletler[i].renk;
        
        setcolor(hayaletRenk);
        setfillstyle(SOLID_FILL, hayaletRenk);
        
        //Vücut
        fillellipse(x, y - 2, Hayalet_Boyut, Hayalet_Boyut - 2);
        bar(x - Hayalet_Boyut, y - 2, x + Hayalet_Boyut, y + Hayalet_Boyut);
        
        setcolor(Arkaplan_Renk);
        for(j = 0; j < 3; j++)
        {
            int alt_X = x - Hayalet_Boyut + j * Hayalet_Boyut;
            line(alt_X, y + Hayalet_Boyut, alt_X + Hayalet_Boyut / 2, y + Hayalet_Boyut - 3);
            line(alt_X + Hayalet_Boyut / 2, y + Hayalet_Boyut - 3, alt_X + Hayalet_Boyut, y + Hayalet_Boyut);
        }
        
        //Gözler
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, WHITE);
        fillellipse(x - 3, y - 3, 2, 2);
        fillellipse(x + 3, y - 3, 2, 2);
        
        setcolor(BLACK);
        fillellipse(x - 3, y - 3, 1, 1);
        fillellipse(x + 3, y - 3, 1, 1);
    }
}

void CizCanlar()
{
    int i;
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(Panel_Baslat_X, 120, "CANLAR:");
    
    for(i = 0; i < KalanCanlar; i++)
    {
        int Can_X = Panel_Baslat_X + 20 + (i * 25);
        int Can_Y = 140;
        
        setcolor(PacMan_Renk);
        setfillstyle(SOLID_FILL, PacMan_Renk);
        fillellipse(Can_X, Can_Y, 6, 6);
        
        setcolor(Arkaplan_Renk);
        pieslice(Can_X, Can_Y, 330, 30, 6);
    }
}

void CizPanel()
{
    //Çerçeve
    setcolor(WHITE);
    rectangle(Panel_Baslat_X - 10, 10, Panel_Baslat_X + Panel_Y, Labirent_Y * Blok_Boyut - 10);
    
    //Bilgiler
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    char puanYaz[50];
    
    sprintf(puanYaz, "PUAN: %d", puan);
    outtextxy(Panel_Baslat_X, 30, puanYaz);
    
    sprintf(puanYaz, "EN YUKSEK: %d", YuksekSkor);
    outtextxy(Panel_Baslat_X, 50, puanYaz);
    
    sprintf(puanYaz, "NOKTA: %d", KalanNokta);
    outtextxy(Panel_Baslat_X, 70, puanYaz);
    
    if(guclendiriciZaman > 0)
    {
        sprintf(puanYaz, "GUC: %d", guclendiriciZaman / 10);
        outtextxy(Panel_Baslat_X, 90, puanYaz);
        setcolor(YELLOW);
        outtextxy(Panel_Baslat_X, 105, "SUPER PACMAN!");
        setcolor(WHITE);
    }
    
    CizCanlar();
    
    //Kontroller
    outtextxy(Panel_Baslat_X, 200, "KONTROLLER:");
    outtextxy(Panel_Baslat_X, 220, "Ok Tuslari:");
    outtextxy(Panel_Baslat_X, 235, "  Hareket");
    outtextxy(Panel_Baslat_X, 255, "R: Yeniden");
    outtextxy(Panel_Baslat_X, 270, "ESC: Cikis");
    
    //Oyun durumu
    if(respawnZaman > 0)
    {
        setcolor(RED);
        sprintf(puanYaz, "RESPAWN: %d", respawnZaman / 20);
        outtextxy(Panel_Baslat_X, 170, puanYaz);
    }
}

void giris()
{
    if(kbhit())
    {
        int key = getch();
        if(key == 0)
        {
            key = getch();
            switch(key)
            {
                case 77:
                    yon = 0; //Sað
                    break;
                case 80:
                    yon = 1; //Aþaðý
                    break;
                case 75:
                    yon = 2; //Sol
                    break;
                case 72:
                    yon = 3; //Yukarý
                    break;
            }
        }
        else if(key == 27)
        {
            exit(0);
        }
        else if(key == 'r' || key == 'R')
        {
            if(oyunBitti || kazandiniz)
                reset();
        }
    }
}

int PacmanHareketTakip(int x, int y)
{
    //Tünel geçiþi
    if(y == 10)
    {
        if(x < 0)
            return 1;
        if(x >= Labirent_X)
            return 1;
    }
    if(x < 0 || x >= Labirent_X || y < 0 || y >= Labirent_Y)
        return 0;
        
    return Labirent[y][x] != 1; //Duvar kontrol
}

int HayaletHareketTakip(int x, int y)
{	
    //Hayalet tünel
    if(x < 0 || x >= Labirent_X || y < 0 || y >= Labirent_Y)
        return 0;
        
    return Labirent[y][x] != 1;
}

void PacmanHareket()
{
    if(oyunBitti || kazandiniz || respawnZaman > 0)
        return;
    
    int yeniX = pacman_X;
    int yeniY = pacman_Y;
    
    switch (yon)
    {
        case 0:
            yeniX++;
            break;
        case 1:
            yeniY++;
            break;
        case 2:
            yeniX--;
            break;
        case 3:
            yeniY--;
            break;
    }
    
    if(yeniY == 10)
    {
        if(yeniX < 0)
        {
            yeniX = Labirent_X - 1;
        }
        else if(yeniX >= Labirent_X)
        {
            yeniX = 0;
        } 
    }
    
    if(PacmanHareketTakip(yeniX, yeniY))
    {
        pacman_X = yeniX;
        pacman_Y = yeniY;
    }
}

void HayaletHareket()
{
    int i;
    
    for(i = 0; i < Hayalet_Sayi; i++)
    {
        int yeniX = hayaletler[i].x;
        int yeniY = hayaletler[i].y;
        
        int deneme = 0;
        do
        {
            hayaletler[i].yon = rand() % 4;
            yeniX = hayaletler[i].x;
            yeniY = hayaletler[i].y;
            
            switch(hayaletler[i].yon)
            {
                case 0:
                    yeniX++;
                    break;
                case 1:
                    yeniY++;
                    break;
                case 2:
                    yeniX--;
                    break;
                case 3:
                    yeniY--;
                    break;
            }
            deneme++;
        }
        while(!HayaletHareketTakip(yeniX, yeniY) && deneme < 10);
        
        if(HayaletHareketTakip(yeniX, yeniY))
        {
            hayaletler[i].x = yeniX;
            hayaletler[i].y = yeniY;
        }
    }
}

void CarpismaTakip()
{
    int i;
    
    if(oyunBitti || kazandiniz || respawnZaman > 0)
        return;
        
    for(i = 0; i < Hayalet_Sayi; i++)
    {
        if(hayaletler[i].x == pacman_X && hayaletler[i].y == pacman_Y)
        {
            if(hayaletler[i].kork)
            {
                puan += 200;
                
                if(i < 3)
                {
                    hayaletler[i].x = 10 + i;
                    hayaletler[i].y = 11;
                }
                else
                {
                    hayaletler[i].x = 10;
                    hayaletler[i].y = 12;
                }
                hayaletler[i].kork = 0;
            }
            else
            {
                //Can kaybetme
                KalanCanlar--;
                
                if(KalanCanlar <= 0)
                {
                    oyunBitti = 1;
                    YuksekSkorKayit();
                }
                else
                {
                    respawnZaman = 60;
                }
                break;
            }
        }
    }
}

void NoktaTakip()
{
    int i;
    if(oyunBitti || kazandiniz || respawnZaman > 0)
        return;
        
    if(Labirent[pacman_Y][pacman_X] == 2)
    {
        Labirent[pacman_Y][pacman_X] = 0;
        puan += 10;
        KalanNokta--;
    }
    else if(Labirent[pacman_Y][pacman_X] == 3)
    {
        Labirent[pacman_Y][pacman_X] = 0;
        puan += 50;
        KalanNokta--;
        guclendiriciZaman = 100;
        
        for(i = 0; i < Hayalet_Sayi; i++)
        {
            hayaletler[i].kork = 1;
        }
    }
    
    if(KalanNokta == 0)
    {
        kazandiniz = 1;
        YuksekSkorKayit();
    }
}

void GosterOyunBitti()
{
    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(120, 200, "OYUN BITTI!");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(Panel_Baslat_X, 300, "R: Yeniden Basla");
    outtextxy(Panel_Baslat_X, 315, "ESC: Cikis");
    
    if(puan == YuksekSkor && puan > 0)
    {
        setcolor(YELLOW);
        outtextxy(Panel_Baslat_X, 280, "YENI REKOR!");
    }
}

void GosterKazandin()
{
    setcolor(GREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(120, 200, "KAZANDINIZ!");
    
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(Panel_Baslat_X, 300, "R: Yeniden Basla");
    outtextxy(Panel_Baslat_X, 315, "ESC: Cikis");
    
    if(puan == YuksekSkor)
    {
        setcolor(YELLOW);
        outtextxy(Panel_Baslat_X, 280, "YENI REKOR!");
    }
}

void Oyun()
{
    int KareSayisi = 0;
    
    while(1)
    {
        int i;
        cleardevice();
        giris();
        
        if(respawnZaman > 0)
        {
            respawnZaman--;
            if(respawnZaman == 0)
            {
                RespawnPacman();
            }
        }
        
        if(!oyunBitti && !kazandiniz)
        {
            if(KareSayisi % 2 == 0)
            {
                PacmanHareket();
                HayaletHareket();
            }
        }
        
        if(KareSayisi % 10 == 0)
        {
            agizAcik = !agizAcik;
        }
        
        if(guclendiriciZaman > 0)
        {
            guclendiriciZaman--;
            if(guclendiriciZaman == 0)
            {
                for(i = 0; i < Hayalet_Sayi; i++)
                {
                    hayaletler[i].kork = 0;
                }
            }
        }
        
        NoktaTakip();
        CarpismaTakip();
        
        CizLab();
        
        if(!oyunBitti)
        {
            CizPacman();
        }
        CizHayaletler();
        CizPanel();
        
        if(oyunBitti)
        {
            GosterOyunBitti();
        }
        else if(kazandiniz)
        {
            GosterKazandin();
        }
        
        KareSayisi++;
        delay(50);
    }
}

int main()
{
    BaslaOyun();
    Oyun();
    
    closegraph();
    
    return 0;
}
