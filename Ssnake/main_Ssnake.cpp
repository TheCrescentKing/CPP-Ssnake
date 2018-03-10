#include <Grapic.h>
#include <fstream>
using namespace grapic;

const int MAXTABLE = 100;
const int DIMW = 700;
const int NB_INSECTES = 100;
const int RANGE = 50;

struct Point{
    int x, y;
};

struct Colour{
    unsigned char r, g, b;
};

bool operator==(Point A, Point B){
    return ((A.x == B.x) && (A.y == B.y));
}

bool operator!=(Point A, Point B){
    return(!(A == B));
}

Point operator+(struct Point a, struct Point b){
    Point res;
    res.x = a.x + b.x;
    res.y = a.y + b.y;
    return res;
}

Point operator-(struct Point a, struct Point b){
    Point res;
    res.x = a.x - b.x;
    res.y = a.y - b.y;
    return res;
}

struct Insect{
    Point pos;
    Colour col;
    float dureeVie;
};

struct Population{
    Insect tabIns[NB_INSECTES];
    int nbInsects;
    float tempsCreation;
    Colour good;
};

struct Body{ //Chaque carreau du corps du serpent.
    Point pos;
    bool isHead;
    int heading;
};

struct Snake{
    Body tabBody[MAXTABLE];
    int length, lastLength;
};

struct Obstacle{
    int obstSize;
    Point T[MAXTABLE];
};

struct Star{
    Point pos;
    Image starImg;
};

struct World{
    Image bG;
    Snake snake;
    int gridSize, squareInGridSize, nbObstacles, highScore, starScore, score, chainedStars, multiplierSize;
    float currentTime, timerTime, worldSpeed;
    bool isStar, isStarTimer, isScoreMultiplier;
    Obstacle tabObstacles[MAXTABLE];
    Star star;
};

Point randPosSnake(World & w){ //Choix de position aleatoire pour le serpent et les insectes et l'etoile
    Point res;
    res.x = rand() % w.gridSize;
    res.y = rand() % w.gridSize;
    return res;
}

void writeHighScore(int intHighScore, bool haveImage){ // On ecrit au fichier le meilleur score, ce qui depend de si on a joue avec l'image du fond ou pas
    if(haveImage){
        std::ofstream highscoreHard;
        highscoreHard.open("../../apps/LIFAMI/Ssnake/highscoreHard.txt");
        highscoreHard<<intHighScore<<std::endl;
        highscoreHard.close();
    }else{
        std::ofstream highscoreNormal;
        highscoreNormal.open("../../apps/LIFAMI/Ssnake/highscoreNormal.txt");
        highscoreNormal<<intHighScore<<std::endl;
        highscoreNormal.close();
        }
}

void getHighScore(int & intHighScore, bool haveImage){ // On lis les fichiers qu'on a ecrit precedament
    if(haveImage){
        std::ifstream highscoreHard;
        highscoreHard.open("../../apps/LIFAMI/Ssnake/highscoreHard.txt");
        highscoreHard>>intHighScore;
        highscoreHard.close();
    }else{
        std::ifstream highscoreNormal;
        highscoreNormal.open("../../apps/LIFAMI/Ssnake/highscoreNormal.txt");
        highscoreNormal>>intHighScore;
        highscoreNormal.close();
        }
}

#include "Insectes.h"
#include "Snake.h"

void initWorld(World & w, bool haveImage){ // On initialise la structure world
    int x, y;
    w.gridSize = DIMW*0.04;
    w.squareInGridSize = DIMW/w.gridSize;
    if(haveImage)
        w.bG = image("data/Insectes/coquelicots.jpg");
    w.star.starImg = image("data/Ssnake/Star.png");
    w.snake.tabBody[0].isHead = true;
    w.snake.tabBody[0].pos = randPosSnake(w);
    w.snake.length = 1;
    w.snake.lastLength = 1;
    w.snake.tabBody[0].heading = 0;
    w.currentTime = 0;
    w.nbObstacles = 0;
    w.starScore = 0;
    w.multiplierSize = 0;
    w.chainedStars = 0;
    w.score = 0;
    w.isStarTimer = false;
    w.isScoreMultiplier = false;
    w.worldSpeed = 0.09;
    w.timerTime = 2000000000;
    getHighScore(w.highScore, haveImage);
}

