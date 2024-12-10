#include "game.h"



std::optional<uint64_t> ParseNumber(std::string& s, int start) {
    uint64_t res = 0;
    for (int i = start; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return {};
        }
        res = res * 10 + (s[i] - '0');
    }
    return res;
}


std::optional<std::pair<uint64_t, uint64_t>> ParsePairNumber(std::string& s, int start) {
    uint64_t res1 = 0;
    uint64_t res2 = 0;
    bool first = true;
    for (int i = start; i < s.size(); ++i) {
        if (s[i] == ' ' && first) {
            first = false;
            continue;
        } else if (s[i] < '0' || s[i] > '9') {
            if (first) {
                return {};
            }
            return {{res1, res2}};
        }
        if (first) {
            res1 = res1 * 10 + (s[i] - '0');
        } else {
            res2 = res2 * 10 + (s[i] - '0');
        }
    }
    return {{res1, res2}};
}

Game::Game(const std::string& path) {
    Load(path);
}

bool Game::SetWidthFromCommand(std::string& cmd) {
    std::optional<uint64_t> number = ParseNumber(cmd, 10);
    return number.has_value() && !is_started_ && field_.SetWidth(number.value());
}

bool Game::SetHeightFromCommand(std::string& cmd) {
    std::optional<uint64_t> number = ParseNumber(cmd, 11);
    return number.has_value() && !is_started_ && field_.SetHeight(number.value());
}

bool Game::CanStart() {
    return field_.GetHeight() > 0 && field_.GetHeight() > 0 && field_.GetShips().size() > 0;
}

void Game::ResetGame() {
    delete strategy_;
    strategy_ = new CustomStrategy;
    is_master_ = false;
    is_started_ = false;
    is_paused_ = false;
    is_ordered = false;
    field_.ResetField();
}


void Game::CreateMaster() {
    srand(time(nullptr));
    ResetGame();
    is_master_ = true;
    SetWidth(6 + rand() % 10);
    SetHeight(6 + rand() % 10);
    int64_t area = GetWidth() * GetHeight();
    uint64_t ships_counter[4]{0};
    ships_counter[1] = rand() % (area / 12);
    SetCount(2, ships_counter[1]);
    area -= 12 * ships_counter[1];
    if (area >= 15) {
        ships_counter[2] = rand() % (area / 15);
        SetCount(3, ships_counter[2]);
        area -= 15 * ships_counter[1];
    }
    if (area >= 18) {
        ships_counter[3] = rand() % (area / 18);
        SetCount(4, ships_counter[3]);
        area -= 18 * ships_counter[3];
    } 
    if (area >= 9) {
        ships_counter[0] = rand() % (area / 9);
        SetCount(1, ships_counter[0]);
        area -= 18 * ships_counter[0];
    }
}


std::string Game::Ping() {
    return "pong";
}

void Game::Stop() {
    is_paused_ = true;
}

bool Game::Start() {
    if (is_started_) {
        is_paused_ = false;
    } else if (!CanStart()) {
        return false;
    }
    is_started_ = true;
    return true;
}

bool Game::SetWidth(uint64_t width) {
    return !is_started_ && field_.SetWidth(width);
}

bool Game::SetHeight(uint64_t height) {
    return !is_started_ && field_.SetHeight(height);
}

bool Game::SetCount(uint8_t ship_size, uint64_t number_of_ships) {
    return !is_started_ && field_.SetCount(ship_size, number_of_ships);
}

uint64_t Game::GetCount(uint8_t ship_size) {
    return field_.GetCount(ship_size);
}

bool Game::IsWin() {
    return field_.GetEnemyShipsAlive() == 0;
}

bool Game::IsLose() {
    return field_.GetMyShipsAlive() == 0;
}

bool Game::IsFinished() {
    return IsLose() || IsWin();    
}

uint64_t Game::GetWidth() {
    return field_.GetWidth();
}

uint64_t Game::GetHeight() {
    return field_.GetHeight();
}


bool Game::SetResult(const std::string& result) {
    return field_.SetResult(result, !is_ordered);
}

bool Game::SetStrategy(const std::string& strategy_name) {
    if (strategy_name == "ordered") {
        delete strategy_;
        strategy_ = new OrderedStrategy;
    } else if (strategy_name == "custom") {
        delete strategy_;
        strategy_ = new CustomStrategy;
    } else {
        return false;
    }
    return true;
}


