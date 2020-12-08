#include <iostream>
#include "game_state.hpp"
#include "game_searcher.hpp"
#include <algorithm>
#include "queue.hpp"
#include <array>
#include <sstream>
#include <iomanip>
#include <string>
#include <limits>
#include <random>
#include<map>

enum field {
    eEmpty=0,
    ePlayer1,
    ePlayer2
};
std::map<field, std::string> fieldString{
        {eEmpty," . "},
        {ePlayer1," X "},
        {ePlayer2," 0 "}
};

template<int Width, int Height, typename Move>
class connect4 : public game_state<Move>
{
public:

    connect4(bool is_player_turn)
    {
        this->is_player_turn = is_player_turn;
        for (int i = 0; i < Height; i++)
            for (int j = 0; j < Width; j++)
                board[i][j] = field::eEmpty;
    }

    connect4(bool is_player_turn, const std::array<std::array<uint8_t, Width>, Height>& board) :board(board)
    {
        this->is_player_turn = is_player_turn;
    }

    std::unique_ptr<game_state<Move>> clone()const
    {
        return std::unique_ptr<game_state<Move>>(new connect4(*this));
    }

    std::vector<Move> generate_moves() const override
    {
        std::vector<Move> moves;
        for (int i = 0; i < Width; i++)
        {
            if (board[0][i] == field::eEmpty)
                moves.push_back(i);
        }
        return moves;
    }

    bool getIsPlayerTurn()
    {
        return is_player_turn;
    }

    std::unique_ptr<game_state<Move>> make_move(const Move& move) const override
    {
        std::unique_ptr<game_state<Move>> next_state = clone();
        connect4& next_state_ref = dynamic_cast<connect4&>(*next_state);

        for (int i = 0; i < Height; i++)
        {
            if (board[i][move] == field::eEmpty)
            {
                if (i == Height - 1) {
                    if (is_player_one)
                        next_state_ref.board[i][move] = field::ePlayer1;
                    else
                        next_state_ref.board[i][move] = field::ePlayer2;
                    next_state_ref.is_player_turn = !next_state_ref.is_player_turn;
                    next_state_ref.is_player_one = !next_state_ref.is_player_one;
                    break;
                }
                continue;
            }
            else {
                if (is_player_one)
                    next_state_ref.board[i-1][move] = field::ePlayer1;
                else 
                    next_state_ref.board[i-1][move] = field::ePlayer2;
                next_state_ref.is_player_turn = !next_state_ref.is_player_turn;
                next_state_ref.is_player_one = !next_state_ref.is_player_one;
                break;
            }
        }
        return next_state;
    }


    std::string to_string() const override
    {
        std::string str;
        for (int i = 0; i < Height; i++)
        {
            for (int j = 0; j < Width; j++)
            {
                str += fieldString[board[i][j]];
            }
            str += "\n";
        }

        return str;
    }

    size_t hash_code()const
    {
        size_t h = 1;
        for (auto i : board)
            for (int j : i)
                h = h * 31 + j;
        return h;
    }

    double get_h() const override
    {
        bool remis = true;
        double score = 0;
        for (int i = 0; i < Height; i++) {
            for (int j = 0; j < Width; j++) {
                double cell_score = 0;
                double initial_value = 0;
                field thisField = board[i][j];
                if (thisField == field::eEmpty) {
                    remis = false;
                    continue;
                }
                else if(thisField==field::ePlayer1){
                    initial_value += 1;
                }
                else {
                    initial_value -= 1;
                }
                cell_score = initial_value;
                if (i < Height - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i + k][j]) {
                            if (board[i + k][j] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if(board[i + k][j] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (i > 2) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i - k][j]) {
                            if (board[i - k][j] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i - k][j] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (j < Width - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i][j + k]) {
                            if (board[i][j + k] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity()*(-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i][j + k] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (j >2) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i][j - k]) {
                            if (board[i][j - k] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i][j - k] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (j < Width - 3 && i < Height - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i + k][j + k]) {
                            if (board[i + k][j + k] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i + k][j + k] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (j > 2 && i >2) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i - k][j - k]) {
                            if (board[i - k][j - k] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i - k][j - k] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (j < Width - 3 && i >2) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i - k][j + k]) {
                            if (board[i - k][j + k] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i - k][j + k] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
                cell_score = initial_value;
                if (j > 2 && i<Height - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i + k][j - k]) {
                            if (board[i + k][j - k] == field::ePlayer1) {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity();
                                else
                                    cell_score += k * k * k;
                            }
                            else {
                                if (k == 3)
                                    cell_score = std::numeric_limits<double>::infinity() * (-1);
                                else
                                    cell_score -= k * k * k;
                            }
                        }
                        else {
                            if (board[i + k][j - k] != field::eEmpty)
                                cell_score = 0;
                            break;
                        }
                    }
                }
                else
                    cell_score = 0;
                score += cell_score;
            }
        }
        return score;
    }

    std::optional<double> is_terminal() const override {
        bool remis = true;
        for (int i = 0; i < Height; i++) {
            for (int j = 0; j < Width; j++) {
                field thisField = board[i][j];
                if (thisField ==field::eEmpty) {
                    remis = false;
                    continue;
                }
                if (i < Height - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i + k][j] && k == 3) {
                            if (thisField == field::ePlayer1)
                                return std::numeric_limits<double>::infinity();
                            else
                                return std::numeric_limits<double>::infinity() * (-1);
                        }
                        if (thisField == board[i+k][j])
                            continue;
                        else
                            break;
                    }
                }
                if (j < Width - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i][j+k] && k == 3) {
                            if (thisField == field::ePlayer1)
                                return std::numeric_limits<double>::infinity();
                            else
                                return std::numeric_limits<double>::infinity() * (-1);
                        }
                        if (thisField == board[i][j + k])
                            continue;
                        else
                            break;
                    }
                }
                if (j < Width - 3 && i < Height - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i+k][j + k] && k == 3) {
                            if (thisField == field::ePlayer1)
                                return std::numeric_limits<double>::infinity();
                            else
                                return std::numeric_limits<double>::infinity() * (-1);
                        }
                        if (thisField == board[i+k][j + k])
                            continue;
                        else
                            break;
                    }
                }
                if (j > 2 && i < Height - 3) {
                    for (int k = 1; k < 4; k++) {
                        if (thisField == board[i + k][j - k] && k == 3) {
                            if (thisField == field::ePlayer1)
                                return std::numeric_limits<double>::infinity();
                            else
                                return std::numeric_limits<double>::infinity() * (-1);
                        }
                        if (thisField == board[i + k][j - k])
                            continue;
                        else
                            break;
                    }
                }
            }
        }
        if (remis)
            return 0;
        return {};


    }