bool isGameOver(World w){ // Le test pour savoir si on a perdu ou pas (sortir de la fenetre collision avec objets etc)
    if((w.snake.tabBody[0].pos.y == (w.gridSize)) || (w.snake.tabBody[0].pos.y == -1) || (w.snake.tabBody[0].pos.x == (w.gridSize)) || (w.snake.tabBody[0].pos.x == -1) || (collisionWithObst(w))){
        return true;
    }
    else{
        for(int i = 5; i < w.snake.length; i++){
             if(w.snake.tabBody[0].pos == w.snake.tabBody[i].pos){
                return true;
            }
        }
        return false;
    }
}

bool notInSnake(World & w, Point pos){ // fonction pour savoir si un placement aleatoire le met dedans le serpent ou pas (surtout pour les obstacles)
    for(int i = 0; i < w.snake.length; i++){
        if(w.snake.tabBody[i].pos == pos)
            return false;
    }
    return true;
}

bool notInStar(World w, Point pos){ // Test pour savoir si un positionement aleatoire n'est pas sur l'etoile
    if(w.star.pos == pos){
        return false;
    }else{
        return true;
    }
}

void updateWorld(World & w, Population & pop, bool & rainbow){ // Mise a jour de toutes les donnes qu'il faout mettre a jour (la position, les obstacles, si on a mange ou pas)
    snakeEat(w, pop, rainbow);
    if(w.currentTime < (elapsedTime()-(w.worldSpeed -(w.snake.length*0.001)))){
         w.currentTime = elapsedTime();
         if((isKeyPressed(SDLK_UP)) && (!willSnakeCollide(w, 1)) && (w.snake.tabBody[0].heading != 1)){
            w.snake.tabBody[0].heading = 1;
            if(w.snake.length != 1){
                updateBody(w);
            }
            w.snake.tabBody[0].pos.y += 1;
            }else if((isKeyPressed(SDLK_DOWN)) && (!willSnakeCollide(w, 2)) && (w.snake.tabBody[0].heading != 2)){
                w.snake.tabBody[0].heading = 2;
                if(w.snake.length != 1){
                    updateBody(w);
                }
                w.snake.tabBody[0].pos.y -= 1;
            }else if((isKeyPressed(SDLK_LEFT)) && (!willSnakeCollide(w, 3)) && (w.snake.tabBody[0].heading != 3)){
                w.snake.tabBody[0].heading = 3;
                if(w.snake.length != 1){
                    updateBody(w);
                }
                w.snake.tabBody[0].pos.x -= 1;
            }else if((isKeyPressed(SDLK_RIGHT)) && (!willSnakeCollide(w, 4)) && (w.snake.tabBody[0].heading != 4)){
                w.snake.tabBody[0].heading = 4;
                if(w.snake.length != 1){
                    updateBody(w);
                }
                w.snake.tabBody[0].pos.x += 1;
            }else if(w.snake.tabBody[0].heading == 1){
                if(w.snake.length != 1){
                    updateBody(w);
                }
                w.snake.tabBody[0].pos.y += 1;
            }else if((w.snake.tabBody[0].heading == 2)){
            if(w.snake.length != 1){
                updateBody(w);
            }
            w.snake.tabBody[0].pos.y -= 1;
            }else if((w.snake.tabBody[0].heading == 3)){
            if(w.snake.length != 1){
                updateBody(w);
            }
            w.snake.tabBody[0].pos.x -= 1;
            }else if((w.snake.tabBody[0].heading == 4)){
            if(w.snake.length != 1){
                updateBody(w);
            }
            w.snake.tabBody[0].pos.x += 1;
            }
    }
    if((w.snake.lastLength + 5) == w.snake.length){ // ajout obstacles
        w.snake.lastLength += 5;
        w.tabObstacles[w.nbObstacles].obstSize = rand() % 2 + 1;
        for(int o = 0; o < w.tabObstacles[w.nbObstacles].obstSize; o++){
            do{
            w.tabObstacles[w.nbObstacles].T[o].x = rand() % w.gridSize;
            w.tabObstacles[w.nbObstacles].T[o].y = rand() % w.gridSize;
            }while ((!notInSnake(w, w.tabObstacles[w.nbObstacles].T[o])), (!(notInStar(w, w.tabObstacles[w.nbObstacles].T[o]))));
        }
        w.nbObstacles++;
    }
}

