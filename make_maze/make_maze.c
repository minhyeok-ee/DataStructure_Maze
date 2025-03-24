#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int SZ;
int** maze;


struct COORD {
    int row;
    int col;
};

struct COORD path_stack[900];
int top = -1;

struct COORD visited[900];
int visitedindex = -1;

//////////////////////////////////////////////////////////////////
// 스택 관련 함수

int isStackEmpty(void) {
   return (top == -1);
}

int isStackFull(void){
   return (top == 899);
}

void push(struct COORD _c) {
   if (isStackFull()){
      return;
   }
   top += 1;
   path_stack[top] = _c;
   return;
}

struct COORD pop(void){
   struct COORD result = {-1, -1};
   if(isStackEmpty()){
      return result;
   }
   result = path_stack[top];
   top -= 1;
   return result;
}

struct COORD peek(void) {
   struct COORD result = {-1, -1};

   if(isStackEmpty()){
      return result;
   }
   result = path_stack[top];
   return result;
}

// 스택 관련 함수
//////////////////////////////////////////////////////////////////
// 방문한 좌표인지 확인

int checkVisited(struct COORD _c) {
   for (int i = 0; i <= visitedindex; i++) {
      if ((visited[i].row == _c.row) && (visited[i].col == _c.col)) {
         return 1;
      }
   }
   return 0;
}

// 방문한 좌표인지 확인
//////////////////////////////////////////////////////////////////
// 방문 좌표에 추가

void addToVisited(struct COORD _c) {

   if (checkVisited(_c) == 0) {
      visitedindex++;
      visited[visitedindex] = _c;
   }
}

// 방문 좌표에 추가
//////////////////////////////////////////////////////////////////
// 도착지인지 확인

int checkDestination(struct COORD _c, struct COORD _dst) {
   return ((_c.row == _dst.row) && (_c.col == _dst.col));
}

// 도착지인지 확인
//////////////////////////////////////////////////////////////////
// 갈 수 있는 경로인지 확인

int isWithinMap(struct COORD _c) {
   return ((_c.row >= 0) && (_c.row < SZ) && (_c.col >= 0) && (_c.col < SZ));
}

int isWall(struct COORD _c) {
   return (1 == maze[_c.row][_c.col]);
}

// 갈 수 있는 경로인지 확인
//////////////////////////////////////////////////////////////////
// 다음 경로 탐색. 없으면 (-1,-1)

struct COORD findWhereToGo(struct COORD _c) {

   // 체크할 방향을 저장하는 변수
   struct COORD target;

   // 위
   target.row = _c.row - 1;
   target.col = _c.col;
   if (isWithinMap(target) == 1 && isWall(target) == 0 &&
      checkVisited(target) == 0) {
      return target;
   }
   // 아래
   target.row = _c.row + 1;
   target.col = _c.col;
   if (isWithinMap(target) == 1 && isWall(target) == 0 &&
      checkVisited(target) == 0) {
      return target;
   }
   // 왼쪽
   target.row = _c.row;
   target.col = _c.col-1;
   if (isWithinMap(target) == 1 && isWall(target) == 0 &&
      checkVisited(target) == 0) {
      return target;
   }
   // 오른쪽
   target.row = _c.row;
   target.col = _c.col + 1;
   if (isWithinMap(target) == 1 && isWall(target) == 0 &&
      checkVisited(target) == 0) {
      return target;
   }

   target.row = -1;
   target.col = -1;
   return target;
}

// 다음 경로 탐색. 없으면 (-1,-1)
//////////////////////////////////////////////////////////////////
// _s 부터 _d 까지 경로 출력(메인)

