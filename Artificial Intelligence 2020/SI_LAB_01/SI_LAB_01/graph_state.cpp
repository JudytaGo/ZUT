#include "graph_state.hpp"
#include "graph_search.hpp"
#include "queue.hpp"
#include <cmath>
#include <array>
#include <iostream>
#include <sstream>

template<int M, int N>
class generic_sudoku : public graph_state
{
public:
    generic_sudoku(const std::string& s)
    {
        std::istringstream str{ s };
        for (auto& i : board)
            for (auto& j : i)
            {
                int v;
                str >> v;
                j = v;
            }
    }

    std::unique_ptr<graph_state> clone() const override
    {
        return std::unique_ptr<graph_state>{new generic_sudoku(*this)};
    }

    size_t hash_code() const override
    {
        size_t h = 1;
        for (const auto& i : board)
        {
            for (auto j : i)
            {
                h = 31 * h + j;
            }
        }
        return h;
    }

    std::vector<std::unique_ptr<graph_state>> get_successors() const override
    {
        std::vector<std::unique_ptr<graph_state>> r;
        for (int i = 0; i < M*N; i++)
            for (int j = 0; j<M*N; j++)
                if (board[i][j] == 0)
                {
                    std::vector<uint8_t> mozliwosci;
                    bool wystapily[10] = { 0 };

                    const int left = (j / N) * N, top = (i / M) * M;
                    for (int l = top; l <= top + M - 1; l++)
                    {
                        for (int m = left; m <= left + N - 1; m++)
                        {
                            wystapily[board[l][m]] = true;
                        }
                    }
                    for (int l = 0; l < M*N; l++)
                    {
                        wystapily[board[l][j]] = true;
                    }
                    for (int l = 0; l < M * N; l++)
                    {
                        wystapily[board[i][l]] = true;
                    }
                    for (uint8_t l = 1; l <= M * N; l++)
                    {
                        if (wystapily[l] == false)
                        {
                            mozliwosci.push_back(l);
                        }
                    }
                    //std::cout << i << "," << j << ":";
                    for (auto n : mozliwosci)
                    {
                        //std::cout << static_cast<int>(n) << " ";
                        auto state_copy = clone();
                        dynamic_cast<generic_sudoku&>(*state_copy).board[i][j] = n;
                        state_copy->update_score(get_g() + 1);
                        state_copy->set_parent(this);
                        r.push_back(std::move(state_copy));
                    }
                    //std::cout << std::endl;
                    return r;
                }
        return r;
    }

    bool is_solution() const override
    {
        for (int i = 0; i < M * N; i++)
        {
            for (int j = 0; j < M * N; j++)
            {
                if (board[i][j] == 0)
                    return false;
            }
        }
        return true;
    }

    std::string to_string() const override
    {
        std::string out;
        for (int i = 0; i < M * N; i++)
        {
            for (int j = 0; j < M * N; j++)
            {
                out += std::to_string(board[i][j]) + " ";
            }
            out += "\n";
        }
        return out;
    }

protected:
    bool is_equal(const graph_state& s) const override
    {
        const generic_sudoku* st = dynamic_cast<const
            generic_sudoku*>(&s);
        return st != nullptr && st->board == this->board;
    }
//private:
    std::array<std::array<uint8_t, M* N>, M* N> board;
};

template<int M, int N, typename Heuristic>
class sudoku_state : public generic_sudoku<M, N>
{
public:
    sudoku_state(const std::string &s) : generic_sudoku<M,N>(s)
    {
    }
    std::unique_ptr<graph_state> clone() const override
    {
        return std::unique_ptr<graph_state>{new sudoku_state{*this}};
    }
protected:
    double get_heuristic_grade() const override
    {
        return heuristic(this->board);
    }
private:
    static constexpr Heuristic heuristic {};
};

template<int M, int N>
struct H_remaining
{
    double operator()(const std::array<std::array<uint8_t, M*N>, M*N> &board) const
    {  //ma zwracac liczbe pustych, czyli zerowych pol w tablicy
        int cnt = 0;
        for (const auto& i : board)
            for (const auto& j : i)
                if (j == 0)
                    cnt++;
        return cnt;
    }   
};

graph_state::graph_state()
    : parent{ nullptr },
    fscore(std::nan("")),
    gscore(0),
    hscore(std::nan(""))
{
}

graph_state::~graph_state()
{
}

bool graph_state::operator== (const graph_state& s) const
{
    return is_equal(s);
}

