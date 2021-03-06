//
// Created by moosong on 6/30/20.
//

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "commonNcurses.h"
#include <pthread.h>


//struct 
typedef struct Array{
	char * array[ARR_SIZ];
	int next;
}Array;

///////////////////////SAMPLE FILES/////////////////////////////////////////


char * sampleFile[] = {
        "this is file 1", "this is file 2", "this is file 3", "this is file 4",
        "this is file 5", "this is file 6", "this is file 7", "(192.168.30.17)server connected..",
        " ", " ", "this is file 11", "this is file 12",
        "this is file 13", "this is file 14", "this is file 15", "this is file 16"
	//"192.168.30.22 server에 접송중..."
};
///////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
/// to make env for ncurses and color pairs
/// input : void
/// return : void
////////////////////////////////////////////////////////////
void init_scr()
{
    initscr();
    start_color();

    ///first:index
    ///second:text
    ///color:background
	init_pair(MAIN1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(MAIN2, COLOR_BLACK, COLOR_WHITE);
    init_pair(CLIENTBAR, COLOR_BLACK, COLOR_YELLOW);
    init_pair(SELECTED, COLOR_BLACK, COLOR_YELLOW);
    init_pair(CHECKED, COLOR_BLACK, COLOR_GREEN);
    ///insert color pairs you want
}

/////////////////////////////////////////////////////////////
/// to prepare up and down window (for ip insert connection)
/// inputs : upwindow, middlewindow, downWindow
/// return : void
////////////////////////////////////////////////////////////
void init_IP_insert_Page(WINDOW *upWindow, WINDOW *middleWindow, WINDOW *downWindow)
{
    upWindow = subwin(stdscr, 14, FTP_WIDE, 0, 0);
    wbkgd(upWindow, COLOR_PAIR(MAIN1));

    middleWindow = subwin(stdscr, 7, FTP_WIDE, 14, 0);
    wbkgd(middleWindow, COLOR_PAIR(MAIN2));

    downWindow = subwin(stdscr, 3, FTP_WIDE, 21, 0);
    wbkgd(downWindow, COLOR_PAIR(MAIN1));
}
 ////////////////////////////////////////////////////////////
 /// to prepare Help Page
 /// input : all windows to make Help Page
 /// return : void
 ///////////////////////////////////////////////////////////
 void init_Help_Page(WINDOW *helpWindow){

     helpWindow = subwin(stdscr, FTP_HEIGHT, FTP_WIDE, 0, 0);
     wbkgd(helpWindow, COLOR_PAIR(MAIN1));

}

/////////////////////////////////////////////////////////////
///to show help page
// return : void
////////////////////////////////////////////////////////////
int Help_Page(){
	werase(stdscr); //clear Window
	curs_set(0); //Not need Cursor pointer
	
	WINDOW *hWin = NULL;
    int key;

    init_Help_Page(hWin);
    //	attron(COLOR_PAIR(MAIN1));
	mvprintw(1, 30, "< HELP PAGE >");

	mvprintw(2, 2, "Usage : PUT IP ADDRESS + ENTER KEY ");
    mvprintw(3, 2, "If you want to move the cursor -> use Arrow key");
	mvprintw(4, 2, "If you want to select the menu -> use Enter key");
	
	mvprintw(6, 25, "---- MENU USAGE ----");
	mvprintw(7, 2, "HISTORY : 'History'page shows the record history where you logged in before");
	mvprintw(8, 2, "EXIT : Terminate the FileDanzi FTP Service");

	mvprintw(10, 20, "---- COMMAND KEY USAGE ----");
	mvprintw(11, 2, "1. Copyfile : Select the file or files to copy and press the 'c' key");
	mvprintw(12, 2, "2. Movefile : Select the file or files to move and press the 'm' key");
	mvprintw(13, 2, "3. RemoveFile : Select the file or files to delete and press the 'r' key");
	mvprintw(14, 2, "3. ReNamefile : Select the file or files to rename and press the 'n' key");
	mvprintw(15, 2, "4. DownloadFile : Select the file or files to download and press the 'f' key");
	mvprintw(16, 2, "5. IPManage : ");
	mvprintw(17, 2, "6. MoVeDir : Select the directory to move and press the 'v' key");
	mvprintw(18, 2, "7. RemovEDir : Select the directory to remove nad press the 'e' key");
	mvprintw(19, 2, "8. RenAmeDir : Select the directory to rename and press the 'a' key");
	mvprintw(20, 2, "9. Help : Manual page FileDanzi FTP Server and press the 'h' key");
	mvprintw(21, 2, "10.MaKeFolder : Make new folder for manage filesystem 'k' key");
	mvprintw(22, 2, "11.EXit : Terminate the FileDanzi FTP Server to press the 'x' key");
    
	
	//underline
    attron(A_UNDERLINE);
    mvprintw(11, 5, "C");
    mvprintw(12, 5, "M");
    mvprintw(13, 5, "R");
    mvprintw(14, 7, "N");
	mvprintw(15, 13,"F");
    mvprintw(16, 6, "P");
    mvprintw(17, 7, "V");
    mvprintw(18, 10,"E");
    mvprintw(19, 8, "A");
    mvprintw(20, 5, "H");
    mvprintw(21, 7, "K");
    mvprintw(22, 6, "X");

    attroff(A_UNDERLINE);

	wrefresh(hWin);
	refresh();

	if (key = getch()){
		return MENU_MAIN;
	}
	delwin(hWin);

}

////////////////////////////////////////////////////////////
/// to show main menu
/// input1 : ip Address space to store
/// return : menunum
/////
int IP_insert_Page(char**ip){
    werase(stdscr); // Clear Window
    curs_set(0); // Not need Cursor pointer
    char key; // User Input
    char ipAddress[IPADDRESSLENGTH] = "";

    int selectingMenu = MAIN_HELP;

    WINDOW *upMenu = NULL;
    WINDOW *middleWindow = NULL;
    WINDOW *downMenu = NULL;
    init_IP_insert_Page(upMenu, middleWindow, downMenu);

    attron(COLOR_PAIR(MAIN1));
    mvprintw(3, 34, "FILE DANZI");
    mvprintw(7, 32, "SIMPLE & CUTE FTP Service");

    attron(COLOR_PAIR(MAIN2));
    mvprintw(16, 16, "INSERT SERVER'S IP ADDRESS (press enter key)");
    mvprintw(18, 36, " : ");

    while(1){
        attron(COLOR_PAIR(MAIN2));
        mvprintw(18, 26, "IP ADDRESS");
        mvprintw(18, 39, "               ");
        mvprintw(18, 39, ipAddress);
        if (selectingMenu == MENU_IP_INSERT){
            attron(A_STANDOUT | A_UNDERLINE);
            mvprintw(18, 39+strlen(ipAddress), " ");
            attroff(A_STANDOUT | A_UNDERLINE);
        }

        attron(COLOR_PAIR(MAIN1));
        mvprintw(22, 15, "HELP");
        mvprintw(22, 36, "HISTORY");
        mvprintw(22, 58, "EXIT");

        attron(A_STANDOUT | A_UNDERLINE); // selected effect
        switch(selectingMenu){
            case MAIN_HELP :
                mvprintw(22, 15, "HELP");
                break;
            case MENU_HISTORY :
                mvprintw(22, 36, "HISTORY");
                break;
            case MENU_EXIT :
                mvprintw(22, 58, "EXIT");
                break;
        }
        attroff(A_STANDOUT | A_UNDERLINE);

        refresh();

        key = getchar();

        switch(key) {
            case KEYBOARD_UP:
                if (selectingMenu == MAIN_HELP || selectingMenu == MENU_EXIT || selectingMenu == MENU_HISTORY) {
                    selectingMenu = MENU_IP_INSERT;
                }
                break;
            case KEYBOARD_DOWN:
                if (selectingMenu == MENU_IP_INSERT) {
                    selectingMenu = MAIN_HELP;
                }
                break;
            case KEYBOARD_RIGHT:
                if (selectingMenu == MAIN_HELP)
                    selectingMenu = MENU_HISTORY;
                else if (selectingMenu == MENU_HISTORY)
                    selectingMenu = MENU_EXIT;
                else if (selectingMenu == MENU_EXIT)
                    selectingMenu = MAIN_HELP;
                break;
            case KEYBOARD_LEFT :
                if (selectingMenu == MENU_HISTORY)
                    selectingMenu = MAIN_HELP;
                else if (selectingMenu == MENU_EXIT)
                    selectingMenu = MENU_HISTORY;
                else if (selectingMenu == MAIN_HELP)
                    selectingMenu = MENU_EXIT;
                break;
            case KEYBOARD_BACKSPACE:
                if (selectingMenu == MENU_IP_INSERT) {
                    ipAddress[strlen(ipAddress) - 1] = '\0';
                }
                break;
            case KEYBOARD_ENTER :
                if (selectingMenu == MENU_IP_INSERT){
					*ip =malloc(sizeof(char)*16);
					strcpy(*ip,ipAddress);
                   // *ip=ipAddress;
                    delwin(upMenu);
                    delwin(middleWindow);
                    delwin(downMenu);
                    return selectingMenu;
                } else if (selectingMenu == MENU_EXIT){
                    *ip=NULL;
                    delwin(upMenu);
                    delwin(middleWindow);
                    delwin(downMenu);
                    return selectingMenu;
                } else if (selectingMenu == MENU_HISTORY){
					*ip =malloc(sizeof(char)*16);
					strcpy(*ip,ipAddress);
                   // *ip=ipAddress;
                    delwin(upMenu);
                    delwin(middleWindow);
                    delwin(downMenu);
                    return selectingMenu;
                } else if (selectingMenu == MAIN_HELP){
					*ip =malloc(sizeof(char)*16);
					strcpy(*ip,ipAddress);
                    //*ip=ipAddress;
                    delwin(upMenu);
                    delwin(middleWindow);
                    delwin(downMenu);
                    return selectingMenu;
                }
            default:
                //if ((selectingMenu == MENU_IP_INSERT) && ((key >= '0' && key <= '9') || (key >= 'a' && key <= 'z') || (key == '.') )) {
                if ((selectingMenu == MENU_IP_INSERT) && ((key >= '0' && key <= '9') || (key == '.') )) {
                    char tempKey[2];
                    tempKey[0] = key;
                    tempKey[1] = '\0';
                    if ((selectingMenu == MENU_IP_INSERT) && (strlen(ipAddress) < IPADDRESSLENGTH)) {
                        strcat(ipAddress, tempKey);
                    }
                }
                break;
        }
    }
}

/////////////////////////////////////////////////////////////
/// to prepare FTP main window
/// input : all windows to make FTP main
/// return : void
////////////////////////////////////////////////////////////
void init_FTP_Main_Page(WINDOW*upWindow, WINDOW * downWindow,
                       WINDOW * logWindow, WINDOW * pathWindow, WINDOW * firstWindow,
                       WINDOW * secondWindow, WINDOW * thirdWindow, WINDOW * fourthWindow,
                       WINDOW * xWindow, WINDOW * yWindow, WINDOW * zWindow){

    upWindow = subwin(stdscr, MANUAL_SUBWINDOW_HEIGHT, FTP_WIDE, 0 , 0);
    wbkgd(upWindow, COLOR_PAIR(CLIENTBAR));

    logWindow = subwin(stdscr, LOG_SUBWINDOW_HEIGHT, FTP_WIDE, MANUAL_SUBWINDOW_HEIGHT, 0);
    wbkgd(logWindow, COLOR_PAIR(MAIN2));

    pathWindow = subwin(stdscr, BAR_HEIGHT, FTP_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT, 0);
    wbkgd(pathWindow, COLOR_PAIR(CLIENTBAR));

    firstWindow = subwin(stdscr, UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT, 0);
    wbkgd(firstWindow, COLOR_PAIR(MAIN2));

    xWindow = subwin(stdscr, UPPER_SUBWINDOW_HEIGHT, BAR_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT, LEFT_SUBWINDOW_WIDE);
    wbkgd(xWindow, COLOR_PAIR(CLIENTBAR));

    secondWindow = subwin(stdscr, UPPER_SUBWINDOW_HEIGHT, RIGHT_SUBWINDOW_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT, LEFT_SUBWINDOW_WIDE + BAR_WIDE);
    wbkgd(secondWindow, COLOR_PAIR(MAIN2));

    yWindow = subwin(stdscr, BAR_HEIGHT, FTP_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT, 0);
    wbkgd(yWindow, COLOR_PAIR(CLIENTBAR));

    thirdWindow = subwin(stdscr, DOWN_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT, 0);
    wbkgd(thirdWindow, COLOR_PAIR(MAIN2));

    zWindow = subwin(stdscr, DOWN_SUBWINDOW_HEIGHT, BAR_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT, LEFT_SUBWINDOW_WIDE);
    wbkgd(zWindow, COLOR_PAIR(CLIENTBAR));

    fourthWindow = subwin(stdscr, DOWN_SUBWINDOW_HEIGHT, RIGHT_SUBWINDOW_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT, LEFT_SUBWINDOW_WIDE + BAR_WIDE);
    wbkgd(fourthWindow, COLOR_PAIR(MAIN2));

    downWindow = subwin(stdscr, BAR_HEIGHT, FTP_WIDE, MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT + DOWN_SUBWINDOW_HEIGHT, 0);
    wbkgd(downWindow, COLOR_PAIR(CLIENTBAR));
}

/////////////////////////////////////////////////////////////
/// to destory_FTP_Main_Page
/// input1 : all windows for Main
/// return : void
////////////////////////////////////////////////////////////
void destroy_FTP_Main_Page(WINDOW*upWindow, WINDOW * downWindow,
                        WINDOW * logWindow, WINDOW * pathWindow, WINDOW * firstWindow,
                        WINDOW * secondWindow, WINDOW * thirdWindow, WINDOW * fourthWindow,
                        WINDOW * xWindow, WINDOW * yWindow, WINDOW * zWindow){
    delwin(upWindow);
    delwin(downWindow);
    delwin(logWindow);
    delwin(pathWindow);
    delwin(firstWindow);
    delwin(secondWindow);
    delwin(thirdWindow);
    delwin(fourthWindow);
    delwin(xWindow);
    delwin(yWindow);
    delwin(zWindow);
}

/////////////////////////////////////////////////////////////
/// to show up and down manual bars
/// input : mode (MODE_CLIENT or MODE_SERVER)
/// return : void
////////////////////////////////////////////////////////////
void print_Manual_Bar(int mode){
    ///print
    char * str;
    if (mode == MODE_CLIENT){
        str = "DownloadFile";
    }else if (mode == MODE_SERVER){
        str = " UploadFile";
    }
    mvprintw(0, 2, "CopyFile");
    mvprintw(0, 14, "MoveFile");
    mvprintw(0, 26, "RemoveFile");
    mvprintw(0, 40, "reNameFile");
    mvprintw(0, 54, str);
    mvprintw(0, 70, "MakeDir");

    int temp = FTP_HEIGHT-1;
   // mvprintw(temp, 12, "MoVeDir");
   // mvprintw(temp, 23, "RemovEDir");
   // mvprintw(temp, 36, "RenameDir");
   // mvprintw(temp, 49, "Help");
   // mvprintw(temp, 57, "MakeFolder");
    mvprintw(temp, 71, "EXit");

    ///underline
    attron(A_UNDERLINE);
    mvprintw(0, 2, "C");
    mvprintw(0, 14, "M");
    mvprintw(0, 26, "R");
    mvprintw(0, 42, "N");
    if (mode == MODE_CLIENT){
        mvprintw(0, 62, "F");
    } else if (mode == MODE_SERVER){
        mvprintw(0, 60, "F");
    }
    mvprintw(0, 72, "K");

    //mvprintw(temp, 14, "V");
    //mvprintw(temp, 28, "E");
    //mvprintw(temp, 39, "A");
    //mvprintw(temp, 49, "H");
    //mvprintw(temp, 59, "K");
    mvprintw(temp, 72, "X");

    attroff(A_UNDERLINE);
}

/////////////////////////////////////////////////////////////
/// to print title
/// input : mode (MODE_CLIENT or MODE_SERVER)
/// return : void
////////////////////////////////////////////////////////////
void print_Title_Block(int mode){
    mvprintw(MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT, 0, "                                        ");
    mvprintw(MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, "                                       ");
    mvprintw(MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT, 0, "LOCAL FILE TREE");
    if (mode == MODE_CLIENT){
        mvprintw(MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, "SERVER FILE TREE");
    } else if (mode == MODE_SERVER){
        mvprintw(MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, "REMOTE FILE TREE");
    }
}

/////////////////////////////////////////////////////////////
/// to print subfiles for up and down window
/// input : mode (MODE_FIRST, MODE_SECOND, MODE_THIRD, MODE_FOURTH), src char* 's ary, a number array
/// return : void
////////////////////////////////////////////////////////////
void print_Sub_Block(int mode, char** srcArray, int aryCount){
    char * nullStr = "                                       ";
    int x;
    int y;
    int totalCount;
    switch (mode) {
        case MODE_FIRST:
            x=0;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount = UPPER_SUBWINDOW_HEIGHT;
            break;
        case MODE_SECOND:
            x=LEFT_SUBWINDOW_WIDE + BAR_WIDE;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount = UPPER_SUBWINDOW_HEIGHT;
            break;
        case MODE_THIRD:
            x=0;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount=DOWN_SUBWINDOW_HEIGHT;
            break;
        case MODE_FOURTH:
            x=LEFT_SUBWINDOW_WIDE + BAR_WIDE;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount = DOWN_SUBWINDOW_HEIGHT;
            break;
        default:
            return;
    }

    //this is reset loop
    for (int i = 0; i < totalCount; ++i) {
        mvprintw(y+i, x, nullStr);
    }
    if (srcArray == NULL){
        return;
    }

    if (aryCount < totalCount){
        totalCount = aryCount;
    }
    //this is print loop
    for (int j = 0; j < totalCount; ++j) {
        mvprintw(y+j, x, srcArray[j]);
    }
    return;
}

/////////////////////////////////////////////////////////////
/// to print Log Block
/// input : log ary and count
/// return : void
////////////////////////////////////////////////////////////
void print_Log_Block(char  ** srcArray, int aryCount){
    int x =0;
    int y = MANUAL_SUBWINDOW_HEIGHT;
    int totalCount = LOG_SUBWINDOW_HEIGHT;
    char * nullStr = "                                                                                ";

    //this is reset loop
    for (int i = 0; i < totalCount; ++i) {
        mvprintw(y+i, x, nullStr);
    }
    if (srcArray == NULL){
        return;
    }

    //this is print loop
    if (aryCount < totalCount){
        for (int i = 0; i < aryCount; ++i) {
            mvprintw(y+i, x, srcArray[i]);
        }
    }else{
        int i = 0;
        for (int j = aryCount - totalCount; j < aryCount; ++j) {
            mvprintw(y+i, x, srcArray[j]);
            i++;
        }
    }
    return;
}

/////////////////////////////////////////////////////////////
/// to print path
/// input : pathOfLeft, pathOfRight
/// return : void
////////////////////////////////////////////////////////////
void print_Path_Block(char * pathOfLeft, char *pathOfRight){
    mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, 0, "                                        ");
    mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, 0, pathOfLeft);
    mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE + BAR_WIDE, "                                       ");
    mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE + BAR_WIDE, pathOfRight);
}

