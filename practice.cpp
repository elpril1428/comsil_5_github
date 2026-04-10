#include <bits/stdc++.h>
using namespace std;

void dijkstra(vector<long long>& dist, const vector<vector<pair<int, int>>>& road) {
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> q;

    q.push({0, 1});
    while (!q.empty()) {
        auto [current_dist, curr] = q.top();
        q.pop();

        if (dist[curr] < current_dist) continue;

        for (const auto& [next, weight] : road[curr]) {
            if (dist[curr] + weight < dist[next]) {
                dist[next] = dist[curr] + weight;
                q.push({dist[next], next});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    int N, M, C;
    cin >> N >> M >> C;
    vector<vector<pair<int, int>>> road(N+1);
    
    long long total = 0; 
    for (int i = 0; i < M; i++){
        int A, B, D;
        cin >> A >> B >> D;
        road[A].push_back({B, D});
        road[B].push_back({A, D});
        total += D;
    }

    vector<long long> dist(N+1, LLONG_MAX);
    dist[1] = 0;
    dijkstra(dist, road);

    vector<pair<long long, int>> nodes;
    for (int i = 1; i <= N; i++) {
        if (dist[i] != LLONG_MAX) {
            nodes.push_back({dist[i], i});
        }
    }
    sort(nodes.begin(), nodes.end());

    long long cost = total;
    long long curr_sum = 0;
    vector<bool> subway(N+1, false);

    for (int i = 0; i < nodes.size(); ){
        long long X = nodes[i].first;

        if (cost <= C * X) break;

        while (i < nodes.size() && nodes[i].first == X){
            int temp = nodes[i].second;
            subway[temp] = true;

            for (const auto& edge: road[temp])
                if (subway[edge.first]) curr_sum += edge.second;

            i++;
        }

        cost = min(cost, total - curr_sum + C * X);
    }

    cout << cost;
}