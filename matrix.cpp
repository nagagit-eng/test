#include <stdio.h>
// – vectorクラスのprint関数を仮想関数に変更
class vector
{
    public:
        vector( ):num(0),x(NULL)
        {
            x = new int[ num ];
            //printf( "constructor\n" );
        }

        vector( int n ):num(n)
        {
            x = new int[ num ];
            for( int i = 0; i < num; i++ )
            {
                x[ i ] = 0;
            }
            //printf( "constructor\n" );
        }

        vector( const vector& v ):num(v.num)
        {
            x = new int[ num ];
            for( int i = 0; i < num; i++ )
            {
                x[ i ] = v.x[ i ];
            }
            //printf( "copy constructor\n" );
        }

        ~vector( )
        {
            delete[] x;
            //printf( "destructor\n" );
        }

        virtual void print( )
        {
            if ( num == 0 )
            {
                //printf( "Empty\n" );
            }
            else
            {
                for( int i = 0; i < num; i++ )
                {
                    //printf( "%d \n", x[ i ] );
                }
            }
        }

        virtual void print() const
        {
            if ( num == 0 )
            {
                //printf( "Empty\n" );
            }
            else
            {
                for( int i = 0; i < num; i++ )
                {
                    //printf( "%d \n", x[ i ] );
                }
            }
        }

        int size( ) const // 要素数を返す software_ex1
        {
            return num;
        }

        int& operator []( int index )
        {
            return x[ index ];
        }

        const int& operator []( int index ) const
        {
            return x[ index ];
        }

        const vector operator +( const vector &v ) const
        {
            if ( num != v.num )
            {
                //printf( "Error: Vectors must be of the same size for addition.\n" );
                return vector(0);
            }
            vector result(num);
            for (int i = 0; i < num; i++)
            {
                result.x[i] = x[i] + v.x[i];
            }
            return result;
        }

        const vector operator -( const vector &v ) const
        {
            if ( num != v.num )
            {
                //printf( "Error: Vectors must be of the same size for subtraction.\n" );
                return vector(0);
            }
            vector result(num);
            for (int i = 0; i < num; i++)
            {
                result.x[i] = x[i] - v.x[i];
            }
            return result;
        }

        void operator =( const vector &v )
        {
            if ( this == &v ) return; // 自分自身への代入を防ぐ
            delete[] x; // 既存のメモリを解放
            num = v.num;
            x = new int[ num ];
            for( int i = 0; i < num; i++ )
            {
                x[ i ] = v.x[ i ];
            }
            //printf( "assignment operator\n" );
        }

        protected:
            int num;
            int* x;
};

// vectorクラスから派生したmatrixクラスを作成
// matrixクラスに行数rと列数cの２引数をとるコンストラクタを実装
// • vectorクラスのコンストラクタを呼び出して初期化
// r行c列の要素にアクセスするアクセス演算子をmatrixクラスに実装

// – vectorクラスのprint関数を仮想関数に変更
// – matrixクラスにprint関数を実装
// • 行列形式で標準出力に表示するように実装
// – vectorクラスの変数を受け取ってprintメンバ関数を呼び出すprint関数を実装
// – matrixクラスの変数をprint関数の引数に指定し，オーバーライドした関数側が呼ばれることを確認



// rowが行、colが列である
class matrix : public vector
{
    public:
        matrix( int r, int c ) :vector(r*c), num1(r), num2(c)
        {
            //printf( "matrix constructor\n" );
        }

        // コピーコンストラクタ
        matrix( const matrix &m ) :vector(m), num1(m.num1), num2(m.num2)
        {
            //printf( "matrix copy constructor\n" );
        }

        ~matrix( )
        {
            //printf( "matrix destructor\n" );
        }

        int& operator ()( int r, int c )
        {
            return x[ r + c*num1 ];
        }

        const int& operator()( int r, int c ) const
        {
            return x[ r + c*num1 ];
        }

        void print() const
        {
            if ( num1 == 0 || num2 == 0 )
            {
                //printf( "Empty matrix\n" );
                return;
            }
            //printf("[\n");
            for ( int r = 0; r < num1; r++ )
            {
                for ( int c = 0; c < num2; c++ )
                {
                    //printf( "%d ", x[ r + c * num1 ] );
                    if ( c < num2 - 1 )
                    {
                        //printf( ", " );
                    }
                }
                //printf( "\n" );
            }
            //printf("]\n");
        }

        void print( const vector &v ) const
        {
            v.print( );
        }

        // 行数を取得するgetter
        int rows() const {
            return num1;
        }

        // 列数を取得するgetter
        int cols() const {
            return num2;
        }

    private:
        int num1; // 行数
        int num2; // 列数
};

// int main()
// {
//     vector v;
//     v.x[ 0 ] = 0;
//     v.x[ 1 ] = 1;
//     v.x[ 2 ] = 2;
//     v.print( );
// }
// int main( )
// {
//     matrix m( 3, 3 );
//     m(0, 0) = m(1, 1) = m(2, 2) = 1;
//     m.print( );

//     matrix m2(2,2);
//     m2.print(m);

//     return 0;
// }