/////////////////////////////////////////////////////////////
/// to print Main Page
/// input : mode MODE_CLIENT or MODE_SERVER, path1, path2
/// return : selectMenu
////////////////////////////////////////////////////////////
int FTP_Main_Page(int mode, char * pathOfLeft, char *pathOfRight,ResInfo *resInfo,char **msg,Array* myDirectories,Array*myFiles,Array*directories,Array* files) {
    werase(stdscr); // Clear Window
    curs_set(0); // Not need Cursor pointer
    char key; // User Input
    int selectingMenu = MENU_FIRSTWINODW;
	pthread_t tid;

    WINDOW *upMenu = NULL;
    WINDOW *downMenu = NULL;
    WINDOW *logWindow = NULL;
    WINDOW *pathWindow = NULL;
    WINDOW *firstWindow = NULL;
    WINDOW *secondWindow =NULL;
    WINDOW *thirdWindow = NULL;
    WINDOW *fourthWindow = NULL;
    WINDOW *xWindow = NULL;
    WINDOW *yWindow = NULL;
    WINDOW *zWindow = NULL;

    init_FTP_Main_Page(upMenu, downMenu, logWindow, pathWindow, firstWindow,
                      secondWindow, thirdWindow, fourthWindow,
                      xWindow, yWindow, zWindow);

    attron(COLOR_PAIR(CLIENTBAR));
    print_Manual_Bar(mode);

    while (1) {
        attron(COLOR_PAIR(MAIN1));
        print_Title_Block(mode);
		print_Path_Block(pathOfLeft, pathOfRight);

        attron(COLOR_PAIR(MAIN2));
        print_Log_Block(sampleFile, 10);
        print_Sub_Block(MODE_FIRST, myDirectories->array, myDirectories->next);
        print_Sub_Block(MODE_SECOND, directories->array, directories->next);
        print_Sub_Block(MODE_THIRD, myFiles->array, myFiles->next);
        print_Sub_Block(MODE_FOURTH, files->array, files->next);

        attron(A_STANDOUT | A_UNDERLINE); // selected effect
        switch (selectingMenu) {
            case MENU_FIRSTWINODW:
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT, 0, "LOCAL FILE TREE (press enter)           ");
                break;
            case MENU_SECONDWINDOW:
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT, 41, "SERVER FILE TREE (press enter)         ");
                break;
            case MENU_THIRDWINDOW:
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, 0, "                                        ");
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, strlen(pathOfLeft), " (press enter)");
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, 0, pathOfLeft);
                break;
            case MENU_FOURTHWINDOW:
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, "                                       ");
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE+strlen(pathOfRight), " (press enter)");
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, pathOfRight);
                break;
        }
        attroff(A_STANDOUT | A_UNDERLINE);

        attron(COLOR_PAIR(MAIN1));
        refresh();

        key = getchar();

        switch (key) {
            case KEYBOARD_TAB:
                if (selectingMenu == MENU_FIRSTWINODW) {
                    selectingMenu = MENU_SECONDWINDOW;
                } else if (selectingMenu == MENU_SECONDWINDOW) {
                    selectingMenu = MENU_THIRDWINDOW;
                } else if (selectingMenu == MENU_THIRDWINDOW) {
                    selectingMenu = MENU_FOURTHWINDOW;
                } else if (selectingMenu == MENU_FOURTHWINDOW) {
                    selectingMenu = MENU_FIRSTWINODW;
                }
                break;
            case KEYBOARD_UP:
            case KEYBOARD_DOWN:
                if (selectingMenu == MENU_FIRSTWINODW) {
                    selectingMenu = MENU_THIRDWINDOW;
                } else if (selectingMenu == MENU_SECONDWINDOW) {
                    selectingMenu = MENU_FOURTHWINDOW;
                } else if (selectingMenu == MENU_THIRDWINDOW) {
                    selectingMenu = MENU_FIRSTWINODW;
                } else if (selectingMenu == MENU_FOURTHWINDOW) {
                    selectingMenu = MENU_SECONDWINDOW;
                }
                break;
            case KEYBOARD_LEFT:
            case KEYBOARD_RIGHT:
                if (selectingMenu == MENU_FIRSTWINODW) {
                    selectingMenu = MENU_SECONDWINDOW;
                } else if (selectingMenu == MENU_SECONDWINDOW) {
                    selectingMenu = MENU_FIRSTWINODW;
                } else if (selectingMenu == MENU_THIRDWINDOW) {
                    selectingMenu = MENU_FOURTHWINDOW;
                } else if (selectingMenu == MENU_FOURTHWINDOW) {
                    selectingMenu = MENU_THIRDWINDOW;
                }
                break;
				
            case KEYBOARD_ENTER:
// 				mvprintw(1,1,"FTP_Main_Page: enter눌림");
                if ( (selectingMenu == MENU_FIRSTWINODW)||(selectingMenu == MENU_SECONDWINDOW)||
                        (selectingMenu == MENU_THIRDWINDOW)||(selectingMenu == MENU_FOURTHWINDOW)){
                    return selectingMenu;
                }
                break;
				
            case IP_MANAGE_KEY:
                return MENU_IP_MANAGE;
            case HELP_KEY:
                return MENU_HELP;
            case EXIT_KEY:
                destroy_FTP_Main_Page(upMenu, downMenu, logWindow, pathWindow, firstWindow,
                                      secondWindow, thirdWindow, fourthWindow,
                                      xWindow, yWindow, zWindow);
				if(pthread_create(&tid, NULL, doQuitThread,resInfo)!=0){
					perror("pthread_create");
					return -1;
				}
				if(pthread_join(tid,(void**)tret)!=0){
					perror("pthread_join");
					return -1;
				}
				free(tret);
                return MENU_EXIT;
        }
    }
}

