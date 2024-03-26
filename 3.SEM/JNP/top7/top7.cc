#include <iostream>
#include <regex>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <sstream> // do wywalenia przy wysyłaniu (przetestować na studentsie)
#include <string>


using namespace std;

// Pair containing song info - song id and collected points,
using song_score = pair<uint32_t, uint32_t>;

// Map used to collect current ranking of songs,
// mapping song id's to collected points
using song_ranking = unordered_map<uint32_t, uint32_t>;

// Vector containing best 7 (podium) song's scores.
using podium = vector<song_score>;

// Parameters passed to top and new_max functions, contains song ranking,
// current podium and previous podium.
using params = tuple<song_ranking &, podium &, podium &>;

// Set containing id's of songs that fall out of new max ranking,
// that are not allowed to vote anymore.
using not_allowed_set = unordered_set<uint32_t>;

// Pair used for checking if the vote is valid, consists of
// set of not allowed votes and actual maximum song id that can be voted for.
using check_if_allowed_pair = pair<not_allowed_set &, uint32_t>;

const static uint32_t NUMBER_OF_PLACES = 7;
const static uint32_t MAX_POINTS = 7;

// Comparator used to sort podium.
static bool compare(const pair<uint32_t, uint32_t> &p1,
                    const pair<uint32_t, uint32_t> &p2) {
    return ((p1.second > p2.second) ||
            ((p1.second == p2.second) && (p1.first < p2.first)));
}

// Function used to update points for a song and update podium.
// Used both for NEW MAX and TOP rankings and podiums.
static void update_ranking(uint32_t song_id, uint32_t points, params &params) {
    song_ranking &ranking = get<0>(params);
    podium &act_podium = get<1>(params);

    ranking[song_id] += points;
    bool was_on_podium = false;
    for (auto &act_song: act_podium) {
        if (act_song.first == song_id) {
            was_on_podium = true;
            act_song.second += points;
        }
    }

    if (!was_on_podium) {
        act_podium.push_back({song_id, ranking[song_id]});
    }
    sort(act_podium.begin(), act_podium.end(), compare);
    if (act_podium.size() > NUMBER_OF_PLACES) {
        act_podium.pop_back();
    }
}

// Function used to print current podium and compare the position with previous.
// Used both for NEW MAX and TOP podiums.
void print_podium(params &params) {
    podium &act_podium = get<1>(params);
    podium &prev_podium = get<2>(params);

    for (uint32_t act_position = 0;
         act_position < act_podium.size(); act_position++) {
        uint32_t name = act_podium[act_position].first;
        cout << name << " ";
        uint32_t prev_position = 8;
        for (uint32_t last = 0; last < prev_podium.size(); last++) {
            uint32_t name2 = prev_podium[last].first;
            if (name == name2) {
                prev_position = last;
                break;
            }
        }
        if (prev_position == 8) {
            cout << "-\n";
        }
        else {
            cout << ((int32_t) prev_position - (int32_t) act_position) << "\n";
        }
    }
}

void new_max(params &new_max_params, params &top_params,
             check_if_allowed_pair &check_if_allowed) {
    song_ranking &new_max_ranking = get<0>(new_max_params);
    podium &act_new_max_podium = get<1>(new_max_params);
    podium &prev_new_max_podium = get<2>(new_max_params);
    not_allowed_set &not_allowed = get<0>(check_if_allowed);

    print_podium(new_max_params);

    for (uint32_t position = 0;
         position < act_new_max_podium.size(); position++) {
        update_ranking(act_new_max_podium[position].first,
                       MAX_POINTS - position, top_params);
    }

    for (song_score prev_song: prev_new_max_podium) {
        if (none_of(act_new_max_podium.begin(), act_new_max_podium.end(),
                    [prev_song](song_score song) {
                        return prev_song.first == song.first;
                    })) {
            not_allowed.insert(prev_song.first);
        }
    }

    new_max_ranking.clear();
    prev_new_max_podium = act_new_max_podium;
    act_new_max_podium.clear();
}

