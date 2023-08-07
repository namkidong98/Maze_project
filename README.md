# Maze_project
Final Project for Sogang Univ, 2022_2nd semester, CSE3013

<br/>

## maze_project: 제리가 치즈를 찾아 미로를 탈출하는 프로그램
![maze_animation](https://github.com/namkidong98/Maze_project/assets/113520117/20a9d197-43d0-4857-b25e-03cf601722d1)
#### 1. phase1을 기반으로, 완전 미로를 랜덤하게 생성하는 기능 탑재
#### 2. phase2를 기반으로, 윈도우 창에 미로를 출력
#### 3. phase3을 기반으로, DFS와 BFS를 활용하여 미로의 출구를 탐색
#### 추가사항1. 미로의 시작점에는 제리를, 미로의 출구에는 치즈를 그린다
#### 추가사항2. 미로의 시작점과 출구를 랜덤하게 배치할 수 있는 기능 추가
#### 추가사항3. DFS, BFS로 그려진 경로에 따라 이동하는 애니메이션 기능 추가
![maze_flowchar.png](https://github.com/namkidong98/Maze_project/blob/main/maze_project/maze_flowchart.png)


<br/>

## phase1: 완전 미로를 랜덤하게 만드는 프로그램
#### 1. maze.c: 완전 미로를 랜덤하게 만드는 기능을 구현
#### 2. read_maze.c: 주어진 형식의 .maz파일의 미로를 읽어서 출력
![phase1.png](https://github.com/namkidong98/Maze_project/blob/main/phase1/phase1.PNG)

<br/>

## phase2: OpenFramework를 이용하여 미로를 윈도우 창에 출력
#### 1. OpenFramework의 project Generator로 프로젝트를 만든다
#### 2. OpenFramework 폴더 - apps - myapps 폴더 안에 만들어진 프로젝트의 src와 bin을 주어진 폴더로 대체한다
#### 3. sln파일을 실행시키고 소스 파일에서 기존 항목 추가로 src의 파일들을 가져온다
#### 4. 실행시킨 후 /bin/data 안의 .maz파일을 열면 해당 파일에 저장된 미로가 윈도우 창에 출력된다
![phase2.png](https://github.com/namkidong98/Maze_project/blob/main/phase2/phase2.PNG)

<br/>

## phase3: DFS와 BFS를 이용하여 미로 탈출 경로를 윈도우 창에 출력
#### 1. OpenFramework의 project Generator로 프로젝트를 만든다
#### 2. OpenFramework 폴더 - apps - myapps 폴더 안에 만들어진 프로젝트의 src와 bin을 주어진 폴더로 대체한다
#### 3. sln파일을 실행시키고 소스 파일에서 기존 항목 추가로 src의 파일들을 가져온다
#### 4. 실행시킨 후 /bin/data 안의 .maz파일을 열면 해당 파일에 저장된 미로가 윈도우 창에 출력된다
![phase3_dfs.png](https://github.com/namkidong98/Maze_project/blob/main/phase3/phase3_dfs.PNG)
![phase3_bfs.png](https://github.com/namkidong98/Maze_project/blob/main/phase3/phase3_bfs.PNG)