/////////////////////////////////////////////////////////////
/// to print selected Page
/// input : mode (MODE_CLIENT or MODE_SERVER), path1, path2
/// return : selectMenu
////////////////////////////////////////////////////////////
int print_Selected_Page(int mode, int selectingMenu, char** srcAry, int** selectedArrPtr, int aryCount, char * pathOfLeft, char * pathOfRight, ResInfo *resInfo,char **msg){
    int startPoint = 0;
    int cursor = startPoint;
    char * nullStr;
    int x;
    int y;
    int totalCount;
    int *selected = calloc(aryCount,sizeof(int));//NULL체크 해야함
    char key;
	int selectedCnt=0;

    switch (selectingMenu) {
        case MENU_FIRSTWINODW:
            x=0;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount = UPPER_SUBWINDOW_HEIGHT;
            nullStr = "                                        ";
            break;
        case MENU_SECONDWINDOW:
            x=LEFT_SUBWINDOW_WIDE + BAR_WIDE;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount = UPPER_SUBWINDOW_HEIGHT;
            nullStr = "                                       ";
            break;
        case MENU_THIRDWINDOW:
            x=0;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount=DOWN_SUBWINDOW_HEIGHT;
            nullStr = "                                        ";
            break;
        case MENU_FOURTHWINDOW:
            x=LEFT_SUBWINDOW_WIDE + BAR_WIDE;
            y=MANUAL_SUBWINDOW_HEIGHT + LOG_SUBWINDOW_HEIGHT + BAR_HEIGHT + UPPER_SUBWINDOW_HEIGHT + BAR_HEIGHT;
            totalCount = DOWN_SUBWINDOW_HEIGHT;
            nullStr = "                                       ";
            break;
        default:
            break;
    }

    attron(COLOR_PAIR(CLIENTBAR));
    attron(A_STANDOUT | A_UNDERLINE); // selected effect
    switch (selectingMenu) {
        case MENU_FIRSTWINODW:
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT, 0, "LOCAL FILE TREE (press back space)      ");
            break;
        case MENU_SECONDWINDOW:
            if (mode == MODE_CLIENT){
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT, 41, "SERVER FILE TREE (press back space)    ");
                break;
            } else if (mode == MODE_SERVER){
                mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT, 41, "REMOTE FILE TREE (press back space)    ");
                break;
            }
        case MENU_THIRDWINDOW:
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, 0, "                                        ");
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, strlen(pathOfLeft), " (press back space)");
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, 0, pathOfLeft);
            break;
        case MENU_FOURTHWINDOW:
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, "                                       ");
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE+strlen(pathOfRight), " (press back space)");
            mvprintw(MANUAL_SUBWINDOW_HEIGHT+LOG_SUBWINDOW_HEIGHT+BAR_HEIGHT+UPPER_SUBWINDOW_HEIGHT, LEFT_SUBWINDOW_WIDE+BAR_WIDE, pathOfRight);
            break;
    }
    attroff(A_STANDOUT | A_UNDERLINE);

    while (1) {
        attron(COLOR_PAIR(SELECTED));

        //this is reset loop
        for (int i = 0; i < totalCount; ++i) {
            mvprintw(y + i, x, nullStr);
        }
        if (srcAry == NULL) {
            break;
        }

        //print files using loop
        if (aryCount - startPoint < totalCount){
            int k = 0;
            for (int i = startPoint; i < aryCount; ++i) {
                attron(COLOR_PAIR(SELECTED));
                if (selected[i] != 0) {
                    attron(COLOR_PAIR(CHECKED));
                }
                if (cursor == i) {
                    attron(A_STANDOUT | A_UNDERLINE);
                }
                mvprintw(y + k, x, srcAry[i]);
                k++;
                if (cursor == i) {
                    attroff(A_STANDOUT | A_UNDERLINE);
                }
            }
        }else{
            int k=0;
            for (int i = startPoint; i < (startPoint+totalCount); ++i) {
                attron(COLOR_PAIR(SELECTED));
                if (selected[i] != 0) {
                    attron(COLOR_PAIR(CHECKED));
                }
                if (cursor == i) {
                    attron(A_STANDOUT | A_UNDERLINE);
                }
                mvprintw(y + k, x, srcAry[i]);
                k++;
                if (cursor == i) {
                    attroff(A_STANDOUT | A_UNDERLINE);
                }
            }
        }

//         mvprintw(1,1,"ENTER입력");  
		refresh();
		
        key = getchar();

        switch (key) {
            case KEYBOARD_UP:
                if (cursor == startPoint) {
                    if (cursor > 0) {
                        cursor--;
                        startPoint--;
                    }
                } else if ((cursor > startPoint) && (cursor <= startPoint+totalCount)) {
                    cursor--;
                }
                break;
            case KEYBOARD_DOWN:
                if (cursor == startPoint + totalCount -1){
                    if (cursor < aryCount-1){
                        cursor++;
                        startPoint++;
                    }
                }
                else if ((cursor >= startPoint) && (cursor < startPoint+totalCount)) {
                    cursor++;
                }

                break;
				/*
            case KEYBOARD_LEFT:
 				mvprintw(1,1,"LEFT입력");
				refresh();
				getchar();
                memset(selected, 0, aryCount);
                selected[0]=cursor;//this is insert
				*selectedArrPtr=selected;
                return MENU_OUT_DIR;
				*/
            case KEYBOARD_RIGHT:
 				//mvprintw(1,1,"ENTER입력");
				//refresh();
                memset(selected, 0, aryCount);
 				//mvprintw(2,1,"memset완료");
				//refresh();
			    selected[0]=cursor;//this is insert
 				//mvprintw(3,1,"selected[0]=%d",selected[0]);
				//refresh();
				*selectedArrPtr=selected;
				if(selectingMenu==MENU_FIRSTWINODW)
					return MENU_INTO_MYDIR;
				else if(selectingMenu==MENU_SECONDWINDOW)
					return MENU_INTO_SERVDIR;
				break;
            case KEYBOARD_SPACEBAR:
                if (selected[cursor] == 0) {
                    selected[cursor] = 1;
					selectedCnt++;
                } else {
                    selected[cursor] = 0;
					selectedCnt--;
                }
				*selectedArrPtr=selected;
                break;
            case KEYBOARD_BACKSPACE:
				*selectedArrPtr=selected;


                return MENU_FTP_PAGE;
            case EXIT_KEY:
				*selectedArrPtr=selected;
				  return MENU_FTP_PAGE; // 4분할 화면 중 하나에 있을 시 , exit키는 ftp_page로 이동
               // return MENU_EXIT;
            case COPY_FILE_KEY:
	
				  {	int i;
					int k;
					int cnt=0;
				  	char * resultAry[100];
					char *destPath=".";
					for(i=0; i<aryCount; i++){
						if(selected[i]!=0){
							resultAry[k]=srcAry[i];
							k++;
						}
					}
					while(1){
						if(resultAry[cnt]==NULL){break;}
						else{cnt++;}
					}
					getDestPath(destPath,msg,"복사할 경로를 입력하세요: ");
					doCopy(resultAry, cnt+1,destPath,msg);
					freeDestPath(destPath,msg);
					free(selected);
				  }

				*selectedArrPtr=selected;
                return COPY_FILE_KEY;

            case MOVE_FILE_KEY:
				  {	int i;
					int k;
					int cnt=0;
				  	char * resultAry[100];
					char *destPath=".";
					for(i=0; i<aryCount; i++){
						if(selected[i]!=0){
							resultAry[k]=srcAry[i];
							k++;
						}
					}
					while(1){
						if(resultAry[cnt]==NULL){break;}
						else{cnt++;}
					}
					getDestPath(destPath,msg,"이동할 경로를 입력하세요: ");
					doMove(resultAry, cnt+1,destPath,msg);
					freeDestPath(destPath,msg);
					free(selected);
				  }

				*selectedArrPtr=selected;
                return MOVE_FILE_KEY;

            case REMOVE_FILE_KEY:
				  {	int i;
					int k;
					int res;
					int cnt=0;
				  	char * resultAry[100];
					char *destPath=".";
					for(i=0; i<aryCount; i++){
						if(selected[i]!=0){
							resultAry[k]=srcAry[i];
							k++;
						}
					}
					while(1){
						if(resultAry[cnt]==NULL){break;}
						else{cnt++;}
					}
					res=doRemove(resultAry, cnt+1,msg);
					if(res==0){break;}

					free(selected);
				  }

				*selectedArrPtr=selected;
				return MENU_FTP_PAGE;

            case RENAME_FILE_KEY:
				  {	int i;
					int k;
					int res;
					int cnt=0;
				  	char * resultAry[100];
					char *destPath=".";
					for(i=0; i<aryCount; i++){
						if(selected[i]!=0){
							resultAry[k]=srcAry[i];
							k++;
						}
					}
					while(1){
						if(resultAry[cnt]==NULL){break;}
						else{cnt++;}
					}
					//getDestPath(destPath,msg,"insert new file or dir name: ");
					res=doRename(resultAry, cnt+1,destPath,msg);
					if(res==0){break;}
					//freeDestPath(destPath,msg);
					free(selected);
				  }

				*selectedArrPtr=selected;
                return MENU_FTP_PAGE;

            case IP_MANAGE_KEY:
				*selectedArrPtr=selected;
                return MENU_IP_MANAGE;
            case MOVE_DIR_KEY:
				*selectedArrPtr=selected;
                return MOVE_DIR_KEY;
            case REMOVE_DIR_KEY:
				*selectedArrPtr=selected;
                return REMOVE_DIR_KEY;
            case RENAME_DIR_KEY:
				*selectedArrPtr=selected;
                return RENAME_DIR_KEY;
            case HELP_KEY:
				*selectedArrPtr=selected;
                return MENU_HELP;

            case MAKE_DIR_KEY:
				{
					doMkdir(msg);
				}
				*selectedArrPtr=selected;
                //return MAKE_DIR_KEY;
				//return MENU_FIRSTWINODW;
				return MENU_FTP_PAGE;
        }
    }

	*selectedArrPtr=selected;
    return MENU_FTP_PAGE;
}
