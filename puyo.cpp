#include <iostream>
#include <curses.h>
#include <unistd.h>


#include "matrix.cpp"

// ゲームの進行速度を調整するためのスリープに用いる値（マイクロ秒）
#define GMCLOCK 60000
// #define GMCLOCK	15000

// ぷよの色を表すの列挙型
// NONEが無し，RED，BLUE，... が色を表す
enum puyocolor {
    NONE = 0,
    RED,
    BLUE,
    GREEN,
    YELLOW
};

// ぷよぷよの盤面を管理するクラス
// オブジェクト指向言語及び演習１で作成した matrix クラスを継承して作成します
// 講義資料の説明に沿って matrix クラスの機能拡張を済ませておいてください．
class PuyoArray : public matrix {
public:
    typedef matrix base;

    // デフォルトコンストラクタ（独自のメンバ変数を追加した場合は適宜修正すること）
    PuyoArray():base(12,6){
        // printf("PuyoArray default constructor\n");
    }
    // 盤面サイズを指定して初期化するコンストラクタ（独自のメンバ変数を追加した場合は適宜修正すること）
    PuyoArray(int rows, int cols) : base(rows, cols){
        // printf("PuyoArray constructor with size %dx%d\n", rows, cols);
    }
    // コピーコンストラクタ（独自のメンバ変数を追加した場合は適宜修正すること）
    PuyoArray(const PuyoArray &puyo) : base(puyo){
        // printf("PuyoArray copy constructor\n");
    }

public:
    // 代入演算子（独自のメンバ変数を追加した場合は適宜修正すること）
    void operator =(const PuyoArray &puyo) {
        base::operator =(puyo);
    }

    // 2つのぷよ配列で同じ位置にぷよが存在する場合は true を返し，
    // 同じ位置にぷよが一つも存在しない場合は false を返す
    bool operator &&(const PuyoArray &puyo) const {
        if (puyo.size() != size()) return false;
        bool flag = false;
        for (int i = 0; i < size(); i++) {
            if (puyo[i] != NONE && this->operator[](i) != NONE) flag = true;
        }
        return flag;
    }

    // 2つのぷよ配列をマージして新しいぷよ配列を生成
    const PuyoArray operator |(const PuyoArray &puyo) const {
        if (puyo.size() != size()) return *this;
        PuyoArray temp(*this);
        for (int i = 0; i < size(); i++) {
            if (puyo[i] != NONE) temp[i] = puyo[i];
        }
        return temp;
    }

    // 盤面上のぷよの総数を返す
    int count() const {
        const base &mat = *this;
        int num = 0;
        for (int i = 0; i < size(); i++) {
            if (mat[i] > 0) num++;
        }
        return num;
    }

public:
    // 盤面に新しいぷよ生成
    void GeneratePuyo() {
        PuyoArray &active = *this;
        // 初期コードはRGのぷよを生成するコードとなっています
        // ランダムなぷよを生成するように修正してください object_ex1
        // active(0, 2) = RED
        // active(0, 3) = GREEN
        int numbers[2];
        
        for(int i=0; i<2; i++){
            numbers[i] = rand() % 4 + 1; // 1から4のぷよ色をランダムに生成
        }

        for(int i=0; i<2; i++){
            switch (numbers[i]) {
                case 1: // RED
                    active(0, i + 2) = RED;
                    break;
                case 2: // BLUE
                    active(0, i + 2) = BLUE;
                    break;
                case 3: // GREEN
                    active(0, i + 2) = GREEN;
                    break;
                case 4: // YELLOW
                    active(0, i + 2) = YELLOW;
                    break;
                default:
                    active(0, i + 2) = NONE; // NONEの場合は何もしない
            }
        }
    }

    // ぷよの着地判定．着地したぷよの数を返す
    int MoveLandedPuyo(PuyoArray &stack) {
        PuyoArray &active = *this;  // アクティブ側のぷよ配列
        int num_landed = 0;         // 着地したぷよの数を数える変数

        // 1番下に到達したぷよを着地と判定する
        {
            int r = active.rows() - 1;
            for (int c = 0; c < active.cols(); c++) {
                if (active(r, c) != NONE) {
                    // 着地判定されたぷよを消す．本処理は必要に応じて変更する．
                    active(r, c) = NONE;
                    num_landed++;
                }
            }
        }

        return num_landed;
    }

    //左移動
    void MoveLeft(const PuyoArray &stack) {
        PuyoArray &active = *this;  // アクティブ側のぷよ配列
        PuyoArray temp(active);     // 一時的なぷよ格納配列

        // 1つ左の位置にぷよを移動させる
        for (int r = 0; r < temp.rows(); r++) {
            for (int c = 1; c < temp.cols(); c++) {
                if (temp(r, c) != NONE && temp(r, c - 1) == NONE) {
                    temp(r, c - 1) = temp(r, c);    // 一つ左にぷよをコピー
                    temp(r, c) = NONE;              // 元の位置のぷよは消す
                }
            }
        }

        // アクティブ側にコピー
        active = temp;
    }

    //右移動
    void MoveRight(const PuyoArray &stack) {
        PuyoArray &active = *this;  // アクティブ側のぷよ配列
        PuyoArray temp(active);     // 一時的なぷよ格納配列

        // 1つ右の位置にぷよを移動させる
        for (int r = 0; r < temp.rows(); r++) {
            for (int c = temp.cols() - 2; c >= 0; c--) {
                if (temp(r, c) != NONE && temp(r, c + 1) == NONE) {
                    temp(r, c + 1) = temp(r, c);    // 一つ右にぷよをコピー
                    temp(r, c) = NONE;              // 元の位置のぷよは消す
                }
            }
        }

        // アクティブ側にコピー
        active = temp;
    }

