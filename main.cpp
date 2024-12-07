#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

/*
Да се дефинира клас Избирателен район. Член – данните на класа са:
 1) име на района (указател към символен низ);
 2) брой гласоподаватели за района;
 3) брой партии, за които се гласува;
 4) динамичен масив, съдържащ броят на гласувалите за всяка партия;
 5) масив от символни низове, съдържащ имената на партиите (инициалите им).
Съставете програма, която:
 *	Създава файл от обекти - избирателни райони.
 *	Извежда имената на районите, в които eспечелила зададена партия, подредени по азбучен ред на името на района.
 *	Записва в нов файл имената на районите заедно с името на партията победител в тях (ако има такава).
 *	Извежда имената на районите, в които няма победител в изборите.
 */

class Constituency{
public:
    string name;
    unsigned int voters_number;
    unsigned int parties_number;
    unsigned int* votes;
    string* parties;

    bool has_winner = false;
    string* winner;

    Constituency(auto name, auto num_v, auto num_p, auto v, auto p) :
        name(name), voters_number(num_v), parties_number(num_p), votes(v), parties(p) {

        auto max_index = distance(votes, max_element(votes, votes + parties_number));
        if ((float)votes[max_index] / (float)voters_number > 0.5) {
            has_winner = true;
            winner = &parties[max_index];
        }
    }

    bool operator<(Constituency& o) const{
        return name < o.name;
    }
};

istream& operator>>(istream& is, Constituency& c){
    is >> c.name;
    is >> c.voters_number;
    is >> c.parties_number;
    for (auto i = 0u; i != c.parties_number; ++i) { is >> c.votes[i]; }
    for (auto i = 0u; i != c.parties_number; ++i) { is >> c.parties[i]; }

    return is;
}

ostream& operator<<(ostream& os, Constituency& c){
    os << c.name << endl;
    os << c.voters_number << endl;
    os << c.parties_number << endl;
    for (auto i = 0u, p = c.parties_number; i != p; ++i) {os << c.votes[i] << " "; }
    os << endl;
    for (auto i = 0u, p = c.parties_number; i != p; ++i) {os << c.parties[i] << " "; }
    os << endl << endl;

    return os;
}

void write_to_files(vector<Constituency>& constituencies) {
    ofstream CFile("Constituencies.txt");
    ofstream winnersFile("Winners.txt");

    for (auto c : constituencies){
        CFile << c;
        winnersFile << c.name << " - " << (c.has_winner ? *c.winner : "no winner") << endl;
    }

    CFile.close();
    winnersFile.close();
}

vector<Constituency> get_constituencies_by_winner(vector<Constituency>& consituencies, string party) {
    vector<Constituency> result;

    for (const auto& c : consituencies) {
        if (c.has_winner && party == *c.winner) {
            result.push_back(c);
        }
    }

    sort(result.begin(), result.end());

    return result;
}

int main() {
    Constituency c1("sofia", 1000, 3, new unsigned int[3]{100, 200, 600}, new string[3]{"OLK", "SMK", "DVP"});
    Constituency c2("pleven", 1000, 3, new unsigned int[3]{500, 50, 150}, new string[3]{"OLK", "SMK", "DVP"});
    Constituency c3("gabrovo",500, 3, new unsigned int[3]{40, 50, 320}, new string[3]{"OLK", "SMK", "DVP"});

    vector<Constituency> constituencies = {c1, c2, c3};

    write_to_files(constituencies);
    vector<Constituency> dvp = get_constituencies_by_winner(constituencies, "DVP");

    for (const auto& c : dvp) {
        cout << c.name << endl;
    }

    return 0;
}