int findPath(struct COORD _s, struct COORD _d) {

   // 현재 위치를 설정
   struct COORD current = _s;

   // 출발 좌표를 저장
   addToVisited(current);

   while (1) {
      // 현재 위치에서 가는 곳을 찾는다.
      struct COORD whereToGo = findWhereToGo(current);

      if ((whereToGo.row != -1) && (whereToGo.col != -1)) {
         // 갈 곳이 있음
         push(current); // 경로를 저장해둔다.
         current = whereToGo;   // current는 항상 현재 위치를 나타내는 역할
         addToVisited(current); // 내가 여기 왔다고 표시,,,
         if (checkDestination(current, _d) == 1) {
            // 끝...도착했음
            // stack에 들어있는 경로를 모두 출력

            // 이 부분은 출력이므로 이따가 필요하면 쓸게요
            // for (int i = 0; i <= top; i++){
            //    printf("(%d, %d)\n", path_stack[i].row, path_stack[i].col);
            // }
            // printf("목적지 도착했음. (%d, %d)\n", current.row, current.col);
            return 1;
         }
      }
      else {

         while(1){
            //top_coord <= stack의 맨 위에 있는 곳을 peek (pop아님)
            struct COORD top_coord = peek();

            // top_coord (-1,-1) : 스택이 비었음. 돌아갈 곳이 없음
            if ((top_coord.row == -1) && (top_coord.col == -1)){
               return 0;
            }
            whereToGo = findWhereToGo(top_coord);
         
            if ((whereToGo.row == -1) && (whereToGo.col == -1)){
               // findwheretogo(top_coord) ==> (-1,-1)
               // 이 좌표 (top_coord)는 도움이 안됨.. 더 이전으로 돌아가야 함. 이 좌표는 버림(pop)
               pop(); // top_coord를 날려버림
            }

            else {
               // findwheretogo(top_coord) ==> 유효한 좌표가 나옴
               // current = wheretogo
               // push가 필요 없음.. 왜냐면 top_coord가 아직 스택에 있기 때문
               // addtovisited(current)
               current = whereToGo;
               addToVisited(current);
               break;
            }
         }
      }
   }
}

// _s 부터 _d 까지 경로 출력(메인)
//////////////////////////////////////////////////////////////////



int main() {
    printf("미로의 크기를 입력하세요 (5 ~ 30): ");
    scanf("%d", &SZ);

    if (SZ < 5 || SZ > 30) {
        printf("미로 크기는 5 이상 30 이하만 가능합니다.\n");
        return 1;
    }

    maze = (int**)malloc(SZ * sizeof(int*));
    for (int i = 0; i < SZ; i++) {
        maze[i] = (int*)malloc(SZ * sizeof(int));
    }

    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            maze[i][j] = 0;
        }
    }

    struct COORD start_point = { 0, 0 };
    struct COORD dest_point = { SZ-1, SZ-1 };

    srand(time(NULL));
    int totalCells = SZ * SZ;
    int raw = totalCells * 70;
    int wallTarget = raw / 100;
    if (raw % 100 != 0) wallTarget++;

    int wallCount = 0;

    // 경로가 사라지지 않게 벽 추가
    while (wallCount < wallTarget) {
        int r = rand() % SZ;
        int c = rand() % SZ;

        if ((r == 0 && c == 0) || (r == SZ-1 && c == SZ-1)) continue;
        if (maze[r][c] == 1) continue;

        maze[r][c] = 1;
        top = -1;
        visitedindex = -1;

        if (findPath(start_point, dest_point)) {
            wallCount++;
        } else {
            maze[r][c] = 0;
        }

        // 모든 공간을 시도해봤는데 더 이상 추가할 수 없을 경우 대비
        int possible = 0;
        for (int i = 0; i < SZ; i++) {
            for (int j = 0; j < SZ; j++) {
                if ((i == 0 && j == 0) || (i == SZ-1 && j == SZ-1)) continue;
                if (maze[i][j] == 0) {
                    maze[i][j] = 1;
                    top = -1;
                    visitedindex = -1;
                    if (findPath(start_point, dest_point)) {
                        possible = 1;
                    }
                    maze[i][j] = 0;
                    if (possible) break;
                }
            }
            if (possible) break;
        }
        if (!possible) break; // 더 이상 벽을 추가할 수 없음
    }

    // 남은 벽 강제로 채우기
    while (wallCount < wallTarget) {
        int r = rand() % SZ;
        int c = rand() % SZ;
        if ((r == 0 && c == 0) || (r == SZ-1 && c == SZ-1)) continue;
        if (maze[r][c] == 1) continue;
        maze[r][c] = 1;
        wallCount++;
    }

    printf("\n생성된 미로 (0: 길, 1: 벽):\n");
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }

    // 마지막 경로 존재 여부 검사
    top = -1;
    visitedindex = -1;
    if (findPath(start_point, dest_point)) {
        printf("\n이 미로는 유효합니다. 경로가 존재합니다.\n");
    } else {
        printf("\n경고 : 이 미로는 경로가 존재하지 않습니다.\n");
    }

    for (int i = 0; i < SZ; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}