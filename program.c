#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

char SPACE = ' ';
char WALL = '#';
char BARRICADE = '|';

char C_UP = '^';
char C_DOWN = 'v';
char C_RIGHT = '>';
char C_LEFT = '<';

char BEEPER_CHAR = '*';

char FILE_NAME[] = "score.txt";


void make_frame(char map[LINES][COLS]);
void make_map(char map[LINES][COLS]);
void make_door(char map[LINES][COLS], const int position);
void print_world(char map[LINES][COLS], const int position_for_door);

int select_level();
char* enter_name(char name[15]);
void make_level_map(int level_number, char map[LINES][COLS]);
void print_score(char user_input[], char map[LINES][COLS], int score);

void first_level_map(char map[LINES][COLS]);
void first_level_map_add_beepers(char map[LINES][COLS]);
void second_level_map(char map[LINES][COLS]);
void second_level_map_add_beepers(char map[LINES][COLS]);
void third_level_map(char map[LINES][COLS]);
void third_level_map_add_beepers(char map[LINES][COLS]);

int is_nickname_taken(char *name);
void change_score_in_file(char *line, int line_number);
int number_of_line(char *world);


int main(int argc, char *argv[]) {
    initscr();
    keypad(stdscr, 1); // allow arrows
    noecho(); // don`t display input
    curs_set(0); // hide cursor

    // initialize colors
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // frame
    init_pair(2, COLOR_WHITE, COLOR_BLACK); // character
    init_pair(3, COLOR_RED, COLOR_BLACK); // Beepers
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK); // menu

    // create map char[][]
    char map[LINES][COLS];

    // print menu to select level
    int level_difficulty;
    if(argc == 1) level_difficulty = select_level();
    else {
        level_difficulty = ((argv[1][0]-'0')-1);
        if((argv[1][0]-'0')!=3 && (argv[1][0]-'0')!=2 && (argv[1][0]-'0')!=1){
            level_difficulty = select_level();
        }
    }

    // user input name
    char name[15] = "";
    if(argc == 1 || argc == 2) enter_name(name);
    else{
        if (strlen(argv[2]) > 12 || strlen(argv[2]) < 3){
            enter_name(name);
        } else strcat(name, argv[2]);
    }

    // create world
    print_world(map, level_difficulty);

    // print score
    int score = 0;
    print_score(name, map, score);

    // make character
    int xindx;
    int yindx;
    if(LINES>=24 && COLS>=80){
        if(level_difficulty == 0){
            xindx = 1;
            yindx = 22;
        } else if(level_difficulty == 1){
            xindx = 9;
            yindx = 8;
        } else if(level_difficulty == 2){
            xindx = 47;
            yindx = 5;
        }
    } else{
        xindx = COLS/2;
        yindx = LINES/2;
    }

    int input = 1;
    char c = C_UP;

    do{
        srand(time(NULL));

        // make the character move
        if (input == KEY_UP){
            if(yindx == 0){
                ;
            }
            else if(map[yindx-1][xindx] == SPACE){
                yindx--;
                map[yindx+1][xindx] = SPACE;
                mvaddch(yindx+1, xindx, SPACE);
            } else if(map[yindx-1][xindx] == BEEPER_CHAR){
                yindx--;
                map[yindx+1][xindx] = SPACE;
                mvaddch(yindx+1, xindx, SPACE);
                if(level_difficulty == 0) score = score+rand()%500;
                else if(level_difficulty == 1) score = score+rand()%100;
                else if(level_difficulty == 2) score++;
                print_score(name, map, score);
            }
            c = C_UP;
        }

        else if (input == KEY_DOWN){
            if(yindx == LINES-1){
                if(level_difficulty == 1 || level_difficulty == 2){
                    clear();
                    break;
                }
            }
            else if(map[yindx+1][xindx] == SPACE){
                yindx++;
                map[yindx-1][xindx] = SPACE;
                mvaddch(yindx-1, xindx, SPACE);
            } else if(map[yindx+1][xindx] == BEEPER_CHAR){
                yindx++;
                map[yindx-1][xindx] = SPACE;
                mvaddch(yindx-1, xindx, SPACE);
                if(level_difficulty == 0) score = score+rand()%500;
                else if(level_difficulty == 1) score = score+rand()%100;
                else if(level_difficulty == 2) score++;
                print_score(name, map, score);
            }
            c = C_DOWN;
        }

        else if (input == KEY_LEFT){
            if (xindx == 0){
                ;
            }
            else if (map[yindx][xindx-1] == SPACE){
                xindx--;
                map[yindx][xindx+1] = SPACE;
                mvaddch(yindx, xindx+1, SPACE);
            } else if (map[yindx][xindx-1] == BEEPER_CHAR){
                xindx--;
                map[yindx][xindx+1] = SPACE;
                mvaddch(yindx, xindx+1, SPACE);
                if(level_difficulty == 0) score = score+rand()%500;
                else if(level_difficulty == 1) score = score+rand()%100;
                else if(level_difficulty == 2) score++;
                print_score(name, map, score);
            }
            c = C_LEFT;
        }

        else if (input == KEY_RIGHT){
            if(xindx == COLS-1){
                if(level_difficulty == 0){
                    clear();
                    break;
                }
            }
            else if(map[yindx][xindx+1] == SPACE){
                xindx++;
                map[yindx][xindx-1] = SPACE;
                mvaddch(yindx, xindx-1, SPACE);
            } else if(map[yindx][xindx+1] == BEEPER_CHAR){
                xindx++;
                map[yindx][xindx-1] = SPACE;
                mvaddch(yindx, xindx-1, SPACE);
                if(level_difficulty == 0) score = score+rand()%500;
                else if(level_difficulty == 1) score = score+rand()%100;
                else if(level_difficulty == 2) score++;
                print_score(name, map, score);
            }
            c = C_RIGHT;
        }

        refresh();
        attron(COLOR_PAIR(2));
        attron(A_BOLD);
        mvaddch(yindx, xindx, c);
        attroff(A_BOLD);

    } while ((input = getch()) != 27); // 27 - ESC

    // This message is displayed only when the user has won
    if(input != 27) {
        input = 1;
        char score_[5] = "";
        attron(COLOR_PAIR(4));
        do {
            mvprintw(LINES / 2, COLS / 2 - strlen("YOU WIN") / 2, "YOU WIN");
        } while ((input = getch()) != 10); // ENTER

        input = 1;
        do {
            mvprintw(LINES / 2 + 1, COLS / 2 - strlen("YOUR SCORE IS: 0000") / 2, "YOUR SCORE IS: ");
            if (score >= 1000 && score <= 9999) {
                printw("%d", score);
                sprintf(score_,"%d",score);
            } else if (score < 1000 && score >= 100) {
                printw("0%d", score);
                sprintf(score_,"0%d",score);
            } else if (score < 100 && score >= 10) {
                printw("00%d", score);
                sprintf(score_,"00%d",score);
            } else if (score < 10 && score >= 0) {
                printw("000%d", score);
                sprintf(score_,"000%d",score);
            }
        } while ((input = getch()) != 10); // ENTER

        // make file
        FILE *file = fopen(FILE_NAME, "a");
        if (is_nickname_taken(name)){
            char new_line[60] = "";
            strcat(new_line, name);
            strcat(new_line, " ");
            strcat(new_line, score_);
            strcat(new_line, "\n");
            change_score_in_file(new_line, number_of_line(name));
        } else fprintf(file, "%s %s\n", name, score_);
        fclose(file);
    }

    endwin();

    if(LINES>=24 && COLS>=80) printf("Thank you for play :)\n");
    else printf("The terminal should be 80x24\n");

    return 0;
}


void make_frame(char map[LINES][COLS]){
    int y_ = LINES-1;
    int x_ = COLS-1;
    attron(COLOR_PAIR(1));

    for (int yy = 0; yy<=y_; yy++){
        mvaddch(yy, 0, WALL);
        map[yy][0] = WALL;
    }
    for (int xx = 0; xx<=x_; xx++){
        mvaddch(0, xx, WALL);
        map[0][xx] = WALL;
    }
    for (int xx = 0; xx<=x_; xx++){
        mvaddch(y_, xx, WALL);
        map[y_][xx] = WALL;
    }
    for (int yy = 0; yy<=y_; yy++){
        mvaddch(yy, x_, WALL);
        map[yy][x_] = WALL;
    }
}

void make_map(char map[LINES][COLS]){
    for (int yy=0; yy<LINES; yy++){
        for(int xx=0; xx<COLS; xx++){
            mvaddch(yy, xx, SPACE);
            map[yy][xx] = SPACE;
        }
    }
}

void print_world(char map[LINES][COLS], const int position_for_door){
    clear();
    make_map(map);
    make_frame(map);
    if(LINES>=24 && COLS>=80) {
        make_level_map(position_for_door, map);
        make_door(map, position_for_door);
    }
    else mvprintw(LINES-2, 1, "The terminal should be 80x24, press ESC");
}

void make_door(char map[LINES][COLS], const int position){
    // position is an argument that shows in which wall you need the door

    if(position == 0){
        // right wall
        for (int i = (LINES/2)-2; i<=(LINES/2)+2; i++){
            mvaddch(i, COLS-1, SPACE);
            map[i][COLS-1] = SPACE;
        }
    } else if(position == 1){
        // bottom wall
        for (int i = (COLS/2)-3; i<=(COLS/2)+3; i++){
            mvaddch(LINES-1, i, SPACE);
            map[LINES-1][i] = SPACE;
        }
    } else if(position == 2){
        mvaddch(LINES-1, 5, SPACE);
        map[LINES-1][5] = SPACE;
    }
}