void drawWorld(World & w, Population & pop, bool haveImage, bool rainbow){ // Dessiner tout ce qu'il faut dessiner
    if(haveImage)
        image_draw(w.bG, 0, 0, DIMW, DIMW);
    else{
        if(rainbow)
            color(rand() % 256, rand() % 256, rand() % 256);
        else
            color(0, 255, 0);
        line(0, DIMW, DIMW, DIMW);
    }
    draw(pop, w, rainbow);
    if(w.isStar){
        image_draw(w.star.starImg, w.star.pos.x*w.squareInGridSize, w.star.pos.y*w.squareInGridSize, w.squareInGridSize, w.squareInGridSize);
    }
    if(rainbow)
        color(rand() % 256, rand() % 256, rand() % 256);
    else
        color(43, 170, 16);
    for(int i = 0; i < w.snake.length; i++){
        rectangleFill(w.snake.tabBody[i].pos.x * w.squareInGridSize, w.snake.tabBody[i].pos.y * w.squareInGridSize, (w.snake.tabBody[i].pos.x + 1) * w.squareInGridSize, (w.snake.tabBody[i].pos.y + 1) * w.squareInGridSize);
    }
    if(rainbow)
        color(rand() % 256, rand() % 256, rand() % 256);
    else
        color(255, 50, 50);
    for(int i = 0; i < w.nbObstacles; i++){
        for(int o = 0; o < w.tabObstacles[i].obstSize; o++){
            rectangleFill(w.tabObstacles[i].T[o].x * w.squareInGridSize, w.tabObstacles[i].T[o].y * w.squareInGridSize, (w.tabObstacles[i].T[o].x + 1) * w.squareInGridSize, (w.tabObstacles[i].T[o].y + 1) * w.squareInGridSize);
        }
    }
    if(rainbow)
        color(rand() % 256, rand() % 256, rand() % 256);
    else
        color(43, 196, 16);
    fontSize(20);
    print(5, DIMW+5, "Score = ");
    print(DIMW*1/4-40, DIMW+5, w.score+w.starScore);
    if(rainbow)
        color(rand() % 256, rand() % 256, rand() % 256);
    else
        color(200, 50, 50);
    print(DIMW*1/3-40, DIMW+5, "Obstacles = ");
    int sumObstacles = 0;
    for(int i = 0; i < w.nbObstacles; i++){
        sumObstacles += w.tabObstacles[i].obstSize;
    }
    print(DIMW*3/4-120, DIMW+5, sumObstacles);
    if(rainbow)
        color(rand() % 256, rand() % 256, rand() % 256);
    else
        color(255, 252, 127);
    print(DIMW*6/9, DIMW+5, "Highscore = ");
    print(DIMW-40, DIMW+5, w.highScore);
    if(w.isScoreMultiplier){
        color(rand() % 256, rand() % 256, rand() % 256);
        fontSize(50);
        print(DIMW/2-25, DIMW/2+25, "x");
        fontSize(100);
        print(DIMW/2, DIMW/2, w.multiplierSize);
    }
}

