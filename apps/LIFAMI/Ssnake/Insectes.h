Point makePoint(float x, float y){
    Point res;
    res.x = x;
    res.y = y;
    return res;
}

Colour makeColour(unsigned char r, unsigned char g, unsigned char b){
    Colour res;
    res.r = r;
    res.g = g;
    res.b = b;
    return res;
}

Colour randColour(Colour good, int range){ // Couleur aleatoire
    Colour res = good;
    int r, g, b;
    if ((r = (rand() % range - range/2) + good.r) > 255){
        res.r = 255;
    }
    else if(r < 0){
        res.r = 0;
    }
    else
        res.r = r;
    if ((g = (rand() % range - range/2) + good.g) > 255){
        res.g = 255;
    }
    else if(g < 0){
        res.g = 0;
    }
    else
        res.g = g;
    if ((b = (rand() % range - range/2) + good.b) > 255){
        res.b = 255;
    }
    else if(b < 0){
        res.b = 0;
    }
    else
        res.b = b;
    return res;
}

float distance(Point A, Point B){
    Point C = A - B;
    return sqrt(pow(C.x, 2) + pow(C.y, 2));
}

bool notInObstacle(World w, Point pos){ // Test pour que les insectes et l'etoile n'apparaisent pas dans un Obstacle
    for (int i = 0; i < w.nbObstacles; i++){
        for(int o = 0; o < w.tabObstacles[i].obstSize; o++){
            if(w.tabObstacles[i].T[o] == pos){
                return false;
            }
        }
    }
    return true;
}

bool notInInsect(Population pop, Point pos){ // Test pour que l'etoile n'apparaisse pas sur un insecte
    for (int i =0; i < pop.nbInsects; i++){
        if(pos == pop.tabIns[i].pos){
            return false;
        }
    }
    return true;
}

void initInsects(Population & pop, int range, World & w){ // Initialisation de la structure des Insectes et ajout de l'etoile (1 chance sur 2)
    for(int i = 0; i < pop.nbInsects; i++){
        pop.tabIns[i].col = randColour(pop.good, range);
        pop.tabIns[i].dureeVie = -1;
        do{
            pop.tabIns[i].pos = randPosSnake(w);
        }while(!(notInObstacle(w, pop.tabIns[i].pos)));
    }
    if(((rand() % 2) == 1) && (!w.isStar)){
        do{
            w.star.pos = randPosSnake(w);
        }while((!notInObstacle(w, w.star.pos)) && (!notInInsect(pop, w.star.pos)));
        w.isStar = true;
    }
}

void init(Population & pop, World & w){ //Initialisation de la popularion
    pop.nbInsects = 2;
    pop.good = makeColour(125, 125, 125);
    initInsects(pop, RANGE, w);
    pop.tempsCreation = elapsedTime();
}

void minMaxLifeTime(Population & pop, float & minLife, float & maxLife){ // Moyenne du temps de vie entre les insectes
    minLife = pop.tabIns[0].dureeVie;
    maxLife = minLife;
    for(int i = 1; i < pop.nbInsects; i++){
        if(pop.tabIns[i].dureeVie < minLife)
            minLife = pop.tabIns[i].dureeVie;
        if(pop.tabIns[i].dureeVie > maxLife)
            maxLife = pop.tabIns[i].dureeVie;
    }
}

Colour averageColourOfGoodInsects(Population & pop, float certaineDuree){ // Moyenne de la couleur des insectes
    Colour res;
    int nbSuperieur = 0;
    float moyenneR, moyenneG, moyenneB;
    moyenneR = 0;
    moyenneG = 0;
    moyenneB = 0;
    for (int i = 0; i < pop.nbInsects; i++){
        if(pop.tabIns[i].dureeVie > certaineDuree){
            nbSuperieur++;
            moyenneR += pop.tabIns[i].col.r;
            moyenneG += pop.tabIns[i].col.g;
            moyenneB += pop.tabIns[i].col.b;
        }
    }
    moyenneR /= nbSuperieur;
    moyenneG /= nbSuperieur;
    moyenneB /= nbSuperieur;
    res = makeColour(moyenneR, moyenneG, moyenneB);
    return res;
}

bool insectsDead(Population & pop){ // Savoir si les insectes sont morts ou pas
    for(int i = 0; i < pop.nbInsects; i++){
        if(pop.tabIns[i].dureeVie == -1){
            return false;
        }
    }
    return true;
}

void draw(Population & pop, World & w, bool rainbow){ // Dessiner les insectes sur l'ecran
    for(int i = 0; i < pop.nbInsects; i++){
        if(pop.tabIns[i].dureeVie == -1){
            int x, y;
            mousePos(x,y);
            Point mouse = makePoint(x, y);
            float dist = distance(mouse, pop.tabIns[i].pos);
            if (dist < 20){
                pop.tabIns[i].dureeVie = elapsedTime() - pop.tempsCreation;
            }
            else{
                if(rainbow)
                    color(rand() % 256, rand() % 256, rand() % 256);
                else
                    color(pop.tabIns[i].col.r, pop.tabIns[i].col.g, pop.tabIns[i].col.b);
                circleFill(pop.tabIns[i].pos.x*w.squareInGridSize+w.squareInGridSize/2, pop.tabIns[i].pos.y*w.squareInGridSize+w.squareInGridSize/2, 5);
            }
        }

    }
    if(insectsDead(pop)){
        float minLife, maxLife;
        minMaxLifeTime(pop, minLife, maxLife);
        float certaineDuree = minLife + (maxLife - minLife) * 0.01;
        pop.good = averageColourOfGoodInsects(pop, certaineDuree);
        initInsects(pop, RANGE, w);
        pop.tempsCreation = elapsedTime();
    }
}