int select_level(){

    attron(COLOR_PAIR(4));
    mvprintw(LINES/5, (COLS/2- strlen("Hello, this is the maze game. To start select level and press \"enter\".")/2), "Hello, this is the maze game. To start select level and press \"enter\".");

    char choices[3][15] = {"Easy level", "Normal level", "Hard level"};
    int choice;
    int hightlight = 0;

    WINDOW * menuwin = newwin(LINES/2, COLS/4, LINES/4, COLS/2-COLS/8);
    box(menuwin, 0, 0);


    wbkgd(menuwin, COLOR_PAIR(4));

    refresh();
    wrefresh(menuwin);

    keypad(menuwin, 1);


    while (1){
        for(int i = 0; i<3; i++) {
            if (i == hightlight) wattron(menuwin, A_REVERSE);

            if (i == 0)
                mvwprintw(menuwin, (LINES / 2 ) / 2-2, COLS / 8 - strlen(choices[i]) / 2, choices[i]);
            else if (i == 1)
                mvwprintw(menuwin, (LINES / 2 ) / 2, COLS / 8 - strlen(choices[i]) / 2, choices[i]);
            else if (i == 2)
                mvwprintw(menuwin, (LINES / 2 ) / 2+2, COLS / 8 - strlen(choices[i]) / 2, choices[i]);

            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);

        switch (choice) {
            case KEY_UP:
                hightlight--;
                if(hightlight == -1) hightlight = 0;
                break;
            case KEY_DOWN:
                hightlight++;
                if(hightlight == 3) hightlight = 2;
                break;
            default:
                break;
        }
        if(choice == 10){
            break;
        }
    }

    clear();
    return hightlight;
}

char* enter_name(char name[15]){
    name[0] = '\0';
    int c = 1;
    int x = COLS/2;
    int len = 0;

    attron(COLOR_PAIR(4));
    mvprintw(LINES/2-1, x-(strlen("Enter your name: ")), "Enter your name: ");
    mvprintw(LINES/2+1, x-(strlen("Enter your name: ")), "The name must be");
    mvprintw(LINES/2+2, x-(strlen("Enter your name: ")), "greater than or equal to 3");
    mvprintw(LINES/2+3, x-(strlen("Enter your name: ")), "and less than or equal to 12");

    while ((c = getch()) != 10){
        if(c != 127){ // delete
            mvprintw(LINES/2-1, x, "%c", c);
            refresh();
            char c1 = c;
            char c_[] = {c1, '\0'};
            strcat(name, c_);
            x++;
            len++;
        } else{
            if(x-1 != COLS/2-1){
                name[len-1] = '\0';
                mvprintw(LINES/2-1, x-1, " ");
                refresh();
                x--;
                len--;
            }
        }

    }

    if (strlen(name) > 12 || strlen(name) < 3){
        for(int i = 0; i<len; i++){
            mvprintw(LINES/2-1, (COLS/2)+i, " ");
        }
        mvprintw(LINES/2+5, (COLS/2)-(strlen("Enter your name: ")), "Please follow the rules above");
        getch();
        move(LINES/2+5, (COLS/2)-(strlen("Enter your name: ")));
        clrtoeol();
        name[0] = '\0';
        enter_name(name);
    }

    return name;
}

void make_level_map(int level_number, char map[LINES][COLS]){
    clear();
    make_map(map);
    make_frame(map);

    if(level_number == 0){
        make_door(map, level_number);
        first_level_map(map);
    } else if (level_number == 1){
        make_door(map, level_number);
        second_level_map(map);
    } else if (level_number == 2){
        make_door(map, level_number);
        third_level_map(map);
    }
}

void print_score(char user_input[], char map[LINES][COLS], int score){
    int len_of_name = strlen(user_input)+2;
    WINDOW * win = newwin(3, len_of_name+14, 1, 2);
    box(win, 0, 0);

    wbkgd(win, COLOR_PAIR(3));
    mvwprintw(win, 1, 1, user_input);
    wprintw(win, " - score: ");
    if(score>=1000 && score<=9999){
        wprintw(win, "%d", score);
    } else if (score<1000 && score>=100){
        wprintw(win, "0%d", score);
    } else if (score<100 && score>=10){
        wprintw(win, "00%d", score);
    } else if (score<10 && score>=0){
        wprintw(win, "000%d", score);
    }
    refresh();
    wrefresh(win);

    for (int i = 0; i<=len_of_name+14+1; i++){
        map[3][i] = WALL;
    }

    for (int i = 0; i<=3; i++){
        map[i][len_of_name+14+1] = WALL;
    }
}