    // 下移動
    void MoveDown(const PuyoArray &stack) {
        PuyoArray &active = *this;  // アクティブ側のぷよ配列
        PuyoArray temp(active);     // 一時的なぷよ格納配列

        // 1つ下の位置にぷよを移動させる
        for (int r = temp.rows() - 2; r >= 0; r--) {
            for (int c = 0; c < temp.cols(); c++) {
                if (temp(r, c) != NONE && temp(r + 1, c) == NONE) {
                    temp(r + 1, c) = temp(r, c);    // 一つ下にぷよをコピー
                    temp(r, c) = NONE;              // 元の位置のぷよは消す

                }
            }
        }

        // アクティブ側にコピー
        active = temp;
    }
};

// 矩形を描画
void draw_rectangle(int r1, int c1, int r2, int c2, int color)
{
    attrset(color);
    mvhline(r1, c1, 0, c2 - c1);
    mvhline(r2, c1, 0, c2 - c1);
    mvvline(r1, c1, 0, r2 - r1);
    mvvline(r1, c2, 0, r2 - r1);
    mvaddch(r1, c1, ACS_ULCORNER);
    mvaddch(r2, c1, ACS_LLCORNER);
    mvaddch(r1, c2, ACS_URCORNER);
    mvaddch(r2, c2, ACS_LRCORNER);
}

// 画面表示
void display(const PuyoArray &puyo) {
    // 色を定義 object_ex1
    init_pair(0, COLOR_WHITE, COLOR_BLACK); //文字が白，背景が黒
    init_pair(1, COLOR_RED, COLOR_BLACK); //文字が赤，背景が黒
    init_pair(2, COLOR_BLUE, COLOR_BLACK); //文字が青，背景が黒
    init_pair(3, COLOR_GREEN, COLOR_BLACK); //文字が緑，背景が黒
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //文字が黄色，背景が黒

    // 枠を描画
    draw_rectangle(1, 0, puyo.rows() + 2, puyo.cols() * 2 + 2, COLOR_PAIR(0));

    // 落下中ぷよ表示
    for (int r = 0; r < puyo.rows(); r++) {
        for (int c = 0; c < puyo.cols(); c++) {
            switch (puyo(r, c)) {
            case NONE:
                attrset(COLOR_PAIR(0)); // 白色の文字属性を設定
                mvaddch(r + 2, c * 2 + 2, '.');
                break;
            case RED:
                attrset(COLOR_PAIR(1)); // 赤色の文字属性を設定
                mvaddch(r + 2, c * 2 + 2, 'R');
                break;
            case BLUE:
                attrset(COLOR_PAIR(2)); // 青色の文字属性を設定
                mvaddch(r + 2, c * 2 + 2, 'B');
                break;
            case GREEN:
                attrset(COLOR_PAIR(3)); // 緑色の文字属性を設定
                mvaddch(r + 2, c * 2 + 2, 'G');
                break;
            case YELLOW:
                attrset(COLOR_PAIR(4)); // 黄色の文字属性を設定
                mvaddch(r + 2, c * 2 + 2, 'Y');
                break;
            default:
                attrset(COLOR_PAIR(0)); // 白色の文字属性を設定
                mvaddch(r + 2, c * 2 + 2, '?');
                break;
            }
        }
    }

    // 情報表示
    int count = 0;
    for (int r = 0; r < puyo.rows(); r++) {
        for (int c = 0; c < puyo.cols(); c++) {
            if (puyo(r, c) != NONE) {
                count++;
            }
        }
    }

    // 画面左上にスコアを表示（最初はぷよの数を表示するダミーコード）
    char msg[256];
    snprintf(msg, 256, "Score: %d", count);
    mvaddstr(0, 1, msg);

    refresh();
}

// ここから実行される
int main(int argc, char **argv) {
    // 画面の初期化
    initscr();

    // カラー属性を扱うための初期化
    start_color();

    // キーを押しても画面に表示しない
    noecho();

    // キー入力を即座に受け付ける
    cbreak();
    curs_set(0);

    // キー入力受付方法指定
    keypad(stdscr, TRUE);

    // キー入力非ブロッキングモード
    timeout(0);

    // 乱数を初期化
    srand((unsigned)time(NULL));

    PuyoArray active(12, 6);                          // 縦12マス✕横6マスで初期化
    PuyoArray stack(active.rows(), active.cols());    // activeと同じサイズで初期化

    // 最初のぷよ生成
    active.GeneratePuyo();

    int delay = 0;
    int waitCount = 6;

    // メイン処理ループ
    bool loop = true;
    while (loop) {
        // キー入力受付
        int ch = getch();

        // 入力キーごとの処理
        switch (ch) {
        case KEY_LEFT:
            active.MoveLeft(stack);
            break;
        case KEY_RIGHT:
            active.MoveRight(stack);
            break;
        case 'q':  // q or Q の入力で終了する
        case 'Q':
            loop = false;
            continue;
        default:
            break;
        }

        // 処理速度調整のためのif文
        if (delay % waitCount == 0) {
            // ぷよ下に移動
            active.MoveDown(stack);

            // ぷよ着地判定
            if(active.MoveLandedPuyo(stack) > 0)
            {
                // 着地していたら新しいぷよ生成
                active.GeneratePuyo();
            }
        }
        delay++;

        // 表示
        display(active | stack);

        // ゲームの進行速度を調整するためのスリープ
        usleep(GMCLOCK);
    }

    // 画面をリセット
    endwin();

    return 0;
}
