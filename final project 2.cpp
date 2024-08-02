#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <limits>
#include <iterator>

using namespace std;

// じゃんけんの手を表す列挙型
enum Hand { ROCK, SCISSORS, PAPER };

// 難易度を表す列挙型
enum Difficulty { LEVEL1, LEVEL2, LEVEL3, LEVEL4, LEVEL5, LEVEL6 };

// じゃんけんゲームクラス
class RockPaperScissorsGame {
public:
    RockPaperScissorsGame() : playerScore(0), computerScore(0), difficulty(LEVEL3), timeLimit(3), useLongWords(false) {
        srand(static_cast<unsigned int>(time(0))); // 乱数の種を初期化
    }

    void run() {
        displayRules();
        getPlayerName();
        setDifficulty();

        while (true) {
            playRound();
            if (playerScore >= 3) {
                cout << playerName << "の勝利！" << endl;
                break;
            }
            else if (computerScore >= 3) {
                cout << "コンピュータの勝利！" << endl;
                break;
            }
        }
    }

private:
    int playerScore;
    int computerScore;
    Difficulty difficulty;
    int timeLimit;
    bool useLongWords;
    string playerName;

    // じゃんけんの手を表示する関数
    string handToString(Hand hand) {
        switch (hand) {
        case ROCK: return "グー";
        case SCISSORS: return "チョキ";
        case PAPER: return "パー";
        default: return "";
        }
    }

    // じゃんけんの結果を判定する関数
    string determineWinner(Hand player, Hand computer) {
        if (player == computer) return "あいこ";
        if ((player == ROCK && computer == SCISSORS) ||
            (player == SCISSORS && computer == PAPER) ||
            (player == PAPER && computer == ROCK)) {
            return "プレイヤーの勝ち";
        }
        else {
            return "コンピュータの勝ち";
        }
    }

    // コンピュータのじゃんけんの手をランダムに生成する関数
    Hand getRandomHand() {
        return static_cast<Hand>(rand() % 3);
    }

    // 入力する単語を生成する関数
    string generateRandomWord(bool longWords) {
        const vector<string> shortWords = { "apple", "banana", "cherry", "orange", "fig", "grape", "lemon" };
        const vector<string> longWordsList = { "pineapple", "blueberry", "watermelon", "strawberry", "chestnut", "persimmon" };

        // イテレータを使ってランダムな単語を選択
        if (longWords) {
            auto it = longWordsList.begin();
            advance(it, rand() % longWordsList.size());
            return *it;
        }
        else {
            auto it = shortWords.begin();
            advance(it, rand() % shortWords.size());
            return *it;
        }
    }

    // 指定された秒数待機する関数
    bool getInputWithinTime(string& input, const string& expectedInput, int seconds, double& elapsedTime) {
        auto start = chrono::steady_clock::now();
        auto end = start + chrono::seconds(seconds);

        while (chrono::steady_clock::now() < end) {
            if (cin >> input) {
                elapsedTime = chrono::duration<double>(chrono::steady_clock::now() - start).count();
                return input == expectedInput;
            }
        }

        elapsedTime = chrono::duration<double>(chrono::steady_clock::now() - start).count();
        return false;
    }

    // ルール説明する関数
    void displayRules() {
        cout << "ルール説明:\n";
        cout << "1. プレイヤーとコンピュータがじゃんけんを行います。\n";
        cout << "2. じゃんけんの結果に応じて、攻守が決定します。\n";
        cout << "3. プレイヤーがじゃんけんに勝った場合、指定された時間内に指定された単語を入力することで得点できます。\n";
        cout << "4. プレイヤーがじゃんけんに負けた場合、指定された時間内に指定された単語を入力しなければ、コンピュータに得点が入ります。\n";
        cout << "5. 先に3点を獲得した方が勝ちです。\n";
        cout << "6. 難易度に応じて、入力する単語と制限時間が変わります。\n";
        cout << "   - Level1: 5秒、短い単語\n";
        cout << "   - Level2: 3秒、短い単語\n";
        cout << "   - Level3: 2秒、短い単語\n";
        cout << "   - Level4: 5秒、長い単語\n";
        cout << "   - Level5: 3秒、長い単語\n";
        cout << "   - Level6: 2秒、長い単語\n";
        cout << "ゲームを開始します。\n\n";
    }

