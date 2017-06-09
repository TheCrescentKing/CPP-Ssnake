void addFinally(World & w, Point pos){ // Finalement ajouter a la structure la nouvelle partie du corps
        w.snake.tabBody[w.snake.length].pos = pos;
        w.snake.tabBody[w.snake.length].isHead = false;
        w.snake.length += 1;

}

void addBody(World & w){ // Ajout du corps en trouvant ou il y a de la place
    Point pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x-1, w.snake.tabBody[w.snake.length-1].pos.y);

    if(w.snake.length == 1){
        if(pos.x != -1){
            addFinally(w, pos);
            return;
        }
        pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x, w.snake.tabBody[w.snake.length-1].pos.y-1);
        if(pos.y != -1){
            addFinally(w, pos);
            return;
        }
        pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x, w.snake.tabBody[w.snake.length-1].pos.y+1);
        if(pos.y != w.gridSize+1){
            addFinally(w, pos);
            return;
        }
        pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x+1, w.snake.tabBody[w.snake.length-1].pos.y);
        if(pos.x != w.gridSize+1){
            addFinally(w, pos);
            return;
        }
        else{
            return;
        }
    }
    else{
        if((w.snake.tabBody[w.snake.length-2].pos != pos) && (pos.x != -1)){
            addFinally(w, pos);
            return;
        }
        pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x, w.snake.tabBody[w.snake.length-1].pos.y-1);
        if((w.snake.tabBody[w.snake.length-2].pos != pos) && (pos.y != -1)){
            addFinally(w, pos);
            return;
        }
        pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x, w.snake.tabBody[w.snake.length-1].pos.y+1);
        if((w.snake.tabBody[w.snake.length-2].pos != pos) && (pos.y != w.gridSize+1)){
            addFinally(w, pos);
            return;
        }
        pos = makePoint(w.snake.tabBody[w.snake.length-1].pos.x+1, w.snake.tabBody[w.snake.length-1].pos.y);
        if((w.snake.tabBody[w.snake.length-2].pos != pos) && (pos.x != w.gridSize+1)){
            addFinally(w, pos);
            return;
        }
        else{
            return;
        }
    }
}

void snakeEat(World & w, Population & pop, bool & rainbow){ // Test pour savoir si le serpent mange des insectes, ou l'etoile ou pas
    for(int i = 0; i < pop.nbInsects; i++){
        if((pop.tabIns[i].pos.x == w.snake.tabBody[0].pos.x) && (pop.tabIns[i].pos.y == w.snake.tabBody[0].pos.y) && (pop.tabIns[i].dureeVie == -1)){
            pop.tabIns[i].dureeVie = elapsedTime() - pop.tempsCreation;
            addBody(w);
            if((w.multiplierSize > 1)){
                w.score += w.multiplierSize;
            }else{
                w.score++;
            }
        }
    }
    if((w.isStar) && (w.star.pos == w.snake.tabBody[0].pos)){
        if(w.isStarTimer){
            w.starScore += 2*w.multiplierSize;
        }else{
            w.starScore += 2;
        }
        w.isStar = false;
        rainbow = true;
        if(!w.isStarTimer){
            w.isStarTimer = true;
            w.timerTime = elapsedTime();
        }
        if(w.isStarTimer){
            w.chainedStars++;
            w.multiplierSize += 1;
        }
        if(w.chainedStars > 1)
            w.isScoreMultiplier = true;
    }
}

bool collisionWithObst(World w){ // Test de colliusion du serpent avec l'obstacle
    for(int i =0; i < w.nbObstacles; i++){
        for(int o = 0; o < w.tabObstacles[i].obstSize; o++){
            if(w.tabObstacles[i].T[o] == w.snake.tabBody[0].pos){
                return true;
            }

        }
    }
    return false;
}

bool willSnakeCollide(World & w, int direction){ //direction = 1 is up, 2 is down, 3 is left, 4 is right
    if(w.snake.length != 1){
        switch (direction) {
            case 1:
                if(w.snake.tabBody[1].pos.y != (w.snake.tabBody[0].pos.y + 1)){
                    return false;
                }else
                    return true;
                break;
            case 2:
                if(w.snake.tabBody[1].pos.y != (w.snake.tabBody[0].pos.y - 1)){
                    return false;
                }else
                    return true;
                break;
            case 3:
                if(w.snake.tabBody[1].pos.x != (w.snake.tabBody[0].pos.x - 1)){
                    return false;
                }else
                    return true;
                break;
            case 4:
                if(w.snake.tabBody[1].pos.x != (w.snake.tabBody[0].pos.x + 1)){
                    return false;
                }else
                    return true;
                break;
            default:
                return true;
        }
    }else{
        return false;
    }
}

void updateBody(World & w){ // Precedure qui met a jour le reste du corps apre que la tete ait bouge
    for(int i = w.snake.length; i > 0; i--){
        w.snake.tabBody[i].pos = w.snake.tabBody[i-1].pos;
    }
}
