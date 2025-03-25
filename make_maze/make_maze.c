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
    struct COORD current = _s;
    addToVisited(current);

    while (1) {
        struct COORD whereToGo = findWhereToGo(current);

        if (whereToGo.row != -1 && whereToGo.col != -1) {
            push(current);
            current = whereToGo;
            addToVisited(current);

            if (checkDestination(current, _d)) {
                push(current);  // 도착 지점도 경로에 포함
                return 1;
            }
        } else {
            while (1) {
                struct COORD top_coord = peek();

                if (top_coord.row == -1 && top_coord.col == -1) {
                    return 0;  // 더 이상 갈 곳 없음
                }

                whereToGo = findWhereToGo(top_coord);

                if (whereToGo.row == -1 && whereToGo.col == -1) {
                    pop();  // 막다른 길
                } else {
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

    // 미로 메모리 할당 및 초기화
    maze = (int**)malloc(SZ * sizeof(int*));
    for (int i = 0; i < SZ; i++) {
        maze[i] = (int*)malloc(SZ * sizeof(int));
        for (int j = 0; j < SZ; j++) {
            maze[i][j] = 0;
        }
    }

    struct COORD start_point = { 0, 0 };
    struct COORD dest_point = { SZ - 1, SZ - 1 };

    srand(time(NULL));

    int totalCells = SZ * SZ;
    int raw = totalCells * 70;
    int wallTarget = raw / 100;
    if (raw % 100 != 0) wallTarget++;

    int wallCount = 0;
    int max_attempts = 1000;
    int attempts = 0;

    while (wallCount < wallTarget && attempts < max_attempts) {
        int r = rand() % SZ;
        int c = rand() % SZ;

        if ((r == 0 && c == 0) || (r == SZ - 1 && c == SZ - 1)) continue;
        if (maze[r][c] == 1) continue;

        maze[r][c] = 1;

        top = -1;
        visitedindex = -1;
        for (int i = 0; i < 900; i++) {
            visited[i].row = -1;
            visited[i].col = -1;
        }

        if (findPath(start_point, dest_point)) {
            wallCount++;
            attempts = 0;
        } else {
            maze[r][c] = 0;
            attempts++;
        }
    }

    // 최종 경로 다시 계산해서 path_stack에 저장
    top = -1;
    visitedindex = -1;
    for (int i = 0; i < 900; i++) {
        visited[i].row = -1;
        visited[i].col = -1;
    }
    findPath(start_point, dest_point);

    // 미로 출력
    printf("\n생성된 미로 (0: 길, 1: 벽):\n");
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            printf("%d ", maze[i][j]);
        }
        printf("\n");
    }

    // 통계 출력
    printf("\n미로 크기: %d x %d\n", SZ, SZ);
    printf("벽 개수: %d개 (%.2f%%)\n", wallCount, (100.0 * wallCount) / (SZ * SZ));

    if ((100.0 * wallCount) / (SZ * SZ) < 70.0) {
        printf("충분한 벽이 생성되지 않은은 미로입니다.\n");
    } else {
        printf("벽이 70%%넘게 생성된 유효한 미로입니다.\n");
    }

    // 경로 출력
    printf("\n탈출 경로: ");
    for (int i = 0; i <= top; i++) {
        printf("(%d,%d)", path_stack[i].row, path_stack[i].col);
        if (i != top) printf(" -> ");
    }
    printf("\n");

    // 메모리 해제
    for (int i = 0; i < SZ; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}
