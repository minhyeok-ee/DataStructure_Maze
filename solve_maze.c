// 2차원 미로에서 출발->도착
// 경로를 찾아가는 프로그램

#include <stdio.h>

#define SZ 7
#define STACK_SZ (SZ*SZ)

// 좌표 정보를 나타내는 구조체
struct COORD {
   int row;
   int col;
};

// 스택을 정의
struct COORD path_stack[STACK_SZ];
int top = -1;

int maze[SZ][SZ] =
{
   {0, 1, 0, 1, 0, 0, 0},
   {0, 1, 0, 1, 0, 1, 0},
   {0, 0, 0, 1, 0, 1, 1},
   {0, 1, 1, 1, 0, 1, 0},
   {0, 1, 0, 0, 0, 0, 0},
   {0, 1, 0, 1, 0, 1, 0},
   {0, 0, 0, 1, 0, 1, 0}
};

// 방문했던 곳을 저장하는 배열
struct COORD visited[SZ * SZ];
int visitedIndex = -1;


// stack 관련 함수 
// push
// pop
// isStackEmpty
// isStackFull
// peek (pop과 유사하지만 실제로 꺼내지 않고 보기만한다.)

int isStackEmpty(void) {
   return (top == -1);
}

int isStackFull(void){
   return (top == STACK_SZ - 1);
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
      return result; // stack이 비었음
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


// 좌표 _c를 방문했었는지 판단
// 했었으면 non-zero
// 아니면 zero
int checkVisited(struct COORD _c) {
   for (int i = 0; i <= visitedIndex; i++) {
      if ((visited[i].row == _c.row) && (visited[i].col == _c.col)) {
         return 1;
      }
   }
   return 0;
}

void addToVisited(struct COORD _c) {

   // 있는지를 검사한다.
   //    없으면, 추가한다. 이때 visitedIndex를 먼저 증가시킨다.
   if (checkVisited(_c) == 0) {
      visitedIndex++; //이게 중요,, 먼저 증가
      visited[visitedIndex] = _c;
   }
}

// _c가 _dst와 같은지 판단.
// 같으면 non-zero을 반환, 아니면 0을 반환
int checkDestination(struct COORD _c, struct COORD _dst) {
   return ((_c.row == _dst.row) && (_c.col == _dst.col));
}

// _c가 영역 내에 있으면 1,  (0 ~ SZ-1)
// 아니면 0
int isWithinMap(struct COORD _c) {
   return ((_c.row >= 0) && (_c.row < SZ) && (_c.col >= 0) && (_c.col < SZ));
}

int isWall(struct COORD _c) {
   return (1 == maze[_c.row][_c.col]);
}


// _c를 기준으로 갈 수 있는 좌표 1곳을 반환
// 만약 없으면, (-1,-1)을 반환
struct COORD findWhereToGo(struct COORD _c) {

   // 체크할 방향을 저장하는 변수
   struct COORD target;

   // 위
   target.row = _c.row - 1;
   target.col = _c.col;
   // 1. 지도 영역 내인가?
   // 2. 벽이냐?
   // 3. 가본 적 있냐?
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


// _s에서 _d까지 경로를 출력하는 함수
void findPath(struct COORD _s, struct COORD _d) {

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
            for (int i = 0; i <= top; i++){
               printf("(%d, %d)\n", path_stack[i].row, path_stack[i].col);
            }
            printf("목적지 도착했음. (%d, %d)\n", current.row, current.col);
            return;
         }
      }
      else {

         while(1){
            //top_coord <= stack의 맨 위에 있는 곳을 peek (pop아님)
            struct COORD top_coord = peek();

            // top_coord (-1,-1) : 스택이 비었음. 돌아갈 곳이 없음
            if ((top_coord.row == -1) && (top_coord.col == -1)){
               printf("경로가 없습니다.\n");
               return;
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

int main(void) {

   struct COORD start_point = { 0, 0 };
   struct COORD dest_point = { 6, 6 };

   // start_point ---> dest_point까지 경로 표시
   findPath(start_point, dest_point);

   return 0;
}