    void getPlayerName() {
        cout << "プレイヤーの名前を入力してください: ";
        cin >> playerName;
    }

    void setDifficulty() {
        while (true) {
            cout << "難易度を選択してください (1: Level1, 2: Level2, 3: Level3, 4: Level4, 5: Level5, 6: Level6): ";
            int difficultyChoice;
            if (cin >> difficultyChoice) {
                switch (difficultyChoice) {
                case 1:
                    difficulty = LEVEL1;
                    timeLimit = 5; // 入力時間: 5秒
                    useLongWords = false; // 短い単語グループ
                    break;
                case 2:
                    difficulty = LEVEL2;
                    timeLimit = 3; // 入力時間: 3秒
                    useLongWords = false; // 短い単語グループ
                    break;
                case 3:
                    difficulty = LEVEL3;
                    timeLimit = 2; // 入力時間: 2秒
                    useLongWords = false; // 短い単語グループ
                    break;
                case 4:
                    difficulty = LEVEL4;
                    timeLimit = 5; // 入力時間: 5秒
                    useLongWords = true; // 長い単語グループ
                    break;
                case 5:
                    difficulty = LEVEL5;
                    timeLimit = 3; // 入力時間: 3秒
                    useLongWords = true; // 長い単語グループ
                    break;
                case 6:
                    difficulty = LEVEL6;
                    timeLimit = 2; // 入力時間: 2秒
                    useLongWords = true; // 長い単語グループ
                    break;
                default:
                    cout << "無効な選択です。1〜6の番号を選んでください。" << endl;
                    continue; // 再入力を促す
                }
                break; // 有効な選択がされた場合はループを終了
            }
            else {
                cout << "無効な入力です。数字を入力してください。" << endl;
                cin.clear(); // 入力エラーのフラグをクリア
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // バッファのクリア
            }
        }
    }

    void playRound() {
        // プレイヤーの手を入力
        int playerChoice;
        Hand playerHand;

        while (true) {
            cout << "グー、チョキ、パーを選んでください (0: グー, 1: チョキ, 2: パー): ";
            if (cin >> playerChoice && playerChoice >= 0 && playerChoice <= 2) {
                playerHand = static_cast<Hand>(playerChoice);
                break;
            }
            else {
                cout << "無効な入力です。再度入力してください。" << endl;
                cin.clear(); // 入力エラーのフラグをクリア
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // バッファのクリア
            }
        }

        // コンピュータの手を生成
        Hand computerHand = getRandomHand();

        // 手を表示
        cout << playerName << "の手: " << handToString(playerHand) << endl;
        cout << "コンピュータの手: " << handToString(computerHand) << endl;

        // 勝敗を決定
        string result = determineWinner(playerHand, computerHand);
        cout << result << endl;

        if (result == "あいこ") {
            return; // あいこの場合はこのラウンドを終了
        }

        // 単語を生成
        string word = generateRandomWord(useLongWords);
        cout << "次の単語を入力してください: " << word << " (制限時間: " << timeLimit << "秒)" << endl;

        // 入力を取得
        string input;
        double elapsedTime = 0.0;
        bool success = getInputWithinTime(input, word, timeLimit, elapsedTime);

        if (!success || elapsedTime > timeLimit) {
            cout << "失敗！解答に要した時間: " << elapsedTime << "秒" << endl;
            if (result == "プレイヤーの勝ち") {
                cout << "攻撃失敗！" << endl;
            }
            else {
                cout << "防御失敗！" << endl;
                computerScore++;
            }
        }
        else {
            cout << "成功！解答に要した時間: " << elapsedTime << "秒" << endl;
            if (result == "プレイヤーの勝ち") {
                playerScore++;
                cout << "攻撃成功！" << endl;
            }
            else {
                cout << "防御成功！" << endl;
            }
        }

        cout << "現在のスコア - " << playerName << ": " << playerScore << " コンピュータ: " << computerScore << endl;
    }
};

int main() {
    RockPaperScissorsGame game;
    game.run();
    return 0;
}
