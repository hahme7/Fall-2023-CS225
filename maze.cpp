/* Your code here! */
#include "maze.h"
#include <cmath> //Need for Random numbers in make maze
#include <algorithm>

SquareMaze::SquareMaze() {} //For compiler


void SquareMaze::makeMaze(int width, int height) {
    //Inits
    wid = width;
    hei = height;
    size = width * height;
    sets.addelements(size);
    for(int i = 0; i < wid; i++) {
        right_walls.push_back(std::vector<bool>());
        down_walls.push_back(std::vector<bool>());
        for(int j = 0; j < hei; j++) {
            right_walls[i].push_back(true);
            down_walls[i].push_back(true);
        }
    }

    // Randomly delete walls and make sure no cylces and that there is a perimeter
    int count = 0; //Makes sure we delete all walls needed to delete
    while(count != size-1) {
        int random_X_coord = std::rand()%wid; //Make sure the number is within width
        int random_Y_coor = std::rand()%hei; //Make sure the number is within height
        bool random_bool = std::rand()%2; //Randomly decide if using x or y
        int curr_square_idx = wid*random_Y_coor + random_X_coord;
        int right_idx = wid*random_Y_coor + random_X_coord + 1;
        int down_idx = wid*random_Y_coor + random_X_coord + wid;
        
        if(!random_bool && random_X_coord != wid-1) { //Make sure x is not perimeter
            if(sets.find(curr_square_idx) != sets.find(right_idx)) { //Check if curr square != right of curr square
                if(right_walls[random_X_coord][random_Y_coor]) {
                    setWall(random_X_coord, random_Y_coor, 0, false);
                    sets.setunion(curr_square_idx, right_idx); //Combine curr square and right of it since no more wall
                    count++;
                }
            }
        }

        if(random_bool && random_Y_coor != hei-1) { //Make sure y is not perimeter
            if(sets.find(curr_square_idx) != sets.find(down_idx)) { //Check if curr square != down of curr square
                if(down_walls[random_X_coord][random_Y_coor]) {
                    setWall(random_X_coord, random_Y_coor, 1, false);
                    sets.setunion(curr_square_idx, down_idx); //Combine curr square and down of it since no more wall
                    count++;
                }
            }
        }
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    switch(dir) {
        case 0: //right
            if(x == wid-1) { return false; } //Perimeter
            if(!right_walls[x][y]) { return true; } //No walls to right
            break;
        case 1: //down
            if(y == hei-1) { return false; } //Perimeter
            if(!down_walls[x][y]) { return true; } //No walls downwards
            break;
        case 2: //left 
            if(x == 0) { return false; } //out of index
            if(!right_walls[x-1][y]) { return true; } //No left walls by checking right walls of left index
            break;
        case 3: //up
            if(y == 0) { return false; } //out of index
            if(!down_walls[x][y-1]) { return true; } //No up walls by checking down walls of up index
        default: //Shouldn't be hit but incase an invalid direction is chosen, it returns false
            return false;
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if(dir == 0) { //right
        right_walls[x][y] = exists;
    }
    if(dir == 1) { //down
        down_walls[x][y] = exists;
    }
}

std::vector<int> SquareMaze::solveMaze() {
    //Inits
    std::vector<std::vector<bool>> visited;
    for(int i = 0; i < wid; i++) {
        visited.push_back(std::vector<bool>());
        for(int j = 0; j < hei; j++) {
            visited[i].push_back(false);
        }
    }
    std::stack<int> st;
    std::map<int, int> taken_path;

    //Traverse with BFS (Stacks are good for this)
    visited[0][0] = true;
    st.push(0);
    while(!st.empty()) {
        //Inits
        int curr_square = st.top();
        int curr_X = curr_square%wid;
        int curr_Y = curr_square/wid;
        st.pop();
        
        //Right
        if(canTravel(curr_X, curr_Y, 0) && !visited[curr_X+1][curr_Y]) {
            visited[curr_X+1][curr_Y] = true;
            st.push(wid*curr_Y+curr_X+1);
            taken_path[wid*curr_Y+curr_X+1] = curr_square;
        }
        //Down
        if(canTravel(curr_X, curr_Y, 1) && !visited[curr_X][curr_Y+1]) {
            visited[curr_X][curr_Y+1] = true;
            st.push(wid*curr_Y+curr_X+wid);
            taken_path[wid*curr_Y+curr_X+wid] = curr_square;
        }
        //Left
        if(canTravel(curr_X, curr_Y, 2) && !visited[curr_X-1][curr_Y]) {
            visited[curr_X-1][curr_Y] = true;
            st.push(wid*curr_Y+curr_X-1);
            taken_path[wid*curr_Y+curr_X-1] = curr_square;
        }
        //Up
        if(canTravel(curr_X, curr_Y, 3) && !visited[curr_X][curr_Y-1]) {
            visited[curr_X][curr_Y-1] = true;
            st.push(wid*curr_Y+curr_X-wid);
            taken_path[wid*curr_Y+curr_X-wid] = curr_square;
        }
    }

    //Our taken_path is now recorded
    int max = 0;
    std::vector<int> longestPath;
    for(int i = 0; i < wid; i++) {
        std::vector<int> longs;
        int len = 0;
        int destination = wid*(hei-1)+i;
        while(destination) {
            int diff = destination - taken_path[destination];
            if(diff == 1) {
                longs.push_back(0);
            } else if(diff == wid) { longs.push_back(1); }
            else if(diff == -1) { longs.push_back(2); }
            else if(diff == -wid) { longs.push_back(3); }
            destination = taken_path[destination];
            len++;
        }
        if(len > max) {
            max = len;
            longestPath = longs;
        }
    }

    std::reverse(longestPath.begin(), longestPath.end());
    return longestPath;
}

cs225::PNG* SquareMaze::drawMaze() const {
    cs225::PNG* maze = new cs225::PNG(wid*10+1, hei*10+1);
    for(int i = 0; i < hei*10+1; i++) {
        cs225::HSLAPixel& currPix = maze->getPixel(0,i);
        currPix.h = 0;
        currPix.s = 0;
        currPix.l = 0; 
    }
    for(int i = 0; i < wid*10+1; i++) { 
        if(i != 0 && i < 10) { continue; }
        cs225::HSLAPixel& currPix = maze->getPixel(i,0);
        currPix.h = 0;
        currPix.s = 0;
        currPix.l = 0; 
    }

    for(int i = 0; i < wid; i++) {
        for(int j = 0; j < hei; j++) {
            if(right_walls[i][j]) {
                for(int k = 0; k < 11; k++) {
                    cs225::HSLAPixel& currPix = maze->getPixel((i+1)*10, j*10+k);
                    currPix.h = 0;
                    currPix.s = 0;
                    currPix.l = 0;
                }
            }
            if(down_walls[i][j]) {
                for(int k = 0; k < 11; k++) {
                    cs225::HSLAPixel& currPix = maze->getPixel((i*10+k), (j+1)*10);
                    currPix.h = 0;
                    currPix.s = 0;
                    currPix.l = 0;
                }
            }
        }
    }
    return maze;
}

cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* maze = drawMaze();
    std::vector<int> solved_path = solveMaze();
    int x=5, y=5;

    for(size_t i = 0; i < solved_path.size(); i++) {
        if(!solved_path[i]) {
            for(int j = 0; j < 10; j++) {
                cs225::HSLAPixel& currPix = maze->getPixel(x,y);
                currPix.h = 0;
                currPix.s = 1;
                currPix.l = 0.5;
                x++;
            }
        }
        if(solved_path[i] == 1) {
            for(int j = 0; j < 10; j++) {
                cs225::HSLAPixel& currPix = maze->getPixel(x,y);
                currPix.h = 0;
                currPix.s = 1;
                currPix.l = 0.5;
                y++;
            }
        }
        if(solved_path[i] == 2) {
            for(int j = 0; j < 10; j++) {
                cs225::HSLAPixel& currPix = maze->getPixel(x,y);
                currPix.h = 0;
                currPix.s = 1;
                currPix.l = 0.5;
                x--;
            }
        }
        if(solved_path[i] == 3) {
            for(int j = 0; j < 10; j++) {
                cs225::HSLAPixel& currPix = maze->getPixel(x,y);
                currPix.h = 0;
                currPix.s = 1;
                currPix.l = 0.5;
                y--;
            }
        }
    }

    cs225::HSLAPixel& currPix = maze->getPixel(x,y);
    currPix.h = 0;
    currPix.s = 1;
    currPix.l = 0.5;
    currPix.a = 1;
    x -= 4;
    y += 5;
    for(int i = 1; i <= 9; i++) {
        cs225::HSLAPixel& currPix = maze->getPixel(x,y);
        currPix.h = 0;
        currPix.s = 0;  
        currPix.l = 1;
        currPix.a = 1;
        x++;
    }
    return maze;
}