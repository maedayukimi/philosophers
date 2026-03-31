今回のPhiloではスレッドとミューテックスを使ってシミュレーションを作る課題

流れとしてはa人の哲学者がいて　彼らは　思考->食事->睡眠　のループを可能な限り　もしくは指定された回数繰り返す
追加して　食事をb秒取らないと死ぬ　食事にc秒かかる　睡眠の時間はd秒...という感じ
思考はあくまで譲り合いの時間というか　食事するナイフがないときの待ち時間というイメージ

哲学者達は一つのテーブルを囲んでいる
哲学者はそれぞれナイフを1本ずつ持っており　食事する際は左にいる哲学者のナイフと自分のナイフの2つを使用する

目的としては...
スレッド　ミューテックスの理解
フォーク（共有リソース）を安全に使えるようにする
デッドロック　レースコンディションを防ぐ

スレッドって何？

プログラムの中で同時に動く小さな実行単位のこと
普通のプログラムはmainという一本の道に従って進むが　スレッドを使えば複数の道を同時に進められる
Pipex minishellでPidやForkを用いたが　あれは寄り道した感覚が強い

mutexとは？

共有リソースへの同時アクセスを防ぐロック機構
init lock unlock destroy の流れ...ここだけ見たら流れはシンプル
mutexが必要なケースは以下の通り

複数のスレッドが同じ変数や配列にアクセスするとき
同時に食べたらおかしいもの（= ナイフ）を扱うとき
ログ出力のように順番が乱れると見にくいもの

例えば今回のphilosopherでは食事の際にナイフを2本使う
その際にmutexを用いることでナイフを予約することができる　そうしなければ競合が発生しデッドロック状態になる
デッドロックは名前の通り詰みの状態　複数のスレッド　プロセスがリソースの解放を待ち続けて永久に進まないLOCKされた状態のこと


今回使う関数
memset, printf, malloc, free, write,
usleep, gettimeofday, pthread_create,
pthread_detach, pthread_join, pthread_mutex_init,
pthread_mutex_destroy, pthread_mutex_lock,
pthread_mutex_unlock

初めての関数
usleep gettimeofday pthread_create pthread_detach pthread_join pthread_mutex_init pthread_mutex_destroy pthread_mutex_lock pthread_mutex_unlock

usleep

int	usleep(useconds_t microseconds);

microsecondsはプログラムを停止させたい時間

int main(void)
{
	printf("start\n");
	usleep(500000); // 0.5秒停止（500,000マイクロ秒）
	printf("end\n");
	return 0;
}
このように使えばusleepの時点でプログラムを停止することができる
今回のphiloではご飯を食べる時間や寝る時間などと絡めて使うことになりそう

gettimeofday

int	gettimeofday(struct timeval *tv, struct timezone *tz);

struct	timeval
{
	time_t	tv_sec;   // 秒（1970年1月1日からの秒数）
	suseconds_t	tv_usec;  // マイクロ秒（1秒 = 1,000,000μ秒）
}
tvは現在の時刻が入る構造体のポインタ
tzはタイムゾーン情報だが　使われていないのでNULLでOK
つまり変数startに代入しておいて　終わりからstartを引けば経過時間を求めることができる

pthread_create

int	pthread_create(
	pthread_t		*thread,
	const pthread_attr_t	*attr,
	void			*(*start_routine)(void *),
	void			*arg
);

threadは作ったスレッドのIDが代入される　joinする際に必要
attrはスレッド属性　基本NULLらしい　要検証
*(*start_routine)(void *)は実行関数　以下に追記
argはstart_routineに引き渡す変数　構造体　intポインタなどを入れるらしい
void	*boil_noodles(void *arg)
{
	printf("🍜 ラーメン茹で開始！\n");
	sleep(3);
	printf("✅ ラーメン茹であがり！\n");
	return NULL;
}

void	*boil_eggs(void *arg)
{
	printf("🥚 卵茹で開始！\n");
	sleep(5);
	printf("✅ 卵茹であがり！\n");
	return NULL;
}

int	main(void)
{
	pthread_t	thread1,
	pthread_t	thread2;

	pthread_create(&thread1, NULL, boil_noodles, NULL);
	pthread_create(&thread2, NULL, boil_eggs, NULL);
//	ここで渡すID,　属性としてのNULL,　実行する関数,　引き渡す変数（今回は使用しないのでNULL）を明示している
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("🍽️ 料理が全部完成！\n");
	return 0;
}

pthread_detach

int	pthread_detach(pthread_t thread);

上に書いたコードにjoinがある　joinともうひとつ使われる待機系の関数がDetach
Detachするとそのスレッドの終了をまたずにMainを進めることができる
そのため通知やバックグラウンドのログ書き出しなどに使われるらしい
スレッドが一瞬の仕事しかしない？
結果がいらない？
スレッドがいつ終わるか気にしない？
他のスレッドと同期しない？
メイン処理の妨げになってほしくない？...全部当てはまるならOKらしい

void	*notify(void *arg)
{
	printf("📣 通知送信！\n");
	return NULL;
}

void	send_notification(void)
{
	pthread_t	tid;
	pthread_create(&tid, NULL, notify, NULL);
	pthread_detach(tid);  // もう終わるまで待たない
}

pthread_join

int	pthread_join(pthread_t thread, void **retval);

joinを用いることでスレッドの終了を待機することができる　waitpidのスレッド版
threadは待機するID
retvalは基本NULLでいいらしい

pthread_mutex_init

int	pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

mutexは初期化するmutex変数のポインタ
attrは属性　基本NULLでOK

pthread_mutex_lock

int	pthread_mutex_lock(pthread_mutex_t *mutex);

mutexをlockしてアクセスできない状態にする

pthread_mutex_unlock

int	pthread_mutex_unlock(pthread_mutex_t *mutex);

mutexをunlockしてアクセスできる状態にする

pthread_mutex_destroy

int	pthread_mutex_destroy(pthread_mutex_t *mutex);

mutexを破棄する　全てのスレッドが終了する　もしくはmainの最後に不要になったmutexを破棄しなければメモリリークする
そのためlockされていたり使用中のmutexを破棄すると未定義動作になるので注意
