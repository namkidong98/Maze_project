####설치 및 실행 방법

1. OpenFramework의 project Generator로 프로젝트를 만든다
2. OpenFramework 폴더 - apps - myapps 폴더 안에 만들어진 프로젝트의 src와 bin을 주어진 폴더로 대체한다
3. sln파일을 실행시키고 소스 파일에서 기존 항목 추가로 src의 파일들을 가져온다
4. 실행시킨 후 /bin/data 안의 .maz파일을 열면 해당 파일에 저장된 미로가 윈도우 창에 출력된다

</br>

####상단 메뉴 별 기능 설명

1. Open : .maz 형식의 파일을 열어 화면에 출력한다
2. Make random maze : Height와 Width를 입력 받아 완전 미로인 maze.maz를 랜덤하게 생성한다
3. Exit : 프로그램을 종료한다
4. Show DFS : 시작점부터 출구까지 DFS로 탐색한 경로를 화면에 출력한다
5. Show BFS : 시작점부터 출구까지 BFS로 탐색한 경로를 화면에 출력한다
6. Full Screen : 전체 화면으로 전환한다
7. Clear : 화면에 출력한 BFS나 DFS 경로를 지우고 미로만 남긴다
8. Random ENTRANCE : 시작점을 우측 상단이 아닌 랜덤한 지점으로 변경한다
9. Random EXIT : 출구를 좌측 하단이 아닌 랜덤한 지점으로 변경한다
10. DFS Animation : DFS 경로가 그려진 것을 따라 제리가 치즈를 향해 움직인다
11. BFS Animation : BFS 경로가 그려진 것을 따라 제리가 치즈를 향해 움직인다