void isStarTimerFinished(World & w, bool & rainbow, bool & timerFinito){ // Test pour savoir si, quand on mange une etoile si au bout de 4 secondes on finit les couleurs et la vitesse augmentee et le multiplicateur de score (etoiles enchainees)
    if((w.timerTime + (4*w.chainedStars)) <= elapsedTime()){
        if(w.isStarTimer){
            w.isStarTimer = false;
            w.timerTime = 2000000000;
            rainbow = false;
            w.chainedStars = 0;
            timerFinito = true;
            w.isScoreMultiplier = false;
            w.multiplierSize = 0;
        }
    }else if(w.isStarTimer){
        timerFinito = false;
    }
}

bool starting(){ // debut du programme qui demande si on veut jouer avec l'image de fond ou pas
    winClearEvent();
    color(0, 255, 0);
    fontSize(30);
    print(5, DIMW/2, "Play with background image ? y/n");
    print(5, DIMW/2 - 50, "[Harder]");
    winDisplay();
    while((!winHasFinished() && (!isKeyPressed(SDLK_n) || !isKeyPressed(SDLK_y))))
    {
        Grapic::singleton().manageEvent();
        delay(50);
    if(isKeyPressed(SDLK_y))
        return true;
    else if(isKeyPressed(SDLK_n))
        return false;
    }
    return false;
}

bool ending(World & w, Population & pop , bool & rainbow, bool & speedSet, bool & timerFinito, bool & haveImage){ // Fin du programme qui affiche les donnees et demande si on veut jouer a nouveau
    if(!isGameOver(w))
        return false;
    backgroundColor(0, 0, 0);
    if(w.highScore < (w.score+w.starScore)){
        writeHighScore((w.score+w.starScore), haveImage);
    }
    while(!isKeyPressed(SDLK_SPACE)){
        winClear();
        if(rainbow)
            color(rand() % 256, rand() % 256, rand() % 256);
        else
            color(43, 196, 16);
        fontSize(45);
        print(DIMW/4, DIMW - 50, "Game Over");
        fontSize(30);
        print(DIMW/4, DIMW - 100, "Your score was :");
        print(DIMW/4, DIMW - 200, "Your highscore is :");
        fontSize(25);
        print(DIMW/2, DIMW - 150, w.score+w.starScore);
        getHighScore(w.highScore, haveImage);
        print(DIMW/2, DIMW- 250, w.highScore);
        print(5, 5, "Press Space");
        winDisplay();
    }
    winClearEvent();
    winClear();
    color(0, 255, 0);
    fontSize(30);
    print(5, DIMW/2, "Play again ? y/n");
    winDisplay();
    while((!winHasFinished() && (!isKeyPressed(SDLK_n) || !isKeyPressed(SDLK_y))))
    {
        Grapic::singleton().manageEvent();
        delay(50);
    if(isKeyPressed(SDLK_y)){
        winClear();
        delay(330);
        haveImage = starting();
        initWorld(w, haveImage);
        init(pop, w);
        rainbow = false;
        speedSet = false;
        timerFinito = true;
        return false;
    }
    else if(isKeyPressed(SDLK_n))
        return true;
    }
    return false;
}

int main(int , char** )
{
    srand(time(0));
    World world;
    Population pop;
    bool stop=false;
    bool rainbow = false; //Ces trois variables j'aurais du les mettre dans la structure world mais...bon j'ai pas fait.
    bool speedSet = false;
    bool timerFinito = true;
	winInit("Project", DIMW, DIMW+50);
	setKeyRepeatMode(false);
    bool haveImage = starting();
    initWorld(world, haveImage);
    init(pop, world);
    backgroundColor( 0, 0, 0);
    while( !stop )
    {
        winClear();
        isStarTimerFinished(world, rainbow, timerFinito);
        if((!timerFinito) && (!speedSet)){
            world.worldSpeed -= 0.02;
            speedSet = true;
        }else if((timerFinito) && (speedSet)){
            world.worldSpeed += 0.02;
            speedSet = false;
        }
        updateWorld(world, pop, rainbow);
        drawWorld(world, pop, haveImage, rainbow);
        winDisplay();
        stop = ending(world, pop, rainbow, speedSet, timerFinito, haveImage);
    }
    winQuit();
	return 0;
}

