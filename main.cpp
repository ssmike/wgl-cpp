#include <map>
#include <iostream>
#include <memory>
#include <bitset>
#include <memory>
#include <vector>
#include <sstream>
#include <list>
#include <limits>
#include <unordered_set>

using namespace std;

const size_t max_string_len = 200;
const int max_int = numeric_limits<int>::max();

vector<int> read_list() {
    string s;
    getline(cin, s);
    int opens = s.find("[");
    int closes = s.find("]");
    s = s.substr(opens + 1, closes - opens - 1);
    stringstream is(s);
    vector<int> res;
    int buf;
    while (is >> buf) {
        res.push_back(buf);
    }
    return res;
}

struct InvokeOp {
    int index;
    int max_index;
    vector<int> transitions;
};

using Graph = vector<vector<int>>;

using Sequential = list<InvokeOp>;

using SequentialEntry = list<InvokeOp>::iterator;

Sequential make_sequential(vector<InvokeOp> history) {
    list<InvokeOp> result;
    for (auto& i: history) {
        result.push_back(i);
    }
    result.push_back({numeric_limits<int>::max(), max_int, {}});
    return result;
}

//struct ExplorationState {
//    SequentialEntry& history;
//    SequentialEntry entry;
//    int state;
//    int max_index;
//};

bool explore(
        const Graph& G,
        Sequential& history,
        SequentialEntry entry,
        int state,
        int max_index) {
    if (history.empty() || entry == history.end()) {
        return true;
    }
    if (entry->index > max_index) {
        return false;
    }
    for (auto t: entry->transitions) {
        if (G[state][t] != -1) {
            SequentialEntry saved_pos = next(entry);
            InvokeOp saved_op = *entry;
            history.erase(entry);
            if (explore(G, history, history.begin(), G[state][t], max_int)) {
                return true;
            }
            history.insert(saved_pos, saved_op);
        }
    }
    return explore(G, history, next(entry), state, min(max_index, entry->max_index));
}

bool explore(const Graph& G, Sequential& history, int state) {
    return explore(G, history, history.begin(), state, max_int);
}

int main() {
    int state;
    cin >> state;

    int transitions, states;
    cin >> states >> transitions;
    int edges;
    cin >> edges;

    Graph G;
    G.resize(states);
    for (auto& i: G) {
        i.assign(transitions, -1);
    }
    {
        string buf;
        getline(cin, buf);
        cerr << "skipped " << buf << endl;
    }

    for (int i = 0; i < edges; i++) {
        auto v = read_list();
        G[v[0]][v[1]] = v[2];
    }
    int history_size;
    cin >> history_size;
    map<int, int> last_op;

    vector<InvokeOp> history;
    for (int i = 0; i < history_size; i++) {
        int type;
        cin >> type;
        int process;
        cin >> process;
        auto value = read_list();
        if (type == 0) {
            history.push_back({i, -1, value});
            last_op[process] = history.size() - 1;
        } else {
            history[last_op[process]].max_index = i;
        }
    }
    Sequential seq_history = make_sequential(history);
    return !explore(G, seq_history, state);
}