void first_level_map(char map[LINES][COLS]){
    first_level_map_add_beepers(map);
    attron(COLOR_PAIR(1));

    if(LINES>24 && COLS>80) {
        for (int i = 1; i <= 78; i++) {
            map[23][i] = BARRICADE;
            mvaddch(23, i, BARRICADE);
        }
        for (int i = 1; i <= 23; i++) {
            map[i][79] = BARRICADE;
            mvaddch(i, 79, BARRICADE);
        }
        for (int i = 10; i <= 14; i++) {
            map[i][79] = SPACE;
            mvaddch(i, 79, SPACE);
        }
    }
    // Only for 80x24

    map[15][1] = BARRICADE;
    mvaddch(15, 1, BARRICADE);
    map[11][1] = BARRICADE;
    mvaddch(11, 1, BARRICADE);
    map[7][1] = BARRICADE;
    mvaddch(7, 1, BARRICADE);
    map[5][1] = BARRICADE;
    mvaddch(5, 1, BARRICADE);


    map[20][2] = BARRICADE;
    mvaddch(20, 2, BARRICADE);
    map[18][2] = BARRICADE;
    mvaddch(18, 2, BARRICADE);
    map[17][2] = BARRICADE;
    mvaddch(17, 2, BARRICADE);
    map[13][2] = BARRICADE;
    mvaddch(13, 2, BARRICADE);
    map[9][2] = BARRICADE;
    mvaddch(9, 2, BARRICADE);
    map[5][2] = BARRICADE;
    mvaddch(5, 2, BARRICADE);


    map[21][3] = BARRICADE;
    mvaddch(21, 3, BARRICADE);
    map[20][3] = BARRICADE;
    mvaddch(20, 3, BARRICADE);
    map[18][3] = BARRICADE;
    mvaddch(18, 3, BARRICADE);
    map[17][3] = BARRICADE;
    mvaddch(17, 3, BARRICADE);
    for (int i = 15; i>=5; i--){
        map[i][3] = BARRICADE;
        mvaddch(i, 3, BARRICADE);
    }


    map[5][4] = BARRICADE;
    mvaddch(5, 4, BARRICADE);


    for (int i = 22; i>=20; i--){
        map[i][5] = BARRICADE;
        mvaddch(i, 5, BARRICADE);
    }
    map[18][5] = BARRICADE;
    mvaddch(18, 5, BARRICADE);
    map[17][5] = BARRICADE;
    mvaddch(17, 5, BARRICADE);
    for (int i = 15; i>=7; i--){
        map[i][5] = BARRICADE;
        mvaddch(i, 5, BARRICADE);
    }
    map[5][5] = BARRICADE;
    mvaddch(5, 5, BARRICADE);


    map[18][6] = BARRICADE;
    mvaddch(18, 6, BARRICADE);
    map[17][6] = BARRICADE;
    mvaddch(17, 6, BARRICADE);
    map[15][6] = BARRICADE;
    mvaddch(15, 6, BARRICADE);
    map[7][6] = BARRICADE;
    mvaddch(7, 6, BARRICADE);
    map[5][6] = BARRICADE;
    mvaddch(5, 6, BARRICADE);

    for (int i = 21; i>=17; i--){
        map[i][7] = BARRICADE;
        mvaddch(i, 7, BARRICADE);
    }
    map[15][7] = BARRICADE;
    mvaddch(15, 7, BARRICADE);
    map[7][7] = BARRICADE;
    mvaddch(7, 7, BARRICADE);
    map[5][7] = BARRICADE;
    mvaddch(5, 7, BARRICADE);


    for (int i = 21; i>=17; i--){
        map[i][8] = BARRICADE;
        mvaddch(i, 8, BARRICADE);
    }
    for (int i = 15; i>=13; i--){
        map[i][8] = BARRICADE;
        mvaddch(i, 8, BARRICADE);
    }
    for (int i = 11; i>=7; i--){
        map[i][8] = BARRICADE;
        mvaddch(i, 8, BARRICADE);
    }
    map[5][8] = BARRICADE;
    mvaddch(5, 8, BARRICADE);


    map[7][9] = BARRICADE;
    mvaddch(7, 9, BARRICADE);
    map[5][9] = BARRICADE;
    mvaddch(5, 9, BARRICADE);


    for(int i = 22; i>=17; i--){
        map[i][10] = BARRICADE;
        mvaddch(i, 10, BARRICADE);
    }
    for(int i = 15; i>=10; i--){
        map[i][10] = BARRICADE;
        mvaddch(i, 10, BARRICADE);
    }
    map[7][10] = BARRICADE;
    mvaddch(7, 10, BARRICADE);
    map[5][10] = BARRICADE;
    mvaddch(5, 10, BARRICADE);


    map[17][11] = BARRICADE;
    mvaddch(17, 11, BARRICADE);
    map[15][11] = BARRICADE;
    mvaddch(15, 11, BARRICADE);
    map[14][11] = BARRICADE;
    mvaddch(14, 11, BARRICADE);
    map[7][11] = BARRICADE;
    mvaddch(7, 11, BARRICADE);
    map[5][11] = BARRICADE;
    mvaddch(5, 11, BARRICADE);


    map[21][12] = BARRICADE;
    mvaddch(21, 12, BARRICADE);
    for(int i = 19; i>=17; i--){
        map[i][12] = BARRICADE;
        mvaddch(i, 12, BARRICADE);
    }
    map[15][12] = BARRICADE;
    mvaddch(15, 12, BARRICADE);
    map[14][12] = BARRICADE;
    mvaddch(14, 12, BARRICADE);
    for(int i = 12; i>=7; i--){
        map[i][12] = BARRICADE;
        mvaddch(i, 12, BARRICADE);
    }
    map[5][12] = BARRICADE;
    mvaddch(5, 12, BARRICADE);


    map[21][13] = BARRICADE;
    mvaddch(21, 13, BARRICADE);
    map[19][13] = BARRICADE;
    mvaddch(19, 13, BARRICADE);
    map[15][13] = BARRICADE;
    mvaddch(15, 13, BARRICADE);
    map[14][13] = BARRICADE;
    mvaddch(14, 13, BARRICADE);
    map[12][13] = BARRICADE;
    mvaddch(12, 13, BARRICADE);
    map[7][13] = BARRICADE;
    mvaddch(7, 13, BARRICADE);
    map[5][13] = BARRICADE;
    mvaddch(5, 13, BARRICADE);

    map[21][14] = BARRICADE;
    mvaddch(21, 14, BARRICADE);
    map[19][14] = BARRICADE;
    mvaddch(19, 14, BARRICADE);
    map[17][14] = BARRICADE;
    mvaddch(17, 14, BARRICADE);
    map[15][14] = BARRICADE;
    mvaddch(15, 14, BARRICADE);
    map[14][14] = BARRICADE;
    mvaddch(14, 14, BARRICADE);
    map[12][14] = BARRICADE;
    mvaddch(12, 14, BARRICADE);
    map[5][14] = BARRICADE;
    mvaddch(5, 14, BARRICADE);

    map[21][15] = BARRICADE;
    mvaddch(21, 15, BARRICADE);
    map[17][15] = BARRICADE;
    mvaddch(17, 15, BARRICADE);
    map[15][15] = BARRICADE;
    mvaddch(15, 15, BARRICADE);
    map[14][15] = BARRICADE;
    mvaddch(14, 15, BARRICADE);
    for(int i = 12; i>=7; i--){
        map[i][15] = BARRICADE;
        mvaddch(i, 15, BARRICADE);
    }
    map[5][15] = BARRICADE;
    mvaddch(5, 15, BARRICADE);




    for(int i = 16; i<=39; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }
    for (int i = 21; i>=17; i--){
        map[i][16] = BARRICADE;
        mvaddch(i, 16, BARRICADE);
    }
    for (int i = 16; i<=36; i++){
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for (int i = 16; i<=19; i++){
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    for(int i = 18; i<=39; i++){
        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
        map[18][i] = BARRICADE;
        mvaddch(18, i, BARRICADE);
    }
    map[18][24] = SPACE;
    mvaddch(18, 24, SPACE);
    map[18][38] = SPACE;
    mvaddch(18, 38, SPACE);
    map[19][18] = BARRICADE;
    mvaddch(19, 18, BARRICADE);
    map[19][39] = BARRICADE;
    mvaddch(19, 39, BARRICADE);

    map[16][17] = BARRICADE;
    mvaddch(16, 17, BARRICADE);
    map[15][17] = BARRICADE;
    mvaddch(15, 17, BARRICADE);
    map[14][17] = BARRICADE;
    mvaddch(14, 17, BARRICADE);
    map[12][17] = BARRICADE;
    mvaddch(12, 17, BARRICADE);
    map[11][17] = BARRICADE;
    mvaddch(11, 17, BARRICADE);
    map[10][17] = BARRICADE;
    mvaddch(10, 17, BARRICADE);

    for (int i = 18; i<=25; i++){
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }
    map[16][18] = SPACE;
    mvaddch(16, 18, SPACE);
    map[16][23] = SPACE;
    mvaddch(16, 23, SPACE);

    for(int i = 18; i<=22; i++){
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }
    map[12][18] = BARRICADE;
    mvaddch(12, 18, BARRICADE);
    map[12][19] = BARRICADE;
    mvaddch(12, 19, BARRICADE);
    map[12][20] = BARRICADE;
    mvaddch(12, 20, BARRICADE);

    map[10][18] = BARRICADE;
    mvaddch(10, 18, BARRICADE);
    map[10][19] = BARRICADE;
    mvaddch(10, 19, BARRICADE);
    map[10][20] = BARRICADE;
    mvaddch(10, 20, BARRICADE);

    map[15][22] = BARRICADE;
    mvaddch(15, 22, BARRICADE);
    map[13][22] = BARRICADE;
    mvaddch(13, 22, BARRICADE);
    map[12][22] = BARRICADE;
    mvaddch(12, 22, BARRICADE);
    map[11][22] = BARRICADE;
    mvaddch(11, 22, BARRICADE);

    map[9][21] = BARRICADE;
    mvaddch(9, 21, BARRICADE);
    map[9][22] = BARRICADE;
    mvaddch(9, 22, BARRICADE);
    map[9][23] = BARRICADE;
    mvaddch(9, 23, BARRICADE);

    map[8][21] = BARRICADE;
    mvaddch(8, 21, BARRICADE);

    for (int i = 21; i<=34; i++){
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    map[7][26] = SPACE;
    mvaddch(7, 26, SPACE);
    map[7][33] = SPACE;
    mvaddch(7, 33, SPACE);

    for (int i = 8; i<=15; i++){
        map[i][25] = BARRICADE;
        mvaddch(i, 25, BARRICADE);
        map[i][27] = BARRICADE;
        mvaddch(i, 27, BARRICADE);
    }
    for (int i = 12; i<=15; i++){
        map[i][24] = BARRICADE;
        mvaddch(i, 24, BARRICADE);
    }
    for (int i = 25; i<=32; i++){
        map[17][i] = BARRICADE;
        mvaddch(17, i, BARRICADE);
    }
    for (int i = 28; i<=32; i++){
        map[15][i] = BARRICADE;
        mvaddch(15, i, BARRICADE);
    }
    map[15][31] = SPACE;
    mvaddch(15, 31, SPACE);
    map[16][32] = BARRICADE;
    mvaddch(16, 32, BARRICADE);
    map[14][30] = BARRICADE;
    mvaddch(14, 30, BARRICADE);

    for (int i = 9; i<=13; i++){
        map[i][29] = BARRICADE;
        mvaddch(i, 29, BARRICADE);
    }
    map[9][30] = BARRICADE;
    mvaddch(9, 30, BARRICADE);
    map[9][31] = BARRICADE;
    mvaddch(9, 31, BARRICADE);
    map[9][32] = BARRICADE;
    mvaddch(9, 32, BARRICADE);

    map[11][31] = BARRICADE;
    mvaddch(11, 31, BARRICADE);
    map[11][32] = BARRICADE;
    mvaddch(11, 32, BARRICADE);
    map[12][31] = BARRICADE;
    mvaddch(12, 31, BARRICADE);
    map[12][32] = BARRICADE;
    mvaddch(12, 32, BARRICADE);
    map[13][32] = BARRICADE;
    mvaddch(13, 32, BARRICADE);
    map[14][32] = BARRICADE;
    mvaddch(14, 32, BARRICADE);

    for(int i = 33; i<=39; i++){
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }
    for (int c = 8; c<=12; c++){
        if (c%2==0){
            for (int i = 34; i<=39; i++){
                map[c][i] = BARRICADE;
                mvaddch(c, i, BARRICADE);
            }
        }
    }
    map[17][39] = BARRICADE;
    mvaddch(17, 39, BARRICADE);
    map[16][39] = BARRICADE;
    mvaddch(16, 39, BARRICADE);
    map[15][39] = BARRICADE;
    mvaddch(15, 39, BARRICADE);
    map[17][37] = BARRICADE;
    mvaddch(17, 37, BARRICADE);
    map[16][37] = BARRICADE;
    mvaddch(16, 37, BARRICADE);
    map[16][36] = BARRICADE;
    mvaddch(16, 36, BARRICADE);
    map[16][35] = BARRICADE;
    mvaddch(16, 35, BARRICADE);
    map[16][34] = BARRICADE;
    mvaddch(16, 34, BARRICADE);

    map[11][39] = BARRICADE;
    mvaddch(11, 39, BARRICADE);

    map[6][36] = BARRICADE;
    mvaddch(6, 36, BARRICADE);
    map[6][37] = BARRICADE;
    mvaddch(6, 37, BARRICADE);

    for(int i = 2; i<=7; i++){
        map[i][39] = BARRICADE;
        mvaddch(i, 39, BARRICADE);
    }
    for(int i = 1; i<=4; i++){
        map[i][31] = BARRICADE;
        mvaddch(i, 31, BARRICADE);
    }

    for(int i = 32; i<=37; i++){
        map[1][i] = BARRICADE;
        mvaddch(1, i, BARRICADE);
    }
    for(int i = 33; i<=38; i++){
        map[3][i] = BARRICADE;
        mvaddch(3, i, BARRICADE);
    }
    map[4][38] = BARRICADE;
    mvaddch(4, 38, BARRICADE);

    for(int i = 40; i<=54; i++){
        map[2][i] = BARRICADE;
        mvaddch(2, i, BARRICADE);
    }
    map[2][44] = SPACE;
    mvaddch(2, 44, SPACE);
    map[1][43] = BARRICADE;
    mvaddch(1, 43, BARRICADE);

    for(int i = 4; i<=21; i++){
        map[i][41] = BARRICADE;
        mvaddch(i, 41, BARRICADE);
    }
    map[9][41] = SPACE;
    mvaddch(9, 41, SPACE);
    map[13][41] = SPACE;
    mvaddch(13, 41, SPACE);

    for(int i = 56; i<=77; i++){
        map[1][i] = BARRICADE;
        mvaddch(1, i, BARRICADE);
    }
    for(int i = 2; i<=9; i++){
        map[i][77] = BARRICADE;
        mvaddch(i, 77, BARRICADE);
    }
    for(int i = 2; i<9; i++){
        map[i][56] = BARRICADE;
        mvaddch(i, 56, BARRICADE);
    }
    for(int i = 3; i<9; i++){
        map[i][58] = BARRICADE;
        mvaddch(i, 58, BARRICADE);
    }
    for(int i = 17; i<=22; i++){
        map[i][58] = BARRICADE;
        mvaddch(i, 58, BARRICADE);
    }
    map[14][58] = BARRICADE;
    mvaddch(14, 58, BARRICADE);
    map[15][58] = BARRICADE;
    mvaddch(15, 58, BARRICADE);
    map[12][58] = BARRICADE;
    mvaddch(12, 58, BARRICADE);
    map[11][58] = BARRICADE;
    mvaddch(11, 58, BARRICADE);
    map[10][58] = BARRICADE;
    mvaddch(10, 58, BARRICADE);

    for(int i = 17; i<=21; i++){
        map[i][60] = BARRICADE;
        mvaddch(i, 60, BARRICADE);
    }
    map[17][59] = BARRICADE;
    mvaddch(17, 59, BARRICADE);
    for(int i = 17; i<=22; i++){
        map[i][62] = BARRICADE;
        mvaddch(i, 62, BARRICADE);
    }
    map[14][62] = BARRICADE;
    mvaddch(14, 62, BARRICADE);
    map[15][62] = BARRICADE;
    mvaddch(15, 62, BARRICADE);
    map[12][62] = BARRICADE;
    mvaddch(12, 62, BARRICADE);
    map[11][62] = BARRICADE;
    mvaddch(11, 62, BARRICADE);
    map[10][62] = BARRICADE;
    mvaddch(10, 62, BARRICADE);
    map[8][62] = BARRICADE;
    mvaddch(8, 62, BARRICADE);

    map[6][62] = BARRICADE;
    mvaddch(6, 62, BARRICADE);
    map[4][62] = BARRICADE;
    mvaddch(4, 62, BARRICADE);
    map[5][62] = BARRICADE;
    mvaddch(5, 62, BARRICADE);
    map[3][62] = BARRICADE;
    mvaddch(3, 62, BARRICADE);

    map[15][61] = BARRICADE;
    mvaddch(15, 61, BARRICADE);
    map[15][60] = BARRICADE;
    mvaddch(15, 60, BARRICADE);
    map[15][59] = BARRICADE;
    mvaddch(15, 59, BARRICADE);
    map[12][61] = BARRICADE;
    mvaddch(12, 61, BARRICADE);
    map[12][59] = BARRICADE;
    mvaddch(12, 59, BARRICADE);
    map[13][60] = BARRICADE;
    mvaddch(13, 60, BARRICADE);

    map[10][59] = BARRICADE;
    mvaddch(10, 59, BARRICADE);
    map[10][60] = BARRICADE;
    mvaddch(10, 60, BARRICADE);
    map[3][59] = BARRICADE;
    mvaddch(3, 59, BARRICADE);
    map[3][60] = BARRICADE;
    mvaddch(3, 60, BARRICADE);
    map[4][60] = BARRICADE;
    mvaddch(4, 60, BARRICADE);

    map[6][60] = BARRICADE;
    mvaddch(6, 60, BARRICADE);
    map[6][61] = BARRICADE;
    mvaddch(6, 61, BARRICADE);

    map[8][61] = BARRICADE;
    mvaddch(8, 61, BARRICADE);
    map[8][59] = BARRICADE;
    mvaddch(8, 59, BARRICADE);
    map[8][60] = BARRICADE;
    mvaddch(8, 60, BARRICADE);

    for(int c = 20; c<=21; c++){
        for(int i = 64; i<=75; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    for(int i = 15; i<=20; i++){
        map[i][78] = BARRICADE;
        mvaddch(i, 78, BARRICADE);
    }

    map[20][77] = BARRICADE;
    mvaddch(20, 77, BARRICADE);
    map[21][77] = BARRICADE;
    mvaddch(21, 77, BARRICADE);

    for(int i = 15; i<=18; i++){
        map[i][76] = BARRICADE;
        mvaddch(i, 76, BARRICADE);
    }

    map[17][75] = BARRICADE;
    mvaddch(17, 75, BARRICADE);
    map[18][75] = BARRICADE;
    mvaddch(18, 75, BARRICADE);

    map[15][74] = BARRICADE;
    mvaddch(15, 74, BARRICADE);
    map[16][74] = BARRICADE;
    mvaddch(16, 74, BARRICADE);
    for(int i = 16; i<=18; i++){
        map[i][73] = BARRICADE;
        mvaddch(i, 73, BARRICADE);
    }
    for(int i = 14; i<=18; i++){
        map[i][70] = BARRICADE;
        mvaddch(i, 70, BARRICADE);
    }
    for(int c = 67; c<=68; c++){
        for(int i = 14; i<=18; i++){
            map[i][c] = BARRICADE;
            mvaddch(i, c, BARRICADE);
        }
    }

    for(int c = 17; c<=18; c++){
        for(int i = 64; i<=66; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    for(int c = 14; c<=15; c++){
        for(int i = 64; i<=65; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    for(int c = 8; c<=12; c++){
        for(int i = 64; i<=65; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    for(int c = 3; c<=7; c++){
        map[c][65] = BARRICADE;
        mvaddch(c, 65, BARRICADE);
    }
    map[3][64] = BARRICADE;
    mvaddch(3, 64, BARRICADE);
    map[3][63] = BARRICADE;
    mvaddch(3, 63, BARRICADE);

    map[8][63] = BARRICADE;
    mvaddch(8, 63, BARRICADE);

    map[18][71] = BARRICADE;
    mvaddch(18, 71, BARRICADE);
    map[18][72] = BARRICADE;
    mvaddch(18, 72, BARRICADE);

    map[16][71] = BARRICADE;
    mvaddch(16, 71, BARRICADE);
    map[14][71] = BARRICADE;
    mvaddch(14, 71, BARRICADE);

    for (int i = 66; i<=75; i++){
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
        map[3][i] = BARRICADE;
        mvaddch(3, i, BARRICADE);
    }
    map[3][66] = SPACE;
    mvaddch(3, 66, SPACE);

    for (int i = 3; i<=9; i++){
        map[i][75] = BARRICADE;
        mvaddch(i, 75, BARRICADE);
    }
    for (int i = 6; i<=9; i++){
        map[i][74] = BARRICADE;
        mvaddch(i, 74, BARRICADE);
    }

    for (int i = 7; i<=14; i++){
        map[i][72] = BARRICADE;
        mvaddch(i, 72, BARRICADE);
    }
    for (int i = 7; i<=12; i++){
        map[i][67] = BARRICADE;
        mvaddch(i, 67, BARRICADE);
    }

    map[13][73] = BARRICADE;
    mvaddch(13, 73, BARRICADE);

    for (int i = 68; i<=71; i++){
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    for (int i = 69; i<=71; i++){
        map[8][i] = BARRICADE;
        mvaddch(8, i, BARRICADE);
    }
    for (int i = 68; i<=70; i++){
        map[10][i] = BARRICADE;
        mvaddch(10, i, BARRICADE);
    }
    for (int i = 69; i<=71; i++){
        map[12][i] = BARRICADE;
        mvaddch(12, i, BARRICADE);
    }

    map[20][42] = BARRICADE;
    mvaddch(20, 42, BARRICADE);
    map[21][42] = BARRICADE;
    mvaddch(21, 42, BARRICADE);

    for (int i = 16; i<=21; i++){
        map[i][43] = BARRICADE;
        mvaddch(i, 43, BARRICADE);
    }

    for (int i = 4; i<=8; i++){
        map[i][43] = BARRICADE;
        mvaddch(i, 43, BARRICADE);
    }
    map[12][43] = BARRICADE;
    mvaddch(12, 43, BARRICADE);
    map[14][43] = BARRICADE;
    mvaddch(14, 43, BARRICADE);
    map[10][42] = BARRICADE;
    mvaddch(10, 42, BARRICADE);
    map[4][42] = BARRICADE;
    mvaddch(4, 42, BARRICADE);
    map[12][42] = BARRICADE;
    mvaddch(12, 42, BARRICADE);
    map[14][42] = BARRICADE;
    mvaddch(14, 42, BARRICADE);

    for (int i = 44; i<=48; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }

    for (int i = 44; i<=49; i++){
        map[17][i] = BARRICADE;
        mvaddch(17, i, BARRICADE);
    }
    for (int i = 45; i<=50; i++){
        map[19][i] = BARRICADE;
        mvaddch(19, i, BARRICADE);
    }

    for (int c = 14; c<=15; c++){
        for (int i = 46; i<=49; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    map[20][45] = BARRICADE;
    mvaddch(20, 45, BARRICADE);
    map[16][44] = BARRICADE;
    mvaddch(16, 44, BARRICADE);
    map[14][44] = BARRICADE;
    mvaddch(14, 44, BARRICADE);

    map[12][44] = BARRICADE;
    mvaddch(12, 44, BARRICADE);
    map[11][44] = BARRICADE;
    mvaddch(11, 44, BARRICADE);
    map[10][44] = BARRICADE;
    mvaddch(10, 44, BARRICADE);

    map[7][44] = BARRICADE;
    mvaddch(7, 44, BARRICADE);

    for(int i = 45; i<=51; i++){
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for(int i = 4; i<=10; i++){
        map[i][49] = BARRICADE;
        mvaddch(i, 49, BARRICADE);
    }
    for(int i = 45; i<=49; i++){
        map[10][i] = BARRICADE;
        mvaddch(10, i, BARRICADE);
    }
    for(int i = 46; i<=52; i++){
        map[12][i] = BARRICADE;
        mvaddch(12, i, BARRICADE);
    }

    map[20][45] = BARRICADE;
    mvaddch(20, 45, BARRICADE);
    map[9][45] = BARRICADE;
    mvaddch(9, 45, BARRICADE);
    map[7][45] = BARRICADE;
    mvaddch(7, 45, BARRICADE);
    map[7][46] = BARRICADE;
    mvaddch(7, 46, BARRICADE);
    map[7][47] = BARRICADE;
    mvaddch(7, 47, BARRICADE);
    map[8][47] = BARRICADE;
    mvaddch(8, 47, BARRICADE);
    map[7][50] = BARRICADE;
    mvaddch(7, 50, BARRICADE);
    for (int i = 7; i<=10; i++){
        map[i][51] = BARRICADE;
        mvaddch(i, 51, BARRICADE);
    }

    for (int c = 53; c<=54; c++){
        for (int i = 3; i<=11; i++){
            map[i][c] = BARRICADE;
            mvaddch(i, c, BARRICADE);
        }
    }
    map[11][54] = SPACE;
    mvaddch(11, 54, SPACE);

    map[20][50] = BARRICADE;
    mvaddch(20, 50, BARRICADE);

    for (int i = 50; i<=56; i++){
        map[21][i] = BARRICADE;
        mvaddch(21, i, BARRICADE);
    }

    for (int i = 17; i<=20; i++){
        map[i][52] = BARRICADE;
        mvaddch(i, 52, BARRICADE);
        map[i][56] = BARRICADE;
        mvaddch(i, 56, BARRICADE);
    }

    map[17][51] = BARRICADE;
    mvaddch(17, 51, BARRICADE);
    map[17][53] = BARRICADE;
    mvaddch(17, 53, BARRICADE);
    map[17][54] = BARRICADE;
    mvaddch(17, 54, BARRICADE);
    map[18][54] = BARRICADE;
    mvaddch(18, 54, BARRICADE);
    map[19][54] = BARRICADE;
    mvaddch(19, 54, BARRICADE);

    map[10][55] = BARRICADE;
    mvaddch(10, 55, BARRICADE);
    map[10][56] = BARRICADE;
    mvaddch(10, 56, BARRICADE);
    map[12][55] = BARRICADE;
    mvaddch(12, 55, BARRICADE);

    map[13][54] = BARRICADE;
    mvaddch(13, 54, BARRICADE);

    for (int c = 14; c<=15; c++){
        for (int i = 51; i<=54; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    for (int i = 13; i<=15; i++){
        map[i][56] = BARRICADE;
        mvaddch(i, 56, BARRICADE);
    }

    map[22][75] = BARRICADE;
    mvaddch(22, 75, BARRICADE);
}

void first_level_map_add_beepers(char map[LINES][COLS]){
    attron(COLOR_PAIR(3));

    map[16][16] = BEEPER_CHAR;
    mvaddch(16, 16, BEEPER_CHAR);

    map[11][26] = BEEPER_CHAR;
    mvaddch(11, 26, BEEPER_CHAR);

    map[21][31] = BEEPER_CHAR;
    mvaddch(21, 31, BEEPER_CHAR);

    map[9][40] = BEEPER_CHAR;
    mvaddch(9, 40, BEEPER_CHAR);

    map[15][42] = BEEPER_CHAR;
    mvaddch(15, 42, BEEPER_CHAR);

    map[3][44] = BEEPER_CHAR;
    mvaddch(3, 44, BEEPER_CHAR);

    map[8][50] = BEEPER_CHAR;
    mvaddch(8, 50, BEEPER_CHAR);

    map[16][57] = BEEPER_CHAR;
    mvaddch(16, 57, BEEPER_CHAR);

    map[11][68] = BEEPER_CHAR;
    mvaddch(11, 68, BEEPER_CHAR);

    map[8][76] = BEEPER_CHAR;
    mvaddch(8, 76, BEEPER_CHAR);
}

void second_level_map(char map[LINES][COLS]){
    second_level_map_add_beepers(map);
    attron(COLOR_PAIR(1));

    if(LINES>24 && COLS>80) {
        for (int i = 1; i <= 78; i++) {
            map[23][i] = BARRICADE;
            mvaddch(23, i, BARRICADE);
        }
        for (int i = 1; i <= 23; i++) {
            map[i][79] = BARRICADE;
            mvaddch(i, 79, BARRICADE);
        }

        for (int i = 37; i <= 43; i++) {
            map[23][i] = SPACE;
            mvaddch(23, i, SPACE);
        }
    }

    // Only for 80x24
    for (int i = 4; i<=22; i++){
        map[i][1] = BARRICADE;
        mvaddch(i, 1, BARRICADE);
    }
    for (int i = 1; i<=31; i++){
        map[4][i] = BARRICADE;
        mvaddch(4, i, BARRICADE);
    }
    for (int i = 1; i<=11; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }
    for (int i = 6; i<=20; i++){
        map[i][3] = BARRICADE;
        mvaddch(i, 3, BARRICADE);
    }
    for (int i = 6; i<=20; i++){
        map[i][5] = BARRICADE;
        mvaddch(i, 5, BARRICADE);
    }
    map[17][5] = SPACE;
    mvaddch(17, 5, SPACE);
    map[18][4] = BARRICADE;
    mvaddch(18, 4, BARRICADE);

    for (int i = 12; i<=21; i++){
        map[i][7] = BARRICADE;
        mvaddch(i, 7, BARRICADE);
    }
    map[19][7] = SPACE;
    mvaddch(19, 7, SPACE);
    map[18][6] = BARRICADE;
    mvaddch(18, 6, BARRICADE);

    for(int i = 6; i<=29; i++){
        map[6][i] = BARRICADE;
        mvaddch(6, i, BARRICADE);
    }
    map[6][9] = SPACE;
    mvaddch(6, 9, SPACE);

    map[15][45] = BARRICADE;
    mvaddch(15, 45, BARRICADE);

    for (int i = 16; i<=35; i++){
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    for(int i = 8; i<=20; i++){
        map[i][29] = BARRICADE;
        mvaddch(i, 29, BARRICADE);
    }
    for(int i = 8; i<=13; i++){
        map[i][16] = BARRICADE;
        mvaddch(i, 16, BARRICADE);
    }
    map[20][28] = BARRICADE;
    mvaddch(20, 28, BARRICADE);
    for(int i = 11; i<=20; i++){
        map[i][27] = BARRICADE;
        mvaddch(i, 27, BARRICADE);
    }
    for(int i = 23; i<=34; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }

    map[7][8] = BARRICADE;
    mvaddch(7, 8, BARRICADE);
    map[7][10] = BARRICADE;
    mvaddch(7, 10, BARRICADE);
    map[7][11] = BARRICADE;
    mvaddch(7, 11, BARRICADE);

    map[9][8] = BARRICADE;
    mvaddch(9, 8, BARRICADE);
    map[9][9] = BARRICADE;
    mvaddch(9, 9, BARRICADE);

    map[10][8] = BARRICADE;
    mvaddch(10, 8, BARRICADE);
    map[10][7] = BARRICADE;
    mvaddch(10, 7, BARRICADE);
    map[10][6] = BARRICADE;
    mvaddch(10, 6, BARRICADE);

    for(int i = 8; i<=13; i++){
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }
    for(int i = 13; i<=18; i++){
        map[15][i] = BARRICADE;
        mvaddch(15, i, BARRICADE);
    }

    for(int i = 18; i<=22; i++){
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }
    for(int i = 18; i<=23; i++){
        map[21][i] = BARRICADE;
        mvaddch(21, i, BARRICADE);
    }
    for(int i = 15; i<=18; i++){
        map[17][i] = BARRICADE;
        mvaddch(17, i, BARRICADE);
    }
    for(int i = 24; i<=27; i++){
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }

    for(int i = 20; i<=25; i++){
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }
    for(int i = 22; i<=26; i++){
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }

    for(int c = 11; c<=12; c++){
        for(int i = 20; i<=22; i++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    for(int i = 9; i<=14; i++){
        map[12][i] = BARRICADE;
        mvaddch(12, i, BARRICADE);
    }

    map[11][9] = BARRICADE;
    mvaddch(11, 9, BARRICADE);

    for(int i = 9; i<=11; i++){
        map[i][11] = BARRICADE;
        mvaddch(i, 11, BARRICADE);

        map[i][13] = BARRICADE;
        mvaddch(i, 13, BARRICADE);

        map[i][14] = BARRICADE;
        mvaddch(i, 14, BARRICADE);
    }

    for(int i = 15; i<=18; i++){
        map[i][8] = BARRICADE;
        mvaddch(i, 8, BARRICADE);
    }
    for(int i = 18; i<=20; i++){
        map[i][9] = BARRICADE;
        mvaddch(i, 9, BARRICADE);
    }
    for(int i = 16; i<=21; i++){
        map[i][11] = BARRICADE;
        mvaddch(i, 11, BARRICADE);
    }
    map[16][10] = BARRICADE;
    mvaddch(16, 10, BARRICADE);

    map[21][12] = BARRICADE;
    mvaddch(21, 12, BARRICADE);
    map[21][14] = BARRICADE;
    mvaddch(21, 14, BARRICADE);

    for(int i = 16; i<=19; i++){
        map[i][13] = BARRICADE;
        mvaddch(i, 13, BARRICADE);
    }
    for(int i = 18; i<=20; i++){
        map[i][15] = BARRICADE;
        mvaddch(i, 15, BARRICADE);
        map[i][18] = BARRICADE;
        mvaddch(i, 18, BARRICADE);
    }

    map[13][15] = BARRICADE;
    mvaddch(13, 15, BARRICADE);

    for(int i = 10; i<=14; i++){
        map[i][18] = BARRICADE;
        mvaddch(i, 18, BARRICADE);
    }

    for(int i = 13; i<=19; i++){
        map[i][20] = BARRICADE;
        mvaddch(i, 20, BARRICADE);
    }
    for(int i = 17; i<=19; i++){
        map[i][21] = BARRICADE;
        mvaddch(i, 21, BARRICADE);
    }
    for(int i = 18; i<=20; i++){
        map[i][23] = BARRICADE;
        mvaddch(i, 23, BARRICADE);
    }

    map[10][22] = BARRICADE;
    mvaddch(10, 22, BARRICADE);

    for(int i = 17; i<=20; i++){
        map[i][25] = BARRICADE;
        mvaddch(i, 25, BARRICADE);
    }

    for(int i = 10; i<=13; i++){
        map[i][24] = BARRICADE;
        mvaddch(i, 24, BARRICADE);
    }
    for(int i = 11; i<=13; i++){
        map[i][26] = BARRICADE;
        mvaddch(i, 26, BARRICADE);
    }

    for(int i = 31; i<=37; i++){
        map[2][i] = BARRICADE;
        mvaddch(2, i, BARRICADE);
    }
    map[3][31] = BARRICADE;
    mvaddch(3, 31, BARRICADE);

    for(int i = 9; i<=20; i++){
        map[i][31] = BARRICADE;
        mvaddch(i, 31, BARRICADE);
        map[i][33] = BARRICADE;
        mvaddch(i, 33, BARRICADE);
    }
    map[9][32] = BARRICADE;
    mvaddch(9, 32, BARRICADE);
    map[20][32] = BARRICADE;
    mvaddch(20, 32, BARRICADE);
    map[19][33] = SPACE;
    mvaddch(19, 33, SPACE);

    for(int i = 9; i<=19; i++){
        map[i][35] = BARRICADE;
        mvaddch(i, 35, BARRICADE);
    }
    map[17][35] = SPACE;
    mvaddch(17, 35, SPACE);
    map[18][34] = BARRICADE;
    mvaddch(18, 34, BARRICADE);
    map[21][35] = BARRICADE;
    mvaddch(21, 35, BARRICADE);
    map[21][36] = BARRICADE;
    mvaddch(21, 36, BARRICADE);
    map[20][36] = BARRICADE;
    mvaddch(20, 36, BARRICADE);
    map[19][36] = BARRICADE;
    mvaddch(19, 36, BARRICADE);
    map[18][36] = BARRICADE;
    mvaddch(18, 36, BARRICADE);
    map[12][36] = BARRICADE;
    mvaddch(12, 36, BARRICADE);

    for(int i = 33; i<=36; i++){
        map[6][i] = BARRICADE;
        mvaddch(6, i, BARRICADE);
    }
    for (int i = 6; i<=12; i++){
        map[i][37] = BARRICADE;
        mvaddch(i, 37, BARRICADE);
    }
    for(int i = 33; i<=38; i++){
        map[4][i] = BARRICADE;
        mvaddch(4, i, BARRICADE);
    }

    for (int i = 37; i <= 44; ++i) {
        map[18][i] = BARRICADE;
        mvaddch(18, i, BARRICADE);
    }
    for (int i = 36; i <= 42; ++i) {
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }
    for (int i = 37; i <= 40; ++i) {
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }

    for (int i = 1; i <= 13; ++i) {
        map[i][39] = BARRICADE;
        mvaddch(i, 39, BARRICADE);
    }
    for (int i = 7; i <= 13; ++i) {
        map[i][40] = BARRICADE;
        mvaddch(i, 40, BARRICADE);
    }
    for (int i = 9; i <= 15; ++i) {
        map[i][42] = BARRICADE;
        mvaddch(i, 42, BARRICADE);
    }
    for (int i = 11; i <= 17; ++i) {
        map[i][44] = BARRICADE;
        mvaddch(i, 44, BARRICADE);
    }
    for (int i = 40; i <= 78; ++i) {
        map[1][i] = BARRICADE;
        mvaddch(1, i, BARRICADE);
    }
    for (int i = 2; i <= 22; ++i) {
        map[i][78] = BARRICADE;
        mvaddch(i, 78, BARRICADE);
    }
    for (int i = 44; i <= 76; ++i) {
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }

    for (int i = 50; i <= 59; ++i) {
        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
    }

    for (int i = 44; i <= 46; ++i) {
        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
    }

    for (int i = 61; i <= 72; ++i) {
        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
    }
    for (int i = 65; i <= 68; ++i) {
        map[19][i] = BARRICADE;
        mvaddch(19, i, BARRICADE);
    }
    for (int i = 48; i <= 54; ++i) {
        map[19][i] = BARRICADE;
        mvaddch(19, i, BARRICADE);
    }
    for (int i = 58; i <= 59; ++i) {
        map[19][i] = BARRICADE;
        mvaddch(19, i, BARRICADE);
    }

    for (int i = 46; i <= 52; ++i) {
        map[17][i] = BARRICADE;
        mvaddch(17, i, BARRICADE);
    }

    for (int i = 46; i <= 50; ++i) {
        map[15][i] = BARRICADE;
        mvaddch(15, i, BARRICADE);
    }

    for (int i = 54; i <= 63; ++i) {
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }

    for (int i = 61; i <= 64; ++i) {
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }

    for (int i = 67; i <= 70; ++i) {
        map[15][i] = BARRICADE;
        mvaddch(15, i, BARRICADE);
    }

    for (int i = 72; i <= 76; ++i) {
        map[15][i] = BARRICADE;
        mvaddch(15, i, BARRICADE);
    }

    for (int i = 65; i <= 70; ++i) {
        map[13][i] = BARRICADE;
        mvaddch(13, i, BARRICADE);
    }
    for (int i = 72; i <= 76; ++i) {
        map[13][i] = BARRICADE;
        mvaddch(13, i, BARRICADE);
    }
    for (int i = 52; i <= 65; ++i) {
        map[12][i] = BARRICADE;
        mvaddch(12, i, BARRICADE);
    }

    for (int i = 56; i <= 59; ++i) {
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }
    map[17][56] = BARRICADE;
    mvaddch(17, 56, BARRICADE);
    map[17][58] = BARRICADE;
    mvaddch(17, 58, BARRICADE);
    map[17][59] = BARRICADE;
    mvaddch(17, 59, BARRICADE);
    map[18][56] = BARRICADE;
    mvaddch(18, 56, BARRICADE);
    map[18][58] = BARRICADE;
    mvaddch(18, 58, BARRICADE);
    map[18][59] = BARRICADE;
    mvaddch(18, 59, BARRICADE);

    for (int i = 67; i <= 76; ++i) {
        map[11][i] = BARRICADE;
        mvaddch(11, i, BARRICADE);
    }

    for (int i = 52; i <= 65; ++i) {
        map[10][i] = BARRICADE;
        mvaddch(10, i, BARRICADE);
    }

    for (int i = 48; i <= 60; ++i) {
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }
    for (int i = 69; i <= 74; ++i) {
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }
    for (int i = 42; i <= 46; ++i) {
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }
    for (int i = 41; i <= 46; ++i) {
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    for (int i = 48; i <= 60; ++i) {
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    for (int i = 69; i <= 72; ++i) {
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }
    for (int i = 40; i <= 43; ++i) {
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for (int i = 45; i <= 46; ++i) {
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for (int i = 50; i <= 60; ++i) {
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for (int i = 62; i <= 65; ++i) {
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for (int i = 67; i <= 74; ++i) {
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }

    for (int i = 41; i <= 76; ++i) {
        map[3][i] = BARRICADE;
        mvaddch(3, i, BARRICADE);
    }

    map[6][46] = BARRICADE;
    mvaddch(6, 46, BARRICADE);

    for (int i = 10; i<=13; i++){
        map[i][46] = BARRICADE;
        mvaddch(i, 46, BARRICADE);
    }

    for (int i = 18; i<=19; i++){
        map[i][46] = BARRICADE;
        mvaddch(i, 46, BARRICADE);
    }

    for (int i = 4; i<=14; i++){
        map[i][48] = BARRICADE;
        mvaddch(i, 48, BARRICADE);
    }
    map[8][48] = SPACE;
    mvaddch(8, 48, SPACE);

    for (int i = 20; i<=21; i++){
        map[i][48] = BARRICADE;
        mvaddch(i, 48, BARRICADE);
    }

    for (int i = 11; i<=14; i++){
        map[i][50] = BARRICADE;
        mvaddch(i, 50, BARRICADE);
    }

    for (int i = 13; i<=16; i++){
        map[i][52] = BARRICADE;
        mvaddch(i, 52, BARRICADE);
    }

    for (int i = 15; i<=18; i++){
        map[i][54] = BARRICADE;
        mvaddch(i, 54, BARRICADE);
    }

    for (int i = 6; i<=8; i++){
        map[i][62] = BARRICADE;
        mvaddch(i, 62, BARRICADE);
        map[i][65] = BARRICADE;
        mvaddch(i, 65, BARRICADE);
    }
    map[9][65] = BARRICADE;
    mvaddch(9, 65, BARRICADE);

    for (int i = 17; i<=19; i++){
        map[i][61] = BARRICADE;
        mvaddch(i, 61, BARRICADE);
    }

    map[18][63] = BARRICADE;
    mvaddch(18, 63, BARRICADE);

    map[14][65] = BARRICADE;
    mvaddch(14, 65, BARRICADE);
    map[15][65] = BARRICADE;
    mvaddch(15, 65, BARRICADE);

    map[16][67] = BARRICADE;
    mvaddch(16, 67, BARRICADE);
    map[17][67] = BARRICADE;
    mvaddch(17, 67, BARRICADE);
    map[17][66] = BARRICADE;
    mvaddch(17, 66, BARRICADE);

    for(int i = 6; i<=10; i++){
        map[i][67] = BARRICADE;
        mvaddch(i, 67, BARRICADE);
    }

    map[6][60] = BARRICADE;
    mvaddch(6, 60, BARRICADE);
    map[12][70] = BARRICADE;
    mvaddch(12, 70, BARRICADE);
    map[14][72] = BARRICADE;
    mvaddch(14, 72, BARRICADE);

    for(int i = 17; i<=18; i++){
        for (int c = 69; c<=70; c++){
            map[i][c] = BARRICADE;
            mvaddch(i, c, BARRICADE);
        }
    }
    map[16][70] = BARRICADE;
    mvaddch(16, 70, BARRICADE);

    map[8][69] = BARRICADE;
    mvaddch(8, 69, BARRICADE);


    for(int i = 17; i<=20; i++){
        map[i][72] = BARRICADE;
        mvaddch(i, 72, BARRICADE);

        map[i][74] = BARRICADE;
        mvaddch(i, 74, BARRICADE);
    }

    map[17][73] = BARRICADE;
    mvaddch(17, 73, BARRICADE);

    for(int i = 6; i<=8; i++){
        map[i][74] = BARRICADE;
        mvaddch(i, 74, BARRICADE);
    }

    for(int i = 4; i<=10; i++){
        map[i][76] = BARRICADE;
        mvaddch(i, 76, BARRICADE);
    }
    for(int i = 16; i<=21; i++){
        map[i][76] = BARRICADE;
        mvaddch(i, 76, BARRICADE);
    }

}

void second_level_map_add_beepers(char map[LINES][COLS]){
    attron(COLOR_PAIR(3));

    map[17][2] = BEEPER_CHAR;
    mvaddch(17, 2, BEEPER_CHAR);

    map[12][15] = BEEPER_CHAR;
    mvaddch(12, 15, BEEPER_CHAR);

    map[22][22] = BEEPER_CHAR;
    mvaddch(22, 22, BEEPER_CHAR);

    map[18][26] = BEEPER_CHAR;
    mvaddch(18, 26, BEEPER_CHAR);

    map[3][30] = BEEPER_CHAR;
    mvaddch(3, 30, BEEPER_CHAR);

    map[17][43] = BEEPER_CHAR;
    mvaddch(17, 43, BEEPER_CHAR);

    map[16][47] = BEEPER_CHAR;
    mvaddch(16, 47, BEEPER_CHAR);

    map[19][47] = BEEPER_CHAR;
    mvaddch(19, 47, BEEPER_CHAR);

    map[17][57] = BEEPER_CHAR;
    mvaddch(17, 57, BEEPER_CHAR);

    map[8][70] = BEEPER_CHAR;
    mvaddch(8, 70, BEEPER_CHAR);
}

void third_level_map(char map[LINES][COLS]){
    third_level_map_add_beepers(map);
    attron(COLOR_PAIR(1));

    if(LINES>24 && COLS>80) {
        for (int i = 1; i <= 78; i++) {
            map[23][i] = BARRICADE;
            mvaddch(23, i, BARRICADE);
        }
        for (int i = 1; i <= 23; i++) {
            map[i][79] = BARRICADE;
            mvaddch(i, 79, BARRICADE);
        }
        mvaddch(23, 5, SPACE);
        map[23][5] = SPACE;
    }
    // Only for 80x24

    for (int i = 17; i<=22; i++){
        map[i][1] = BARRICADE;
        mvaddch(i, 1, BARRICADE);
    }
    for (int i = 5; i<=15; i++){
        map[i][2] = BARRICADE;
        mvaddch(i, 2, BARRICADE);
    }
    map[22][2] = BARRICADE;
    mvaddch(22, 2, BARRICADE);

    map[5][3] = BARRICADE;
    mvaddch(5, 3, BARRICADE);
    map[8][3] = BARRICADE;
    mvaddch(8, 3, BARRICADE);
    map[15][3] = BARRICADE;
    mvaddch(15, 3, BARRICADE);
    for(int i = 17; i<=20; i++){
        map[i][3] = BARRICADE;
        mvaddch(i, 3, BARRICADE);
    }
    map[22][3] = BARRICADE;
    mvaddch(22, 3, BARRICADE);

    map[5][4] = BARRICADE;
    mvaddch(5, 4, BARRICADE);
    map[6][4] = BARRICADE;
    mvaddch(6, 4, BARRICADE);
    map[6][5] = BARRICADE;
    mvaddch(6, 5, BARRICADE);
    map[8][4] = BARRICADE;
    mvaddch(8, 4, BARRICADE);
    map[8][5] = BARRICADE;
    mvaddch(8, 5, BARRICADE);
    map[10][4] = BARRICADE;
    mvaddch(10, 4, BARRICADE);
    map[10][5] = BARRICADE;
    mvaddch(10, 5, BARRICADE);
    for(int i = 11; i<=15; i++){
        map[i][4] = BARRICADE;
        mvaddch(i, 4, BARRICADE);
    }
    map[18][4] = BARRICADE;
    mvaddch(18, 4, BARRICADE);
    map[21][4] = BARRICADE;
    mvaddch(21, 4, BARRICADE);
    map[22][4] = BARRICADE;
    mvaddch(22, 4, BARRICADE);

    map[16][5] = BARRICADE;
    mvaddch(16, 5, BARRICADE);
    map[18][5] = BARRICADE;
    mvaddch(18, 5, BARRICADE);
    map[20][5] = BARRICADE;
    mvaddch(20, 5, BARRICADE);

    for (int i = 6; i<=24; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }
    map[18][6] = BARRICADE;
    mvaddch(18, 6, BARRICADE);

    for (int i = 17; i<=20; i++){
        map[i][7] = BARRICADE;
        mvaddch(i, 7, BARRICADE);
    }

    for (int i = 12; i<=16; i++){
        map[i][6] = BARRICADE;
        mvaddch(i, 6, BARRICADE);
    }
    map[4][6] = BARRICADE;
    mvaddch(4, 6, BARRICADE);

    for (int i = 6; i<=10; i++){
        map[i][7] = BARRICADE;
        mvaddch(i, 7, BARRICADE);
    }
    for (int i = 5; i<=11; i++){
        map[i][8] = BARRICADE;
        mvaddch(i, 8, BARRICADE);
    }
    for (int i = 13; i<=17; i++){
        map[i][8] = BARRICADE;
        mvaddch(i, 8, BARRICADE);
    }
    for (int i = 19; i<=21; i++){
        map[i][9] = BARRICADE;
        mvaddch(i, 9, BARRICADE);
    }

    for(int i = 9; i<=14; i++){
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }

    for(int i = 6; i<=9; i++){
        map[i][10] = BARRICADE;
        mvaddch(i, 10, BARRICADE);
    }

    for(int i = 9; i<=14; i++){
        map[11][i] = BARRICADE;
        mvaddch(11, i, BARRICADE);
    }

    for(int i = 13; i<=16; i++){
        map[i][10] = BARRICADE;
        mvaddch(i, 10, BARRICADE);

        map[i][12] = BARRICADE;
        mvaddch(i, 12, BARRICADE);
    }

    for(int i = 9; i<=11; i++){
        map[17][i] = BARRICADE;
        mvaddch(17, i, BARRICADE);
    }

    for(int i = 11; i<=14; i++){
        map[18][i] = BARRICADE;
        mvaddch(18, i, BARRICADE);

        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
    }
    map[19][11] = BARRICADE;
    mvaddch(19, 11, BARRICADE);

    for(int i = 12; i<=17; i++){
        map[i][14] = BARRICADE;
        mvaddch(i, 14, BARRICADE);
    }

    for (int i = 16; i<=17; i++){
        for(int c = 9; c<=20; c++){
            map[c][i] = BARRICADE;
            mvaddch(c, i, BARRICADE);
        }
    }

    map[14][16] = SPACE;
    mvaddch(14, 16, SPACE);
    map[14][17] = SPACE;
    mvaddch(14, 17, SPACE);

    map[20][15] = BARRICADE;
    mvaddch(20, 15, BARRICADE);

    for (int i = 12; i<=15; i++){
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);

        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }
    map[9][11] = BARRICADE;
    mvaddch(9, 11, BARRICADE);

    for (int i = 4; i<=7; i++){
        map[i][16] = BARRICADE;
        mvaddch(i, 16, BARRICADE);

        map[i][19] = BARRICADE;
        mvaddch(i, 19, BARRICADE);
    }
    mvaddch(7, 18, BARRICADE);

    for(int i=9; i<=20; i++){
        if(i!=13 && i!=18){
            map[i][19] = BARRICADE;
            mvaddch(i, 19, BARRICADE);
        }
    }
    map[15][18] = BARRICADE;
    mvaddch(15, 18, BARRICADE);

    for(int i=9; i<=12; i++){
        if(i!=11){
            map[i][20] = BARRICADE;
            mvaddch(i, 20, BARRICADE);
        }
    }

    for(int i=16; i<=20; i++){
        if(i!=18){
            map[i][20] = BARRICADE;
            mvaddch(i, 20, BARRICADE);
        }
    }

    for(int i=5; i<=20; i++){
        if(i!=11 && i!=13 && i!=15 && i!=18){
            map[i][21] = BARRICADE;
            mvaddch(i, 21, BARRICADE);
        }
    }
    for(int i=5; i<=20; i++){
        if(i!=11){
            map[i][22] = BARRICADE;
            mvaddch(i, 22, BARRICADE);
        }
    }

    for (int i = 24; i<=41; i++){
        for (int c = 5; c<=8; c++){
            if(c!=7){
                map[c][i] = BARRICADE;
                mvaddch(c, i, BARRICADE);
            }
        }
    }
    map[5][23] = BARRICADE;
    mvaddch(5, 23, BARRICADE);
    map[6][23] = BARRICADE;
    mvaddch(6, 23, BARRICADE);
    map[6][42] = BARRICADE;
    mvaddch(6, 42, BARRICADE);
    map[6][43] = BARRICADE;
    mvaddch(6, 43, BARRICADE);
    for(int i = 42; i<=46; i++){
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }
    for(int i = 9; i<=21; i++){
        map[i][24] = BARRICADE;
        mvaddch(i, 24, BARRICADE);
    }
    for(int i = 8; i<=21; i++){
        map[i][24] = BARRICADE;
        mvaddch(i, 24, BARRICADE);
    }
    for(int i = 26; i<=39; i++){
        map[10][i] = BARRICADE;
        mvaddch(10, i, BARRICADE);
    }
    for(int i = 11; i<=22; i++){
        if(i!=17){
            map[i][26] = BARRICADE;
            mvaddch(i, 26, BARRICADE);
        }
    }
    for(int i = 9; i<=14; i++){
        map[i][41] = BARRICADE;
        mvaddch(i, 41, BARRICADE);
    }
    for(int i = 27; i<=40; i++){
        map[14][i] = BARRICADE;
        mvaddch(14, i, BARRICADE);
    }

    for(int i = 28; i<=30; i++){
        map[12][i] = BARRICADE;
        mvaddch(12, i, BARRICADE);
    }

    map[11][32] = BARRICADE;
    mvaddch(11, 32, BARRICADE);
    map[12][32] = BARRICADE;
    mvaddch(12, 32, BARRICADE);

    map[12][35] = BARRICADE;
    mvaddch(12, 35, BARRICADE);
    map[12][34] = BARRICADE;
    mvaddch(12, 34, BARRICADE);
    map[11][35] = BARRICADE;
    mvaddch(11, 35, BARRICADE);

    map[11][39] = BARRICADE;
    mvaddch(11, 39, BARRICADE);
    map[12][39] = BARRICADE;
    mvaddch(12, 39, BARRICADE);
    map[12][38] = BARRICADE;
    mvaddch(12, 38, BARRICADE);
    map[12][37] = BARRICADE;
    mvaddch(12, 37, BARRICADE);

    for(int i = 27; i<=38; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }

    for(int i = 28; i<=42; i++){
        map[18][i] = BARRICADE;
        mvaddch(18, i, BARRICADE);
    }

    for(int i = 32; i<=42; i++){
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }

    map[19][28] = BARRICADE;
    mvaddch(19, 28, BARRICADE);
    map[19][29] = BARRICADE;
    mvaddch(19, 29, BARRICADE);

    map[20][28] = BARRICADE;
    mvaddch(20, 28, BARRICADE);
    map[20][29] = BARRICADE;
    mvaddch(20, 29, BARRICADE);
    map[21][29] = BARRICADE;
    mvaddch(21, 29, BARRICADE);

    map[15][29] = BARRICADE;
    mvaddch(15, 29, BARRICADE);
    map[16][29] = BARRICADE;
    mvaddch(16, 29, BARRICADE);
    map[16][28] = BARRICADE;
    mvaddch(16, 28, BARRICADE);
    map[15][30] = BARRICADE;
    mvaddch(15, 30, BARRICADE);
    map[16][30] = BARRICADE;
    mvaddch(16, 30, BARRICADE);
    map[17][30] = BARRICADE;
    mvaddch(17, 30, BARRICADE);

    map[19][42] = BARRICADE;
    mvaddch(19, 42, BARRICADE);

    for(int i = 31; i<=40; i++){
        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
    }
    for(int i = 40; i<=59; i++){
        map[21][i] = BARRICADE;
        mvaddch(21, i, BARRICADE);
    }
    for(int i = 30; i<=46; i++){
        map[3][i] = BARRICADE;
        mvaddch(3, i, BARRICADE);
    }
    for(int i = 7; i<=16; i++){
        map[i][43] = BARRICADE;
        mvaddch(i, 43, BARRICADE);
    }

    for(int i = 11; i<=19; i++){
        if(i!=12 && i!=14){
            map[i][44] = BARRICADE;
            mvaddch(i, 44, BARRICADE);
        }
    }

    for(int i = 59; i<=78; i++){
        map[22][i] = BARRICADE;
        mvaddch(22, i, BARRICADE);
    }
    for(int i = 61; i<=76; i++){
        map[20][i] = BARRICADE;
        mvaddch(20, i, BARRICADE);
    }
    for(int i = 1; i<=21; i++){
        map[i][78] = BARRICADE;
        mvaddch(i, 78, BARRICADE);
    }
    for(int i = 11; i<=19; i++){
        map[i][76] = BARRICADE;
        mvaddch(i, 76, BARRICADE);
    }
    for(int i = 3; i<=7; i++){
        map[i][76] = BARRICADE;
        mvaddch(i, 76, BARRICADE);
    }
    for(int i = 54; i<=77; i++){
        map[1][i] = BARRICADE;
        mvaddch(1, i, BARRICADE);
    }
    for(int i = 46; i<=54; i++){
        map[2][i] = BARRICADE;
        mvaddch(2, i, BARRICADE);
    }
    for(int i = 3; i<=9; i++){
        map[i][50] = BARRICADE;
        mvaddch(i, 50, BARRICADE);
    }
    for(int i = 4; i<=11; i++){
        map[i][48] = BARRICADE;
        mvaddch(i, 48, BARRICADE);

        map[i][52] = BARRICADE;
        mvaddch(i, 52, BARRICADE);
    }
    for(int i = 45; i<=51; i++){
        map[11][i] = BARRICADE;
        mvaddch(11, i, BARRICADE);
    }

    for(int i = 6; i<=9; i++){
        map[i][46] = BARRICADE;
        mvaddch(i, 46, BARRICADE);
    }

    map[9][45] = BARRICADE;
    mvaddch(9, 45, BARRICADE);

    map[12][46] = BARRICADE;
    mvaddch(12, 46, BARRICADE);

    for(int i = 46; i<=54; i++){
        map[13][i] = BARRICADE;
        mvaddch(13, i, BARRICADE);
    }

    map[15][46] = BARRICADE;
    mvaddch(15, 46, BARRICADE);
    map[15][45] = BARRICADE;
    mvaddch(15, 45, BARRICADE);
    map[15][49] = BARRICADE;
    mvaddch(15, 49, BARRICADE);
    map[15][50] = BARRICADE;
    mvaddch(15, 50, BARRICADE);

    for(int i = 45; i<=50; i++){
        map[16][i] = BARRICADE;
        mvaddch(16, i, BARRICADE);
    }

    map[20][46] = BARRICADE;
    mvaddch(20, 46, BARRICADE);

    for(int i = 18; i<=19; i++){
        for(int c = 46; c<=50; c++){
            map[i][c] = BARRICADE;
            mvaddch(i, c, BARRICADE);
        }
    }

    for(int i = 14; i<=20; i++){
        if(i!=17){
            map[i][52] = BARRICADE;
            mvaddch(i, 52, BARRICADE);

            map[i][54] = BARRICADE;
            mvaddch(i, 54, BARRICADE);
        }
    }

    map[20][59] = BARRICADE;
    mvaddch(20, 59, BARRICADE);
    map[19][59] = BARRICADE;
    mvaddch(19, 59, BARRICADE);
    map[19][58] = BARRICADE;
    mvaddch(19, 58, BARRICADE);
    map[19][57] = BARRICADE;
    mvaddch(19, 57, BARRICADE);
    map[19][56] = BARRICADE;
    mvaddch(19, 56, BARRICADE);

    for(int i = 56; i<=74; i++){
        if(i!=65){
            map[18][i] = BARRICADE;
            mvaddch(18, i, BARRICADE);
        }
    }
    map[19][66] = BARRICADE;
    mvaddch(19, 66, BARRICADE);

    for(int i = 54; i<=75; i++){
        if(i!=55 && i!=63){
            map[3][i] = BARRICADE;
            mvaddch(3, i, BARRICADE);
        }
    }
    map[4][54] = BARRICADE;
    mvaddch(4, 54, BARRICADE);
    for(int i = 54; i<=60; i++){
        map[5][i] = BARRICADE;
        mvaddch(5, i, BARRICADE);
    }

    for(int i = 4; i<=16; i++){
        if(i!=8 && i!=14){
            map[i][62] = BARRICADE;
            mvaddch(i, 62, BARRICADE);
        }
    }

    for(int i = 53; i<=61; i++){
        map[7][i] = BARRICADE;
        mvaddch(7, i, BARRICADE);
    }

    for(int i = 54; i<=61; i++){
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }

    for(int i = 56; i<=61; i++){
        map[11][i] = BARRICADE;
        mvaddch(11, i, BARRICADE);
    }

    for(int i = 64; i<=76; i++){
        map[11][i] = BARRICADE;
        mvaddch(11, i, BARRICADE);
    }

    for(int i = 66; i<=77; i++){
        map[9][i] = BARRICADE;
        mvaddch(9, i, BARRICADE);
    }

    for(int i = 63; i<=74; i++){
        map[13][i] = BARRICADE;
        mvaddch(13, i, BARRICADE);
    }

    for(int i = 10; i<=12; i++){
        map[i][54] = BARRICADE;
        mvaddch(i, 54, BARRICADE);
    }
    for(int i = 12; i<=13; i++){
        map[i][56] = BARRICADE;
        mvaddch(i, 56, BARRICADE);
    }
    map[16][56] = BARRICADE;
    mvaddch(16, 56, BARRICADE);

    for(int i = 13; i<=16; i++){
        map[i][57] = BARRICADE;
        mvaddch(i, 57, BARRICADE);
    }

    for(int i = 12; i<=16; i++){
        map[i][60] = BARRICADE;
        mvaddch(i, 60, BARRICADE);
    }
    for(int i = 12; i<=16; i++){
        if(i!=14){
            map[i][59] = BARRICADE;
            mvaddch(i, 59, BARRICADE);
        }
    }
    for(int i = 15; i<=16; i++){
        map[i][61] = BARRICADE;
        mvaddch(i, 61, BARRICADE);
    }

    for(int i = 14; i<=16; i++){
        map[i][64] = BARRICADE;
        mvaddch(i, 64, BARRICADE);

        map[i][66] = BARRICADE;
        mvaddch(i, 66, BARRICADE);
    }

    for(int i = 15; i<=17; i++){
        map[i][64] = BARRICADE;
        mvaddch(i, 64, BARRICADE);
    }

    for(int i = 69; i<=72; i++){
        map[15][i] = BARRICADE;
        mvaddch(15, i, BARRICADE);
    }

    for(int i = 15; i<=17; i++){
        map[i][68] = BARRICADE;
        mvaddch(i, 68, BARRICADE);
    }

    for(int i = 14; i<=17; i++){
        map[i][74] = BARRICADE;
        mvaddch(i, 74, BARRICADE);
    }

    map[16][70] = BARRICADE;
    mvaddch(16, 70, BARRICADE);

    map[16][72] = BARRICADE;
    mvaddch(16, 72, BARRICADE);

    for(int i = 4; i<=10; i++){
        map[i][64] = BARRICADE;
        mvaddch(i, 64, BARRICADE);
    }

    for(int i = 5; i<=8; i++){
        map[i][66] = BARRICADE;
        mvaddch(i, 66, BARRICADE);

        map[i][70] = BARRICADE;
        mvaddch(i, 70, BARRICADE);

        map[i][74] = BARRICADE;
        mvaddch(i, 74, BARRICADE);
    }

    for(int i = 4; i<=7; i++){
        map[i][68] = BARRICADE;
        mvaddch(i, 68, BARRICADE);

        map[i][72] = BARRICADE;
        mvaddch(i, 72, BARRICADE);
    }

    map[17][64] = SPACE;
    mvaddch(17, 64, SPACE);
}

void third_level_map_add_beepers(char map[LINES][COLS]){
    attron(COLOR_PAIR(3));

    map[21][3] = BEEPER_CHAR;
    mvaddch(21, 3, BEEPER_CHAR);

    map[6][9] = BEEPER_CHAR;
    mvaddch(6, 9, BEEPER_CHAR);

    map[17][12] = BEEPER_CHAR;
    mvaddch(17, 12, BEEPER_CHAR);

    map[4][34] = BEEPER_CHAR;
    mvaddch(4, 34, BEEPER_CHAR);

    map[11][36] = BEEPER_CHAR;
    mvaddch(11, 36, BEEPER_CHAR);

    map[12][45] = BEEPER_CHAR;
    mvaddch(12, 45, BEEPER_CHAR);

    map[8][53] = BEEPER_CHAR;
    mvaddch(8, 53, BEEPER_CHAR);

    map[12][57] = BEEPER_CHAR;
    mvaddch(12, 57, BEEPER_CHAR);

    map[22][58] = BEEPER_CHAR;
    mvaddch(22, 58, BEEPER_CHAR);

    map[16][69] = BEEPER_CHAR;
    mvaddch(16, 69, BEEPER_CHAR);
}


int is_nickname_taken(char name[15]){
    FILE *file = fopen(FILE_NAME, "r");
    int is_flag = 0;
    char line[120];

    if ( file != NULL ) {
        while ( fgets ( line, sizeof line, file ) != NULL ) {
            if (strstr(line, name) != NULL) {
                is_flag = 1;
            }
        }
    }
    fclose(file);

    return is_flag;
}

void change_score_in_file(char *line, int line_number){
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        return;
    }

    char buffer[1024];
    int currentLine = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        currentLine++;
        if (currentLine == line_number) {
            fputs(line, tempFile);
        } else {
            fputs(buffer, tempFile);
        }
    }
    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
}

int number_of_line(char *world){
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return 0;
    }

    char buffer[1024];
    int lineNumber = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        lineNumber++;
        if (strstr(buffer, world) != NULL) {
            fclose(file);
            return lineNumber;
        }
    }

    fclose(file);
    return 0;
}