void top(params &top_params) {
    podium &act_top_podium = get<1>(top_params);
    podium &prev_top_podium = get<2>(top_params);

    print_podium(top_params);

    prev_top_podium = act_top_podium;
}

void print_error(pair<string, uint32_t> &input_line) {
    cerr << "Error in line " << input_line.second << ": " << input_line.first
         << "\n";
}

bool not_allowed_vote(const vector<uint32_t> &new_votes,
                      check_if_allowed_pair &check_if_allowed) {
    not_allowed_set &not_allowed = get<0>(check_if_allowed);
    uint32_t &act_max = get<1>(check_if_allowed);

    for (uint32_t vote: new_votes) {
        if (not_allowed.contains(vote) || vote > act_max || vote == 0) {
            return true;
        }
    }
    return false;
}

// The vote is good when there are no repetitions and if each song
// is allowed to be voted on.
bool good_vote(vector<uint32_t> &new_votes,
               check_if_allowed_pair &check_if_allowed) {
    sort(new_votes.begin(), new_votes.end());
    return (adjacent_find(new_votes.begin(), new_votes.end()) == new_votes.end()
            && !not_allowed_vote(new_votes, check_if_allowed));
}

// Function checks if given vote is correct and if so,
// passes it to update the ranking.
void parse_votes_line(pair<string, uint32_t> &input_line, params &new_max_params,
                 check_if_allowed_pair &check_if_allowed) {
    vector<uint32_t> new_votes;
    stringstream streams;
    streams << input_line.first;
    uint32_t vote;
    while (streams >> vote) {
        new_votes.push_back(vote);
    }

    if (!good_vote(new_votes, check_if_allowed)) {
        print_error(input_line);
        return;
    }

    for (uint32_t v: new_votes) {
        update_ranking(v, 1, new_max_params);
    }
}

void parse_new_max_line(const string &new_max_string, params &new_max_params,
                        params &top_params,
                        check_if_allowed_pair &check_if_allowed) {
    uint32_t &act_max = get<1>(check_if_allowed);

    stringstream streams;
    streams << new_max_string;
    uint32_t new_new_max;
    streams >> new_new_max;
    act_max = new_new_max;
    new_max(new_max_params, top_params, check_if_allowed);
}

bool empty_line(const string &input_line, const regex &whitespace_regex) {
    return regex_match(input_line, whitespace_regex);
}

// Function checks if the input is correct
// and in each case proceeds with the appropriate action.
void parse_input_and_process(params &new_max_params, params &top_params,
                             check_if_allowed_pair &check_if_allowed) {
    const regex whitespace_regex("^\\s*$");
    const regex vote_regex_check(R"(^(\s*(\d{1,8}\s+)*\d{1,8}\s*)$)");
    const regex new_max_regex(R"(^(?:\s*NEW\s*(\d{1,8})\s*)$)");
    const regex top_regex("^(?:\\s*(TOP)\\s*)$");
    smatch match;

    pair<string, uint32_t> input_line;
    input_line.second = 1;

    while (getline(cin, input_line.first)) {
        if (!empty_line(input_line.first, whitespace_regex)) {
            if (regex_match(input_line.first, vote_regex_check)) {
                parse_votes_line(input_line, new_max_params, check_if_allowed);
            }
            else if (regex_search(input_line.first, match, new_max_regex)) {
                parse_new_max_line(match.str(1), new_max_params, top_params,
                                   check_if_allowed);
            }
            else if (regex_search(input_line.first, match, top_regex)) {
                top(top_params);
            }
            else {
                print_error(input_line);
            }
        }
        input_line.second++;
    }
}

int main() {
    song_ranking new_max_ranking;
    podium act_new_max_podium;
    podium prev_new_max_podium;

    params new_max_params = {new_max_ranking, act_new_max_podium,
                             prev_new_max_podium};

    song_ranking top_ranking;
    podium act_top_podium;
    podium prev_top_podium;

    params top_params = {top_ranking, act_top_podium, prev_top_podium};

    not_allowed_set not_allowed;
    check_if_allowed_pair check_if_allowed = {not_allowed, 0};

    parse_input_and_process(new_max_params, top_params, check_if_allowed);

    return 0;
}