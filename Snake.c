#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>  //控制台IO 为了使用getch() 输入字符不回显到屏幕上

#define Height 22
#define Width 22
char mainboard[Height][Width]; // 背景板
char snake[2][100];
int x1,y1;


// 利用随机函数产生一个食物，坐标为x1 y1 然后在背景坐标中标识$
void generate_food(){

    srand(time(0));
    do
    {
        x1=rand()%Width-2+1;
        y1=rand()%Height-2+1;
    }
    while(mainboard[x1][y1]!=' ');
    mainboard[x1][y1]='$';
}

// 背景板部分 把四周用#围起来，然后调用generate_food函数来产生一个食物
void board_init(){
    int i=1,j=1;
    for(;i<=Height-2;i++){
        for(;j<=Width-2;j++)
            mainboard[i][j] = ' ';
    }

    for(i=0; i<=Height-1;i++){
        mainboard[i][0] = '#';
        mainboard[i][Width-1] = '#';
    }
    for(j=1;j<=Width-2;j++){
        mainboard[0][j] = '#';
        mainboard[Height-1][j] = '#';
    }
    generate_food();
}

// 将字符数组打印在屏幕上，这个函数会被snake_move函数调用 每次刷新后打印不同的数值
void print_to_Screen(int grade, int score, int gamespeed){
    system("cls");
    printf("\n");
    for(int i=0; i < Height; i++)
    {
        printf("\t");
        for(int j=0;j<Width;j++)
            printf("%c ",mainboard[i][j]);

        if(i==0) 
        printf("\tGrade:%d",grade);
        if(i==2) 
        printf("\tScore:%d",score);
        if(i==4) 
        printf("\tAutomatic forward");
        if(i==5) 
        printf("\ttime interval:%d ms",gamespeed);
        printf("\n");
    }

}

void snake_init(){

    long start;

    // 这一段for循环是为了展示出倒计时的效果 不要也无所谓
    printf("\n\n\t\tThe game is about to begin!\n");
    for(int i=3; i>=0;i--){
        // clock()为time.h里函数，获得当前时间
        start = clock();
        // 等待1s
        while(clock() - start <= 100){
            ;
        }
        // 对Windows命令行发送一个命令cls 清屏
        system("cls");

        if(i > 1)
        printf("\n\n\t\tCountdown:%d\n",i);
    }
    // 画出一条蛇 长度为4 头部为@
    for(int i = 1; i <= 3; i++){
        mainboard[1][i] = '*'; 
    }
    mainboard[1][4] = '@';

    for(int i = 0; i < 4; i++){
        snake[0][i] = 1;
        snake[1][i] = i + 1;
    }

}

void snake_move(){
    int head,tail,grade,score,gamespeed,length,timeover,x,y;
    char direction;
    long start;

    score=0; // 分数
    head = 3,tail = 0; // 头与尾
    grade = 1, length = 4; // 初始阶段和蛇的长度
    gamespeed = 500; // 刷新屏幕时间
    direction = 77; // 这是输入->对应的扫描码 snake的初始方向为右边
    // 死循环 接受输入
    while(1){
        timeover = 1;
        start = clock();
        // 在时间没有过去gamespeed且没有敲击键盘时 进入循环，所以在时间超时或者敲击键盘会从循环跳出
        while((timeover = (clock() - start <= gamespeed)) && !kbhit()){
            ;
        }
        // 当在时间范围内输入上下左右时，才会通过getch函数获得输入的上下左右的扩展码，这里需要调用两次getch 因为C语言在输入不是ASCII码 
        // 要第二次getch才会返回扩展码
        if(timeover){
            getch();
            direction = getch(); // 获得方向键扩展码
        }
        // 分别是按下 上下左右时得到头部新坐标 snake二维数组里存的就是蛇的各个段的行列坐标 snake[0] 存的是蛇的行坐标 从tail到head，snake[1]存的是列坐标
        switch(direction){
            case 72: {
                x = snake[0][head] - 1;
                y = snake[1][head];
                break;
            }
            case 80: {
                x = snake[0][head] + 1;
                y = snake[1][head];
                break;                
            }
            case 75: {
                x = snake[0][head];
                y = snake[1][head] - 1;
                break;                
            }
            case 77: {
                x = snake[0][head];
                y = snake[1][head] + 1;
                break;
            }
        }
        // 撞到边界 退出死循环
        if(x == 0 || x == 21 || y == 0 || y == 21){
            printf("\tGame over!\n");
            break;
        }
        // 头部撞到了自己的身体，也就是头部前进遇到的那个字符不是' ' 也不是食物 x1 y1 为食物坐标
        if(mainboard[x][y] == '*'){
            if((x==x1 && y==y1)){
                ;
            }else{
            printf("\tGame over!\n");
            break;   
            }
        
        }
        // 吃到食物
        if(x == x1 && y == y1){
            length++;
            score=score+100;
            // 如果长度到了8 进入下一阶段游戏，也就是过关了 此时游戏速度加快
            if(length>=8)
            {
                length -= 8;
                grade ++;
                // 游戏速度加快 但是最快不小于2s
                if(gamespeed>=200)
                    gamespeed = 550 - grade * 50;
            }
            // 原来食物位置变为头部@  原来头部位置变为蛇身体*
            mainboard[x][y] = '@';
            mainboard[snake[0][head]][snake[1][head]] = '*';
            // head++ 表示头部位置前进了 tail保持，在吃到食物这一步，尾部保持不变
            head = (head+1)%100;
            snake[0][head] = x;
            snake[1][head] = y;
            generate_food();
            print_to_Screen(grade,score,gamespeed);
        }
        else{
            // 当没吃到食物 原尾部坐标位置变为' ' 新尾部坐标++
            mainboard[snake[0][tail]][snake[1][tail]] = ' ';
            tail = (tail + 1) % 100;
            // 没迟到食物 原头部坐标变为蛇身体 头部坐标++
            mainboard[snake[0][head]][snake[1][head]] = '*';    
            head = (head + 1) % 100;
            // head有+1 所以要把新的头部坐标更新 tail的+1是继承原尾部前驱的坐标，所以不需要更新
            snake[0][head] = x;
            snake[1][head] = y;
            // 新头部
            mainboard[snake[0][head]][snake[1][head]] = '@';
            print_to_Screen(grade,score,gamespeed);
        
        }
    }

}

int main(){
    board_init();
    snake_init();
    snake_move();
    getchar();
}