bool Game::Load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "failed. Unable to open file \"" << path << "\"\n";
        return false;
    }
    int width, height;
    file >> width >> height;
    field_.ResetShips();
    field_.SetWidth(width);
    field_.SetHeight(height);

    int ship_size, x, y;
    bool is_horizontal;
    std::string direction;
    while (!file.eof()) {
        file >> ship_size >> direction >> x >> y;
        is_horizontal = direction == "h" ? true : false;
        field_.AddShip(ship_size, is_horizontal, x, y);
    }
    return true;
}

bool Game::Dump(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cout << "failed. Unable to open file \"" << path << "\"\n";
        return false;
    }
    file << field_.GetWidth() << ' ' << field_.GetHeight() << '\n';
    for (const Ship& ship : field_.GetShips()) {
        file << ship << '\n';
    }
    
    return true;
}

bool Game::DrawEnemyField(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cout << "failed. Unable to open file \"" << path << "\"\n";
        return false;
    }
    std::unordered_map<Point, bool>& enemy_field = field_.GetEnemyField(); 
    for (uint64_t y = 0; y < field_.GetHeight(); ++y) {
        for (uint64_t x = 0; x < field_.GetWidth(); ++x) {
            Point a{x, y};
            if (enemy_field.find({x, y}) != enemy_field.end()) {
                file << (enemy_field[{x, y}] ? '1' : '0');
            } else {
                file << '.';
            }
        }
        file << '\n';
    }

    return true;
}



ShotResult Game::Shoot(const Point& target) {
    return field_.ShootMine(target);
}

Point Game::Shoot() {
    field_.GetLastShot() = strategy_->Shoot(field_);
    return field_.GetLastShot().value();
}

void Game::Play(std::istream& input, std::ostream& output) {
    std::optional<bool> is_okay;
    while(true) {
        std::string cmd;
        std::getline(input, cmd);
        is_okay.reset();

        if(cmd == "exit") {
            return;
        } else if(cmd == "ping") {
            output << Ping() << std::endl;
        } else if (cmd == "start") {
            is_okay = Start();
        } else if (cmd == "stop") {
            is_paused_ = true;
        } else if (cmd == "get width") {
            output << GetWidth() << '\n';
        } else if (cmd == "get height") {
            output << GetHeight() << '\n';
        } else if (cmd.find("set width ") == 0) {
            is_okay = SetWidthFromCommand(cmd);
        } else if (cmd.find("set height ") == 0) {
            is_okay = SetHeightFromCommand(cmd);
        } else if (cmd.find("set count ") == 0) {
            std::optional<std::pair<uint64_t, uint64_t>> pair = ParsePairNumber(cmd, 10);
            if (!pair.has_value()) {
                is_okay = false;
            } else {
                is_okay = SetCount(pair.value().first, pair.value().second);
            }
        } else if (cmd.find("get count ") == 0) {
            output << GetCount(ParseNumber(cmd, 10).value_or(5)) << '\n';
        } else if (cmd.find("load ", 0) == 0) {
            is_okay = Load(cmd.substr(5));
        } else if (cmd.find("dump ") == 0) {
            is_okay = Dump(cmd.substr(5));
        } else if (cmd.find("draw enemy field ") == 0) {
            is_okay = DrawEnemyField(cmd.substr(17));
        } else if (cmd == "win") {
            output << (IsWin() ? "yes" : "no") << '\n';
        } else if (cmd == "lose") {
            output << (IsLose() ? "yes" : "no") << '\n';
        } else if (cmd == "finished") {
            output << (IsFinished()  ? "yes" : "no") << '\n';
        } else if (cmd.find("create ") == 0) {
            is_okay = true;
            if (cmd == "create master") {
                CreateMaster();
            } else if (cmd == "create slave") {
                ResetGame();
            } else {
                is_okay = false;
            }
        } else if (cmd.find("set result ") == 0) {
            is_okay = SetResult(cmd.substr(11));
        } else if (cmd.find("set strategy ") == 0) {
            is_okay = SetStrategy(cmd.substr(13));
        } else if (is_paused_ || !is_started_) {
            is_okay = false;
        } else if (cmd.find("shot ") == 0) {
            std::optional<std::pair<uint64_t, uint64_t>> pair = ParsePairNumber(cmd, 5);
            if (!pair.has_value()) {
                is_okay = false;
            } else {
                output << Shoot(pair.value()) << '\n';
            }
        } else if (cmd == "shot") {
            output << Shoot() << '\n';
        }
        if (is_okay.has_value()) {
            output << (is_okay.value() ? "ok\n" : "failed\n");
        }
    }

    return;
}