double graph_state::get_f() const
{
    return fscore;
}

double graph_state::get_g() const
{
    return gscore;
}

double graph_state::get_h() const
{
    return hscore;
}

void graph_state::update_score(double new_g)
{
    gscore = new_g;
    hscore = get_heuristic_grade();
    fscore = gscore + hscore;
}

const graph_state* graph_state::get_parent() const
{
    return parent;
}

void graph_state::set_parent(const graph_state* parent)
{
    this->parent = parent;
}

double graph_state::get_heuristic_grade() const
{
    return 0;
}

namespace std
{
    ostream& operator << (ostream& out, const graph_state& s)
    {
        out << s.to_string();
        return out;
    }
}

template<int M, int N, typename Heuristic>
class minimal_sudoku : public sudoku_state<M, N,typename Heuristic>
{
public:
    minimal_sudoku(const std::string& s) : sudoku_state<M, N,Heuristic>(s)
    {
    }
    std::unique_ptr<graph_state> clone() const override
    {
        return std::unique_ptr<graph_state>{new minimal_sudoku{ *this }};
    }
    std::vector<std::unique_ptr<graph_state>> get_successors() const override
    {
        int min_mozliwosci = 9;
        int min_x = 0;
        int min_y = 0;
        std::vector<uint8_t> mozliwosciMin;
        std::vector<std::unique_ptr<graph_state>> r;
        for (int i = 0; i < M * N; i++)
            for (int j = 0; j < M * N; j++)
                if (this->board[i][j] == 0)
                {
                    std::vector<uint8_t> mozliwosci;
                    bool wystapily[10] = { 0 };

                    const int left = (j / N) * N, top = (i / M) * M;
                    for (int l = top; l <= top + M - 1; l++)
                    {
                        for (int m = left; m <= left + N - 1; m++)
                        {
                            wystapily[this->board[l][m]] = true;
                        }
                    }
                    for (int l = 0; l < M * N; l++)
                    {
                        wystapily[this->board[l][j]] = true;
                    }
                    for (int l = 0; l < M * N; l++)
                    {
                        wystapily[this->board[i][l]] = true;
                    }
                    for (uint8_t l = 1; l <= M * N; l++)
                    {
                        if (wystapily[l] == false)
                        {
                            mozliwosci.push_back(l);
                        }
                    }
                    if (mozliwosci.size() < min_mozliwosci) {
                        min_mozliwosci = mozliwosci.size();
                        min_x = i;
                        min_y = j;
                        mozliwosciMin = mozliwosci;
                    }
                }
        for (auto n : mozliwosciMin)
        {
            //std::cout << static_cast<int>(n) << " ";
            auto state_copy = clone();
            dynamic_cast<minimal_sudoku&>(*state_copy).board[min_x][min_y] = n;
            state_copy->update_score(this->get_g() + 1);
            state_copy->set_parent(this);
            r.push_back(std::move(state_copy));
        }
        return r;
    }

};



int main()
{
    //generic_sudoku<3, 3> s {
    sudoku_state<3,3, H_remaining<3,3>> s {
    //minimal_sudoku<3,3, H_remaining<3,3>> s {
        "5 3 0 0 7 0 0 0 0 \
        6 0 0 1 9 5 0 0 0 \
        0 9 8 0 0 0 0 6 0 \
        8 0 0 0 6 0 0 0 3 \
        4 0 0 8 0 3 0 0 2 \
        7 0 0 0 2 0 0 0 6 \
        0 6 0 0 0 0 2 8 0 \
        0 0 0 4 1 9 0 0 5 \
        0 0 0 0 8 0 0 7 9" };
        
    sudoku_state<2, 3, H_remaining<2, 3>> s6{
        "1 2 3 4 5 6 \
        0 0 0 0 0 0 \
        0 0 0 0 0 0 \
        0 0 0 0 0 0 \
        0 0 0 0 0 0 \
        0 0 0 0 0 0" };
    //std::cout << s << std::endl;
    auto comp = [](const graph_state &a, const graph_state &b)
    {
        return a.get_h() < b.get_h();
    };
    informative_searcher searcher(s, comp, std::numeric_limits<size_t>::max());
    std::cout << searcher.get_stats() << std::endl;
    /*for (int i = 0; i < searcher.get_number_of_solutions(); i++) {
        std::cout << searcher.get_solution(i)->to_string() << std::endl;
    }*/
    //s.get_successors();
    return 0;
}
