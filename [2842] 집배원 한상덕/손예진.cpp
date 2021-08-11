#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#define MAX 1000001

using namespace std;

int sy, sx, N, alt[51][51];
int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dx[] = { -1, 1, 0, -1, 1, -1, 1, 0 };
int must[2];
vector<string> map(51);
vector<int> height;
vector<pair<int, int>> k;
int ans = MAX, l, r;
bool visited[51][51];

//가능한 고도 범위 설정 (최소와 최대) -> 범위에 해당하는 고도값을 가진 위치로만 이동하도록
//모든 K와 P는 반드시 거쳐야 하기 때문에 최소, 최대 범위 설정시 해당 범위를 벗어나지 않도록 지정.
// **가능한 최단경로를 찾는 문제가 아님. 따라서 이미 방문한 곳을 재방문하더라도 최소한의 고도 차이로만 이동하면 됨.
// ->연속된 이동 경로를 찾지 않고 점 찍듯이 필요한 위치로만 이동하도록 구현함.

void dfs(int y, int x) {
	for (int i = 0; i < 8; i++) {
		int ny = y + dy[i];
		int nx = x + dx[i];
		if (ny < 0 || nx < 0 || ny >= N || nx >= N || visited[ny][nx])	//이동 가능 여부 확인
			continue;
		if (alt[ny][nx] < height[l] || alt[ny][nx] > height[r])	//지정한 최소, 최대 범위 내인지 확인
			continue;
		visited[ny][nx] = true;
		dfs(ny, nx);
	}
}

bool Check() {  //모든 '집'을 방문하였는지 
	for (int i = 0; i < k.size(); i++) {
		if (!visited[k[i].first][k[i].second])
			return false;
	}
	return true;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);
	cin >> N;

	for (int i = 0; i < N; i++) {
		cin >> map[i];
	}
	must[0] = MAX;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> alt[i][j];
			height.push_back(alt[i][j]);
			if (map[i][j] == 'P' || map[i][j] == 'K') {
				//반드시 방문해야하는 고도(우체국, 집)의 최솟값, 최댓값
				must[0] = min(alt[i][j], must[0]);
				must[1] = max(alt[i][j], must[1]);
				switch (map[i][j])
				{
				case 'P':
					sy = i;
					sx = j;
					break;
				case 'K':
					k.push_back({ i, j });
					break;
				}
			}
		}
	}

	sort(height.begin(), height.end());
	height.erase(unique(height.begin(), height.end()), height.end());
	int size = height.size();
	l = r = 0;

	//최소한 must[0]와 must[1]는 포함시켜야됨
	//right >= must[1] && left <= must[0]
	while (r < size) {
		while (l <= r) {
			if (height[l] > must[0]) {  //must[0]은 범위에 반드시 포함되어야 함.
				break;
			}
			if (height[r] < must[1]) {  //must[1]은 범위에 반드시 포함되어야 함.
				break;
			}
			memset(visited, false, sizeof(visited));
			visited[sy][sx] = true;
			dfs(sy, sx);

			if (Check()) {
				ans = min(ans, height[r] - height[l++]);
			}
			else
				break;
		}
		r++;
	}
	cout << ans << "\n";
}