protected:

    std::array<std::array<field, Width>, Height> board;

    bool is_player_turn;

    bool is_player_one = true;

    bool is_equal(const game_state<Move>& s) const override
    {
        const connect4* st = dynamic_cast<const connect4*>(&s);
        return st != nullptr && st->board == this->board;
    }

};

int main()
{
    const int sWidth = 7, sHeight = 6;
    int StartingPlayer;
    bool isPlayerStarting;


    std::cout << "Starting player: 0-Computer, 1-You" << std::endl;
    std::cin >> StartingPlayer;
    if (StartingPlayer == 1)
        isPlayerStarting = true;
    else 
        isPlayerStarting = false;
    connect4<sWidth, sHeight, int> newboard(isPlayerStarting);

    alpha_beta_searcher<int> game_searcher(2, !isPlayerStarting);
    int move;
    while (true)
    {
        if (newboard.getIsPlayerTurn() == true)
        {
            std::cout << newboard.to_string() << std::endl;
            std::cout << "Your move:";
            std::cin >> move;
            std::vector<int> possible_moves = newboard.generate_moves();
            bool correctMove = false;
            while (!correctMove) {
                for (int i = 0; i < possible_moves.size();i++) {
                    if (possible_moves[i] == move) {
                        correctMove = true;
                        break;
                    }
                }
                if (!correctMove) {
                    std::cout << "Wrong move! Pick another one!" << std::endl;
                    std::cin >> move;
                }
            }

            newboard = dynamic_cast<connect4<7, 6, int>&>(*newboard.make_move(move));
            
            if (newboard.is_terminal().has_value())
            {
                std::cout << newboard.to_string();
                if (newboard.is_terminal().value() == 0)
                    std::cout << "Remis ;/" << std::endl;
                else {
                    std::cout << "You won!";
                }
                return 0;
            }
        }
        else
        {
            std::cout << newboard.to_string() << std::endl;
            game_searcher.do_search(newboard);
            auto aiScores = game_searcher.get_scores();
            std::cout << "Scores for every move:" << std::endl;
            for (int i = 0; i < aiScores.size(); i++)
            {
                std::cout << aiScores[i].first << "-" << aiScores[i].second << std::endl;
            }
            double maxi = aiScores[0].second, mini = aiScores[0].second;
            int maxiMove = aiScores[0].first, miniMove = aiScores[0].first;
            for (int i = 1; i < aiScores.size(); i++) {
                if (aiScores[i].second < mini) {
                    mini = aiScores[i].second;
                    miniMove = aiScores[i].first;
                }
                else if (aiScores[i].second > maxi) {
                    maxi = aiScores[i].second;
                    maxiMove = aiScores[i].first;
                }
            }
            //losowosc
            srand(time(NULL));

            std::vector<int> randomPossibleMoves;
            if (isPlayerStarting) {
                for (int i = 0; i < aiScores.size(); i++) {
                    int znak = 10;
                    if (mini < 0)
                        znak = -10;
                    if (mini * (100 + znak) / 100 >= aiScores[i].second)
                        randomPossibleMoves.push_back(aiScores[i].first);
                }
            }
            else{
                for (int i = 0; i < aiScores.size(); i++) {
                    int znak = -10;
                    if (maxi < 0)
                        znak = 10;
                    if (maxi * (100 + znak) / 100 <= aiScores[i].second)
                        randomPossibleMoves.push_back(aiScores[i].first);
                }
            }
            int randomIndex = rand() % randomPossibleMoves.size();

            if (!isPlayerStarting)
                newboard = dynamic_cast<connect4<7, 6, int>&>(*newboard.make_move(randomPossibleMoves[randomIndex]));
            else
                newboard = dynamic_cast<connect4<7, 6, int>&>(*newboard.make_move(randomPossibleMoves[randomIndex]));
            if (newboard.is_terminal().has_value())
            {
                std::cout << newboard.to_string();
                if (newboard.is_terminal().value() == 0)
                    std::cout << "Remis ;/" << std::endl;
                else {
                    std::cout << "You lost!";
                }
                return 0;
            }
        }
    }

}

